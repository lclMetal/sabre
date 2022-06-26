// ..\source\global-code\00-debug.c
#define DEBUG 1 // 0: disable debug, 1: enable debug

#define FPS_AVERAGE 0
#define FPS_RANGE 1
#define FPS_BOTH 2

// -----DEBUG MACROS-----
//
// Can be used if DEBUG above == 1, otherwise the macros will expand to empty
// space. That way it's not necessary to remove them for shipping the game.
//
// Usage:
//     use DEBUG_INIT(); to initialize the timestamp base time and the output file
//         (optional, if omitted, will be initialized on first debug message)
//     use DEBUG_MSG("Message"); to write a debug message to the "debug.log" file
//     use DEBUG_MSG_FROM("Message", "Origin"); to write a debug message
//         and it's origin location to the "debug.log" file
//         example: DEBUG_MSG_FROM("Message", "view Create Actor");

#if DEBUG == 1
    #define DEBUG_INIT() debugCreateFile()
    #define DEBUG_MSG(X) debugMsg((X))
    #define DEBUG_MSG_FROM(X, Y) debugMsgFrom((X), (Y), __LINE__)
#else
    #define DEBUG_INIT()
    #define DEBUG_MSG(X)
    #define DEBUG_MSG_FROM(X, Y)
#endif

struct debugStruct
{
    int fileInitialized;
    unsigned long startTime;

    stTime fpsTime;
    unsigned long fpsLastUpdate;

    int fpsHeap;
    int fpsCount;
    int fpsFindLowest;
    int fpsFindHighest;

    int fpsLowest;
    int fpsHighest;
    double fpsAverage;
}debugController;

void debugPrintFps(int mode);
void debugUpdateFps(void);
int debugCreateFile(void);
void debugMsg(const char *msg);
void debugMsgFrom(const char *msg, const char *label, int line);

void debugPrintFps(int mode)
{
    debugUpdateFps();

    switch (mode)
    {
        default:
        case FPS_AVERAGE:
            sprintf(text, "FPS %.02f", debugController.fpsAverage);
        break;

        case FPS_RANGE:
            sprintf(text, "Min FPS: %i\nMax FPS: %i",
                debugController.fpsLowest, debugController.fpsHighest);
        break;

        case FPS_BOTH:
            sprintf(text, "FPS: %.02f\n+ min: %i\n+ max: %i",
                debugController.fpsAverage, debugController.fpsLowest, debugController.fpsHighest);
        break;
    }
}

void debugUpdateFps(void)
{
    debugController.fpsTime = getTime();

    if (!debugController.fpsLastUpdate)
        debugController.fpsLastUpdate = debugController.fpsTime.sec_utc;

    if (!debugController.fpsCount)
    {
        debugController.fpsFindLowest = 512;
        debugController.fpsFindHighest = 0;
    }

    debugController.fpsHeap += real_fps;
    debugController.fpsCount ++;

    if (real_fps < debugController.fpsFindLowest) debugController.fpsFindLowest = real_fps;
    if (real_fps > debugController.fpsFindHighest) debugController.fpsFindHighest = real_fps;

    if (debugController.fpsTime.sec_utc != debugController.fpsLastUpdate)
    {
        debugController.fpsAverage = debugController.fpsHeap / (double) debugController.fpsCount;
        debugController.fpsLowest = debugController.fpsFindLowest;
        debugController.fpsHighest = debugController.fpsFindHighest;
        debugController.fpsHeap = 0;
        debugController.fpsCount = 0;
        debugController.fpsFindLowest = 512;
        debugController.fpsFindHighest = 0;

        debugController.fpsLastUpdate = debugController.fpsTime.sec_utc;
    }
}

// It is recommended to only use this function through the macro DEBUG_INIT()
int debugCreateFile(void)
{
    FILE *f;

    f = fopen("debug.log", "w+");

    if (f)
    {
        stTime t = getTime();
        debugController.startTime = t.sec_utc;

        debugController.fileInitialized = 1;
        fclose(f);
        debugMsgFrom("Debug file initialized.", __FILE__, __LINE__);
        return true;
    }

    return false;
}

// It is recommended to only use this function through the macro DEBUG_MSG()
void debugMsg(const char *msg)
{
    FILE *f = NULL;

    if (!debugController.fileInitialized)
        debugCreateFile();

    f = fopen("debug.log", "a+");

    if (f)
    {
        fprintf(f, "%s\n", msg);
        fclose(f);
    }
}

// It is recommended to use this function only through the macro DEBUG_MSG_FROM()
void debugMsgFrom(const char *msg, const char *label, int line)
{
    char temp[256];
    stTime t = getTime();

    if (!debugController.fileInitialized)
        debugCreateFile();

    sprintf(temp, "[%-25s, line: %4i, time: %3i, frame: %5i]: \"%s\"",
            label, line, t.sec_utc - debugController.startTime, frame, msg);
    debugMsg(temp);
}


// ..\source\global-code\10-vector2.c
struct SABRE_Vector2Struct
{
    float x, y;
};

struct SABRE_Vector2Struct SABRE_CreateVector2(float x, float y)
{
    struct SABRE_Vector2Struct new;
    new.x = x;
    new.y = y;
    return new;
}

struct SABRE_Vector2Struct SABRE_ScaleVector2(struct SABRE_Vector2Struct vec, float scale)
{
    struct SABRE_Vector2Struct new;
    new.x = vec.x * scale;
    new.y = vec.y * scale;
    return new;
}

void SABRE_ScaleVector2InPlace(struct SABRE_Vector2Struct *vec, float scale)
{
    vec->x *= scale;
    vec->y *= scale;
}

