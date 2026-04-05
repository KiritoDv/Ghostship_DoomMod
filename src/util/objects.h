#include "sm64.h"
#include "engine/math_util.h"
#include "game/object_list_processor.h"

extern struct AllocOnlyPool *sLevelPool;

extern struct Object* SpawnObject(u32 modelId, const BehaviorScript* behavior, s16 x, s16 y, s16 z, s32 param);
extern void LoadModel(uint16_t modelId, const void* data);
extern void OnLoadArea(void*);