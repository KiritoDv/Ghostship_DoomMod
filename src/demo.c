#include "mod.h"

#include <stdio.h>
#include <stdlib.h>

#include "sm64.h"
#include "gfx_dimensions.h"
#include "port/events/Events.h"
#include "game/game_init.h"
#include "port/api/ui.h"
#include "types.h"
#include "doom.h"
#include "util/draw.h"

Gfx gPool[0x1024] = { 0 };
ListenerID gListenerIDs[4];
uint8_t* gDoomScreenBuffer = NULL;

void SetupUI() {
    C_WidgetConfig chk = {0};
    chk.type = C_WIDGET_CVAR_CHECKBOX;
    chk.cvar = "gSkipIntro";
    chk.opts.checkbox.tooltip = "Skip the intro cutscene.";
    chk.opts.checkbox.default_val = true;
    C_AddSidebarEntry("My Mod", 1);
    C_AddWidget("My Mod", 1, "Draw Mod Hi", &chk);
}

int stick_to_key(f32 axis, int is_y_axis) {
    if (axis > 40.0f) {
        return is_y_axis ? DOOMKEY_UPARROW : DOOMKEY_RIGHTARROW;
    } else if (axis < -40.0f) {
        return is_y_axis ? DOOMKEY_DOWNARROW : DOOMKEY_LEFTARROW;
    }
    return 0;
}

void send_key_if_not_zero(int key, int pressed) {
    if (key != 0) {
        DoomDLL_Input(pressed, key);
        printf("Sent key %d with pressed=%d\n", key, pressed);
    }
}

void HandleDoomInput() {
    struct Controller *input = gPlayer1Controller;

    static f32 prev_stick_x = 0.0f;
    static f32 prev_stick_y = 0.0f;
    static u16 prev_buttons = 0;

    int prev_x_key = stick_to_key(prev_stick_x, 0);
    int cur_x_key  = stick_to_key(input->stickX, 0);
    int prev_y_key = stick_to_key(prev_stick_y, 1);
    int cur_y_key  = stick_to_key(input->stickY, 1);

    if (prev_x_key != cur_x_key) {
        if (prev_x_key != 0) DoomDLL_Input(0, prev_x_key); 
        if (cur_x_key != 0)  DoomDLL_Input(1, cur_x_key);  
    }

    if (prev_y_key != cur_y_key) {
        if (prev_y_key != 0) DoomDLL_Input(0, prev_y_key); 
        if (cur_y_key != 0)  DoomDLL_Input(1, cur_y_key);  
    }

    prev_stick_x = input->stickX;
    prev_stick_y = input->stickY;

    HANDLE_DOOM_BTN(BTN_A,      DOOMKEY_ENTER);    // Menu Select
    HANDLE_DOOM_BTN(BTN_CDOWN,  DOOMKEY_USE);      // Shoot
    HANDLE_DOOM_BTN(BTN_B,      DOOMKEY_FIRE);     // Open Doors
    HANDLE_DOOM_BTN(BTN_CUP,    DOOMKEY_ESCAPE);   // Pause Menu
    HANDLE_DOOM_BTN(BTN_CLEFT,  DOOMKEY_STRAFE_L); // Strafe Left
    HANDLE_DOOM_BTN(BTN_CRIGHT, DOOMKEY_STRAFE_R); // Strafe Right

    prev_buttons = input->buttonDown;
}

void OnGameRender(IEvent* event) {
    // if (CVarGetInteger("gSkipIntro", 0) == 0) {
    //     return;
    // }

    Gfx* head = &gPool[0];

    gDPPipeSync(head++);
    gDPSetOtherMode(head++,
                    G_AD_DISABLE | G_CD_DISABLE | G_CK_NONE | G_TC_FILT | G_TF_POINT | G_TT_IA16 | G_TL_TILE |
                        G_TD_CLAMP | G_TP_NONE | G_CYC_1CYCLE | G_PM_NPRIMITIVE,
                    G_AC_NONE | G_ZS_PRIM | G_RM_XLU_SURF | G_RM_XLU_SURF2);
    gDPSetCombineMode(head++, G_CC_DECALRGBA, G_CC_DECALRGBA);
    gDPSetColor(head++, G_SETPRIMCOLOR, 0xFFFFFFFF);

    DoomDLL_Tick(NULL);
    DoomDLL_ScreenCopy(gDoomScreenBuffer);
    HandleDoomInput();

    u32 rowsPerSlice = 3; 
    u32 totalSlices = (DOOMGENERIC_RESY + rowsPerSlice - 1) / rowsPerSlice;

    for (u32 i = 0; i < totalSlices; i++) {
        u32 currentY = i * rowsPerSlice;
        u32 rowsToDraw = (currentY + rowsPerSlice > DOOMGENERIC_RESY) ? (DOOMGENERIC_RESY - currentY) : rowsPerSlice;

        u32* slicePtr = (u32*)((u8*)gDoomScreenBuffer + (currentY * DOOMGENERIC_RESX * 4));
        gSPInvalidateTexCache(head++, slicePtr);
        Lib_TextureRect_RGBA32(&head, slicePtr, DOOMGENERIC_RESX, rowsToDraw, 0.0f, (f32)currentY, 1.0f, 1.0f);
    }

    print_text(OTRGetDimensionFromLeftEdgeOverride(5), 5, ",");

    gSPEndDisplayList(head);
    gSPDisplayList(gDisplayListHead++, gPool);
}

MOD_INIT() {
    SetupUI();
    gDoomScreenBuffer = (uint8_t*) malloc(DOOMGENERIC_RESX * DOOMGENERIC_RESY * 4);
    DoomDLL_Initialize(NULL);
    gListenerIDs[0] = REGISTER_LISTENER(RenderGamePost, EVENT_PRIORITY_NORMAL, OnGameRender);
}

MOD_EXIT() {
    C_RemoveSidebarEntry("My Mod");
    free(gDoomScreenBuffer);
    UNREGISTER_LISTENER(RenderGamePost, gListenerIDs[0]);
}