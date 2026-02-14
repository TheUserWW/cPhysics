#ifndef CPHYSICS_PLOG_H
#define CPHYSICS_PLOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "core/entity.h"
#include "stdio.h"
#include "error_codes.h"

char* show_entity_details(Entity* obj);

char* LOG(const char* str);

const char* get_error_description(ErrorCode error);

#ifdef __cplusplus
}
#endif

#endif //CPHYSICS_PLOG_H