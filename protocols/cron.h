#ifndef _HAUSY_PROTOCOL_CRON_H
#define _HAUSY_PROTOCOL_CRON_H

#include <stdint.h>
#include "../hausy.h"

#define TIMESTAMP_LENGTH                       32

#define CRON_PROTOCOL_ID                       38 // 'C'

#define CRON_CMD_ADD                           36 // 'A'
#define CRON_CMD_ADD_ARG1_TIMESTAMP            TIMESTAMP_LENGTH
#define CRON_CMD_ADD_ARG2_PROTOCOL_ID          HAUSY_ID_BITLENGTH
#define CRON_CMD_ADD_ARG3_COMMAND_ID           HAUSY_ID_BITLENGTH
#define CRON_CMD_ADD_ARG4_REQUEST              0 // dynamic length

#define CRON_CMD_REMOVE                        53 // 'R'
#define CRON_CMD_REMOVE_ARG1_TIMESTAMP         TIMESTAMP_LENGTH
#define CRON_CMD_REMOVE_ARG2_PROTOCOL_ID       HAUSY_ID_BITLENGTH
#define CRON_CMD_REMOVE_ARG3_COMMAND_ID        HAUSY_ID_BITLENGTH
#define CRON_CMD_REMOVE_ARG4_REQUEST           0 // dynamic length

#define CRON_CMD_QUERY                         52 // 'Q'

#ifdef __cplusplus
extern "C" {
#endif

static inline
size_t cron_create_add
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

   pos = hausy_create_request(request, CRON_PROTOCOL_ID, systemId, unitId, CRON_CMD_ADD);
   if (! pos)
      return 0;

   pos = hausy_write_32(request, cron_timestamp, CRON_CMD_ADD_ARG1_TIMESTAMP, pos);
   if (! pos)
      return 0;

   pos = hausy_write_32(request, cron_protocolId, CRON_CMD_ADD_ARG2_PROTOCOL_ID, pos);
   if (! pos)
      return 0;

   pos = hausy_write_32(request, cron_commandId, CRON_CMD_ADD_ARG3_COMMAND_ID, pos);
   if (! pos)
      return 0;

   if (cron_request) {
      return hausy_request_copy(cron_request, 0, 0, request, pos);
   }

   return pos;
}

static inline
size_t cron_parse_add
 (
   hausy_request *request,
   uint32_t *cron_timestamp,
   hausy_id *cron_protocolId,
   hausy_id *cron_commandId,
   hausy_request *cron_request
 )
{
   size_t pos;
   
   pos = hausy_read_32(request, cron_timestamp, CRON_CMD_ADD_ARG1_TIMESTAMP, 0);
   if (! pos)
      return 0;

   pos = hausy_read_32(request, cron_protocolId, CRON_CMD_ADD_ARG2_PROTOCOL_ID, pos);
   if (! pos)
      return 0;

   pos = hausy_read_32(request, cron_commandId, CRON_CMD_ADD_ARG3_COMMAND_ID, pos);
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
