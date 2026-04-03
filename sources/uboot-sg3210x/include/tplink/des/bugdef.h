/* $Header: /usr/cvsroot/target/h/wrn/wm/common/bugdef.h,v 1.2 2003/01/16 18:20:10 josh Exp $ */

/*
 * Copyright (C) 1999-2004 Wind River Systems, Inc.
 * All rights reserved.  Provided under license only.
 * Distribution or other use of this software is only
 * permitted pursuant to the terms of a license agreement
 * from Wind River Systems (and is otherwise prohibited).
 * Refer to that license agreement for terms of use.
 */


/****************************************************************************
 *  Copyright 1995-1997 Epilogue Technology Corporation.
 *  Copyright 1998-1999 Integrated Systems, Inc.
 *  All rights reserved.
*****************************************************************************/

/*
 * $Log: bugdef.h,v $
 * Revision 1.2  2003/01/16 18:20:10  josh
 * directory structure shifting
 *
 * Revision 1.1.1.1  2001/11/05 17:47:17  tneale
 * Tornado shuffle
 *
 * Revision 1.32  2001/06/12 08:06:02  paul
 * Added BUG_SOCKET_UPCALL_BAD_CV, BUG_SOCKET_UPCALL_CLOSING.
 *
 * Revision 1.31  2001/06/08 19:11:48  paul
 * Added BUG_ATTACHE_PSOS_THREAD_INFO.
 *
 * Revision 1.30  2001/05/31 17:12:18  paul
 * Added BUG_MEMPOOL_TRACE.
 *
 * Revision 1.29  2001/01/19 22:21:27  paul
 * Update copyright.
 *
 * Revision 1.28  2000/12/02 20:23:56  sar
 * Added a series of bugdefs for v6 stuff
 * Added bug_pkt_dup_alloc_failed as it got lost somewhere
 *
 * Revision 1.27  2000/10/16 19:21:43  paul
 * Restore sockets and mempool code.
 *
 * Revision 1.26  2000/03/17 07:13:38  paul
 * Roll IPv6 back to the last released state.
 *
 * Revision 1.25  2000/03/17 00:16:34  meister
 * Update copyright message
 *
 * Revision 1.24  2000/03/13 21:21:44  paul
 * Removed some code that we are no longer working on.
 *
 * Revision 1.23  2000/02/01 22:01:14  tneale
 * Added BUG_SNMP_RCV_TRACE
 *
 * Revision 1.22  1999/11/10 21:57:09  paul
 * Added BUG_IF_SEND_TRACE
 *
 * Revision 1.14  1999/03/08 22:12:58  paul
 *  added a bunch of socket bugdefs
 *
 * Revision 1.13  1999/02/18 04:41:22  wes
 * Sockets merge: Everything Else
 *  - memory pools
 *  - thread support
 *  - port-specific headers
 *
 * Revision 1.12  1998/11/23 20:52:59  wes
 * Make MEDIA_CTL_CHECK_ADDRESS succeed rather than die.
 * Die using BUG instead of abort() for unknown media_ctl ops.
 *
 * Revision 1.11.2.4  1998/10/21 20:22:41  wes
 * Correct comments to reflect new usage of SOCKET_{TRACE,INFO}
 *
 * Revision 1.11.2.3  1998/10/01 22:43:46  wes
 * May the Fleas of a Thousand Camels infest the Nether Regions of
 * Whoever thought of Trigraphs.
 *
 * Revision 1.11.2.2  1998/10/01 21:52:05  wes
 * Use the BUG macro closer to how it was intended to be used
 *
 * Revision 1.11.2.1  1998/09/30 22:05:56  wes
 * Add THREAD_TRACE bugdef
 *
 * Revision 1.11  1998/09/06 08:11:05  sra
 * Packet filtering hooks in Attache, rudimentary firewall
 * implementation in Snark.
 *
 * Revision 1.10  1998/08/07 23:56:47  meister
 * added BUG_ARP_SEND_REQUEST BUG_ARP_SEND BUG_ET_ARP_SEND_TRACE
 *       BUG_ARP_RCV_TRACE
 *
 * Revision 1.9  1998/07/30 22:11:49  meister
 * added BUG_SNARK_PSOSTTY_INFO
 *
 * Revision 1.8  1998/07/24 16:53:02  meister
 * Added BUG_ATTACHE_PSOS_NIDRV_INFO
 *
 * Revision 1.7  1998/07/03 18:37:32  sra
 * Add trace calls to route_host_lookup().
 *
 * Revision 1.6  1998/06/29 22:25:56  sra
 * More trace symbols.
 *
 * Revision 1.5  1998/06/29 03:07:46  sra
 * Add tracing code to et_rcv().
 *
 * Revision 1.4  1998/04/24 18:42:37  wes
 * Two new bug codes for DHCP
 *
 * Revision 1.3  1998/04/16 15:57:46  josh
 * two new bug definitions for Envoy
 *
 * Revision 1.2  1998/02/25 04:43:24  sra
 * Update copyrights.
 *
 * Revision 1.1  1998/02/05 22:19:28  josh
 * moving commonly used include files into common/h from top-level directory
 *
 * Revision 3.26  1997/11/20 17:33:15  sra
 * Clear out a bunch of unused PAP and CHAP bug codes.
 *
 * Revision 3.25  1997/11/19 21:14:42  josh
 * Liaison bug definitions
 *
 * Revision 3.24  1997/11/18 03:18:24  sra
 * Brand new PAP and CHAP code for PPP.
 *
 * Revision 3.23  1997/10/28 00:51:48  sra
 * Convert PPP code to use BUG() macro.
 * Bang on CHAP and PAP code some more.
 * Create test driver for PPP under Snark/BSD.
 *
 * Revision 3.22  1997/10/15 19:49:36  sra
 * Preliminary cleanup of PPP PAP and CHAP modules.
 *
 * Revision 3.21  1997/08/15 22:14:57  lowell
 * PAP/CHAP polishing
 *
 * Revision 3.20  1997/08/14 16:04:33  lowell
 * boilerplate
 *
 * Revision 3.19  1997/07/29 00:18:41  lowell
 * PAP/CHAP messages for logging already in file
 *
 * Revision 3.18  1997/07/17 02:15:56  alan
 * New errors for a new Decorum
 *
 * Revision 3.17  1997/06/17 19:00:20  lowell
 * more extensive TFTP debugging
 *
 * Revision 3.16  1997/05/30 17:00:18  lowell
 * TFTP (got rid of old TFTP_DEBUG)
 *
 * Revision 3.15  1997/05/23 04:13:32  sra
 * Add BUG_IPADDR_TEST_UNKNOWN_CODE.
 *
 * Revision 3.14  1997/05/22 03:35:25  sra
 * Get rid of struct udp_hdr.  Change pkt_rcv_info into an array, convert
 * receive path to use it instead of passing pointer arguments in upcalls.
 *
 * Revision 3.13  1997/05/17 07:39:02  sra
 * Change IP protocol dispatch to use ip_listener table.
 *
 * Revision 3.12  1997/03/20 06:58:16  sra
 * DFARS-safe copyright text.  Zap!
 *
 * Revision 3.11  1997/03/19 04:11:35  sra
 * BUG() now common to all products, so move the associated .h files.
 *
 * Revision 3.10  1997/02/24 14:43:18  josh
 * new bug definitions for Courier RIP and Attache Plus TCP
 *
 * Revision 3.9  1996/10/28 18:19:01  lowell
 * Decorum messages
 * Looks like this needs to be freed of its ownership by Attache...
 *
 * Revision 3.8  1996/03/22  10:04:12  sra
 * Update copyrights prior to Attache 3.2 release.
 *
 * Revision 3.7  1996/03/14  17:51:55  lowell
 * DHCP packet dumper code
 *
 * Revision 3.6  1995/12/02  09:15:04  sra
 * Add a bunch of Courier stuff.
 *
 * Revision 3.5  1995/09/28  23:06:59  sra
 * Fix Courier identifiers and copyrights for release packaging.
 *
 * Revision 3.4  1995/06/30  23:24:10  sra
 * Back out hairy interface to RCI TRACE() to a simpler scheme.
 *
 * Revision 3.3  1995/06/30  22:44:00  sra
 * Add RCI trace bug codes.
 *
 * Revision 3.2  1995/06/30  16:10:40  lowell
 * first few ARP and DHCP bug definitions (more to come)
 *
 * Revision 3.1  1995/06/27  06:20:23  sra
 * Add some debugging code to catch botched packet buffer allocs/frees.
 *
 * Revision 3.0  1995/05/10  22:36:18  sra
 * Release 3.0.
 *
 * Revision 1.1  1995/04/25  21:39:43  sra
 * Initial revision
 *
 */

