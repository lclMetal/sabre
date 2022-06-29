#ifndef SABRE_ENTITY_DEFINED
typedef struct SABRE_EntityStruct
{
    float radius;
    SABRE_Vector2 pos;
    unsigned int sprite;
    unsigned char attributes;
    char name[256];
}SABRE_Entity;
#define SABRE_ENTITY_DEFINED
#endif

typedef union SABRE_ListTypesUnion
{
    struct SABRE_EntityStruct entity;
}SABRE_ListTypes;

typedef struct SABRE_ListStruct
{
    struct SABRE_ListStruct *next;
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

    if (list && !(*list))
    {
        new->next = NULL;
        *list = new;
        return *list;
    }

    new->next = *list;
    *list = new;
    return *list;
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
