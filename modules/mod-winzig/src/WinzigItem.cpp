#include <algorithm>
#include "Chat.h"
#include "Common.h"
#include "DatabaseEnv.h"
#include "Item.h"
#include "Player.h"
#include "WinzigCommon.h"
#include "WinzigItem.h"
#include "WinzigWorld.h"
#include "WinzigCreature.h"
#include "ScriptedGossip.h"
#include "Spell.h"

const std::string SUBJECT = "Loot Box reward";
const std::string BODY = "Congratulations, you got a reward but your inventory was full."
    "Please take your reward when you free space from your inventory.";

bool isBannerItem(float result)
{
    return result <= 0.5f;
}

float item_loot_box::roll()
{
    return dis(gen);
}

uint32 item_loot_box::randomId(std::vector<uint32> ids)
{
    std::uniform_int_distribution<> dis(0, ids.size() - 1);
    int i = dis(gen);
    return ids[i];
}

bool item_loot_box::sendRewardToPlayer(Player *player, uint32 itemId, enum Rarity rarity, enum Banner banner)
{
    ItemTemplate const *proto = sObjectMgr->GetItemTemplate(itemId);

    if (!proto)
    {
        LOG_ERROR("server", "[Loot Box] Item ID is invalid: %u", itemId);
        return false;
    }

    CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();

    uint32 count = proto->Stackable >= WinzigWorld::StackSize ? uint32(WinzigWorld::StackSize) : uint32(proto->Stackable);
    Item *item = Item::CreateItem(itemId, count);
    ItemPosCountVec dest;

    InventoryResult msg = player->CanStoreItem(NULL_BAG, NULL_SLOT, dest, item, false);
    if (msg == EQUIP_ERR_OK)
    {
        player->StoreItem(dest, item, true);
        CharacterDatabase.CommitTransaction(trans);
        return true;
    }

    player->SendEquipError(msg, item);
    ChatHandler(player->GetSession()).PSendSysMessage("You will receive your reward in the mail.");
    MailSender sender(WinzigWorld::NPC);
    MailDraft draft(SUBJECT, BODY);

    draft.AddItem(item);
    draft.SendMailTo(trans, MailReceiver(player), sender);
    CharacterDatabase.CommitTransaction(trans);
    return true;
}

void item_loot_box::openLootBox(Player *player, Item *item, enum Box box, enum Rarity rarity)
{
    if (rarity != RARITY_THREE_STAR && rarity != RARITY_FOUR_STAR && rarity != RARITY_FIVE_STAR)
    {
        ChatHandler(player->GetSession()).PSendSysMessage("Failed to open loot box.");
        LOG_ERROR("server", "[Loot Box] Invalid rarity: {}", rarity);
        return;
    }

    enum Banner banner;

    switch (rarity)
    {
        case RARITY_FIVE_STAR:
            banner = isBannerItem(roll()) && box != BOX_STARTER ? BANNER_PROMOTIONAL : BANNER_NONE;
            break;
        case RARITY_FOUR_STAR:
            banner = isBannerItem(roll()) && box != BOX_STARTER ? BANNER_FEATURED : BANNER_NONE;
            break;
        default:
            banner = BANNER_NONE;
            break;
    }

    auto now = std::chrono::system_clock::now();
    auto months = std::chrono::duration_cast<Months>(now.time_since_epoch()).count();
    uint8 season = (months % 9) + 1;

    QueryResult rows = WorldDatabase.Query(
        "SELECT `item` FROM `item_loot_box` "
        "WHERE ((`season` = {} AND {}) OR (`season` <> {} AND NOT {} AND `exclusive` IS NULL)) "
        "AND `rarity` <= {} AND `box` = {} "
        "ORDER BY `rarity` DESC, FIELD(`season`, {}) DESC, RAND() "
        "LIMIT 1",
        season, banner, season, banner, rarity, box, season
    );

    if (!rows)
    {
        ChatHandler(player->GetSession()).PSendSysMessage("Failed to open loot box.");
        LOG_ERROR("server", "[Loot Box] No rewards configured.");
        return;
    }

    if (rarity == RARITY_FOUR_STAR)
        player->CastSpell(player, 7931300);
    else if (rarity == RARITY_FIVE_STAR)
        player->CastSpell(player, 7931301);

    uint32 itemId = rows->Fetch()->Get<uint32>();
    bool sent = sendRewardToPlayer(player, itemId, rarity, banner);

    if (!sent)
    {
        ChatHandler(player->GetSession()).PSendSysMessage("Failed to open loot box.");
        return;
    }

    uint32 one = 1;
    uint32 &count = one;
    player->DestroyItemCount(item->GetEntry(), count, true);
}

