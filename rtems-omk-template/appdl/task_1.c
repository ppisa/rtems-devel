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
  int loops = 2;
  rtems_status_code status;
  printf("*** Starting up Task_1 ***\n");

  while(loops--){
    printf("Task_1 woken\n");
    status = rtems_task_wake_after(rtems_clock_get_ticks_per_second());
    check_rtems_status( status, 0, "rtems_task_wake_after" );
  }
  printf("*** Suspend Task_1 ***\n");
  rtems_task_suspend( RTEMS_SELF );
}

