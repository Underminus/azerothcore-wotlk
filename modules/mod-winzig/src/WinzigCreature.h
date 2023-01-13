#ifndef _WINZIG_CREATURE_H_
#define _WINZIG_CREATURE_H_

#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"

class npc_winzig : public CreatureScript
{
public:
    npc_winzig() : CreatureScript("npc_winzig") { }

    bool OnGossipHello(Player* player, Creature* creature) override;
    bool OnGossipSelect(Player* player, Creature* creature, uint32 item_subclass, uint32 gossipPageNumber) override;
};

#endif // _WINZIG_CREATURE_H_
