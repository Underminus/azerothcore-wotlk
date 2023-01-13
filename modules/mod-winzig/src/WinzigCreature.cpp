#include "WinzigCommon.h"
#include "WinzigCreature.h"

bool npc_winzig::OnGossipHello(Player* player, Creature* creature)
{
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, Winzig::GetItemIcon(37711, 30, 30, -18, 0) + "Vendor", VENDOR, 0);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, Winzig::GetItemIcon(42440, 30, 30, -18, 0) + "Customization", CUSTOMIZATION, 0);
    AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, Winzig::GetItemIcon(2251094, 30, 30, -18, 0) + "Reagent Bank", REAGENT_BANK, 0);
    SendGossipMenuFor(player, NPC_TEXT_ID, creature->GetGUID());
    return true;
}

bool npc_winzig::OnGossipSelect(Player* player, Creature* creature, uint32 item_subclass, uint32 gossipPageNumber)
{
    player->PlayerTalkClass->ClearMenus();

    if (item_subclass > MAX_PAGE_NUMBER)
    {
        // item_subclass is actually an item ID to withdraw. Get the actual item
        // subclass from the template.
        const ItemTemplate *temp = sObjectMgr->GetItemTemplate(item_subclass);
        Winzig::WithdrawItem(player, item_subclass);
        Winzig::ShowReagentItems(player, creature, temp->SubClass, gossipPageNumber);
        return true;
    }

    if (item_subclass == DEPOSIT_ALL_REAGENTS)
    {
        Winzig::DepositAllReagents(player);
        return true;
    }
    else if (item_subclass == MAIN_MENU)
    {
        OnGossipHello(player, creature);
        return true;
    }
    else if (item_subclass == VENDOR)
    {
        player->GetSession()->SendListInventory(creature->GetGUID(), NPC_VENDOR_ID);
        return true;
    }
    else if (item_subclass == REAGENT_BANK)
    {
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
        AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|tBack...", MAIN_MENU, 0);
        SendGossipMenuFor(player, NPC_TEXT_ID, creature->GetGUID());
        return true;
    }
    else if (item_subclass == CUSTOMIZATION)
    {
        AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, "|TInterface/Icons/Achievement_BG_returnXflags_def_WSG:30:30:-18:0|tChange My Appearance", CHANGE_APPEARANCE, 0, "Are you sure you want to change your appearance?", 50000, false);
        AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, "|TInterface/Icons/Ability_Paladin_BeaconofLight:30:30:-18:0|tChange My Race", CHANGE_RACE, 0, "Are you sure you want to change race?", 500000, false);
        AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, "|TInterface/Icons/INV_BannerPVP_01:30:30:-18:0|tChange My Faction", CHANGE_FACTION, 0, "Are you sure you want to change faction?", 5000000, false);
        AddGossipItemFor(player, GOSSIP_ICON_MONEY_BAG, "|TInterface/ICONS/Ability_Spy:30:30:-18:0|tBack...", MAIN_MENU, 0);
        SendGossipMenuFor(player, DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
        return true;
    }
    else if (item_subclass == CHANGE_APPEARANCE)
    {
        player->PlayerTalkClass->SendCloseGossip();

        // 5 gold
        int32 cost = 50000;
        if (player->ModifyMoney(-cost))
        {
            player->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
            ChatHandler(player->GetSession()).PSendSysMessage("Please log out for appearance change.");
        }

        return true;
    }
    else if (item_subclass == CHANGE_RACE)
    {
        player->PlayerTalkClass->SendCloseGossip();

        // 50 gold
        int32 cost = 500000;
        if (player->ModifyMoney(-cost))
        {
            player->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
            ChatHandler(player->GetSession()).PSendSysMessage("Please log out for race change.");
        }

        return true;
    }
    else if (item_subclass == CHANGE_FACTION)
    {
        player->PlayerTalkClass->SendCloseGossip();

        // 500 gold
        int32 cost = 5000000;
        if (player->ModifyMoney(-cost))
        {
            player->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
            ChatHandler(player->GetSession()).PSendSysMessage("Please log out for faction change.");
        }

        return true;
    }
    else
    {
        Winzig::ShowReagentItems(player, creature, item_subclass, gossipPageNumber);
        return true;
    }
}

// Add all scripts in one
void AddWinzigCreatureScripts()
{
    new npc_winzig();
}
