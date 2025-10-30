#ifndef SEMAPHORE_H
#define SEMAPHORE_H
#include <atomic>     // atomic_flag

using namespace std;

class Semaphore {
 public:
    // Initializes semaphore with a given value
    // sem: pointer to the semaphore value
    // value: initial semaphore count
  int init( int *sem, int pshared, int value );
    // Decrements (waits on) the semaphore
    // If the semaphore value is > 0, decrement and proceed
    // Else, the thread spins (busy-waits) until the value becomes > 0
  int wait( int *sem );
    // Increments (posts) the semaphore
    // Increases the semaphore value and releases any waiting threads
  int post( int *sem );

 private:
    // Atomic flag used for hardware-level locking, only one thread modifies the semaphore at a time
  atomic_flag lock = ATOMIC_FLAG_INIT;
};

#endif // SEMAPHORE_H
