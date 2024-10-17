#define SABRE_DATA_STORE_DOUBLING_LIMIT 128
#define SABRE_DATA_STORE_GROW_AMOUNT 64 // SABRE_DATA_STORE_DOUBLING_LIMIT / 2

#define SABRE_DATA_STORE_AS_CAST_ARRAY(DATA_STORE, DATA_TYPE) (DATA_TYPE(DATA_STORE).elems)

#define SABRE_SPRITE_ACTOR "SABRE_SpriteActor"
#define SABRE_TEXTURE_ACTOR "SABRE_TextureActor"
#define SABRE_PROJECTILE_HANDLER_ACTOR "SABRE_ProjectileHandler"
#define SABRE_CEILING_ACTOR "SABRE_Ceiling"
#define SABRE_FLOOR_ACTOR "SABRE_Floor"

#define SABRE_DATA_STORE_AS_SPRITE_ARRAY(DATA_STORE) SABRE_DATA_STORE_AS_CAST_ARRAY(DATA_STORE, (SABRE_Sprite *))
#define SABRE_SPRITE_POINTER_CAST(POINTER) ((SABRE_Sprite *)POINTER)
#define SABRE_DATA_STORE_AS_TEXTURE_ARRAY(DATA_STORE) SABRE_DATA_STORE_AS_CAST_ARRAY(DATA_STORE, (SABRE_Texture *))
#define SABRE_TEXTURE_POINTER_CAST(POINTER) ((SABRE_Texture *)POINTER)

#define SABRE_MAX_LEVEL_EDGE_WRAP_DEPTH 5

#define SABRE_EDGE_MODE_NO_RENDER 0
#define SABRE_EDGE_MODE_TEXTURE 1
#define SABRE_EDGE_MODE_WRAP 2

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

#define SABRE_ANIMATOR_LITERAL(FPS, NFRAMES, SPRITE) { 0, 0, 0.0f, { FPS, NFRAMES, SPRITE } }

#define SABRE_PROJECTILE_HIT_WALL 1
#define SABRE_PROJECTILE_HIT_ENTITY 2
#define SABRE_PROJECTILE_HIT_FLOOR 3

// Not really anything like infinity, merely a very high value,
// but the word infinity is shorter than "very high value" :D
#define SABRE_INFINITY 1e30

typedef struct SABRE_DataStoreStruct
{
    size_t capacity; // the maximum amount of elements the store can hold at the moment
    size_t count; // the amount of elements the store actually holds at the moment
    size_t elemSize; // the size of a single element in the store
    void *elems; // pointer to the elements
    void (*addData)(struct SABRE_DataStoreStruct*, void*); // pointer to a function that handles adding data to the store
    void (*updateShortcutPointer)(void); // pointer to a function that handles updating a shortcut pointer to the data
}SABRE_DataStore;

typedef struct SABRE_AnimationStruct
{
    float frameRate;
    unsigned int nframes;
    unsigned int sprite;
}SABRE_Animation;

typedef struct SABRE_AnimatorStruct
{
    int state;
    int animpos;
    float accumulatedAnimpos;
    SABRE_Animation anim;
}SABRE_Animator;

typedef struct SABRE_EntityStruct
{
    float radius;
    Vector pos;
    Vector dir;
    unsigned int attributes;
    SABRE_Animator animator;
    char name[256];
}SABRE_Entity;

typedef struct SABRE_ProjectileStruct
{
    float speed;
    float dropFactor;
    Vector dir;
    SABRE_Entity *entity;
}SABRE_Projectile;

typedef union SABRE_ListTypesUnion
{
    struct SABRE_EntityStruct entity;
    struct SABRE_ProjectileStruct projectile;
}SABRE_ListTypes;

typedef struct SABRE_ListStruct
{
    struct SABRE_ListStruct *next;
    struct SABRE_ListStruct *prev;
    SABRE_ListTypes data;
}SABRE_List;

typedef struct SABRE_SpriteStruct
{
    unsigned int width;
    unsigned int height;
    unsigned int halfWidth;
    unsigned int halfHeight;
    unsigned int nframes;
    char name[256];
}SABRE_Sprite;

