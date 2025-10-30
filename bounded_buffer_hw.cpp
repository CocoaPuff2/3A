#include "bounded_buffer_hw.h"
#include <iostream>

/* Create an empty, bounded, shared FIFO buffer with n slots */
BoundedBufferHW::BoundedBufferHW( int bufsize ) {
  if ( bufsize <= 0 )
    bufsize = BUFSIZE;
  buf = new int[bufsize];
  n = bufsize;              /* Buffer holds max of n items */
  front = rear = 0;         /* Empty buffer iff front == rear */
  sem_m.init(&mutex, 0, 1); /* Binary semaphore for locking */
  sem_f.init(&full, 0, n);  /* Initially, buf has n available slots */
  sem_e.init(&empty, 0, 0); /* Initially, buf has zero data items */
}

/* Destructor: Clean up buffer sp */
BoundedBufferHW::~BoundedBufferHW( ) {
    // delete[] buf;
  delete buf;
}

// Insert item onto the rear of shared buffer sp, blocks w/ busy-wait if full
/*   Pseudocode:
 *     wait (full) - for free slot
 *     wait (mutex) - acquire lock enter Crit Sec
 *     Add item to buffer (cicular logic)
 *     post (mutex) - release lock, other threads can access buffer
 *     post (empty) - signal, new item availible for consumers
 */
void BoundedBufferHW::insert( int item ) {
  sem_f.wait(&full);
  sem_m.wait(&mutex);

  buf[rear % n] = item;
  rear++; // index moved to next pos

  sem_m.post(&mutex);
  sem_e.post(&empty);
 }

/* Remove and return the first item from buffer sp */
/*   Pseudocode:
 *     wait (empty) - for free slot
 *     wait (mutex) - acquire lock enter Crit Sec
 *     Remove item from buffer (cicular logic)
 *     post (mutex) - release lock, other threads can access buffer
 *     post (full) - signal, new item availible for consumers
 */
// todo
int BoundedBufferHW::remove( ) {
    int item;

    sem_e.wait(&empty);
    sem_m.wait(&mutex);

    item = buf[front % n];
    front++; // moves the index to the next item in buffer

    sem_m.post(&mutex);
    sem_f.post(&full);

    return item;
 }