struct SABRE_Vector2Struct SABRE_RotateVector2(struct SABRE_Vector2Struct vec, float rotation)
{
    struct SABRE_Vector2Struct new;
    new.x = vec.x * cos(rotation) - vec.y * sin(rotation);
    new.y = vec.x * sin(rotation) + vec.y * cos(rotation);
    return new;
}

void SABRE_RotateVector2InPlace(struct SABRE_Vector2Struct *vec, float rotation)
{
    float oldX = vec->x;
    vec->x = vec->x * cos(rotation) - vec->y * sin(rotation);
    vec->y = oldX   * sin(rotation) + vec->y * cos(rotation);
}

struct SABRE_Vector2Struct SABRE_AddVector2(struct SABRE_Vector2Struct a, struct SABRE_Vector2Struct b)
{
    struct SABRE_Vector2Struct new;
    new.x = a.x + b.x;
    new.y = a.y + b.y;
    return new;
}

void SABRE_AddVector2InPlace(struct SABRE_Vector2Struct *a, struct SABRE_Vector2Struct b)
{
    a->x += b.x;
    a->y += b.y;
}

float SABRE_DotProductVector2(struct SABRE_Vector2Struct a, struct SABRE_Vector2Struct b)
{
    return a.x * b.x + a.y * b.y;
}

float SABRE_MagnitudeVector2(struct SABRE_Vector2Struct a)
{
    return sqrt(SABRE_DotProductVector2(a, a));
}

struct SABRE_Vector2Struct SABRE_NormalizeVector2(struct SABRE_Vector2Struct vec)
{
    float magnitude = SABRE_MagnitudeVector2(vec);

    if (abs(magnitude) <= 0.0001f)
    {
        DEBUG_MSG_FROM("Unable to normalize: vector magnitude was 0.", "SABRE_NormalizeVector2");
        return SABRE_CreateVector2(0.0f, 0.0f);
    }

    return SABRE_ScaleVector2(vec, 1.0f / magnitude);
}

void SABRE_NormalizeVector2InPlace(struct SABRE_Vector2Struct *vec)
{
    float magnitude = SABRE_MagnitudeVector2(*vec);

    if (abs(magnitude) <= 0.0001f)
    {
        DEBUG_MSG_FROM("Unable to normalize: vector magnitude was 0.", "SABRE_NormalizeVector2InPlace");
        vec->x = 0.0f;
        vec->y = 0.0f;
        return;
    }

    SABRE_ScaleVector2InPlace(vec, 1.0f / magnitude);
}


// ..\source\global-code\20-misc-functions.c
float SABRE_NormalizeAngleTo360(float ang)
{
    float tempAng = ang;

    while (tempAng < 0)
    {
        tempAng += 360;
    }

    while (tempAng > 360.0f)
    {
        tempAng -= 360;
    }

    return tempAng;
}

int SABRE_ActorExists(const char *actorName)
{
    // GE uses the cloneindex -1 to indicate that an actor is not a valid, existing actor.
    // The actor count needs to be checked because:
    //
    // 1. IF an actor with the given base name exists in the project
    // 2. AND the lowest indexed clone (= the actor itself, if no clones present)
    //      in editor mode has "Create at startup" set to "No"
    // 3. AND the actor currently has no alive instances in the game
    //
    // ..getclone will return an invalid Actor* that has the cloneindex of that specific
    // clone / actor over in editor mode despite it not existing in the game.
    // And that would break this function without the ActorCount check.
    Actor *a = getclone(actorName);
    return (ActorCount(a->name) > 0 && a->cloneindex != -1);
}

// ----- WARNING! -----
//
// Using this function on a stale pointer is undefined behavior!
// Use this function ONLY for checking the result of getclone() right after fetching it.
//
// If you didn't fully understand the above warnings, DO NOT USE THIS FUNCTION!
// Use the above version instead (actorExists())
//
// This function only exists for optimizing performance in cases where a fresh actor pointer
// is already at hand.
int SABRE_ActorExists2(Actor *a)
{
    // GE uses the cloneindex -1 to indicate that an actor is not a valid, existing actor.
    // The actor count needs to be checked because:
    //
    // 1. IF an actor with the given base name exists in the project
    // 2. AND the lowest indexed clone (= the actor itself, if no clones present)
    //      in editor mode has "Create at startup" set to "No"
    // 3. AND the actor currently has no alive instances in the game
    //
    // ..getclone will return an invalid Actor* that has the cloneindex of that specific
    // clone / actor over in editor mode despite it not existing in the game.
    // And that would break this function without the ActorCount check.
    return (ActorCount(a->name) > 0 && a->cloneindex != -1);
}

int SABRE_StringEndsWith(const char *str, const char *str2)
{
    size_t len1 = strlen(str);
    size_t len2 = strlen(str2);

    if (len1 < len2)
    {
        return 0;
    }

    return !strcmp(&str[len1 - len2], str2);
}

#define SABRE_DIMENSION_X 0
#define SABRE_DIMENSION_Y 1

