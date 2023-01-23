-- class
set @class_container = 0;
set @class_weapon = 2;
set @class_armor = 4;

-- class id
set @class_warrior = 1;
set @class_paladin = 2;
set @class_hunter = 3;
set @class_rogue = 4;
set @class_priest = 5;
set @class_shaman = 7;
set @class_mage = 8;
set @class_warlock = 9;
set @class_druid = 11;

-- subclass weapon
set @subclass_axe_one_hand = 0;
set @subclass_axe_two_hand = 1;
set @subclass_bow = 2;
set @subclass_gun = 3;
set @subclass_mace_one_hand = 4;
set @sublcass_mace_two_hand = 5;
set @subclass_polearm = 6;
set @subclass_sword_one_hand = 7;
set @subclass_sword_two_hand = 8;
set @subclass_staff = 10;
set @subclass_dagger = 15;
set @subclass_crossbow = 18;
set @subclass_wand = 19;

-- sublcass armor
set @subclass_miscellaneous = 0;
set @subclass_cloth = 1;
set @subclass_leather = 2;
set @subclass_mail = 3;
set @subclass_shield = 6;

-- inventorytype
set @inventorytype_non_equip = 0;
set @inventorytype_shoulders = 3;
set @inventorytype_chest = 5;
set @inventorytype_waist = 6;
set @inventorytype_legs = 7;
set @inventorytype_feet = 8;
set @inventorytype_wrists = 9;
set @inventorytype_hands = 10;
set @inventorytype_one_hand = 13;
set @inventorytype_shield = 14;
set @inventorytype_bow = 15;
set @inventorytype_two_hand = 17;
set @inventorytype_tabard = 19;
set @inventorytype_robe = 20;
set @inventorytype_main_hand = 21;
set @inventorytype_off_hand = 22;
set @inventorytype_holdable = 23;
set @inventorytype_ranged = 26;

-- sheath 
set @sheath_ranged_and_frill = 0;
set @sheath_two_hand_weapon = 1;
set @sheath_staff = 2;
set @sheath_one_handed = 3;
set @sheath_shield = 4;
set @sheath_off_hand = 6;

-- material
set @material_metal = 1;
set @material_wood = 2;
set @material_liquid = 3;
set @material_chain = 5;
set @material_cloth = 7;
set @material_leather = 8;

-- stats
set @agility = 3;
set @strength = 4;
set @intellect = 5;
set @spirit = 6;
set @stamina = 7;
set @crit_rating = 32;
set @attack_power = 38;
set @block_value = 48;
set @armor_weapon = 0;
set @armor_mail_shoulders = 138;
set @armor_mail_chest = 184;
set @armor_mail_bracers = 72;
set @armor_mail_gloves = 105;
set @armor_mail_belt = 94;
set @armor_mail_legs = 207;
set @armor_mail_boots = 115;
set @armor_leather_shoulders = 64;
set @armor_leather_chest = 85;
set @armor_leather_bracers = 34;
set @armor_leather_gloves = 48;
set @armor_leather_belt = 44;
set @armor_leather_legs = 85;
set @armor_leather_boots = 53;
set @armor_cloth_shoulders = 28;
set @armor_cloth_chest = 37;
set @armor_cloth_bracers = 15;
set @armor_cloth_gloves = 23;
set @armor_cloth_belt = 19;
set @armor_cloth_legs = 55;
set @armor_cloth_boots = 23;
set @stat_count_armor = 3;
set @stat_count_weapon = 2;

-- armor/weapon
set @dmg_type1 = 0;
set @itemlevel = 20;
set @dmg_type1_arcane = 6;
set @ammo_type_arrow= 2;
set @ammo_type_bullet = 3;
set @ammo_type_default = 0;
set @block_default = 0;
set @rangedmodrange_default = 0;
set @rangedmodrange = 100;
set @flags_default = 32768;
set @max_durability = 75;
set @min_count_1 = 1;
set @max_count_1 = 1;
set @min_count_2 = 2;
set @max_count_2 = 2;

-- container
set @flags_container = 32804;
set @bind_on_pickup = 1;
set @chance = 100;
set @all_races = 0;
set @min_count = 1;
set @max_count = 1;
set @quest_required = 0;
set @loot_mode = 1;
set @group_id = 0;
set @reference = 0;
set @comment = "";
set @container_description = "Includes everything that a new recruit needs to get started";
set @container_name = "Initiate's Supplies";

-- other
set @quality = 2;
set @sell_price = 0;

-- base id
set @base_item_container = 90000;
set @base_item_armor = 91000;
set @base_item_weapon = 92000;

DELIMITER //

DROP PROCEDURE IF EXISTS `create_item_loot_template` //
CREATE PROCEDURE create_item_loot_template(IN entry MEDIUMINT(7), IN item MEDIUMINT(7), IN reference MEDIUMINT(7), IN chance FLOAT, IN questrequired TINYINT(3), IN lootmode SMALLINT(5), IN groupid TINYINT(3), IN mincount TINYINT(3), IN maxcount TINYINT(3), IN comment VARCHAR(255))
BEGIN
    REPLACE INTO item_loot_template(entry, item, reference, chance, questrequired, lootmode, groupid, mincount, maxcount, comment) VALUES
    (entry, item, reference, chance, questrequired, lootmode, groupid, mincount, maxcount, comment);
END//

DROP PROCEDURE IF EXISTS `create_container_loot` //
CREATE PROCEDURE create_container_loot(IN entry MEDIUMINT(7), IN item MEDIUMINT(7), IN mincount TINYINT(3), IN maxcount TINYINT(3))
BEGIN
    CALL create_item_loot_template(entry, item, @reference, @chance, @quest_required, @loot_mode, @group_id, mincount, maxcount, @comment);
END//

DROP PROCEDURE IF EXISTS `create_item_template` //
CREATE PROCEDURE create_item_template(IN entry MEDIUMINT(7), IN displayid MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN class TINYINT(3), IN subclass TINYINT(3), IN inventorytype TINYINT(3), IN quality TINYINT(3), IN flags INT(10), IN material TINYINT(10), IN bonding TINYINT(3), IN sellprice INT(10))
BEGIN
    REPLACE INTO item_template(entry, displayid, name, description, class, subclass, inventorytype, quality, flags, material, bonding, sellprice) VALUES
    (entry, displayid, name, description, class, subclass, inventorytype, quality, flags, material, bonding, sellprice);
END//

DROP PROCEDURE IF EXISTS `create_item_container` //
CREATE PROCEDURE create_item_container(IN entry MEDIUMINT(70), IN displayid MEDIUMINT(7))
BEGIN
    CALL create_item_template(entry, displayid, @container_name, @container_description, @class_container, @subclass_miscellaneous, @inventorytype_non_equip, @quality, @flags_container, @material_wood, @bind_on_pickup, @sell_price);
