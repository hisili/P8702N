/*
 Copyright 2002-2010 Broadcom Corp. All Rights Reserved.

 <:label-BRCM:2011:DUAL/GPL:standard    
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License, version 2, as published by
 the Free Software Foundation (the "GPL").
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 
 A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
 writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 Boston, MA 02111-1307, USA.
 
 :>
*/
#ifndef _BCMENET_COMMON_H_
#define _BCMENET_COMMON_H_

#if defined(_BCMENET_LOCAL_)
#include "bcm_OS_Deps.h"
#include "bcmtypes.h"
#include "bcmmii.h"
#endif

#include "bcm_mm.h"

#if (defined(CONFIG_BCM_FAP) || defined(CONFIG_BCM_FAP_MODULE))
    #define _CONFIG_BCM_FAP
#endif

#if (defined(CONFIG_BCM_BPM) || defined(CONFIG_BCM_BPM_MODULE))
    #define _CONFIG_BCM_BPM
#endif

#if (defined(CONFIG_BCM_INGQOS) || defined(CONFIG_BCM_INGQOS_MODULE))
    #define _CONFIG_BCM_INGQOS
#endif

#if defined(CONFIG_BCM_PKTCMF) || defined(CONFIG_BCM_PKTCMF_MODULE)
    #define _CONFIG_BCM_PKTCMF
#endif

#if (defined(CONFIG_BCM_ARL) || defined(CONFIG_BCM_ARL_MODULE))
    #define _CONFIG_BCM_ARL
#endif

#if defined(CONFIG_BCM_XTMCFG) || defined(CONFIG_BCM_XTMCFG_MODULE)
    #define _CONFIG_BCM_XTMCFG
#endif

#if defined(CONFIG_BCM_ENDPOINT) || defined(CONFIG_BCM_ENDPOINT_MODULE)
    #define _CONFIG_BCM_ENDPOINT
#endif

#if defined(CONFIG_BCM96818) || ((defined(CONFIG_BCM96838) ) && defined(CONFIG_BCM_GPON_STACK_MODULE))
#define ENET_GPON_CONFIG  /* GPON support in Ethernet driver */
#endif

#if defined(CONFIG_BCM_EPON_STACK_MODULE)
#define ENET_EPON_CONFIG
#endif

#ifndef FAP_4KE
#include <linux/skbuff.h>
#include <linux/if_ether.h>
#include <linux/if_vlan.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include "boardparms.h"
#include "bcm_OS_Deps.h"
#include <linux/bcm_log.h>
#include <bcmnet.h>
#include <bcm/bcmswapitypes.h>
#include <linux/version.h>
#endif

#include "bcmtypes.h"

#if defined(CONFIG_BCM_MAX_GEM_PORTS)
#define MAX_MARK_VALUES   32
#define MAX_GEM_IDS       CONFIG_BCM_MAX_GEM_PORTS
#define MAX_GPON_IFS      40
/* The bits[0:6] of status field in DmaDesc are Rx Gem ID. For now, we are
   using only 5 bits */
#define RX_GEM_ID_MASK    0x7F
#endif

#define BCM_PORT_FROM_TYPE2_TAG(tag) (tag & 0x1f) // Big Endian
#define BCM_PORT_FROM_TYPE2_TAG_LE(tag) ((tag >> 8) & 0x1f) // Little Endian

#define VLAN_TYPE           0x8100
typedef struct {
    unsigned char da[6];
    unsigned char sa[6];
    uint16 brcm_type;
    uint32 brcm_tag;
    uint16 vlan_proto;
    uint16 vlan_TCI;
    uint16 encap_proto;
} __attribute__((packed)) BcmVlan_ethhdr;

typedef struct {
    unsigned char da[6];
    unsigned char sa[6];
    uint16 brcm_type;
    uint16 brcm_tag;
    uint16 vlan_proto;
    uint16 vlan_TCI;
    uint16 encap_proto;
} __attribute__((packed)) BcmVlan_ethhdr2;

typedef struct extsw_info_s {
    unsigned int switch_id;
    int brcm_tag_type;
    int accessType;
    int page;
    int bus_num;
    int spi_ss;
    int spi_cid;
    int present;
    int connected_to_internalPort;
} extsw_info_t;

