#ifndef Hausy_Protocol_Volume_h
#define Hausy_Protocol_Volume_h

#include <stdint.h>
#include "../../hausy.h"

#define VOLUME_BIT_LENGTH               7 // 0 - 100

#define VOLUME_PROTOCOL_ID              57 // 'V'

#define VOLUME_CMD_SET                  54 // 'S'
#define VOLUME_CMD_SET_ARG1_VOLUME      VOLUME_BIT_LENGTH

#define VOLUME_CMD_UP                   56 // 'U'
#define VOLUME_CMD_UP_ARG1_STEP         VOLUME_BIT_LENGTH

#define VOLUME_CMD_DOWN                 39 // 'D'
#define VOLUME_CMD_DOWN_ARG1_STEP       VOLUME_BIT_LENGTH

#define VOLUME_CMD_MUTE                 48 // 'M'
#define VOLUME_CMD_UNMUTE               22 // 'm'
#define VOLUME_CMD_MUTE_TOGGLE          55 // 'T'

#define VOLUME_CMD_QUERY                52 // 'Q'

#ifdef __cplusplus
extern "C" {
#endif

size_t volume_create_set(
   hausy_request *request,
   hausy_id systemID,
   hausy_id unitID,
   uint8_t step
);
size_t volume_parse_set(
   hausy_request *request,
   uint8_t *step
);

size_t volume_create_up(
   hausy_request *request,
   hausy_id systemID,
   hausy_id unitID,
   uint8_t step
);
size_t volume_parse_up(
   hausy_request *request,
   uint8_t *step
);

size_t volume_create_down(
   hausy_request *request,
   hausy_id systemID,
   hausy_id unitID,
   uint8_t step
);
size_t volume_parse_down(
   hausy_request *request,
   uint8_t *step
);

size_t volume_create_mute(
   hausy_request *request,
   hausy_id systemID,
   hausy_id unitID
);

size_t volume_create_unmute(
   hausy_request *request,
   hausy_id systemID,
   hausy_id unitID
);

size_t volume_create_mute_toggle(
   hausy_request *request,
   hausy_id systemID,
   hausy_id unitID
);

size_t volume_create_query(
   hausy_request *request,
   hausy_id systemID,
   hausy_id unitID
);

#ifdef __cplusplus
}
#endif

#endif