END//

DROP PROCEDURE IF EXISTS `create_armor` //
CREATE PROCEDURE create_armor(IN entry MEDIUMINT(7), IN name VARCHAR(255),  IN description VARCHAR(255), IN class TINYINT(3), IN subclass TINYINT(3), IN displayid MEDIUMINT(7), IN inventorytype TINYINT(3), IN quality TINYINT(3), IN itemlevel SMALLINT(5), IN flags INT(10), IN material TINYINT(10), IN bonding TINYINT(3), IN sellprice INT(10), IN statscount TINYINT(3), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN stat_type2 TINYINT(3), in stat_value2 SMALLINT(5), IN stat_type3 TINYINT(3), IN stat_value3 SMALLINT(5), IN armor SMALLINT(5), IN maxdurability SMALLINT(5)) 
BEGIN
    REPLACE INTO item_template(entry, name, description, class, subclass, displayid, inventorytype, quality, itemlevel, flags, material, bonding, sellprice, statscount, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3, armor, maxdurability) VALUES
      (entry, name, description, class, subclass, displayid, inventorytype, quality, itemlevel, flags, material, bonding, sellprice, statscount, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3, armor, maxdurability);
END//

DROP PROCEDURE IF EXISTS `create_shoulders` //
CREATE PROCEDURE create_shoulders(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN subclass TINYINT(3), IN displayid MEDIUMINT(7), IN flags INT(10), IN material TINYINT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN stat_type2 TINYINT(3), in stat_value2 SMALLINT(5), IN stat_type3 TINYINT(3), IN stat_value3 SMALLINT(5), IN armor SMALLINT(5))
BEGIN
    CALL create_armor(entry, name, description, @class_armor, subclass, displayid, @inventorytype_shoulders, @quality, @itemlevel, flags, material, @bind_on_pickup, @sell_price, @stat_count_armor, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3, armor, @max_durability);
END//

DROP PROCEDURE IF EXISTS `create_robe` //
CREATE PROCEDURE create_robe(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN subclass TINYINT(3), IN displayid MEDIUMINT(7), IN flags INT(10), IN material TINYINT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN stat_type2 TINYINT(3), in stat_value2 SMALLINT(5), IN stat_type3 TINYINT(3), IN stat_value3 SMALLINT(5), IN armor SMALLINT(5))
BEGIN
    CALL create_armor(entry, name, description, @class_armor, subclass, displayid, @inventorytype_robe, @quality, @itemlevel, flags, material, @bind_on_pickup, @sell_price, @stat_count_armor, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3, armor, @max_durability);
END//

DROP PROCEDURE IF EXISTS `create_chest` //
CREATE PROCEDURE create_chest(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN subclass TINYINT(3), IN displayid MEDIUMINT(7), IN flags INT(10), IN material TINYINT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN stat_type2 TINYINT(3), in stat_value2 SMALLINT(5), IN stat_type3 TINYINT(3), IN stat_value3 SMALLINT(5), IN armor SMALLINT(5))
BEGIN
    CALL create_armor(entry, name, description, @class_armor, subclass, displayid, @inventorytype_chest, @quality, @itemlevel, flags, material, @bind_on_pickup, @sell_price, @stat_count_armor, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3, armor, @max_durability);
END//

DROP PROCEDURE IF EXISTS `create_bracers` //
CREATE PROCEDURE create_bracers(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN subclass TINYINT(3), IN displayid MEDIUMINT(7), IN flags INT(10), IN material TINYINT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN stat_type2 TINYINT(3), in stat_value2 SMALLINT(5), IN stat_type3 TINYINT(3), IN stat_value3 SMALLINT(5), IN armor SMALLINT(5))
BEGIN
    CALL create_armor(entry, name, description, @class_armor, subclass, displayid, @inventorytype_wrists, @quality, @itemlevel, flags, material, @bind_on_pickup, @sell_price, @stat_count_armor, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3, armor, @max_durability);
END//

DROP PROCEDURE IF EXISTS `create_gloves` //
CREATE PROCEDURE create_gloves(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN subclass TINYINT(3), IN displayid MEDIUMINT(7), IN flags INT(10), IN material TINYINT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN stat_type2 TINYINT(3), in stat_value2 SMALLINT(5), IN stat_type3 TINYINT(3), IN stat_value3 SMALLINT(5), IN armor SMALLINT(5))
BEGIN
    CALL create_armor(entry, name, description, @class_armor, subclass, displayid, @inventorytype_hands, @quality, @itemlevel, flags, material, @bind_on_pickup, @sell_price, @stat_count_armor, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3, armor, @max_durability);
END//

DROP PROCEDURE IF EXISTS `create_belt` //
CREATE PROCEDURE create_belt(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN subclass TINYINT(3), IN displayid MEDIUMINT(7), IN flags INT(10), IN material TINYINT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN stat_type2 TINYINT(3), in stat_value2 SMALLINT(5), IN stat_type3 TINYINT(3), IN stat_value3 SMALLINT(5), IN armor SMALLINT(5))
BEGIN
    CALL create_armor(entry, name, description, @class_armor, subclass, displayid, @inventorytype_waist, @quality, @itemlevel, flags, material, @bind_on_pickup, @sell_price, @stat_count_armor, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3, armor, @max_durability);
END//

DROP PROCEDURE IF EXISTS `create_legs` //
CREATE PROCEDURE create_legs(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN subclass TINYINT(3), IN displayid MEDIUMINT(7), IN flags INT(10), IN material TINYINT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN stat_type2 TINYINT(3), in stat_value2 SMALLINT(5), IN stat_type3 TINYINT(3), IN stat_value3 SMALLINT(5), IN armor SMALLINT(5))
BEGIN
    CALL create_armor(entry, name, description, @class_armor, subclass, displayid, @inventorytype_legs, @quality, @itemlevel, flags, material, @bind_on_pickup, @sell_price, @stat_count_armor, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3, armor, @max_durability);
END//

DROP PROCEDURE IF EXISTS `create_boots` //
CREATE PROCEDURE create_boots(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN subclass TINYINT(3), IN displayid MEDIUMINT(7), IN flags INT(10), IN material TINYINT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN stat_type2 TINYINT(3), in stat_value2 SMALLINT(5), IN stat_type3 TINYINT(3), IN stat_value3 SMALLINT(5), IN armor SMALLINT(5))
BEGIN
    CALL create_armor(entry, name, description, @class_armor, subclass, displayid, @inventorytype_feet, @quality, @itemlevel, flags, material, @bind_on_pickup, @sell_price, @stat_count_armor, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3, armor, @max_durability);
END//

