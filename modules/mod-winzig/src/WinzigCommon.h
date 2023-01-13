#ifndef _WINZIG_COMMON_H_
#define _WINZIG_COMMON_H_

#include <string>
#include <map>
#include "Chat.h"
#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "Item.h"
#include "ItemTemplate.h"

#define MAX_OPTIONS         23
#define MAX_PAGE_NUMBER     700 // Values higher than this are considered Item IDs
#define NPC_TEXT_ID         4259 // Pre-existing NPC text
#define NPC_VENDOR_ID       5126979

enum GossipItemType : uint8
{
    DEPOSIT_ALL_REAGENTS = 16,
    MAIN_MENU = 17,
    VENDOR = 18,
    REAGENT_BANK = 19,
    CUSTOMIZATION = 20,
    CHANGE_APPEARANCE = 21,
    CHANGE_RACE = 22,
    CHANGE_FACTION = 23
};

namespace Winzig
{
    std::string GetItemLink(uint32 entry, WorldSession* session);
    std::string GetItemIcon(uint32 entry, uint32 width, uint32 height, int x, int y);
    void ShowReagentItems(Player* player, Creature* creature, uint32 item_subclass, uint16 gossipPageNumber);
    void ShowReagentItems(Player* player, Item* item, uint32 item_subclass, uint16 gossipPageNumber);
    void WithdrawItem(Player* player, uint32 entry);
    void UpdateItemCount(std::map<uint32, uint32>& entryToAmountMap, std::map<uint32, uint32>& entryToSubclassMap, Item* pItem, Player* player, uint32 bagSlot, uint32 itemSlot);
    void DepositAllReagents(Player* player);
}

#endif // _WINZIG_COMMON_H_
