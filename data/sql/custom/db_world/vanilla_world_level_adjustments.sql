/* Karazhan Quest NPCs */
UPDATE `creature_template` SET `minlevel` = 60, `maxlevel` = 60, `ScriptName`='npc_ipp_tbc' WHERE `entry` IN (17613);
UPDATE `creature_template` SET `minlevel` = 59, `maxlevel` = 59, `ScriptName`='npc_ipp_tbc' WHERE `entry` IN (18253);  # TODO: This is currently overriding some SmartAI and needs a custom fix
UPDATE `creature_template` SET `minlevel` = 58, `maxlevel` = 58, `ScriptName`='npc_ipp_tbc' WHERE `entry` IN (18255);

/* Arena NPCs */
UPDATE `creature_template` SET `minlevel` = 60, `maxlevel` = 60, `ScriptName`='npc_ipp_tbc' WHERE `entry` IN
(19915, 19909, 19911, 26012, 26007, 26075, 26307, 26309, 26760, 19912, 19859, 19860, 19861, 21448, 20499, 20497,
 34093, 20278, 33939, 33935, 33934, 30610, 30611, 32407, 32385, 32832, 32383, 33924, 34090, 33928, 33929, 33915,
 34088, 33920, 33917);

/* WotLK Arena NPCs */
UPDATE `creature_template` SET `minlevel` = 60, `maxlevel` = 60, `ScriptName`='npc_ipp_wotlk' WHERE `entry` IN
(34060, 34063, 34038);