DROP PROCEDURE IF EXISTS `create_weapon` //
CREATE PROCEDURE create_weapon(IN entry MEDIUMINT(7), IN name VARCHAR(255),  IN description VARCHAR(255), IN class TINYINT(3), IN subclass TINYINT(3), IN displayid MEDIUMINT(7), IN inventorytype TINYINT(3), IN sheath TINYINT(3), IN quality TINYINT(3), IN itemlevel SMALLINT(5), IN flags INT(10), IN material TINYINT(10), IN bonding TINYINT(3), IN sellprice INT(10), IN statscount TINYINT(3), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN armor SMALLINT(5), IN dmg_type1 TINYINT(3), IN ammo_type SMALLINT(5), IN rangedmodrange SMALLINT(5), IN dmg_min1 FLOAT, IN dmg_max1 FLOAT, IN delay SMALLINT(5), IN block MEDIUMINT(7), IN maxdurability SMALLINT(5), IN spellid_1 MEDIUMINT(7), IN spelltrigger_1 TINYINT(3))
BEGIN
    REPLACE INTO item_template(entry, name, description, class, subclass, displayid, inventorytype, sheath, quality, itemlevel, flags, material, bonding, sellprice, statscount, stat_type1, stat_value1, armor, dmg_type1, dmg_min1, dmg_max1, delay, block, maxdurability, spellid_1, spelltrigger_1, ammo_type, rangedmodrange) VALUES
      (entry, name, description, class, subclass, displayid, inventorytype, sheath, quality, itemlevel, flags, material, bonding, sellprice, statscount, stat_type1, stat_value1, armor, dmg_type1, dmg_min1, dmg_max1, delay, block, maxdurability, spellid_1, spelltrigger_1, ammo_type, rangedmodrange);
END//

DROP PROCEDURE IF EXISTS `create_two_hand_axe` //
CREATE PROCEDURE create_two_hand_axe(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN displayid MEDIUMINT(7), IN flags INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN dmg_min1 FLOAT, IN dmg_max1 FLOAT, IN delay SMALLINT(5), IN maxdurability SMALLINT(5), IN spellid_1 MEDIUMINT(7), IN spelltrigger_1 TINYINT(3))
BEGIN
    CALL create_weapon(entry, name, description, @class_weapon, @subclass_axe_two_hand, displayid, @inventorytype_two_hand, @sheath_two_hand_weapon, @quality, @itemlevel, flags, @material_metal, @bind_on_pickup, @sell_price, @stat_count_weapon, stat_type1, stat_value1, @armor_weapon, @dmg_type1, @ammo_type_default, @rangedmodrange_default, dmg_min1, dmg_max1, delay, @block_default, maxdurability, spellid_1, spelltrigger_1);
END//

DROP PROCEDURE IF EXISTS `create_two_hand_mace` //
CREATE PROCEDURE create_two_hand_mace(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN displayid MEDIUMINT(7), IN flags INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN dmg_min1 FLOAT, IN dmg_max1 FLOAT, IN delay SMALLINT(5), IN maxdurability SMALLINT(5), IN spellid_1 MEDIUMINT(7), IN spelltrigger_1 TINYINT(3))
BEGIN
    CALL create_weapon(entry, name, description, @class_weapon, @sublcass_mace_two_hand, displayid, @inventorytype_two_hand, @sheath_two_hand_weapon, @quality, @itemlevel, flags, @material_metal, @bind_on_pickup, @sell_price, @stat_count_weapon, stat_type1, stat_value1, @armor_weapon, @dmg_type1, @ammo_type_default, @rangedmodrange_default, dmg_min1, dmg_max1, delay, @block_default, maxdurability, spellid_1, spelltrigger_1);
END//

DROP PROCEDURE IF EXISTS `create_two_hand_sword` //
CREATE PROCEDURE create_two_hand_sword(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN displayid MEDIUMINT(7), IN flags INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN dmg_min1 FLOAT, IN dmg_max1 FLOAT, IN delay SMALLINT(5), IN maxdurability SMALLINT(5), IN spellid_1 MEDIUMINT(7), IN spelltrigger_1 TINYINT(3))
BEGIN
    CALL create_weapon(entry, name, description, @class_weapon, @subclass_sword_two_hand, displayid, @inventorytype_two_hand, @sheath_two_hand_weapon, @quality, @itemlevel, flags, @material_metal, @bind_on_pickup, @sell_price, @stat_count_weapon, stat_type1, stat_value1, @armor_weapon, @dmg_type1, @ammo_type_default, @rangedmodrange_default, dmg_min1, dmg_max1, delay, @block_default, maxdurability, spellid_1, spelltrigger_1);
END//

DROP PROCEDURE IF EXISTS `create_one_hand_dagger` //
CREATE PROCEDURE create_one_hand_dagger(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN displayid MEDIUMINT(7), IN flags INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN dmg_min1 FLOAT, IN dmg_max1 FLOAT, IN delay SMALLINT(5), IN maxdurability SMALLINT(5), IN spellid_1 MEDIUMINT(7), IN spelltrigger_1 TINYINT(3))
BEGIN
    CALL create_weapon(entry, name, description, @class_weapon, @subclass_dagger, displayid, @inventorytype_one_hand, @sheath_one_handed, @quality, @itemlevel, flags, @material_metal, @bind_on_pickup, @sell_price, @stat_count_weapon, stat_type1, stat_value1, @armor_weapon, @dmg_type1, @ammo_type_default, @rangedmodrange_default, dmg_min1, dmg_max1, delay, @block_default, maxdurability, spellid_1, spelltrigger_1);
END//

DROP PROCEDURE IF EXISTS `create_one_hand_mace` //
CREATE PROCEDURE create_one_hand_mace(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN displayid MEDIUMINT(7), IN flags INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN dmg_min1 FLOAT, IN dmg_max1 FLOAT, IN delay SMALLINT(5), IN maxdurability SMALLINT(5), IN spellid_1 MEDIUMINT(7), IN spelltrigger_1 TINYINT(3))
BEGIN
    CALL create_weapon(entry, name, description, @class_weapon, @subclass_mace_one_hand, displayid, @inventorytype_one_hand, @sheath_one_handed, @quality, @itemlevel, flags, @material_metal, @bind_on_pickup, @sell_price, @stat_count_weapon, stat_type1, stat_value1, @armor_weapon, @dmg_type1, @ammo_type_default, @rangedmodrange_default, dmg_min1, dmg_max1, delay, @block_default, maxdurability, spellid_1, spelltrigger_1);
END//

