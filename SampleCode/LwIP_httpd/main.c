/**************************************************************************//**
 * @file     main.c
 * @version  V1.00
 * $Date: 15/05/15 5:20p $
 * @brief    NUC970 LwIP Sample Code
 *
 * @note
 * Copyright (C) 2015 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "nuc970.h"
#include "sys.h"
#include "netif/ethernetif.h"
#include "netif/etharp.h"
#include "lwip/init.h"
#include "lwip/tcp.h"
#include "lwip/stats.h"
#include "lwip/memp.h"
#include "lwip/timers.h"

//#define USE_DHCP

/* webpage*/
static CHAR index[] = {
    0x48, 0x54, 0x54, 0x50, 0x2f, 0x31, 0x2e, 0x30, 0x20, 0x32, 
    0x30, 0x30, 0x20, 0x4f, 0x4b, 0xd, 0xa, 0x53, 0x65, 0x72, 
    0x76, 0x65, 0x72, 0x3a, 0x20, 0x6c, 0x77, 0x49, 0x50, 0x2f, 
    0x70, 0x72, 0x65, 0x2d, 0x30, 0x2e, 0x36, 0x20, 0x28, 0x68, 
    0x74, 0x74, 0x70, 0x3a, 0x2f, 0x2f, 0x77, 0x77, 0x77, 0x2e, 
    0x73, 0x69, 0x63, 0x73, 0x2e, 0x73, 0x65, 0x2f, 0x7e, 0x61, 
    0x64, 0x61, 0x6d, 0x2f, 0x6c, 0x77, 0x69, 0x70, 0x2f, 0x29, 
    0xd, 0xa, 0x43, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74, 0x2d, 
    0x74, 0x79, 0x70, 0x65, 0x3a, 0x20, 0x74, 0x65, 0x78, 0x74, 
    0x2f, 0x68, 0x74, 0x6d, 0x6c, 0xd, 0xa, 0xd, 0xa, 0x3c, 
    0x48, 0x54, 0x4d, 0x4c, 0x3e, 0xd, 0xa, 0x3c, 0x42, 0x4f, 
    0x44, 0x59, 0x3e, 0xd, 0xa, 0x4e, 0x55, 0x43, 0x39, 0x37, 
    0x30, 0x20, 0x77, 0x65, 0x62, 0x20, 0x73, 0x65, 0x72, 0x76, 
    0x65, 0x72, 0x20, 0x64, 0x65, 0x6d, 0x6f, 0x20, 0x62, 0x61, 
    0x73, 0x65, 0x64, 0x20, 0x6f, 0x6e, 0x20, 0x4c, 0x77, 0x49, 
    0x50, 0xd, 0xa, 0x3c, 0x70, 0x3e, 0xd, 0xa, 0x54, 0x68, 
    0x65, 0x20, 0x4e, 0x55, 0x43, 0x39, 0x37, 0x30, 0x20, 0x73, 
    0x65, 0x72, 0x69, 0x65, 0x73, 0x20, 0x74, 0x61, 0x72, 0x67, 
    0x65, 0x74, 0x65, 0x64, 0x20, 0x66, 0x6f, 0x72, 0x20, 0x67, 
    0x65, 0x6e, 0x65, 0x72, 0x61, 0x6c, 0x20, 0x70, 0x75, 0x72, 
    0x70, 0x6f, 0x73, 0x65, 0x20, 0x33, 0x32, 0x2d, 0x62, 0x69, 
    0x74, 0x20, 0x6d, 0x69, 0x63, 0x72, 0x6f, 0x63, 0x6f, 0x6e, 
    0x74, 0x72, 0x6f, 0x6c, 0x6c, 0x65, 0x72, 0x20, 0xd, 0xa, 
    0x65, 0x6d, 0x62, 0x65, 0x64, 0x73, 0x20, 0x61, 0x6e, 0x20, 
    0x6f, 0x75, 0x74, 0x73, 0x74, 0x61, 0x6e, 0x64, 0x69, 0x6e, 
    0x67, 0x20, 0x43, 0x50, 0x55, 0x20, 0x63, 0x6f, 0x72, 0x65, 
    0x20, 0x41, 0x52, 0x4d, 0x39, 0x32, 0x36, 0x45, 0x4a, 0x2d, 
    0x53, 0x2c, 0x20, 0x61, 0x20, 0x52, 0x49, 0x53, 0x43, 0x20, 
    0x70, 0x72, 0x6f, 0x63, 0x65, 0x73, 0x73, 0x6f, 0x72, 0x20, 
    0x64, 0x65, 0x73, 0x69, 0x67, 0x6e, 0x65, 0x64, 0x20, 0xd, 
    0xa, 0x62, 0x79, 0x20, 0x41, 0x64, 0x76, 0x61, 0x6e, 0x63, 
    0x65, 0x64, 0x20, 0x52, 0x49, 0x53, 0x43, 0x20, 0x4d, 0x61, 
    0x63, 0x68, 0x69, 0x6e, 0x65, 0x73, 0x20, 0x4c, 0x74, 0x64, 
    0x2e, 0x2c, 0x20, 0x72, 0x75, 0x6e, 0x73, 0x20, 0x75, 0x70, 
    0x20, 0x74, 0x6f, 0x20, 0x33, 0x30, 0x30, 0x20, 0x4d, 0x48, 
    0x7a, 0x2c, 0x20, 0x77, 0x69, 0x74, 0x68, 0x20, 0x31, 0x36, 
    0x20, 0x4b, 0x42, 0x20, 0x49, 0x2d, 0x63, 0x61, 0x63, 0x68, 
    0x65, 0x2c, 0x20, 0xd, 0xa, 0x31, 0x36, 0x20, 0x4b, 0x42, 
    0x20, 0x44, 0x2d, 0x63, 0x61, 0x63, 0x68, 0x65, 0x20, 0x61, 
    0x6e, 0x64, 0x20, 0x4d, 0x4d, 0x55, 0x2c, 0x20, 0x35, 0x36, 
    0x4b, 0x42, 0x20, 0x65, 0x6d, 0x62, 0x65, 0x64, 0x64, 0x65, 
    0x64, 0x20, 0x53, 0x52, 0x41, 0x4d, 0x20, 0x61, 0x6e, 0x64, 
    0x20, 0x31, 0x36, 0x20, 0x4b, 0x42, 0x20, 0x49, 0x42, 0x52, 
    0x20, 0x28, 0x49, 0x6e, 0x74, 0x65, 0x72, 0x6e, 0x61, 0x6c, 
    0x20, 0x42, 0x6f, 0x6f, 0x74, 0x20, 0xd, 0xa, 0x52, 0x4f, 
    0x4d, 0x29, 0x20, 0x66, 0x6f, 0x72, 0x20, 0x62, 0x6f, 0x6f, 
    0x74, 0x69, 0x6e, 0x67, 0x20, 0x66, 0x72, 0x6f, 0x6d, 0x20, 
    0x65, 0x4d, 0x4d, 0x43, 0x2c, 0x20, 0x55, 0x53, 0x42, 0x2c, 
    0x20, 0x4e, 0x41, 0x4e, 0x44, 0x20, 0x61, 0x6e, 0x64, 0x20, 
    0x53, 0x50, 0x49, 0x20, 0x46, 0x4c, 0x41, 0x53, 0x48, 0x2e, 
    0x20, 0xd, 0xa, 0x3c, 0x2f, 0x70, 0x3e, 0xd, 0xa, 0x3c, 
    0x2f, 0x42, 0x4f, 0x44, 0x59, 0x3e, 0xd, 0xa, 0x3c, 0x2f, 
    0x48, 0x54, 0x4d, 0x4c, 0x3e, 0xd, 0xa, };