#define BRCM_TYPE2               0x888A // Big Endian
#define BRCM_TYPE2_LE            0x8A88 // Little Endian
#define BRCM_TAG_TYPE2_LEN       4
#define BRCM_TAG2_EGRESS         0x2000
#define BRCM_TAG2_EGRESS_TC_MASK 0x1c00

#if defined(BCM_ENET_CB_WAN_PORT_LNX_INTF_SUPPORT)
#define MAX_NUM_OF_VPORTS   10
#else
#define MAX_NUM_OF_VPORTS   8
#endif
#define BRCM_TAG_LEN        6
#define BRCM_TYPE           0x8874
#define BRCM_TAG_UNICAST    0x00000000
#define BRCM_TAG_MULTICAST  0x20000000
#define BRCM_TAG_EGRESS     0x40000000
#define BRCM_TAG_INGRESS    0x60000000

typedef struct {
    unsigned char da[6];
    unsigned char sa[6];
    uint16 brcm_type;
    uint32 brcm_tag;
    uint16 encap_proto;
} __attribute__((packed)) BcmEnet_hdr;

typedef struct {
    unsigned char da[6];
    unsigned char sa[6];
    uint16 brcm_type;
    uint16 brcm_tag;
    uint16 encap_proto;
} __attribute__((packed)) BcmEnet_hdr2;

/* IEEE 802.3 Ethernet constant */
#define ETH_CRC_LEN             4
#define ETH_MULTICAST_BIT       0x01

#ifndef ERROR
#define ERROR(x)        printk x
#endif
#ifndef ASSERT
#define ASSERT(x)       if (x); else ERROR(("assert: "__FILE__" line %d\n", __LINE__)); 
#endif

#if defined(DUMP_TRACE)
#define TRACE(x)        printk x
#else
#define TRACE(x)
#endif

/* 
    This is general guide of return code across
    Ethernet hardware dependent API. The exact meaning
    of code set depend on each API local context.
*/
  
enum {
    BCMEAPI_CTRL_CONTINUE,  /* Return code to continue execution or loop */
    BCMEAPI_CTRL_SKIP,      /* Return code to skip current execution */
    BCMEAPI_CTRL_BREAK,     /* Return code to break current loop */
    BCMEAPI_CTRL_TRUE,      /* Return code for TRUE result */
    BCMEAPI_CTRL_FALSE,     /* Return code for FALSE result */
};

/*
    The flag which could be set in high bits
    in API return code if needed.
*/
enum {
    BCMEAPI_CTRL_FLAG_TRUE = (1<<31),       /* The TRUE bit */
    BCMEAPI_CTRL_FLAG_MASK = BCMEAPI_CTRL_FLAG_TRUE,    /* The bit mask for all return flag bits */
};

#if defined(CONFIG_BCM96818) || defined(CONFIG_BCM96838)
#define CONFIG_BCMGPON
#endif

typedef struct emac_pm_addr_t {
    BOOL                valid;          /* 1 indicates the corresponding address is valid */
    unsigned int        ref;            /* reference count */
    unsigned char       dAddr[ETH_ALEN];/* perfect match register's destination address */
    char                unused[2];      /* pad */
} emac_pm_addr_t;                    
#define MAX_PMADDR          4           /* # of perfect match address */

#if defined(CONFIG_BCMGPON)
#define BcmEnetDevGponBaseClass_s struct { \
    /* For gpon virtual interfaces */ \
    int gem_count;       /* Number of gem ids */ \
    int gponifid;   /* Unique ifindex in [0:31] for gpon virtual interface  */ \
}
#else
#define BcmEnetDevGponBaseClass_s
#endif

