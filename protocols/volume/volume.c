#ifndef _HAUSY_PROTOCOL_VOLUME_C
#define _HAUSY_PROTOCOL_VOLUME_C

#include "volume.h"

size_t volume_create_up
 (
   hausy_request *request,
   hausy_id systemID,
   hausy_id unitID,
   uint8_t step
 )
{
   if (request == NULL)
      return hausy_create_command(NULL, 0, 0, 0, 0)
             + VOLUME_CMD_UP_ARG1_STEP;

   size_t at;
   at = hausy_create_command(request, VOLUME_PROTOCOL_ID, systemID, unitID, VOLUME_CMD_UP);
   at = hausy_write_32(request, step, VOLUME_CMD_UP_ARG1_STEP, at);
   return at;
}
size_t volume_parse_up
 (
   hausy_request *request,
   size_t request_size,
   size_t at,
   unsigned int *volume
 )
{
   if (request == NULL)
      return at + VOLUME_CMD_UP_ARG1_STEP;

   if (request_size < at + VOLUME_CMD_UP_ARG1_STEP)
      return 0;
            
   *volume = hausy_read_32(request, VOLUME_CMD_UP_ARG1_STEP, &at);

   return at;
}

size_t volume_create_down
 (
   hausy_request *request,
   hausy_id systemID,
   hausy_id unitID,
   unsigned int step
 )
{
   if (request == NULL)
      return hausy_create_command(NULL, 0, 0, 0, 0)
             + VOLUME_CMD_DOWN_ARG1_STEP;

   size_t at;
   at = hausy_create_command(request, VOLUME_PROTOCOL_ID, systemID, unitID, VOLUME_CMD_DOWN);
   at = hausy_write_32(request, step, VOLUME_CMD_DOWN_ARG1_STEP, at);
   return at;
}
size_t volume_parse_down
 (
   hausy_request *request,
   size_t request_size,
   size_t at,
   unsigned int *volume
 )
{
   if (request == NULL)
      return at + VOLUME_CMD_DOWN_ARG1_STEP;

   if (request_size < at + VOLUME_CMD_DOWN_ARG1_STEP)
      return 0;
            
   *volume = hausy_read_32(request, VOLUME_CMD_DOWN_ARG1_STEP, &at);

   return at;
}

size_t volume_create_mute
 (
   hausy_request *request,
   hausy_id systemID,
   hausy_id unitID
 )
{
   return hausy_create_command(request, VOLUME_PROTOCOL_ID, systemID, unitID, VOLUME_CMD_MUTE);
}

size_t volume_create_unmute
 (
   hausy_request *request,
   hausy_id systemID,
   hausy_id unitID
 )
{
   return hausy_create_command(request, VOLUME_PROTOCOL_ID, systemID, unitID, VOLUME_CMD_UNMUTE);
}

size_t volume_create_mute_toggle
 (
   hausy_request *request,
   hausy_id systemID,
   hausy_id unitID
 )
{
   return hausy_create_command(request, VOLUME_PROTOCOL_ID, systemID, unitID, VOLUME_CMD_MUTE_TOGGLE);
}

size_t volume_create_state_query
 (
   hausy_request *request,
   hausy_id systemID,
   hausy_id unitID
 )
{
   return hausy_create_command(request, VOLUME_PROTOCOL_ID, systemID, unitID, VOLUME_CMD_STATE_QUERY);
}

#endif // _HAUSY_PROTOCOL_VOLUME_C
