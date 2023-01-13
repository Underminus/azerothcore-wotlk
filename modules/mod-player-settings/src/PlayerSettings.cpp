#include <algorithm>
#include <cstdlib>
#include <vector>
#include "Chat.h"
#include "Config.h"
#include "Language.h"
#include "MapMgr.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "ScriptMgrMacros.h"
#include "Unit.h"
#include "Formulas.h"
#include "Group.h"
#include "DBUpdater.h"
#include "ItemTemplate.h"
#include "Spell.h"
#include "Pet.h"
#include "SpellInfo.h"

// DPS count as 1 offensive unit. Tanks and healers count as 1 defensive unit.
// 5 man: 1 tank, 3 dps, 1 healer = 3 offensive units and 2 defensive units.
const float Offense5M = 1 / 3.0f, Defense5M = 1 / 2.0f;

std::array<uint32, 13> WorldBosses =
{
    13020, // Vaelastrasz
    14890, // Taerar
    14889, // Emeriss
    14888, // Lethon
    14887, // Ysondre
    6109,  // Azuregos
    12397, // Kazzak
    15571, // Maws
    15491, // Eranikus
    15625, // Twilight Corrupter
    12803, // Lord Kamaeran
    15552, // Dr. Weavil
    15629  // Nightmare Phantasm
};

class PlayerSettingsMapInfo : public DataMap::Base
{
public:
    PlayerSettingsMapInfo() {}
    uint32 nplayers = 0;
    uint32 veto = 0;
};

static bool enabled;

class PlayerSettingsWorldScript : public WorldScript
{
public:
    PlayerSettingsWorldScript() : WorldScript("PlayerSettingsWorldScript") {}

    void OnBeforeConfigLoad(bool /*reload*/) override
    {
        enabled = sConfigMgr->GetOption<bool>("PlayerSettings.Enable", 1);
    }
};

class PlayerSettingsPlayerScript : public PlayerScript
{
public:
    PlayerSettingsPlayerScript() : PlayerScript("PlayerSettingsPlayer") {}

    void OnUpdateArea(Player* player, uint32 oldArea, uint32 newArea) override
    {
        if (!enabled)
            return;

        if (player->IsGameMaster())
            return;

        Map *map = player->GetMap();
        PlayerSettingsMapInfo *mapInfo = map->CustomData.GetDefault<PlayerSettingsMapInfo>("PlayerSettingsMapInfo");
        Map::PlayerList const &players = map->GetPlayers();
        mapInfo->nplayers = map->GetPlayersCountExceptGMs();

        if (!mapInfo->nplayers)
            mapInfo->nplayers = 1;

        if (!mapInfo->veto)
            mapInfo->veto = mapInfo->nplayers;
    }

    void OnGiveXP(Player *player, uint32 &amount, Unit *victim) override
    {
        if (victim)
        {
            Map *map = player->GetMap();

            if (map->IsDungeon() && !map->IsBattlegroundOrArena())
            {
                PlayerSettingsMapInfo *mapInfo = map->CustomData.GetDefault<PlayerSettingsMapInfo>("PlayerSettingsMapInfo");
                uint32 maxPlayers = ((InstanceMap *)sMapMgr->FindMap(map->GetId(), map->GetInstanceId()))->GetMaxPlayers();

                amount = amount * mapInfo->nplayers / maxPlayers;
            }
        }
    }

    void OnMoneyChanged(Player *player, int32 &amount) override
    {
        Map *map = player->GetMap();

        if (map->IsDungeon() && amount > 0)
        {
            uint32 maxPlayers = ((InstanceMap *)sMapMgr->FindMap(map->GetId(), map->GetInstanceId()))->GetMaxPlayers();
            PlayerSettingsMapInfo *mapInfo = map->CustomData.GetDefault<PlayerSettingsMapInfo>("PlayerSettingsMapInfo");

            amount = amount * mapInfo->nplayers / maxPlayers;
        }
    }
};

bool isWorldBoss(Unit *creature)
{
    return std::find(WorldBosses.begin(), WorldBosses.end(), creature->GetEntry()) != WorldBosses.end();
}

class PlayerSettingsUnitScript : public UnitScript
{
public:
    PlayerSettingsUnitScript() : UnitScript("PlayerSettingsUnitScript", true) {}

    void ModifyPeriodicDamageAurasTick(Unit* target, Unit* attacker, uint32& damage, SpellInfo const* /*spellInfo*/) override
    {
        if (check(target, attacker))
            damage = modify(target, attacker, damage);
    }

    void ModifyMeleeDamage(Unit* target, Unit* attacker, uint32& damage) override
    {
        if (check(target, attacker))
            damage = modify(target, attacker, damage);
    }

