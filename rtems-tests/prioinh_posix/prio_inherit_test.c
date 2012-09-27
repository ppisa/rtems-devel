#define _GNU_SOURCE
#define _XOPEN_SOURCE  600

#include <sched.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "app_def.h"

pthread_mutex_t shared_with_hi_mtx;
pthread_mutex_t shared_with_lo_mtx;

sem_t release_hi_sem;
sem_t release_mid_sem;
sem_t release_lo_sem;

pthread_t hi_task_id;
pthread_t mid_task_id;
pthread_t lo_task_id;


void bad_status(int status, int fail_level, const char *text)
{
  printf("ERROR: %s status %d\n", text, status);
  exit(1);
}


void block_delay(int delay)
{
  volatile int l;
  
  while(delay--) {
    l = 100000;
    while(l--);
  }
}


void *hi_task(void *arg)
{
  int status;

  printf("THI created\n"); fflush(stdout);
  while(1){
    status=sem_wait(&release_hi_sem);
    if(status!=0) bad_status(status, 0, "sem_wait RHI from THI");
    printf("THI released (RHI)\n"); fflush(stdout);

    status=pthread_mutex_lock(&shared_with_hi_mtx);
    if(status!=0) bad_status(status, 0, "pthread_mutex_lock SHI from THI");
    printf("THI obtained SHI\n"); fflush(stdout);

    block_delay(100);

    printf("THI going to release SHI\n"); fflush(stdout);
    status=pthread_mutex_unlock(&shared_with_hi_mtx);
    if(status!=0) bad_status(status, 0, "pthread_mutex_unlock SHI from THI");
    printf("THI released SHI\n"); fflush(stdout);
  }
}

void *mid_task(void *arg)
{
  int status;

  printf("TMID created\n"); fflush(stdout);
  while(1){
    status=sem_wait(&release_mid_sem);
    if(status!=0) bad_status(status, 0, "sem_wait RMID from TMID");
    printf("MID released (RMID)\n"); fflush(stdout);

    block_delay(100);

    printf("MID going to sleep\n"); fflush(stdout);
  }
}

void *lo_task(void *arg)
{
  int status;

  printf("LO created\n"); fflush(stdout);
  while(1){
    status=sem_wait(&release_lo_sem);
    if(status!=0) bad_status(status, 0, "sem_wait RLO from TLO");
    printf("TLO released (RLO)\n"); fflush(stdout);

    status=pthread_mutex_lock(&shared_with_lo_mtx);
    if(status!=0) bad_status(status, 0, "pthread_mutex_lock SLO from TLO");
    printf("TLO obtained SLO\n"); fflush(stdout);

    block_delay(100);

    printf("TLO going to release SLO\n"); fflush(stdout);
    status=pthread_mutex_unlock(&shared_with_lo_mtx);
    if(status!=0) bad_status(status, 0, "pthread_mutex_unlock SLO from TLO");
    printf("TLO released SLO\n"); fflush(stdout);
  }
}


