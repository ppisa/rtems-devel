/*  Init
 *
 *  This routine is the initialization task for this test program.
 *  It is called from init_exec and has the responsibility for creating
 *  and starting the tasks that make up the test.  If the time of day
 *  clock is required for the test, it should also be set to a known
 *  value by this function.
 *
 *  Input parameters:  NONE
 *
 *  Output parameters:  NONE
 *
 *  COPYRIGHT (c) 1989-1999.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id: init.c,v 1.12.4.1 2003/09/04 18:46:30 joel Exp $
 */

#define CONFIGURE_INIT
#include "system.h"
#include "app_def.h"
#include <stdio.h>
#include <stdlib.h>
#include <rtems/error.h>
#include <rtems/monitor.h>
#include <rtems/shell.h>
#include <pthread.h>

#define VER_CODE(major,minor,patch) (major*0x10000+minor*0x100+patch)

#define BUILD_VERSION_STRING(major,minor,patch) \
        __XSTRING(major) "." __XSTRING(minor) "." __XSTRING(patch)

#define RTEMS_VER_CODE VER_CODE(__RTEMS_MAJOR__ ,__RTEMS_MINOR__ ,__RTEMS_REVISION__)

#if RTEMS_VER_CODE < VER_CODE(4,7,99)
  #define rtems_shell_add_cmd shell_add_cmd
  #define rtems_shell_init(m_task_name,m_task_stacksize,m_task_priority,m_devname,m_forever,m_wait,m_login_check) \
                shell_init(m_task_name,m_task_stacksize,m_task_priority,m_devname,B19200 | CS8,m_forever)
#elif RTEMS_VER_CODE < VER_CODE(4,9,99)
  #define rtems_shell_init(m_task_name,m_task_stacksize,m_task_priority,m_devname,m_forever,m_wait,m_login_check) \
          rtems_shell_init(m_task_name,m_task_stacksize,m_task_priority,m_devname,m_forever,m_wait)
#endif

void 
bad_rtems_status(rtems_status_code status, int fail_level, const char *text)
{
  printf("ERROR: %s status %s\n", text, rtems_status_text(status));
  status = rtems_task_delete( RTEMS_SELF );
}

int testcmd_forshell(int argc, char **argv)
{
  int i;
  printf("Command %s called\n",argv[0]);
  for(i=1;i<argc;i++)
    if(argv[i])
      printf("%s",argv[i]);
  printf("\n");
  return 0;
}

rtems_task Init(
  rtems_task_argument ignored
)
{
  rtems_status_code status;
  pthread_attr_t taskattr;
  struct sched_param schedparam;
  pthread_t task_id;

  printf( "\n\nRTEMS v "
          BUILD_VERSION_STRING(__RTEMS_MAJOR__ ,__RTEMS_MINOR__ ,__RTEMS_REVISION__)
	  "\n");
  
  rtems_monitor_init(RTEMS_MONITOR_SUSPEND|RTEMS_MONITOR_GLOBAL);
  /*rtems_capture_cli_init (0);*/
  
  printf( "Starting application prioinh_check\n" );

  status = pthread_attr_init(&taskattr);
  if(status!=0) bad_status(status, 0, "pthread_attr_init 1" );
  status = pthread_attr_setinheritsched(&taskattr, PTHREAD_EXPLICIT_SCHED);
  if(status!=0) bad_status(status, 0, "pthread_attr_setinheritsched 1" );
  status = pthread_attr_setschedpolicy(&taskattr, SCHED_FIFO);
  if(status!=0) bad_status(status, 0, "pthread_attr_setschedpolicy 1" );
  schedparam.sched_priority = sched_get_priority_min(SCHED_FIFO) + TASK_1_PRIORITY;
  status = pthread_attr_setschedparam(&taskattr, &schedparam);
  if(status!=0) bad_status(status, 0, "pthread_attr_setschedparam 1" );
  status = pthread_create(&task_id, &taskattr, Task_1, NULL);
  if(status!=0) bad_status(status, 0, "pthread_create 1\n");

  if(1) {

    rtems_shell_init("SHLL",RTEMS_MINIMUM_STACK_SIZE+0x1000,
              SHELL_TASK_PRIORITY,"/dev/console",1,0, NULL);

    rtems_shell_add_cmd("testcmd", "app",
                "test command for shell",
                testcmd_forshell);

    //rtems_monitor_wakeup();

  }

  status = rtems_task_delete( RTEMS_SELF );
  
  printf( "*** END OF TEST2 ***\n" );
  exit( 0 );
}
