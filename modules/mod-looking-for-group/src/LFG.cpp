#include <climits>
#include <random>
#include "Common.h"
#include "Chat.h"
#include "ScriptMgr.h"
#include "LFGMgr.h"
#include "Player.h"
#include "DBUpdater.h"
#include "Group.h"
#include "ObjectAccessor.h"
#include "Spell.h"
#include "MapMgr.h"

class LFGPlayer : public PlayerScript
{
public:
    LFGPlayer() : PlayerScript("LFGPlayer") {}

    bool CanJoinLfg(Player* player, uint8 /*roles*/, lfg::LfgDungeonSet& dungeons, const std::string& /*comment*/) override
    {
        bool hardcore = player->HasAura(3758285);

        if (hardcore)
        {
            ChatHandler(player->GetSession()).SendSysMessage("Hardcore players cannot join random dungeons.");
            return false;
        }

        return true;
    }

    void OnQueueRandomDungeon(Player* player, uint32& rDungeonId) override
    {
        lfg::LfgDungeonSet const& dungeons = sLFGMgr->GetDungeonsByRandom(rDungeonId);

        if (dungeons.empty())
            return;

        uint32 minlevel = UINT_MAX;
        uint32 maxlevel = 0;

        for (lfg::LfgDungeonSet::const_iterator it = dungeons.begin(); it != dungeons.end(); it++)
        {
            lfg::LFGDungeonData const* dungeon = sLFGMgr->GetLFGDungeon(*it);
            uint8 level = player->getLevel();
            bool inRange = dungeon->minlevel <= level && level <= (dungeon->minlevel + 10);

            if (!inRange)
                continue;

            if (minlevel > dungeon->minlevel)
                minlevel = dungeon->minlevel;

            if (maxlevel < (dungeon->minlevel + 10))
                maxlevel = dungeon->minlevel + 10;
        }

        if (minlevel != UINT_MAX && maxlevel != 0)
            Announce(player, minlevel, maxlevel);
    }

    static void Announce(Player* player, uint32 minlevel, uint32 maxlevel)
    {
        uint32 count;
        Player* leader;

        getLeaderAndCount(player, &leader, &count);
        announceToSession(player->GetSession(), minlevel, maxlevel, count, leader->GetPlayerName());

        const SessionMap& sessions = sWorld->GetAllSessions();
        for (SessionMap::const_iterator itr = sessions.begin(); itr != sessions.end(); ++itr)
        {
            if (!itr->second || !itr->second->GetPlayer() || !itr->second->GetPlayer()->IsInWorld())
                continue;

            if (player == itr->second->GetPlayer())
                continue;

            announceToSession(itr->second, minlevel, maxlevel, count, leader->GetPlayerName());
        }
    }

    static lfg::LfgTeleportError CanTeleport(Player* player)
    {
        lfg::LfgTeleportError error = lfg::LFG_TELEPORTERROR_OK;

        if (!player->IsAlive())
        {
            error = lfg::LFG_TELEPORTERROR_PLAYER_DEAD;
        }
        else if (player->IsFalling() || player->HasUnitState(UNIT_STATE_JUMPING))
        {
            error = lfg::LFG_TELEPORTERROR_FALLING;
        }
        else if (player->IsMirrorTimerActive(FATIGUE_TIMER))
        {
            error = lfg::LFG_TELEPORTERROR_FATIGUE;
        }
        else if (player->GetVehicle() || player->HasUnitState(UNIT_STATE_IN_FLIGHT))
        {
            error = lfg::LFG_TELEPORTERROR_IN_VEHICLE;
        }
        else if (player->GetCharmGUID() || player->IsInCombat())
        {
            error = lfg::LFG_TELEPORTERROR_COMBAT;
        }

        return error;
    }

private:
    bool isRaidBrowser(Player* player)
    {
        return sLFGMgr->GetState(player->GetGUID()) == lfg::LFG_STATE_RAIDBROWSER;
    }

    bool isRandomDungeon(lfg::LfgDungeonSet& dungeons)
    {
        for (lfg::LfgDungeonSet::const_iterator it = dungeons.begin(); it != dungeons.end(); it++)
            if (sLFGMgr->GetDungeonType(*it) == lfg::LFG_TYPE_RANDOM)
                return true;

        return false;
    }

