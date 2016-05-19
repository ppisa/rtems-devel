/*  Init
 *
 *  This routine is the initialization task for this test program.
 *  It is called from init_exec and has the responsibility for creating
 *  and starting the tasks that make up the test.  If the time of day
 *  clock is required for the test, it should also be set to a known
 *  value by this function.
 *
 *  (C) 2015 Pavel Pisa <pisa@cmp.felk.cvut.cz>
 *
 *  Based on RTEMS example test by
 *  On-Line Applications Research Corporation (OAR).
 *
 */

#define CONFIGURE_INIT
#include <system_def.h>
#include "appl_config.h"
#include "system.h"
#include "app_def.h"
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <rtems/error.h>
#include <rtems/monitor.h>
#include <rtems/shell.h>

#define USE_RTEMS_TARFS_LOAD

#ifdef USE_RTEMS_TARFS_LOAD
#include <rtems/imfs.h>
#else /*USE_RTEMS_TARFS_LOAD*/
#include <rtems/untar.h>
#endif /*USE_RTEMS_TARFS_LOAD*/

#ifdef CONFIG_OC_APP_APPDL_NET
#include <rtems/rtems_bsdnet.h>

#include "networkconfig.h"
#endif /*CONFIG_OC_APP_APPDL_NET*/

#include <sys/types.h>

#ifdef CONFIG_OC_APP_APPDL_NET
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif /*CONFIG_OC_APP_APPDL_NET*/

#include <string.h>
#include <unistd.h>

#define CONFIGURE_SHELL_COMMANDS_INIT
#define CONFIGURE_SHELL_COMMANDS_ALL
#define CONFIGURE_SHELL_COMMANDS_ALL_NETWORKING
#define CONFIGURE_SHELL_MOUNT_MSDOS
#ifdef CONFIG_OC_APP_APPDL_NET
#define CONFIGURE_SHELL_MOUNT_NFS
#endif

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

extern int _binary_rootfs_tarfile_start;
extern int _binary_rootfs_tarfile_size;
#define TARFILE_START _binary_rootfs_tarfile_start
#define TARFILE_SIZE _binary_rootfs_tarfile_size

#ifdef CONFIG_OC_APP_APPDL_TELNETD
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
#endif /*CONFIG_OC_APP_APPDL_TELNETD*/

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

typedef int (*call_t)(int argc, char* argv[]);

int dlopen_forshell(int argc, char **argv)
{
  void * handle;
  int    unresolved;
  char * message = "loaded";
  char   *call_file_name;
  char   *call_symbol = NULL;
  call_t call;
  int    call_ret;

  if (argc < 2) {
    printf ("file to load not specified\n");
    return 1;
  }
  call_file_name = argv[1];

  if (argc >= 3)
    call_symbol = argv[2];

  argc -= 2;
  argv += 2;

  handle = dlopen (call_file_name, RTLD_NOW | RTLD_GLOBAL);
  if (!handle)
  {
    printf("dlopen failed: %s\n", dlerror());
    return 1;
  }

  if (dlinfo (handle, RTLD_DI_UNRESOLVED, &unresolved) < 0)
    message = "dlinfo error checking unresolved status";
  else if (unresolved)
    message = "has unresolved externals";

  if (call_symbol == NULL) {
    printf ("handle: %p %s\n", handle, message);
    return 1;
  }

  call = dlsym (handle, call_symbol);
  if (call == NULL)
  {
    printf("dlsym failed: symbol %s not found\n", call_symbol);
    return 1;
  }

  call_ret = call(argc, argv);

  return call_ret;
}

rtems_task Init(
  rtems_task_argument ignored
)
{
  rtems_status_code status;
 #ifdef USE_RTEMS_TARFS_LOAD
  int res;
 #endif /*USE_RTEMS_TARFS_LOAD*/

  printf( "\n\nRTEMS v "
          BUILD_VERSION_STRING(__RTEMS_MAJOR__ ,__RTEMS_MINOR__ ,__RTEMS_REVISION__)
          "\n");

  rtems_monitor_init(RTEMS_MONITOR_SUSPEND|RTEMS_MONITOR_GLOBAL);
  /*rtems_capture_cli_init (0);*/

 #ifdef CONFIG_OC_APP_APPDL_NET
  if (rtems_bsdnet_initialize_network() < 0)
    printf( "Network initialization failed\n");
  else
    printf( "Network initialization OK\n");
 #endif /*CONFIG_OC_APP_APPDL_NET*/


  printf( "Starting application " SW_VER_ID " v "
          BUILD_VERSION_STRING(SW_VER_MAJOR,SW_VER_MINOR,SW_VER_PATCH)
          "\n" );

 #ifdef USE_RTEMS_TARFS_LOAD
  res = rtems_tarfs_load("/", (void*)(&TARFILE_START), (long)&TARFILE_SIZE);
  printf("rtems_tarfs_load returned %d\n", res);
 #else /*USE_RTEMS_TARFS_LOAD*/
  status = Untar_FromMemory((unsigned char *)(&TARFILE_START), (long)&TARFILE_SIZE);
  printf("Untar_FromMemory returned %s\n",rtems_status_text(status));
 #endif /*USE_RTEMS_TARFS_LOAD*/

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

  rtems_shell_add_cmd("dlopen", "app",
                "runtime load object and call contained function",
                dlopen_forshell);

  //rtems_monitor_wakeup();

 #ifdef CONFIG_OC_APP_APPDL_TELNETD
  rtems_telnetd_config.command = run_telnetd_command;
  rtems_telnetd_config.arg = NULL;
  rtems_telnetd_config.priority = SHELL_TASK_PRIORITY;
  rtems_telnetd_config.stack_size = RTEMS_MINIMUM_STACK_SIZE+0x1000;
  rtems_telnetd_config.login_check = NULL;
  rtems_telnetd_config.keep_stdio = 0;

  status = rtems_telnetd_initialize();
  check_rtems_status(status, 0, "rtems_telnetd_initialize\n");
 #endif /*CONFIG_OC_APP_APPDL_TELNETD*/

  status = rtems_task_delete( RTEMS_SELF );

  printf( "*** END OF APPDL ***\n" );
  exit( 0 );
}
