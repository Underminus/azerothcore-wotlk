#!/usr/bin/env bash

sed --in-place "s/^bind-address.*/bind-address = 0.0.0.0/" /etc/mysql/mysql.conf.d/mysqld.cnf
service mysql start
mysql --user=root < /azerothcore/data/sql/create/create_mysql.sql
/azerothcore/bin/authserver &
until /azerothcore/bin/worldserver; do sleep 1; done