#define BcmEnetDevctrlBaseClass_s \
struct { \
    struct net_device *dev;             /* ptr to net_device */ \
    struct net_device *next_dev;        /* next device */ \
    struct net_device_stats stats;      /* statistics used by the kernel */ \
    struct tasklet_struct task;         /* tasklet */ \
    int             linkState;          /* link status */ \
    int             wanPort;            /* wan port selection */ \
    int             unit;               /* device control index */ \
    unsigned int    vid; \
    uint16          chipId;             /* chip's id */ \
    uint16          chipRev;            /* step */ \
 \
    spinlock_t ethlock_tx; \
    spinlock_t ethlock_moca_tx; \
    spinlock_t ethlock_rx; \
 \
    emac_pm_addr_t  pmAddr[MAX_PMADDR]; /* perfect match address */ \
    extsw_info_t  *extSwitch;          /* external switch */ \
    ETHERNET_MAC_INFO EnetInfo[BP_MAX_ENET_MACS]; \
    IOCTL_MIB_INFO MibInfo; \
 \
    int sw_port_id; /* Physical port index of the Ethernet switch  */ \
    int vport_id;   /* Unique id of virtual eth interface */ \
    int cb_ext_port; /* Crossbar external port; BP_CROSSBAR_NOT_DEFINED if not applicable */ \
 \
    int default_txq; \
    int use_default_txq; \
 \
    struct task_struct *rx_thread; \
    wait_queue_head_t   rx_thread_wqh; \
    int                 rx_work_avail; \
    struct napi_struct napi; /* obsolete: used in 2.6.30 kernel */ \
 \
    int eee_enable_request_flag[2]; \
    uint16 bulk_rx_lock_active[2];    /* optimization: hold rx lock for \
                                         multiple pkts. */ \
    BcmEnetDevGponBaseClass_s; \
\
    int softSwitchingMap;   /* port map of LAN ports using MIPS for RX and TX */ \
    int txSoftSwitchingMap; /* port map of LAN ports using MIPS for TX, RX through HW */ \
    int stpDisabledPortMap; /* port map of ports with HW stp disabled */ \
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 1)
#define BCMENET_WAKEUP_RXWORKER(x) do { \
           if ((x)->rx_work_avail == 0) { \
               (x)->rx_work_avail = 1; \
               wake_up_interruptible(&((x)->rx_thread_wqh)); }} while (0)
#else
#define BCMENET_WAKEUP_RXWORKER(x) napi_schedule(&((x)->napi))
#endif

typedef struct BcmEnet_devctrl BcmEnet_devctrl;
typedef struct enet_xmit_params EnetXmitParams;

#define enet_xmit_params_base struct { \
    unsigned int len; \
    unsigned int mark; \
    unsigned int priority; \
    unsigned int r_flags; \
    int egress_queue; \
    uint16 port_id; \
    uint8 * data; \
    BcmEnet_devctrl *pDevPriv; \
    struct net_device_stats *vstats; \
    uint32_t blog_chnl, blog_phy;       /* used if CONFIG_BLOG enabled */ \
    int gemid; \
    int is_chained; \
    pNBuff_t pNBuff; \
}

/* Below macros should control the logical switch port number mapping.
 * Currently Internal switch port numbers are 0..7
 * External switch port numbers are 8..15
 * The above numbering is fixed irrespective of compile or run time determination of EXT_SW
 * It is assumed that both internal and external switch have max 8 ports
 */

#define SW_PORT_M   (0x07)
#define SW_PORT_S   (0)
#define SW_UNIT_M   (0x08)
#define SW_UNIT_S   (3)

#define SW_PORTMAP_M (0xFF)
#define SW_PORTMAP_S (0)
#define SW_PORTMAP_EXT_M (0xFF00)
#define SW_PORTMAP_EXT_S (8)

#if defined(CONFIG_BCM_EXT_SWITCH)

#define LOGICAL_PORT_TO_UNIT_NUMBER(port)   ( ((port) & SW_UNIT_M) >> SW_UNIT_S  )
#define LOGICAL_PORT_TO_PHYSICAL_PORT(port) ( (port) & SW_PORT_M )
#define PHYSICAL_PORT_TO_LOGICAL_PORT(port, unit) ( (port) | ((unit << SW_UNIT_S) & SW_UNIT_M) )

#define LOGICAL_PORTMAP_TO_PHYSICAL_PORTMAP(pmap) ( ((pmap) & SW_PORTMAP_EXT_M) ? ( ((pmap) & SW_PORTMAP_EXT_M) >> SW_PORTMAP_EXT_S ) : ((pmap) & SW_PORTMAP_M) )
#define PHYSICAL_PORTMAP_TO_LOGICAL_PORTMAP(pmap, unit) ( ((pmap) & SW_PORTMAP_M) << (unit*SW_PORTMAP_EXT_S) )