int SABRE_GetAnimationDimensionInPixels(const char actorName[256], const char animName[256], int dimension)
{
    int i = 0;
    int dimensionPixels = 0;
    int animFrameCount = 0;
    Actor *animationActor = getclone(actorName);

    if (!SABRE_ActorExists2(animationActor))
    {
        DEBUG_MSG_FROM("The actor doesn't exist.", "SABRE_GetAnimationDimensionInPixels");
        return 1;
    }

    ChangeAnimation(animationActor->clonename, animName, STOPPED);
    SendActivationEvent(actorName); // this "finalizes" the animation change, resetting the actor's animpos
    animFrameCount = animationActor->nframes;

    for (i = 0; i < animFrameCount; i++)
    {
        animationActor = getclone(animationActor->clonename); // this updates the width and height values

        switch (dimension)
        {
            default:
            case SABRE_DIMENSION_X:
                if (animationActor->width > dimensionPixels)
                {
                    dimensionPixels = animationActor->width;
                }
                break;

            case SABRE_DIMENSION_Y:
                if (animationActor->height > dimensionPixels)
                {
                    dimensionPixels = animationActor->height;
                }
                break;
        }

        animationActor->animpos++;
        // Send activation event to apply the animpos advancement during this frame already.
        // The normal behavior of Game Editor is to update the animpos of an actor in the next
        // frame. This same trick is used for changing the TextureSlice's animpos multiple
        // times per frame when drawing the game view. Notice, however, that if you were to
        // inspect the actor during a frame where this trick is used you still wouldn't see
        // the animpos changing more than once during a single frame. This is because Game Editor
        // only draws the actor on screen once per frame. But behind the scenes the animpos
        // still changes multiple times per frame, affecting the actor's dimensions as well as
        // its appearance if drawn using draw_from().
        SendActivationEvent(actorName);
    }

    return dimensionPixels;
}


// ..\source\global-code\30-data-store.c
#define SABRE_DATA_STORE_DOUBLING_LIMIT 128
#define SABRE_DATA_STORE_GROW_AMOUNT 64 // SABRE_DATA_STORE_DOUBLING_LIMIT / 2

#define SABRE_DATA_STORE_AS_CAST_ARRAY(DATA_STORE, DATA_TYPE) (DATA_TYPE(DATA_STORE).elems)

struct SABRE_DataStoreStruct
{
    size_t capacity; // the maximum amount of elements the store can hold at the moment
    size_t count; // the amount of elements the store actually holds at the moment
    size_t elemSize; // the size of a single element in the store
    void *elems; // pointer to the elements
    void (*addFunc)(struct SABRE_DataStoreStruct*, void*);
};

void SABRE_SetDataStoreAddFunc(struct SABRE_DataStoreStruct *dataStore, void (*addDataFunc)(struct SABRE_DataStoreStruct*, void*));
int SABRE_InitDataStore(struct SABRE_DataStoreStruct *dataStore, size_t elemSize);
int SABRE_GrowDataStore(struct SABRE_DataStoreStruct *dataStore);
int SABRE_PrepareDataStore(struct SABRE_DataStoreStruct *dataStore);
int SABRE_AddToDataStore(struct SABRE_DataStoreStruct *dataStore, void *elem);
void SABRE_FreeDataStore(struct SABRE_DataStoreStruct *dataStore);

void SABRE_SetDataStoreAddFunc(struct SABRE_DataStoreStruct *dataStore, void (*addDataFunc)(struct SABRE_DataStoreStruct*, void*))
{
    dataStore->addFunc = addDataFunc;
}

int SABRE_InitDataStore(struct SABRE_DataStoreStruct *dataStore, size_t elemSize)
{
    dataStore->capacity = 16;
    dataStore->count = 0;
    dataStore->elemSize = elemSize;
    dataStore->elems = calloc(dataStore->capacity, dataStore->elemSize);

    if (!dataStore->elems)
    {
        DEBUG_MSG_FROM("Memory allocation failed!", "SABRE_InitDataStore");
        return 1;
    }

    return 0;
}

int SABRE_GrowDataStore(struct SABRE_DataStoreStruct *dataStore)
{
    void *newElems = NULL;

    // double the data store size or grow it by SABRE_DATA_STORE_GROW_AMOUNT
    if (dataStore->capacity < SABRE_DATA_STORE_DOUBLING_LIMIT) dataStore->capacity *= 2;
    else dataStore->capacity += SABRE_DATA_STORE_GROW_AMOUNT;

    newElems = realloc(dataStore->elems, dataStore->capacity);

    if (!newElems)
    {
        DEBUG_MSG_FROM("Memory allocation failed!", "SABRE_GrowDataStore");
        return 1;
    }

    dataStore->elems = newElems;

    return 0;
}

int SABRE_PrepareDataStore(struct SABRE_DataStoreStruct *dataStore)
{
    // the data store has not been initialized, initialize it and make sure no errors occurred
    if (!dataStore->capacity && SABRE_InitDataStore(dataStore, dataStore->elemSize) != 0)
    {
        DEBUG_MSG_FROM("Unable to initialize data store.", "SABRE_PrepareDataStore");
        return 1;
    }
    // the data store is full, grow it and make sure no errors occurred
    else if (dataStore->count == dataStore->capacity && SABRE_GrowDataStore(dataStore) != 0)
    {
        DEBUG_MSG_FROM("Unable to grow data store.", "SABRE_PrepareDataStore");
        return 2;
    }
    // otherwise no-op

    return 0;
}

int SABRE_AddToDataStore(struct SABRE_DataStoreStruct *dataStore, void *elem)
{
    int err = 0;

    err = SABRE_PrepareDataStore(dataStore);

    if (err != 0) return err;

    dataStore->addFunc(dataStore, elem);
    dataStore->count++; // new element has been added, increment count

    return 0;
}

void SABRE_FreeDataStore(struct SABRE_DataStoreStruct *dataStore)
{
    if (dataStore->elems)
    {
        free(dataStore->elems);
        dataStore->capacity = 0;
        dataStore->count = 0;
        dataStore->elemSize = 0;
        dataStore->elems = NULL;
    }
}


