#include "Chat.h"
#include "Config.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "ScriptedGossip.h"
#include "DataMap.h"
#include "Group.h"
#include "DBUpdater.h"

#define SPELL_AURA_HARDCORE     3758285
#define SPELL_AURA_IMMORTAL     3758286

#define TITLES_OFFSET           180
#define TITLES_PER_RANK         9

const std::map<uint32, uint32> Class2Index = {
    { CLASS_WARRIOR, 0 },
    { CLASS_PALADIN, 1 },
    { CLASS_HUNTER, 2 },
    { CLASS_ROGUE, 3 },
    { CLASS_PRIEST, 4 },
    { CLASS_SHAMAN, 5 },
    { CLASS_MAGE, 6 },
    { CLASS_WARLOCK, 7 },
    { CLASS_DRUID, 8 }
};

class HardcorePlayer : public PlayerScript
{
public:
    HardcorePlayer() : PlayerScript("HardcorePlayer") {}

    void OnLogin(Player* player) override
    {
        if (!sConfigMgr->GetOption<bool>("Hardcore.Enable", false))
            return;

        if (player->getLevel() >= sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL)) {
            Aura* hardcore = player->GetAura(SPELL_AURA_HARDCORE);

            if (hardcore) {
                player->RemoveAura(hardcore);
                player->AddAura(SPELL_AURA_IMMORTAL, player);
            }
        } else {
            Aura* immortal = player->GetAura(SPELL_AURA_IMMORTAL);

            if (immortal) {
                player->RemoveAura(immortal);
                player->AddAura(SPELL_AURA_HARDCORE, player);
            }
        }

        if (player->HasAura(SPELL_AURA_HARDCORE) || player->HasAura(SPELL_AURA_IMMORTAL)) {
            if (player->getLevel() >= 20) {
                setTitle(player, 0);
            }

            if (player->getLevel() >= 40) {
                setTitle(player, 1);
            }

            if (player->getLevel() >= 60) {
                setTitle(player, 2);
            }
        }

    }

    void OnLevelChanged(Player* player, uint8 /*oldlevel*/) override
    {
        if (player->HasAura(SPELL_AURA_HARDCORE) || player->HasAura(SPELL_AURA_IMMORTAL)) {
            if (player->getLevel() >= 20) {
                setTitle(player, 0);
            }

            if (player->getLevel() >= 40) {
                setTitle(player, 1);
            }

            if (player->getLevel() >= 60) {
                setTitle(player, 2);
            }
        }

        if (player->getLevel() >= sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL)) {
            Aura* hardcore = player->GetAura(SPELL_AURA_HARDCORE);

            if (hardcore) {
                player->RemoveAura(hardcore);
                player->AddAura(SPELL_AURA_IMMORTAL, player);
            }
        }
    }

    bool CanInitTrade(Player* player, Player* target) override
    {
        bool hardcorePlayer = player->HasAura(SPELL_AURA_HARDCORE);
        bool hardcoreTarget = target->HasAura(SPELL_AURA_HARDCORE);

        if (!hardcorePlayer && !hardcoreTarget)
            return true;

        if (!hardcorePlayer && hardcoreTarget)
            ChatHandler(player->GetSession()).PSendSysMessage("Cannot trade from a softcore character to a hardcore character.");

        if (hardcorePlayer && !hardcoreTarget)
            ChatHandler(player->GetSession()).PSendSysMessage("Cannot trade from a hardcore character to a softcore character.");

        if (hardcorePlayer && hardcoreTarget)
            return true;

        return false;
    }

    bool CanGroupInvite(Player* player, std::string& membername) override
    {
        Player* target = ObjectAccessor::FindPlayerByName(membername, true);
        if (!target)
            return false;

        bool hardcorePlayer = player->HasAura(SPELL_AURA_HARDCORE);
        bool hardcoreTarget = target->HasAura(SPELL_AURA_HARDCORE);

        if (!hardcorePlayer && !hardcoreTarget)
            return true;

        if (!hardcorePlayer && hardcoreTarget)
            ChatHandler(player->GetSession()).PSendSysMessage("Cannot invite a hardcore character to a softcore group.");

        if (hardcorePlayer && !hardcoreTarget)
            ChatHandler(player->GetSession()).PSendSysMessage("Cannot invite a softcore character to a hardcore group.");

        if (hardcorePlayer && hardcoreTarget)
            return true;

        return false;
    }

    bool CanSendMail(Player* player, ObjectGuid receiverGuid, ObjectGuid /*mailbox*/, std::string& /*subject*/, std::string& /*body*/, uint32 money, uint32 /*COD*/, Item* item) override
    {
        if (!money && !item)
            return true;

        Player* receiver = ObjectAccessor::FindConnectedPlayer(receiverGuid);
        bool hardcorePlayer = player->HasAura(SPELL_AURA_HARDCORE), hardcoreReceiver = false;

        if (receiver) {
            hardcoreReceiver = receiver->HasAura(SPELL_AURA_HARDCORE);
        } else {
            std::string query = "SELECT spell FROM character_aura WHERE guid = " + std::to_string(receiverGuid.GetCounter());
            QueryResult result = CharacterDatabase.Query(query);

            if (result) {
                do {
                    uint32 spell = (*result)[0].Get<uint32>();
                    if (spell == SPELL_AURA_HARDCORE) {
                        hardcoreReceiver = true;
                        break;
                    }
                } while (result->NextRow());
            }
        }

        if (!hardcorePlayer && !hardcoreReceiver)
            return true;

        if (!hardcorePlayer && hardcoreReceiver)
            ChatHandler(player->GetSession()).PSendSysMessage("Cannot send items or gold from a softcore character to a hardcore character.");

        if (hardcorePlayer && !hardcoreReceiver)
            ChatHandler(player->GetSession()).PSendSysMessage("Cannot send items or gold from a hardcore character to a softcore character.");

        if (hardcorePlayer && hardcoreReceiver)
            return true;

        player->SendMailResult(0, MAIL_SEND, MAIL_ERR_MAIL_ATTACHMENT_INVALID);
        return false;
    }