    void ModifySpellDamageTaken(Unit* target, Unit* attacker, int32& damage, SpellInfo const* /*spellInfo*/) override
    {
        if (check(target, attacker))
            damage = modify(target, attacker, damage);
    }

    void ModifyPeriodicHealAurasTick(Unit* target, Unit* healer, uint32& heal, SpellInfo const* /*spellInfo*/) override
    {
        if (check(target, healer))
            heal = modify(target, healer, heal, false, true);
    }

    void ModifyHealReceived(Unit* target, Unit* healer, uint32& heal, SpellInfo const* /*spellInfo*/) override
    {
        if (check(target, healer))
            heal = modify(target, healer, heal, false);
    }

private:
    bool inDungeon(Unit *target, Unit *attacker)
    {
        return target->GetMap()->IsDungeon() && attacker->GetMap()->IsDungeon();
    }

    bool inBattleground(Unit *target, Unit *attacker)
    {
        return target->GetMap()->IsBattleground() && attacker->GetMap()->IsBattleground();
    }

    bool check(Unit* target, Unit* attacker)
    {
        if (!target || !target->GetMap())
            return false;

        if (!attacker || !attacker->GetMap())
            return false;

        if (isWorldBoss(target) || isWorldBoss(attacker))
            return true;

        if (!inDungeon(target, attacker) && !inBattleground(target, attacker))
            return false;

        return true;
    }

    bool isPlayerPet(Unit* unit)
    {
        return (unit->IsHunterPet() || unit->IsPet() || unit->IsSummon() || unit->IsTotem()) && unit->IsControlledByPlayer();
    }

    uint32 modify(Unit* target, Unit* attacker, uint32 amount, bool isDamage = true, bool isPeriodicHeal = false)
    {
        PlayerSettingsMapInfo *mapInfo = target->GetMap()->CustomData.GetDefault<PlayerSettingsMapInfo>("PlayerSettingsMapInfo");
        InstanceMap *instanceMap = ((InstanceMap *)sMapMgr->FindMap(target->GetMapId(), target->GetInstanceId()));

        uint32 nplayers = std::max(mapInfo->nplayers, mapInfo->veto);
        uint32 maxPlayers = instanceMap->GetMaxPlayers();

        if (maxPlayers == 0 && instanceMap->GetEntry()->IsWorldMap())
            maxPlayers = MAXRAIDSIZE;

        float defense = Defense5M;

        if (maxPlayers > 5)
            defense = 1 / (2 + (maxPlayers / 5.0f));

        float multiplier = 1.0f;
        bool isAttackerPlayer = attacker->GetTypeId() == TYPEID_PLAYER;
        bool isAttackerPet = isPlayerPet(attacker);
        bool isTargetPlayer = target->GetTypeId() == TYPEID_PLAYER;
        bool isTargetPet = isPlayerPet(target);
        bool isSelfHarm = (isAttackerPlayer && isTargetPlayer) && attacker->GetGUID() == target->GetGUID() && isDamage;
        bool isCharmedPlayer = isAttackerPlayer && attacker->GetCharmerGUID();

        if ((!isAttackerPlayer || isCharmedPlayer || isSelfHarm) && !isAttackerPet)
            multiplier = defense + (1 - defense) / (maxPlayers - 1) * (nplayers - 1);

        bool isPlayerVsPlayer = (isAttackerPlayer || isAttackerPet) && (isTargetPlayer || isTargetPet);
        bool isEnvironmentVsPlayer = (!isAttackerPlayer && !isAttackerPet) && (isTargetPlayer || isTargetPet);
        bool isPlayerVsEnvironment = (isAttackerPlayer || isAttackerPet) && (!isTargetPlayer && !isTargetPet);
        uint32 targetLevel = target->getLevel();
        uint32 attackerLevel = attacker->getLevel();

        if (isEnvironmentVsPlayer)
            multiplier = multiplier * playerCurve(targetLevel) / playerCurve(attackerLevel);
        else if (isPlayerVsEnvironment || (isPlayerVsPlayer && (!isPeriodicHeal || targetLevel > attackerLevel)))
            multiplier = multiplier * creatureCurve(target->getLevel()) / creatureCurve(attacker->getLevel());

        return amount * multiplier;
    }

    static float playerCurve(uint32 level)
    {
        return 45.2 * exp(0.0647 * level);
    }

    static float creatureCurve(uint32 level)
    {
        return 71.7 + 5.98 * level + 0.738 * pow(level, 2);
    }
};

class PlayerSettingsAllMapScript : public AllMapScript
{
public:
    PlayerSettingsAllMapScript() : AllMapScript("PlayerSettingsAllMapScript") {}