static CHAR err404[] = {
    0x48, 0x54, 0x54, 0x50, 0x2f, 0x31, 0x2e, 0x30, 0x20, 0x34, 
    0x30, 0x34, 0x20, 0x46, 0x69, 0x6c, 0x65, 0x20, 0x6e, 0x6f, 
    0x74, 0x20, 0x66, 0x6f, 0x75, 0x6e, 0x64, 0xd, 0xa, 0x53, 
    0x65, 0x72, 0x76, 0x65, 0x72, 0x3a, 0x20, 0x6c, 0x77, 0x49, 
    0x50, 0x2f, 0x70, 0x72, 0x65, 0x2d, 0x30, 0x2e, 0x36, 0x20, 
    0x28, 0x68, 0x74, 0x74, 0x70, 0x3a, 0x2f, 0x2f, 0x77, 0x77, 
    0x77, 0x2e, 0x73, 0x69, 0x63, 0x73, 0x2e, 0x73, 0x65, 0x2f, 
    0x7e, 0x61, 0x64, 0x61, 0x6d, 0x2f, 0x6c, 0x77, 0x69, 0x70, 
    0x2f, 0x29, 0xd, 0xa, 0x43, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 
    0x74, 0x2d, 0x74, 0x79, 0x70, 0x65, 0x3a, 0x20, 0x74, 0x65, 
    0x78, 0x74, 0x2f, 0x68, 0x74, 0x6d, 0x6c, 0xd, 0xa, 0xd, 
    0xa, 0x3c, 0x48, 0x54, 0x4d, 0x4c, 0x3e, 0xd, 0xa, 0x3c, 
    0x42, 0x4f, 0x44, 0x59, 0x3e, 0xd, 0xa, 0x45, 0x52, 0x52, 
    0x4f, 0x52, 0x20, 0x2d, 0x20, 0x46, 0x69, 0x6c, 0x65, 0x20, 
    0x6e, 0x6f, 0x74, 0x20, 0x66, 0x6f, 0x75, 0x6e, 0x64, 0xd, 
    0xa, 0x3c, 0x2f, 0x42, 0x4f, 0x44, 0x59, 0x3e, 0xd, 0xa, 
    0x3c, 0x2f, 0x48, 0x54, 0x4d, 0x4c, 0x3e, 0xd, 0xa
};