DROP PROCEDURE IF EXISTS `create_one_hand_sword` //
CREATE PROCEDURE create_one_hand_sword(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN displayid MEDIUMINT(7), IN flags INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN dmg_min1 FLOAT, IN dmg_max1 FLOAT, IN delay SMALLINT(5), IN maxdurability SMALLINT(5), IN spellid_1 MEDIUMINT(7), IN spelltrigger_1 TINYINT(3))
BEGIN
    CALL create_weapon(entry, name, description, @class_weapon, @subclass_sword_one_hand, displayid, @inventorytype_one_hand, @sheath_one_handed, @quality, @itemlevel, flags, @material_metal, @bind_on_pickup, @sell_price, @stat_count_weapon, stat_type1, stat_value1, @armor_weapon, @dmg_type1, @ammo_type_default, @rangedmodrange_default, dmg_min1, dmg_max1, delay, @block_default, maxdurability, spellid_1, spelltrigger_1);
END//

DROP PROCEDURE IF EXISTS `create_one_hand_axe` //
CREATE PROCEDURE create_one_hand_axe(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN displayid MEDIUMINT(7), IN flags INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN dmg_min1 FLOAT, IN dmg_max1 FLOAT, IN delay SMALLINT(5), IN maxdurability SMALLINT(5), IN spellid_1 MEDIUMINT(7), IN spelltrigger_1 TINYINT(3))
BEGIN
    CALL create_weapon(entry, name, description, @class_weapon, @subclass_axe_one_hand, displayid, @inventorytype_one_hand, @sheath_one_handed, @quality, @itemlevel, flags, @material_metal, @bind_on_pickup, @sell_price, @stat_count_weapon, stat_type1, stat_value1, @armor_weapon, @dmg_type1, @ammo_type_default, @rangedmodrange_default, dmg_min1, dmg_max1, delay, @block_default, maxdurability, spellid_1, spelltrigger_1);
END//

DROP PROCEDURE IF EXISTS `create_staff` //
CREATE PROCEDURE create_staff (IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN displayid MEDIUMINT(7), IN flags INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN dmg_min1 FLOAT, IN dmg_max1 FLOAT, IN delay SMALLINT(5), IN maxdurability SMALLINT(5), IN spellid_1 MEDIUMINT(7), IN spelltrigger_1 TINYINT(3))
BEGIN
    CALL create_weapon(entry, name, description, @class_weapon, @subclass_staff, displayid, @inventorytype_two_hand, @sheath_staff, @quality, @itemlevel, flags, @material_metal, @bind_on_pickup, @sell_price, @stat_count_weapon, stat_type1, stat_value1, @armor_weapon, @dmg_type1, @ammo_type_default, @rangedmodrange_default, dmg_min1, dmg_max1, delay, @block_default, maxdurability, spellid_1, spelltrigger_1);
END//

DROP PROCEDURE IF EXISTS `create_wand` //
CREATE PROCEDURE create_wand(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN displayid MEDIUMINT(7), IN flags INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN dmg_min1 FLOAT, IN dmg_max1 FLOAT, IN delay SMALLINT(5), IN maxdurability SMALLINT(5), IN spellid_1 MEDIUMINT(7), IN spelltrigger_1 TINYINT(3))
BEGIN
    CALL create_weapon(entry, name, description, @class_weapon, @subclass_wand, displayid, @inventorytype_ranged, @sheath_ranged_and_frill, @quality, @itemlevel, flags, @material_wood, @bind_on_pickup, @sell_price, @stat_count_weapon, stat_type1, stat_value1, @armor_weapon, @dmg_type1_arcane, @ammo_type_default, @rangedmodrange, dmg_min1, dmg_max1, delay, @block_default, maxdurability, spellid_1, spelltrigger_1);
END//

DROP PROCEDURE IF EXISTS `create_gun` //
CREATE PROCEDURE create_gun(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN displayid MEDIUMINT(7), IN flags INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN dmg_min1 FLOAT, IN dmg_max1 FLOAT, IN delay SMALLINT(5), IN maxdurability SMALLINT(5), IN spellid_1 MEDIUMINT(7), IN spelltrigger_1 TINYINT(3))
BEGIN
    CALL create_weapon(entry, name, description, @class_weapon, @subclass_gun, displayid, @inventorytype_ranged, @sheath_ranged_and_frill, @quality, @itemlevel, flags, @material_metal, @bind_on_pickup, @sell_price, @stat_count_weapon, stat_type1, stat_value1, @armor_weapon, @dmg_type1, @ammo_type_bullet, @rangedmodrange, dmg_min1, dmg_max1, delay, @block_default, maxdurability, spellid_1, spelltrigger_1);
END//

DROP PROCEDURE IF EXISTS `create_bow` //
CREATE PROCEDURE create_bow(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN displayid MEDIUMINT(7), IN flags INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN dmg_min1 FLOAT, IN dmg_max1 FLOAT, IN delay SMALLINT(5), IN maxdurability SMALLINT(5), IN spellid_1 MEDIUMINT(7), IN spelltrigger_1 TINYINT(3))
BEGIN
    CALL create_weapon(entry, name, description, @class_weapon, @subclass_bow, displayid, @inventorytype_bow, @sheath_ranged_and_frill, @quality, @itemlevel, flags, @material_wood, @bind_on_pickup, @sell_price, @stat_count_weapon, stat_type1, stat_value1, @armor_weapon, @dmg_type1, @ammo_type_arrow, @rangedmodrange, dmg_min1, dmg_max1, delay, @block_default, maxdurability, spellid_1, spelltrigger_1);
END//

DROP PROCEDURE IF EXISTS `create_off_hand_weapon` //
CREATE PROCEDURE create_off_hand_weapon(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN displayid MEDIUMINT(7), IN flags INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN dmg_min1 FLOAT, IN dmg_max1 FLOAT, IN delay SMALLINT(5), IN maxdurability SMALLINT(5), IN spellid_1 MEDIUMINT(7), IN spelltrigger_1 TINYINT(3))
BEGIN
    CALL create_weapon(entry, name, description, @class_weapon, subclass, displayid, @inventorytype_off_hand, @sheath_one_handed, @quality, @itemlevel, flags, @material_metal, @bind_on_pickup, @bind_on_pickup, @sell_price, @stat_count_weapon, stat_type1, stat_value1, @armor_weapon, @dmg_type1, @ammo_type_default, @rangedmodrange_default, dmg_min1, dmg_max1, delay, @block_default, maxdurability, spellid_1, spelltrigger_1);
END//

