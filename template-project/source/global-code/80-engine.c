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
    SABRE_DisableActor("SABRE_Ceiling");
    SABRE_DisableActor("SABRE_Floor");
}

void SABRE_EnableActors()
{
    SABRE_EnableActor("SABRE_Screen");
    SABRE_EnableActor("SABRE_PlayerController");
    SABRE_EnableActor("SABRE_ProjectileHandler");
    SABRE_EnableActor("SABRE_TriggerHandler");
    SABRE_EnableActor("SABRE_Ceiling");
    SABRE_EnableActor("SABRE_Floor");
}

#define SABRE_STRINGIFY(X) #X
#define SABRE_EXPAND_STRINGIFY(X) SABRE_STRINGIFY(X)
#define SABRE_PROCESS_STEP_LABEL(NAME, STAGE, STAGES) "[" NAME " (" SABRE_STRINGIFY(STAGE) "/" SABRE_EXPAND_STRINGIFY(STAGES) ")] "

#define SABRE_INIT_STAGES 5
#define SABRE_INIT_STEP_LABEL(STAGE) SABRE_PROCESS_STEP_LABEL("init", STAGE, SABRE_INIT_STAGES)

#define SABRE_CLEANUP_STAGES 7
#define SABRE_CLEANUP_STEP_LABEL(STAGE) SABRE_PROCESS_STEP_LABEL("cleanup", STAGE, SABRE_CLEANUP_STAGES)

#define SABRE_START_STAGES 2
#define SABRE_START_STEP_LABEL(STAGE) SABRE_PROCESS_STEP_LABEL("start", STAGE, SABRE_START_STAGES)

void SABRE_ValidateGraphicsSettings()
{
    SABRE_graphicsSettings.levelEdgeWrapDepth = SABRE_LimitIntValue(SABRE_graphicsSettings.levelEdgeWrapDepth, 0, SABRE_MAX_LEVEL_EDGE_WRAP_DEPTH);
    SABRE_graphicsSettings.levelEdgeTextureIndex += 2;
    if (SABRE_ValidateTextureIndex(SABRE_graphicsSettings.levelEdgeTextureIndex - 1) == 0)
    {
        SABRE_graphicsSettings.levelEdgeTextureIndex = 1; // texture index 1 indicates a missing texture
    }
}

void SABRE_BuildTextureStore()
{
    if (SABRE_gameState < SABRE_TEXTURES_ADDED)
    {
        SendActivationEvent("SABRE_TextureActor");
    }
}

void SABRE_BuildSpriteStore()
{
    if (SABRE_gameState < SABRE_SPRITES_ADDED)
    {
        SendActivationEvent("SABRE_SpriteActor");
    }
}

void SABRE_CreateScreen()
{
    if (SABRE_gameState == SABRE_INITIALIZED)
    {
        CreateActor("SABRE_Screen", "icon", "(none)", "(none)", view.x, view.y, true);
        CreateActor("SABRE_Ceiling", "background", "(none)", "(none)", view.x + view.width * 0.5, view.y + SABRE_Screen.height * 0.5 - 270, true);
        CreateActor("SABRE_Floor", "background", "(none)", "(none)", view.x + view.width * 0.5, view.y + SABRE_Screen.height * 0.5 + 270, true);
        SABRE_SetCeilingColor(SABRE_defaultCeiling);
        SABRE_SetFloorColor(SABRE_defaultFloor);
    }
    else
    {
        DEBUG_MSG_FROM("SABRE has not been initialized, will not create screen.", "SABRE_CreateScreen");
    }
}

void SABRE_DestroyScreen()
{
    DestroyActor("SABRE_Screen");
    DestroyActor("SABRE_Ceiling");
    DestroyActor("SABRE_Floor");
}

void SABRE_Start()
{
    if (SABRE_gameState == SABRE_INITIALIZED)
    {
        if (SABRE_ValidateCurrentLevel() == 0)
        {
            DEBUG_MSG_FROM(SABRE_START_STEP_LABEL(1) "Level validation done, no errors.", "SABRE_Start");
        }
        else
        {
            DEBUG_MSG_FROM(SABRE_START_STEP_LABEL(1) "Warning! At least one missing texture was detected.", "SABRE_Start");
        }

        SABRE_ValidateGraphicsSettings();
        SABRE_CreateScreen();
        SABRE_EnableActors();
        SABRE_gameState = SABRE_RUNNING;
        DEBUG_MSG_FROM(SABRE_START_STEP_LABEL(2) "SABRE started.", "SABRE_Start");

        SABRE_SetEntities(); // Set the test entities
    }
    else
    {
        DEBUG_MSG_FROM("SABRE has not been initialized, can't start.", "SABRE_Start");
    }
}

