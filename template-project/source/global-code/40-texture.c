#define SABRE_TEXTURE_ACTOR "SABRE_TextureActor"

struct SABRE_TextureStruct
{
    int width;
    int height;
    short slices;
    char isWindow;
    char name[256];
};

#define SABRE_DATA_STORE_AS_TEXTURE_ARRAY(DATA_STORE) SABRE_DATA_STORE_AS_CAST_ARRAY(DATA_STORE, (struct SABRE_TextureStruct *))
#define SABRE_TEXTURE_POINTER_CAST(POINTER) ((struct SABRE_TextureStruct *)POINTER)

// Actual texture data store
struct SABRE_DataStoreStruct SABRE_textureStore;
// A shortcut pointer to access the data from the store
// without having to cast pointers all the time
struct SABRE_TextureStruct *SABRE_textures = NULL;

int SABRE_AutoAddTextures();
int SABRE_AddTexture(const char textureName[256]);
int SABRE_CalculateTextureWidth(struct SABRE_TextureStruct *texture);
int SABRE_CalculateTextureHeight(struct SABRE_TextureStruct *texture);

void SABRE_AddTextureToDataStore(struct SABRE_DataStoreStruct *dataStore, void *texture);
void SABRE_FreeTextureStore();

// only works for non-animated textures
int SABRE_AutoAddTextures()
{
    int i = 1; // Start from 1, don't add project management label as a texture
    int err = 0;
    char animName[256];

    strcpy(animName, getAnimName(i));
    SABRE_SetDataStoreAddFunc(&SABRE_textureStore, SABRE_AddTextureToDataStore);
    SABRE_textureStore.elemSize = sizeof(struct SABRE_TextureStruct);
    SABRE_PrepareDataStore(&SABRE_textureStore);

    // Set the shortcut pointer to allow easier access to texture data
    SABRE_textures = SABRE_DATA_STORE_AS_TEXTURE_ARRAY(SABRE_textureStore);

    while (strcmp(animName, "") != 0)
    {
        err = SABRE_AddTexture(animName);

        if (err) return err;

#if DEBUG
{
    char temp[256];
    sprintf(temp, "Added texture: [%d \"%s\"]", i - 1, animName);
    DEBUG_MSG_FROM(temp, "SABRE_AutoAddTextures");
}
#endif

        strcpy(animName, getAnimName(++i));
    }

    return 0;
}

int SABRE_AddTexture(const char textureName[256])
{
    struct SABRE_TextureStruct newTexture;

    strcpy(newTexture.name, textureName);
    newTexture.width = SABRE_CalculateTextureWidth(&newTexture);
    newTexture.height = SABRE_CalculateTextureHeight(&newTexture);
    newTexture.isWindow = SABRE_StringEndsWith(newTexture.name, "-window");

    return SABRE_AddToDataStore(&SABRE_textureStore, &newTexture);
}

int SABRE_CalculateTextureWidth(struct SABRE_TextureStruct *texture)
{
    // TODO: make a check for if the animation actually exists, use getAnimIndex(), if -1, doesn't exist
    ChangeAnimation(SABRE_TEXTURE_ACTOR, texture->name, STOPPED);
    return getclone(SABRE_TEXTURE_ACTOR)->nframes;
}

int SABRE_CalculateTextureHeight(struct SABRE_TextureStruct *texture)
{
    return SABRE_GetAnimationDimensionInPixels(SABRE_TEXTURE_ACTOR, texture->name, SABRE_DIMENSION_Y);
}

void SABRE_AddTextureToDataStore(struct SABRE_DataStoreStruct *dataStore, void *texture)
{
    SABRE_textures[dataStore->count] = *SABRE_TEXTURE_POINTER_CAST(texture);
}

void SABRE_FreeTextureStore()
{
    SABRE_FreeDataStore(&SABRE_textureStore);
    SABRE_textures = NULL;
}
