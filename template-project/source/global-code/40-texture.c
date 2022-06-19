#define SABRE_TEXTURE_ACTOR "SABRE_TextureActor"

struct SABRE_TextureStruct
{
    int width;
    int height;
    short slices;
    char isWindow;
    char name[256];
};

#define SABRE_GET_TEXTURE(DATA_STORE, INDEX) ((struct SABRE_TextureStruct *)(DATA_STORE)->elems)[INDEX]

struct SABRE_DataStoreStruct SABRE_textureStore;

int SABRE_AutoAddTextures();
int SABRE_AddTexture(const char textureName[256]);
int SABRE_CalculateTextureWidth(struct SABRE_TextureStruct *texture);
int SABRE_CalculateTextureHeight(struct SABRE_TextureStruct *texture);

void SABRE_AddTextureToDataStore(struct SABRE_DataStoreStruct *dataStore, void *texture);

// only works for non-animated textures
int SABRE_AutoAddTextures()
{
    int i = 0;
    int err = 0;
    char animName[256];

    strcpy(animName, getAnimName(i));
    SABRE_SetDataStoreAddFunc(&SABRE_textureStore, SABRE_AddTextureToDataStore);
    SABRE_textureStore.elemSize = sizeof(struct SABRE_TextureStruct);

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

void SABRE_AddTextureToDataStore(struct SABRE_DataStoreStruct *dataStore, void *texture)
{
    SABRE_GET_TEXTURE(dataStore, dataStore->count) = (*(struct SABRE_TextureStruct *)texture);
}
