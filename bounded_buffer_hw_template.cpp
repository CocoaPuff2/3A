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

/* Clean up buffer sp */
BoundedBufferHW::~BoundedBufferHW( ) {
  delete buf;
}

/* Insert item onto the rear of shared buffer sp */
void BoundedBufferHW::insert( int item ) {
  /* Implement by yourself */
 }

/* Remove and return the first item from buffer sp */
int BoundedBufferHW::remove( )
{
 int item;
  /* Implement by yourself */  
 return item;
 }

