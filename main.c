#include <stdlib.h>

#include <stdio.h>

#include <time.h>

#include <pthread.h>

#include <unistd.h>


void timespec_add_us(struct timespec * tm, long us) {
  tm -> tv_nsec += us * 1000;
  while (tm -> tv_nsec > 1000000000) {
    tm -> tv_nsec = tm -> tv_nsec - 1000000000;
    tm -> tv_sec += 1;
  }
}

void * temperature(void * arg) {
  int LOWER_TEMPERATURE_BOUND = 0;
  float UPPER_TEMPERATURE_BOUND = 100.0;
  struct timespec end;
  long period_us1 = 4000000;
  int * temperature = (int * ) arg;

  clock_gettime(CLOCK_MONOTONIC, & end);
  while (1) {
    timespec_add_us( & end, period_us1);
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, & end, NULL);
    temperature[0] = LOWER_TEMPERATURE_BOUND + (int)(UPPER_TEMPERATURE_BOUND * rand() / (RAND_MAX + 1.0));
    printf("Temperature \t : %d Celcius \n", temperature[0]);

  }
  return (void * ) temperature;
}

void * pressure(void * arg) {
  int LOWER_PRESSURE_BOUND = 40;
  float UPPER_PRESSURE_BOUND = 300.0;
  struct timespec end;
  long period_us2 = 3500000;
  int * pressure = (int * ) arg;

  clock_gettime(CLOCK_MONOTONIC, & end);
  while (1) {
    timespec_add_us( & end, period_us2);
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, & end, NULL);
    pressure[1] = LOWER_PRESSURE_BOUND + (int)(UPPER_PRESSURE_BOUND * rand() / (RAND_MAX + 1.0));
    printf("Pressure \t : %d Pascal \n", pressure[1]);

  }
  return (void * ) pressure;
}

void * both(void * arg) {
  struct timespec next;
  long period_us3 = 4000000;
  int * display = (int * ) arg;

  clock_gettime(CLOCK_MONOTONIC, & next);
  while (1) {
    timespec_add_us( & next, period_us3);
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, & next, NULL);
    printf("\n(Both Thread )\nTemperature : %d Celcius\nPressure    : %d Pascal \n\n", display[0], display[1]);
  }
  return NULL;
}

void main(void) {
  pthread_t thread_ID1, thread_ID2, thread_ID3;
  void * exitstatus1, * exitstatus2, * exitstatus3;
  pthread_attr_t my_attr;
  struct sched_param param1, param2, param3;
  param1.sched_priority = 5;
  param2.sched_priority = 3;
  param3.sched_priority = 1;

  int * address = (int * ) malloc(2 * sizeof(int));

  pthread_attr_init( & my_attr);
  pthread_attr_setinheritsched( & my_attr, PTHREAD_EXPLICIT_SCHED);
  pthread_attr_setschedpolicy( & my_attr, SCHED_FIFO);

  pthread_attr_setschedparam( & my_attr, & param1);
  pthread_create( & thread_ID1, & my_attr, temperature, & address);
  pthread_attr_setschedparam( & my_attr, & param2);
  pthread_create( & thread_ID2, & my_attr, pressure, & address);
  pthread_attr_setschedparam( & my_attr, & param3);
  pthread_create( & thread_ID3, & my_attr, both, & address);
  pthread_attr_destroy( & my_attr);

  pthread_join(thread_ID1, & exitstatus1);
  pthread_join(thread_ID2, & exitstatus2);
  pthread_join(thread_ID3, & exitstatus3);

  free(address);
}
