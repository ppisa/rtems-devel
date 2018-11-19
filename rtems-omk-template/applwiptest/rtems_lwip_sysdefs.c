#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "rtems_lwip_int.h"

const int rtems_lwip_sysdefs_AF_UNSPEC = AF_UNSPEC;
const int rtems_lwip_sysdefs_AF_UNIX = AF_UNIX;
const int rtems_lwip_sysdefs_AF_INET = AF_INET;
const int rtems_lwip_sysdefs_AF_INET6 = AF_INET6;

const int rtems_lwip_sysdefs_PF_UNSPEC = PF_UNSPEC;
const int rtems_lwip_sysdefs_PF_UNIX = PF_UNIX;
const int rtems_lwip_sysdefs_PF_INET = PF_INET;
const int rtems_lwip_sysdefs_PF_INET6 = PF_INET6;

const int rtems_lwip_sysdefs_SOCK_STREAM = SOCK_STREAM;
const int rtems_lwip_sysdefs_SOCK_DGRAM = SOCK_DGRAM;
const int rtems_lwip_sysdefs_SOCK_RAW = SOCK_RAW;

const int rtems_lwip_sysdefs_sockaddr_in_size = sizeof( struct sockaddr_in );
const int rtems_lwip_sysdefs_sockaddr_in6_size = sizeof( struct sockaddr_in6 );

int rtems_lwip_sysdefs_sockaddr_get_len( const void *sockaddr )
{
  const struct sockaddr *so = sockaddr;

  return so->sa_len;
}

int rtems_lwip_sysdefs_sockaddr_get_family( const void *sockaddr )
{
  const struct sockaddr *so = sockaddr;

  return so->sa_family;
}

uint16_t rtems_lwip_sysdefs_sockaddr_in_get_sin_port( const void *sockaddr )
{
  const struct sockaddr_in *so = sockaddr;

  return so->sin_port;
}

uint32_t rtems_lwip_sysdefs_sockaddr_in_get_sin_addr( const void *sockaddr )
{
  const struct sockaddr_in *so = sockaddr;

  return so->sin_addr.s_addr;
}

uint16_t rtems_lwip_sysdefs_sockaddr_in6_get_sin6_port( const void *sockaddr )
{
  const struct sockaddr_in6 *so = sockaddr;

  return so->sin6_port;
}

const uint8_t *rtems_lwip_sysdefs_sockaddr_in6_get_sin6_addr_ptr(
  const void *sockaddr )
{
  const struct sockaddr_in6 *so = sockaddr;

  return (uint8_t *) &so->sin6_addr;
}

uint32_t rtems_lwip_sysdefs_sockaddr_in6_get_sin6_flowinfo(
  const void *sockaddr )
{
  const struct sockaddr_in6 *so = sockaddr;

  return so->sin6_flowinfo;
}

uint32_t rtems_lwip_sysdefs_sockaddr_in6_get_sin6_scope_id(
  const void *sockaddr )
{
  const struct sockaddr_in6 *so = sockaddr;

  return so->sin6_scope_id;
}

void rtems_lwip_sysdefs_sockaddr_set_len(
  void *sockaddr,
  int   len
)
{
  struct sockaddr *so = sockaddr;

  so->sa_len = len;
}

void rtems_lwip_sysdefs_sockaddr_set_family(
  void *sockaddr,
  int   family
)
{
  struct sockaddr *so = sockaddr;

  so->sa_family = family;
}

void rtems_lwip_sysdefs_sockaddr_in_set_sin_port(
  void    *sockaddr,
  uint16_t port
)
{
  struct sockaddr_in *so = sockaddr;

  so->sin_port = port;
}

void rtems_lwip_sysdefs_sockaddr_in_set_sin_addr(
  void    *sockaddr,
  uint32_t addr
)
{
  struct sockaddr_in *so = sockaddr;

  so->sin_addr.s_addr = addr;
}

void rtems_lwip_sysdefs_sockaddr_in6_set_sin6_port(
  void    *sockaddr,
  uint16_t port
)
{
  struct sockaddr_in6 *so = sockaddr;

  so->sin6_port = port;
}

void rtems_lwip_sysdefs_sockaddr_in6_set_sin6_flowinfo(
  void    *sockaddr,
  uint32_t flowinfo
)
{
  struct sockaddr_in6 *so = sockaddr;

  so->sin6_flowinfo = flowinfo;
}

void rtems_lwip_sysdefs_sockaddr_in6_set_sin6_scope_id(
  void    *sockaddr,
  uint32_t scope_id
)
{
  struct sockaddr_in6 *so = sockaddr;

  so->sin6_scope_id = scope_id;
}
