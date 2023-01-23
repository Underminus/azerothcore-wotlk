#include <ctime>
#include <chrono>
#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "WinzigCommon.h"
#include "WinzigCreature.h"
#include "WinzigItem.h"
#include "WinzigWorld.h"
#include "Group.h"

const std::string SUBJECT = "I got what you need!";
const std::string BODY =
    "Ah, potential customer. Here's a sample of my wares.\n\n"
    "Meet me in Booty Bay to get the best deals, anywhere!\n\n"
    "Be careful out there,\n"
    "Winzig";

class WinzigPlayer : public PlayerScript
{
public:
    WinzigPlayer() : PlayerScript("WinzigPlayer") { }

    void OnLogin(Player *player) override
    {
        if (!WinzigWorld::Enabled)
            return;

        linkBannerItems(player);

        uint32 guid = player->GetGUID().GetCounter();
        QueryResult result = CharacterDatabase.Query("SELECT `logout_time` FROM `characters` WHERE guid = '{}'", guid);
        std::chrono::system_clock::time_point logout_time_point(std::chrono::seconds(result->Fetch()->Get<uint32>()));
        time_t logout_time = std::chrono::system_clock::to_time_t(logout_time_point);
        time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        time_t reset = getReset();

        if (std::difftime(reset, logout_time) > 0 && std::difftime(now, reset) >= 0)
            sendDailyReward(player);
    }

    void OnLevelChanged(Player *player, uint8 oldlevel) override
    {
        if (!WinzigWorld::Enabled)
            return;

        uint32 amount = 0;
        uint8 level = player->getLevel();

        if (oldlevel < 20 && level >= 20)
        {
            CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();

            Item *boxes = Item::CreateItem(WinzigWorld::StarterBox, 20);
            boxes->SaveToDB(trans);

            Item *pouch = Item::CreateItem(WinzigWorld::ReagentPouch, 1);
            pouch->SaveToDB(trans);

            MailSender sender(WinzigWorld::NPC);
            MailDraft draft(SUBJECT, BODY);
            draft.AddItem(boxes);
            draft.AddItem(pouch);
            draft.SendMailTo(trans, MailReceiver(player), sender);

            CharacterDatabase.CommitTransaction(trans);
        }

        if (level <= 19)
            amount = 5;
        else if (level <= 29)
            amount = 15;
        else if (level <= 44)
            amount = 20;
        else if (level <= 54)
            amount = 25;
        else if (level <= 60)
            amount = 50;

        player->AddItem(WinzigWorld::CustomCurrency, amount);
    }

    void OnCreatureKill(Player *killer, Creature *killed) override
    {
        if (!WinzigWorld::Enabled)
            return;

        if (!killed)
            return;

        if (killed->GetCreatureTemplate()->type_flags & CREATURE_TYPE_FLAG_BOSS_MOB)
        {
            giveCurrency(killer, 50);
            return;
        }

        if (killed->IsDungeonBoss())
            giveCurrency(killer, 15);
    }

    void OnCreatureKilledByPet(Player* owner, Creature* killed) override
    {
        if (!WinzigWorld::Enabled)
            return;

        if (!killed)
            return;

        if (killed->GetCreatureTemplate()->type_flags & CREATURE_TYPE_FLAG_BOSS_MOB)
        {
            giveCurrency(owner, 50);
            return;
        }

        if (killed->IsDungeonBoss())
            giveCurrency(owner, 15);
    }

    void OnPlayerCompleteQuest(Player* player, Quest const* quest) override
    {
        if (!WinzigWorld::Enabled)
            return;

        switch (quest->GetQuestId())
        {
            // Completed first random dungeon of the day
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
                player->AddItem(WinzigWorld::CustomCurrency, 200);
                return;

            // Completed another random dungeon
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
                player->AddItem(WinzigWorld::CustomCurrency, 100);
                return;
        }
    }


private:
    void giveCurrency(Player *killer, uint32 amount)
    {
        Map *map = killer->GetMap();
        if (map->GetEntry()->IsDungeon())
        {
            Map::PlayerList const &players = map->GetPlayers();

            for (Map::PlayerList::const_iterator iter = players.begin(); iter != players.end(); ++iter)
                if (Player *player = iter->GetSource())
                    player->AddItem(WinzigWorld::CustomCurrency, amount);
        }
    }

    void sendDailyReward(Player *player)
    {
        ChatHandler(player->GetSession()).PSendSysMessage("Daily login reward!");
        player->AddItem(WinzigWorld::CustomCurrency, 90);
    }

    time_t getReset()
    {
        time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        tm local = *localtime(&now);
        local.tm_sec = 0;
        local.tm_min = 0;
        local.tm_hour = 0;
        time_t reset = mktime(&local);
        return reset;
    }

    void linkBannerItems(Player* player)
    {
        auto months = std::chrono::duration_cast<Months>(std::chrono::system_clock::now().time_since_epoch()).count();
        uint8 season = (months % 9) + 1;
        uint8 box = 0;
        uint8 level = player->getLevel();
        std::vector<uint32> boxes = { 0, WinzigWorld::StarterBox, WinzigWorld::ClassicBox, WinzigWorld::BurntBox, WinzigWorld::FrozenBox };

        QueryResult rows = WorldDatabase.Query(
            "SELECT `item`, `box` FROM `item_loot_box` WHERE `season` = {} AND `box` IN (1, 2, 3)",
            season, box
        );

        if (!rows)
            return;

        std::map<uint8, std::vector<uint32>> entries;

        do
        {
            Field* row = rows->Fetch();
            uint32 entry = row[0].Get<uint32>();
            uint8 box = row[1].Get<uint8>();

            if (box == BOX_CLASSIC || (box == BOX_BURNT && level > 60) || (box == BOX_FROZEN && level > 70))
                entries[box].push_back(entry);
        } while (rows->NextRow());

        for (int box = BOX_CLASSIC; box <= BOX_FROZEN; box++) {
            std::stringstream ss;
            bool first = true;
            std::vector<uint32>::const_iterator entry;

            for (entry = entries[box].begin(); entry != entries[box].end(); entry++)
            {
                if (first)
                    first = false;
                else if (std::next(entry) == entries[box].end())
                    ss << ", and ";
                else
                    ss << ", ";

                std::string link = Winzig::GetItemLink(*entry, player->GetSession());
                ss << link;
            }

            if (box == BOX_CLASSIC || (box == BOX_BURNT && level > 60) || (box == BOX_FROZEN && level > 70))
            {
                std::string link = Winzig::GetItemLink(boxes[box], player->GetSession());
                std::string links = ss.str();

                ChatHandler(player->GetSession()).PSendSysMessage("%s features %s.", link, links.c_str());
            }
        }
    }
};

void AddWinzigPlayerScripts()
{
    new WinzigPlayer();
}
