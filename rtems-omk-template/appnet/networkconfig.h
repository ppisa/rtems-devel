#ifndef __NETWORKCONFIG_H__
#define __NETWORKCONFIG_H__

#include <rtems/rtems_bsdnet.h>
#include <rtems/rtems_dhcp_failsafe.h>
#include <bsp.h>

static char ethernet_address[6] = {0x00, 0x04, 0x9F, 0x00, 0x27, 0x50 };

int rtems_fxp_attach(struct rtems_bsdnet_ifconfig *config, int attaching);
int rtems_vtnet_driver_attach(struct rtems_bsdnet_ifconfig *config, int attaching);

#if 1
static struct rtems_bsdnet_ifconfig netdriver_config = {
	.name = RTEMS_BSP_NETWORK_DRIVER_NAME,
	.attach = RTEMS_BSP_NETWORK_DRIVER_ATTACH,
	.next = NULL,
	.ip_address = "192.168.3.66",
	.ip_netmask = "255.255.255.0",
	.hardware_address = ethernet_address,
	.ignore_broadcast = 0,
	.mtu = 0,
	.rbuf_count = 0,
	.xbuf_count = 0,
	.port = 0,
	.irno = 0,
	.bpar = 0,
	.drv_ctrl = NULL
};
#elif 1
static struct rtems_bsdnet_ifconfig netdriver_config = {
	.name = "fxp1" /*RTEMS_BSP_NETWORK_DRIVER_NAME*/,
	.attach = rtems_fxp_attach /*RTEMS_BSP_NETWORK_DRIVER_ATTACH*/,
	.next = NULL,
	.ip_address = "192.168.3.66",
	.ip_netmask = "255.255.255.0",
	.hardware_address = ethernet_address,
	.ignore_broadcast = 0,
	.mtu = 0,
	.rbuf_count = 0,
	.xbuf_count = 0,
	.port = 0,
	.irno = 0,
	.bpar = 0,
	.drv_ctrl = NULL
};
#elif 1
static struct rtems_bsdnet_ifconfig netdriver_config = {
	.name = "vio0" /*RTEMS_BSP_NETWORK_DRIVER_NAME*/,
	.attach = rtems_vtnet_driver_attach /*RTEMS_BSP_NETWORK_DRIVER_ATTACH*/,
	.next = NULL,
	.ip_address = "192.168.3.66",
	.ip_netmask = "255.255.255.0",
	.hardware_address = ethernet_address,
	.ignore_broadcast = 0,
	.mtu = 0,
	.rbuf_count = 0,
	.xbuf_count = 0,
	.port = 0,
	.irno = 0,
	.bpar = 0,
	.drv_ctrl = NULL
};
#else
static struct rtems_bsdnet_ifconfig netdriver_config = {
	.name = (char *)BSP_NE2000_NETWORK_DRIVER_NAME,
	.attach = BSP_NE2000_NETWORK_DRIVER_ATTACH,
	.next = NULL,
	.ip_address = "192.168.3.66",
	.ip_netmask = "255.255.255.0",
	.hardware_address = ethernet_address,
	.ignore_broadcast = 0,
	.mtu = 0,
	.rbuf_count = 0,
	.xbuf_count = 0,
	.port = 0x300,
	.irno = 9,
	.bpar = 0,
	.drv_ctrl = NULL
};
#endif

struct rtems_bsdnet_config rtems_bsdnet_config = {
	.ifconfig = &netdriver_config,
/*	.bootp = rtems_bsdnet_do_bootp,*/
/*	.bootp = rtems_bsdnet_do_dhcp,*/
#if 1
	.bootp = rtems_bsdnet_do_dhcp_failsafe,
#endif
	.network_task_priority = 0,		/* 100        */
#if 1
	.mbuf_bytecount = 256 * 1024,		/* 64 kbytes  */
	.mbuf_cluster_bytecount = 256 * 1024,	/* 128 kbytes */
#else
	.mbuf_bytecount = 2048 * 1024,		/* 2 MB */
	.mbuf_cluster_bytecount = 2048 * 1024,	/* 2 MB */
#endif
	.hostname = NULL,
	.domainname = NULL,
	.gateway = "192.168.3.1",
	.log_host = NULL,
	.name_server = { "NULL" },
	.ntp_server = { "NULL" },
	.sb_efficiency = 0,
	.udp_tx_buf_size = 0,			/* UDP TX: 9216 bytes */
	.udp_rx_buf_size = 0,			/* UDP RX: 40 * (1024 + sizeof(struct sockaddr_in)) */
	.tcp_tx_buf_size = 0,			/* TCP TX: 16 * 1024 bytes */
	.tcp_rx_buf_size = 0,			/* TCP TX: 16 * 1024 bytes */
};

#endif

