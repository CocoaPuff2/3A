#include <pthread.h>            /* pthread_create, pthread_join */
#include <iostream>             /* cout, cerr, endl */
#include <stdlib.h>             /* atoi */
#include <time.h>               /* clock */
#include "bounded_buffer_hw.h"
#include "bounded_buffer_os.h"

// Desc:  M I P
//       M = 1 (HW), = 2 (OS); I = #items to exchange; P = 0: no print, = 1 print
//     main( ) spawns two threads, calling producer( ) and consumer( ) respectively.
//      Depending on mode (1 == HW or 2 == OS), each thread calls
//              hBuf.insert( i ) and oBuf.insert( i )
//          or
//              hBuf.remove( i ) and oBuf.removeI i )
//      where hBuf is a bounded buffer with HW semaphores,
//      whereas oBuf is one with OS semaphores.

#define MAX 10  // Default number of items if not specified in command line

int mode = 0;  // 1 = HW version, 2 = OS version
int nItems = MAX; // Total number of items to produce/consume
int print = 0; // 0 = no output, 1 = print each item

BoundedBufferHW hBuf( 100 );
BoundedBufferOS oBuf( 100 );

/* Thread routines */
void *producer( void *arg ) {
  
  for ( int i = 0; i < nItems; i++ ) {
    switch( mode ) {
    case 1: hBuf.insert( i ); break; // HW-based synchronization
    case 2: oBuf.insert( i ); break; // OS-based synchronization
    default: cerr << "wrong mode" << endl; exit( -1 );
    }
  }
  return NULL;
}

void *consumer( void *arg ) {

  for ( int i = 0; i < nItems; i++ ) {
    int item = -1;
    switch( mode ) {
    case 1: item = hBuf.remove( ); break;
    case 2: item = oBuf.remove( ); break;
    default: cerr << "wrong mode" << endl; exit( -1 );
    }
    if ( print != 0 )
      cout << item << endl;
  }
  return NULL;
}

/* main() thread */
int main( int argc, char **argv ) {
  if ( argc != 4 ) {
    cerr << "usage: driver M I P" << endl;
    cerr << "       where M = 1 (HW), 2 (OS)" << endl;
    cerr << "             I = #items" << endl;
    cerr << "             P = 0: no printf, 1: printf" << endl;
    return -1;
  }
  mode = atoi( argv[1] ); // 1: HW, 2: OS, 3: Language
  if ( atoi( argv[2] ) > MAX ) nItems = atoi( argv[2] );
  print = atoi( argv[3] );

  // create a cpu set with core 1
  cpu_set_t cpuset_1;
  CPU_ZERO(&cpuset_1);
  CPU_SET(1, &cpuset_1);

// create a cpu set with core 2
  cpu_set_t cpuset_2;
  CPU_ZERO(&cpuset_2);
  CPU_SET(2, &cpuset_2);

  // start the timer
  clock_t start = clock( );
  /* Create threads and wait for them to finish */
  pthread_t tid1, tid2;
  pthread_create( &tid1, NULL, producer, NULL ); // Start producer thread
  pthread_create( &tid2, NULL, consumer, NULL ); // Start consumer thread

  // Set the affinity to thread 1
  if ( pthread_setaffinity_np(tid1, sizeof(cpu_set_t), &cpuset_1) != 0 ) {
    cerr << "pthread_set_affinity_np, s1" << endl;
    return -1;
  }
  
  // set the affinity to thread 2
  if ( pthread_setaffinity_np(tid2, sizeof(cpu_set_t), &cpuset_2) != 0 ) {
    cerr << "pthread_set_affinity_np, s2" << endl;
    return -1;
  }
  
  pthread_join( tid1, NULL ); // Wait for producer to finish
  pthread_join( tid2, NULL ); // Wait for consumer to finish
  // finish the timer
  clock_t end = clock( );

  printf( "time = %lf seconds\n", ((double)(end - start)) / CLOCKS_PER_SEC );
}