bool item_loot_box::OnUse(Player *player, Item *item, SpellCastTargets const &/*targets*/)
{
    if (!WinzigWorld::Enabled)
        return false;

    enum Box box;
    uint32 entry = item->GetEntry();
    if (entry == WinzigWorld::StarterBox)
    {
        box = BOX_STARTER;
    }
    else if (entry == WinzigWorld::ClassicBox)
    {
        box = BOX_CLASSIC;
    }
    else if (entry == WinzigWorld::BurntBox)
    {

        box = BOX_BURNT;
    }
    else if (entry == WinzigWorld::FrozenBox)
    {
        box = BOX_FROZEN;
    }
    else
    {
            return false;
    }

    float result = roll();
    if (result <= WinzigWorld::FiveStarRate)
    {
        openLootBox(player, item, box, RARITY_FIVE_STAR);
        return true;
    }
    else if (result <= WinzigWorld::FourStarRate)
    {
        openLootBox(player, item, box, RARITY_FOUR_STAR);
        return true;
    }
    else
    {
        openLootBox(player, item, box, RARITY_THREE_STAR);
        return true;
    }
}

bool item_reagent_pouch::OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/)
{
    if (item->GetEntry() != (uint32)WinzigWorld::ReagentPouch)
        return false;

    ClearGossipMenuFor(player);

    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, Winzig::GetItemIcon(4359, 30, 30, -18, 0) + "Parts", ITEM_SUBCLASS_PARTS, 0);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, Winzig::GetItemIcon(4358, 30, 30, -18, 0) + "Explosives", ITEM_SUBCLASS_EXPLOSIVES, 0);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, Winzig::GetItemIcon(4388, 30, 30, -18, 0) + "Devices", ITEM_SUBCLASS_DEVICES, 0);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, Winzig::GetItemIcon(1206, 30, 30, -18, 0) + "Jewelcrafting", ITEM_SUBCLASS_JEWELCRAFTING, 0);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, Winzig::GetItemIcon(2589, 30, 30, -18, 0) + "Cloth", ITEM_SUBCLASS_CLOTH, 0);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, Winzig::GetItemIcon(2318, 30, 30, -18, 0) + "Leather", ITEM_SUBCLASS_LEATHER, 0);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, Winzig::GetItemIcon(2772, 30, 30, -18, 0) + "Metal & Stone", ITEM_SUBCLASS_METAL_STONE, 0);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, Winzig::GetItemIcon(12208, 30, 30, -18, 0) + "Meat", ITEM_SUBCLASS_MEAT, 0);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, Winzig::GetItemIcon(2453, 30, 30, -18, 0) + "Herb", ITEM_SUBCLASS_HERB, 0);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, Winzig::GetItemIcon(7068, 30, 30, -18, 0) + "Elemental", ITEM_SUBCLASS_ELEMENTAL, 0);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, Winzig::GetItemIcon(10940, 30, 30, -18, 0) + "Enchanting", ITEM_SUBCLASS_ENCHANTING, 0);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, Winzig::GetItemIcon(23572, 30, 30, -18, 0) + "Nether Material", ITEM_SUBCLASS_MATERIAL, 0);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, Winzig::GetItemIcon(2604, 30, 30, -18, 0) + "Other Trade Goods", ITEM_SUBCLASS_TRADE_GOODS_OTHER, 0);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, Winzig::GetItemIcon(38682, 30, 30, -18, 0) + "Armor Vellum", ITEM_SUBCLASS_ARMOR_ENCHANTMENT, 0);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, Winzig::GetItemIcon(39349, 30, 30, -18, 0) + "Weapon Vellum", ITEM_SUBCLASS_WEAPON_ENCHANTMENT, 0);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, Winzig::GetItemIcon(4500, 30, 30, -18, 0) + "Deposit All Reagents", DEPOSIT_ALL_REAGENTS, 0);

    SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, item->GetGUID());

    return false;
}

void item_reagent_pouch::OnGossipSelect(Player* player, Item* item, uint32 item_subclass, uint32 action)
{
    player->PlayerTalkClass->ClearMenus();

    if (item_subclass > MAX_PAGE_NUMBER)
    {
        // Item subclass is actually an item ID to withdraw. Get the actual item
        // subclass from the template.
        const ItemTemplate *temp = sObjectMgr->GetItemTemplate(item_subclass);
        Winzig::WithdrawItem(player, item_subclass);
        Winzig::ShowReagentItems(player, item, temp->SubClass, action);
        return;
    }

    if (item_subclass == DEPOSIT_ALL_REAGENTS)
    {
        Winzig::DepositAllReagents(player);
        return;
    }
    else if (item_subclass == MAIN_MENU)
    {
        OnUse(player, item, SpellCastTargets());
        return;
    }
    else
    {
        Winzig::ShowReagentItems(player, item, item_subclass, action);
        return;
    }
}

void AddWinzigItemScripts()
{
    new item_loot_box();
    new item_reagent_pouch();
}