// ..\source\global-code\35-engine.c
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
    struct SABRE_Vector2Struct prevDir;
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
    DEBUG_MSG_FROM("[init (1/5)] Signal textureActor to start adding textures.", "SABRE_Start");
    SendActivationEvent("SABRE_TextureActor");
    if (SABRE_gameState == SABRE_TEXTURES_ADDED)
    {
        DEBUG_MSG_FROM("[init (2/5)] Texture addition successful.", "SABRE_Start");
        DEBUG_MSG_FROM("[init (3/5)] Signal spriteActor to start adding sprites.", "SABRE_Start");
        SendActivationEvent("SABRE_SpriteActor");
        if (SABRE_gameState == SABRE_SPRITES_ADDED)
        {
            DEBUG_MSG_FROM("[init (4/5)] Sprite addition successful.", "SABRE_Start");
            CreateActor("SABRE_Screen", "icon", "(none)", "(none)", view.x, view.y, true);
            SABRE_gameState = SABRE_RUNNING;
            DEBUG_MSG_FROM("[init (5/5)] SABRE initialization complete.", "SABRE_Start");
        }
        else
        {
            DEBUG_MSG_FROM("[init (4/5)] Sprite addition failed.", "SABRE_Start");
            SABRE_Quit();
        }
    }
    else
    {
        DEBUG_MSG_FROM("[init (2/5)] Texture addition failed.", "SABRE_Start");
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
        DEBUG_MSG_FROM("[quit (1/4)] Freed texture store memory.", "SABRE_Quit");
        SABRE_FreeSpriteStore();
        DEBUG_MSG_FROM("[quit (2/4)] Freed sprite store memory.", "SABRE_Quit");
        SABRE_FreeRenderObjectList();
        DEBUG_MSG_FROM("[quit (3/4)] Freed render object list memory.", "SABRE_Quit");
        SABRE_gameState = SABRE_FINISHED;
        DEBUG_MSG_FROM("[quit (4/4)] SABRE cleanup complete.", "SABRE_Quit");
    }
}


// ..\source\global-code\37-event.c
#define SABRE_EVENT_PERMANENT_CHANGE      0
#define SABRE_EVENT_PRESENCE_TOGGLE       1
#define SABRE_EVENT_PRESENCE_TOGGLE_ONCE  2
#define SABRE_EVENT_SIGNAL                3
#define SABRE_EVENT_SIGNAL_ONCE           4

#define SABRE_EVENT_FLAG_ACTIVE 1
#define SABRE_EVENT_FLAG_LOCKED 2

#define SABRE_TRIGGER_HANDLER "SABRE_TriggerHandler"

#define SABRE_EVENT_ACTIVATE(EVENT) do { EVENT->state |= SABRE_EVENT_FLAG_ACTIVE;   \
                                         SABRE_updatedTrigger = EVENT;              \
                                         SendActivationEvent(SABRE_TRIGGER_HANDLER); } while (0)
#define SABRE_EVENT_DEACTIVATE(EVENT) do { EVENT->state &= (~SABRE_EVENT_FLAG_ACTIVE);  \
                                           SABRE_updatedTrigger = EVENT;                \
                                           SendActivationEvent(SABRE_TRIGGER_HANDLER); } while (0)

#define SABRE_EVENT_LOCK(EVENT) do { EVENT->state |= SABRE_EVENT_FLAG_LOCKED; } while (0)
#define SABRE_EVENT_UNLOCK(EVENT) do { EVENT->state &= (~SABRE_EVENT_FLAG_LOCKED); } while (0)

#define SABRE_EVENT_ACTIVE(EVENT) (EVENT->state & SABRE_EVENT_FLAG_ACTIVE)
#define SABRE_EVENT_NOT_ACTIVE(EVENT) (EVENT->state & SABRE_EVENT_FLAG_ACTIVE) == 0

#define SABRE_EVENT_LOCKED(EVENT) (EVENT->state & SABRE_EVENT_FLAG_LOCKED)
#define SABRE_EVENT_NOT_LOCKED(EVENT) (EVENT->state & SABRE_EVENT_FLAG_LOCKED) == 0

enum SABRE_EventNameEnum
{
    LVL1_CASH_MACHINE_EVENT = 0,
    LVL1_WINDOW_EVENT
};

struct SABRE_EventTriggerStruct
{
    unsigned char type;
    enum SABRE_EventNameEnum event;
    struct SABRE_Vector2Struct p1;
    struct SABRE_Vector2Struct p2;
    float facingDir;
    float facingFov;
    unsigned char state;
};

struct SABRE_EventTriggerStruct event1 =
{
    SABRE_EVENT_PRESENCE_TOGGLE,
    LVL1_CASH_MACHINE_EVENT,
    { 1, 8 },
    { 2, 9 },
    0.0f, 45.0f,
    0
};

struct SABRE_EventTriggerStruct event2 =
{
    SABRE_EVENT_PRESENCE_TOGGLE,
    LVL1_WINDOW_EVENT,
    { 0, 5 },
    { 2.6, 6 },
    0.0f, -30.0f
};

struct SABRE_EventTriggerStruct *SABRE_updatedTrigger = NULL;

void UpdateEvents();
void SABRE_UpdateEvent(struct SABRE_EventTriggerStruct *event);
void SABRE_EnterEventTrigger(struct SABRE_EventTriggerStruct *event);
void SABRE_StayInEventTrigger(struct SABRE_EventTriggerStruct *event);
void SABRE_LeaveEventTrigger(struct SABRE_EventTriggerStruct *event);

#if DEBUG
void SABRE_ResetEvent(struct SABRE_EventTriggerStruct *event);
void SABRE_CycleEventType(struct SABRE_EventTriggerStruct *event);
#endif