/* [clearcase]
modification history
-------------------
*/


/*
 * BUGDEF() table (see bug.h)
 *
 * NB: This file intentionally does NOT protect against multiple inclusion.
 *
 * Bug symbol definitions.  This file is included by bug.h,
 * and may be included by other modules as necessary.  It's a separate
 * file to make it easy to build a translation tables, bitvectors, and
 * so forth.
 *
 * There should not be anything in this file except comments and
 * invocations of the BUGDEF() macro.
 *
 * We'll probably add more of these in every release.  Check these
 * with a switch() or if() statement, don't depend on specific numeric
 * values.
 */

BUGDEF(BUG_ASSERTION_FAILED)			/* bug.h */
BUGDEF(BUG_RIP_MBZ_WASNT)			/* attache/net/rip.c */
BUGDEF(BUG_RIP_BAD_ENTRY)			/* attache/net/rip.c */
BUGDEF(BUG_RIP_COULDNT_CREATE_ROUTE)		/* attache/net/rip.c */
BUGDEF(BUG_RIP_COULDNT_CREATE_ROUTE_EXT)	/* attache/net/rip.c */
BUGDEF(BUG_RIP_DURCV)				/* attache/net/rip.c */
BUGDEF(BUG_RIP_NO_BUFFERS)			/* attache/net/rip.c */
BUGDEF(BUG_RIP_SKIPPING_ADDRESS)		/* attache/net/rip.c */
BUGDEF(BUG_RIP_SKIPPING_BROADCAST_ADDRESS)	/* attache/net/rip.c */
BUGDEF(BUG_RIP_ROUTE_CHANGE)			/* attache/net/rip.c */
BUGDEF(BUG_RIP_CLEAR_ROUTE_CACHE)		/* attache/net/rip.c */
BUGDEF(BUG_RIP_BAD_BROADCAST_ADDRESS)		/* attache/net/rip.c */
BUGDEF(BUG_IPOPT_TOO_LONG)			/* attache/net/ip_opt.c */
BUGDEF(BUG_PKT_INIT_NO_MEMORY)			/* attache/net/packet.c */
BUGDEF(BUG_PKT_ALLOC_ALREADY)			/* attache/net/packet.c */
BUGDEF(BUG_PKT_FREE_ALREADY)			/* attache/net/packet.c */
BUGDEF(BUG_IP_SEND_FREED)			/* attache/net/ip_send.c */
BUGDEF(BUG_ARP_NULL_POINTER)			/* attache/net/arp.c */
BUGDEF(BUG_DHCP_BAD_PACKET)			/* (unused?) */
BUGDEF(BUG_DHCP_PACKET_DUMP)			/* attache/net/dhcp.c */
BUGDEF(BUG_COURIER_TRACE)			/* courier/common/trace_ut.h */
BUGDEF(BUG_COURIER_IPI_ENABLE_FAILED)		/* courier/common/epilogue.c */
BUGDEF(BUG_COURIER_IPI_INIT_FAILED)		/* courier/common/epilogue.c */
BUGDEF(BUG_COURIER_IPI_REENABLE_FAILED)		/* courier/common/epilogue.c */
BUGDEF(BUG_COURIER_OSPF_INIT_FAILED)		/* courier/common/epilogue.c */
BUGDEF(BUG_COURIER_OSPF_RCV_NO_IFB)		/* courier/common/epilogue.c */
BUGDEF(BUG_COURIER_OSPF_RCV_IFB_DOWN)		/* courier/common/epilogue.c */
BUGDEF(BUG_COURIER_RIP_RCV_NO_IFB)              /* courier/common/udp_ut.c */
BUGDEF(BUG_COURIER_RIP_RCV_IFB_DOWN)            /* courier/common/udp_ut.c */
BUGDEF(BUG_IF_ATTACH_IFB_CREATE_FAILED)		/* attache/net/if.c */
BUGDEF(BUG_COURIER_TCP_CANT_ALLOCATE_TCB)	/* courier/common/tcp_ut.c */
BUGDEF(BUG_COURIER_TCP_CANT_BIND_HOST_PASSIVE)	/* courier/common/tcp_ut.c */
BUGDEF(BUG_COURIER_TCP_MULTIPLE_LISTENS)	/* courier/common/tcp_ut.c */
BUGDEF(BUG_COURIER_TCP_START_FAILED)		/* courier/common/tcp_ut.c */
BUGDEF(BUG_COURIER_FATAL_ERROR)			/* courier/sys_incl.h */
BUGDEF(BUG_DECORUM_BAD_DATE_TYPE) 		/* decorum/http/date.c */
BUGDEF(BUG_DECORUM_BAD_VERSION)			/* (unused) */
BUGDEF(BUG_DECORUM_CLIENT_ERROR)		/* decorum/http/http.c */
BUGDEF(BUG_DECORUM_ENV_DUPLICATE_NAME)		/* decorum/http/envcfg.c */
BUGDEF(BUG_DECORUM_INFO)			/* decorum/http/xxx.c */
BUGDEF(BUG_DECORUM_MISSING_INBUF)		/* (unused) */
BUGDEF(BUG_DECORUM_NO_DATE)			/* decorum/http/date.c */
BUGDEF(BUG_DECORUM_UNEXPECTED_ERR) 		/* decorum/http/xxx.c */
BUGDEF(BUG_DECORUM_XATTACHE_ERROR)		/* decorum/http/xattache.c */
BUGDEF(BUG_DECORUM_XATTACHE_INFO)		/* decorum/http/xattache.c */
BUGDEF(BUG_LIAISON_CANT_BIND_OBJECT)            /* liaison/xrmon/xsnmp.c */
BUGDEF(BUG_LIAISON_UNKNOWN_OBJECT)              /* liaison/xrmon/xsnmp.c */
BUGDEF(BUG_LIAISON_CANT_ENCODE_TRAP)            /* liaison/xrmon/xsnmp.c */
BUGDEF(BUG_SNARK_UNEXPECTED_EOF)		/* snark/test/main.c */
BUGDEF(BUG_TCP_ACK_DATA_FREEING)		/* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_ACK_DATA_HUSHING)		/* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_ACK_DATA_SQUEEZING)		/* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_ACK_DATA_FAST_RETRANSMIT_DONE)	/* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_CLOSE_INVALID_CONNECTION)	/* attache/net/tcp.c */
BUGDEF(BUG_TCP_CLOSE_INVALID_STATE)		/* attache/net/tcp.c */
BUGDEF(BUG_TCP_ENTER_FUNC)			/* attache/net/tcpintnl.h */
BUGDEF(BUG_TCP_GET_SND_MSS)			/* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_HANDLE_ACK_DALLY)		/* attache/net/tcp.c */
BUGDEF(BUG_TCP_HANDLE_SWS_DALLY)		/* attache/net/tcp.c */
BUGDEF(BUG_TCP_POST_DATA_AHEAD)			/* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_POST_DATA_OBSOLETE)		/* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_POST_DATA_POSTING)		/* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_PRINT_PKTHDR)			/* attache/net/tcpintnl.h */
BUGDEF(BUG_TCP_RCV_ACKED_UNSENT_DATA)		/* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_RCV_ACKING_UNACCEPTABLE)		/* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_RCV_LOG_PKT)			/* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_RCV_REUSING_SAVED_PKT)		/* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_RCV_SAVING_EARLY_PKT)		/* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_RCV_SCHEDULING_ZERO_WINDOW_PROBE) /* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_RCV_FAST_RETRANSMIT_EQUAL)	/* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_RCV_FAST_RETRANSMIT_GREATER)	/* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_RETRANSMIT_HANDLER)		/* attache/net/tcp_send.c */
BUGDEF(BUG_TCP_RTT_LOG_1)			/* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_RTT_LOG_2)			/* attache/net/tcp_rcv.c */
BUGDEF(BUG_TCP_SEND_COPYING)			/* attache/net/tcp_send.c */
BUGDEF(BUG_TCP_SEND_DECIDED_TO_SEND)		/* attache/net/tcp_send.c */
BUGDEF(BUG_TCP_SEND_ENTRY_LOG)			/* attache/net/tcp_send.c */
BUGDEF(BUG_TCP_SEND_FORBIDDEN_BY_SWS)		/* attache/net/tcp_send.c */
BUGDEF(BUG_TCP_SEND_LOG_PKT)			/* attache/net/tcp_send.c */
BUGDEF(BUG_TCP_SEND_NEW_DATA_BEYOND_WINDOW)	/* attache/net/tcp_send.c */
BUGDEF(BUG_TCP_SEND_NOTHING_TO_DO)		/* attache/net/tcp_send.c */
BUGDEF(BUG_TCP_SEND_PKTALLOC_FAILED)		/* attache/net/tcp_send.c */
BUGDEF(BUG_TCP_SEND_SCHEDULING_RETRANSMISSION)	/* attache/net/tcp_send.c */
BUGDEF(BUG_TCP_SET_ACK_DALLY)			/* attache/net/tcp.c */
BUGDEF(BUG_TCP_SET_RCV_MSS)			/* attache/net/tcp_send.c */
BUGDEF(BUG_TCP_SET_SWS_DALLY)			/* attache/net/tcp.c */
BUGDEF(BUG_TCP_SET_TIME_WAIT)			/* attache/net/tcp.c */
BUGDEF(BUG_TFTP_BAD_PORT)			/* attache/net/tftpc.c */
BUGDEF(BUG_TFTP_CNXN_NOTFOUND)			/* attache/net/tftpc.c */
BUGDEF(BUG_TFTP_ALLOC_PKT_FAIL)			/* attache/net/tftpc.c */
BUGDEF(BUG_TFTP_ALLOC_MEM_FAIL)			/* attache/net/tftpc.c */
BUGDEF(BUG_TFTP_TIMEOUT)			/* attache/net/tftpc.c */
BUGDEF(BUG_TFTP_WRONG_ACK)			/* attache/net/tftpc.c */
BUGDEF(BUG_TFTP_WRONG_BLOCK)			/* attache/net/tftpc.c */
BUGDEF(BUG_IP_RCV_UNRECOGNIZED_IP_VERSION)	/* attache/net/ip_rcv.c */
BUGDEF(BUG_PKT_APPEND_OVERFLOW)			/* attache/h/packet.h */
BUGDEF(BUG_IPADDR_TEST_UNKNOWN_CODE)		/* attache/net/ipaddr.c */
BUGDEF(BUG_PPP_CHAP_PKT_ALLOC_FAILED)		/* attache/net/pppchap.c */
BUGDEF(BUG_ATTACHE_PPP_LCP_BAD_TIMEOUT)		/* attache/net/ppp.c */
BUGDEF(BUG_ATTACHE_PPP_IPCP_BAD_TIMEOUT)	/* attache/net/ppp.c */
BUGDEF(BUG_ATTACHE_PPP_LCP_EVENT)		/* attache/net/ppp.c */
BUGDEF(BUG_ATTACHE_PPP_IPCP_EVENT)		/* attache/net/ppp.c */
BUGDEF(BUG_SNARK_PPP_AUTH_STATUS)		/* snark/lib/netconf.c */
BUGDEF(BUG_PPP_CHAP_CHALLENGE_TIMEOUT)		/* attache/net/pppchap.c */
BUGDEF(BUG_PPP_CHAP_SERVER_UNEXPECTED_STATE)	/* attache/net/pppchap.c */
BUGDEF(BUG_PPP_CHAP_CLIENT_UNEXPECTED_STATE)	/* attache/net/pppchap.c */
BUGDEF(BUG_PPP_AUTH_ALLOC_FAILED)		/* attache/net/pppauth.c */
BUGDEF(BUG_PPP_AUTH_BAD_FLAGS)			/* attache/net/pppauth.c */
BUGDEF(BUG_PPP_CHAP_SERVER_ENABLED)		/* attache/net/ppplcp.c */
BUGDEF(BUG_PPP_PAP_SERVER_ENABLED)		/* attache/net/ppplcp.c */
BUGDEF(BUG_PPP_NEGOTIATING_CHAP)		/* attache/net/ppplcp.c */
BUGDEF(BUG_PPP_NEGOTIATING_PAP)			/* attache/net/ppplcp.c */
BUGDEF(BUG_PPP_LCP_UNKNOWN_NAK)			/* attache/net/ppplcp.c */
BUGDEF(BUG_PPP_LCP_UNKNOWN_REQ)			/* attache/net/ppplcp.c */
BUGDEF(BUG_PPP_LCP_RCV)				/* attache/net/ppplcp.c */
BUGDEF(BUG_PPP_PAP_CLIENT_UNEXPECTED_STATE)	/* attache/net/ppppap.c */
BUGDEF(BUG_PPP_PAP_PKT_ALLOC_FAILED)		/* attache/net/ppppap.c */
BUGDEF(BUG_PPP_PAP_REQUEST_TIMEOUT)		/* attache/net/ppppap.c */
BUGDEF(BUG_PPP_PAP_SERVER_UNEXPECTED_STATE)	/* attache/net/ppppap.c */
BUGDEF(BUG_ENVOY_LOCKING)                       /* envoy/snmp */
BUGDEF(BUG_ENVOY_INSUFFICIENT_MEMORY)           /* envoy/snmp */
BUGDEF(BUG_DHCP_STATE_TRACE)			/* attache/net/dhcp.c */
BUGDEF(BUG_DHCP_TIMER_TRACE)			/* attache/net/dhcp.c */
BUGDEF(BUG_ET_RCV_TRACE)			/* attache/net/et_rcv.c */
BUGDEF(BUG_ET_SEND_TRACE)			/* attache/net/et_send.c */
BUGDEF(BUG_IF_SEND_TRACE)			/* attache/net/if.c */
BUGDEF(BUG_IP_RCV_TRACE)			/* attache/net/ip_rcv.c */
BUGDEF(BUG_IP_SEND_TRACE)			/* attache/net/ip_send.c */
BUGDEF(BUG_ICMP_RCV_TRACE)			/* attache/net/icmp.c */
BUGDEF(BUG_ICMP_SEND_TRACE)			/* attache/net/icmp.c */
BUGDEF(BUG_IP_RTE_TRACE)			/* attache/net/ip_rte.c */
BUGDEF(BUG_ATTACHE_PSOS_NIDRV_INFO)             /* attache/psos support */
BUGDEF(BUG_SNARK_PSOSTTY_INFO)                  /* snark/psostty.c      */
BUGDEF(BUG_ARP_SEND_REQUEST)                    /* attache/net/arp.c    */
BUGDEF(BUG_ARP_SEND)                            /* attache/net/arp.c    */
BUGDEF(BUG_ET_ARP_SEND_TRACE)                   /* attache/net/et_send.c*/
BUGDEF(BUG_ARP_RCV_TRACE)                       /* attache/net/arp.c    */
BUGDEF(BUG_SNARK_FIREWALL_CONFIG_ALLOC_FAILED)	/* snark/lib/firewall.c */
BUGDEF(BUG_SNARK_FIREWALL_CONFIG_PARSE_ERROR)	/* snark/lib/firewall.c */
BUGDEF(BUG_SNARK_FIREWALL_ALLOW_PKT)		/* snark/lib/firewall.c */
BUGDEF(BUG_SNARK_FIREWALL_DROP_PKT)		/* snark/lib/firewall.c */
BUGDEF(BUG_SNARK_FIREWALL_RESET_PKT)		/* snark/lib/firewall.c */
BUGDEF(BUG_SNARK_TUN_ERROR)			/* snark/lib/tun.c */
BUGDEF(BUG_THREAD_TRACE)		        /* common/lib/thread_debug.c */
BUGDEF(BUG_SOCKET_ENTER_FUNC)		/* attache/sockets/sockintl.h */
BUGDEF(BUG_SOCKET_TRACE)		/* attache/sockets/sockintl.h */
BUGDEF(BUG_SOCKET_ACCEPT_EMFILE)	/* attache/sockets/socktcp.c */
BUGDEF(BUG_SOCKET_ALLOC)		/* attache/sockets/socktcp.c */
BUGDEF(BUG_SOCKET_LISTEN_BACKLOG)	/* attache/sockets/socktcp.c */
BUGDEF(BUG_SOCKET_RCVQ_EMPTY)		/* attache/sockets/socktcp.c */
BUGDEF(BUG_SOCKET_UPCALL_BAD_SOCKET)	/* attache/sockets/socktcp.c */
BUGDEF(BUG_SOCKET_UPCALL_BAD_STATE)	/* attache/sockets/socktcp.c */
BUGDEF(BUG_SOCKET_UPCALL_BAD_CV)	/* attache/sockets/socktcp.c */
BUGDEF(BUG_SOCKET_UPCALL_DFQ_FULL)	/* attache/sockets/socktcp.c */
BUGDEF(BUG_SOCKET_UPCALL_RCVQ_FULL)	/* attache/sockets/socktcp.c */
BUGDEF(BUG_SOCKET_UPCALL_SHUTDOWN)	/* attache/sockets/socktcp.c */
BUGDEF(BUG_SOCKET_UPCALL_CLOSING)	/* attache/sockets/socktcp.c */
BUGDEF(BUG_SOCKET_IOCTL_ADD_ROUTE)	/* attache/sockets/sockif.c */
BUGDEF(BUG_SOCKET_IOCTL_DEL_ROUTE)	/* attache/sockets/sockif.c */
BUGDEF(BUG_SNMP_RCV_TRACE) 		/* attache/net/snmpfunc.c  */
BUGDEF(BUG_IPV6_SEND_TRACE)		/* attache/net/ip6_send.c  */
BUGDEF(BUG_IPV6_SEND_ERROR)		/* attache/net/ip6_send.c  */
BUGDEF(BUG_IPV6_RCV_TRACE)		/* attache/net/ip6_rcv.c   */
BUGDEF(BUG_IPV6_RCV_ERROR)		/* attache/net/ip6_rcv.c   */
BUGDEF(BUG_IPV6_CKSUM_TRACE)		/* attache/net/ip6_cksum.c */
BUGDEF(BUG_IPV6_ICMP_TRACE)		/* attache/net/icmpv6.c    */
BUGDEF(BUG_IPV6_ICMP_ERROR)		/* attache/net/icmpv6.c    */
BUGDEF(BUG_IPV6_DAD_TRACE)		/* attache/net/ip6_cfg.c   */
BUGDEF(BUG_IPV6_DAD_ERROR)		/* attache/net/ip6_cfg.c   */
BUGDEF(BUG_IPV6_ND_TRACE)		/* attache/net/ip6_nd.c    */
BUGDEF(BUG_IPV6_ND_ERROR)		/* attache/net/ip6_nd.c    */
BUGDEF(BUG_IPV6_RD_TRACE)		/* attache/net/ip6_rd.c    */
BUGDEF(BUG_IPV6_RD_ERROR)		/* attache/net/ip6_rd.c    */
BUGDEF(BUG_IPV6_RDRT_TRACE)		/* attache/net/ip6_rdrt.c  */
BUGDEF(BUG_IPV6_RDRT_ERROR)		/* attache/net/ip6_rdrt.c  */
BUGDEF(BUG_IPV6_FRAG_TRACE)		/* attache/net/ip6_frag.c  */
BUGDEF(BUG_IPV6_FRAG_ERROR)		/* attache/net/ip6_frag.c  */
BUGDEF(BUG_PKT_DUP_ALLOC_FAILED)	/* attache/net/ip_rcv.c    */
BUGDEF(BUG_MEMPOOL_TRACE)		/* snark/lib/pool.c */
BUGDEF(BUG_ATTACHE_PSOS_THREAD_INFO)	/* attache/psos/thread.c */
/* End of bugdef.h */
