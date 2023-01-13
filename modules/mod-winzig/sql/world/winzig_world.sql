DELETE FROM `item_template` WHERE `entry` = 2251094;
INSERT INTO `item_template` (`entry`, `name`, `displayid`, `Quality`, `BuyPrice`, `maxcount`, `stackable`, `spellid_1`, `spellcooldown_1`, `description`, `duration`, `ScriptName`, `flagsCustom`)
VALUES (2251094, 'Reagent Pouch', 34798, 6, 900000, 1, 1, 18282, 0, 'This bag has an interior space considerably larger than its outside dimensions.', 2592000, 'item_reagent_pouch', 1);

DELETE FROM `item_template` WHERE `entry` = 5621798;
INSERT INTO `item_template` (`entry`, `name`, `displayid`, `Quality`, `stackable`, `spellid_1`, `spellcooldown_1`, `description`, `ScriptName`)
VALUES (5621798, 'Starter Loot Box', 12331, 6, 250, 18282, 0, 'Just a taste of the real thing.', 'item_loot_box');

DELETE FROM `item_template` WHERE `entry` = 5621799;
INSERT INTO `item_template` (`entry`, `name`, `displayid`, `Quality`, `stackable`, `spellid_1`, `spellcooldown_1`, `description`, `ScriptName`)
VALUES (5621799, 'Classic Loot Box', 12644, 6, 250, 18282, 0, "I can't believe it's not gambling!", 'item_loot_box');

DELETE FROM `item_template` WHERE `entry` = 5621800;
INSERT INTO `item_template` (`entry`, `name`, `displayid`, `Quality`, `RequiredLevel`, `stackable`, `spellid_1`, `spellcooldown_1`, `description`, `ScriptName`)
VALUES (5621800, 'Burnt Loot Box', 12332, 6, 61, 250, 18282, 0, "I can't believe it's not gambling!", 'item_loot_box');

DELETE FROM `item_template` WHERE `entry` = 5621801;
INSERT INTO `item_template` (`entry`, `name`, `displayid`, `Quality`, `RequiredLevel`, `stackable`, `spellid_1`, `spellcooldown_1`, `description`, `ScriptName`)
VALUES (5621801, 'Frozen Loot Box', 16028, 6, 71, 250, 18282, 0, "I can't believe it's not gambling!", 'item_loot_box');