DROP PROCEDURE IF EXISTS `create_shield` //
CREATE PROCEDURE create_shield(IN entry MEDIUMINT(7), IN name VARCHAR(255), IN description VARCHAR(255), IN displayid MEDIUMINT(7), IN flags INT(10), IN stat_type1 TINYINT(3), IN stat_value1 SMALLINT(5), IN armor SMALLINT(5), IN dmg_min1 FLOAT, IN dmg_max1 FLOAT, IN delay SMALLINT(5), IN block MEDIUMINT(7), IN maxdurability SMALLINT(5), IN spellid_1 MEDIUMINT(7), IN spelltrigger_1 TINYINT(3))
BEGIN
    CALL create_weapon(entry, name, description, @class_armor, @subclass_shield, displayid, @inventorytype_shield, @sheath_shield, @quality, @itemlevel, flags, @material_metal, @bind_on_pickup, @sell_price, @stat_count_weapon, stat_type1, stat_value1, armor, @dmg_type1, @ammo_type_default, @rangedmodrange_default, dmg_min1, dmg_max1, delay, block, maxdurability, spellid_1, spelltrigger_1);
END//

DELIMITER ;

-- Weapons
CALL create_two_hand_axe(@base_item_weapon, "Initiate's Battleaxe", '', @base_item_weapon, @flags_default, @crit_rating, 5, 51, 77, 3800, 75, 9136, 1);
CALL create_two_hand_sword(@base_item_weapon+1, "Initiate's Greatsword", '', @base_item_weapon+1, @flags_default, @crit_rating, 5, 51, 77, 3800, 75, 9136, 1);
call create_bow(@base_item_weapon+2, "Initiate's Shortbow", '', @base_item_weapon+2, @flags_default, @crit_rating, 1, 25, 47, 2800, 75, 9136, 1);
call create_one_hand_mace(@base_item_weapon+3, "Initiate's Mace", '', @base_item_weapon+3, @flags_default, @crit_rating, 5, 24, 45, 2700, 75, 9396, 1);
call create_one_hand_sword(@base_item_weapon+4, "Initiate's Shortsword", '', @base_item_weapon+4, @flags_default, @crit_rating, 2, 25, 47, 2800, 75, 9136, 1);
call create_wand(@base_item_weapon+5, "Initiate's Wand", '', @base_item_weapon+5, @flags_default, @crit_rating, 2, 25, 39, 1600, 75, 0, 0);
call create_one_hand_axe(@base_item_weapon+6, "Initiate's Axe", '', @base_item_weapon+6, @flags_default, @crit_rating, 2, 25, 47, 2800, 75, 9136, 1);
call create_staff(@base_item_weapon+7, "Initiate's Staff", '', @base_item_weapon+7, @flags_default, @crit_rating, 5, 39, 59, 2900, 75, 9417, 1);
call create_staff(@base_item_weapon+8, "Initiate's Battle Staff", '', @base_item_weapon+8, @flags_default, @crit_rating, 5, 39, 59, 2900, 75, 9136, 1);
call create_shield(@base_item_weapon+9, "Initiate's Wooden Shield", '', @base_item_weapon+9, @flags_default, @block_value, 0, 471, 0, 0, 0, 9, 75, 9396, 1);
call create_shield(@base_item_weapon+10, "Initiate's Wooden Buckler", '', @base_item_weapon+10, @flags_default, @block_value, 0, 471, 0, 0, 0, 9, 75, 7517, 1);
call create_gun(@base_item_weapon+11, "Initiate's Blunderbuss", '', @base_item_weapon+11, @flags_default, @crit_rating, 1, 25, 47, 2800, 75, 9136, 1);
call create_one_hand_dagger(@base_item_weapon+12, "Initiate's Dagger", '', @base_item_weapon+12, @flags_default, @crit_rating, 2, 15, 28, 1700, 75, 9136, 1);

/*
* Warrior
*/
call create_shoulders(@base_item_armor, "Initiate's Spaulders of Valor", '', @subclass_mail, @base_item_armor, @flags_default, @material_chain, @strength, 1, @agility, 1, @stamina, 2, @armor_mail_shoulders);
call create_chest(@base_item_armor+1, "Initiate's Brestplate of Valor", '', @subclass_mail, @base_item_armor+1, @flags_default, @material_chain, @strength, 2, @agility, 1, @stamina, 3, @armor_mail_chest);
call create_bracers(@base_item_armor+2, "Initiate's Bracers of Valor", '', @subclass_mail, @base_item_armor+2, @flags_default, @material_chain, @strength, 1, @agility, 1, @stamina, 2, @armor_mail_bracers);
call create_gloves(@base_item_armor+3, "Initiate's Gauntlets of Valor", '', @subclass_mail, @base_item_armor+3, @flags_default, @material_chain, @strength, 2, @agility, 1, @stamina, 1, @armor_mail_gloves);
call create_belt(@base_item_armor+4, "Initiate's Belt of Valor", '', @subclass_mail, @base_item_armor+4, @flags_default, @material_chain, @strength, 2, @agility, 1, @stamina, 1, @armor_mail_belt);
call create_legs(@base_item_armor+5, "Initiate's Legplates of Valor", '', @subclass_mail, @base_item_armor+5, @flags_default, @material_chain, @strength, 2, @agility, 1, @stamina, 2, @armor_mail_legs);
call create_boots(@base_item_armor+6, "Initiate's Boots of Valor", '', @subclass_mail, @base_item_armor+6, @flags_default, @material_chain, @strength, 1, @agility, 1, @stamina, 2, @armor_mail_boots);

/*
* Paladin
*/
call create_shoulders(@base_item_armor+7, "Initiate's Lightforge Spaulders", '', @subclass_mail, @base_item_armor+7, @flags_default, @material_chain, @strength, 1, @intellect, 2, @stamina, 2, @armor_mail_shoulders);
call create_chest(@base_item_armor+8, "Initiate's Lightforge Breastplate", '', @subclass_mail, @base_item_armor+8, @flags_default, @material_chain, @strength, 2, @intellect, 2, @stamina, 3, @armor_mail_chest);
call create_bracers(@base_item_armor+9, "Initiate's Lightforge Bracers", '', @subclass_mail, @base_item_armor+9, @flags_default, @material_chain, @strength, 1, @intellect, 0, @stamina, 1, @armor_mail_bracers);
call create_gloves(@base_item_armor+10, "Initiate's Lightforge Gauntlets", '', @subclass_mail, @base_item_armor+10, @flags_default, @material_chain, @strength, 2, @intellect, 0, @stamina, 1, @armor_mail_gloves);
call create_belt(@base_item_armor+11, "Initiate's Lightforge Belt", '', @subclass_mail, @base_item_armor+11, @flags_default, @material_chain, @strength, 1, @intellect, 2, @stamina, 1, @armor_mail_belt);
call create_legs(@base_item_armor+12, "Initiate's Lightforge Legplates", '', @subclass_mail, @base_item_armor+12, @flags_default, @material_chain, @strength, 2, @intellect, 2, @stamina, 2, @armor_mail_legs);
call create_boots(@base_item_armor+13, "Initiate's Lightforge Boots", '', @subclass_mail, @base_item_armor+13, @flags_default, @material_chain, @strength, 1, @intellect, 0, @stamina, 2, @armor_mail_boots);

