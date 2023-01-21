# Patch 3.3.5a
The following is a collection of SQL files to create Wobblin' Goblin's `patch-4.MPQ`. It uses [WoW Spell Editor](https://github.com/stoneharry/WoW-Spell-Editor) for creating the SQL files, editing spells, and exporting the resulting DBCs. You'll need to have a familiarity with databases to follow this process.

## Building
1. It's assumed that your MySQL 8.0 database is hosted locally and that there is a user named `acore` with password `acore` who can create databases
1. To get started make sure you have `make` installed. Run `make` to import all the data into your database
1. Now you can make changes to the database. Manipulate it with your database editor or WoW Spell Editor until you're happy with the results.
1. To export to DBC for testing or production you'll have to export from WoW Spell Editor as DBC for the server and as an MPQ for the client
1. Once you have tested your changes export the SQL statements from your database and add them to the appropriate files in `wobgob/` or `retro/`. Commit and push the latest changes to your fork.
1. Open or update your pull request to `wobgob/azerothcore`
1. Wait for review and if necessary repeat from step 4

## Usage
* Run `make` to run the `create`, `base`, `wobgob`, and `retro` tasks
* Run `make create` to create the `dbc` database
* Run `make base` to recreate the database with the base data set
* Run `make wobgob` to apply the updates found in `wobgob/`
* Run `make retro` to apply the updates found in `retro/`
* Run `make clean` to drop the `dbc` database