unsigned char my_mac_addr0[6] = {0x00, 0x00, 0x00, 0x55, 0x66, 0x77};
unsigned char my_mac_addr1[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x01}; // Not used in this sample, but provide a dummy avoid compilation errro
/* recv callback function */
static err_t http_recv(void *arg, struct tcp_pcb *pcb, struct pbuf*p, err_t err)
{
    CHAR *rq;
    /* If we got a NULL pbuf in p, the remote end has closed
    the connection.*/
    if (p != NULL)
    {

        /* The payload pointer in the pbuf contains the data
        in the TCP segment.*/

        rq = p->payload;
        /* Get and response the request file. 
           To support other webpage, extent the if statement below 
           If the object is larger than single TCP payload, need to send reset
           for the content in main loop and then close the pcb */
        if ((strncmp(rq, "GET /index.htm", 14) == 0)||(strncmp(rq, "GET / ", 6) == 0)) {
            /* Send the webpage to the remote host. A zero
            in the last argument means that the data should
            not be copied into internal buffers. */
            tcp_recved(pcb, p->tot_len);
            tcp_write(pcb, index, sizeof(index) -1, 0);
            tcp_close(pcb);
        } else {    // err 404
            tcp_recved(pcb, p->tot_len);
            tcp_write(pcb, err404, sizeof(err404) -1, 0);
            tcp_close(pcb);
        }
        /* Free the pbuf. */
        pbuf_free(p);
    }

    return(ERR_OK);
}

/*accept callback function */
static err_t http_acpt(void *arg,struct tcp_pcb *pcb, err_t err)
{
    /* Setup the function http_recv() to be called when data arrives.*/
    tcp_recv(pcb, http_recv);
    return ERR_OK;
}

/*httpd initialization function.*/
static void httpd_init(void)
{
    struct tcp_pcb *pcb, *pcb_listen;
    /*Create a new TCP PCB.*/
    pcb = tcp_new();
    /*Bind the PCB to TCP port 80.*/
    if (tcp_bind(pcb,NULL,80) != ERR_OK)
        sysprintf("bind error\n");
    /*Change TCP state to LISTEN.*/
    pcb_listen = tcp_listen(pcb);
    /*Setup http_accet() function to be called
    when a new connection arrives.*/
    tcp_accept(pcb_listen, http_acpt);
}


struct ip_addr gw, ipaddr, netmask;
struct netif netif;

/* network initialization function.*/
static void net_init(void)
{
    // LwIP needs a timer @ 100Hz. To use another timer source, please modify sys_now() in sys_arch.c as well
    sysStartTimer(TIMER0, 100, PERIODIC_MODE);  
    
#ifdef USE_DHCP

    IP4_ADDR(&gw, 0, 0, 0, 0);
    IP4_ADDR(&ipaddr, 0, 0, 0, 0);
    IP4_ADDR(&netmask, 0, 0, 0, 0);

#else

    IP4_ADDR(&gw, 192, 168, 1, 1);
    IP4_ADDR(&ipaddr, 192, 168, 1, 227);
    IP4_ADDR(&netmask, 255, 255, 255, 0);

#endif

    lwip_init();
    // To use EMAC1, change ethernetif_init0 to ethernetif_init1
    netif_add(&netif, &ipaddr, &netmask, &gw, NULL, ethernetif_init0, ethernet_input);    
    netif_set_default(&netif);

#ifdef USE_DHCP
    dhcp_start(&netif);
#else
    netif_set_up(&netif);
#endif
}



int main(void)
{
		int counter=0;
	
    // Disable all interrupts.
    outpw(REG_AIC_MDCR, 0xFFFFFFFE);
    outpw(REG_AIC_MDCRH, 0x3FFFFFFF);

    sysDisableCache();
    sysFlushCache(I_D_CACHE);
    sysEnableCache(CACHE_WRITE_BACK);
    sysInitializeUART();
    
		sysprintf("[%s %d]\r\n", __func__, __LINE__);
    net_init();
//		sysprintf("[%s %d]\r\n", __func__, __LINE__);
//    httpd_init();
//		sysprintf("[%s %d]\r\n", __func__, __LINE__);

    while (1)
    {
			//sysprintf("[%s %d]\r\n", __func__, __LINE__);
			ETH0_RX_NAPI_SIM ();
			sys_check_timeouts();  // All network traffic is handled in interrupt handler
			if((counter++)%1000==999) print_stat();
		}
}