/*
* Hunter
*/
call create_shoulders(@base_item_armor+14, "Initiate's Beaststalker Mantle", '', @subclass_leather, @base_item_armor+14, @flags_default, @material_leather, @agility, 3, @intellect, 0, @stamina, 1, @armor_leather_shoulders);
call create_chest(@base_item_armor+15, "Initiate's Beaststalker Tunic", '', @subclass_leather, @base_item_armor+15, @flags_default, @material_leather, @agility, 3, @intellect, 2, @stamina, 2, @armor_leather_chest);
call create_bracers(@base_item_armor+16, "Initiate's Beaststalker Bindings", '', @subclass_leather, @base_item_armor+16, @flags_default, @material_leather, @agility, 2, @intellect, 0, @stamina, 1, @armor_leather_bracers);
call create_gloves(@base_item_armor+17, "Initiate's Beaststalker Gloves", '', @subclass_leather, @base_item_armor+17, @flags_default, @material_leather, @agility, 2, @intellect, 1, @stamina, 1, @armor_leather_gloves);
call create_belt(@base_item_armor+18, "Initiate's Beaststalker Belt", '', @subclass_leather, @base_item_armor+18, @flags_default, @material_leather, @agility, 1, @intellect, 1, @stamina, 1, @armor_leather_belt);
call create_legs(@base_item_armor+19, "Initiate's Beaststalker Pants", '', @subclass_leather, @base_item_armor+19, @flags_default, @material_leather, @agility, 2, @intellect, 1, @stamina, 2, @armor_leather_legs);
call create_boots(@base_item_armor+20, "Initiate's Beaststalker Boots", '', @subclass_leather, @base_item_armor+20, @flags_default, @material_leather, @agility, 3, @intellect, 0, @stamina, 1, @armor_leather_boots);

/*
* Rogue
*/
call create_shoulders(@base_item_armor+21, "Initiate's Shadowcraft Spaulders", '', @subclass_leather, @base_item_armor+21, @flags_default, @material_leather, @strength, 0, @agility, 3, @stamina, 1, @armor_leather_shoulders);
call create_chest(@base_item_armor+22, "Initiate's Shadowcraft Tunic", '', @subclass_leather, @base_item_armor+22, @flags_default, @material_leather, @strength, 0, @agility, 3, @stamina, 2, @armor_leather_chest);
call create_bracers(@base_item_armor+23, "Initiate's Shadowcraft Bracers", '', @subclass_leather, @base_item_armor+23, @flags_default, @material_leather, @strength, 0, @agility, 2, @stamina, 1, @armor_leather_bracers);
call create_gloves(@base_item_armor+24, "Initiate's Shadowcraft Gloves", '', @subclass_leather, @base_item_armor+24, @flags_default, @material_leather, @strength, 1, @agility, 2, @stamina, 1, @armor_leather_gloves);
call create_belt(@base_item_armor+25, "Initiate's Shadowcraft Belt", '', @subclass_leather, @base_item_armor+25, @flags_default, @material_leather, @strength, 1, @agility, 1, @stamina, 1, @armor_leather_belt);
call create_legs(@base_item_armor+26, "Initiate's Shadowcraft Pants", '', @subclass_leather, @base_item_armor+26, @flags_default, @material_leather, @strength, 2, @agility, 1, @stamina, 2, @armor_leather_legs);
call create_boots(@base_item_armor+27, "Initiate's Shadowcraft Boots", '', @subclass_leather, @base_item_armor+27, @flags_default, @material_leather, @strength, 3, @agility, 0, @stamina, 1, @armor_leather_boots);

/*
* Priest
*/
call create_shoulders(@base_item_armor+28, "Initiate's Devout Mantle", '', @subclass_cloth, @base_item_armor+28, @flags_default, @material_cloth, @intellect, 3, @spirit, 1, @stamina, 1, @armor_cloth_shoulders);
call create_robe(@base_item_armor+29, "Initiate's Devout Tunic", '', @subclass_cloth, @base_item_armor+29, @flags_default, @material_cloth, @intellect, 3, @spirit, 2, @stamina, 2, @armor_cloth_chest);
call create_bracers(@base_item_armor+30, "Initiate's Devout Bracers", '', @subclass_cloth, @base_item_armor+30, @flags_default, @material_cloth, @intellect, 1, @spirit, 1, @stamina, 1, @armor_cloth_bracers);
call create_gloves(@base_item_armor+31, "Initiate's Devout Gloves", '', @subclass_cloth, @base_item_armor+31, @flags_default, @material_cloth, @intellect, 1, @spirit, 2, @stamina, 1, @armor_cloth_gloves);
call create_belt(@base_item_armor+32, "Initiate's Devout Belt", '', @subclass_cloth, @base_item_armor+32, @flags_default, @material_cloth, @intellect, 2, @spirit, 1, @stamina, 1, @armor_cloth_belt);
call create_legs(@base_item_armor+33, "Initiate's Devout Skirt", '', @subclass_cloth, @base_item_armor+33, @flags_default, @material_cloth, @intellect, 2, @spirit, 2, @stamina, 2, @armor_cloth_legs);
call create_boots(@base_item_armor+34, "Initiate's Devout Sandals", '', @subclass_cloth, @base_item_armor+34, @flags_default, @material_cloth, @intellect, 1, @spirit, 1, @stamina, 1, @armor_cloth_boots);

/*
* Shaman
*/
call create_shoulders(@base_item_armor+35, "Initiate's Pauldrons of Elements", '', @subclass_leather, @base_item_armor+35, @flags_default, @material_leather, @agility, 1, @intellect, 2, @stamina, 2, @armor_leather_shoulders);
call create_robe(@base_item_armor+36, "Initiate's Vest of Elements", '', @subclass_leather, @base_item_armor+36, @flags_default, @material_leather, @agility, 2, @intellect, 2, @stamina, 3, @armor_leather_chest);
call create_bracers(@base_item_armor+37, "Initiate's Bindings of Elements", '', @subclass_leather, @base_item_armor+37, @flags_default, @material_leather, @agility, 1, @intellect, 1, @stamina, 1, @armor_leather_bracers);
call create_gloves(@base_item_armor+38, "Initiate's Gaunlets of Elements", '', @subclass_leather, @base_item_armor+38, @flags_default, @material_leather, @agility, 2, @intellect, 1, @stamina, 1, @armor_leather_gloves);
call create_belt(@base_item_armor+39, "Initiate's Cord of Elements", '', @subclass_leather, @base_item_armor+39, @flags_default, @material_leather, @agility, 0, @intellect, 0, @stamina, 1, @armor_leather_belt);
call create_legs(@base_item_armor+40, "Initiate's Kilt of Elements", '', @subclass_leather, @base_item_armor+40, @flags_default, @material_leather, @agility, 2, @intellect, 2, @stamina, 2, @armor_leather_legs);
call create_boots(@base_item_armor+41, "Initiate's Boots of Elements", '', @subclass_leather, @base_item_armor+41, @flags_default, @material_leather, @agility, 1, @intellect, 0, @stamina, 1, @armor_leather_boots);

