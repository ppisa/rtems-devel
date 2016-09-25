#ifndef _APP_DEF_H
#define _APP_DEF_H

#ifndef COND_EXTERN
  #ifdef CONFIGURE_INIT
    #define COND_EXTERN
  #else
    #define COND_EXTERN extern
  #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define APP_VER_ID  "prioinh_posix"

/* Posix style startup task */
void *Task_1(void *argument);

#define SHELL_TASK_PRIORITY 14
#define TASK_HI_PRIORITY    13
#define TASK_MID_PRIORITY   12
#define TASK_1_PRIORITY     11
#define TASK_LO_PRIORITY    10

void bad_status(int status, int fail_level, const char *text);

#ifdef __cplusplus
}
#endif

#endif /*_APP_DEF_H*/
