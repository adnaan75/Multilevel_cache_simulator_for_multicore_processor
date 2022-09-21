#ifndef CACHE_H
#define CACHE_H
#include "types.h"
#define MAX_WAYS 16

typedef struct Cache_Line Cache_Line;
typedef struct Cache_Set Cache_Set;
typedef struct Cache Cache;
/////////////////////////////////////////////////////////////////////////////////////////////
// Define the Data Structures here with correct field (Look at Appendix A for more details)
//////////////////////////////////////////////////////////////////////////////////////////////
struct Cache_Line {
    Flag    valid;
    Flag    dirty;
    Addr    tag;
    uns     core_id;
    uns    last_access_time;
};

struct Cache_Set {
    Cache_Line cline[MAX_WAYS];
};

struct Cache{

  Cache_Set *csets;
  uns64 way_num;

  uns64 repl_policy;
  uns64 set_num;
  Cache_Line last_evicted_line;

  uns64 stat_read_access;
  uns64 stat_write_access;
  uns64 stat_read_miss;
  uns64 stat_write_miss;
  uns64 stat_dirty_evicts;
};



//////////////////////////////////////////////////////////////////////////////////////////////
// Following functions might be helpful(Look at Appendix A for more details)
//////////////////////////////////////////////////////////////////////////////////////////////


Cache  *cache_new(uns64 size, uns64 assocs, uns64 linesize, uns64 repl_policy);
Flag    cache_access         (Cache *c, Addr lineaddr, uns is_write, uns core_id);
void    cache_install        (Cache *c, Addr lineaddr, uns is_write, uns core_id);
void    cache_print_stats    (Cache *c, char *header);
uns     cache_find_victim    (Cache *c, uns set_index, uns core_id);

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

#endif // CACHE_H
