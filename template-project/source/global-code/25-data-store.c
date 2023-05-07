void SABRE_SetDataStoreAddFunc(SABRE_DataStore *dataStore, void (*addDataFunc)(SABRE_DataStore*, void*))
{
    dataStore->addFunc = addDataFunc;
}

int SABRE_InitDataStore(SABRE_DataStore *dataStore, size_t elemSize)
{
    dataStore->capacity = 16;
    dataStore->count = 0;
    dataStore->elemSize = elemSize;
    dataStore->elems = calloc(dataStore->capacity, dataStore->elemSize);

    if (!dataStore->elems)
    {
        DEBUG_MSG_FROM("Memory allocation failed!", "SABRE_InitDataStore");
        return 1;
    }

    return 0;
}

int SABRE_GrowDataStore(SABRE_DataStore *dataStore)
{
    void *newElems = NULL;

    // double the data store size or grow it by SABRE_DATA_STORE_GROW_AMOUNT
    if (dataStore->capacity < SABRE_DATA_STORE_DOUBLING_LIMIT) dataStore->capacity *= 2;
    else dataStore->capacity += SABRE_DATA_STORE_GROW_AMOUNT;

    newElems = realloc(dataStore->elems, dataStore->capacity);

    if (!newElems)
    {
        DEBUG_MSG_FROM("Memory allocation failed!", "SABRE_GrowDataStore");
        return 1;
    }

    dataStore->elems = newElems;

    return 0;
}

int SABRE_PrepareDataStore(SABRE_DataStore *dataStore)
{
    // the data store has not been initialized, initialize it and make sure no errors occurred
    if (!dataStore->capacity && SABRE_InitDataStore(dataStore, dataStore->elemSize) != 0)
    {
        DEBUG_MSG_FROM("Unable to initialize data store.", "SABRE_PrepareDataStore");
        return 1;
    }
    // the data store is full, grow it and make sure no errors occurred
    else if (dataStore->count == dataStore->capacity && SABRE_GrowDataStore(dataStore) != 0)
    {
        DEBUG_MSG_FROM("Unable to grow data store.", "SABRE_PrepareDataStore");
        return 2;
    }
    // otherwise no-op

    return 0;
}

int SABRE_AddToDataStore(SABRE_DataStore *dataStore, void *elem)
{
    int err = 0;

    err = SABRE_PrepareDataStore(dataStore);

    if (err != 0) return err;

    dataStore->addFunc(dataStore, elem);
    dataStore->count++; // new element has been added, increment count

    return 0;
}

void SABRE_FreeDataStore(SABRE_DataStore *dataStore)
{
    if (dataStore->elems)
    {
        free(dataStore->elems);
        dataStore->capacity = 0;
        dataStore->count = 0;
        dataStore->elemSize = 0;
        dataStore->elems = NULL;
    }
}