void SABRE_UpdateEvent(struct SABRE_EventTriggerStruct *event)
{
    struct SABRE_Vector2Struct camPos = SABRE_camera.pos;
    struct SABRE_Vector2Struct prevPos = SABRE_camera.prevPos;

    float deltaTo180 = 180.0f - event->facingDir;
    float normalizedDir = SABRE_NormalizeAngleTo360(direction(0, 0, SABRE_camera.dir.x, SABRE_camera.dir.y) + deltaTo180);
    float normalizedPrevDir = SABRE_NormalizeAngleTo360(direction(0, 0, SABRE_camera.prevDir.x, SABRE_camera.prevDir.y) + deltaTo180);

    short isFacingRightDirection = (event->facingFov < 0 || (abs(normalizedDir - 180.0f) <= event->facingFov / 2.0f));
    short wasFacingRightDirection = (event->facingFov < 0 || (abs(normalizedPrevDir - 180.0f) <= event->facingFov / 2.0f));

    short isInsideTrigger = (camPos.x >= event->p1.x &&
                                camPos.x <= event->p2.x &&
                                camPos.y >= event->p1.y &&
                                camPos.y <= event->p2.y);
    short wasInsideTrigger = (prevPos.x >= event->p1.x &&
                                prevPos.x <= event->p2.x &&
                                prevPos.y >= event->p1.y &&
                                prevPos.y <= event->p2.y);

    if (isFacingRightDirection && isInsideTrigger && (!wasFacingRightDirection && isFacingRightDirection || !wasInsideTrigger && isInsideTrigger))
    {
        SABRE_EnterEventTrigger(event);
    }
    else if (!isFacingRightDirection || !isInsideTrigger && (wasFacingRightDirection && wasInsideTrigger))
    {
        SABRE_LeaveEventTrigger(event);
    }
    else if (wasFacingRightDirection && isFacingRightDirection && wasInsideTrigger && isInsideTrigger)
    {
        SABRE_StayInEventTrigger(event);
    }
}

void SABRE_UpdateEvents()
{
    // TODO: Function that updates the events associated with the tile the player is standing in.
    SABRE_UpdateEvent(&event1);
    SABRE_UpdateEvent(&event2);
}

#if DEBUG
void SABRE_ResetEvent(struct SABRE_EventTriggerStruct *event)
{
    event->state = 0;
}

void SABRE_CycleEventType(struct SABRE_EventTriggerStruct *event)
{
    event->type++;
    if (event->type > 4) event->type = 0;
}
#endif

void SABRE_EnterEventTrigger(struct SABRE_EventTriggerStruct *event)
{
    switch (event->type)
    {
        case SABRE_EVENT_PERMANENT_CHANGE:
            if (SABRE_EVENT_NOT_ACTIVE(event))
            {
                SABRE_EVENT_ACTIVATE(event);
            }
            break;
        case SABRE_EVENT_PRESENCE_TOGGLE:
            SABRE_EVENT_ACTIVATE(event);
            break;
        case SABRE_EVENT_PRESENCE_TOGGLE_ONCE:
            if (SABRE_EVENT_NOT_LOCKED(event))
            {
                SABRE_EVENT_ACTIVATE(event);
                SABRE_EVENT_LOCK(event);
            }
            break;
        case SABRE_EVENT_SIGNAL:
            if (SABRE_EVENT_NOT_LOCKED(event))
            {
                SABRE_EVENT_ACTIVATE(event);
                SABRE_EVENT_LOCK(event);
            }
            else
            {
                SABRE_EVENT_DEACTIVATE(event);
            }
            break;
        case SABRE_EVENT_SIGNAL_ONCE:
            if (SABRE_EVENT_NOT_LOCKED(event))
            {
                SABRE_EVENT_ACTIVATE(event);
                SABRE_EVENT_LOCK(event);
            }
            else
            {
                SABRE_EVENT_DEACTIVATE(event);
            }
            break;
    }
}

void SABRE_StayInEventTrigger(struct SABRE_EventTriggerStruct *event)
{
    switch (event->type)
    {
        case SABRE_EVENT_SIGNAL:
        case SABRE_EVENT_SIGNAL_ONCE:
            SABRE_EnterEventTrigger(event);
            break;
        default:
            break;
    }
}

void SABRE_LeaveEventTrigger(struct SABRE_EventTriggerStruct *event)
{
    switch (event->type)
    {
        case SABRE_EVENT_PRESENCE_TOGGLE:
            SABRE_EVENT_DEACTIVATE(event);
            break;
        case SABRE_EVENT_PRESENCE_TOGGLE_ONCE:
            SABRE_EVENT_DEACTIVATE(event);
            break;
        case SABRE_EVENT_SIGNAL:
            if (SABRE_EVENT_LOCKED(event))
            {
                SABRE_EVENT_UNLOCK(event);
            }
            break;
        case SABRE_EVENT_SIGNAL_ONCE:
            if (SABRE_EVENT_LOCKED(event))
            {
                SABRE_EVENT_DEACTIVATE(event);
            }
            break;
    }
}


// ..\source\global-code\40-texture.c
#define SABRE_TEXTURE_ACTOR "SABRE_TextureActor"

struct SABRE_TextureStruct
{
    int width;
    int height;
    short slices;
    char isWindow;
    char name[256];
};

#define SABRE_DATA_STORE_AS_TEXTURE_ARRAY(DATA_STORE) SABRE_DATA_STORE_AS_CAST_ARRAY(DATA_STORE, (struct SABRE_TextureStruct *))
#define SABRE_TEXTURE_POINTER_CAST(POINTER) ((struct SABRE_TextureStruct *)POINTER)

