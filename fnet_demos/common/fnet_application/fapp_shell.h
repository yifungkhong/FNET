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
*  FNET Application private definitions.
*
***************************************************************************/

#ifndef _FAPP_SHELL_H_

#define _FAPP_SHELL_H_

/************************************************************************
*    Definitions.
*************************************************************************/
extern const fnet_char_t FAPP_HELP_STR[];
extern const fnet_char_t FAPP_DELIMITER_STR[];
extern const fnet_char_t FAPP_CANCELLED_STR[];
extern const fnet_char_t FAPP_UPDATED_IP_STR[];
extern const fnet_char_t FAPP_TOCANCEL_STR[];
extern const fnet_char_t FAPP_COMMAND_RELEASE[]; /* Service release command */
extern const fnet_char_t FAPP_PARAM_ERR[];
extern const fnet_char_t FAPP_INIT_ERR[];
extern const fnet_char_t FAPP_SHELL_INFO_FORMAT_S[];
extern const fnet_char_t FAPP_SHELL_INFO_FORMAT_D[];
extern const fnet_char_t FAPP_SHELL_INFO_FORMAT_H32[];
extern const fnet_char_t FAPP_SHELL_INFO_FORMAT_H16[];
extern const fnet_char_t FAPP_SHELL_CANCELED_CTRL_C[];
extern const fnet_char_t *const fapp_is_enabled_str[];

extern const fnet_shell_command_t fapp_cmd_table[];

/************************************************************************
*     Function Prototypes
*************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif
fnet_shell_desc_t fapp_shell_init(void);
void fapp_help_cmd ( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv );
void fapp_shell_on_init( fnet_shell_desc_t desc );
void fapp_go_cmd ( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv );

#if FAPP_CFG_GO_CMD || FAPP_CFG_BOOTLOADER || FAPP_CFG_SETGET_CMD_BOOT
void fapp_go ( fnet_shell_desc_t desc, fnet_uint32_t address );
#endif

#if defined(__cplusplus)
}
#endif

#endif
