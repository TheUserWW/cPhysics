#ifndef CPHYSICS_PLOG_H
#define CPHYSICS_PLOG_H
#include "entity.h"
#include "stdio.h"

#define OPERATION_SET_FAILED     0x00000000
#define OPERATION_SET_SUCCESS    0x00000001
#define OPERATION_GET_FAILED     0x00000002
#define OPEARTION_GET_SUCCESS    0x00000003
#define ENTITY_VOID_ERROR        0x00000004
#define ENTITY_UNFULLY_UNDEFINED 0x00000005
#define DIVISION_BY_ZERO         0x00000006

char* show_entity_details(Entity* obj);

char* LOG(const char* str);

#endif //CPHYSICS_PLOG_H