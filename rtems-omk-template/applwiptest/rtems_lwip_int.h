#ifndef RTEMS_LWIP_INT_H
#define RTEMS_LWIP_INT_H

#include <stdint.h>

const int rtems_lwip_sysdefs_AF_UNSPEC;
const int rtems_lwip_sysdefs_AF_UNIX;
const int rtems_lwip_sysdefs_AF_INET;
const int rtems_lwip_sysdefs_AF_INET6;

const int rtems_lwip_sysdefs_PF_UNSPEC;
const int rtems_lwip_sysdefs_PF_UNIX;
const int rtems_lwip_sysdefs_PF_INET;
const int rtems_lwip_sysdefs_PF_INET6;

const int rtems_lwip_sysdefs_SOCK_STREAM;
const int rtems_lwip_sysdefs_SOCK_DGRAM;
const int rtems_lwip_sysdefs_SOCK_RAW;

const int rtems_lwip_sysdefs_sockaddr_in_size;
const int rtems_lwip_sysdefs_sockaddr_in6_size;

int rtems_lwip_sysdefs_sockaddr_get_len( const void *sockaddr );
int rtems_lwip_sysdefs_sockaddr_get_family( const void *sockaddr );
uint16_t rtems_lwip_sysdefs_sockaddr_in_get_sin_port( const void *sockaddr );
uint32_t rtems_lwip_sysdefs_sockaddr_in_get_sin_addr( const void *sockaddr );
uint16_t rtems_lwip_sysdefs_sockaddr_in6_get_sin6_port( const void *sockaddr );
const uint8_t *rtems_lwip_sysdefs_sockaddr_in6_get_sin6_addr_ptr_const(
  const void *sockaddr );
uint8_t *rtems_lwip_sysdefs_sockaddr_in6_get_sin6_addr_ptr(
  void *sockaddr );
uint32_t rtems_lwip_sysdefs_sockaddr_in6_get_sin6_flowinfo(
  const void *sockaddr );
uint32_t rtems_lwip_sysdefs_sockaddr_in6_get_sin6_scope_id(
  const void *sockaddr );

void rtems_lwip_sysdefs_sockaddr_set_len(
  void *sockaddr,
  int   len
);
void rtems_lwip_sysdefs_sockaddr_set_family(
  void *sockaddr,
  int   family
);
void rtems_lwip_sysdefs_sockaddr_in_set_sin_port(
  void    *sockaddr,
  uint16_t port
);
void rtems_lwip_sysdefs_sockaddr_in_set_sin_addr(
  void    *sockaddr,
  uint32_t addr
);
void rtems_lwip_sysdefs_sockaddr_in6_set_sin6_port(
  void    *sockaddr,
  uint16_t port
);
void rtems_lwip_sysdefs_sockaddr_in6_set_sin6_flowinfo(
  void    *sockaddr,
  uint32_t flowinfo
);
void rtems_lwip_sysdefs_sockaddr_in6_set_sin6_scope_id(
  void    *sockaddr,
  uint32_t scope_id
);

#endif /*RTEMS_LWIP_INT_H*/