#include "relay.h"

size_t relay_parse_state_inform
 (
   hausy_bitstorage *data,
   size_t data_size,
   size_t at,
   hausy_id *systemID,
   hausy_id *unitID,
   hausy_bool *state
 )
{
   if (data == NULL)
      return at
             + RELAY_CMD_STATE_INFORM_ARG1_SYSTEM_ID
             + RELAY_CMD_STATE_INFORM_ARG2_UNIT_ID
             + RELAY_CMD_STATE_INFORM_ARG3_STATE;

   if (data_size < at
         + RELAY_CMD_STATE_INFORM_ARG1_SYSTEM_ID
         + RELAY_CMD_STATE_INFORM_ARG2_UNIT_ID
         + RELAY_CMD_STATE_INFORM_ARG3_STATE
      )
      return 0;
            
   *systemID = hausy_read_long(data, RELAY_CMD_STATE_INFORM_ARG1_SYSTEM_ID, &at);
   *unitID   = hausy_read_long(data, RELAY_CMD_STATE_INFORM_ARG2_UNIT_ID,   &at);
   *state    = hausy_read_long(data, RELAY_CMD_STATE_INFORM_ARG3_STATE,     &at);

   return at;
}

size_t relay_create_on
 (
   hausy_bitstorage *data,
   hausy_id systemID,
   hausy_id unitID
 )
{
   return hausy_create_command(data, RELAY_PROTOCOL_ID, systemID, unitID, RELAY_CMD_ON);
}

size_t relay_create_off
 (
   hausy_bitstorage *data,
   hausy_id systemID,
   hausy_id unitID
 )
{
   return hausy_create_command(data, RELAY_PROTOCOL_ID, systemID, unitID, RELAY_CMD_OFF);
}

size_t relay_create_state_query
 (
   hausy_bitstorage *data,
   hausy_id systemID,
   hausy_id unitID
 )
{
   return hausy_create_command(data, RELAY_PROTOCOL_ID, systemID, unitID, RELAY_CMD_STATE_QUERY);
}

size_t relay_create_state_inform
 (
   hausy_bitstorage *data,
   hausy_id systemID,
   hausy_id unitID,
   hausy_bool state
 )
{
   if (data == NULL)
      return hausy_create_command(NULL, 0, 0, 0, 0)
             + RELAY_CMD_STATE_INFORM_ARG1_SYSTEM_ID
             + RELAY_CMD_STATE_INFORM_ARG2_UNIT_ID
             + RELAY_CMD_STATE_INFORM_ARG3_STATE;

   size_t at;
   at = hausy_create_command(data, RELAY_PROTOCOL_ID, HAUSY_BROADCAST_ID, HAUSY_BROADCAST_ID, RELAY_CMD_STATE_INFORM);
   at = hausy_write_long(data, systemID, RELAY_CMD_STATE_INFORM_ARG1_SYSTEM_ID, at);
   at = hausy_write_long(data, unitID,   RELAY_CMD_STATE_INFORM_ARG2_UNIT_ID,   at);
   at = hausy_write_long(data, state,    RELAY_CMD_STATE_INFORM_ARG3_STATE,     at);
   return at;
}

