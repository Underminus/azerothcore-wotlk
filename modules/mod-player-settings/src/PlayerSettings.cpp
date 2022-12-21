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
#include <algorithm>
#include <cstdlib>
#include <vector>

// DPS count as 1 offensive unit. Tanks and healers count as 1 defensive unit.
// 5 man: 1 tank, 3 dps, 1 healer = 3 offensive units and 2 defensive units.
const float Offense5M = 1 / 3.0f, Defense5M = 1 / 2.0f;

enum WorldBosses
{
    BOSS_VAELASTRASZ    = 13020,
    BOSS_TAERAR         = 14890,
    BOSS_EMERISS        = 14889,
    BOSS_LETHON         = 14888,
    BOSS_YSONDRE        = 14887,
    BOSS_AZUREGOS       = 6109,
    BOSS_KAZZAK         = 12397,
    BOSS_MAWS           = 15571,
    BOSS_ERANIKUS       = 15491,
};

std::array<uint32, 9> WorldBossArray =
{
    BOSS_VAELASTRASZ, BOSS_TAERAR, BOSS_EMERISS, BOSS_LETHON,
    BOSS_YSONDRE, BOSS_AZUREGOS, BOSS_KAZZAK, BOSS_MAWS,
    BOSS_ERANIKUS
};

enum WorldBossZones
{
    ZONE_TAERAR     = 856,  // Twilight Grove
    ZONE_EMERISS    = 1111,  // Dream Bough
    ZONE_LETHON     = 356,  // Seradane
    ZONE_YSONDRE    = 438,  // Bough Shadow
    ZONE_AZUREGOS   = 16,  // Azshara
    ZONE_KAZZAK     = 73  // The Tainted Scar
};

std::array<uint32, 6> WorldBossZoneArray =
{
    ZONE_TAERAR, ZONE_EMERISS, ZONE_LETHON, ZONE_YSONDRE,
    ZONE_AZUREGOS, ZONE_KAZZAK
};

enum Spells
{
    SPELL_LUCIFRON_CURSE = 19703,
    SPELL_GEHENNAS_CURSE = 19716,
    SPELL_IGNITE_MANA = 19659,
    SPELL_SHAZZRAH_CURSE = 19713,
    SPELL_BURNING_ADRENALINE = 18173,
    SPELL_BROOD_AFFLICTION_BLACK = 23154,
    SPELL_BROOD_AFFLICTION_BLUE = 23153,
    SPELL_BROOD_AFFLICTION_BRONZE = 23170,
    SPELL_BROOD_AFFLICTION_GREEN = 23169,
    SPELL_BROOD_AFFLICTION_RED = 23155,
    SPELL_ONYXIA_SCALE_CLOAK = 22683
};

class PlayerSettingsCreatureInfo : public DataMap::Base
{
public:
    PlayerSettingsCreatureInfo() {}
    uint32 nplayers = 0;
    // This is used to detect creatures that update their entry.
    uint32 entry = 0;
    float HealthMultiplier = 1;
};

class PlayerSettingsMapInfo : public DataMap::Base
{
public:
    PlayerSettingsMapInfo() {}
    uint32 nplayers = 0;
    uint32 veto = 0;
    std::map<uint32, float> honor;
    std::map<uint32, bool> rewarded;
    std::map<uint32, uint32> rdf;
};

static bool enabled;
static float goldMultiplier, honorMultiplier;

class PlayerSettingsWorldScript : public WorldScript
{
public:
    PlayerSettingsWorldScript() : WorldScript("PlayerSettingsWorldScript") {}

    void OnBeforeConfigLoad(bool /*reload*/) override
    {
        SetInitialWorldSettings();
    }

