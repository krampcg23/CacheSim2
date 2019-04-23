# CacheSim2
## Author: Clayton Kramp

## Usage
type `make` to compile.  To execute, type `./cache` or `./cache --prefetcher [BestEffortPrefetch | AdjPrefetch | SeqPrefetch]`

### AdjPrefetch
Fairly simple prefetcher that prefetches the next line.  Currently gets 856  misses

### SeqPrefetch
Prefetches the next n lines is sequential access is detected. Toggle the n value in prefetch.h.  Currently gets 1176  misses

### BestEffortPrefetch
Has the best prefetcher of the three, and always prefetches some number of lines.  The values can be toggled in prefetch.h.  Currently gets 408 misses
