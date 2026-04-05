#include "objects.h"

#include "objs/models.h"
#include "game/object_helpers.h"

struct Object* SpawnObject(u32 modelId, const BehaviorScript* behavior, s16 x, s16 y, s16 z, s32 param) {
    struct Object* object =
        spawn_object_abs_with_rot(&gMacroObjectDefaultParent, 0, modelId, behavior, x, y, z, 0, 0, 0);
    object->custom = true;
    return object;
}

void LoadModel(uint16_t modelId, const void* data) {
    gLoadedGraphNodes[modelId] = (struct GraphNode *) init_graph_node_display_list(sLevelPool, 0, LAYER_OPAQUE, data);
}

void OnLoadArea(void* ev) {
    LoadModel(MODEL_TVBOX, tv_model);
    LoadModel(MODEL_SNES, snes_model);
    LoadModel(MODEL_SNES_CONTROLLER, snes_controller_model);
}