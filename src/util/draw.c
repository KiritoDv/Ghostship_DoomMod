#include "draw.h"

void Lib_TextureRect_RGBA32(Gfx** gfxPtr, u32* texture, u32 width, u32 height, f32 xPos, f32 yPos, f32 xScale,
                            f32 yScale) {
    gDPLoadTextureBlock((*gfxPtr)++, texture, G_IM_FMT_RGBA, G_IM_SIZ_32b, width, height, 0, G_TX_NOMIRROR | G_TX_WRAP,
                        G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
    gSPWideTextureRectangle((*gfxPtr)++, (s32) (xPos * 4.0f), (s32) (yPos * 4.0f),
                            (s32) ((xPos + width * xScale) * 4.0f), (s32) ((yPos + height * yScale) * 4.0f),
                            G_TX_RENDERTILE, 0, 0, (s32) (1.0f / xScale * 1024.0f), (s32) (1.0f / yScale * 1024.0f));
}