SABRE_Entity *SABRE_AddEntity(float radius, SABRE_Vector3 pos, SABRE_Animator animator, unsigned int attributes, const char name[256]);
SABRE_Entity *SABRE_GetEntity(const char name[256]);
int SABRE_CountEntitiesInList();
void SABRE_FreeEntityList();

SABRE_Entity *movableFlowerPot = NULL;
SABRE_Entity *hiddenFlowerPot = NULL;
SABRE_Entity *inCorner = NULL;

#define SABRE_ENTITY_COUNT 22

void SABRE_SetEntities()
{
    int i;
    SABRE_ListTypes data;

    SABRE_Entity tempEntities[SABRE_ENTITY_COUNT] =
    {
        { 0.15f, { 8.5f, 1.5f, 0.0f }, 0, SABRE_ANIMATOR_LITERAL(0, 1, 1), "barrel.0" },
        { 0.15f, { 7.5f, 1.5f, 0.0f }, 0, SABRE_ANIMATOR_LITERAL(0, 1, 1), "barrel.1" },
        { 0.15f, { 6.5f, 1.5f, 0.0f }, 0, SABRE_ANIMATOR_LITERAL(0, 1, 2), "terra-cotta-planter.0" },
        { 0.15f, { 8.5f, 2.5f, 0.0f }, 0, SABRE_ANIMATOR_LITERAL(0, 1, 1), "barrel.2" },
        { 0.14f, { 2.5f, 2.5f, 0.0f }, 0, SABRE_ANIMATOR_LITERAL(0, 1, 0), "pillar.0" },
        { 0.15f, { 6.5f, 2.5f, 0.0f }, 0, SABRE_ANIMATOR_LITERAL(0, 1, 1), "barrel.3" },
        { 0.19f, { 6.5f, 3.5f, 0.0f }, 0, SABRE_ANIMATOR_LITERAL(0, 1, 3), "flower-pot.0" },
        { 0.19f, { 7.0f, 3.5f, 0.0f }, 0, SABRE_ANIMATOR_LITERAL(0, 1, 3), "flower-pot.1" },
        { 0.19f, { 7.5f, 3.5f, 0.0f }, 0, SABRE_ANIMATOR_LITERAL(0, 1, 3), "flower-pot.2" },
        { 0.19f, { 8.0f, 3.5f, 0.0f }, 0, SABRE_ANIMATOR_LITERAL(0, 1, 3), "flower-pot.3" },
        { 0.19f, { 8.5f, 3.5f, 0.0f }, 0, SABRE_ANIMATOR_LITERAL(0, 1, 3), "flower-pot.4" },
        { 0.19f, { 6.5f, 5.5f, 0.0f }, 0, SABRE_ANIMATOR_LITERAL(0, 1, 9), "pit.0" },
        { 0.19f, { 7.0f, 5.5f, 0.0f }, 0, SABRE_ANIMATOR_LITERAL(0, 1, 9), "pit.1" },
        { 0.19f, { 7.5f, 5.5f, 0.0f }, 0, SABRE_ANIMATOR_LITERAL(0, 1, 9), "pit.2" },
        { 0.19f, { 8.0f, 5.5f, 0.0f }, 0, SABRE_ANIMATOR_LITERAL(0, 1, 9), "pit.3" },
        { 0.19f, { 8.5f, 5.5f, 0.0f }, 0, SABRE_ANIMATOR_LITERAL(0, 1, 9), "pit.4" },
        { 0.19f, { 6.75f, 5.5f, 0.0f }, 0, SABRE_ANIMATOR_LITERAL(0, 1, 9), "pit.5" },
        { 0.19f, { 7.25f, 5.5f, 0.0f }, 0, SABRE_ANIMATOR_LITERAL(0, 1, 9), "pit.6" },
        { 0.19f, { 7.75f, 5.5f, 0.0f }, 0, SABRE_ANIMATOR_LITERAL(0, 1, 9), "pit.7" },
        { 0.19f, { 8.25f, 5.5f, 0.0f }, 0, SABRE_ANIMATOR_LITERAL(0, 1, 9), "pit.8" },
        { 0.19f, { 8.75f, 5.5f, 0.0f }, 0, SABRE_ANIMATOR_LITERAL(0, 1, 9), "pit.9" },
        // { 0.15f, { 2.5f, 8.5f }, 0, SABRE_ANIMATOR_LITERAL(0, 1, 5), "tree.4" }
        { 0.15f, { 2.5f, 8.5f, 0.0f }, 0, SABRE_ANIMATOR_LITERAL(10, 1, 7), "dude.0" }
    };

    for (i = 0; i < SABRE_ENTITY_COUNT; i++)
    {
        data.entity = tempEntities[i];
        data.entity.animator = SABRE_CreateAnimator(SABRE_CreateAnimation(tempEntities[i].animator.anim.frameRate, tempEntities[i].animator.anim.sprite));

        if (SABRE_AddToList(&SABRE_entities, data))
            DEBUG_MSG_FROM("Added new entity", "SABRE_SetEntities");
    }
}

SABRE_Entity *SABRE_AddEntity(float radius, SABRE_Vector3 pos, SABRE_Animator animator, unsigned int attributes, const char name[256])
{
    SABRE_Entity new;
    SABRE_ListTypes newListElement;

    new.radius = radius;
    new.pos = pos;
    new.attributes = attributes;
    new.animator = animator;

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
