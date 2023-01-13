-- MC and BWL should have a 3 day lockout
DELETE FROM `mapdifficulty_dbc` WHERE `ID` IN (34,39);
INSERT INTO `mapdifficulty_dbc` (`ID`, `MapID`, `Difficulty`, `RaidDuration`, `MaxPlayers`, `Difficultystring`) VALUES
(34, 409, 0, 259200, 40, 'RAID_DIFFICULTY_40PLAYER'), -- MC
(39, 469, 0, 259200, 40, 'RAID_DIFFICULTY_40PLAYER'); -- BWL
