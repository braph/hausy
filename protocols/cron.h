/*
 * Hausy protocol for executing requests in the future
 */

#ifndef _HAUSY_PROTOCOL_CRON_H
#define _HAUSY_PROTOCOL_CRON_H

#include <stdint.h>
#include "../hausy.h"

/* We only send the first 23 bits of the timestamp, removig the first bit.
 * Our timestamp can hold a value up to 47 days in the future,
 * beeing inaccurate by 1 second.  */
#define TIMESTAMP_LENGTH                       22

#define CRON_PROTOCOL_ID                       38 // 'C'

#define CRON_CMD_ADD                           36 // 'A'
#define CRON_CMD_ADD_ARG1_TIMESTAMP            TIMESTAMP_LENGTH
#define CRON_CMD_ADD_ARG2_PROTOCOL_ID          HAUSY_ID_BITLENGTH
#define CRON_CMD_ADD_ARG3_SYSTEM_ID            HAUSY_ID_BITLENGTH
#define CRON_CMD_ADD_ARG4_REQUEST              0 // dynamic length

#define CRON_CMD_REMOVE                        53 // 'R'
#define CRON_CMD_REMOVE_ARG1_TIMESTAMP         TIMESTAMP_LENGTH
#define CRON_CMD_REMOVE_ARG2_PROTOCOL_ID       HAUSY_ID_BITLENGTH
#define CRON_CMD_REMOVE_ARG3_SYSTEM_ID         HAUSY_ID_BITLENGTH
#define CRON_CMD_REMOVE_ARG4_REQUEST           0 // dynamic length

#define CRON_CMD_QUERY                         52 // 'Q'

#ifdef __cplusplus
extern "C" {
#endif

static inline
size_t cron_create_add
 (
   hausy_request *request,
   uint32_t cron_timestamp,
   hausy_request *cron_request
 )
{
   size_t pos, cron_pos;
   hausy_id protocolId;
   hausy_id systemId;
   hausy_id unitId;
   hausy_id cmdId;

   /* parse the original identifiers of the cron request */
   cron_pos = hausy_parse_request(cron_request, &protocolId, &systemId, &unitId, &cmdId);
   if (! cron_pos)
      return 0;

   /* create our request with extracted identifiers from above, replacing protocolId and
    * commandId by the cron-protocol ones. */
   pos = hausy_create_request(request, CRON_PROTOCOL_ID, systemId, unitId, CRON_CMD_ADD);
   if (! pos)
      return 0;

   /* write our truncated timestamp into the request */
   pos = hausy_write_32(request, (cron_timestamp >> 1), CRON_CMD_ADD_ARG1_TIMESTAMP, pos);
   if (! pos)
      return 0;

   /* append the original protocolId ... */
   pos = hausy_write_32(request, cron_protocolId, CRON_CMD_ADD_ARG2_PROTOCOL_ID, pos);
   if (! pos)
      return 0;

   /* ... and the commandId */
   pos = hausy_write_32(request, cron_commandId, CRON_CMD_ADD_ARG3_COMMAND_ID, pos);
   if (! pos)
      return 0;

   /* append the rest of cron_request to request */
   if (cron_request->size > cron_pos) {
      return hausy_request_copy(cron_request, cron_pos, 0, request, pos);
   }

   return pos;
}

static inline
size_t cron_parse_add
 (
   hausy_request *request,
   size_t pos,
   uint32_t *cron_timestamp,
   hausy_request *cron_request
 )
{
   size_t pos;
   size_t cron_pos;
   hausy_id protocolId, systemId, unitId, cmdId;

   /* parse the original identifiers of the request */
   if (hausy_parse_request(request, &protocolId, &systemId, &unitId, &cmdId) != pos)
      return 0;
   
   /* parse timestamp */
   pos = hausy_read_32(request, cron_timestamp, CRON_CMD_ADD_ARG1_TIMESTAMP, pos);
   *cron_timestamp = *cron_timestamp << 1;
   if (! pos)
      return 0;

   /* parse desired protocolId ... */
   pos = hausy_read_32(request, protocolId, CRON_CMD_ADD_ARG2_PROTOCOL_ID, pos);
   if (! pos)
      return 0;

   /* ... and cmdId */
   pos = hausy_read_32(request, cmdId, CRON_CMD_ADD_ARG3_COMMAND_ID, pos);
   if (! pos)
      return 0;

   /* create cron_request */
   cron_pos = hausy_create_request(cron_request, protocolId, systemId, unitId, cmdId);
   if (! cron_pos)
      return 0;

   /* append rest of request to cron_request */
   if (pos < request->size) {
      if (! hausy_request_copy(request, pos, CRON_CMD_ADD_ARG4_REQUEST, cron_request, cron_pos))
         return 0;
      return request->size;
   }

   return pos;
}

static inline
size_t cron_create_remove
 (
   hausy_request *request,
   hausy_id systemId,
   hausy_id unitId,
   uint32_t cron_timestamp,
   hausy_id cron_protocolId,
   hausy_id cron_commandId,
   hausy_request *cron_request
 )
{
   size_t pos;

   pos = hausy_create_request(request, CRON_PROTOCOL_ID, systemId, unitId, CRON_CMD_REMOVE);
   if (! pos)
      return 0;

   pos = hausy_write_32(request, cron_timestamp, CRON_CMD_REMOVE_ARG1_TIMESTAMP, pos);
   if (! pos)
      return 0;

   pos = hausy_write_32(request, cron_protocolId, CRON_CMD_REMOVE_ARG2_PROTOCOL_ID, pos);
   if (! pos)
      return 0;

   pos = hausy_write_32(request, cron_commandId, CRON_CMD_REMOVE_ARG3_COMMAND_ID, pos);
   if (! pos)
      return 0;

   if (cron_request) {
      return hausy_request_copy(cron_request, 0, 0, request, pos);
   }

   return pos;
}

static inline
size_t cron_parse_remove
 (
   hausy_request *request,
   uint32_t *cron_timestamp,
   hausy_id *cron_protocolId,
   hausy_id *cron_commandId,
   hausy_request *cron_request
 )
{
   size_t pos;
   
   pos = hausy_read_32(request, cron_timestamp, CRON_CMD_REMOVE_ARG1_TIMESTAMP, 0);
   if (! pos)
      return 0;

   pos = hausy_read_32(request, cron_protocolId, CRON_CMD_REMOVE_ARG2_PROTOCOL_ID, pos);
   if (! pos)
      return 0;

   pos = hausy_read_32(request, cron_commandId, CRON_CMD_REMOVE_ARG3_COMMAND_ID, pos);
   if (! pos)
      return 0;

   if (pos < request->size) {
      if (! hausy_request_copy(request, pos, 0, cron_request, 0))
         return 0;
      return request->size;
   }
   else {
      hausy_request_free(cron_request);
      return pos;
   }
}

static inline
size_t cron_create_state_query
 (
   hausy_request *request,
   hausy_id systemId,
   hausy_id unitId
 )
{
   return hausy_create_request(request, CRON_PROTOCOL_ID, systemId, unitId, CRON_CMD_QUERY);
}

#ifdef __cplusplus
}
#endif

#endif
