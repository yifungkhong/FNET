/**************************************************************************
*
* Copyright 2008-2018 by Andrey Butok. FNET Community.
*
***************************************************************************
*
*  Licensed under the Apache License, Version 2.0 (the "License"); you may
*  not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*  http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
*  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
***************************************************************************
*
*  Ethernet driver implementation.
*
***************************************************************************/

#include "fnet.h"
#if FNET_MCF && (FNET_CFG_CPU_ETH0 ||FNET_CFG_CPU_ETH1)
#include "port/netif/fec/fnet_fec.h"

static fnet_return_t fnet_mcf_eth_init(fnet_netif_t *netif);
static fnet_return_t fnet_mcf_eth_phy_init(fnet_netif_t *netif);

#if FNET_CFG_CPU_ETH0
/************************************************************************
* FEC0 interface structure.
*************************************************************************/
struct fnet_eth_if fnet_mcf_eth0_if =
{
    .eth_prv = &fnet_fec0_if,							/* Points to Ethernet driver-specific control data structure. */
    .eth_mac_number = 0,                            	/* MAC module number.*/
    .eth_phy_addr = FNET_CFG_CPU_ETH0_PHY_ADDR,         /* Set default PHY address */
    .eth_output = fnet_fec_output,						/* Ethernet driver output.*/
    .eth_cpu_init = fnet_mcf_eth_init,
    .eth_cpu_phy_init = fnet_mcf_eth_phy_init,
#if FNET_CFG_MULTICAST
    .eth_multicast_join = fnet_fec_multicast_join,		/* Ethernet driver join multicast group.*/
    .eth_multicast_leave = fnet_fec_multicast_leave,	/* Ethernet driver leave multicast group.*/
#endif
};
/************************************************************************
* eth0 Interface structure.
*************************************************************************/
fnet_netif_t fnet_cpu_eth0_if =
{
    .netif_name = FNET_CFG_CPU_ETH0_NAME, /* Network interface name.*/
    .netif_mtu = FNET_CFG_CPU_ETH0_MTU,   /* Maximum transmission unit.*/
    .netif_prv = &fnet_mcf_eth0_if,       /* Points to interface specific data structure.*/
    .netif_api = &fnet_fec_api            /* Interface API */
};
#endif /*FNET_CFG_CPU_ETH0*/

#if FNET_CFG_CPU_ETH1
/************************************************************************
* FEC1 interface structure.
*************************************************************************/
struct fnet_eth_if fnet_mcf_eth1_if =
{
    .eth_prv = &fnet_fec1_if,							/* Points to Ethernet driver-specific control data structure. */
    .eth_mac_number = 1,                  				/* MAC module number.*/
    .eth_phy_addr = FNET_CFG_CPU_ETH1_PHY_ADDR,         /* Set default PHY address */
    .eth_output = fnet_fec_output,						/* Ethernet driver output.*/
    .eth_cpu_init = fnet_mcf_eth_init,
#if FNET_CFG_MULTICAST
    .eth_multicast_join = fnet_fec_multicast_join,		/* Ethernet driver join multicast group.*/
    .eth_multicast_leave = fnet_fec_multicast_leave,	/* Ethernet driver leave multicast group.*/
#endif
};
/************************************************************************
* eth1 Interface structure.
*************************************************************************/
fnet_netif_t fnet_cpu_eth1_if =
{
    .netif_name = FNET_CFG_CPU_ETH1_NAME, /* Network interface name.*/
    .netif_mtu = FNET_CFG_CPU_ETH1_MTU,   /* Maximum transmission unit.*/
    .netif_prv = &fnet_mcf_eth1_if,       /* Points to interface specific data structure.*/
    .netif_api = &fnet_fec_api            /* Interface API */
};
#endif /*FNET_CFG_CPU_ETH0*/

