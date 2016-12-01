#ifndef _HAUSY_PROTOCOL_TIME_H
#define _HAUSY_PROTOCOL_TIME_H

#include <stdint.h>
#include "../hausy.h"

#define TIMESTAMP_LENGTH                       32

#define TIME_PROTOCOL_ID                       55 // 'T'

#define TIME_CMD_SYNC                          54 // 'S'
#define TIME_CMD_SYNC_ARG1_TIMESTAMP           TIMESTAMP_LENGTH

#define TIME_CMD_QUERY                         52 // 'Q'

#ifdef __cplusplus
extern "C" {
#endif

static inline
size_t time_create_sync
 (
   hausy_request *request,
   uint32_t timestap
 )
{
   size_t pos;

   pos = hausy_create_request(request, TIME_PROTOCOL_ID, HAUSY_BROADCAST_ID, HAUSY_BROADCAST_ID, TIME_CMD_SYNC);
   if (! pos)
      return 0;

   return hausy_write_32(request, timestamp, TIME_CMD_SYNC_ARG1_TIMESTAMP, pos);
}

static inline
size_t time_parse_sync
 (
   hausy_request *request,
   size_t pos,
   uint32_t *timestamp
 )
{
   return hausy_read_32(request, timestamp, TIME_CMD_SYNC_ARG1_TIMESTAMP, pos);
}

static inline
size_t time_create_state_query
 (
   hausy_request *request
 )
{
   return hausy_create_request(request, TIME_PROTOCOL_ID, HAUSY_BROADCAST_ID, HAUSY_BROADCAST_ID, TIME_CMD_QUERY);
}

#ifdef __cplusplus
}
#endif

#endif
