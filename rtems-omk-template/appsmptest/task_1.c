#include <system_def.h>
#include "system.h"
#include "app_def.h"
#include <stdio.h>
#include <stdlib.h>
#include <rtems/untar.h>
#include <rtems/error.h>
#include <rtems/mw_uid.h>
#include <errno.h>


rtems_task Task_1(
  rtems_task_argument argument
)
{
  int loops = 10;
  int id = (int)argument;
  rtems_status_code status;
  printf("*** Starting up Task_%d ***\n", id);

  while(loops--){
    printf("Task_%d woken\n", id);
    status = rtems_task_wake_after(rtems_clock_get_ticks_per_second());
    check_rtems_status( status, 0, "rtems_task_wake_after" );
  }
  printf("*** Suspend Task_%d ***\n", id);
  rtems_task_suspend( RTEMS_SELF );
}

