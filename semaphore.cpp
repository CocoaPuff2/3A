#include "semaphore.h"

int Semaphore::init( int *sem, int pshared, int value ) {
  *sem = value;
  return 0;
}

int Semaphore::wait( int *sem ) {
  /* Implement it by yourself */
  return 0;
}

int Semaphore::post( int *sem ) {
  /* Implement it by yourself */    
  return 0;
}