private:
    void setTitle(Player* player, uint32 rank)
    {
        auto it = Class2Index.find((Classes)player->getClass());

        if (it != Class2Index.end()) {
            uint32 title = TITLES_OFFSET + TITLES_PER_RANK * rank + it->second;
            CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(title);

            if (player->HasTitle(titleInfo))
                return;

            player->SetTitle(titleInfo, false);
        }
    }
};

class HardcoreGuild : public GuildScript
{
public:
    HardcoreGuild() : GuildScript("HardcoreGuild") {}

    bool CanGuildSendBankList(Guild const* /*guild*/, WorldSession* session, uint8 /*tabId*/, bool /*sendAllSlots*/) override
    {
        if (!session)
            return true;

        Player* player = session->GetPlayer();
        if (!player)
            return true;

        if (player->HasAura(SPELL_AURA_HARDCORE)) {
            ChatHandler(player->GetSession()).PSendSysMessage("Hardcore characters cannot use guild banks.");
            return false;
        }

        return true;
    }
};

class HardcoreMisc : public MiscScript
{
public:
    HardcoreMisc() : MiscScript("HardcoreMisc") {}

    bool CanSendAuctionHello(WorldSession const* session, ObjectGuid /*guid*/, Creature* /*creature*/) override
    {
        if (!session)
            return true;

        Player* player = session->GetPlayer();
        if (!player)
            return true;

        if (player->HasAura(SPELL_AURA_HARDCORE)) {
            ChatHandler(player->GetSession()).PSendSysMessage("Hardcore characters cannot use the auction house.");
            return false;
        }

        return true;
    }
};

void AddHardcoreScripts()
{
    new HardcorePlayer();
    new HardcoreGuild();
    new HardcoreMisc();
}
