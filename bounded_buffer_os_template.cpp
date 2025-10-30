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
  delete buf;
}

/* Insert item onto the rear of shared buffer sp */
void BoundedBufferOS::insert( int item ) {
  /* Implement by yourself */
 }

/* Remove and return the first item from buffer sp */
int BoundedBufferOS::remove( )
{
 int item;
 /* Implement by yourself */
 return item;
 }