    bool isLFGGroup(Player* player)
    {
        Group* group = player->GetGroup();

        if (!group)
            return false;

        return group->isLFGGroup();
    }

    static void getLeaderAndCount(Player* player, Player** leader, uint32* count)
    {
        *leader = player;
        *count = 1;
        Group* group = player->GetGroup();

        if (group)
        {
            *count = group->GetMembersCount();

            for (GroupReference* itr = group->GetFirstMember(); itr != nullptr; itr = itr->next())
                if (Player* member = itr->GetSource())
                    if (group->GetLeaderGUID() == member->GetGUID())
                        *leader = member;
        }
    }

    static void announceToSession(WorldSession* session, uint32 minlevel, uint32 maxlevel, uint32 count, std::string name)
    {
        if (count == 1)
            ChatHandler(session).PSendSysMessage(
                "|cff00CC00[LFG Queue Announcer]:|r Random Dungeon -- [%u-%u][%u/5] Join %s now!",
                minlevel, maxlevel, count, name.c_str()
            );
        else if (count < 5)
            ChatHandler(session).PSendSysMessage(
                "|cff00CC00[LFG Queue Announcer]:|r Random Dungeon -- [%u-%u][%u/5] Ask %s to open it up!",
                minlevel, maxlevel, count, name.c_str()
            );
        else
            ChatHandler(session).PSendSysMessage(
                "|cff00CC00[LFG Queue Announcer]:|r Random Dungeon -- [%u-%u][%u/5]",
                minlevel, maxlevel, count, name.c_str()
            );
    }
};

class LFGGroup : public GroupScript
{
public:
    LFGGroup() : GroupScript("LFGGroup") {}

    void OnAddMember(Group* group, ObjectGuid guid)
    {
        Player* player = ObjectAccessor::FindConnectedPlayer(guid);

        if (!player || player->GetGUID() == group->GetLeaderGUID() || !group->isLFGGroup())
            return;

        uint32 dungeonId = sLFGMgr->GetDungeon(group->GetGUID(), true);
        lfg::LFGDungeonData const* dungeon = sLFGMgr->GetLFGDungeon(dungeonId);

        if (!dungeon)
            return;

        LFGPlayer::Announce(player, dungeon->minlevel, dungeon->minlevel + 10);
    }
};

constexpr uint32 SPELL_HEARTHSTONE = 8690;

class item_hearthstone : public ItemScript
{
public:
    item_hearthstone() : ItemScript("item_hearthstone") {}

    bool OnUse(Player* player, Item* /*hearthstone*/, SpellCastTargets const& /*targets*/) override
    {
        bool isDungeon = player->GetMap()->IsDungeon();
        bool isEntryPointValid = player->GetEntryPoint().GetMapId() != MAPID_INVALID;

        if (!isDungeon || !isEntryPointValid)
        {
            player->CastSpell(player, SPELL_HEARTHSTONE, false);
            return true;
        }

        if (LFGPlayer::CanTeleport(player) == lfg::LFG_TELEPORTERROR_OK)
        {
            player->TeleportToEntryPoint();
            player->ClearEntryPoint();
        }

        return true;
    }
};

class LFGDatabase : public DatabaseScript
{
public:
    LFGDatabase() : DatabaseScript("LFGDatabase") {}

    std::string path = "/modules/mod-looking-for-group/sql/";
    void OnAfterDatabasesLoaded(uint32 updateFlags) override
    {
        if (DBUpdater<LoginDatabaseConnection>::IsEnabled(updateFlags))
        {
            std::vector<std::string> directories;
            directories.push_back(path + "auth");
            DBUpdater<LoginDatabaseConnection>::Update(LoginDatabase, &directories);
        }

        if (DBUpdater<CharacterDatabaseConnection>::IsEnabled(updateFlags))
        {
            std::vector<std::string> directories;
            directories.push_back(path + "characters");
            DBUpdater<CharacterDatabaseConnection>::Update(CharacterDatabase, &directories);
        }

        if (DBUpdater<WorldDatabaseConnection>::IsEnabled(updateFlags))
        {
            std::vector<std::string> directories;
            directories.push_back(path + "world");
            DBUpdater<WorldDatabaseConnection>::Update(WorldDatabase, &directories);
        }
    }
};

void AddLFGScripts()
{
    new LFGPlayer();
    new LFGGroup();
    new item_hearthstone();
    new LFGDatabase();
}
