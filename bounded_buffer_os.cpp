#include "bounded_buffer_os.h"

/* Create an empty, bounded, shared FIFO buffer with n slots */
BoundedBufferOS::BoundedBufferOS( int bufsize ) {
  if ( bufsize <= 0 )
    bufsize = BUFSIZE;
  buf = new int[bufsize];
  n = bufsize;            /* Buffer holds max of n items */
  front = rear = 0;       /* Empty buffer iff front == rear */
  sem_init(&mutex, 0, 1); /* Binary semaphore for locking */
  sem_init(&full, 0, n);  /* Initially, buf has n available slots */
  sem_init(&empty, 0, 0); /* Initially, buf has zero data items */
}

/* Clean up buffer sp */
BoundedBufferOS::~BoundedBufferOS( ) {
    // delete[] buf
  delete buf;
}

/* Insert item onto the rear of shared buffer sp */
void BoundedBufferOS::insert( int item ) {
    // 1. Wait for at least one empty slot
    sem_wait(&full); // waits if buffer full

    // 2; Enter Critical Section
    sem_wait(&mutex); // locks buffer, acquire lock

    // 3. Insert Item at rear / end pos
    buf[rear] = item;
    rear = (rear + 1) % n;    // move forward in circular way

    // 4. Leave Critical Section
    sem_post(&mutex); // release / unlock buffer

    // 5. Signal: new item now availible!
    sem_post(&empty); // filled slots ++

 }

/* Remove and return the first item from buffer sp */
int BoundedBufferOS::remove( ) {
    int item;

    // 1. Wait for a filled slot
    sem_wait(&empty);

    // 2. Enter Critical Section
    sem_wait(&mutex);

    // 3. Remove item from front
    item = buf[front];
    front = (front + 1) % n;

    // 4. Leave Critical Section
    sem_post(&mutex);

    // 5. Signal: One more free slot
    sem_post(&full);

    return item;
 }

