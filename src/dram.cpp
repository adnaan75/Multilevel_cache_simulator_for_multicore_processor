#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "dram.h"

#define DRAM_RAS         45
#define DRAM_CAS         45
#define DRAM_PRE         45
#define DRAM_BUS         10
extern MODE SIM_MODE;
extern Flag DRAM_PAGE_POLICY;
extern uns64 CACHE_LINESIZE;
////////////////////////////////////////////////////////////////////
// ------------- DO NOT MODIFY THE PRINT STATS FUNCTION -------
////////////////////////////////////////////////////////////////////

void    dram_print_stats(DRAM *dram){
  double rddelay_avg=0;
  double wrdelay_avg=0;
  char header[256];
  sprintf(header, "DRAM");

  if(dram->stat_read_access){
    rddelay_avg=(double)(dram->stat_read_delay)/(double)(dram->stat_read_access);
  }

  if(dram->stat_write_access){
    wrdelay_avg=(double)(dram->stat_write_delay)/(double)(dram->stat_write_access);
  }

  printf("\n%s_READ_ACCESS\t\t : %10llu", header, dram->stat_read_access);
  printf("\n%s_WRITE_ACCESS\t\t : %10llu", header, dram->stat_write_access);
  printf("\n%s_READ_DELAY_AVG\t\t : %10.3f", header, rddelay_avg);
  printf("\n%s_WRITE_DELAY_AVG\t\t : %10.3f", header, wrdelay_avg);


}
//////////////////
DRAM *dram_new(){
  DRAM *dram = (DRAM *) calloc (1, sizeof (DRAM));
  return dram;
}

/////////////////////////////////////////////////////////////////////////////
uns64 dram_access(DRAM *dram,Addr lineaddr, Flag is_dram_write){
  uns64 delay=DRAM_LATENCY_FIXED;

  if(SIM_MODE!=SIM_MODE_A && SIM_MODE!=SIM_MODE_B){
    delay = dram_access_mode_CDE(dram, lineaddr, is_dram_write);
  }
  if(is_dram_write)
  {
    dram->stat_write_delay=dram->stat_write_delay+delay;
    dram->stat_write_access++;
  }
  else
  {
    dram->stat_read_access++;
    dram->stat_read_delay=dram->stat_read_delay+delay;
  }

  return delay;
}

static int first_call = 1;
uns64 dram_access_mode_CDE(DRAM *dram,Addr lineaddr, Flag is_dram_write)
{
  int i;
  uns64 delay=0,bnk_indx,rbf_indx;
  static int open_rbf[DRAM_BANKS];
  bnk_indx = (lineaddr / (ROWBUF_SIZE / CACHE_LINESIZE)) % DRAM_BANKS;
  rbf_indx = lineaddr /(ROWBUF_SIZE / CACHE_LINESIZE)/DRAM_BANKS;
  if (first_call==1) {
    for(i=0;i<DRAM_BANKS; i++) {
      open_rbf[i] = -1;
    }
    first_call = 0;
  }

  if(DRAM_PAGE_POLICY==0){
      if(open_rbf[bnk_indx] == -1) {
        open_rbf[bnk_indx] = rbf_indx;
        delay = DRAM_RAS + DRAM_BUS+DRAM_CAS ;
      }
      else if(open_rbf[bnk_indx] == rbf_indx) {
        delay = DRAM_BUS+DRAM_CAS ;
      }
      else {
        open_rbf[bnk_indx] = rbf_indx;
        delay = DRAM_PRE + DRAM_RAS + DRAM_CAS + DRAM_BUS;
      }
  }
  else{
    delay =  DRAM_RAS + DRAM_CAS + DRAM_BUS;
  }

  return delay;
}
