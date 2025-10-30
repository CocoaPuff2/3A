#include <atomic>     // atomic_flag
#include <iostream>   // cerr
#include <stdlib.h>   // atoi
#include <pthread.h>  // pthread_create, pthread_join

using namespace std;

// atomic_flag = special atomic type that can only be true or false
// used as spinlock for  mutual exclusion
//      (only one thread enters the critical section at a time)
atomic_flag lock = ATOMIC_FLAG_INIT;

// shared counter that all threads will increment
static int cnt = 0;

// thread function to increment counter
void *inc( void* arg ) {
  int tid = *(int *)arg; // get the thread ID (logical number passed from main)
  while ( true ) { // infinte loop, threads will break out once cnt reaches 100
    while ( atomic_flag_test_and_set_explicit( &lock, memory_order_acquire ) )
      ;

    //  ---- Critical Section Start ----
      // only one thread can execute this part at a time
    if ( cnt < 100 ) {
      cout << cnt++ << ": output from thread " << tid << endl;
      // release lock, new thread can enter
      atomic_flag_clear_explicit( &lock, memory_order_release );      
    }
    else {
      // if cnt >= 100, unlock and exit the loop (thread done)
      atomic_flag_clear_explicit( &lock, memory_order_release );
      break;
    }
      // ---- Critical Section End ----
  }

  return NULL;
}

int main( int argc, char* argv[]  ) {
  if ( argc != 2 ) {
    cerr << "usage: tas nThreads" << endl;
    return -1;
  }
  int nThreads = atoi( argv[1] );
  pthread_t tid[nThreads];
  int logical_tid[nThreads];

  for ( int i = 0; i < nThreads; i++ )
    logical_tid[i] = i;

  for ( int i = 0; i < nThreads; i++ )
    pthread_create( &tid[i], NULL, inc, (void *)&logical_tid[i] );
  
  for ( int i = 0; i < nThreads; i++ )
    pthread_join( tid[i], NULL );
}
