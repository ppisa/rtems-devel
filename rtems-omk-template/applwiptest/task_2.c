#include <system_def.h>
#include "system.h"
#include "app_def.h"

#include "appl_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define BUFFER_SIZE 1024

rtems_task Task_2( rtems_task_argument argument )
{
  int                so;
  int                so2;
  int                res;
  socklen_t          peer_addr_len;
  struct sockaddr_in peer_addr;
  struct sockaddr_in serv_addr;
  rtems_status_code  status;
  char               buffer[ BUFFER_SIZE ];
  int                loops = 3;

  printf( "*** Starting up Task_2 ***\n" );

  status = rtems_task_wake_after( rtems_clock_get_ticks_per_second() * 15 );

  so = socket( PF_INET, SOCK_STREAM, 0 );

  if ( so < 0 ) {
    perror( "socket" );
  }

  peer_addr.sin_family = AF_INET;
  peer_addr.sin_port = htons( 5555 );     // short, network byte order
  peer_addr.sin_addr.s_addr = inet_addr( "192.168.1.10" );

  printf( "trying connect\n" );
  res = connect( so, (struct sockaddr *) &peer_addr, sizeof( peer_addr ) );

  if ( res < 0 ) {
    perror( "connect" );
  }

  res = write( so, "test of RTEMS LWIP IO\n", 22 );

  if ( res < 0 ) {
    perror( "write" );
  }

  res = send( so, "the second line\n", 16, 0 );

  if ( res < 0 ) {
    perror( "send" );
  }

  so2 = socket( PF_INET, SOCK_STREAM, 0 );

  if ( so < 0 ) {
    perror( "socket 2" );
  }

  res = close( so2 );

  if ( res < 0 ) {
    perror( "close socket 2" );
  }

  res = close( so );

  if ( res < 0 ) {
    perror( "close" );
  }

  so = socket( PF_INET, SOCK_STREAM, 0 );

  if ( so < 0 ) {
    perror( "socket serv" );
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons( 5555 );     // short, network byte order
  serv_addr.sin_addr.s_addr = INADDR_ANY;

  res = bind( so, (struct sockaddr *) &serv_addr, sizeof( serv_addr ) );

  if ( res < 0 ) {
    perror( "bind serv" );
  }

  res = listen( so, 10 );

  if ( res < 0 ) {
    perror( "listen serv" );
  }

  printf( "trying accept\n" );
  peer_addr_len = sizeof( peer_addr );
  so2 = accept( so, (struct sockaddr *) &peer_addr, &peer_addr_len );

  if ( so2 < 0 ) {
    perror( "accept" );
  }

  res = write( so2, "RTEMS LWIP server reached\n", 26 );

  if ( res < 0 ) {
    perror( "write" );
  }

  res = read( so2, buffer, BUFFER_SIZE );

  if ( res < 0 ) {
    perror( "read" );
  } else {
    printf( "received %d characters\n", res );
  }

  res = close( so2 );

  if ( res < 0 ) {
    perror( "close client" );
  }

  res = close( so );

  if ( res < 0 ) {
    perror( "close serv" );
  }

  while ( loops-- ) {
    printf( "Task 2\n" );
    status = rtems_task_wake_after( rtems_clock_get_ticks_per_second() * 5 );
    (void) status;
  }

  printf( "*** End of task 2 ***\n" );
  rtems_task_suspend( RTEMS_SELF );
}
