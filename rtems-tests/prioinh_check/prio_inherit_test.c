#include "system.h"
#include <string.h>
#include <rtems.h>
#include <rtems/error.h>
#include <stdio.h>

#include "app_def.h"

rtems_id shared_with_hi_sem;
rtems_id shared_with_lo_sem;

rtems_id release_hi_sem;
rtems_id release_mid_sem;
rtems_id release_lo_sem;

rtems_id hi_task_id;
rtems_id mid_task_id;
rtems_id lo_task_id;


void block_delay(int delay)
{
  volatile int l;
  
  while(delay--) {
    l = 100000;
    while(l--);
  }
}


rtems_task hi_task(
  rtems_task_argument argument
)
{
  rtems_status_code status;

  printf("THI created\n");
  while(1){
    status=rtems_semaphore_obtain(release_hi_sem,RTEMS_WAIT,RTEMS_NO_TIMEOUT);
    check_rtems_status(status, 0, "rtems_semaphore_obtain RHI from THI");
    printf("THI released (RHI)\n");

    status=rtems_semaphore_obtain(shared_with_hi_sem,RTEMS_WAIT,RTEMS_NO_TIMEOUT);
    check_rtems_status(status, 0, "rtems_semaphore_obtain SHI from THI");
    printf("THI obtained SHI\n");

    block_delay(100);

    printf("THI going to release SHI\n");
    status=rtems_semaphore_release(shared_with_hi_sem);
    check_rtems_status(status, 0, "rtems_semaphore_release SHI from THI");
    printf("THI released SHI\n");
  }
}

rtems_task mid_task(
  rtems_task_argument argument
)
{
  rtems_status_code status;

  printf("TMID created\n");
  while(1){
    status=rtems_semaphore_obtain(release_mid_sem,RTEMS_WAIT,RTEMS_NO_TIMEOUT);
    check_rtems_status(status, 0, "rtems_semaphore_obtain RMID from TMID");
    printf("MID released (RMID)\n");

    block_delay(100);

    printf("MID going to sleep\n");
  }
}

rtems_task lo_task(
  rtems_task_argument argument
)
{
  rtems_status_code status;

  printf("LO created\n");
  while(1){
    status=rtems_semaphore_obtain(release_lo_sem,RTEMS_WAIT,RTEMS_NO_TIMEOUT);
    check_rtems_status(status, 0, "rtems_semaphore_obtain RLO from TLO");
    printf("TLO released (RLO)\n");

    status=rtems_semaphore_obtain(shared_with_lo_sem,RTEMS_WAIT,RTEMS_NO_TIMEOUT);
    check_rtems_status(status, 0, "rtems_semaphore_obtain SLO from TLO");
    printf("TLO obtained SLO\n");

    block_delay(100);

    printf("TLO going to release SLO\n");
    status=rtems_semaphore_release(shared_with_lo_sem);
    check_rtems_status(status, 0, "rtems_semaphore_release SLO from TLO");
    printf("TLO released SLO\n");
  }
}


