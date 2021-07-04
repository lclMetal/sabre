#define SABRE_TEXTURE_ACTOR "SABRE_TextureSlice"

#define SABRE_TEXTURE_STORE_DOUBLING_LIMIT 128
#define SABRE_TEXTURE_STORE_GROW_AMOUNT 64 // SABRE_TEXTURE_STORE_DOUBLING_LIMIT / 2

struct SABRE_TextureStruct
{
    int width;
    int height;
    short slices;
    char name[256];
};

struct SABRE_TextureStoreStruct
{
    size_t size; // the maximum amount of textures the store can hold at the moment
    size_t count; // the amount of textures the store actually holds at the moment
    struct SABRE_TextureStruct *textures;
}SABRE_textureStore;

int SABRE_InitTextureStore();
int SABRE_GrowTextureStore();
int SABRE_AutoAddTextures();
int SABRE_PrepareTextureAddition();
int SABRE_AddTexture(const char textureName[256]);
int SABRE_CalculateTextureWidth(struct SABRE_TextureStruct *texture);
int SABRE_CalculateTextureHeight(struct SABRE_TextureStruct *texture);
void SABRE_FreeTextureStore();

int SABRE_InitTextureStore()
{
    SABRE_textureStore.size = 16;
    SABRE_textureStore.count = 0;
    SABRE_textureStore.textures = calloc(SABRE_textureStore.size, sizeof *SABRE_textureStore.textures);

    if (!SABRE_textureStore.textures)
    {
        DEBUG_MSG_FROM("Memory allocation failed!", "SABRE_InitTextureStore");
        return 1;
    }

    return 0;
}

int SABRE_GrowTextureStore()
{
    struct SABRE_TextureStruct *new = NULL;

    // double the texture store size or grow it by SABRE_TEXTURE_STORE_GROW_AMOUNT
    if (SABRE_textureStore.size < SABRE_TEXTURE_STORE_DOUBLING_LIMIT) SABRE_textureStore.size *= 2;
    else SABRE_textureStore.size += SABRE_TEXTURE_STORE_GROW_AMOUNT;

    new = realloc(SABRE_textureStore.textures, SABRE_textureStore.size);

    if (!new)
    {
        DEBUG_MSG_FROM("Memory allocation failed!", "SABRE_GrowTextureStore");
        return 1;
    }

    SABRE_textureStore.textures = new;

    return 0;
}

// only works for non-animated textures
int SABRE_AutoAddTextures()
{
    int i = 0;
    int err;
    char animName[256];

    strcpy(animName, getAnimName(i));

    while (strcmp(animName, ""))
    {
        err = SABRE_AddTexture(animName);

        if (err) return err;

        #if DEBUG
        {
            char temp[256];
            sprintf(temp, "Added texture: [%d %s]", i, animName);
            DEBUG_MSG(temp);
        }
        #endif

        strcpy(animName, getAnimName(++i));
    }

    return 0;
}

int SABRE_PrepareTextureAddition()
{
    // the texture store has not been initialized, initialize it and make sure no errors occurred
    if (!SABRE_textureStore.size && SABRE_InitTextureStore() != 0)
    {
        DEBUG_MSG_FROM("Unable to initialize texture store.", "SABRE_AddTexture");
        return 1;
    }
    // the texture store is full, grow it and check make sure no errors occurred
    else if (SABRE_textureStore.count == SABRE_textureStore.size && SABRE_GrowTextureStore() != 0)
    {
        DEBUG_MSG_FROM("Unable to grow texture store.", "SABRE_AddTexture");
        return 2;
    }
    // otherwise no-op

    return 0;
}

int SABRE_AddTexture(const char textureName[256])
{
    int err;

    struct SABRE_TextureStruct *textures = NULL;
    struct SABRE_TextureStoreStruct *ts = NULL;

    if (err = SABRE_PrepareTextureAddition() != 0) return err;

    ts = &SABRE_textureStore;
    textures = ts->textures;

    strcpy(textures[ts->count].name, textureName);
    textures[ts->count].width = SABRE_CalculateTextureWidth(&textures[ts->count]);
    textures[ts->count].height = SABRE_CalculateTextureHeight(&textures[ts->count]);
    ts->count++; // new texture has been added, increment count

    return 0;
}

int SABRE_CalculateTextureWidth(struct SABRE_TextureStruct *texture)
{
    // TODO: make a check for if the animation actually exists, use getAnimIndex(), if -1, doesn't exist
    ChangeAnimation(SABRE_TEXTURE_ACTOR, texture->name, STOPPED);
    return getclone(SABRE_TEXTURE_ACTOR)->nframes;
}

int SABRE_CalculateTextureHeight(struct SABRE_TextureStruct *texture)
{
    int i;
    int textureHeight = 0;
    Actor *textureActor = getclone(SABRE_TEXTURE_ACTOR);

    // TODO: make a check for if the animation actually exists, use getAnimIndex(), if -1, doesn't exist
    ChangeAnimation(SABRE_TEXTURE_ACTOR, texture->name, STOPPED);

    for (i = 0; i < textureActor->nframes; i++)
    {
        textureActor = getclone(SABRE_TEXTURE_ACTOR); // this updates the width and height values

        if (textureActor->height > textureHeight)
        {
            textureHeight = textureActor->height;
        }

        textureActor->animpos++;
        // Send activation event to apply the animpos advancement during this frame already.
        // The normal behavior of Game Editor is to update the animpos of an actor in the next
        // frame. This same trick is used for changing the TextureSlice's animpos multiple
        // times per frame when drawing the game view. Notice, however, that if you were to
        // inspect the actor during a frame where this trick is used you still wouldn't see
        // the animpos changing more than once during a single frame. This is because Game Editor
        // only draws the actor on screen once per frame. But behind the scenes the animpos
        // still changes multiple times per frame, affecting the actor's dimensions as well as
        // its appearance if drawn using draw_from().
        SendActivationEvent(SABRE_TEXTURE_ACTOR);
    }

    return textureHeight;
}

void SABRE_FreeTextureStore()
{
    if (SABRE_textureStore.textures)
    {
        free(SABRE_textureStore.textures);
        SABRE_textureStore.size = 0;
        SABRE_textureStore.textures = NULL;
    }
}
