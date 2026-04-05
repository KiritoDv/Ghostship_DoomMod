#define OBJECT_FIELDS_INDEX_DIRECTLY

#include "behavior_data.h"
#include "behavior_macros.h"
#include "object_constants.h"
#include "object_fields.h"
#include "engine/surface_load.h"
#include "game/object_helpers.h"
#include "game/interaction.h"
#include "game/object_list_processor.h"

#include "assets/actors/breakable_box.h"

#define o gCurrentObject

struct ObjectHitbox sTVHitbox = {
    /* interactType:      */ INTERACT_BREAKABLE,
    /* downOffset:        */ 20,
    /* damageOrCoinValue: */ 0,
    /* health:            */ 1,
    /* numLootCoins:      */ 0,
    /* radius:            */ 150,
    /* height:            */ 200,
    /* hurtboxRadius:     */ 150,
    /* hurtboxHeight:     */ 200,
};

static void bhv_tv_loop(void) {
    obj_set_hitbox(o, &sTVHitbox);
}

const BehaviorScript bhvCustomTv[] = {
    BEGIN(OBJ_LIST_SURFACE),
    OR_INT(oFlags, OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE),
    LOAD_COLLISION_DATA(breakable_box_seg8_collision_08012D70),
    SET_FLOAT(oCollisionDistance, 500),
    BEGIN_LOOP(),
        CALL_NATIVE(bhv_tv_loop),
        CALL_NATIVE(load_object_collision_model),
    END_LOOP(),
    BREAK(),
};