#ifndef _WINZIG_ITEM_H_
#define _WINZIG_ITEM_H_

#include <random>
#include <vector>
#include "ScriptMgr.h"

enum Rarity
{
    RARITY_NONE,
    RARITY_ONE_STAR,
    RARITY_TWO_STAR,
    RARITY_THREE_STAR,
    RARITY_FOUR_STAR,
    RARITY_FIVE_STAR
};

enum Banner
{
    BANNER_NONE,
    BANNER_FEATURED,
    BANNER_PROMOTIONAL
};

enum Box
{
    BOX_NONE,
    BOX_STARTER,
    BOX_CLASSIC,
    BOX_BURNT,
    BOX_FROZEN
};

class item_loot_box : public ItemScript
{
public:
    item_loot_box() : ItemScript("item_loot_box"), gen(rd()), dis(0.0, 1.0) {}

    bool OnUse(Player *player, Item *item, SpellCastTargets const &/*targets*/) override;

private:
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<> dis;

    float roll();
    uint32 randomId(std::vector<uint32> ids);
    void openLootBox(Player *player, Item *item, enum Box box, enum Rarity rarity);
    bool sendRewardToPlayer(Player *player, uint32 itemId, enum Rarity rarity, enum Banner banner);
};

class item_reagent_pouch : public ItemScript
{
public:
    item_reagent_pouch() : ItemScript("item_reagent_pouch") {}

    bool OnUse(Player* player, Item* item, SpellCastTargets const& /*targets*/) override;
    void OnGossipSelect(Player* player, Item* /*item*/, uint32 /*sender*/, uint32 action) override;
};

#endif // _WINZIG_ITEM_H_
