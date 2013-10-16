CREATE TABLE IF NOT EXISTS players (
name varchar(64) NOT NULL,
age INT,
PRIMARY KEY (name)
)ENGINE=MyISAM;


DROP TRIGGER IF EXISTS tri_redis;
CREATE TRIGGER tri_redis BEFORE INSERT ON players
for EACH ROW
BEGIN
DECLARE done INT DEFAULT 999;

set done = redis_command("127.0.0.1", 6379, concat("set player::", 
cast(NEW.name as CHAR), " ", json_object(NEW.name as name, NEW.age as age)));
if done>0 then
   SIGNAL sqlstate '45001' set message_text = "redis error!";
end if;
END;

DROP TRIGGER IF EXISTS tri_redis_update;
CREATE TRIGGER tri_redis_update BEFORE UPDATE ON players
for EACH ROW
BEGIN
DECLARE done INT DEFAULT 999;

set done = redis_command("127.0.0.1", 6379, concat("set player::", 
cast(NEW.name as CHAR), " ", json_object(NEW.name as name, NEW.age as age)));
if done>0 then
   SIGNAL sqlstate '45001' set message_text = "redis error!";
end if;
END;

DROP TRIGGER IF EXISTS tri_redis_delete;
CREATE TRIGGER tri_redis_delete BEFORE DELETE ON players
for EACH ROW
BEGIN
DECLARE done INT DEFAULT 999;
set done = redis_command("127.0.0.1", 6379, concat("del player::", 
cast(OLD.name as CHAR)));
if done>0 then
   SIGNAL sqlstate '45001' set message_text = "redis error!";
end if;
END;
