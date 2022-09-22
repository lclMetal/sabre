#define SABRE_CEILING_ACTOR "SABRE_Ceiling"
#define SABRE_FLOOR_ACTOR "SABRE_Floor"

typedef struct SABRE_SliceStruct
{
    short anim;
    short slice;
}SABRE_Slice;

#ifndef SABRE_RENDER_OBJECT_DEFINED
enum SABRE_RenderObjectTypeEnum
{
    SABRE_TEXTURE_RO,
    SABRE_SPRITE_RO
};

typedef struct SABRE_RenderObjectStruct
{
    float sortValue;
    enum SABRE_RenderObjectTypeEnum objectType;

    float scale;
    int width;
    int height;
    float verticalOffset;
    int horizontalPosition;
    int horizontalScalingCompensation;
    SABRE_Slice slice;

    struct SABRE_RenderObjectStruct *prev;
    struct SABRE_RenderObjectStruct *next;
}SABRE_RenderObject;
#define SABRE_RENDER_OBJECT_DEFINED
#endif

#ifndef SABRE_LEVEL_DEFINED
typedef struct SABRE_LevelTileStruct
{
    unsigned texture;
    unsigned long properties;
    struct SABRE_RenderObjectStruct *renderObject;
}SABRE_LevelTile;

typedef struct SABRE_LevelStruct
{
    char validated;

    unsigned width;
    unsigned height;
    SABRE_LevelTile *map;
}SABRE_Level;
#define SABRE_LEVEL_DEFINED
#endif

enum SABRE_GameStatesEnum
{
    SABRE_UNINITIALIZED = 0,
    SABRE_TEXTURES_ADDED,
    SABRE_SPRITES_ADDED,
    SABRE_RUNNING,
    SABRE_FINISHED
}SABRE_gameState = SABRE_UNINITIALIZED;

typedef struct SABRE_CameraStruct
{
    SABRE_Vector2 prevPos;
    SABRE_Vector2 pos;
    SABRE_Vector2 prevDir;
    SABRE_Vector2 dir;
    SABRE_Vector2 plane;
    float vPos;
}SABRE_Camera;

const float SABRE_heightUnit = 480.0f;
const float SABRE_halfHeightUnit = SABRE_heightUnit * 0.5f;
float SABRE_screenWidth = 640.0f, SABRE_screenHeight = 480.0f;

SABRE_Camera SABRE_camera;

struct SABRE_KeybindStruct
{
    short forward, backward;
    short turnLeft, turnRight;
    short strafeLeft, strafeRight;
    short crouch;
    short interact;
}SABRE_binds =
{
    KEY_w, KEY_s, // forward, backward
    KEY_a, KEY_d, // turn left, right
    KEY_q, KEY_e, // strafe left, right
    KEY_LCTRL,    // crouch
    KEY_r         // interact
};

typedef struct SABRE_KeyboardStateStruct
{
    char pressedForward,     releasedForward,     forward,     prevForward;
    char pressedBackward,    releasedBackward,    backward,    prevBackward;
    char pressedTurnLeft,    releasedTurnLeft,    turnLeft,    prevTurnLeft;
    char pressedTurnRight,   releasedTurnRight,   turnRight,   prevTurnRight;
    char pressedStrafeLeft,  releasedStrafeLeft,  strafeLeft,  prevStrafeLeft;
    char pressedStrafeRight, releasedStrafeRight, strafeRight, prevStrafeRight;
    char pressedCrouch,      releasedCrouch,      crouch,      prevCrouch;
    char pressedInteract,    releasedInteract,    interact,    prevInteract;
}SABRE_KeyboardState;

SABRE_KeyboardState SABRE_keys;

struct SABRE_PlayerStruct
{
    float moveSpeed;
    float turnSpeed;
    float crouchSpeed;
    float crouchHeightChange;
    float radius;
}SABRE_player = { 0.05f, 0.05f, 5.0f, 60.0f, 0.2f };

