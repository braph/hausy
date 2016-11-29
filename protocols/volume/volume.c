#ifndef _HAUSY_PROTOCOL_VOLUME_C
#define _HAUSY_PROTOCOL_VOLUME_C

#include "volume.h"

size_t volume_create_set
 (
   hausy_request *request,
   hausy_id systemID,
   hausy_id unitID,
   uint8_t volume
 )
{
   size_t at;
   at = hausy_create_request(request, VOLUME_PROTOCOL_ID, systemID, unitID, VOLUME_CMD_SET);
   at = hausy_write_32(request, volume, VOLUME_CMD_SET_ARG1_VOLUME, at);
   return at;
}
size_t volume_parse_set
 (
   hausy_request *request,
   uint8_t *volume
 )
{
   return hausy_read_32(request, (uint32_t*) volume, VOLUME_CMD_SET_ARG1_VOLUME, 0);
}

size_t volume_create_up
 (
   hausy_request *request,
   hausy_id systemID,
   hausy_id unitID,
   uint8_t step
 )
{
   size_t at;
   at = hausy_create_request(request, VOLUME_PROTOCOL_ID, systemID, unitID, VOLUME_CMD_UP);
   at = hausy_write_32(request, step, VOLUME_CMD_UP_ARG1_STEP, at);
   return at;
}
size_t volume_parse_up
 (
   hausy_request *request,
   uint8_t *volume
 )
{
   return hausy_read_32(request, (uint32_t*) volume, VOLUME_CMD_UP_ARG1_STEP, 0);
}

size_t volume_create_down
 (
   hausy_request *request,
   hausy_id systemID,
   hausy_id unitID,
   uint8_t step
 )
{
   size_t at;
   at = hausy_create_request(request, VOLUME_PROTOCOL_ID, systemID, unitID, VOLUME_CMD_DOWN);
   at = hausy_write_32(request, step, VOLUME_CMD_DOWN_ARG1_STEP, at);
   return at;
}
size_t volume_parse_down
 (
   hausy_request *request,
   uint8_t *volume
 )
{
   return hausy_read_32(request, (uint32_t*) volume, VOLUME_CMD_DOWN_ARG1_STEP, 0);
}

size_t volume_create_mute
 (
   hausy_request *request,
   hausy_id systemID,
   hausy_id unitID
 )
{
   return hausy_create_request(request, VOLUME_PROTOCOL_ID, systemID, unitID, VOLUME_CMD_MUTE);
}

size_t volume_create_unmute
 (
   hausy_request *request,
   hausy_id systemID,
   hausy_id unitID
 )
{
   return hausy_create_request(request, VOLUME_PROTOCOL_ID, systemID, unitID, VOLUME_CMD_UNMUTE);
}

size_t volume_create_mute_toggle
 (
   hausy_request *request,
   hausy_id systemID,
   hausy_id unitID
 )
{
   return hausy_create_request(request, VOLUME_PROTOCOL_ID, systemID, unitID, VOLUME_CMD_MUTE_TOGGLE);
}

size_t volume_create_query
 (
   hausy_request *request,
   hausy_id systemID,
   hausy_id unitID
 )
{
   return hausy_create_request(request, VOLUME_PROTOCOL_ID, systemID, unitID, VOLUME_CMD_QUERY);
}

#endif // _HAUSY_PROTOCOL_VOLUME_C
