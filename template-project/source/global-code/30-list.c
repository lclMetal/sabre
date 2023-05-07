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