struct SABRE_GraphicsSettingsStruct
{
    unsigned char windowRenderDepth;// how many windows can be rendered in a line, 0 means no limit
    unsigned char levelEdgeMode;    // how the level edges should be handled, 0: don't render, 1: render with specified texture
    int levelEdgeTextureIndex;      // the texture to use for level edge mode 1
}SABRE_graphicsSettings = { 0, 2, 5 };

SABRE_Slice SABRE_slice;
SABRE_Color SABRE_defaultCeiling = { 215.0, 54.0, 91.0, 106, 158, 231, 1.0 };
SABRE_Color SABRE_defaultFloor   = {  86.0, 76.0, 62.0, 106, 158,  38, 1.0 };

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

void SABRE_SetCeilingColor(SABRE_Color color)
{
    SABRE_ColorActorByName(SABRE_CEILING_ACTOR, color);
}

void SABRE_SetFloorColor(SABRE_Color color)
{
    SABRE_ColorActorByName(SABRE_FLOOR_ACTOR, color);
}

int SABRE_GetSurroundingWalls(float *px, float *py, SABRE_Level *level)
{
    int i, j, edge = 0, rows = 3, cols = 3, mid = 0, collisions = 0;

    for (j = 0; j < rows; j++)
    {
        for (i = 0; i < cols; i++)
        {
            edge = 0;

            if (j == 1 && i == 1) mid = 1;
            else
            {
                int row = (int)*py - 1 + j;
                int col = (int)*px - 1 + i;

                if (SABRE_graphicsSettings.levelEdgeMode < 2)
                {
                    if (row < 1 || row > level->height - 2) edge = 1;
                    if (col < 1 || col > level->width - 2) edge = 1;
                }
                else
                {
                    row = SABRE_WrapIntValue(row, level->height);
                    col = SABRE_WrapIntValue(col, level->width);
                }

                collisions += (level->map[row * level->width + col].texture > 0 || edge) << SABRE_COLLISION_MASK_SIZE - (j * cols + i - mid);
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

    SABRE_keys.prevForward      = SABRE_keys.forward;
    SABRE_keys.prevBackward     = SABRE_keys.backward;
    SABRE_keys.prevTurnLeft     = SABRE_keys.turnLeft;
    SABRE_keys.prevTurnRight    = SABRE_keys.turnRight;
    SABRE_keys.prevStrafeLeft   = SABRE_keys.strafeLeft;
    SABRE_keys.prevStrafeRight  = SABRE_keys.strafeRight;
    SABRE_keys.prevCrouch       = SABRE_keys.crouch;
    SABRE_keys.prevInteract     = SABRE_keys.interact;

    SABRE_keys.forward          = keys[SABRE_binds.forward];
    SABRE_keys.backward         = keys[SABRE_binds.backward];
    SABRE_keys.turnLeft         = keys[SABRE_binds.turnLeft];
    SABRE_keys.turnRight        = keys[SABRE_binds.turnRight];
    SABRE_keys.strafeLeft       = keys[SABRE_binds.strafeLeft];
    SABRE_keys.strafeRight      = keys[SABRE_binds.strafeRight];
    SABRE_keys.crouch           = keys[SABRE_binds.crouch];
    SABRE_keys.interact         = keys[SABRE_binds.interact];

    SABRE_keys.pressedForward       = !SABRE_keys.prevForward       && SABRE_keys.forward;
    SABRE_keys.pressedBackward      = !SABRE_keys.prevBackward      && SABRE_keys.backward;
    SABRE_keys.pressedTurnLeft      = !SABRE_keys.prevTurnLeft      && SABRE_keys.turnLeft;
    SABRE_keys.pressedTurnRight     = !SABRE_keys.prevTurnRight     && SABRE_keys.turnRight;
    SABRE_keys.pressedStrafeLeft    = !SABRE_keys.prevStrafeLeft    && SABRE_keys.strafeLeft;
    SABRE_keys.pressedStrafeRight   = !SABRE_keys.prevStrafeRight   && SABRE_keys.strafeRight;
    SABRE_keys.pressedCrouch        = !SABRE_keys.prevCrouch        && SABRE_keys.crouch;
    SABRE_keys.pressedInteract      = !SABRE_keys.prevInteract      && SABRE_keys.interact;

    SABRE_keys.releasedForward      = SABRE_keys.prevForward        && !SABRE_keys.forward;
    SABRE_keys.releasedBackward     = SABRE_keys.prevBackward       && !SABRE_keys.backward;
    SABRE_keys.releasedTurnLeft     = SABRE_keys.prevTurnLeft       && !SABRE_keys.turnLeft;
    SABRE_keys.releasedTurnRight    = SABRE_keys.prevTurnRight      && !SABRE_keys.turnRight;
    SABRE_keys.releasedStrafeLeft   = SABRE_keys.prevStrafeLeft     && !SABRE_keys.strafeLeft;
    SABRE_keys.releasedStrafeRight  = SABRE_keys.prevStrafeRight    && !SABRE_keys.strafeRight;
    SABRE_keys.releasedCrouch       = SABRE_keys.prevCrouch         && !SABRE_keys.crouch;
    SABRE_keys.releasedInteract     = SABRE_keys.prevInteract       && !SABRE_keys.interact;
}

void SABRE_DisableActor(const char *actorName)
{
    VisibilityState(actorName, DISABLE);
    EventDisable(actorName, EVENTALL);
}

void SABRE_EnableActor(const char *actorName)
{
    VisibilityState(actorName, ENABLE);
    EventEnable(actorName, EVENTALL);
}

void SABRE_DisableActors()
{
    SABRE_DisableActor("SABRE_Screen");
    SABRE_DisableActor("SABRE_PlayerController");
    SABRE_DisableActor("SABRE_ProjectileHandler");
    SABRE_DisableActor("SABRE_TriggerHandler");
    SABRE_DisableActor("SABRE_TextureActor");
    SABRE_DisableActor("SABRE_SpriteActor");
    SABRE_DisableActor("SABRE_Ceiling");
    SABRE_DisableActor("SABRE_Floor");
}

void SABRE_EnableActors()
{
    SABRE_EnableActor("SABRE_Screen");
    SABRE_EnableActor("SABRE_PlayerController");
    SABRE_EnableActor("SABRE_ProjectileHandler");
    SABRE_EnableActor("SABRE_TriggerHandler");
    SABRE_EnableActor("SABRE_TextureActor");
    SABRE_EnableActor("SABRE_SpriteActor");
    SABRE_EnableActor("SABRE_Ceiling");
    SABRE_EnableActor("SABRE_Floor");
}

// from level.c
int SABRE_ValidateCurrentLevel();

// from entity.c
void SABRE_SetEntities();

void SABRE_Quit();

#define SABRE_STRINGIFY(X) #X
#define SABRE_EXPAND_STRINGIFY(X) SABRE_STRINGIFY(X)
#define SABRE_PROCESS_STEP_LABEL(NAME, STAGE, STAGES) "[" NAME " (" SABRE_STRINGIFY(STAGE) "/" SABRE_EXPAND_STRINGIFY(STAGES) ")] "

#define SABRE_INIT_STAGES 6
#define SABRE_INIT_STEP_LABEL(STAGE) SABRE_PROCESS_STEP_LABEL("init", STAGE, SABRE_INIT_STAGES)

#define SABRE_QUIT_STAGES 7
#define SABRE_QUIT_STEP_LABEL(STAGE) SABRE_PROCESS_STEP_LABEL("quit", STAGE, SABRE_QUIT_STAGES)

void SABRE_Start()
{
    SABRE_EnableActors();

    DEBUG_MSG_FROM(SABRE_INIT_STEP_LABEL(1) "Signal textureActor to start adding textures.", "SABRE_Start");
    SendActivationEvent("SABRE_TextureActor");
    if (SABRE_gameState == SABRE_TEXTURES_ADDED)
    {
        DEBUG_MSG_FROM(SABRE_INIT_STEP_LABEL(2) "Texture addition successful.", "SABRE_Start");
        DEBUG_MSG_FROM(SABRE_INIT_STEP_LABEL(3) "Signal spriteActor to start adding sprites.", "SABRE_Start");
        SendActivationEvent("SABRE_SpriteActor");
        if (SABRE_gameState == SABRE_SPRITES_ADDED)
        {
            DEBUG_MSG_FROM(SABRE_INIT_STEP_LABEL(4) "Sprite addition successful.", "SABRE_Start");

            if (SABRE_ValidateCurrentLevel() == 0)
                DEBUG_MSG_FROM(SABRE_INIT_STEP_LABEL(5) "Level validation done, no errors.", "SABRE_Start");
            else
                DEBUG_MSG_FROM(SABRE_INIT_STEP_LABEL(5) "Warning! At least one missing texture was detected.", "SABRE_Start");

            CreateActor("SABRE_Screen", "icon", "(none)", "(none)", view.x, view.y, true);
            CreateActor("SABRE_Ceiling", "background", "(none)", "(none)", view.x + view.width * 0.5, view.y + SABRE_Screen.height * 0.5 - 270, true);
            CreateActor("SABRE_Floor", "background", "(none)", "(none)", view.x + view.width * 0.5, view.y + SABRE_Screen.height * 0.5 + 270, true);
            SABRE_SetCeilingColor(SABRE_defaultCeiling);
            SABRE_SetFloorColor(SABRE_defaultFloor);
            SABRE_gameState = SABRE_RUNNING;
            DEBUG_MSG_FROM(SABRE_INIT_STEP_LABEL(6) "SABRE initialization complete.", "SABRE_Start");

            SABRE_SetEntities(); // Set the test entities
        }
        else
        {
            DEBUG_MSG_FROM(SABRE_INIT_STEP_LABEL(4) "Error! Sprite addition failed.", "SABRE_Start");
            SABRE_Quit();
        }
    }
    else
    {
        DEBUG_MSG_FROM(SABRE_INIT_STEP_LABEL(2) "Error! Texture addition failed.", "SABRE_Start");
        SABRE_Quit();
    }
}

int SABRE_FreeLevel();              // from level.c
void SABRE_FreeTextureStore();      // from texture.c
void SABRE_FreeSpriteStore();       // from sprite.c
void SABRE_FreeRenderObjectList();  // from render.c
void SABRE_FreeEntityList();        // from entity.c
void SABRE_FreeProjectileList();    // from projectile.c

void SABRE_Quit()
{
    if (SABRE_gameState != SABRE_FINISHED)
    {
        SABRE_DisableActors();

        SABRE_FreeLevel();
        DEBUG_MSG_FROM(SABRE_QUIT_STEP_LABEL(1) "Freed level data memory.", "SABRE_Quit");

        SABRE_FreeTextureStore();
        DEBUG_MSG_FROM(SABRE_QUIT_STEP_LABEL(2) "Freed texture store memory.", "SABRE_Quit");

        SABRE_FreeSpriteStore();
        DEBUG_MSG_FROM(SABRE_QUIT_STEP_LABEL(3) "Freed sprite store memory.", "SABRE_Quit");

        SABRE_FreeProjectileList();
        DEBUG_MSG_FROM(SABRE_QUIT_STEP_LABEL(4) "Freed projectile list memory.", "SABRE_Quit");

        SABRE_FreeEntityList();
        DEBUG_MSG_FROM(SABRE_QUIT_STEP_LABEL(5) "Freed entity list memory.", "SABRE_Quit");

        SABRE_FreeRenderObjectList();
        DEBUG_MSG_FROM(SABRE_QUIT_STEP_LABEL(6) "Freed render object list memory.", "SABRE_Quit");

        SABRE_gameState = SABRE_FINISHED;
        DEBUG_MSG_FROM(SABRE_QUIT_STEP_LABEL(7) "SABRE cleanup complete.", "SABRE_Quit");
    }
}