#define SET_PORT_IN_LOGICAL_PORTMAP(port, unit)  ( 1 << ( ((port) & SW_PORT_M) + ((unit)*SW_PORTMAP_EXT_S)) )
#define GET_PORTMAP_FROM_LOGICAL_PORTMAP(pmap, unit)  ( (((pmap) >> ((unit)*SW_PORTMAP_EXT_S)) & SW_PORTMAP_M) )

#else  /* No external switch compiled-in */

#define LOGICAL_PORT_TO_UNIT_NUMBER(port)   (0)
#define LOGICAL_PORT_TO_PHYSICAL_PORT(port) ( (port) & SW_PORT_M )
#define PHYSICAL_PORT_TO_LOGICAL_PORT(port, unit) ( (port) & SW_PORT_M )
#define LOGICAL_PORTMAP_TO_PHYSICAL_PORTMAP(pmap) ( (pmap) & SW_PORTMAP_M )
#define PHYSICAL_PORTMAP_TO_LOGICAL_PORTMAP(pmap, unit) ( (pmap) & SW_PORTMAP_M )
#define SET_PORT_IN_LOGICAL_PORTMAP(port, unit)  ( 1 << ((port) & SW_PORT_M) )
#define GET_PORTMAP_FROM_LOGICAL_PORTMAP(pmap, unit)  ( (pmap) & SW_PORTMAP_M)

#endif /* No External switch */

#define IsExternalSwitchUnit(unit)  (unit > 0)
#define IsExternalSwitchPort(port)   IsExternalSwitchUnit(LOGICAL_PORT_TO_UNIT_NUMBER(port))

#define LOGICAL_INT_PORT_START  (0)
#define LOGICAL_INT_PORT_END    (LOGICAL_INT_PORT_START + MAX_SWITCH_PORTS - 1)

#define LOGICAL_EXT_PORT_START  (LOGICAL_INT_PORT_END+1)
#define LOGICAL_EXT_PORT_END    (LOGICAL_EXT_PORT_START + MAX_SWITCH_PORTS - 1)


#define LOGICAL_PORT_START      LOGICAL_INT_PORT_START

// LINUX_PRIORITY_BIT_POS_IN_MARK macro must be in sync with PRIO_LOC_NFMARK
// defined in linux_osl_dslcpe.h
#define LINUX_PRIORITY_BIT_POS_IN_MARK    16
#define LINUX_PRIORITY_BIT_MASK          (0x7 << LINUX_PRIORITY_BIT_POS_IN_MARK)
#define LINUX_GET_PRIO_MARK(MARK)        (((MARK) & LINUX_PRIORITY_BIT_MASK) >> LINUX_PRIORITY_BIT_POS_IN_MARK)
#define LINUX_SET_PRIO_MARK(MARK, PRIO)  (((MARK) & (~LINUX_PRIORITY_BIT_MASK)) | ((PRIO) << LINUX_PRIORITY_BIT_POS_IN_MARK))

#if defined(CONFIG_BCM_EXT_SWITCH)
#define LOGICAL_PORT_END        LOGICAL_EXT_PORT_END
#else
#define LOGICAL_PORT_END        LOGICAL_INT_PORT_END
#endif
int enet_logport_to_phyid(int log_port);
int enet_sw_port_to_phyid(int unit, int phy_port);
int enet_cb_port_to_phyid(int unit, int cb_port);
#define IsPortRgmiiDirect(unit, port) \
    IsRgmiiDirect(enet_sw_port_to_phyid(unit, port))
#define IsPortPhyConnected(unit, port) \
    IsPhyConnected(enet_sw_port_to_phyid(unit, port))
unsigned long enet_get_portmap(unsigned char unit);
unsigned long enet_get_port_flags(unsigned char unit, int port);
unsigned int enet_get_consolidated_portmap(void);

extern struct kmem_cache *enetSkbCache;
#endif /* _BCMENET_COMMON_H_ */
