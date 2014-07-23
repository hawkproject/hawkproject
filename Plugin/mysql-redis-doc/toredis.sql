select concat(
redis_cmd, ' ',
rkey, ' ',
rval, ' '
)
from (
select 'set' as redis_cmd, name as rkey, age as rval from players
) as t
