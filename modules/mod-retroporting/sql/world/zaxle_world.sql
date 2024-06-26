-- Zaxle variables
set @zaxle_entry_id = 5126980;
set @zaxle_guid = 563;
set @zaxle_display_id = 7112; 

-- Set spawn variables
set @map = 0;
set @zone_id = 33;
set @area_id = 35;
set @position_x = -14395.200195;
set @position_y = 426.953003;
set @position_z = 7.776840;
set @orientation = 5.20108;

-- Set base id's
set @weapon_base = 70000;
set @gear_base = 80000;

-- Set extended cost variables (weapons)
set @1h = 5003;
set @2h = 5002;
set @off_hand = 5000;
set @shield = 5001;
set @ranged = 5004;

-- Set extended cost variables (armor)
set @slot_wrist = 6000;
set @slot_waist = 6001;
set @slot_feet = 6002;
set @slot_shoulders = 6003;
set @slot_hands = 6004; 
set @slot_chest = 6005;
set @slot_legs = 6006;
set @slot_head = 6007;


-- Create Zaxle
DELETE FROM `creature_template` WHERE `entry` = @zaxle_entry_id;
INSERT INTO `creature_template` (
    `entry`, `name`, `subname`, `gossip_menu_id`, `minlevel`,
    `maxlevel`, `faction`, `npcflag`, `DamageModifier`, `BaseAttackTime`,
    `RangeAttackTime`, `unit_class`, `unit_flags`, `unit_flags2`, `type`,
    `flags_extra`, `ScriptName`, `VerifiedBuild`
) VALUES (
    @zaxle_entry_id, 'Zaxle', 'Wobbling Goblin', 0, 60, 60, 35, 129, 1,
    2000, 2000, 4, 768, 2048, 7, 2, '', 12340
);

-- Replace one of Winzig's bodyguards with Zaxle
DELETE FROM `creature` WHERE `guid` = @zaxle_guid;
INSERT INTO `creature` (
    `guid`, `id1`, `map`, `zoneId`, `areaId`, `equipment_id`,
    `position_x`, `position_y`, `position_z`, `orientation`,
    `spawntimesecs`, `wander_distance`, `curhealth`
) VALUES (
    @zaxle_guid, @zaxle_entry_id, @map, @zone_id, @area_id, 1, @position_x, @position_y, 
    @position_z, @orientation, 300, 0, 4979
);

