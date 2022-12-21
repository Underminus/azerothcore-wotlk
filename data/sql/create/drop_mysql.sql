REVOKE ALL PRIVILEGES ON * . * FROM 'wobgob'@'localhost';

REVOKE ALL PRIVILEGES ON `arcade_world` . * FROM 'wobgob'@'localhost';

REVOKE GRANT OPTION ON `arcade_world` . * FROM 'wobgob'@'localhost';

REVOKE ALL PRIVILEGES ON `arcade_characters` . * FROM 'wobgob'@'localhost';

REVOKE GRANT OPTION ON `arcade_characters` . * FROM 'wobgob'@'localhost';

REVOKE ALL PRIVILEGES ON `arcade_auth` . * FROM 'wobgob'@'localhost';

REVOKE GRANT OPTION ON `arcade_auth` . * FROM 'wobgob'@'localhost';

DROP USER 'wobgob'@'localhost';

DROP DATABASE IF EXISTS `arcade_world`;

DROP DATABASE IF EXISTS `arcade_characters`;

DROP DATABASE IF EXISTS `auth`;