// Actual texture data store
struct SABRE_DataStoreStruct SABRE_textureStore;
// A shortcut pointer to access the data from the store
// without having to cast pointers all the time
struct SABRE_TextureStruct *SABRE_textures = NULL;

int SABRE_AutoAddTextures();
int SABRE_AddTexture(const char textureName[256]);
int SABRE_CalculateTextureWidth(struct SABRE_TextureStruct *texture);
int SABRE_CalculateTextureHeight(struct SABRE_TextureStruct *texture);

void SABRE_AddTextureToDataStore(struct SABRE_DataStoreStruct *dataStore, void *texture);
void SABRE_FreeTextureStore();

// only works for non-animated textures
int SABRE_AutoAddTextures()
{
    int i = 0;
    int err = 0;
    char animName[256];

    strcpy(animName, getAnimName(i));
    SABRE_SetDataStoreAddFunc(&SABRE_textureStore, SABRE_AddTextureToDataStore);
    SABRE_textureStore.elemSize = sizeof(struct SABRE_TextureStruct);
    SABRE_PrepareDataStore(&SABRE_textureStore);

    // Set the shortcut pointer to allow easier access to texture data
    SABRE_textures = SABRE_DATA_STORE_AS_TEXTURE_ARRAY(SABRE_textureStore);

    while (strcmp(animName, "") != 0)
    {
        err = SABRE_AddTexture(animName);

        if (err) return err;

#if DEBUG
{
    char temp[256];
    sprintf(temp, "Added texture: [%d \"%s\"]", i, animName);
    DEBUG_MSG_FROM(temp, "SABRE_AutoAddTextures");
}
#endif

        strcpy(animName, getAnimName(++i));
    }

    return 0;
}

int SABRE_AddTexture(const char textureName[256])
{
    struct SABRE_TextureStruct newTexture;

    strcpy(newTexture.name, textureName);
    newTexture.width = SABRE_CalculateTextureWidth(&newTexture);
    newTexture.height = SABRE_CalculateTextureHeight(&newTexture);
    newTexture.isWindow = SABRE_StringEndsWith(newTexture.name, "-window");

    return SABRE_AddToDataStore(&SABRE_textureStore, &newTexture);
}

int SABRE_CalculateTextureWidth(struct SABRE_TextureStruct *texture)
{
    // TODO: make a check for if the animation actually exists, use getAnimIndex(), if -1, doesn't exist
    ChangeAnimation(SABRE_TEXTURE_ACTOR, texture->name, STOPPED);
    return getclone(SABRE_TEXTURE_ACTOR)->nframes;
}

int SABRE_CalculateTextureHeight(struct SABRE_TextureStruct *texture)
{
    return SABRE_GetAnimationDimensionInPixels(SABRE_TEXTURE_ACTOR, texture->name, SABRE_DIMENSION_Y);
}

void SABRE_AddTextureToDataStore(struct SABRE_DataStoreStruct *dataStore, void *texture)
{
    SABRE_textures[dataStore->count] = *SABRE_TEXTURE_POINTER_CAST(texture);
}

void SABRE_FreeTextureStore()
{
    SABRE_FreeDataStore(&SABRE_textureStore);
    SABRE_textures = NULL;
}


// ..\source\global-code\50-sprite.c
#define SABRE_SPRITE_ACTOR "SABRE_SpriteActor"

struct SABRE_SpriteStruct
{
    unsigned int width;
    unsigned int height;
    unsigned int halfWidth;
    unsigned int halfHeight;
    unsigned int sprite;
    char name[256];
};

#define SABRE_DATA_STORE_AS_SPRITE_ARRAY(DATA_STORE) SABRE_DATA_STORE_AS_CAST_ARRAY(DATA_STORE, (struct SABRE_SpriteStruct *))
#define SABRE_SPRITE_POINTER_CAST(POINTER) ((struct SABRE_SpriteStruct *)POINTER)

// Actual sprite data store
struct SABRE_DataStoreStruct SABRE_spriteStore;
// A shortcut pointer to access the data from the store
// without having to cast pointers all the time
struct SABRE_SpriteStruct *SABRE_sprites = NULL;

int SABRE_AutoAddSprites();
int SABRE_AddSprite(const char spriteName[256]);

void SABRE_AddSpriteToDataStore(struct SABRE_DataStoreStruct *dataStore, void *sprite);
void SABRE_FreeSpriteStore();

int SABRE_AutoAddSprites()
{
    int i = 0;
    int err = 0;
    char animName[256];

    strcpy(animName, getAnimName(i));
    SABRE_SetDataStoreAddFunc(&SABRE_spriteStore, SABRE_AddSpriteToDataStore);
    SABRE_spriteStore.elemSize = sizeof(struct SABRE_SpriteStruct);
    SABRE_PrepareDataStore(&SABRE_spriteStore);

    // Set the shortcut pointer to allow easier access to sprite data
    SABRE_sprites = SABRE_DATA_STORE_AS_SPRITE_ARRAY(SABRE_spriteStore);

    while (strcmp(animName, "") != 0)
    {
        err = SABRE_AddSprite(animName);

        if (err) return err;

#if DEBUG
{
    char temp[256];
    sprintf(temp, "Added sprite: [%d \"%s\"]", i, animName);
    DEBUG_MSG_FROM(temp, "SABRE_AutoAddSprites");
}
#endif

        strcpy(animName, getAnimName(++i));
    }

    return 0;
}

