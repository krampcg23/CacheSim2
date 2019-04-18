#ifndef PREFETCH_H
#define PREFETCH_H

#include <cstdint>

class System;

class Prefetch {
public:
   virtual int prefetchMiss(uint64_t address, unsigned int tid,
                              System* sys)=0;
   virtual int prefetchHit(uint64_t address, unsigned int tid,
                              System* sys)=0;
};

//"Prefetcher" that does nothing
class NullPrefetch : public Prefetch {
public:
   int prefetchMiss(uint64_t address, unsigned int tid,
                              System* sys);
   int prefetchHit(uint64_t address, unsigned int tid,
                              System* sys);
};

// Part 1: AdjPrefetch
class AdjPrefetch : public Prefetch {
public:
    int prefetchMiss(uint64_t address, unsigned int tid,
                              System* sys);
    int prefetchHit(uint64_t address, unsigned int tid,
                              System* sys);
};

// Part 2: SeqPrefetch
class SeqPrefetch : public Prefetch {
public:
    int n = 8;
    int prefetchMiss(uint64_t address, unsigned int tid,
                              System* sys);
    int prefetchHit(uint64_t address, unsigned int tid,
                              System* sys);
};

// Part 3: BestEffortPrefetch
class BestEffortPrefetch : public Prefetch {
public:
    int prefetchMiss(uint64_t address, unsigned int tid,
                              System* sys);
    int prefetchHit(uint64_t address, unsigned int tid,
                              System* sys);
};


#endif
