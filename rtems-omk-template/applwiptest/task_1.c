#include <system_def.h>
#include "system.h"
#include "app_def.h"
#include <stdio.h>
#include <stdlib.h>
#include <rtems/untar.h>
#include <rtems/error.h>
#include <rtems/mw_uid.h>
#include <errno.h>

#include <lwip/init.h>
#include "lwip/err.h"
#include "lwip/tcp.h"
#include <netif/etharp.h>
#include <lwip/tcp_impl.h>

#include "lwip/tcpip.h"
#include "arch/eth_lwip.h"
#include "lwip/api.h"
#include "lwip/netbuf.h"
#include "lwip/netdb.h"
#include "lwip/netifapi.h"
#include "lwip/pppapi.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"

void lwip_echo_task(rtems_task_argument arg)
{
  int clientfd = (int) arg;
  char buffer[1024];
  int nbytes;
  printf("ECHO THREAD STARTED %d\n", clientfd);
  do{
    nbytes=lwip_recv(clientfd, buffer, sizeof(buffer),0);
    if (nbytes>0) lwip_send(clientfd, buffer, nbytes, 0);
  } while (nbytes>0);
  lwip_close(clientfd);
  printf("ECHO THREAD STOPPED %d\n", clientfd);
  rtems_task_delete( RTEMS_SELF );
}

void lwip_echo_server(void)
{
  int lSocket;
  struct sockaddr_in sLocalAddr;

  lSocket = lwip_socket(AF_INET, SOCK_STREAM, 0);
  if (lSocket < 0) return;

  memset((char *)&sLocalAddr, 0, sizeof(sLocalAddr));
  sLocalAddr.sin_family = AF_INET;
  sLocalAddr.sin_len = sizeof(sLocalAddr);
  sLocalAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  sLocalAddr.sin_port = 23;

  if (lwip_bind(lSocket, (struct sockaddr *)&sLocalAddr, sizeof(sLocalAddr)) < 0) {
          lwip_close(lSocket);
          return;
  }

  if ( lwip_listen(lSocket, 20) != 0 ){
          lwip_close(lSocket);
          return;
  }

  while (1)
  {
        int clientfd;
        struct sockaddr_in client_addr;
        int addrlen=sizeof(client_addr);
        rtems_id id;
        rtems_status_code res;

        clientfd = lwip_accept(lSocket, (struct sockaddr*)&client_addr, (socklen_t)&addrlen);
        if (clientfd>0){
          res = rtems_task_create(rtems_build_name('E', 'C', 'H', '0'+(clientfd & 7)),
            90, RTEMS_MINIMUM_STACK_SIZE+2048, RTEMS_PREEMPT, 0, &id );

          if (res != RTEMS_SUCCESSFUL) {
            lwip_close(clientfd);
            printf("ECHO THREAD NOT CREATED %d\n", clientfd);
            continue;
          }
          res = rtems_task_start(id, (rtems_task_entry)lwip_echo_task, (rtems_task_argument)clientfd);
          if (res != RTEMS_SUCCESSFUL) {
            rtems_task_delete(id);
            lwip_close(clientfd);
            continue;
          }
        }
  }
  lwip_close(lSocket);
}

rtems_task Task_1(
  rtems_task_argument argument
)
{
  printf("*** Starting up Task_1 ***\n");
  
  int loops = 3;
  rtems_status_code status;
  while(loops--){
    eth_lwip_get_dhcp_info();
    status = rtems_task_wake_after(rtems_clock_get_ticks_per_second()*5);
  }
  printf("*** Start Echo server ***\n");
  lwip_echo_server();
  printf("*** Echo server ended ***\n");
  rtems_task_suspend( RTEMS_SELF );
}

