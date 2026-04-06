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

    if (o->rawData.asS32[0x33] == 0) {
        breakable_box_init();
    }

    if (cur_obj_was_attacked_or_ground_pounded()) {
        obj_explode_and_spawn_coins(46.0f, 1);
        create_sound_spawner(SOUND_GENERAL_BREAK_BOX);
    }
}

extern void bhv_beta_holdable_object_init(void);
extern void bhv_beta_holdable_object_loop(void);

const BehaviorScript bhvCustomTv[] = {
    BEGIN(OBJ_LIST_GENACTOR),
    OR_INT(oFlags, (OBJ_FLAG_HOLDABLE | OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE)),
    SET_INT(oInteractType, INTERACT_GRABBABLE),
    DROP_TO_FLOOR(),
    SET_HITBOX(/*Radius*/ 150, /*Height*/ 200),
    CALL_NATIVE(bhv_beta_holdable_object_init),
    BEGIN_LOOP(),
        SET_INT(oIntangibleTimer, 0),
        CALL_NATIVE(bhv_beta_holdable_object_loop),
    END_LOOP(),
};