rtems_task Task_1(
  rtems_task_argument argument
)
{
  rtems_status_code status;
  rtems_task_priority prio;

  status = rtems_task_wake_after( TICKS_PER_SECOND );
  check_rtems_status( status, 0, "rtems_task_wake_after" );

  printf("*** Starting up Task_1 ***\n");


  status=rtems_semaphore_create(rtems_build_name('S','H','I',' '),
          1,
          RTEMS_PRIORITY|RTEMS_BINARY_SEMAPHORE|RTEMS_INHERIT_PRIORITY|RTEMS_LOCAL,
          0,
          &shared_with_hi_sem);
  check_rtems_status( status, 0, "rtems_semaphore_create SHI" );

  status=rtems_semaphore_create(rtems_build_name('S','L','O',' '),
          1,
          RTEMS_PRIORITY|RTEMS_BINARY_SEMAPHORE|RTEMS_INHERIT_PRIORITY|RTEMS_LOCAL,
          0,
          &shared_with_lo_sem);
  check_rtems_status( status, 0, "rtems_semaphore_create SLO" );

  status=rtems_semaphore_create(rtems_build_name('R','H','I',' '),
          0,
          RTEMS_PRIORITY|RTEMS_SIMPLE_BINARY_SEMAPHORE|RTEMS_LOCAL,
          0,
          &release_hi_sem);
  check_rtems_status( status, 0, "rtems_semaphore_create RHI" );

  status=rtems_semaphore_create(rtems_build_name('R','M','I','D'),
          0,
          RTEMS_PRIORITY|RTEMS_SIMPLE_BINARY_SEMAPHORE|RTEMS_LOCAL,
          0,
          &release_mid_sem);
  check_rtems_status( status, 0, "rtems_semaphore_create RMID" );

  status=rtems_semaphore_create(rtems_build_name('R','L','O',' '),
          0,
          RTEMS_PRIORITY|RTEMS_SIMPLE_BINARY_SEMAPHORE|RTEMS_LOCAL,
          0,
          &release_lo_sem);
  check_rtems_status( status, 0, "rtems_semaphore_create RLO" );

  status = rtems_task_create(rtems_build_name('T','H','I',' '),
     TASK_HI_PRIORITY,
     RTEMS_MINIMUM_STACK_SIZE+0x1000,
     RTEMS_DEFAULT_MODES /*& ~(RTEMS_TIMESLICE_MASK) | RTEMS_TIMESLICE*/,
     RTEMS_DEFAULT_ATTRIBUTES,
     &hi_task_id
  );
  check_rtems_status(status, 0, "rtems_task_create THI");

  status = rtems_task_create(rtems_build_name('T','M','I','D'),
     TASK_MID_PRIORITY,
     RTEMS_MINIMUM_STACK_SIZE+0x1000,
     RTEMS_DEFAULT_MODES /*& ~(RTEMS_TIMESLICE_MASK) | RTEMS_TIMESLICE*/,
     RTEMS_DEFAULT_ATTRIBUTES,
     &mid_task_id
  );
  check_rtems_status(status, 0, "rtems_task_create MID");

  status = rtems_task_create(rtems_build_name('T','L','O',' '),
     TASK_LO_PRIORITY,
     RTEMS_MINIMUM_STACK_SIZE+0x1000,
     RTEMS_DEFAULT_MODES /*& ~(RTEMS_TIMESLICE_MASK) | RTEMS_TIMESLICE*/,
     RTEMS_DEFAULT_ATTRIBUTES,
     &lo_task_id
  );
  check_rtems_status(status, 0, "rtems_task_create LO");

  status = rtems_task_start( hi_task_id, hi_task, 0 );
  check_rtems_status(status, 0, "rtems_task_start THI\n");

  status = rtems_task_start( mid_task_id, mid_task, 0 );
  check_rtems_status(status, 0, "rtems_task_start MID\n");

  status = rtems_task_start( lo_task_id, lo_task, 0 );
  check_rtems_status(status, 0, "rtems_task_start LO\n");


  while(1){


    status=rtems_semaphore_obtain(shared_with_lo_sem,RTEMS_WAIT,RTEMS_NO_TIMEOUT);
    check_rtems_status(status, 0, "rtems_semaphore_obtain SLO");
    printf("1 obtained SLO\n");

    printf("1 going to release RLO\n");
    status=rtems_semaphore_release(release_lo_sem);
    check_rtems_status(status, 0, "rtems_semaphore_release RLO");

    status=rtems_semaphore_obtain(shared_with_hi_sem,RTEMS_WAIT,RTEMS_NO_TIMEOUT);
    check_rtems_status(status, 0, "rtems_semaphore_obtain SHI");
    printf("1 obtained SHI\n");

    printf("1 going to release RHI\n");
    status=rtems_semaphore_release(release_hi_sem);
    check_rtems_status(status, 0, "rtems_semaphore_release RHI");

    status = rtems_task_wake_after( TICKS_PER_SECOND );
    check_rtems_status( status, 0, "rtems_task_wake_after" );

    status = rtems_task_set_priority(RTEMS_SELF, RTEMS_CURRENT_PRIORITY, &prio);
    check_rtems_status( status, 0, "rtems_task_set_priority");
    printf("1 priority is %d\n", (int)prio);

    printf("1 going to release RMID\n");
    status=rtems_semaphore_release(release_mid_sem);
    check_rtems_status(status, 0, "rtems_semaphore_release RMID");

    block_delay(100);

    printf("1 going to release SHI\n");
    status=rtems_semaphore_release(shared_with_hi_sem);
    check_rtems_status(status, 0, "rtems_semaphore_release SHI");

    status = rtems_task_set_priority(RTEMS_SELF, RTEMS_CURRENT_PRIORITY, &prio);
    check_rtems_status( status, 0, "rtems_task_set_priority");
    printf("1 priority is %d\n", (int)prio);

    block_delay(100);

    printf("1 going to release SLO\n");
    status=rtems_semaphore_release(shared_with_lo_sem);
    check_rtems_status(status, 0, "rtems_semaphore_release SLO");
    printf("1 released both SHI and SLO\n");

    status = rtems_task_set_priority(RTEMS_SELF, RTEMS_CURRENT_PRIORITY, &prio);
    check_rtems_status( status, 0, "rtems_task_set_priority");
    printf("1 priority is %d\n", (int)prio);

    block_delay(100);

    printf("1 going to sleep\n");
    status = rtems_task_wake_after( TICKS_PER_SECOND );
    check_rtems_status( status, 0, "rtems_task_wake_after" );
  }
}
