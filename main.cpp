#include <iostream>
#include <fstream>
#include <cassert>
#include <sstream>
#include <string>
#include "system.h"

using namespace std;

int main(int argc, char* argv[])
{
   Prefetch* prefetch = nullptr;
   if (argc == 2) {
        cerr << "Wrong usage, expectes --prefetcher [BestEffortPrefetch | SeqPrefetch | AdjPrefetch]" << endl;
        return -1;
   }

   if (argc > 1) {
       if (string(argv[1]) != string("--prefetcher")) {
           cerr << "Wrong usage, expectes --prefetcher [BestEffortPrefetch | SeqPrefetch | AdjPrefetch]" << endl;
           return -1;
       }
       if (string(argv[2]) == string("SeqPrefetch")) prefetch = new SeqPrefetch();
       else if (string(argv[2]) == string("AdjPrefetch")) prefetch = new AdjPrefetch();
       else if (string(argv[2]) == string("BestEffortPrefetch")) prefetch = new BestEffortPrefetch();
       else prefetch = new NullPrefetch();
   }
   // tid_map is used to inform the simulator how
   // thread ids map to NUMA/cache domains. Using
   // the tid as an index gives the NUMA domain.
   unsigned int arr_map[] = {0};
   vector<unsigned int> tid_map(arr_map, arr_map +
         sizeof(arr_map) / sizeof(unsigned int));
   // The constructor parameters are:
   // the tid_map, the cache line size in bytes,
   // number of cache lines, the associativity,
   // the prefetcher object,
   // whether to count compulsory misses,
   // whether to do virtual to physical translation,
   // and number of caches/domains
   // WARNING: counting compulsory misses doubles execution time
   MultiCacheSystem sys(tid_map, 64, 1024, 64, prefetch, true, false, 1);
   char rw;
   uint64_t address;
   unsigned long long lines = 0;
   ifstream infile;
   // This code works with the output from the
   // ManualExamples/pinatrace pin tool
   infile.open("pinatrace.out", ifstream::in);
   assert(infile.is_open());

   string line;
   while(getline(infile,line))
   {
      stringstream ss(line);
      ss >> rw;
      assert(rw == 'R' || rw == 'W');
      ss >> hex >> address;
      if(address != 0) {
         // By default the pinatrace tool doesn't record the tid,
         // so we make up a tid to stress the MultiCache functionality
         sys.memAccess(address, rw, 0);
      }

      ++lines;
   }

   cout << "Accesses: " << lines << endl;
   cout << "Hits: " << sys.stats.hits << endl;
   cout << "Misses: " << lines - sys.stats.hits << endl;
   cout << "Local reads: " << sys.stats.local_reads << endl;
   cout << "Local writes: " << sys.stats.local_writes << endl;
   cout << "Remote reads: " << sys.stats.remote_reads << endl;
   cout << "Remote writes: " << sys.stats.remote_writes << endl;
   cout << "Other-cache reads: " << sys.stats.othercache_reads << endl;
   //cout << "Compulsory Misses: " << sys.stats.compulsory << endl;

   infile.close();

   return 0;
}
