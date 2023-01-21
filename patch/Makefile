HOST ?= 127.0.0.1

.PHONY: all create base wobgob clean
.PHONY: all create base wobgob retro clean

all: create base wobgob retro

create:
	@mysql --host=$(HOST) --user=acore --password=acore --execute 'CREATE DATABASE IF NOT EXISTS dbc'

base:
	@cat base/*.sql | mysql --host=$(HOST) --user=acore --password=acore dbc

wobgob:
	@cat wobgob/*.sql | mysql --host=$(HOST) --user=acore --password=acore dbc

retro:
	@cat ../modules/mod-retroporting/sql/world/item_creation_item_template.sql | \
		mysql --host=$(HOST) --user=acore --password=acore dbc
	@cat retro/*.sql | mysql --host=$(HOST) --user=acore --password=acore dbc

clean:
	@mysql --host=$(HOST) --user=acore --password=acore --execute 'DROP DATABASE dbc'
