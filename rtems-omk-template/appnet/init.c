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
#include <system_def.h>
#include "system.h"
#include "app_def.h"
#include "appl_config.h"
#include <stdio.h>
#include <stdlib.h>
#include <rtems/error.h>
#include <rtems/monitor.h>
#include <rtems/shell.h>

#include <rtems/rtems_bsdnet.h>

#include "networkconfig.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define CONFIGURE_SHELL_COMMANDS_INIT
#define CONFIGURE_SHELL_COMMANDS_ALL
#define CONFIGURE_SHELL_COMMANDS_ALL_NETWORKING
#define CONFIGURE_SHELL_MOUNT_MSDOS
#define CONFIGURE_SHELL_MOUNT_NFS

#include <rtems/shellconfig.h>

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

#ifdef CONFIG_OC_APP_APPNET_TELNETD
#include <rtems/telnetd.h>

rtems_telnetd_config_table rtems_telnetd_config;

void run_telnetd_command(char *device_name,  void *arg)
{
  rtems_shell_env_t shell_env;

  rtems_shell_dup_current_env(&shell_env);
  shell_env.taskname = NULL;
  shell_env.devname = device_name;
  rtems_shell_main_loop(&shell_env);
}
#endif /*CONFIG_OC_APP_APPNET_TELNETD*/

void 
bad_rtems_status(rtems_status_code status, int fail_level, const char *text)
{
  printf("ERROR: %s status %s", text, rtems_status_text(status));
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

  printf( "\n\nRTEMS v "
          BUILD_VERSION_STRING(__RTEMS_MAJOR__ ,__RTEMS_MINOR__ ,__RTEMS_REVISION__)
          "\n");

  printf("RTEMS_BSP_NETWORK_DRIVER_NAME   = %s\n", __XSTRING(RTEMS_BSP_NETWORK_DRIVER_NAME));
  printf("RTEMS_BSP_NETWORK_DRIVER_ATTACH = %s\n", __XSTRING(RTEMS_BSP_NETWORK_DRIVER_ATTACH));

  rtems_monitor_init(RTEMS_MONITOR_SUSPEND|RTEMS_MONITOR_GLOBAL);
  /*rtems_capture_cli_init (0);*/

  if (rtems_bsdnet_initialize_network() < 0)
    printf( "Network initialization failed\n");
  else
    printf( "Network initialization OK\n");


  printf( "Starting application " APP_VER_ID " v "
          BUILD_VERSION_STRING(SW_VER_MAJOR,SW_VER_MINOR,SW_VER_PATCH)
	  "\n" );

  Task_1_name = rtems_build_name( 'T', 'S', 'K', '1' );

  status = rtems_task_create(
     Task_1_name,
     TASK_1_PRIORITY,
     RTEMS_MINIMUM_STACK_SIZE+0x10000,
     RTEMS_DEFAULT_MODES /*& ~(RTEMS_TIMESLICE_MASK) | RTEMS_TIMESLICE*/,
     RTEMS_DEFAULT_ATTRIBUTES,
     &Task_1_id
  );
  check_rtems_status(status, 0, "rtems_task_create of Task_1");

  status = rtems_task_start( Task_1_id, Task_1, 0 );
  check_rtems_status(status, 0, "rtems_task_start of Task_1\n");

  rtems_shell_init("SHLL",RTEMS_MINIMUM_STACK_SIZE+0x1000,
              SHELL_TASK_PRIORITY,"/dev/console",1,0, NULL);

  rtems_shell_add_cmd("testcmd", "app",
                "test command for shell",
                testcmd_forshell);

  //rtems_monitor_wakeup();

 #ifdef CONFIG_OC_APP_APPNET_TELNETD
  rtems_telnetd_config.command = run_telnetd_command;
  rtems_telnetd_config.arg = NULL;
  rtems_telnetd_config.priority = SHELL_TASK_PRIORITY;
  rtems_telnetd_config.stack_size = RTEMS_MINIMUM_STACK_SIZE+0x1000;
  rtems_telnetd_config.login_check = NULL;
  rtems_telnetd_config.keep_stdio = 0;

  status = rtems_telnetd_initialize();
  check_rtems_status(status, 0, "rtems_telnetd_initialize\n");
 #endif /*CONFIG_OC_APP_APPNET_TELNETD*/

  status = rtems_task_delete( RTEMS_SELF );

  printf( "*** END OF TEST2 ***\n" );
  exit( 0 );
}
