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
  rtems_status_code status;
  printf("*** Starting up Task_1 ***\n");

  while(1){
    printf("Task_1 woken\n");
    status = rtems_task_wake_after( TICKS_PER_SECOND );
    check_rtems_status( status, 0, "rtems_task_wake_after" );
  }
}

