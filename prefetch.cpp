#include "prefetch.h"
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
   uint64_t shifter = 1 << sys->SET_SHIFT;
   address = address + shifter;
   sys->memAccess(address, 'R', tid, true);
   return 1;
}

int SeqPrefetch::prefetchHit(uint64_t address, unsigned int tid, System* sys)
{
   prefetchMiss(address, tid, sys);
   return 1;
}