void *Task_1(void *argument)
{
  int status;
  pthread_mutexattr_t mutexattr;
  pthread_attr_t taskattr;
  struct sched_param schedparam;

  sleep(1);

  printf("*** Starting up Task_1 ***\n");

  schedparam.sched_priority = sched_get_priority_min(SCHED_FIFO) + TASK_1_PRIORITY;
  status = pthread_setschedparam(pthread_self(), SCHED_FIFO, &schedparam);
  if(status!=0) bad_status(status, 0, "pthread_setschedparam" );

  status = pthread_mutexattr_init(&mutexattr);
  if(status!=0) bad_status(status, 0, "pthread_mutexattr_init" );
  status=pthread_mutexattr_setprotocol(&mutexattr, PTHREAD_PRIO_INHERIT);
  if(status!=0) bad_status(status, 0, "pthread_mutexattr_setprotocol" );

  status=pthread_mutex_init(&shared_with_hi_mtx, &mutexattr);
  if(status!=0) bad_status(status, 0, "pthread_mutex_init SHI" );

  status=pthread_mutex_init(&shared_with_lo_mtx, &mutexattr);
  if(status!=0) bad_status(status, 0, "pthread_mutex_init SLO" );

  status=sem_init(&release_hi_sem, 0, 0);
  if(status!=0) bad_status(status, 0, "sem_init RHI" );

  status=sem_init(&release_mid_sem, 0, 0);
  if(status!=0) bad_status(status, 0, "sem_init RMID" );

  status=sem_init(&release_lo_sem, 0, 0);
  if(status!=0) bad_status(status, 0, "sem_init RLO" );

  status = pthread_attr_init(&taskattr);
  if(status!=0) bad_status(status, 0, "pthread_attr_init" );
  status = pthread_attr_setinheritsched(&taskattr, PTHREAD_EXPLICIT_SCHED);
  if(status!=0) bad_status(status, 0, "pthread_attr_setinheritsched" );
  status = pthread_attr_setschedpolicy(&taskattr, SCHED_FIFO);
  if(status!=0) bad_status(status, 0, "pthread_attr_setschedpolicy" );

  schedparam.sched_priority = sched_get_priority_min(SCHED_FIFO) + TASK_HI_PRIORITY;
  status = pthread_attr_setschedparam(&taskattr, &schedparam);
  if(status!=0) bad_status(status, 0, "pthread_attr_setschedparam" );

  status = pthread_create(&hi_task_id, &taskattr, hi_task, NULL);
  if(status!=0) bad_status(status, 0, "pthread_create THI\n");

  schedparam.sched_priority = sched_get_priority_min(SCHED_FIFO) + TASK_MID_PRIORITY;
  status = pthread_attr_setschedparam(&taskattr, &schedparam);
  if(status!=0) bad_status(status, 0, "pthread_attr_setschedparam" );

  status = pthread_create(&hi_task_id, &taskattr, mid_task, NULL);
  if(status!=0) bad_status(status, 0, "pthread_create TMID\n");

  schedparam.sched_priority = sched_get_priority_min(SCHED_FIFO) + TASK_LO_PRIORITY;
  status = pthread_attr_setschedparam(&taskattr, &schedparam);
  if(status!=0) bad_status(status, 0, "pthread_attr_setschedparam" );

  status = pthread_create(&hi_task_id, &taskattr, lo_task, NULL);
  if(status!=0) bad_status(status, 0, "pthread_create TLO\n");

  sleep(1);

  while(1){
  
    status=pthread_mutex_lock(&shared_with_lo_mtx);
    if(status!=0) bad_status(status, 0, "pthread_mutex_lock SLO");
    printf("1 obtained SLO\n"); fflush(stdout);

    printf("1 going to release RLO\n"); fflush(stdout);
    status=sem_post(&release_lo_sem);
    if(status!=0) bad_status(status, 0, "sem_post RLO");

    status=pthread_mutex_lock(&shared_with_hi_mtx);
    if(status!=0) bad_status(status, 0, "pthread_mutex_lock SHI");
    printf("1 obtained SHI\n"); fflush(stdout);

    printf("1 going to release RHI\n"); fflush(stdout);
    status=sem_post(&release_hi_sem);
    if(status!=0) bad_status(status, 0, "sem_post RHI");

    sleep(1);

    printf("1 going to release RMID\n"); fflush(stdout);
    status=sem_post(&release_mid_sem);
    if(status!=0) bad_status(status, 0, "sem_post RMID");

    block_delay(100);

    printf("1 going to release SHI\n"); fflush(stdout);
    status=pthread_mutex_unlock(&shared_with_hi_mtx);
    if(status!=0) bad_status(status, 0, "pthread_mutex_unlock SHI");

    block_delay(100);

    printf("1 going to release SLO\n"); fflush(stdout);
    status=pthread_mutex_unlock(&shared_with_lo_mtx);
    if(status!=0) bad_status(status, 0, "pthread_mutex_unlock SLO");
    printf("1 released both SHI and SLO\n"); fflush(stdout);

    block_delay(100);

    printf("1 going to sleep\n"); fflush(stdout);
    sleep(1);
  }
}

#ifndef COMPILE_FOR_RTEMS

int main(int argc, char *argv[])
{
  Task_1(NULL);
  return 0;
}

#endif /*COMPILE_FOR_RTEMS*/