CREATE TABLE IF NOT EXISTS `item_loot_box` (
    `id` SERIAL PRIMARY KEY,
    `item` INT UNSIGNED,
    `season` TINYINT,
    `rarity` TINYINT,
    `box` TINYINT,
    `exclusive` BOOLEAN
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DELETE FROM `item_loot_box`;
INSERT INTO `item_loot_box` (`item`, `season`, `rarity`, `box`, `exclusive`)
VALUES
    -- Classic Tabards
    (38309, 1, 5, 2, true), -- Tabard of Nature (Season 1)
    (38310, 2, 5, 2, true), -- Tabard of the Arcane (Season 2)
    (38313, 3, 5, 2, true), -- Tabard of Fury (Season 3)
    (23709, 4, 5, 2, true), -- Tabard of Frost (Season 4)
    (38311, 5, 5, 2, true), -- Tabard of the Void (Season 5)
    (38314, 6, 5, 2, true), -- Tabard of the Defender (Season 6)
    (23705, 7, 5, 2, true), -- Tabard of Flame (Season 7)
    (38312, 8, 5, 2, true), -- Tabard of Brilliance (Season 8)
    (36941, 9, 5, 2, true); -- Competitor's Tabard (Season 9)

INSERT INTO `item_loot_box` (`item`, `season`, `rarity`, `box`)
VALUES
    -- Common Starter
    (804, 0, 3, 1), -- Large Blue Sack
    (3386, 0, 3, 1), -- Potion of Curing
    (6048, 0, 3, 1), -- Shadow Protection Potion
    (3388, 0, 3, 1), -- Strong Troll's Blood Elixir
    (45621, 0, 3, 1), -- Elixir of Minor Accuracy
    -- Rare Starter
    (13583, 0, 4, 1), -- Panda Cub
    (19054, 0, 4, 1), -- Red Dragon Orb
    (19055, 0, 4, 1), -- Green Dragon Orb
    -- Epic Starter
    (54452, 0, 5, 1); -- Ethereal Portal

INSERT INTO `item_loot_box` (`item`, `season`, `rarity`, `box`)
VALUES
    -- Common Classic
    (46779, 0, 3, 2), -- Path of Cenarius
    (13446, 0, 3, 2), -- Major Healing Potion
    (13444, 0, 3, 2), -- Major Mana Potion
    (13462, 0, 3, 2), -- Purification Potion
    (3387, 0, 3, 2), -- Limited Invulnerability Potion
    (13459, 0, 3, 2), -- Greater Shadow Protection Potion
    (13457, 0, 3, 2), -- Greater Fire Protection Potion
    (13461, 0, 3, 2), -- Greater Arcane Protection Potion
    (13458, 0, 3, 2), -- Greater Nature Protection Potion
    (13456, 0, 3, 2), -- Greater Frost Protection Potion
    -- Rare Classic
    (19160, 1, 4, 2), -- Contest Winner's Tabard (Season 1)
    (19160, 9, 4, 2), -- Contest Winner's Tabard (Season 9)
    (49704, 1, 4, 2), -- Carved Ogre Idol (Season 1)
    (49704, 6, 4, 2), -- Carved Ogre Idol (Season 6)
    (39656, 1, 4, 2), -- Mini Tyrael (Season 1)
    (18964, 2, 4, 2), -- Loggerhead Snapjaw (Season 2)
    (18964, 4, 4, 2), -- Loggerhead Snapjaw (Season 4)
    (18964, 8, 4, 2), -- Loggerhead Snapjaw (Season 8)
    (33223, 2, 4, 2), -- Fishing Chair (Season 2)
    (33223, 7, 4, 2), -- Fishing Chair (Season 7)
    (13584, 2, 4, 2), -- Mini Diablo (Season 2)
    (45047, 3, 4, 2), -- Sandbox Tiger (Season 3)
    (45047, 8, 4, 2), -- Sandbox Tiger (Season 8)
    (44819, 3, 4, 2), -- Baby Blizzard Bear (Season 3)
    (45063, 4, 4, 2), -- Foam Sword Rack (Season 4)
    (45063, 9, 4, 2), -- Form Sword Rack (Season 9)
    (49343, 4, 4, 2), -- Spectral Tiger Cub (Season 4)
    (45037, 5, 4, 2), -- Epic Purple Shirt (Season 5)
    (32588, 5, 4, 2), -- Bananas (Season 5)
    (32588, 7, 4, 2), -- Bananas (Season 7)
    (20371, 6, 4, 2), -- Blue Murloc Egg (Murky) (Season 6)
    (30360, 3, 4, 2), -- Lurky's Egg (Lurky) (Season 3)
    (30360, 7, 4, 2), -- Lurky's Egg (Lurky) (Season 7)
    (34519, 6, 4, 2), -- Silver Pig (Season 6)
    (34519, 8, 4, 2), -- Silver Pig (Season 8)
    (34518, 5, 4, 2), -- Golden Pig (Season 5)
    (34518, 9, 4, 2), -- Golden Pig (Season 9)
    -- Epic Classic
    (49283, 0, 5, 2), -- Reins of the Spectral Tiger
    (37719, 0, 5, 2), -- Swift Zhevra
    (49290, 0, 5, 2); -- Magic Rooster Egg

UPDATE `item_template`
SET `name` = 'Macaroon', `displayid` = 51567, `maxcount` = 2147483647, `stackable` = 2147483647, `description` = 'Time is money, friend.'
WHERE entry = 37711;

UPDATE `item_template` SET `bonding` = 0, `maxcount` = 0 WHERE `entry` IN (
    -- Tabards
    38309, -- Tabard of Nature
    38310, -- Tabard of the Arcane
    38313, -- Tabard of Fury
    23709, -- Tabard of Frost
    38311, -- Tabard of the Void
    38314, -- Tabard of the Defender
    23705, -- Tabard of Flame
    38312, -- Tabard of Brilliance
    36941, -- Competitor's Tabard

    -- Starter Loot Box
    804, -- Large Blue Sack
    13583, -- Panda Cub
    19054, -- Red Dragon Orb
    19055, -- Green Dragon Orb
    54452, -- Ethereal Portal
    3386, -- Potion of Curing
    6048, -- Shadow Protection Potion
    3388, -- Strong Troll's Blood Elixir
    45621, -- Elixir of Minor Accuracy

    -- Classic Loot Box
    46779, -- Path of Cenarius
    19160, -- Contest Winner's Tabard
    13446, -- Major Healing Potion
    13444, -- Major Mana Potion
    49704, -- Carved Ogre Idol
    39656, -- Mini Tyrael
    18964, -- Loggerhead Snapjaw
    33223, -- Fishing Chair
    49283, -- Reins of the Spectral Tiger
    13462, -- Purification Potion
    13584, -- Mini Diablo
    45047, -- Sandbox Tiger
    3387, -- Limited Invulnerability Potion
    44819, -- Baby Blizzard Bear
    45063, -- Foam Sword Rack
    13459, -- Greater Shadow Protection Potion
    49343, -- Spectral Tiger Cub
    37719, -- Swift Zhevra
    45037, -- Epic Purple Shirt
    13457, -- Greater Fire Protection Potion
    32588, -- Bananas
    13461, -- Greater Arcane Protection Potion
    20371, -- Blue Murloc Egg (Murky)
    30360, -- Lurky's Egg (Lurky)
    13458, -- Greater Nature Protection Potion
    49290, -- Magic Rooster Egg
    13456, -- Greater Frost Protection Potion
    34519, -- Silver Pig
    34518 -- Golden Pig
);

UPDATE `item_template` SET `spellcharges_1` = -1 WHERE `entry` IN (
    -- Starter Loot Box
    13583, -- Panda Cub
    19054, -- Red Dragon Orb
    19055, -- Green Dragon Orb

    -- Classic Loot Box
    39656, -- Mini Tyrael
    18964, -- Loggerhead Snapjaw
    13584, -- Mini Diablo
    44819, -- Baby Blizzard Bear
    49343, -- Spectral Tiger Cub
    32588, -- Bananas
    20371, -- Blue Murloc Egg (Murky)
    30360, -- Lurky's Egg (Lurky)
    34519, -- Silver Pig
    34518 -- Golden Pig
);

UPDATE `item_template` SET `stackable` = 250 WHERE `entry` IN (
    43489 -- Mohawk Grenade
);

DELETE FROM `creature_model_info` WHERE `DisplayID` = 7359065;
INSERT INTO `creature_model_info` (`DisplayID`, `CombatReach`, `Gender`)
VALUES (7359065, 1.5, 1);

DELETE FROM `creature_template` WHERE `entry` = 5126979;
INSERT INTO `creature_template` (
    `entry`, `modelid1`, `name`, `subname`, `gossip_menu_id`, `minlevel`,
    `maxlevel`, `faction`, `npcflag`, `DamageModifier`, `BaseAttackTime`,
    `RangeAttackTime`, `unit_class`, `unit_flags`, `unit_flags2`, `type`,
    `flags_extra`, `ScriptName`, `VerifiedBuild`
) VALUES (
    5126979, 11689, 'Winzig', 'Wobbling Goblin', 0, 60, 60, 35, 129, 1,
    2000, 2000, 4, 768, 2048, 7, 2, 'npc_winzig', 12340
);
-- Replace Landro Longshot.
DELETE FROM `creature` WHERE `guid` = 565;
INSERT INTO `creature` (
    `guid`, `id1`, `equipment_id`, `position_x`, `position_y`,
    `position_z`, `orientation`, `spawntimesecs`, `wander_distance`,
    `curhealth`
) VALUES (
    565, 5126979, 0, -14397.5, 426.08, 7.97355, 5.51524, 300, 0, 4979
);
-- Rename `subname` of the two guards.
UPDATE `creature_template` SET `subname` = 'Wobbling Goblin' WHERE `entry` = 21045;

-- Populate Winzig's stock.
DELETE FROM `npc_vendor` WHERE `entry` = 5126979;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `ExtendedCost`)
VALUES
    (5126979, 0, 5621799, 3870077), -- Loot Box
    (5126979, 1, 35223, 3870076), -- Papa Hummel's Old-Fashioned Pet Biscuit
    (5126979, 2, 2251094, 0), -- Reagent Pouch (Gold)
    (5126979, 3, 2251094, 3870079); -- Reagent Pouch (Macaroons)

