#ifndef SABRE_ANIMATION_DEFINED
typedef struct SABRE_AnimationStruct
{
    float frameRate;
    unsigned int nframes;
    unsigned int sprite;
}SABRE_Animation;
#define SABRE_ANIMATION_DEFINED
#endif

#ifndef SABRE_ANIMATOR_DEFINED
typedef struct SABRE_AnimatorStruct
{
    char state;
    unsigned int animpos;
    float accumulatedAnimpos;
    SABRE_Animation anim;
}SABRE_Animator;
#define SABRE_ANIMATOR_DEFINED
#endif

#ifndef SABRE_ENTITY_DEFINED
typedef struct SABRE_EntityStruct
{
    float radius;
    SABRE_Vector3 pos;
    unsigned char attributes;
    SABRE_Animator animator;
    char name[256];
}SABRE_Entity;
#define SABRE_ENTITY_DEFINED
#endif

#ifndef SABRE_PROJECTILE_DEFINED
typedef struct SABRE_ProjectileStruct
{
    float speed;
    float dropFactor;
    SABRE_Vector3 dir;
    SABRE_Entity *entity;
}SABRE_Projectile;
#define SABRE_PROJECTILE_DEFINED
#endif

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

SABRE_List *SABRE_AddToList(SABRE_List **list, SABRE_ListTypes elem);
void SABRE_FreeList(SABRE_List *list);

SABRE_List *SABRE_AddToList(SABRE_List **list, SABRE_ListTypes elem)
{
    SABRE_List *new = malloc(sizeof *new);

    if (!new)
    {
        DEBUG_MSG_FROM("Malloc failed!", "SABRE_AddToList");
        return NULL;
    }

    new->data = elem;
    new->next = NULL;
    new->prev = NULL;

    if (list && !(*list))
    {
        new->next = NULL;
        new->prev = NULL;
        *list = new;
        return *list;
    }
    else if (!list)
    {
        DEBUG_MSG_FROM("Invalid list pointer.", "SABRE_AddToList");
        free(new);
        return NULL;
    }

    new->next = *list;
    new->prev = NULL;
    new->next->prev = new;
    *list = new;
    return *list;
}

void SABRE_RemoveFromList(SABRE_List **list, SABRE_List *remove)
{
    if (!list || !(*list) || !remove) return;

    if (remove->prev)
        remove->prev->next = remove->next;
    else
        *list = remove->next;

    if (remove->next)
        remove->next->prev = remove->prev;

    if (!remove->prev && !remove->next)
    {
        *list = NULL;
    }

    free(remove);
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
