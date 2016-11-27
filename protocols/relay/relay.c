#ifndef _HAUSY_PROTOCOL_RELAY_C
#define _HAUSY_PROTOCOL_RELAY_C

#include "relay.h"

size_t relay_create_on
 (
   hausy_request *req,
   hausy_id systemID,
   hausy_id unitID
 )
{
   return hausy_create_request(req, RELAY_PROTOCOL_ID, systemID, unitID, RELAY_CMD_ON);
}

size_t relay_create_off
 (
   hausy_request *req,
   hausy_id systemID,
   hausy_id unitID
 )
{
   return hausy_create_request(req, RELAY_PROTOCOL_ID, systemID, unitID, RELAY_CMD_OFF);
}

size_t relay_create_toggle
 (
   hausy_request *req,
   hausy_id systemID,
   hausy_id unitID
 )
{
   return hausy_create_request(req, RELAY_PROTOCOL_ID, systemID, unitID, RELAY_CMD_TOGGLE);
}

size_t relay_create_query
 (
   hausy_request *req,
   hausy_id systemID,
   hausy_id unitID
 )
{
   return hausy_create_request(req, RELAY_PROTOCOL_ID, systemID, unitID, RELAY_CMD_STATE_QUERY);
}

#endif // _HAUSY_PROTOCOL_RELAY_C
