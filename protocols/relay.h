#ifndef _HAUSY_PROTOCOL_RELAY_H
#define _HAUSY_PROTOCOL_RELAY_H

#define RELAY_PROTOCOL_ID     53 // 'R'
#define RELAY_CMD_OFF         0  // '0'
#define RELAY_CMD_ON          1  // '1'
#define RELAY_CMD_TOGGLE      55 // 'T'
#define RELAY_CMD_QUERY       52 // 'Q'

#ifdef __cplusplus
extern "C" {
#endif

static inline
size_t relay_create_on
 (
   hausy_request *req,
   hausy_id systemID,
   hausy_id unitID
 )
{
   return hausy_create_request(req, RELAY_PROTOCOL_ID, systemID, unitID, RELAY_CMD_ON);
}

static inline
size_t relay_create_off
 (
   hausy_request *req,
   hausy_id systemID,
   hausy_id unitID
 )
{
   return hausy_create_request(req, RELAY_PROTOCOL_ID, systemID, unitID, RELAY_CMD_OFF);
}

static inline
size_t relay_create_toggle
 (
   hausy_request *req,
   hausy_id systemID,
   hausy_id unitID
 )
{
   return hausy_create_request(req, RELAY_PROTOCOL_ID, systemID, unitID, RELAY_CMD_TOGGLE);
}

static inline
size_t relay_create_query
 (
   hausy_request *req,
   hausy_id systemID,
   hausy_id unitID
 )
{
   return hausy_create_request(req, RELAY_PROTOCOL_ID, systemID, unitID, RELAY_CMD_QUERY);
}

#ifdef __cplusplus
}
#endif

#endif // _HAUSY_PROTOCOL_RELAY_H
