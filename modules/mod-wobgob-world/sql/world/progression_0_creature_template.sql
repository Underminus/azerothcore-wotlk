-- Disable NPC vendor of Arena Season 4
UPDATE `creature_template` SET `npcflag`=`npcflag`&~(128) WHERE `entry` = 28225;

-- Disable NPCs vendor of Arena Season 7
UPDATE `creature_template` SET `npcflag`=`npcflag`&~(128) WHERE `entry` IN (34093, 33939, 33935, 33929);

-- Remove endgame PvP vendors (level 70 and level 80)
-- Alliance: 12778, 34075 34078, 34081, 34084
-- Horde: 12788, 34043, 34038, 34060, 34063
UPDATE `creature_template` SET `npcflag` = `npcflag` &~128 WHERE `entry` IN (12778, 12788, 34075, 34078, 34081, 34084, 34043, 34038, 34060, 34063);
UPDATE `creature_template` SET `npcflag` = `npcflag` &~4096 WHERE `entry` IN (12778, 12788, 34075, 34078, 34081, 34084, 34043, 34038, 34060, 34063);

-- Remove level 70 and level 80 items from <Accessories Quartermaster> (12781, 12793)
DELETE FROM `npc_vendor` WHERE (`entry` IN (12781, 12793))
    AND (`item` IN (SELECT `item_template`.`entry` FROM `item_template`
        WHERE (`item_template`.`RequiredLevel` > 60 OR `item_template`.`class` = 3) AND `item_template`.`entry` = `npc_vendor`.`item`));
