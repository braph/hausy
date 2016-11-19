#ifndef Hausy_Protocol_Relay_h
#define Hausy_Protocol_Relay_h


#include "../../hausy.h"

#define RELAY_PROTOCOL_ID                       53 // 'R'

#define RELAY_CMD_OFF                           0  // '0'
#define RELAY_CMD_ON                            1  // '1'
#define RELAY_CMD_TOGGLE                        55 // 'T'
#define RELAY_CMD_STATE_QUERY                   52 // 'Q'

#define RELAY_CMD_STATE_INFORM                  44 // 'I'
#define RELAY_CMD_STATE_INFORM_ARG1_SYSTEM_ID   HAUSY_ID_BITLENGTH
#define RELAY_CMD_STATE_INFORM_ARG2_UNIT_ID     HAUSY_ID_BITLENGTH
#define RELAY_CMD_STATE_INFORM_ARG3_STATE       HAUSY_BOOL_BITLENGTH

#ifdef __cplusplus
extern "C" {
#endif

size_t relay_parse_state_inform(
   hausy_bitstorage *data,
   size_t data_size,
   size_t at,
   hausy_id *systemID,
   hausy_id *unitID,
   hausy_bool *state
);

size_t relay_create_on(
   hausy_bitstorage *data,
   hausy_id systemID,
   hausy_id unitID
);

size_t relay_create_off(
   hausy_bitstorage *data,
   hausy_id systemID,
   hausy_id unitID
);

size_t relay_create_toggle(
   hausy_bitstorage *data,
   hausy_id systemID,
   hausy_id unitID
);

size_t relay_create_state_query(
   hausy_bitstorage *data,
   hausy_id systemID,
   hausy_id unitID
);

size_t relay_create_state_inform(
   hausy_bitstorage *data,
   hausy_id systemID,
   hausy_id unitID,
   hausy_bool state
);

#ifdef __cplusplus
}
#endif

#endif