    void OnPlayerEnterAll(Map *map, Player *player)
    {
        if (!enabled)
            return;

        if (player->IsGameMaster())
            return;

        PlayerSettingsMapInfo *mapInfo = map->CustomData.GetDefault<PlayerSettingsMapInfo>("PlayerSettingsMapInfo");
        Map::PlayerList const &players = map->GetPlayers();
        mapInfo->nplayers = map->GetPlayersCountExceptGMs();

        if (!mapInfo->nplayers)
            mapInfo->nplayers = 1;

        if (!mapInfo->veto)
            mapInfo->veto = mapInfo->nplayers;

        for (Map::PlayerList::const_iterator iter = players.begin(); iter != players.end(); ++iter)
            if (Player *player = iter->GetSource())
            {
                if (mapInfo->nplayers > 1)
                {
                    if (map->GetEntry()->IsDungeon())
                        ChatHandler(player->GetSession()).PSendSysMessage("%s has entered the instance. The minions of hell grow stronger.", player->GetName().c_str());
                }
            }
    }

    void OnPlayerLeaveAll(Map *map, Player *player)
    {
        if (!enabled)
            return;

        if (player->IsGameMaster())
            return;

        PlayerSettingsMapInfo *mapInfo = map->CustomData.GetDefault<PlayerSettingsMapInfo>("PlayerSettingsMapInfo");
        Map::PlayerList const &players = map->GetPlayers();

        for (Map::PlayerList::const_iterator iter = players.begin(); iter != players.end(); ++iter)
            if (Player *player = iter->GetSource())
            {
                if (!player->IsInCombat())
                    mapInfo->nplayers = map->GetPlayersCountExceptGMs() - 1;

                if (mapInfo->nplayers > 0)
                {
                    if (map->GetEntry()->IsDungeon())
                        ChatHandler(player->GetSession()).PSendSysMessage("%s has left the instance. The minions of hell grow weaker.", player->GetName().c_str());
                }
            }
    }
};

class PlayerSettingsAllCreatureScript : public AllCreatureScript
{
public:
    PlayerSettingsAllCreatureScript() : AllCreatureScript("PlayerSettingsAllCreatureScript") {}

    void OnAllCreatureUpdate(Creature *creature, uint32 /*diff*/) override
    {
        if (!enabled)
            return;

        ModifyCreatureAttributes(creature);
    }

    void ModifyCreatureAttributes(Creature *creature)
    {
        if (!creature || !creature->GetMap())
            return;

        if (!creature->IsAlive())
            return;

        if (!creature->GetMap()->IsDungeon() && !creature->GetMap()->IsBattleground())
            if (!isWorldBoss(creature))
                return;

        if (((creature->IsHunterPet() || creature->IsPet() || creature->IsSummon()) && creature->IsControlledByPlayer()))
            return;

        PlayerSettingsMapInfo *mapInfo = creature->GetMap()->CustomData.GetDefault<PlayerSettingsMapInfo>("PlayerSettingsMapInfo");
        InstanceMap *instanceMap = ((InstanceMap *)sMapMgr->FindMap(creature->GetMapId(), creature->GetInstanceId()));
        uint32 nplayers = std::max(mapInfo->nplayers, mapInfo->veto);
        uint32 maxPlayers = instanceMap->GetMaxPlayers();

        if (maxPlayers == 0 && instanceMap->GetEntry()->IsWorldMap())
            maxPlayers = MAXRAIDSIZE;

        CreatureTemplate const *creatureTemplate = creature->GetCreatureTemplate();
        CreatureBaseStats const *stats = sObjectMgr->GetCreatureBaseStats(creature->getLevel(), creatureTemplate->unit_class);
        uint32 baseHealth = stats->GenerateHealth(creatureTemplate);
        uint32 scaledHealth = 0;

        float offense = Offense5M;

        if (maxPlayers > 5)
            offense = 1 / (maxPlayers - (2 + (maxPlayers / 5.0f)));

        float multiplier = offense + (1 - offense) / (maxPlayers - 1) * (nplayers - 1);
        scaledHealth = round((float)baseHealth * multiplier + 1.0f);

        uint32 previousHealth = creature->GetHealth();
        uint32 previousMaxHealth = creature->GetMaxHealth();

        if (scaledHealth != previousMaxHealth)
        {
            creature->SetCreateHealth(scaledHealth);
            creature->SetMaxHealth(scaledHealth);
            creature->ResetPlayerDamageReq();
            creature->SetModifierValue(UNIT_MOD_HEALTH, BASE_VALUE, (float)scaledHealth);
        }

        uint32 scaledCurrentHealth = previousHealth && previousMaxHealth ? float(scaledHealth) / float(previousMaxHealth) * float(previousHealth) : 0;

        static bool initialized;
        if (isWorldBoss(creature))
        {
            creature->SetHealth(scaledCurrentHealth);
            creature->UpdateAllStats();
            return;
        }

        if (!initialized)
        {
            initialized = true;
            creature->SetHealth(scaledCurrentHealth);
            creature->UpdateAllStats();
        }
    }
};