/*
* Mage
*/
call create_shoulders(@base_item_armor+42, "Initiate's Magister Mantle", '', @subclass_cloth, @base_item_armor+42, @flags_default, @material_cloth, @intellect, 3, @spirit, 1, @stamina, 1, @armor_cloth_shoulders);
call create_robe(@base_item_armor+43, "Initiate's Magister Tunic", '', @subclass_cloth, @base_item_armor+43, @flags_default, @material_cloth, @intellect, 3, @spirit, 1, @stamina, 1, @armor_cloth_chest);
call create_bracers(@base_item_armor+44, "Initiate's Magister Bindings", '', @subclass_cloth, @base_item_armor+44, @flags_default, @material_cloth, @intellect, 2, @spirit, 1, @stamina, 1, @armor_cloth_bracers);
call create_gloves(@base_item_armor+45, "Initiate's Magister Gloves", '', @subclass_cloth, @base_item_armor+45, @flags_default, @material_cloth, @intellect, 2, @spirit, 1, @stamina, 1, @armor_cloth_gloves);
call create_belt(@base_item_armor+46, "Initiate's Magister Belt", '', @subclass_cloth, @base_item_armor+46, @flags_default, @material_cloth, @intellect, 3, @spirit, 1, @stamina, 1, @armor_cloth_belt);
call create_legs(@base_item_armor+47, "Initiate's Magister Leggings", '', @subclass_cloth, @base_item_armor+47, @flags_default, @material_cloth, @intellect, 2, @spirit, 1, @stamina, 2, @armor_cloth_legs);
call create_boots(@base_item_armor+48, "Initiate's Magister Boots", '', @subclass_cloth, @base_item_armor+48, @flags_default, @material_cloth, @intellect, 2, @spirit, 1, @stamina, 1, @armor_cloth_boots);

/*
* Warlock
*/
call create_shoulders(@base_item_armor+49, "Initiate's Dreadmist Mantle", '', @subclass_cloth, @base_item_armor+49, @flags_default, @material_cloth, @intellect, 2, @spirit, 1, @stamina, 2, @armor_cloth_shoulders);
call create_robe(@base_item_armor+50 , "Initiate's Dreadmist Tunic", '', @subclass_cloth, @base_item_armor+50, @flags_default, @material_cloth, @intellect, 3, @spirit, 1, @stamina, 2, @armor_cloth_chest);
call create_bracers(@base_item_armor+51, "Initiate's Dreadmist Bracers", '', @subclass_cloth, @base_item_armor+51, @flags_default, @material_cloth, @intellect, 1, @spirit, 1, @stamina, 1, @armor_cloth_bracers);
call create_gloves(@base_item_armor+52, "Initiate's Dreadmist Wraps", '', @subclass_cloth, @base_item_armor+52, @flags_default, @material_cloth, @intellect, 1, @spirit, 1, @stamina, 2, @armor_cloth_gloves);
call create_belt(@base_item_armor+53, "Initiate's Dreadmist Belt", '', @subclass_cloth, @base_item_armor+53, @flags_default, @material_cloth, @intellect, 2, @spirit, 1, @stamina, 1, @armor_cloth_belt);
call create_legs(@base_item_armor+54, "Initiate's Dreadmist Leggings", '', @subclass_cloth, @base_item_armor+54, @flags_default, @material_cloth, @intellect, 2, @spirit, 1, @stamina, 2, @armor_cloth_legs);
call create_boots(@base_item_armor+55, "Initiate's Dreadmist Sandals", '', @subclass_cloth, @base_item_armor+55, @flags_default, @material_cloth, @intellect, 1, @spirit, 1, @stamina, 2, @armor_cloth_boots);

/*
* Druid
*/
call create_shoulders(@base_item_armor+56, "Initiate's Wildheart Spaulders", '', @subclass_leather, @base_item_armor+56, @flags_default, @material_leather, @agility, 2, @intellect, 2, @stamina, 2, @armor_leather_shoulders);
call create_robe(@base_item_armor+57, "Initiate's Wildheart Vest", '', @subclass_leather, @base_item_armor+57, @flags_default, @material_leather, @agility, 2, @intellect, 2, @stamina, 2, @armor_leather_chest);
call create_bracers(@base_item_armor+58, "Initiate's Wildheart Bracers", '', @subclass_leather, @base_item_armor+58, @flags_default, @material_leather, @agility, 1, @intellect, 2, @stamina, 1, @armor_leather_bracers);
call create_gloves(@base_item_armor+59, "Initiate's Wildheart Gloves", '', @subclass_leather, @base_item_armor+59, @flags_default, @material_leather, @agility, 1, @intellect, 1, @stamina, 1, @armor_leather_gloves);
call create_belt(@base_item_armor+60, "Initiate's Wildheart Belt", '', @subclass_leather, @base_item_armor+60, @flags_default, @material_leather, @agility, 1, @intellect, 2, @stamina, 1, @armor_leather_belt);
call create_legs(@base_item_armor+61, "Initiate's Wildheart Kilt", '', @subclass_leather, @base_item_armor+61, @flags_default, @material_leather, @agility, 2, @intellect, 2, @stamina, 2, @armor_leather_legs);
call create_boots(@base_item_armor+62, "Initiate's Wildheart Boots", '', @subclass_leather, @base_item_armor+62, @flags_default, @material_leather, @agility, 1, @intellect, 1, @stamina, 1, @armor_leather_boots);


/*
* Class Containers
*/

-- Warrior Container 1 (Sword/Bow)
CALL create_item_container(@base_item_container, @base_item_container);
CALL create_container_loot(@base_item_container, @base_item_armor, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container, @base_item_armor+1, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container, @base_item_armor+2, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container, @base_item_armor+3, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container, @base_item_armor+4, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container, @base_item_armor+5, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container, @base_item_armor+6, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container, @base_item_weapon+1, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container, @base_item_weapon+2, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container, @base_item_weapon+4, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container, @base_item_weapon+10, @min_count_1, @max_count_1);

-- Warrior Container 2 (Axe/Bow)
CALL create_item_container(@base_item_container+1, @base_item_container+1);
CALL create_container_loot(@base_item_container+1, @base_item_armor, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+1, @base_item_armor+1, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+1, @base_item_armor+2, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+1, @base_item_armor+3, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+1, @base_item_armor+4, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+1, @base_item_armor+5, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+1, @base_item_armor+6, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+1, @base_item_weapon, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+1, @base_item_weapon+2, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+1, @base_item_weapon+6, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+1, @base_item_weapon+10, @min_count_1, @max_count_1);