int SABRE_AddSprite(const char spriteName[256])
{
    struct SABRE_SpriteStruct newSprite;

    strcpy(newSprite.name, spriteName);
    newSprite.width = SABRE_GetAnimationDimensionInPixels(SABRE_SPRITE_ACTOR, spriteName, SABRE_DIMENSION_X);
    newSprite.height = SABRE_GetAnimationDimensionInPixels(SABRE_SPRITE_ACTOR, spriteName, SABRE_DIMENSION_Y);
    newSprite.halfWidth = newSprite.width * 0.5f;
    newSprite.halfHeight = newSprite.height * 0.5f;

    return SABRE_AddToDataStore(&SABRE_spriteStore, &newSprite);
}

void SABRE_AddSpriteToDataStore(struct SABRE_DataStoreStruct *dataStore, void *sprite)
{
    SABRE_sprites[dataStore->count] = *SABRE_SPRITE_POINTER_CAST(sprite);
}

void SABRE_FreeSpriteStore()
{
    SABRE_FreeDataStore(&SABRE_spriteStore);
    SABRE_sprites = NULL;
}


// ..\source\global-code\55-entity.c
enum SABRE_EntityAttributeFlags
{
    SABRE_ENTITY_HIDDEN = 1
};

struct SABRE_EntityStruct
{
    float radius;
    struct SABRE_Vector2Struct pos;
    // struct SABRE_SpriteStruct *sprite;
    unsigned int sprite;
    unsigned char attributes;
};

#define SABRE_ENTITY_COUNT 11

struct SABRE_EntityStruct entities[SABRE_ENTITY_COUNT] =
{
    { 0.4f, { 8.5f, 1.5f }, 1 },
    { 0.4f, { 7.5f, 1.5f }, 1 },
    { 0.4f, { 6.5f, 1.5f }, 2 },
    { 0.4f, { 8.5f, 2.5f }, 1 },
    { 0.4f, { 7.5f, 2.5f }, 0 },
    { 0.4f, { 6.5f, 2.5f }, 1 },
    { 0.4f, { 6.5f, 3.5f }, 3 },
    { 0.4f, { 7.0f, 3.5f }, 3 },
    { 0.4f, { 7.5f, 3.5f }, 3 },
    { 0.4f, { 8.0f, 3.5f }, 3 },
    { 0.4f, { 8.5f, 3.5f }, 3 }
};


// ..\source\global-code\60-renderer.c
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
    int horizontalPosition;
    int horizontalScalingCompensation;
    struct SABRE_SliceStruct slice;

    struct SABRE_RenderObjectStruct *prev;
    struct SABRE_RenderObjectStruct *next;
}SABRE_RenderObject;

SABRE_RenderObject *mapROs[LEVEL_HEIGHT][LEVEL_WIDTH];

struct SABRE_RenderObjectListManagerStruct
{
    SABRE_RenderObject *head;
    SABRE_RenderObject *curr;
    SABRE_RenderObject *reusableCache;
}SABRE_ROListManager;

#if DEBUG
int allocations = 0;
int traversals = 0;
int singleSliceTraversals = 0;
int maxTraversals = 0;
size_t allocatedMemory = 0;
#endif

void SABRE_FreeRenderObjectList()
{
    SABRE_RenderObject *iterator = NULL;
    SABRE_RenderObject *next = NULL;

    for (iterator = SABRE_ROListManager.head; iterator != NULL; iterator = next)
    {
        next = iterator->next;
        free(iterator);

#if DEBUG
        allocations--;
        allocatedMemory -= sizeof *iterator;
#endif
    }

    SABRE_ROListManager.head = NULL;
    SABRE_ROListManager.curr = NULL;
    SABRE_ROListManager.reusableCache = NULL;
}

SABRE_RenderObject *SABRE_GetLastROInList(SABRE_RenderObject *list)
{
    SABRE_RenderObject *iterator = NULL;

    for (iterator = list; iterator != NULL; iterator = iterator->next)
    {
        if (!iterator->next)
        {
            return iterator;
        }
    }

    return NULL;
}

SABRE_RenderObject *SABRE_ConcatenateROList(SABRE_RenderObject *dest, SABRE_RenderObject *src)
{
    SABRE_RenderObject *tail = SABRE_GetLastROInList(dest);
    SABRE_RenderObject *result = NULL;

    if (tail)
    {
        tail->next = src;
        result = dest;
    }
    else
    {
        result = src;
    }

    return result;
}

void SABRE_InitializeFrame()
{
    int i, j;

    for (j = 0; j < LEVEL_HEIGHT; j++)
    {
        for (i = 0; i < LEVEL_WIDTH; i++)
        {
            mapROs[j][i] = NULL;
        }
    }

#if DEBUG
    traversals = 0;
    maxTraversals = 0;
#endif

    SABRE_ROListManager.reusableCache = SABRE_ConcatenateROList(SABRE_ROListManager.reusableCache, SABRE_ROListManager.head);
    SABRE_ROListManager.head = NULL;
    SABRE_ROListManager.curr = NULL;
}