-- Populate Zaxle's stock
DELETE FROM `npc_vendor` WHERE `entry` = @zaxle_entry_id;
INSERT INTO `npc_vendor` (`entry`, `item`, `ExtendedCost`)
VALUES
    -- Weapons
    (@zaxle_entry_id, @weapon_base, @2h), -- Decapitator
    (@zaxle_entry_id, @weapon_base+1, @2h), -- Greatsword
    (@zaxle_entry_id, @weapon_base+2, @2h), -- Bonegrinder
    (@zaxle_entry_id, @weapon_base+3, @1h), -- Shanker
    (@zaxle_entry_id, @weapon_base+4, @2h), -- Pike
    (@zaxle_entry_id, @weapon_base+5, @ranged), -- Longbow
    (@zaxle_entry_id, @weapon_base+6, @1h), -- Gavel
    (@zaxle_entry_id, @weapon_base+7, @1h), -- Slicer
    (@zaxle_entry_id, @weapon_base+8, @1h), -- Right Ripper
    (@zaxle_entry_id, @weapon_base+9, @off_hand), -- Left Ripper
    (@zaxle_entry_id, @weapon_base+10, @ranged), -- Rifle
    (@zaxle_entry_id, @weapon_base+11, @ranged), -- Heavy Crossbow
    (@zaxle_entry_id, @weapon_base+12, @ranged), -- Touch of Defeat
    (@zaxle_entry_id, @weapon_base+13, @1h), -- Cleaver
    (@zaxle_entry_id, @weapon_base+14, @2h), -- Staff
    (@zaxle_entry_id, @weapon_base+15, @shield), -- Shield Wall
    (@zaxle_entry_id, @weapon_base+16, @off_hand), -- Endgame

    -- Bloodthirsty Gladiator's Plate Set
    (@zaxle_entry_id, @gear_base, @slot_head),
    (@zaxle_entry_id, @gear_base+1, @slot_shoulders),
    (@zaxle_entry_id, @gear_base+2, @slot_chest),
    (@zaxle_entry_id, @gear_base+3, @slot_wrist),
    (@zaxle_entry_id, @gear_base+4, @slot_hands),
    (@zaxle_entry_id, @gear_base+5, @slot_waist),
    (@zaxle_entry_id, @gear_base+6, @slot_legs),
    (@zaxle_entry_id, @gear_base+7, @slot_feet),

    -- Bloodthirsty Gladiator's Mail Set
    (@zaxle_entry_id, @gear_base+8, @slot_head),
    (@zaxle_entry_id, @gear_base+9, @slot_shoulders),
    (@zaxle_entry_id, @gear_base+10, @slot_chest),
    (@zaxle_entry_id, @gear_base+11, @slot_wrist),
    (@zaxle_entry_id, @gear_base+12, @slot_hands),
    (@zaxle_entry_id, @gear_base+13, @slot_waist),
    (@zaxle_entry_id, @gear_base+14, @slot_legs),
    (@zaxle_entry_id, @gear_base+15, @slot_feet),

    -- Bloodthirsty Gladiator's Leather Set
    (@zaxle_entry_id, @gear_base+16, @slot_head),
    (@zaxle_entry_id, @gear_base+17, @slot_shoulders),
    (@zaxle_entry_id, @gear_base+18, @slot_chest),
    (@zaxle_entry_id, @gear_base+19, @slot_wrist),
    (@zaxle_entry_id, @gear_base+20, @slot_hands),
    (@zaxle_entry_id, @gear_base+21, @slot_waist),
    (@zaxle_entry_id, @gear_base+22, @slot_legs),
    (@zaxle_entry_id, @gear_base+23, @slot_feet),

    -- Bloodthirsty Gladiator's Satin Set
    (@zaxle_entry_id, @gear_base+24, @slot_head),
    (@zaxle_entry_id, @gear_base+25, @slot_shoulders),
    (@zaxle_entry_id, @gear_base+26, @slot_chest),
    (@zaxle_entry_id, @gear_base+27, @slot_chest),
    (@zaxle_entry_id, @gear_base+28, @slot_wrist),
    (@zaxle_entry_id, @gear_base+29, @slot_hands),
    (@zaxle_entry_id, @gear_base+30, @slot_waist),
    (@zaxle_entry_id, @gear_base+31, @slot_legs),
    (@zaxle_entry_id, @gear_base+32, @slot_feet),

    -- Bloodthirsty Gladiator's Felweave Set
    (@zaxle_entry_id, @gear_base+33, @slot_head),
    (@zaxle_entry_id, @gear_base+34, @slot_shoulders),
    (@zaxle_entry_id, @gear_base+35, @slot_chest),
    (@zaxle_entry_id, @gear_base+36, @slot_chest),
    (@zaxle_entry_id, @gear_base+37, @slot_wrist),
    (@zaxle_entry_id, @gear_base+38, @slot_hands),
    (@zaxle_entry_id, @gear_base+39, @slot_waist),
    (@zaxle_entry_id, @gear_base+40, @slot_legs),
    (@zaxle_entry_id, @gear_base+41, @slot_feet),

    -- Bloodthirsty Gladiator's Dreadplate Set
    (@zaxle_entry_id, @gear_base+42, @slot_head),
    (@zaxle_entry_id, @gear_base+43, @slot_shoulders),
    (@zaxle_entry_id, @gear_base+44, @slot_chest),
    (@zaxle_entry_id, @gear_base+45, @slot_wrist),
    (@zaxle_entry_id, @gear_base+46, @slot_hands),
    (@zaxle_entry_id, @gear_base+47, @slot_waist),
    (@zaxle_entry_id, @gear_base+48, @slot_legs),
    (@zaxle_entry_id, @gear_base+49, @slot_feet),

    -- Bloodthirsty Gladiator's Dragonhide Set
    (@zaxle_entry_id, @gear_base+50, @slot_head),
    (@zaxle_entry_id, @gear_base+51, @slot_shoulders),
    (@zaxle_entry_id, @gear_base+52, @slot_chest),
    (@zaxle_entry_id, @gear_base+53, @slot_chest),
    (@zaxle_entry_id, @gear_base+54, @slot_wrist),
    (@zaxle_entry_id, @gear_base+55, @slot_hands),
    (@zaxle_entry_id, @gear_base+56, @slot_waist),
    (@zaxle_entry_id, @gear_base+57, @slot_legs),
    (@zaxle_entry_id, @gear_base+58, @slot_feet),

    -- Bloodthirsty Gladiator's Silk Set
    (@zaxle_entry_id, @gear_base+59, @slot_head),
    (@zaxle_entry_id, @gear_base+60, @slot_shoulders),
    (@zaxle_entry_id, @gear_base+61, @slot_chest),
    (@zaxle_entry_id, @gear_base+62, @slot_chest),
    (@zaxle_entry_id, @gear_base+63, @slot_wrist),
    (@zaxle_entry_id, @gear_base+64, @slot_hands),
    (@zaxle_entry_id, @gear_base+65, @slot_waist),
    (@zaxle_entry_id, @gear_base+66, @slot_legs),
    (@zaxle_entry_id, @gear_base+67, @slot_feet),

    -- Bloodthirsty Gladiator's Mail Set
    (@zaxle_entry_id, @gear_base+68, @slot_head),
    (@zaxle_entry_id, @gear_base+69, @slot_shoulders),
    (@zaxle_entry_id, @gear_base+70, @slot_chest),
    (@zaxle_entry_id, @gear_base+71, @slot_wrist),
    (@zaxle_entry_id, @gear_base+72, @slot_hands),
    (@zaxle_entry_id, @gear_base+73, @slot_waist),
    (@zaxle_entry_id, @gear_base+74, @slot_legs),
    (@zaxle_entry_id, @gear_base+75, @slot_feet),

    -- Bloodthirsty Gladiator's Scaled Set
    (@zaxle_entry_id, @gear_base+76, @slot_head),
    (@zaxle_entry_id, @gear_base+77, @slot_shoulders),
    (@zaxle_entry_id, @gear_base+78, @slot_chest),
    (@zaxle_entry_id, @gear_base+79, @slot_wrist),
    (@zaxle_entry_id, @gear_base+80, @slot_hands),
    (@zaxle_entry_id, @gear_base+81, @slot_waist),
    (@zaxle_entry_id, @gear_base+82, @slot_legs),
    (@zaxle_entry_id, @gear_base+83, @slot_feet);