    void SetInitialWorldSettings()
    {
        enabled = sConfigMgr->GetOption<bool>("PlayerSettings.Enable", 1);
        goldMultiplier = sConfigMgr->GetOption<float>("PlayerSettings.Gold", 0.1);
        honorMultiplier = sConfigMgr->GetOption<float>("PlayerSettings.Honor", 0.1);
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

        if (mapInfo->nplayers > 5 && oldArea != newArea)
        {
            if (std::find(WorldBossZoneArray.begin(), WorldBossZoneArray.end(), newArea) != WorldBossZoneArray.end())
                for (Map::PlayerList::const_iterator iter = players.begin(); iter != players.end(); ++iter)
                    if (Player *player = iter->GetSource())
                            ChatHandler(player->GetSession()).PSendSysMessage("%s has entered a World Boss area. The minions of hell grow stronger.", player->GetName().c_str());

            if (std::find(WorldBossZoneArray.begin(), WorldBossZoneArray.end(), oldArea) != WorldBossZoneArray.end())
                for (Map::PlayerList::const_iterator iter = players.begin(); iter != players.end(); ++iter)
                    if (Player *player = iter->GetSource())
                            ChatHandler(player->GetSession()).PSendSysMessage("%s has left a World Boss area. The minions of hell grow weaker.", player->GetName().c_str());
        }
    }
    void OnGiveXP(Player *player, uint32 &amount, Unit *victim) override
    {
        if (victim)
        {
            Map *map = player->GetMap();

            if (map->IsDungeon() && !map->IsBattlegroundOrArena())
            {
                uint32 maxPlayers = ((InstanceMap *)sMapMgr->FindMap(map->GetId(), map->GetInstanceId()))->GetMaxPlayers();
                PlayerSettingsMapInfo *mapInfo = map->CustomData.GetDefault<PlayerSettingsMapInfo>("PlayerSettingsMapInfo");

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

            amount = amount * mapInfo->nplayers / maxPlayers * (1 + goldMultiplier * (mapInfo->nplayers - 1));
        }
    }

    void OnCreatureKill(Player *player, Creature *killed) override
    {
        if (!killed)
            return;

        if (killed->GetCreatureTemplate()->type_flags & CREATURE_TYPE_FLAG_BOSS_MOB)
            return;

        if (killed->IsDungeonBoss())
            rewardHonor(player, killed);
    }

    void OnCreatureKilledByPet(Player *owner, Creature *killed) override
    {
        if (!killed)
            return;

        if (killed->GetCreatureTemplate()->type_flags & CREATURE_TYPE_FLAG_BOSS_MOB)
            return;

        if (killed->IsDungeonBoss())
            rewardHonor(owner, killed);
    }

    void OnPlayerCompleteQuest(Player *player, Quest const *quest) override
    {
        uint32 guid = player->GetGUID().GetCounter();
        Map *map = player->GetMap();
        PlayerSettingsMapInfo *mapInfo = map->CustomData.GetDefault<PlayerSettingsMapInfo>("PlayerSettingsMapInfo");

        switch (quest->GetQuestId())
        {
        case 24881:
        case 24882:
        case 24883:
        case 24884:
        case 24885:
        case 24886:
        case 24888:
        case 24922:
        case 24790:
        case 24788:
            mapInfo->rdf[guid] = 2;
            return;
        case 24889:
        case 24890:
        case 24891:
        case 24892:
        case 24893:
        case 24894:
        case 24896:
        case 24923:
        case 24791:
        case 24789:
            mapInfo->rdf[guid] = 1;
            return;
        }
    }

    void OnPlayerLeaveCombat(Player* player) override
    {
        Aura* lucifron = player->GetAura(SPELL_LUCIFRON_CURSE);

        if (lucifron)
            player->RemoveAura(lucifron);

        Aura* gehennas = player->GetAura(SPELL_GEHENNAS_CURSE);

        if (gehennas)
            player->RemoveAura(gehennas);

        Aura* ignite = player->GetAura(SPELL_IGNITE_MANA);

        if (ignite)
            player->RemoveAura(ignite);

        Aura* shazzrah = player->GetAura(SPELL_SHAZZRAH_CURSE);

        if (shazzrah)
            player->RemoveAura(shazzrah);

        Aura* vaelastrasz = player->GetAura(SPELL_BURNING_ADRENALINE);

        if (vaelastrasz)
            player->RemoveAura(vaelastrasz);

        Aura* black = player->GetAura(SPELL_BROOD_AFFLICTION_BLACK);

        if (black)
            player->RemoveAura(black);

        Aura* blue = player->GetAura(SPELL_BROOD_AFFLICTION_BLUE);

        if (blue)
            player->RemoveAura(blue);

        Aura* bronze = player->GetAura(SPELL_BROOD_AFFLICTION_BRONZE);

        if (bronze)
            player->RemoveAura(bronze);

        Aura* green = player->GetAura(SPELL_BROOD_AFFLICTION_GREEN);

        if (green)
            player->RemoveAura(green);

        Aura* red = player->GetAura(SPELL_BROOD_AFFLICTION_RED);

        if (red)
            player->RemoveAura(red);
    }

private:
    void rewardHonor(Player *killer, Creature *killed)
    {
        Map *map = killer->GetMap();

        if (map->IsDungeon() && !map->IsBattlegroundOrArena())
        {
            Map::PlayerList const &players = map->GetPlayers();

            if (!players.IsEmpty())
            {
                for (Map::PlayerList::const_iterator iter = players.begin(); iter != players.end(); ++iter)
                {
                    if (Player *player = iter->GetSource())
                    {
                        player->UpdateHonorFields();

                        int honor = 0;
                        PlayerSettingsMapInfo *mapInfo = map->CustomData.GetDefault<PlayerSettingsMapInfo>("PlayerSettingsMapInfo");
                        uint32 nplayers = std::min(5u, std::max(mapInfo->nplayers, mapInfo->veto));
                        Group* group = player->GetGroup();
                        uint8 playerLevel = player->getLevel();
                        uint8 gray = Acore::XP::GetGrayLevel(playerLevel);
                        uint8 bossLevel = killed->getLevel();

                        if (group && group->isLFGGroup())
                        {
                            float honor_f = Acore::Honor::hk_honor_at_level_f(playerLevel);
                            honor_f *= sWorld->getRate(RATE_HONOR);
                            honor_f *= 1 + honorMultiplier * (nplayers - 1);
                            honor = int32(honor_f);
                        }
                        else if (bossLevel > gray)
                        {
                            float multiplier = float(bossLevel - gray) / float(playerLevel - gray);
                            float honor_f = ceil(Acore::Honor::hk_honor_at_level_f(playerLevel) * multiplier);
                            honor_f *= sWorld->getRate(RATE_HONOR);
                            honor_f *= 1 + honorMultiplier * (nplayers - 1);
                            honor = int32(honor_f);
                        }

                        if (honor)
                        {
                            player->ModifyHonorPoints(honor);
                            ChatHandler(player->GetSession()).PSendSysMessage("You have been awarded %i honor.", honor);
                            uint32 guid = player->GetGUID().GetCounter();
                            mapInfo->honor[guid] += honor;
                            rewardBonusHonor(player, mapInfo->honor[guid]);
                        }
                    }
                }
            }
        }
    }

    void rewardBonusHonor(Player *player, float honor_f)
    {
        uint32 guid = player->GetGUID().GetCounter();
        Map *map = player->GetMap();

        if (map->IsDungeon() && !map->IsBattlegroundOrArena())
        {
            PlayerSettingsMapInfo *mapInfo = map->CustomData.GetDefault<PlayerSettingsMapInfo>("PlayerSettingsMapInfo");

            if (mapInfo->rewarded[guid] || !mapInfo->rdf[guid])
                return;

            honor_f *= sWorld->getRate(RATE_HONOR);
            honor_f *= mapInfo->rdf[guid];
            player->UpdateHonorFields();
            int honor = int32(honor_f);
            player->ModifyHonorPoints(honor);
            ChatHandler(player->GetSession()).PSendSysMessage("You have been awarded %i bonus honor.", honor);
            mapInfo->rewarded[guid] = true;
        }
    }
};

class PlayerSettingsUnitScript : public UnitScript
{
public:
    PlayerSettingsUnitScript() : UnitScript("PlayerSettingsUnitScript", true) {}

