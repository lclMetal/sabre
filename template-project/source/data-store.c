#define SABRE_DATA_STORE_DOUBLING_LIMIT 128
#define SABRE_DATA_STORE_GROW_AMOUNT 64 // SABRE_DATA_STORE_DOUBLING_LIMIT / 2

#define SABRE_CAST_TEXTURE_PTR(PTR) (struct SABRE_TextureStruct *)PTR
#define SABRE_DATA_STORE_GET_ELEMS(PTR, TYPE) ((TYPE)PTR)
#define SABRE_DATA_STORE_GET_TEXTURE_ELEMS(PTR) SABRE_DATA_STORE_GET_ELEMS(PTR, struct SABRE_TextureStruct *)

struct SABRE_DataStoreStruct
{
    size_t capacity; // the maximum amount of elements the store can hold at the moment
    size_t count; // the amount of elements the store actually holds at the moment
    size_t elemSize; // the size of a single element in the store
    void *elems; // pointer to the elements
    void (*addFunc)(struct SABRE_DataStoreStruct*, void*);
};

void SABRE_AddTextureToDataStore(struct SABRE_DataStoreStruct *dataStore, void *texture);

void SABRE_SetDataStoreAddFunc(struct SABRE_DataStoreStruct *dataStore, void (*addDataFunc)(struct SABRE_DataStoreStruct*, void*));
int SABRE_InitDataStore(struct SABRE_DataStoreStruct *dataStore, size_t elemSize);
int SABRE_GrowDataStore(struct SABRE_DataStoreStruct *dataStore);
int SABRE_PrepareDataStore(struct SABRE_DataStoreStruct *dataStore);
int SABRE_AddToDataStore(struct SABRE_DataStoreStruct *dataStore, void *elem);
void SABRE_FreeDataStore(struct SABRE_DataStoreStruct *dataStore);

void SABRE_AddTextureToDataStore(struct SABRE_DataStoreStruct *dataStore, void *texture)
{
    SABRE_DATA_STORE_GET_TEXTURE_ELEMS(dataStore->elems)[dataStore->count] = (*SABRE_CAST_TEXTURE_PTR(texture));
}

void SABRE_SetDataStoreAddFunc(struct SABRE_DataStoreStruct *dataStore, void (*addDataFunc)(struct SABRE_DataStoreStruct*, void*))
{
    dataStore->addFunc = addDataFunc;
}

int SABRE_InitDataStore(struct SABRE_DataStoreStruct *dataStore, size_t elemSize)
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

int SABRE_GrowDataStore(struct SABRE_DataStoreStruct *dataStore)
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

int SABRE_PrepareDataStore(struct SABRE_DataStoreStruct *dataStore)
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

int SABRE_AddToDataStore(struct SABRE_DataStoreStruct *dataStore, void *elem)
{
    int err = 0;

    err = SABRE_PrepareDataStore(dataStore);

    if (err != 0) return err;

    dataStore->addFunc(dataStore, elem);
    dataStore->count++; // new element has been added, increment count

    return 0;
}

void SABRE_FreeDataStore(struct SABRE_DataStoreStruct *dataStore)
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