-- Warrior Container 3 (Axe/Gun)
CALL create_item_container(@base_item_container+2, @base_item_container+2);
CALL create_container_loot(@base_item_container+2, @base_item_armor, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+2, @base_item_armor+1, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+2, @base_item_armor+2, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+2, @base_item_armor+3, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+2, @base_item_armor+4, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+2, @base_item_armor+5, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+2, @base_item_armor+6, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+2, @base_item_weapon, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+2, @base_item_weapon+6, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+2, @base_item_weapon+10, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+2, @base_item_weapon+11, @min_count_1, @max_count_1);
 
-- Paladin
CALL create_item_container(@base_item_container+3, @base_item_container+3);
CALL create_container_loot(@base_item_container+3, @base_item_armor+7, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+3, @base_item_armor+8, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+3, @base_item_armor+9, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+3, @base_item_armor+10, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+3, @base_item_armor+11, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+3, @base_item_armor+12, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+3, @base_item_armor+13, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+3, @base_item_weapon+1, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+3, @base_item_weapon+4, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+3, @base_item_weapon+10, @min_count_1, @max_count_1);

-- Hunter Container 1 (Axe/Bow)
CALL create_item_container(@base_item_container+4, @base_item_container+4);
CALL create_container_loot(@base_item_container+4, @base_item_armor+14, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+4, @base_item_armor+15, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+4, @base_item_armor+16, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+4, @base_item_armor+17, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+4, @base_item_armor+18, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+4, @base_item_armor+19, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+4, @base_item_armor+20, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+4, @base_item_weapon, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+4, @base_item_weapon+2, @min_count_1, @max_count_1);

-- Hunter Container 1 (Axe/Gun)
CALL create_item_container(@base_item_container+5, @base_item_container+5);
CALL create_container_loot(@base_item_container+5, @base_item_armor+14, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+5, @base_item_armor+15, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+5, @base_item_armor+16, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+5, @base_item_armor+17, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+5, @base_item_armor+18, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+5, @base_item_armor+19, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+5, @base_item_armor+20, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+5, @base_item_weapon, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+5, @base_item_weapon+11, @min_count_1, @max_count_1);

-- Rogue Container 1 (Sword/Bow)
CALL create_item_container(@base_item_container+6, @base_item_container+6);
CALL create_container_loot(@base_item_container+6, @base_item_armor+21, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+6, @base_item_armor+22, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+6, @base_item_armor+23, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+6, @base_item_armor+24, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+6, @base_item_armor+25, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+6, @base_item_armor+26, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+6, @base_item_armor+27, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+6, @base_item_weapon+2, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+6, @base_item_weapon+4, @min_count_2, @max_count_2);
CALL create_container_loot(@base_item_container+6, @base_item_weapon+12, @min_count_1, @max_count_1);

-- Rogue Container 2 (Axe/Bow)
CALL create_item_container(@base_item_container+7, @base_item_container+7);
CALL create_container_loot(@base_item_container+7, @base_item_armor+21, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+7, @base_item_armor+22, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+7, @base_item_armor+23, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+7, @base_item_armor+24, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+7, @base_item_armor+25, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+7, @base_item_armor+26, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+7, @base_item_armor+27, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+7, @base_item_weapon+2, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+7, @base_item_weapon+6, @min_count_2, @max_count_2);
CALL create_container_loot(@base_item_container+7, @base_item_weapon+12, @min_count_1, @max_count_1);

-- Rogue Container 3 (Sword/Gun)
CALL create_item_container(@base_item_container+8, @base_item_container+8);
CALL create_container_loot(@base_item_container+8, @base_item_armor+21, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+8, @base_item_armor+22, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+8, @base_item_armor+23, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+8, @base_item_armor+24, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+8, @base_item_armor+25, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+8, @base_item_armor+26, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+8, @base_item_armor+27, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+8, @base_item_weapon+11, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+8, @base_item_weapon+4, @min_count_2, @max_count_2);
CALL create_container_loot(@base_item_container+8, @base_item_weapon+12, @min_count_1, @max_count_1);

-- Priest
CALL create_item_container(@base_item_container+9, @base_item_container+9);
CALL create_container_loot(@base_item_container+9, @base_item_armor+28, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+9, @base_item_armor+29, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+9, @base_item_armor+30, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+9, @base_item_armor+31, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+9, @base_item_armor+32, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+9, @base_item_armor+33, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+9, @base_item_armor+34, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+9, @base_item_weapon+5, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+9, @base_item_weapon+7, @min_count_1, @max_count_1);

-- Shaman
CALL create_item_container(@base_item_container+10, @base_item_container+10);
CALL create_container_loot(@base_item_container+10, @base_item_armor+35, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+10, @base_item_armor+36, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+10, @base_item_armor+37, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+10, @base_item_armor+38, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+10, @base_item_armor+39, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+10, @base_item_armor+40, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+10, @base_item_armor+41, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+10, @base_item_weapon, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+10, @base_item_weapon+3, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+10, @base_item_weapon+6, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+10, @base_item_weapon+7, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+10, @base_item_weapon+9, @min_count_1, @max_count_1);

-- Mage
CALL create_item_container(@base_item_container+11, @base_item_container+11);
CALL create_container_loot(@base_item_container+11, @base_item_armor+42, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+11, @base_item_armor+43, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+11, @base_item_armor+44, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+11, @base_item_armor+45, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+11, @base_item_armor+46, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+11, @base_item_armor+47, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+11, @base_item_armor+48, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+11, @base_item_weapon+5, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+11, @base_item_weapon+7, @min_count_1, @max_count_1);

-- Warlock
CALL create_item_container(@base_item_container+12, @base_item_container+12);
CALL create_container_loot(@base_item_container+12, @base_item_armor+49, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+12, @base_item_armor+50, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+12, @base_item_armor+51, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+12, @base_item_armor+52, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+12, @base_item_armor+53, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+12, @base_item_armor+54, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+12, @base_item_armor+55, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+12, @base_item_weapon+5, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+12, @base_item_weapon+7, @min_count_1, @max_count_1);

-- Druid
CALL create_item_container(@base_item_container+13, @base_item_container+13);
CALL create_container_loot(@base_item_container+13, @base_item_armor+56, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+13, @base_item_armor+57, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+13, @base_item_armor+58, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+13, @base_item_armor+59, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+13, @base_item_armor+60, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+13, @base_item_armor+61, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+13, @base_item_armor+62, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+13, @base_item_weapon+7, @min_count_1, @max_count_1);
CALL create_container_loot(@base_item_container+13, @base_item_weapon+8, @min_count_1, @max_count_1);