int SABRE_InsertRO(SABRE_RenderObject *object)
{
    SABRE_RenderObject *iterator = NULL;
    SABRE_RenderObject *prev = NULL;
    SABRE_RenderObject *next = NULL;

    if (!object)
    {
        return 1;
    }

    if (!SABRE_ROListManager.head || !SABRE_ROListManager.curr)
    {
        SABRE_ROListManager.head = object;
        SABRE_ROListManager.curr = object;
        return 0;
    }

    iterator = SABRE_ROListManager.curr;

#if DEBUG
    singleSliceTraversals = 0;
#endif

    if (object->sortValue <= iterator->sortValue)
    {
        while (iterator && object->sortValue <= iterator->sortValue)
        {
            prev = iterator;
            iterator = iterator->next;
#if DEBUG
            traversals++;
            singleSliceTraversals++;
#endif
        }

        if (iterator)
        {
            object->prev = iterator->prev;
            object->next = iterator;
            if (iterator->prev)
            {
                iterator->prev->next = object;
            }
            iterator->prev = object;

        }
        else
        {
            object->prev = prev;
            object->next = NULL;
            prev->next = object;
        }
        SABRE_ROListManager.curr = object;
    }
    else
    {
        while (iterator && object->sortValue > iterator->sortValue)
        {
            next = iterator;
            iterator = iterator->prev;
#if DEBUG
            traversals++;
            singleSliceTraversals++;
#endif
        }

        if (iterator)
        {
            object->prev = iterator;
            object->next = iterator->next;
            if (iterator->next)
            {
                iterator->next->prev = object;
            }
            iterator->next = object;
        }
        else
        {
            object->prev = NULL;
            object->next = next;
            next->prev = object;
            SABRE_ROListManager.head = object;
        }
        SABRE_ROListManager.curr = object;
    }

#if DEBUG
    if (singleSliceTraversals > maxTraversals) maxTraversals = singleSliceTraversals;
#endif

    return 0;
}

SABRE_RenderObject *SABRE_GetNextUnusedRO()
{
    if (SABRE_ROListManager.reusableCache)
    {
        SABRE_RenderObject *new = SABRE_ROListManager.reusableCache;
        SABRE_ROListManager.reusableCache = SABRE_ROListManager.reusableCache->next;
        return new;
    }
    else
    {
        SABRE_RenderObject *new = malloc(sizeof *new);

        if (!new)
        {
            DEBUG_MSG_FROM("Memory allocation failed!", "SABRE_GetNextUnusedRO");
            return NULL;
        }

#if DEBUG
    allocations++;
    allocatedMemory += sizeof *new;
#endif

        return new;
    }
}

SABRE_RenderObject *SABRE_AddTextureRO(float sortValue, float scale, int horizontalPosition, int compensation, struct SABRE_SliceStruct slice)
{
    int err = 0;
    SABRE_RenderObject *new = SABRE_GetNextUnusedRO();

    if (!new)
    {
        return NULL;
    }

    new->sortValue = sortValue;
    new->objectType = SABRE_TEXTURE_RO;
    new->scale = scale;
    new->horizontalPosition = horizontalPosition;
    new->horizontalScalingCompensation = compensation;
    new->slice = slice;
    new->prev = NULL;
    new->next = NULL;

    err = SABRE_InsertRO(new);

    return new;
}

SABRE_RenderObject *SABRE_AddSpriteRO(float sortValue, float scale, int horizontalPosition, struct SABRE_SliceStruct slice)
{
    int err = 0;
    SABRE_RenderObject *new = SABRE_GetNextUnusedRO();

    if (!new)
    {
        return NULL;
    }

    new->sortValue = sortValue;
    new->objectType = SABRE_SPRITE_RO;
    new->scale = scale;
    new->horizontalPosition = horizontalPosition;
    new->horizontalScalingCompensation = 0;
    new->slice = slice;
    new->prev = NULL;
    new->next = NULL;

    err = SABRE_InsertRO(new);

    return new;
}

void SABRE_PrintROList()
{
    int counter = 0;
    char temp[256];
    struct SABRE_RenderObjectStruct *iterator = NULL;

    sprintf(temp, "head: %f, curr: %f", SABRE_ROListManager.head->sortValue, SABRE_ROListManager.curr->sortValue);
    DEBUG_MSG(temp);

    for (iterator = SABRE_ROListManager.head; iterator != NULL; iterator = iterator->next)
    {
        sprintf(temp, "frame: %3d render object %3d: [sortValue: %f, scale: %f, hpos: %d, compensation: %d, slice: [anim: %d, slice: %d]]",
            frame, counter++, iterator->sortValue, iterator->scale, iterator->horizontalPosition, iterator->horizontalScalingCompensation,
            iterator->slice.anim, iterator->slice.slice);
        DEBUG_MSG(temp);
    }
}

void SABRE_RenderObjects()
{
    int horizontalPosition = 0;
    float verticalPosition = height * 0.5f;
    SABRE_RenderObject *iterator = NULL;
    float verticalResolutionFactor = screenHeight / 480.0f;
    const float horizontalCompensationThreshold = 0.0315f; // threshold for growing the compensation

    for (iterator = SABRE_ROListManager.head; iterator != NULL; iterator = iterator->next)
    {
        if (iterator->objectType == SABRE_TEXTURE_RO)
        {
            SABRE_slice.anim = iterator->slice.anim;
            SABRE_slice.slice = iterator->slice.slice;
            SendActivationEvent(SABRE_TEXTURE_ACTOR);
            draw_from(SABRE_TEXTURE_ACTOR, iterator->horizontalPosition + iterator->horizontalScalingCompensation, verticalPosition, iterator->scale);
        }
        else if (iterator->objectType == SABRE_SPRITE_RO)
        {
            SABRE_slice.anim = iterator->slice.anim;
            SABRE_slice.slice = iterator->slice.slice;
            SendActivationEvent(SABRE_SPRITE_ACTOR);
            draw_from(SABRE_SPRITE_ACTOR, iterator->horizontalPosition, verticalPosition + (((480.0f * iterator->scale) - ((float)iterator->scale * (float)getclone("SABRE_SpriteActor.0")->height))*0.5f) * verticalResolutionFactor, iterator->scale * verticalResolutionFactor);
        }
    }
}