/************************************************************************
* DESCRIPTION: Ethernet IO initialization.
*************************************************************************/
static fnet_return_t fnet_mcf_eth_init(fnet_netif_t *netif)
{
#if FNET_CFG_CPU_ETH_IO_INIT
#if FNET_CFG_CPU_MCF52235 /* Kirin2 */

    FNET_MCF5223X_GPIO_PLDPAR = (0
                                 | FNET_MCF5223X_GPIO_PLDPAR_ACTLED_ACTLED
                                 | FNET_MCF5223X_GPIO_PLDPAR_LINKLED_LINKLED
                                 | FNET_MCF5223X_GPIO_PLDPAR_SPDLED_SPDLED
                                 | FNET_MCF5223X_GPIO_PLDPAR_DUPLED_DUPLED
                                 | FNET_MCF5223X_GPIO_PLDPAR_COLLED_COLLED
                                 | FNET_MCF5223X_GPIO_PLDPAR_RXLED_RXLED
                                 | FNET_MCF5223X_GPIO_PLDPAR_TXLED_TXLED);

#endif

#if FNET_CFG_CPU_MCF52259 /* Kirin3 */

    FNET_MCF5225X_GPIO_PNQPAR = 0x4880; /* 0x4880 (for Gamma environment, reported by a customer).*/
    FNET_MCF5225X_GPIO_PTIPAR = 0xFF;
    FNET_MCF5225X_GPIO_PTJPAR = 0xFF;

#endif

#if FNET_CFG_CPU_MCF5282 /* Reindeer */

    FNET_MCF5282_GPIO_PEHLPAR = 0xC0;
    FNET_MCF5282_GPIO_PASPAR |= 0x0f00; /* Initialize PEHLPAR to enable ethernet signals.*/

#endif

#if FNET_CFG_CPU_MCF5235 /* CPUV2 */

    FNET_MCF523x_GPIO_PAR_FECI2C = 0xF0;

#endif

#if FNET_CFG_CPU_MCF51CN128 /* Lasko */

    /* Enable MII pins by setting Pin Mux Control Registers: */
    /* Port A */
    FNET_MCF_PTAPF1 = 0x55;    /* MII_RXD2, MII_RXD1, MII_RXD0, MII_RX_DV */
    FNET_MCF_PTAPF2 = 0x54;    /* MII_MDIO, MII_MDC, MII_RXD3 */
    /* Port B */
    FNET_MCF_PTBPF1 = 0x55;    /* MII_TX_EN, MII_TXD0, MII_TXD1, MII_TXD2 */
    FNET_MCF_PTBPF2 = 0x55;    /* MII_RX_CLK, MII_RX_ER, MII_TX_ER, MII_TX_CLK */
    /* Port C */
    FNET_MCF_PTCPF2 = 0x15;    /* MII_TXD3, MII_COL, MII_CRS */

    /* Set high output drive strength for output pins: */
    FNET_MCF_PTADS = 0x6;  /* Output Drive Enable for Port A*/
    FNET_MCF_PTBDS = 0xF4; /* Output Drive Enable for Port B*/
    FNET_MCF_PTCDS = 0x1;  /* Output Drive Enable for Port C*/

    /* Enable filter. */
    FNET_MCF_PTAIFE = 0x06;
    FNET_MCF_PTBIFE = 0xf4;
    FNET_MCF_PTCIFE = 1;

#endif


#if FNET_CFG_CPU_MCF54418 /* Modelo */

    /* FEC Pin Assignment. */
#if FNET_CFG_CPU_ETH_RMII /* Default, RMII on both MACs */
    FNET_MCF5441X_GPIO_PAR_FEC = FNET_MCF5441X_GPIO_PAR_FEC_PAR_FEC_RMII0_RMII1_FULL;
#else /* In this case only MAC0 works. MAC1 is not functional.*/
    FNET_MCF5441X_GPIO_PAR_FEC = FNET_MCF5441X_GPIO_PAR_FEC_PAR_FEC_MII_FULL;
#endif

    /* Set slew rate to highest, otherwise data will be corrupted (checked on practice).*/
    FNET_MCF5441X_GPIO_SRCR_FEC = FNET_MCF5441X_GPIO_SRCR_FEC_SRE_RMII1_SR_HIGHEST | FNET_MCF5441X_GPIO_SRCR_FEC_SRE_RMII0_SR_HIGHEST;

    /* Needed for RMMI1 */
    FNET_MCF5441X_GPIO_PDDR_G |= FNET_MCF5441X_GPIO_PDDR_G_PDDR_G4; 	/* Set GPIO4 as output.*/
    FNET_MCF5441X_GPIO_PODR_G &= ~FNET_MCF5441X_GPIO_PODR_G_PODR_G4; 	/* Clear GPIO4 pin to enable RMMI1 on the QS3VH16233PAG QUICKSWITCH*/

#endif /* FNET_CFG_CPU_MCF54418 */
#endif /*FNET_CFG_CPU_ETH_IO_INIT*/

    return FNET_OK;
}


