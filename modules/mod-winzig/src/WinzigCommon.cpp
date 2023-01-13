#include "WinzigCommon.h"

std::string Winzig::GetItemLink(uint32 entry, WorldSession* session)
{
    int loc_idx = session->GetSessionDbLocaleIndex();
    const ItemTemplate *temp = sObjectMgr->GetItemTemplate(entry);
    std::string name = temp->Name1;

    if (ItemLocale const* il = sObjectMgr->GetItemLocale(temp->ItemId))
        ObjectMgr::GetLocaleString(il->Name, loc_idx, name);

    std::ostringstream oss;
    oss << "|c" << std::hex << ItemQualityColors[temp->Quality] << std::dec <<
        "|Hitem:" << temp->ItemId << ":" << (uint32)0 << "|h[" << name <<
        "]|h|r";

    return oss.str();
}

std::string Winzig::GetItemIcon(uint32 entry, uint32 width, uint32 height, int x, int y)
{
    std::ostringstream ss;
    ss << "|TInterface";
    const ItemTemplate *temp = sObjectMgr->GetItemTemplate(entry);
    const ItemDisplayInfoEntry *dispInfo = NULL;

    if (temp)
    {
        dispInfo = sItemDisplayInfoStore.LookupEntry(temp->DisplayInfoID);
        if (dispInfo)
            ss << "/ICONS/" << dispInfo->inventoryIcon;
    }

    if (!dispInfo)
        ss << "/InventoryItems/WoWUnknownItem01";

    ss << ":" << width << ":" << height << ":" << x << ":" << y << "|t";

    return ss.str();
}

void Winzig::WithdrawItem(Player* player, uint32 entry)
{
    // This query can be changed to async to improve performance, but there will be some visual bugs because the query will not be done executing when the menu refreshes
    std::string query = "SELECT amount FROM reagent_bank WHERE account_id = " + std::to_string(player->GetSession()->GetAccountId()) + " AND item_entry = " + std::to_string(entry);
    QueryResult result = CharacterDatabase.Query("SELECT amount FROM reagent_bank WHERE account_id = " + std::to_string(player->GetSession()->GetAccountId()) + " AND item_entry = " + std::to_string(entry));

    if (result)
    {
        uint32 storedAmount = (*result)[0].Get<uint32>();
        const ItemTemplate *temp = sObjectMgr->GetItemTemplate(entry);
        uint32 stackSize = temp->GetMaxStackSize();

        if (storedAmount <= stackSize)
        {
            // Give the player all of the items and remove them from the DB
            ItemPosCountVec dest;
            InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, entry, storedAmount);

            if (msg == EQUIP_ERR_OK)
            {
                CharacterDatabase.Execute("DELETE FROM reagent_bank WHERE account_id = {} AND item_entry = {}", player->GetSession()->GetAccountId(), entry);
                Item* item = player->StoreNewItem(dest, entry, true);
                player->SendNewItem(item, storedAmount, true, false);
            }
            else
            {
                player->SendEquipError(msg, nullptr, nullptr, entry);
                return;
            }
        }
        else
        {
            // Give the player a single stack
            ItemPosCountVec dest;
            InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, entry, stackSize);

            if (msg == EQUIP_ERR_OK)
            {
                CharacterDatabase.Execute("UPDATE reagent_bank SET amount = {} WHERE account_id = {} AND item_entry = {}", storedAmount - stackSize, player->GetSession()->GetAccountId(), entry);
                Item* item = player->StoreNewItem(dest, entry, true);
                player->SendNewItem(item, stackSize, true, false);
            }
            else
            {
                player->SendEquipError(msg, nullptr, nullptr, entry);
                return;
            }
        }
    }
}

void Winzig::UpdateItemCount(std::map<uint32, uint32> &entryToAmountMap, std::map<uint32, uint32> &entryToSubclassMap, Item* pItem, Player* player, uint32 bagSlot, uint32 itemSlot)
{
    uint32 count = pItem->GetCount();
    ItemTemplate const *itemTemplate = pItem->GetTemplate();

    if ((itemTemplate->Class != ITEM_CLASS_TRADE_GOODS && itemTemplate->Class != ITEM_CLASS_GEM) || itemTemplate->GetMaxStackSize() == 1)
        return;

    uint32 itemEntry = itemTemplate->ItemId;
    uint32 itemSubclass = itemTemplate->SubClass;

    if (itemTemplate->Class == ITEM_CLASS_GEM)
        itemSubclass = ITEM_SUBCLASS_JEWELCRAFTING;

    if (!entryToAmountMap.count(itemEntry))
    {
        // Item does not exist yet in storage
        entryToAmountMap[itemEntry] = count;
        entryToSubclassMap[itemEntry] = itemSubclass;
    }
    else
    {
        entryToAmountMap[itemEntry] = entryToAmountMap.find(itemEntry)->second + count;
    }

    // The item counts have been updated, remove the original items from the player
    player->DestroyItem(bagSlot, itemSlot, true);
}

