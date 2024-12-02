/*
  ***************************************************************************************************************
  ***************************************************************************************************************
  ***************************************************************************************************************

  File:		  udpServerRAW.c
  Author:     ControllersTech.com
  Updated:    Jul 23, 2021

  ***************************************************************************************************************
  Copyright (C) 2017 ControllersTech.com

  This is a free software under the GNU license, you can redistribute it and/or modify it under the terms
  of the GNU General Public License version 3 as published by the Free Software Foundation.
  This software library is shared with public for educational purposes, without WARRANTY and Author is not liable for any damages caused directly
  or indirectly by this software, read more about this on the GNU General Public License.

  ***************************************************************************************************************
*/


#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"

#include "stdio.h"
#include "udpServerRAW.h"


void udp_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);


/* IMPLEMENTATION FOR UDP Server :   source:https://www.geeksforgeeks.org/udp-server-client-implementation-c/

1. Create UDP socket.
2. Bind the socket to server address.
3. Wait until datagram packet arrives from client.
4. Process the datagram packet and send a reply to client.
5. Go back to Step 3.
*/

//void udpServer_init(void)
//{
//	// UDP Control Block structure
//   struct udp_pcb *upcb;
//   err_t err;
//
//   /* 1. Create a new UDP control block  */
//   upcb = udp_new();
//
//   /* 2. Bind the upcb to the local port */
//   ip_addr_t myIPADDR;
//   IP_ADDR4(&myIPADDR, 192, 168, 1, 110);
//
//   err = udp_bind(upcb, &myIPADDR, 10);  // 7 is the server UDP port
//
//
//   /* 3. Set a receive callback for the upcb */
//   if(err == ERR_OK)
//   {
//	   udp_recv(upcb, udp_receive_callback, NULL);
//   }
//   else
//   {
//	   udp_remove(upcb);
//   }
//}

// udp_receive_callback will be called, when the client sends some data to the server
/* 4. Process the datagram packet and send a reply to client. */
void udp_receive_callback(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, uint16_t port) {
    uint8_t response[256];
    uint16_t response_length = 0;

    // Step 1: Handle Modbus request
    handle_modbus_request((uint8_t *)p->payload, p->len, response, &response_length);

    // Step 2: Send response if valid
    if (response_length > 0) {
        struct pbuf *resp_buf = pbuf_alloc(PBUF_TRANSPORT, response_length, PBUF_RAM);
        if (resp_buf != NULL) {
            memcpy(resp_buf->payload, response, response_length);
            udp_sendto(pcb, resp_buf, addr, port);
            pbuf_free(resp_buf);
        }
    }

    // Free the received packet
    pbuf_free(p);
}
