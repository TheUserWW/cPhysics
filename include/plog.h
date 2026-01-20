#ifndef CPHYSICS_PLOG_H
#define CPHYSICS_PLOG_H
#include "entity.h"
#include "stdio.h"
#include "error_codes.h"

char* show_entity_details(Entity* obj);

char* LOG(const char* str);

const char* get_error_description(ErrorCode error);

#endif //CPHYSICS_PLOG_H