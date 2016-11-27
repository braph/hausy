#ifndef _HAUSY_PROTOCOL_RELAY_H
#define _HAUSY_PROTOCOL_RELAY_H

#include "../../hausy.h"

#define RELAY_PROTOCOL_ID         53 // 'R'
#define RELAY_CMD_OFF             0  // '0'
#define RELAY_CMD_ON              1  // '1'
#define RELAY_CMD_TOGGLE          55 // 'T'
#define RELAY_CMD_STATE_QUERY     52 // 'Q'

#ifdef __cplusplus
extern "C" {
#endif

size_t relay_create_on(
   hausy_request *request,
   hausy_id systemID,
   hausy_id unitID
);

size_t relay_create_off(
   hausy_request *request,
   hausy_id systemID,
   hausy_id unitID
);

size_t relay_create_toggle(
   hausy_request *request,
   hausy_id systemID,
   hausy_id unitID
);

size_t relay_create_query(
   hausy_request *request,
   hausy_id systemID,
   hausy_id unitID
);

#ifdef __cplusplus
}
#endif

#endif
