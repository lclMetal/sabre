#define DEBUG_INSPECT_DEPTH_ORDERING 1

#if DEBUG_INSPECT_DEPTH_ORDERING
#define DEBUG_PRINT_INSERTION_DATA 1
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
}SABRE_ROListManager;

#if DEBUG
int allocations;
size_t allocatedMemory;
#endif

#if DEBUG_INSPECT_DEPTH_ORDERING
int renderFrameByFrame = 0;
SABRE_RenderObject *globalIterator = NULL;
float globalVPos = 0;
const float globalHCThreshold = 0.0315f; // threshold for growing the compensation

int SABRE_GetPositionInROList(SABRE_RenderObject *object)
{
    int counter = -1;
    SABRE_RenderObject *iterator = NULL;

    if (!object || !SABRE_ROListManager.head)
    {
        return counter;
    }

    for (iterator = SABRE_ROListManager.head; iterator != NULL; iterator = iterator->next)
    {
        counter++;

        if (object == iterator)
        {
            return counter;
        }
    }

    return -2;
}
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
}

int SABRE_InsertRO(SABRE_RenderObject *object)
{
#if DEBUG_PRINT_INSERTION_DATA
    int counter = 0;
#endif
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
#if DEBUG_PRINT_INSERTION_DATA
            counter++;
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
#if DEBUG_PRINT_INSERTION_DATA
            counter++;
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

#if DEBUG_PRINT_INSERTION_DATA
if (renderFrameByFrame == 2)
{
    char temp[256];
    sprintf(temp, "Inserted slice %3d. List nodes traversed: %3d. Nodes from beginning of the list: %3d",
            object->horizontalPosition, counter, SABRE_GetPositionInROList(object));
    DEBUG_MSG(temp);
}
#endif

    return 0;
}

SABRE_RenderObject *SABRE_AddTextureRO(float sortValue, float scale, int horizontalPosition, int compensation, struct SABRE_SliceStruct slice)
{
    int err = 0;
    SABRE_RenderObject *new = malloc(sizeof *new);

    if (!new)
    {
        return NULL;
    }

#if DEBUG
    allocations++;
    allocatedMemory += sizeof *new;
#endif

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

#if DEBUG_INSPECT_DEPTH_ORDERING
    if (renderFrameByFrame == 0)
    {
#endif
        for (iterator = SABRE_ROListManager.head; iterator != NULL; iterator = iterator->next)
        {
            SABRE_slice.anim = iterator->slice.anim;
            SABRE_slice.slice = iterator->slice.slice;
            SendActivationEvent("SABRE_TextureSlice");
            draw_from("SABRE_TextureSlice", iterator->horizontalPosition + iterator->horizontalScalingCompensation, verticalPosition, iterator->scale);
        }
#if DEBUG_INSPECT_DEPTH_ORDERING
    }
    else if (renderFrameByFrame == 2)
    {
        renderFrameByFrame = 1;
        globalIterator = SABRE_ROListManager.head;
        globalVPos = height * 0.5f;
    }
#endif
}

#if DEBUG_INSPECT_DEPTH_ORDERING
void SABRE_Render1by1()
{
    if (globalIterator)
    {
        SABRE_slice.anim = globalIterator->slice.anim;
        SABRE_slice.slice = globalIterator->slice.slice;
        SendActivationEvent("SABRE_TextureSlice");
        draw_from("SABRE_TextureSlice", globalIterator->horizontalPosition + globalIterator->horizontalScalingCompensation,
            globalVPos, globalIterator->scale);
        globalIterator = globalIterator->next;
    }
    else
    {
        renderFrameByFrame = 0;
    }
}
#endif
