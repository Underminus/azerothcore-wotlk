-- Blackwing Spellbinder shouldn't polymorph.
DELETE FROM `smart_scripts` WHERE `entryorguid` = 12457 and `id` = 0;

-- Death Talon Wyrmguard shouldn't use brood powers.
DELETE FROM `smart_scripts` WHERE `entryorguid` = 12460 AND `id` = 1;