void SABRE_Stop()
{
    if (SABRE_gameState == SABRE_RUNNING)
    {
        SABRE_gameState = SABRE_INITIALIZED;
        SABRE_DisableActors();
        SABRE_DestroyScreen();
        DEBUG_MSG_FROM("SABRE stopped.", "SABRE_Stop");
    }
    else
    {
        DEBUG_MSG_FROM("SABRE is not running, no-op.", "SABRE_Stop");
    }
}

void SABRE_Cleanup()
{
    SABRE_Stop();

    if (SABRE_gameState != SABRE_FINISHED)
    {
        SABRE_DisableActors();

        SABRE_FreeLevel();
        DEBUG_MSG_FROM(SABRE_CLEANUP_STEP_LABEL(1) "Freed level data memory.", "SABRE_Cleanup");

        SABRE_FreeTextureStore();
        DEBUG_MSG_FROM(SABRE_CLEANUP_STEP_LABEL(2) "Freed texture store memory.", "SABRE_Cleanup");

        SABRE_FreeSpriteStore();
        DEBUG_MSG_FROM(SABRE_CLEANUP_STEP_LABEL(3) "Freed sprite store memory.", "SABRE_Cleanup");

        SABRE_FreeProjectileList();
        DEBUG_MSG_FROM(SABRE_CLEANUP_STEP_LABEL(4) "Freed projectile list memory.", "SABRE_Cleanup");

        SABRE_FreeEntityList();
        DEBUG_MSG_FROM(SABRE_CLEANUP_STEP_LABEL(5) "Freed entity list memory.", "SABRE_Cleanup");

        SABRE_FreeRenderObjectList();
        DEBUG_MSG_FROM(SABRE_CLEANUP_STEP_LABEL(6) "Freed render object list memory.", "SABRE_Cleanup");

        SABRE_gameState = SABRE_FINISHED;
        DEBUG_MSG_FROM(SABRE_CLEANUP_STEP_LABEL(7) "SABRE cleanup complete.", "SABRE_Cleanup");
    }
}

void SABRE_Initialize()
{
    int nextStep = 1;

    while (nextStep)
    {
        switch (SABRE_gameState)
        {
            case SABRE_UNINITIALIZED:
                DEBUG_MSG_FROM(SABRE_INIT_STEP_LABEL(1) "Signal textureActor to start adding textures.", "SABRE_Initialize");
                SABRE_BuildTextureStore();
                if (SABRE_gameState != SABRE_TEXTURES_ADDED)
                {
                    DEBUG_MSG_FROM(SABRE_INIT_STEP_LABEL(2) "Error! Texture addition failed.", "SABRE_Initialize");
                    SABRE_Cleanup();
                    nextStep = 0;
                }
                break;

            case SABRE_TEXTURES_ADDED:
                DEBUG_MSG_FROM(SABRE_INIT_STEP_LABEL(2) "Texture addition successful.", "SABRE_Initialize");
                DEBUG_MSG_FROM(SABRE_INIT_STEP_LABEL(3) "Signal spriteActor to start adding sprites.", "SABRE_Initialize");
                SABRE_BuildSpriteStore();
                if (SABRE_gameState != SABRE_SPRITES_ADDED)
                {
                    DEBUG_MSG_FROM(SABRE_INIT_STEP_LABEL(4) "Error! Sprite addition failed.", "SABRE_Initialize");
                    SABRE_Cleanup();
                    nextStep = 0;
                }
                break;

            case SABRE_SPRITES_ADDED:
                DEBUG_MSG_FROM(SABRE_INIT_STEP_LABEL(4) "Sprite addition successful.", "SABRE_Initialize");
                DEBUG_MSG_FROM(SABRE_INIT_STEP_LABEL(5) "SABRE initialization complete.", "SABRE_Initialize");
                SABRE_gameState = SABRE_INITIALIZED;
                break;

            case SABRE_INITIALIZED:
                nextStep = 0;
                break;

            default:
                nextStep = 0;
                break;
        }
    }
}

void SABRE_InitializeAndStart()
{
    SABRE_Initialize();
    SABRE_Start();
}
