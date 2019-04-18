#include "prefetch.h"
#include <iostream>
#include "system.h"

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
   uint64_t shifter = 1 << sys->SET_SHIFT;
   address = address + shifter;
   sys->memAccess(address, 'R', tid, true);
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
   uint64_t shifter = 1 << sys->SET_SHIFT;
   uint64_t mySet = address & sys->SET_MASK;
   uint64_t myTag = address & sys->TAG_MASK;

   if (myTag == MRM_tag && mySet == (MRM_set + shifter)){
       for(int i = 1; i <= n; i++) {
          sys->memAccess(address + (shifter * i), 'R', tid, true);
       }
       MRPrefetch = address + shifter;
       MRP_set = (address + shifter) & sys->SET_MASK;
       MRP_tag = (address + shifter)  & sys->TAG_MASK;
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

   if (myTag == MRP_tag & mySet == MRP_set) {
        uint64_t shifter = 1 << sys->SET_SHIFT;
        sys->memAccess(address + (shifter * n), 'R', tid, true);
        MRPrefetch += shifter;
        MRP_set = MRPrefetch & sys->SET_MASK;
        MRP_tag = MRPrefetch & sys->TAG_MASK;
   }
   return 1;
}

int BestEffortPrefetch::prefetchMiss(uint64_t address, unsigned int tid, System* sys)
{
   uint64_t shifter = 1 << sys->SET_SHIFT;
   for (int i = 1; i <= 7; i++) {
       sys->memAccess(address + (shifter * i), 'R', tid, true);
   }
   return 7;
}

int BestEffortPrefetch::prefetchHit(uint64_t address, unsigned int tid, System* sys)
{
   uint64_t shifter = 1 << sys->SET_SHIFT;
   sys->memAccess(address + shifter, 'R', tid, true);
   return 1;
}
