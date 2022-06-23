enum SABRE_GameStatesEnum
{
    SABRE_UNINITIALIZED = 0,
    SABRE_TEXTURES_ADDED,
    SABRE_SPRITES_ADDED,
    SABRE_RUNNING,
    SABRE_FINISHED
}SABRE_gameState = SABRE_UNINITIALIZED;

struct SABRE_CameraStruct
{
    struct SABRE_Vector2Struct prevPos;
    struct SABRE_Vector2Struct pos;
    struct SABRE_Vector2Struct dir;
    struct SABRE_Vector2Struct plane;
}SABRE_camera;

struct SABRE_KeybindStruct
{
    char forward, backward;
    char turnLeft, turnRight;
    char strafeLeft, strafeRight;
    char interact;
}SABRE_binds =
{
    KEY_w, KEY_s, // forward, backward
    KEY_a, KEY_d, // turn left, right
    KEY_q, KEY_e, // strafe left, right
    KEY_r         // interact
};

struct SABRE_KeyboardState
{
    char forward, backward;
    char turnLeft, turnRight;
    char strafeLeft, strafeRight;
    char interact;
}SABRE_keys;

struct SABRE_PlayerStruct
{
    float moveSpeed;
    float turnSpeed;
    struct SABRE_KeybindStruct keys;
    struct SABRE_CameraStruct camera;
}SABRE_player;

struct SABRE_SliceStruct
{
    short anim;
    short slice;
}SABRE_slice;

// x = player
// 000     abc
// 0x0  => d e
// 000     fgh

#define SABRE_COLLISION_MASK_SIZE 7
#define SABRE_TOP_L      0x80 // a
#define SABRE_TOP_L_MASK 0xD0 // a && b && d
#define SABRE_TOP        0x40 // b
#define SABRE_TOP_MASK   0x40 // b
#define SABRE_TOP_R      0x20 // c
#define SABRE_TOP_R_MASK 0x68 // c && b && e
#define SABRE_LEFT       0x10 // d
#define SABRE_LEFT_MASK  0x10 // d
#define SABRE_RIGHT      0x08 // e
#define SABRE_RIGHT_MASK 0x08 // e
#define SABRE_LOW_L      0x04 // f
#define SABRE_LOW_L_MASK 0x16 // f && d && g
#define SABRE_LOW        0x02 // g
#define SABRE_LOW_MASK   0x02 // g
#define SABRE_LOW_R      0x01 // h
#define SABRE_LOW_R_MASK 0x0B // h && e && g

int SABRE_GetSurroundingWalls(float *px, float *py, int map[LEVEL_HEIGHT][LEVEL_WIDTH])
{
    int i, j, rows = 3, cols = 3, mid = 0, collisions = 0;

    for (j = 0; j < rows; j++)
    {
        for (i = 0; i < cols; i++)
        {
            if (j == 1 && i == 1) mid = 1;
            else
            {
                int row = (int)*py - 1 + j;
                int col = (int)*px - 1 + i;

                collisions += (map[row][col] > 0) << SABRE_COLLISION_MASK_SIZE - (j * cols + i - mid);
            }
        }
    }

    return collisions;
}

void SABRE_KeepDistance(float *x1, float *y1, float x2, float y2, float dist)
{
    float len = distance(*x1, *y1, x2, y2);
    float ang = degtorad(direction(x2, y2, *x1, *y1));

    if (len < dist)
    {
        // multiply the results by 1.001 to make the above condition evaluate
        // to false if the player hasn't moved since their position was last
        // modified by this function
        *x1 = x2 + cos(ang) * dist * 1.001f;
        *y1 = y2 + -sin(ang) * dist * 1.001f;
    }
}

void SABRE_UpdateKeyboardState()
{
    char *keys = GetKeyState();

    SABRE_keys.forward      = keys[SABRE_binds.forward];
    SABRE_keys.backward     = keys[SABRE_binds.backward];
    SABRE_keys.turnLeft     = keys[SABRE_binds.turnLeft];
    SABRE_keys.turnRight    = keys[SABRE_binds.turnRight];
    SABRE_keys.strafeLeft   = keys[SABRE_binds.strafeLeft];
    SABRE_keys.strafeRight  = keys[SABRE_binds.strafeRight];
    SABRE_keys.interact     = keys[SABRE_binds.interact];
}

void SABRE_Quit();

void SABRE_Start()
{
    DEBUG_MSG_FROM("[init sequence (1/5)] Signal textureActor to start adding textures.", "SABRE_Start");
    SendActivationEvent("SABRE_TextureActor");
    if (SABRE_gameState == SABRE_TEXTURES_ADDED)
    {
        DEBUG_MSG_FROM("[init sequence (2/5)] Texture addition successful.", "SABRE_Start");
        DEBUG_MSG_FROM("[init sequence (3/5)] Signal spriteActor to start adding sprites.", "SABRE_Start");
        SendActivationEvent("SABRE_SpriteActor");
        if (SABRE_gameState == SABRE_SPRITES_ADDED)
        {
            DEBUG_MSG_FROM("[init sequence (4/5)] Sprite addition successful.", "SABRE_Start");
            CreateActor("SABRE_Screen", "icon", "(none)", "(none)", view.x, view.y, true);
            SABRE_gameState = SABRE_RUNNING;
            DEBUG_MSG_FROM("[init sequence (5/5)] SABRE initialization complete.", "SABRE_Start");
        }
        else
        {
            DEBUG_MSG_FROM("[init sequence (4/5)] Sprite addition failed.", "SABRE_Start");
            SABRE_Quit();
        }
    }
    else
    {
        DEBUG_MSG_FROM("[init sequence (2/5)] Texture addition failed.", "SABRE_Start");
        SABRE_Quit();
    }
}

void SABRE_FreeTextureStore();
void SABRE_FreeSpriteStore();
void SABRE_FreeRenderObjectList();

void SABRE_Quit()
{
    if (SABRE_gameState != SABRE_FINISHED)
    {
        VisibilityState("SABRE_Screen", DISABLE);
        VisibilityState("SABRE_PlayerController", DISABLE);
        VisibilityState("SABRE_TextureActor", DISABLE);
        VisibilityState("SABRE_SpriteActor", DISABLE);
        EventDisable("SABRE_Screen", EVENTALL);
        EventDisable("SABRE_PlayerController", EVENTALL);
        EventDisable("SABRE_TextureActor", EVENTALL);
        EventDisable("SABRE_SpriteActor", EVENTALL);
        SABRE_FreeTextureStore();
        SABRE_FreeSpriteStore();
        SABRE_FreeRenderObjectList();
        SABRE_gameState = SABRE_FINISHED;
    }
}
