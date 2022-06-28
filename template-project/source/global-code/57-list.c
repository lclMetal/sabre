typedef union SABRE_ListTypesUnion
{
    SABRE_Entity entity;
    SABRE_Texture texture;
}SABRE_ListTypes;

typedef struct SABRE_ListStruct
{
    struct SABRE_ListStruct *next;
    SABRE_ListTypes data;
}SABRE_List;

SABRE_List *SABRE_entities = NULL;

#define SABRE_ENTITY_COUNT 11

SABRE_List *SABRE_NewListElement(SABRE_ListTypes elem);
SABRE_List *SABRE_AddToList(SABRE_List *list, SABRE_List *elem);
void SABRE_FreeList(SABRE_List *list);

void SABRE_SetEntities()
{
    int i;
    SABRE_List *new = NULL;
    SABRE_ListTypes data;

    SABRE_Entity tempEntities[SABRE_ENTITY_COUNT] =
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

    for (i = 0; i < SABRE_ENTITY_COUNT; i++)
    {
        data.entity = tempEntities[i];
        new = SABRE_NewListElement(data);
        if (new)
        {
            if ((SABRE_entities = SABRE_AddToList(SABRE_entities, new)))
                DEBUG_MSG_FROM("Added new entity", "SABRE_SetEntities");
        }
    }
}

int SABRE_CountEntitiesInList()
{
    int count = 0;
    SABRE_List *iterator = NULL;

    for (iterator = SABRE_entities; iterator != NULL; iterator = iterator->next)
    {
        count++;
    }

    return count;
}

void SABRE_FreeEntityList()
{
    SABRE_FreeList(SABRE_entities);
    SABRE_entities = NULL;
}

SABRE_List *SABRE_NewListElement(SABRE_ListTypes elem)
{
    SABRE_List *new = malloc(sizeof *new);

    if (!new)
    {
        DEBUG_MSG_FROM("Malloc failed!", "SABRE_NewListElement");
        return NULL;
    }

    new->data = elem;
    new->next = NULL;

    return new;
}

SABRE_List *SABRE_AddToList(SABRE_List *list, SABRE_List *elem)
{
    if (!list)
    {
        elem->next = NULL;
        list = elem;
        return list;
    }

    elem->next = list;
    list = elem;
    return list;
}

void SABRE_FreeList(SABRE_List *list)
{
    SABRE_List *temp = NULL;
    SABRE_List *iterator = list;

    while (iterator)
    {
        temp = iterator->next;
        free(iterator);
        iterator = temp;
    }
}
