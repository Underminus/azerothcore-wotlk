-- Kwixal variables
set @kwixal_entry_id = 5126981;
set @kwixal_guid = 564;
set @kwixal_display_id = 7113; 

-- Set spawn variables
set @map = 0;
set @zone_id = 33;
set @area_id = 35;
set @position_x = -14398.4;
set @position_y = 424.36;
set @position_z = 8.02625;
set @orientation = 0.05236;

-- Create Kwixal
DELETE FROM `creature_template` WHERE `entry` = @kwixal_entry_id;
INSERT INTO `creature_template` (
    `entry`, `modelid1`, `name`, `subname`, `gossip_menu_id`, `minlevel`,
    `maxlevel`, `faction`, `npcflag`, `scale`, `DamageModifier`, `BaseAttackTime`,
    `RangeAttackTime`, `unit_class`, `unit_flags`, `unit_flags2`, `type`,
    `flags_extra`, `ScriptName`, `VerifiedBuild`
) VALUES (
    @kwixal_entry_id, @kwixal_display_id, 'Kwixal', 'Wobblin Goblin', 0, 60, 60, 35, 1, 0.7, 1,
    2000, 2000, 4, 768, 2048, 7, 2, 'npc_transmogrifier', 12340
);

-- Replace one of Winzig's bodyguards with Kwixal
DELETE FROM `creature` WHERE `guid` = @kwixal_guid;
INSERT INTO `creature` (
    `guid`, `id1`, `map`, `zoneId`, `areaId`, `equipment_id`,
    `position_x`, `position_y`, `position_z`, `orientation`,
    `spawntimesecs`, `wander_distance`, `curhealth`
) VALUES (
    @kwixal_guid, @kwixal_entry_id, @map, @zone_id, @area_id, 1, @position_x, @position_y, 
    @position_z, @orientation, 300, 0, 4979
);