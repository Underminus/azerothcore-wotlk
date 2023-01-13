#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "AchievementMgr.h"
#include "DBUpdater.h"

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
};

class WobgobWorldPlayer : public PlayerScript
{
public:
    WobgobWorldPlayer() : PlayerScript("WobgobWorldPlayer") {}

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
};

class WobgobWorldDatabase : public DatabaseScript
{
public:
    WobgobWorldDatabase() : DatabaseScript("ClassChangesDatabase") {}

    std::string path = "/modules/mod-wobgob-world/sql/";
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

void AddWobgobWorldScripts()
{
    new WobgobWorldPlayer();
    new WobgobWorldDatabase();
}
