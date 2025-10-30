#include "semaphore.h"


// 1. Initialize the semaphore:
//  sem: pointer to the semaphore counter (integer)
//  pshared: ignored (mimics Linux sem_init)
//  value: initial value of semaphore
int Semaphore::init( int *sem, int pshared, int value ) {
  *sem = value;
  return 0;
}

// Wait (P / down) operation for HW semaphore.
// Blocks (busy-wait) until semaphore value > 0, then decrements it.
int Semaphore::wait( int *sem ) {
    while (true) {
        // Acquire spinlock w/ atomic test-and-set
        while (lock.test_and_set(std::memory_order_acquire))
            ; // busy wait till lock is free

            // ----- CRITICAL SECTION (Semaphore counter)---- //
            if (*sem > 0) {
                (*sem)--; // decrement the semaphore, takes 1 resource
                lock.clear(std::memory_order_release); //  release the spinlock
                return 0;  // resouce was successfully acquired
            }

            // if sem <= 0, release lock (start spinnig)
            lock.clear(std::memory_order_release);
    }
}

// Post (V / up) operation for HW semaphore.
// Increments semaphore value and releases the lock.
int Semaphore::post( int *sem ) {
    // Get spinlock
    while(lock.test_and_set(std::memory_order_acquire))
        ; // busy-wait

    (*sem)++; // increment semaphore (release resource)
    lock.clear(std::memory_order_release); // release the spinlock

  return 0;
}
