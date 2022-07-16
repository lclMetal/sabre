enum SABRE_EntityAttributeFlags
{
    SABRE_ENTITY_HIDDEN         = 1,
    SABRE_ENTITY_NO_COLLISION   = 2
};

#ifndef SABRE_ENTITY_DEFINED
typedef struct SABRE_EntityStruct
{
    float radius;
    SABRE_Vector3 pos;
    unsigned int sprite;
    unsigned char attributes;
    char name[256];
}SABRE_Entity;
#define SABRE_ENTITY_DEFINED
#endif

SABRE_List *SABRE_entities = NULL;

SABRE_Entity *SABRE_AddEntity(float radius, SABRE_Vector3 pos, unsigned int sprite, unsigned char attributes, const char name[256]);
SABRE_Entity *SABRE_GetEntity(const char name[256]);
int SABRE_CountEntitiesInList();
void SABRE_FreeEntityList();

SABRE_Entity *movableFlowerPot = NULL;
SABRE_Entity *hiddenFlowerPot = NULL;

#define SABRE_ENTITY_COUNT 12

void SABRE_SetEntities()
{
    int i;
    SABRE_ListTypes data;

    SABRE_Entity tempEntities[SABRE_ENTITY_COUNT] =
    {
        { 0.15f, { 8.5f, 1.5f }, 1, 0, "barrel.0" },
        { 0.15f, { 7.5f, 1.5f }, 1, 0, "barrel.1" },
        { 0.15f, { 6.5f, 1.5f }, 2, 0, "terra-cotta-planter.0" },
        { 0.15f, { 8.5f, 2.5f }, 1, 0, "barrel.2" },
        { 0.14f, { 2.5f, 2.5f }, 0, 0, "pillar.0" },
        { 0.15f, { 6.5f, 2.5f }, 1, 0, "barrel.3" },
        { 0.19f, { 6.5f, 3.5f }, 3, 0, "flower-pot.0" },
        { 0.19f, { 7.0f, 3.5f }, 3, 0, "flower-pot.1" },
        { 0.19f, { 7.5f, 3.5f }, 3, 0, "flower-pot.2" },
        { 0.19f, { 8.0f, 3.5f }, 3, 0, "flower-pot.3" },
        { 0.19f, { 8.5f, 3.5f }, 3, 0, "flower-pot.4" },
        { 0.15f, { 2.5f, 8.5f }, 5, 0, "tree.4" }
    };

    for (i = 0; i < SABRE_ENTITY_COUNT; i++)
    {
        data.entity = tempEntities[i];

        if (SABRE_AddToList(&SABRE_entities, data))
            DEBUG_MSG_FROM("Added new entity", "SABRE_SetEntities");
    }
}

SABRE_Entity *SABRE_AddEntity(float radius, SABRE_Vector3 pos, unsigned int sprite, unsigned char attributes, const char name[256])
{
    SABRE_Entity new;
    SABRE_ListTypes newListElement;

    new.radius = radius;
    new.pos = pos;
    new.sprite = sprite;
    new.attributes = attributes;

    if (strlen(name) > 0)
        strcpy(new.name, name);
    else
        sprintf(new.name, "entity.%d", SABRE_CountEntitiesInList());

#if DEBUG
// {
    // char temp[300];
    // sprintf(temp, "Added entity: %s", name);
    // DEBUG_MSG_FROM(temp, "SABRE_AddEntity");
// }
#endif

    newListElement.entity = new;
    return &(SABRE_AddToList(&SABRE_entities, newListElement)->data.entity);
}

SABRE_Entity *SABRE_GetEntity(const char name[256])
{
    SABRE_List *iterator = NULL;

    for (iterator = SABRE_entities; iterator != NULL; iterator = iterator->next)
    {
        if (strcmp(name, iterator->data.entity.name) == 0)
        {
            return &(iterator->data.entity);
        }
    }

    return NULL;
}

SABRE_List *SABRE_GetEntityListObject(SABRE_Entity *entity)
{
    SABRE_List *iterator = NULL;

    for (iterator = SABRE_entities; iterator != NULL; iterator = iterator->next)
    {
        if (entity == &iterator->data.entity)
        {
            return iterator;
        }
    }

    return NULL;
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
