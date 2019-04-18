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
    for(int i = 1; i <= n; i++) {
        sys->memAccess(address + (SHIFTER * i), 'R', tid, true);
    }
    return n;
}

int SeqPrefetch::prefetchHit(uint64_t address, unsigned int tid, System* sys)
{
    prefetchMiss(address, tid, sys);
    return n;
}

int BestEffortPrefetch::prefetchMiss(uint64_t address, unsigned int tid, System* sys)
{
   for (int i = 1; i <= 3; i++) {
       sys->memAccess(address + (SHIFTER * i), 'R', tid, true);
   }
   return 3;
}

int BestEffortPrefetch::prefetchHit(uint64_t address, unsigned int tid, System* sys)
{
   sys->memAccess(address + SHIFTER, 'R', tid, true);
   return 1;
}
