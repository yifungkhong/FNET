/**************************************************************************
*
* Copyright 2011-2017 by Andrey Butok. FNET Community.
* Copyright 2008-2010 by Andrey Butok. Freescale Semiconductor, Inc.
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
* @brief FNET Shell Demo (HTTP Server).
*
***************************************************************************/

/************************************************************************
*     Private Definitions.
*************************************************************************/
#ifndef _FAPP_HTTP_H_

#define _FAPP_HTTP_H_

#if (FAPP_CFG_HTTP_CMD || FAPP_CFG_HTTP_TLS_CMD) && FNET_CFG_HTTP

#define FAPP_HTTP_MOUNT_NAME   FAPP_FS_MOUNT_NAME
#define FAPP_HTTP_INDEX_FILE   "index.html"

#if defined(__cplusplus)
extern "C" {
#endif
extern fnet_http_desc_t fapp_http_desc; /* HTTP service descriptor. */

void fapp_http_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv );
void fapp_http_info(fnet_shell_desc_t desc);
void fapp_http_release(void);

#if FAPP_CFG_HTTP_TLS_CMD && FNET_CFG_HTTP_TLS && FNET_CFG_TLS
extern fnet_http_desc_t fapp_http_tls_desc; /* HTTPS service descriptor. */
void fapp_http_tls_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv );
void fapp_http_tls_release(void);
#endif

#if defined(__cplusplus)
}
#endif

#endif

#endif
