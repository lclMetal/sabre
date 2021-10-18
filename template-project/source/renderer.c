#if DEBUG
#define DEBUG_PRINT_MEMORY_USAGE 1
#endif

enum SABRE_RenderObjectTypeEnum
{
    SABRE_TextureRO,
    SABRE_SpriteRO
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

struct SABRE_RenderObjectListManagerStruct
{
    SABRE_RenderObject *head;
    SABRE_RenderObject *curr;
    SABRE_RenderObject *reusableCache;
}SABRE_ROListManager;

#if DEBUG
int allocations = 0;
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

    if (object->sortValue <= iterator->sortValue)
    {
        while (iterator && object->sortValue <= iterator->sortValue)
        {
            prev = iterator;
            iterator = iterator->next;
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
    new->objectType = SABRE_TextureRO;
    new->scale = scale;
    new->horizontalPosition = horizontalPosition;
    new->horizontalScalingCompensation = compensation;
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
    const float horizontalCompensationThreshold = 0.0315f; // threshold for growing the compensation

    for (iterator = SABRE_ROListManager.head; iterator != NULL; iterator = iterator->next)
    {
        SABRE_slice.anim = iterator->slice.anim;
        SABRE_slice.slice = iterator->slice.slice;
        SendActivationEvent("SABRE_TextureSlice");
        draw_from("SABRE_TextureSlice", iterator->horizontalPosition + iterator->horizontalScalingCompensation, verticalPosition, iterator->scale);
    }
}