typedef struct SABRE_TextureStruct
{
    int width;
    int height;
    short slices;
    char isWindow;
    char name[256];
}SABRE_Texture;

typedef struct SABRE_SliceStruct
{
    short anim;
    short slice;
}SABRE_Slice;

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

typedef struct SABRE_LevelTileStruct
{
    unsigned texture;
    unsigned int properties;
    struct SABRE_RenderObjectStruct *renderObject[SABRE_MAX_LEVEL_EDGE_WRAP_DEPTH + 1];
}SABRE_LevelTile;

typedef struct SABRE_LevelStruct
{
    char validated;

    unsigned width;
    unsigned height;
    SABRE_LevelTile *map;
}SABRE_Level;

typedef struct SABRE_CameraStruct
{
    SABRE_Vector2 prevPos;
    SABRE_Vector2 pos;
    SABRE_Vector2 prevDir;
    SABRE_Vector2 dir;
    SABRE_Vector2 plane;
    float vPos;
}SABRE_Camera;

typedef enum SABRE_EventNameEnum
{
    LVL1_CASH_MACHINE_EVENT = 0,
    LVL1_WINDOW_EVENT
}SABRE_EventName;

typedef struct SABRE_EventTriggerStruct
{
    unsigned char type;
    SABRE_EventName event;
    SABRE_Vector2 p1;
    SABRE_Vector2 p2;
    float facingDir;
    float facingFov;
    unsigned char state;
}SABRE_EventTrigger;

typedef struct SABRE_ProjectileHitDataStruct
{
    float dist;
    unsigned char hitType;
    SABRE_Projectile *projectile;
    Vector hitPosition;
    SABRE_Entity *entityHit;
}SABRE_ProjectileHitData;

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

enum SABRE_EntityAttributeFlags
{
    SABRE_ENTITY_HIDDEN         = 1,
    SABRE_ENTITY_NO_COLLISION   = 2
};

// Actual sprite data store
SABRE_DataStore SABRE_spriteStore;
// A shortcut pointer to access the data from the store
// without having to cast pointers all the time
SABRE_Sprite *SABRE_sprites = NULL;

// Actual texture data store
SABRE_DataStore SABRE_textureStore;
// A shortcut pointer to access the data from the store
// without having to cast pointers all the time
SABRE_Texture *SABRE_textures = NULL;

SABRE_Slice SABRE_slice;

enum SABRE_RenderObjectTypeEnum
{
    SABRE_TEXTURE_RO,
    SABRE_SPRITE_RO
};

SABRE_Level SABRE_level;
SABRE_Camera SABRE_camera;

struct SABRE_GraphicsSettingsStruct
{
    unsigned char windowRenderDepth;    // how many windows can be rendered in a line, 0 means no limit
    unsigned char levelEdgeWrapDepth;   // how many times the raycast is allowed to wrap around the level in level edge mode 2
    unsigned char levelEdgeMode;        // how the level edges should be handled, 0: don't render, 1: render with specified texture
    int levelEdgeTextureIndex;          // the texture to use for level edge mode 1
}SABRE_graphicsSettings = { 0, 2, SABRE_EDGE_MODE_TEXTURE, 3 };


SABRE_List *SABRE_entities = NULL;
SABRE_List *SABRE_projectiles = NULL;

SABRE_ProjectileHitData SABRE_projectileHitData;

enum SABRE_GameStatesEnum
{
    SABRE_UNINITIALIZED = 0,
    SABRE_TEXTURES_ADDED,
    SABRE_SPRITES_ADDED,
    SABRE_INITIALIZED,
    SABRE_RUNNING,
    SABRE_FINISHED
}SABRE_gameState = SABRE_UNINITIALIZED;

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

SABRE_KeyboardState SABRE_keys;

struct SABRE_PlayerStruct
{
    float moveSpeed;
    float turnSpeed;
    float crouchSpeed;
    float crouchHeightChange;
    float radius;
}SABRE_player = { 0.05f, 0.05f, 5.0f, 60.0f, 0.2f };

SABRE_Color SABRE_defaultCeiling = { 215.0, 54.0, 91.0, 106, 158, 231, 1.0 };
SABRE_Color SABRE_defaultFloor   = {  86.0, 76.0, 62.0, 106, 158,  38, 1.0 };
