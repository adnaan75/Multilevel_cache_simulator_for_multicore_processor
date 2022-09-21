#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "cache.h"

extern uns64 cycle;

////////////////////////////////////////////////////////////////////
// ------------- DO NOT MODIFY THE PRINT STATS FUNCTION -----------
////////////////////////////////////////////////////////////////////

void    cache_print_stats    (Cache *c, char *header){
  double read_mr =0;
  double write_mr =0;

  if(c->stat_read_access){
    read_mr=(double)(c->stat_read_miss)/(double)(c->stat_read_access);
  }

  if(c->stat_write_access){
    write_mr=(double)(c->stat_write_miss)/(double)(c->stat_write_access);
  }

  printf("\n%s_READ_ACCESS    \t\t : %10llu", header, c->stat_read_access);
  printf("\n%s_WRITE_ACCESS   \t\t : %10llu", header, c->stat_write_access);
  printf("\n%s_READ_MISS      \t\t : %10llu", header, c->stat_read_miss);
  printf("\n%s_WRITE_MISS     \t\t : %10llu", header, c->stat_write_miss);
  printf("\n%s_READ_MISS_PERC  \t\t : %10.3f", header, 100*read_mr);
  printf("\n%s_WRITE_MISS_PERC \t\t : %10.3f", header, 100*write_mr);
  printf("\n%s_DIRTY_EVICTS   \t\t : %10llu", header, c->stat_dirty_evicts);

  printf("\n");
}

/////////////////
Cache  *cache_new(uns64 size, uns64 assoc, uns64 linesize, uns64 repl_policy){

   Cache *cache = (Cache *) calloc (1, sizeof (Cache));
   cache->set_num = size/(linesize*assoc);
   cache->way_num = assoc;
   cache->repl_policy = repl_policy;
   cache->csets = (Cache_Set *) calloc (cache->set_num, sizeof(Cache_Set));

   return cache;
}

////////
Flag cache_access(Cache *c, Addr lineaddr, uns is_write, uns core_id){

  unsigned int index,tag,i;
  int f=0;
  index = lineaddr % c->set_num;
  tag = lineaddr / c->set_num;

  if (is_write) {
    c->stat_write_access++;
  }
  else {
    c->stat_read_access++;
  }

  for(i=0; i<c->way_num; i++)
  {
    if(c->csets[index].cline[i].valid && (c->csets[index].cline[i].tag == tag))
    {
      if (is_write) {
        c->csets[index].cline[i].dirty = 1;
      }
      c->csets[index].cline[i].last_access_time = cycle;
      f=1;
      break;
    }
  }




  if (is_write) {
    c->stat_write_miss++;
  }
  else {
    c->stat_read_miss++;
  }
  if (f==1)
  {
    return HIT;
  }
  else
  {
    return MISS;
  }

}

//////////

void cache_install(Cache *c, Addr lineaddr, uns is_write, uns core_id){

  unsigned int index,tag,victim;
  index = lineaddr % c->set_num;
  tag = lineaddr / c->set_num;

  victim = cache_find_victim(c, index, core_id);

  c->last_evicted_line = c->csets[index].cline[victim];

  c->csets[index].cline[victim].dirty = is_write;
  c->csets[index].cline[victim].valid = 1;
  c->csets[index].cline[victim].tag = tag;
  c->csets[index].cline[victim].core_id = 0;
  c->csets[index].cline[victim].last_access_time = cycle;
  if ( c->csets[index].cline[victim].dirty==1 ) {
    c->stat_dirty_evicts++;
  }
}

/////////////
uns cache_find_victim(Cache *c, uns set_index, uns core_id){
  uns victim=0;
  int f=0;
  uns64 i,x=0;
  unsigned long earliest;

  for(i=0; i<c->way_num; i++)
  {
    if(c->csets[set_index].cline[i].valid==0)
    {
      f=1;
      break;
    }
  }
  if(f)
  {
      victim=i;
  }
  else if(c->repl_policy == 0)
  {
    earliest=c->csets[set_index].cline[0].last_access_time;
    for(i=0; i<c->way_num; i++)
    {
      if((c->csets[set_index].cline[i].last_access_time <= earliest))
      {
        x = i;
        earliest = c->csets[set_index].cline[i].last_access_time;
      }
    }
    victim = x;
  }
   else
   {
      victim = rand()% c->way_num;
   }

  return victim;
}