void Winzig::DepositAllReagents(Player* player) {
    WorldSession *session = player->GetSession();
    std::string query = "SELECT item_entry, item_subclass, amount FROM reagent_bank WHERE account_id = " + std::to_string(player->GetSession()->GetAccountId());

    session->GetQueryProcessor().AddCallback( CharacterDatabase.AsyncQuery(query).WithCallback([=](QueryResult result)
    {
        std::map<uint32, uint32> entryToAmountMap;
        std::map<uint32, uint32> entryToSubclassMap;

        if (result)
        {
            do {
                uint32 itemEntry = (*result)[0].Get<uint32>();
                uint32 itemSubclass = (*result)[1].Get<uint32>();
                uint32 itemAmount = (*result)[2].Get<uint32>();
                entryToAmountMap[itemEntry] = itemAmount;
                entryToSubclassMap[itemEntry] = itemSubclass;
            } while (result->NextRow());
        }

        // Inventory Items
        for (uint8 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; ++i)
        {
            if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
            {
                UpdateItemCount(entryToAmountMap, entryToSubclassMap, pItem, player, INVENTORY_SLOT_BAG_0, i);
            }
        }

        // Bag Items
        for (uint32 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; i++)
        {
            Bag* bag = player->GetBagByPos(i);

            if (!bag)
                continue;

            for (uint32 j = 0; j < bag->GetBagSize(); j++)
            {
                if (Item * pItem = player->GetItemByPos(i, j))
                {
                    UpdateItemCount(entryToAmountMap, entryToSubclassMap, pItem, player, i, j);
                }
            }
        }

        if (entryToAmountMap.size() != 0)
        {
            auto trans = CharacterDatabase.BeginTransaction();

            for (std::pair<uint32, uint32> mapEntry : entryToAmountMap)
            {
                uint32 itemEntry = mapEntry.first;
                uint32 itemAmount = mapEntry.second;
                uint32 itemSubclass = entryToSubclassMap.find(itemEntry)->second;
                trans->Append("REPLACE INTO reagent_bank (account_id, item_entry, item_subclass, amount) VALUES ({}, {}, {}, {})", player->GetSession()->GetAccountId(), itemEntry, itemSubclass, itemAmount);
            }

            CharacterDatabase.CommitTransaction(trans);
        }
    }));

    ChatHandler(player->GetSession()).PSendSysMessage("All reagents deposited successfully.");
    CloseGossipMenuFor(player);
}

void Winzig::ShowReagentItems(Player* player, Creature* creature, uint32 item_subclass, uint16 gossipPageNumber)
{
    WorldSession* session = player->GetSession();
    std::string query = "SELECT item_entry, amount FROM reagent_bank WHERE account_id = "
        + std::to_string(player->GetSession()->GetAccountId()) + " AND item_subclass = "
        + std::to_string(item_subclass) + " ORDER BY item_entry";

    session->GetQueryProcessor().AddCallback(CharacterDatabase.AsyncQuery(query).WithCallback([=](QueryResult result)
    {
        uint32 startValue = (gossipPageNumber * (MAX_OPTIONS));
        uint32 endValue = (gossipPageNumber + 1) * (MAX_OPTIONS) - 1;
        std::map<uint32, uint32> entryToAmountMap;
        std::vector<uint32> itemEntries;

        if (result)
        {
            do
            {
                uint32 itemEntry = (*result)[0].Get<uint32>();
                uint32 itemAmount = (*result)[1].Get<uint32>();
                entryToAmountMap[itemEntry] = itemAmount;
                itemEntries.push_back(itemEntry);
            } while (result->NextRow());
        }

        for (uint32 i = startValue; i <= endValue; i++)
        {
            if (itemEntries.empty() || i > itemEntries.size() - 1)
            {
                break;
            }
            uint32 itemEntry = itemEntries.at(i);
            AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetItemIcon(itemEntry, 30, 30, -18, 0) + GetItemLink(itemEntry, session) + " (" + std::to_string(entryToAmountMap.find(itemEntry)->second) + ")", itemEntry, gossipPageNumber);
        }

        if (gossipPageNumber > 0)
        {
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Previous Page", item_subclass, gossipPageNumber - 1);
        }

        if (endValue < entryToAmountMap.size())
        {
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Next Page", item_subclass, gossipPageNumber + 1);
        }

        AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|tBack...", MAIN_MENU, 0);
        SendGossipMenuFor(player, NPC_TEXT_ID, creature->GetGUID());
    }));
}

void Winzig::ShowReagentItems(Player* player, Item* item, uint32 item_subclass, uint16 gossipPageNumber)
{
    WorldSession* session = player->GetSession();
    std::string query = "SELECT item_entry, amount FROM reagent_bank WHERE account_id = "
        + std::to_string(player->GetSession()->GetAccountId()) + " AND item_subclass = "
        + std::to_string(item_subclass) + " ORDER BY item_entry";

    session->GetQueryProcessor().AddCallback(CharacterDatabase.AsyncQuery(query).WithCallback([=](QueryResult result)
    {
        uint32 startValue = (gossipPageNumber * (MAX_OPTIONS));
        uint32 endValue = (gossipPageNumber + 1) * (MAX_OPTIONS) - 1;
        std::map<uint32, uint32> entryToAmountMap;
        std::vector<uint32> itemEntries;

        if (result)
        {
            do
            {
                uint32 itemEntry = (*result)[0].Get<uint32>();
                uint32 itemAmount = (*result)[1].Get<uint32>();
                entryToAmountMap[itemEntry] = itemAmount;
                itemEntries.push_back(itemEntry);
            } while (result->NextRow());
        }

        for (uint32 i = startValue; i <= endValue; i++)
        {
            if (itemEntries.empty() || i > itemEntries.size() - 1)
            {
                break;
            }

            uint32 itemEntry = itemEntries.at(i);
            AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, GetItemIcon(itemEntry, 30, 30, -18, 0) + GetItemLink(itemEntry, session) + " (" + std::to_string(entryToAmountMap.find(itemEntry)->second) + ")", itemEntry, gossipPageNumber);
        }

        if (gossipPageNumber > 0)
        {
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Previous Page", item_subclass, gossipPageNumber - 1);
        }

        if (endValue < entryToAmountMap.size())
        {
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Next Page", item_subclass, gossipPageNumber + 1);
        }

        AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|tBack...", MAIN_MENU, 0);
        SendGossipMenuFor(player, NPC_TEXT_ID, item->GetGUID());
    }));
}