class PlayerSettingsCommandScript : public CommandScript
{
public:
    PlayerSettingsCommandScript() : CommandScript("PlayerSettingsCommandScript") {}

    Acore::ChatCommands::ChatCommandTable GetCommands() const
    {
        static Acore::ChatCommands::ChatCommandTable commands =
        {
            {"players", HandlePlayersCommand, SEC_PLAYER, Acore::ChatCommands::Console::No},
            {"playersettings", HandlePlayerSettingsCommand, SEC_PLAYER, Acore::ChatCommands::Console::No}
        };

        return commands;
    }

    static bool HandlePlayersCommand(ChatHandler *handler, std::string args)
    {
        char *x = strtok((char *)args.c_str(), " ");
        Player *player = handler->getSelectedPlayerOrSelf();
        Map *map = player->GetMap();
        PlayerSettingsMapInfo *mapInfo = map->CustomData.GetDefault<PlayerSettingsMapInfo>("PlayerSettingsMapInfo");
        InstanceMap *instanceMap = ((InstanceMap *)sMapMgr->FindMap(map->GetId(), map->GetInstanceId()));
        uint32 maxPlayers = instanceMap->GetMaxPlayers();

        if (maxPlayers == 0 && instanceMap->GetEntry()->IsWorldMap())
            maxPlayers = MAXRAIDSIZE;

        Map::PlayerList const &players = map->GetPlayers();
        if (!players.IsEmpty())
        {
            for (Map::PlayerList::const_iterator iter = players.begin(); iter != players.end(); ++iter)
            {
                if (Player *player = iter->GetSource())
                {
                    if (player->IsInCombat())
                    {
                        handler->SendSysMessage("Only usable outside of combat.");
                        return true;
                    }
                }
            }
        }

        if (x)
        {
            long int n = strtol(x, nullptr, 10);

            if (n < 1)
                n = 1;
            else if (n > maxPlayers)
                n = maxPlayers;

            mapInfo->veto = (uint32)n;
        }

        if (!players.IsEmpty())
        {
            for (Map::PlayerList::const_iterator iter = players.begin(); iter != players.end(); ++iter)
            {
                if (Player *player = iter->GetSource())
                {
                    ChatHandler(player->GetSession()).PSendSysMessage("Players set to %i.", std::max(mapInfo->nplayers, mapInfo->veto));
                }
            }
        }

        return true;
    }

    static bool HandlePlayerSettingsCommand(ChatHandler *handler)
    {

        Player *player = handler->getSelectedPlayerOrSelf();

        Map *map = player->GetMap();
        if (!map)
            return false;

        PlayerSettingsMapInfo *mapInfo = map->CustomData.GetDefault<PlayerSettingsMapInfo>("PlayerSettingsMapInfo");
        if (!mapInfo)
            return false;

        InstanceMap *instanceMap = ((InstanceMap *)sMapMgr->FindMap(player->GetMapId(), player->GetInstanceId()));
        if (!instanceMap)
            return false;

        uint32 maxPlayers = instanceMap->GetMaxPlayers();

        if (maxPlayers == 0 && instanceMap->GetEntry()->IsWorldMap())
            maxPlayers = MAXRAIDSIZE;

        float offense = Offense5M;
        float defense = Defense5M;

        if (maxPlayers > 5)
        {
            float tanks = 2.0f;
            float healers = maxPlayers / 5.0f;
            float dps = maxPlayers - (tanks + healers);
            offense = 1 / dps;
            defense = 1 / (tanks + healers);
        }

        uint32 nplayers = std::max(mapInfo->nplayers, mapInfo->veto);

        handler->PSendSysMessage("Players set to %i.", nplayers);
        handler->PSendSysMessage("Health multiplier set to %.2f.", offense + (1 - offense) / (maxPlayers - 1) * (nplayers - 1));
        handler->PSendSysMessage("Damage multiplier set to %.2f.", defense + (1 - defense) / (maxPlayers - 1) * (nplayers - 1));

        return true;
    }
};

void AddPlayerSettingsScripts()
{
    new PlayerSettingsWorldScript();
    new PlayerSettingsPlayerScript();
    new PlayerSettingsUnitScript();
    new PlayerSettingsAllMapScript();
    new PlayerSettingsAllCreatureScript();
    new PlayerSettingsCommandScript();
}
