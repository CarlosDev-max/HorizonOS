#pragma once

// lwIP configuration for HorizonOS
// Required by pico_cyw43_arch_lwip_threadsafe_background.
// Keep minimal — we only need basic WiFi station mode.

#define NO_SYS                      1
#define LWIP_SOCKET                 0
#define LWIP_NETCONN                0

// TCP/IP
#define LWIP_TCP                    1
#define LWIP_UDP                    1
#define LWIP_DNS                    1
#define LWIP_DHCP                   1
#define LWIP_AUTOIP                 0
#define LWIP_IGMP                   0
#define LWIP_ICMP                   1

// Buffers
#define MEM_SIZE                    4000
#define TCP_MSS                     1460
#define TCP_WND                     (4 * TCP_MSS)
#define TCP_SND_BUF                 (2 * TCP_MSS)
#define TCP_SND_QUEUELEN            8
#define MEMP_NUM_TCP_SEG            TCP_SND_QUEUELEN

// ARP
#define LWIP_ARP                    1
#define ARP_TABLE_SIZE              4
#define ARP_MAXAGE                  60

// Ethernet
#define LWIP_ETHERNET               1
#define LWIP_NETIF_HOSTNAME         1
#define LWIP_NETIF_API              0

// Pico/FreeRTOS integration
#define LWIP_SUPPORT_CUSTOM_PBUF    1
#define LWIP_TIMEVAL_PRIVATE        0
#define LWIP_NUM_NETIF_CLIENT_DATA  1

// Debug (disable in production)
#define LWIP_DEBUG                  0
#define ETHARP_DEBUG                LWIP_DBG_OFF
#define NETIF_DEBUG                 LWIP_DBG_OFF
#define PBUF_DEBUG                  LWIP_DBG_OFF
#define IP_DEBUG                    LWIP_DBG_OFF
#define TCP_DEBUG                   LWIP_DBG_OFF
#define UDP_DEBUG                   LWIP_DBG_OFF
#define DHCP_DEBUG                  LWIP_DBG_OFF