/************************************************************************
* DESCRIPTION: Ethernet Physical Transceiver initialization and/or reset.
*************************************************************************/
static fnet_return_t fnet_mcf_eth_phy_init(fnet_netif_t *netif)
{

#if FNET_CFG_CPU_MCF52235
    fnet_uint16_t reg_value;
    /* Enable EPHY module, Enable auto_neg at start-up, Let PHY PLLs be determined by PHY.*/
    FNET_MCF_EPHY_EPHYCTL0 = (fnet_uint8_t)(FNET_MCF_EPHY_EPHYCTL0_EPHYEN);

    /* Start-up Delay for Kirin2 = 350uS */
    fnet_timer_delay(200); /* Delay for (200ms) */

    /* Disable ANE that causes problems with some routers.  Enable full-duplex and 100Mbps */
    _fnet_eth_phy_read(netif, FNET_ETH_MII_REG_CR, &reg_value);
    _fnet_eth_phy_write(netif, FNET_ETH_MII_REG_CR, (fnet_uint16_t)(reg_value & (~FNET_ETH_MII_REG_CR_ANE) | FNET_ETH_MII_REG_CR_DPLX | FNET_ETH_MII_REG_CR_DATARATE));
#endif

#if FNET_CFG_CPU_MCF52259
    fnet_uint8_t tmp_ptipar;
    fnet_uint8_t tmp_ptjpar;

    fnet_timer_delay(300);
    /* Workaround for PHY reset */
    tmp_ptipar = FNET_MCF5225X_GPIO_PTIPAR; /* Save current state */
    tmp_ptjpar = FNET_MCF5225X_GPIO_PTJPAR;
    FNET_MCF5225X_GPIO_PTIPAR = 0x00;		/* Ethernet signals now GPIO*/
    FNET_MCF5225X_GPIO_PTJPAR = 0x00;
    FNET_MCF5225X_GPIO_DDRTI = 0xFF;		/* GPIO output.*/
    FNET_MCF5225X_GPIO_DDRTJ = 0xFF;
    FNET_MCF5225X_GPIO_PORTTI = 0x00;		/* Force Ethernet signals low.*/
    FNET_MCF5225X_GPIO_PORTTJ = 0x00;

    /* Reset PHY.*/
    _fnet_eth_phy_write(netif, FNET_ETH_MII_REG_CR, FNET_ETH_MII_REG_CR_RESET);
    fnet_timer_delay(500);

    /* RSTO signal.*/
    FNET_MCF_RCM_RCR |= FNET_MCF_RCM_RCR_FRCRSTOUT;     /* Assert RSTO.*/
    fnet_timer_delay(500);
    FNET_MCF_RCM_RCR &= ~(FNET_MCF_RCM_RCR_FRCRSTOUT);  /* Negate RSTO.*/

    FNET_MCF5225X_GPIO_PTIPAR = tmp_ptipar;		/* Restore Ethernet signals.*/
    FNET_MCF5225X_GPIO_PTJPAR = tmp_ptjpar;
#endif

    {
        /* Check if the PHY is powered down or isolated, before using it.*/
        fnet_uint16_t reg_value;

        if (_fnet_eth_phy_read(netif, FNET_ETH_MII_REG_CR, &reg_value) == FNET_OK)
        {
            if(reg_value & (FNET_ETH_MII_REG_CR_PDWN | FNET_ETH_MII_REG_CR_ISOL))
            {
                reg_value &= ~(FNET_ETH_MII_REG_CR_PDWN | FNET_ETH_MII_REG_CR_ISOL);
                _fnet_eth_phy_write(netif, FNET_ETH_MII_REG_CR, reg_value);
            }
        }
    }

    return FNET_OK;
}

#endif /* FNET_MCF && FNET_CFG_ETH */


