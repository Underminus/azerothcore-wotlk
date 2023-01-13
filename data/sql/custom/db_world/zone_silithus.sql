-- Scarab Gong
-- See related AC PR: https://github.com/azerothcore/azerothcore-wotlk/pull/12879/files
UPDATE `gameobject_template` SET `type` = 2, `ScriptName` = "gobject_scarab_gong" WHERE `entry` = 180717;
UPDATE `gameobject` SET `id` = 180717 WHERE `guid` = 49451;
DELETE FROM `gameobject_questender` WHERE `quest`=8743;
INSERT INTO `gameobject_questender` (`id`, `quest`) VALUES (180717, 8743);
DELETE FROM `gameobject_queststarter` WHERE `quest`=8743;
INSERT INTO `gameobject_queststarter` (`id`, `quest`) VALUES (180717, 8743);

SET @STRING_ENTRY := 41100;
DELETE FROM `acore_string` WHERE `entry` IN  (@STRING_ENTRY);
INSERT INTO `acore_string` (`entry`, `content_default`) VALUES
(@STRING_ENTRY, '%s, Champion of the Bronze Dragonflight, has rung the Scarab Gong. The ancient gates of Ahn''Qiraj open, revealing the horrors of a forgotten war...');