    void ModifyPeriodicDamageAurasTick(Unit* target, Unit* attacker, uint32& damage) override
    {
        if (check(target, attacker))
            damage = modify(target, attacker, damage);
    }

    void ModifyMeleeDamage(Unit* target, Unit* attacker, uint32& damage) override
    {
        if (check(target, attacker))
            damage = modify(target, attacker, damage);
    }

    void ModifySpellDamageTaken(Unit* target, Unit* attacker, int32& damage, SpellInfo const* spellInfo) override
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

    bool isWorldBoss(Unit *target, Unit *attacker)
    {
        if (std::find(WorldBossArray.begin(), WorldBossArray.end(), target->GetEntry()) != WorldBossArray.end())
            return true;

        if (std::find(WorldBossArray.begin(), WorldBossArray.end(), attacker->GetEntry()) != WorldBossArray.end())
            return true;

        return false;
    }

    bool check(Unit* attacker, Unit* target)
    {
        if (!target || !target->GetMap())
            return false;

        if (!attacker || !attacker->GetMap())
            return false;

        if (isWorldBoss(target, attacker))
            return true;

        if (!inDungeon(target, attacker) && !inBattleground(target, attacker))
            return false;

        return true;
    }

    uint32 modify(Unit* target, Unit* attacker, uint32 amount, bool isDamage = true, bool isPeriodicHeal = false)
    {
        PlayerSettingsMapInfo *mapInfo = target->GetMap()->CustomData.GetDefault<PlayerSettingsMapInfo>("PlayerSettingsMapInfo");
        InstanceMap *instanceMap = ((InstanceMap *)sMapMgr->FindMap(target->GetMapId(), target->GetInstanceId()));

        if (!check(target, attacker))
            return amount;

        uint32 nplayers = std::max(mapInfo->nplayers, mapInfo->veto);
        uint32 maxPlayers = instanceMap->GetMaxPlayers();

        float defense = Defense5M;

        if (maxPlayers > 5)
            defense = 1 / (2 + (maxPlayers / 5.0f));

        float multiplier = 1.0f;
        bool isAttackerPlayer = attacker->GetTypeId() == TYPEID_PLAYER;
        bool isAttackerPet = (attacker->IsHunterPet() || attacker->IsPet() || attacker->IsSummon() || attacker->IsTotem()) && attacker->IsControlledByPlayer();
        bool isTargetPlayer = target->GetTypeId() == TYPEID_PLAYER;
        bool isTargetPet = (target->IsHunterPet() || target->IsPet() || target->IsSummon() || target->IsTotem()) && attacker->IsControlledByPlayer();
        bool isSelfHarm = (isAttackerPlayer && isTargetPlayer) && attacker->GetGUID() == target->GetGUID() && isDamage;
        bool isCharmedPlayer = isAttackerPlayer && attacker->GetCharmerGUID();

        if ((!isAttackerPlayer || isCharmedPlayer || isSelfHarm) && !isAttackerPet)
            multiplier = defense + (1 - defense) / (maxPlayers - 1) * (nplayers - 1);

        Player* player = nullptr;

        if (isAttackerPlayer)
            player = attacker->ToPlayer();
        else if (isTargetPlayer)
            player = target->ToPlayer();
        else if (isAttackerPet)
            player = attacker->GetOwner()->ToPlayer();
        else if (isTargetPet)
            player = target->GetOwner()->ToPlayer();

        Group* group = nullptr;

        if (player)
            group = player->GetGroup();

        if (group && group->isLFGGroup())
        {
            bool isPlayerVsPlayer = (isAttackerPlayer || isAttackerPet) && (isTargetPlayer || isTargetPet);
            bool isEnvironmentVsPlayer = (!isAttackerPlayer && !isAttackerPet) && (isTargetPlayer || isTargetPet);
            bool isPlayerVsEnvironment = (isAttackerPlayer || isAttackerPet) && (!isTargetPlayer && !isTargetPet);
            uint32 targetLevel = target->getLevel();
            uint32 attackerLevel = attacker->getLevel();

            if (isEnvironmentVsPlayer)
                multiplier = multiplier * playerCurve(targetLevel) / playerCurve(attackerLevel);
            else if (isPlayerVsEnvironment || (isPlayerVsPlayer && (!isPeriodicHeal || targetLevel > attackerLevel)))
                multiplier = multiplier * creatureCurve(target->getLevel()) / creatureCurve(attacker->getLevel());
        }

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
            if (!(std::find(WorldBossArray.begin(), WorldBossArray.end(), creature->GetEntry()) != WorldBossArray.end()))
                return;

        if (((creature->IsHunterPet() || creature->IsPet() || creature->IsSummon()) && creature->IsControlledByPlayer()))
            return;

        PlayerSettingsMapInfo *mapInfo = creature->GetMap()->CustomData.GetDefault<PlayerSettingsMapInfo>("PlayerSettingsMapInfo");
        CreatureTemplate const *creatureTemplate = creature->GetCreatureTemplate();
        InstanceMap *instanceMap = ((InstanceMap *)sMapMgr->FindMap(creature->GetMapId(), creature->GetInstanceId()));
        PlayerSettingsCreatureInfo *creatureInfo = creature->CustomData.GetDefault<PlayerSettingsCreatureInfo>("PlayerSettingsCreatureInfo");

        creatureInfo->entry = creature->GetEntry();
        creatureInfo->nplayers = std::max(mapInfo->nplayers, mapInfo->veto);

        if (!creatureInfo->nplayers)
            return;

        CreatureBaseStats const *stats = sObjectMgr->GetCreatureBaseStats(creature->getLevel(), creatureTemplate->unit_class);
        uint32 baseHealth = stats->GenerateHealth(creatureTemplate);
        uint32 scaledHealth = 0;
        uint32 nplayers = creatureInfo->nplayers;
        uint32 maxPlayers = instanceMap->GetMaxPlayers();

        float offense = Offense5M;

        if (maxPlayers > 5)
            offense = 1 / (maxPlayers - (2 + (maxPlayers / 5.0f)));

        creatureInfo->HealthMultiplier = offense + (1 - offense) / (maxPlayers - 1) * (nplayers - 1);
        scaledHealth = round(((float)baseHealth * creatureInfo->HealthMultiplier) + 1.0f);

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
        if (std::find(WorldBossArray.begin(), WorldBossArray.end(), creature->GetEntry()) != WorldBossArray.end())
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
        uint32 maxPlayers = ((InstanceMap *)sMapMgr->FindMap(map->GetId(), map->GetInstanceId()))->GetMaxPlayers();

        if (!map->IsDungeon())
        {
            handler->SendSysMessage("Only usable in dungeons.");
            return true;
        }

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

        if (!map->IsDungeon())
            return false;

        PlayerSettingsMapInfo *mapInfo = map->CustomData.GetDefault<PlayerSettingsMapInfo>("PlayerSettingsMapInfo");

        if (!mapInfo)
            return false;

        InstanceMap *instanceMap = ((InstanceMap *)sMapMgr->FindMap(player->GetMapId(), player->GetInstanceId()));

        if (!instanceMap)
            return false;

        uint32 maxPlayers = instanceMap->GetMaxPlayers();

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

        nplayers = std::min(5u, std::max(mapInfo->nplayers, mapInfo->veto));

        handler->PSendSysMessage("Gold multiplier set to %.2f.", !nplayers ? 0 : (1 + goldMultiplier * (nplayers - 1)));
        handler->PSendSysMessage("Honor multiplier set to %.2f.", !nplayers ? 0 : (1 + honorMultiplier * (nplayers - 1)));

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
