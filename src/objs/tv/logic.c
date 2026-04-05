#include "doom.h"

#include "sm64.h"
#include "objs/models.h"
#include "util/objects.h"
#include "game/level_update.h"
#include "game/object_list_processor.h"
#include "game/object_helpers.h"

#define o gCurrentObject

extern const BehaviorScript bhvCustomTv[];

void SpawnDoomTV(void) {
    struct Object* object = SpawnObject(MODEL_TVBOX, bhvCustomTv, gMarioState->pos[0], gMarioState->pos[1], gMarioState->pos[2], 0);
    object->oMoveAngleYaw = atan2s(gMarioState->pos[0] - object->oPosX, gMarioState->pos[2] - object->oPosZ);
}