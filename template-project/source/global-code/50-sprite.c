#define SABRE_SPRITE_ACTOR "SABRE_SpriteActor"

struct SABRE_SpriteStruct
{
    unsigned int width;
    unsigned int height;
    unsigned int halfWidth;
    unsigned int halfHeight;
    unsigned int sprite;
    char name[256];
};

#define SABRE_DATA_STORE_AS_SPRITE_ARRAY(DATA_STORE) SABRE_DATA_STORE_AS_CAST_ARRAY(DATA_STORE, (struct SABRE_SpriteStruct *))
#define SABRE_SPRITE_POINTER_CAST(POINTER) ((struct SABRE_SpriteStruct *)POINTER)

// Actual sprite data store
struct SABRE_DataStoreStruct SABRE_spriteStore;
// A shortcut pointer to access the data from the store
// without having to cast pointers all the time
struct SABRE_SpriteStruct *SABRE_sprites = NULL;

int SABRE_AutoAddSprites();
int SABRE_AddSprite(const char spriteName[256]);

void SABRE_AddSpriteToDataStore(struct SABRE_DataStoreStruct *dataStore, void *sprite);
void SABRE_FreeSpriteStore();

int SABRE_AutoAddSprites()
{
    int i = 0;
    int err = 0;
    char animName[256];

    strcpy(animName, getAnimName(i));
    SABRE_SetDataStoreAddFunc(&SABRE_spriteStore, SABRE_AddSpriteToDataStore);
    SABRE_spriteStore.elemSize = sizeof(struct SABRE_SpriteStruct);
    SABRE_PrepareDataStore(&SABRE_spriteStore);

    // Set the shortcut pointer to allow easier access to sprite data
    SABRE_sprites = SABRE_DATA_STORE_AS_SPRITE_ARRAY(SABRE_spriteStore);

    while (strcmp(animName, "") != 0)
    {
        err = SABRE_AddSprite(animName);

        if (err) return err;

#if DEBUG
{
    char temp[256];
    sprintf(temp, "Added sprite: [%d \"%s\"]", i, animName);
    DEBUG_MSG_FROM(temp, "SABRE_AutoAddSprites");
}
#endif

        strcpy(animName, getAnimName(++i));
    }

    return 0;
}

int SABRE_AddSprite(const char spriteName[256])
{
    struct SABRE_SpriteStruct newSprite;

    strcpy(newSprite.name, spriteName);
    newSprite.width = SABRE_GetAnimationDimensionInPixels(SABRE_SPRITE_ACTOR, spriteName, SABRE_DIMENSION_X);
    newSprite.height = SABRE_GetAnimationDimensionInPixels(SABRE_SPRITE_ACTOR, spriteName, SABRE_DIMENSION_Y);
    newSprite.halfWidth = newSprite.width * 0.5f;
    newSprite.halfHeight = newSprite.height * 0.5f;

    return SABRE_AddToDataStore(&SABRE_spriteStore, &newSprite);
}

void SABRE_AddSpriteToDataStore(struct SABRE_DataStoreStruct *dataStore, void *sprite)
{
    SABRE_sprites[dataStore->count] = *SABRE_SPRITE_POINTER_CAST(sprite);
}

void SABRE_FreeSpriteStore()
{
    SABRE_FreeDataStore(&SABRE_spriteStore);
    SABRE_sprites = NULL;
}
