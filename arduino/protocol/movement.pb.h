/* Automatically generated nanopb header */
/* Generated by nanopb-0.2.6 at Sun Mar 23 20:35:43 2014. */

#ifndef _PB_MOVEMENT_PB_H_
#define _PB_MOVEMENT_PB_H_
#include <pb.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Enum definitions */
/* Struct definitions */
typedef struct _Movement {
    pb_callback_t power;
    int32_t duration;
} Movement;

/* Default values for struct fields */

/* Field tags (for use in manual encoding/decoding) */
#define Movement_power_tag                       1
#define Movement_duration_tag                    2

/* Struct field encoding specification for nanopb */
extern const pb_field_t Movement_fields[3];

/* Maximum encoded size of messages (where known) */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
