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
    int i, j, k;
    size_t index = 0;

    for (j = 0; j < SABRE_level.height; j++)
    {
        for (i = 0; i < SABRE_level.width; i++)
        {
            index = j * SABRE_level.width + i;

            for (k = 0; k <= SABRE_MAX_LEVEL_EDGE_WRAP_DEPTH; k++)
            {
                SABRE_level.map[index].renderObject[k] = NULL;
            }
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

SABRE_RenderObject *SABRE_AddTextureRO(float sortValue, float scale, int width, int height, int horizontalPosition, int compensation, SABRE_Slice slice)
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
    new->width = width;
    new->height = height;
    new->verticalOffset = 0;
    new->horizontalPosition = horizontalPosition;
    new->horizontalScalingCompensation = compensation;
    new->slice = slice;
    new->prev = NULL;
    new->next = NULL;

    err = SABRE_InsertRO(new);

    return new;
}

SABRE_RenderObject *SABRE_AddSpriteRO(float sortValue, float scale, int horizontalPosition, float verticalOffset, SABRE_Slice slice)
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
    new->width = 0;
    new->height = 0;
    new->verticalOffset = verticalOffset;
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

SABRE_Vector2 SABRE_WorldToScreen(SABRE_Vector2 pos)
{
    float invDet = 1.0f / (float)(SABRE_camera.plane.x * SABRE_camera.dir.y - SABRE_camera.dir.x * SABRE_camera.plane.y);
    SABRE_Vector2 translatedPos;
    SABRE_Vector2 transformedPos;
    SABRE_Vector2 screenPos;
    translatedPos.x = pos.x - SABRE_camera.pos.x;
    translatedPos.y = pos.y - SABRE_camera.pos.y;

    transformedPos.x = invDet * (SABRE_camera.dir.y * translatedPos.x - SABRE_camera.dir.x * translatedPos.y);
    transformedPos.y = invDet * (-SABRE_camera.plane.y * translatedPos.x + SABRE_camera.plane.x * translatedPos.y);

    screenPos.x = (SABRE_screenWidth / 2.0f) * (1 + transformedPos.x / transformedPos.y);
    screenPos.y = (SABRE_screenHeight / 2.0f) * (1 + 1.0f / transformedPos.y);//(1 + translatedPos.y / transformedPos.y);

    return screenPos;
}

void SABRE_RenderObjects()
{
    int horizontalPosition = 0;
    float verticalPosition = height * 0.5f;
    SABRE_RenderObject *iterator = NULL;
    float verticalResolutionFactor = SABRE_screenHeight / SABRE_heightUnit;
    const float horizontalCompensationThreshold = 0.0315f; // threshold for growing the compensation

    textureDrawCalls = 0;
    spriteDrawCalls = 0;

    for (iterator = SABRE_ROListManager.head; iterator != NULL; iterator = iterator->next)
    {
        if (iterator->objectType == SABRE_TEXTURE_RO)
        {
            SABRE_slice.anim = iterator->slice.anim;
            SABRE_slice.slice = iterator->slice.slice;
            SendActivationEvent(SABRE_TEXTURE_ACTOR);
            draw_from(SABRE_TEXTURE_ACTOR, iterator->horizontalPosition + iterator->horizontalScalingCompensation, verticalPosition - SABRE_camera.vPos * (iterator->height / SABRE_halfHeightUnit),
                iterator->scale);
            textureDrawCalls++;
        }
        else if (iterator->objectType == SABRE_SPRITE_RO)
        {
            SABRE_slice.anim = iterator->slice.anim;
            SABRE_slice.slice = iterator->slice.slice;
            SendActivationEvent(SABRE_SPRITE_ACTOR);
            draw_from(SABRE_SPRITE_ACTOR, iterator->horizontalPosition,
                verticalPosition + ((SABRE_halfHeightUnit - (float)SABRE_sprites[SABRE_slice.anim - 1].halfHeight - SABRE_camera.vPos * 2) - (iterator->verticalOffset * SABRE_heightUnit)) * iterator->scale * verticalResolutionFactor,
                iterator->scale * verticalResolutionFactor);
            spriteDrawCalls++;
        }
    }
}
