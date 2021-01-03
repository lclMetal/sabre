#define SABRE_TEXTURE_STORE_DOUBLING_LIMIT 128
#define SABRE_TEXTURE_STORE_GROW_AMOUNT SABRE_TEXTURE_STORE_DOUBLING_LIMIT / 2

struct SABRE_TextureStruct
{
    short width;
    short height;
    char name[256];
};

typedef struct SABRE_TextureStoreStruct
{
    size_t size; // the maximum amount of textures the store can hold at the moment
    size_t count; // the amount of textures the store actually holds at the moment
    struct SABRE_TextureStruct *textures;
}SABRE_TextureStore;

SABRE_TextureStore sabreTextureStore;

int SABRE_InitTextureStore();
int SABRE_GrowTextureStore();
int SABRE_AddTexture(short width, short height, char name[256]);
void SABRE_FreeTextureStore();

int SABRE_InitTextureStore()
{
    sabreTextureStore.size = 16;
    sabreTextureStore.count = 0;
    sabreTextureStore.textures = calloc(sabreTextureStore.size, sizeof *sabreTextureStore.textures);

    if (!sabreTextureStore.textures)
    {
        DEBUG_MSG_FROM("Memory allocation failed!", "SABRE_InitTextureStore"__FILE__);
        return 1;
    }

    return 0;
}

int SABRE_GrowTextureStore()
{
    struct SABRE_TextureStruct *new = NULL;

    // double the texture store size or grow it by SABRE_TEXTURE_STORE_GROW_AMOUNT
    if (sabreTextureStore.size < SABRE_TEXTURE_STORE_DOUBLING_LIMIT) sabreTextureStore.size *= 2;
    else sabreTextureStore.size += SABRE_TEXTURE_STORE_GROW_AMOUNT;

    new = realloc(sabreTextureStore.textures, sabreTextureStore.size);

    if (!new)
    {
        DEBUG_MSG_FROM("Memory allocation failed!", "SABRE_GrowTextureStore");
        return 1;
    }

    sabreTextureStore.textures = new;

    return 0;
}

int SABRE_AddTexture(short width, short height, char name[256])
{
    struct SABRE_TextureStruct *textures = NULL;
    SABRE_TextureStore *sabreTS = NULL;

    if (!sabreTextureStore.size) // the texture store has not been initialized
    {
        if (SABRE_InitTextureStore() != 0) // error happened
        {
            DEBUG_MSG_FROM("Unable to add texture.", "SABRE_AddTexture");
            return 1;
        }
    }
    else if (sabreTextureStore.count == sabreTextureStore.size) // the texture store is full
    {
        if (SABRE_GrowTextureStore() != 0) // error happened
        {
            DEBUG_MSG_FROM("Unable to add texture.", "SABRE_AddTexture");
            return 1;
        }
    }

    sabreTS = &sabreTextureStore;
    textures = sabreTS->textures;

    textures[sabreTS->count].width = width;
    textures[sabreTS->count].height = height;
    strcpy(textures[sabreTS->count].name, name);
    sabreTS->count++; // new texture has been added, increment count

    return 0;
}

void SABRE_FreeTextureStore()
{
    if (sabreTextureStore.textures)
    {
        free(sabreTextureStore.textures);
        sabreTextureStore.textures = NULL;
    }
}
