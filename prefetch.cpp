#include "prefetch.h"
#include <iostream>
#include "system.h"
#define SHIFTER 64 // this is the same as 2 ^ sys->SET_SHIFT

int NullPrefetch::prefetchMiss(uint64_t address __attribute__((unused)),
                              unsigned int tid __attribute__((unused)),
                              System* sys __attribute__((unused)))
{
   return 0;
}
int NullPrefetch::prefetchHit(uint64_t address __attribute__((unused)),
                           unsigned int tid __attribute__((unused)),
                           System* sys __attribute__((unused)))
{
   return 0;
}

int AdjPrefetch::prefetchMiss(uint64_t address, unsigned int tid, System* sys)
{
   sys->memAccess(address + SHIFTER, 'R', tid, true);
   return 1;
}

int AdjPrefetch::prefetchHit(uint64_t address, unsigned int tid, System* sys)
{
   prefetchMiss(address, tid, sys);
   return 1;
}

int SeqPrefetch::prefetchMiss(uint64_t address, unsigned int tid, System* sys)
{
   // Prefetch sequentially only if the previous miss was the one right before
   uint64_t mySet = address & sys->SET_MASK;
   uint64_t myTag = address & sys->TAG_MASK;

   if (myTag == MRM_tag && mySet == (MRM_set + SHIFTER)){
       for(int i = 1; i <= n; i++) {
          sys->memAccess(address + (SHIFTER * i), 'R', tid, true);
       }
       MRPrefetch = address + SHIFTER;
       MRP_set = (address + SHIFTER) & sys->SET_MASK;
       MRP_tag = (address + SHIFTER)  & sys->TAG_MASK;
   }
   MRM_set = address & sys->SET_MASK;
   MRM_tag = address & sys->TAG_MASK;

   return n;
}

int SeqPrefetch::prefetchHit(uint64_t address, unsigned int tid, System* sys)
{
   // Prefetch the next Nth block only if the current address is the prefetched one
   uint64_t mySet = address & sys->SET_MASK;
   uint64_t myTag = address & sys->TAG_MASK;

   if (myTag == MRP_tag && mySet == MRP_set) {
        sys->memAccess(address + (SHIFTER * n), 'R', tid, true);
        MRPrefetch += SHIFTER;
        MRP_set = MRPrefetch & sys->SET_MASK;
        MRP_tag = MRPrefetch & sys->TAG_MASK;
   }
   return 1;
}

int BestEffortPrefetch::prefetchMiss(uint64_t address, unsigned int tid, System* sys)
{
    for (int i = (-1) * (n_miss-2); i < 0; i++) {
        sys->memAccess(address + (SHIFTER * i), 'R', tid, true);
    }
    for(int i = 1; i <= n_miss; i++) {
        sys->memAccess(address + (SHIFTER * i), 'R', tid, true);
    }
    return 2*n_miss;
}

int BestEffortPrefetch::prefetchHit(uint64_t address, unsigned int tid, System* sys)
{
    for (int i = (-1) * (n_hit-3); i < 0; i++) {
        sys->memAccess(address + (SHIFTER * i), 'R', tid, true);
    }

    for(int i = 1; i <= n_hit; i++) {
        sys->memAccess(address + (SHIFTER * i), 'R', tid, true);
    }
    return 2*n_hit;
}
