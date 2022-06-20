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

#define SABRE_GET_SPRITE(DATA_STORE, INDEX) ((struct SABRE_SpriteStruct *)(DATA_STORE)->elems)[INDEX]

struct SABRE_DataStoreStruct SABRE_spriteStore;

int SABRE_AutoAddSprites();
int SABRE_AddSprite(const char spriteName[256]);

void SABRE_AddSpriteToDataStore(struct SABRE_DataStoreStruct *dataStore, void *sprite);

int SABRE_AutoAddSprites()
{
    int i = 0;
    int err = 0;
    char animName[256];

    strcpy(animName, getAnimName(i));
    SABRE_SetDataStoreAddFunc(&SABRE_spriteStore, SABRE_AddSpriteToDataStore);
    SABRE_spriteStore.elemSize = sizeof(struct SABRE_SpriteStruct);

    while (strcmp(animName, "") != 0)
    {
        err = SABRE_AddSprite(animName);

        if (err) return err;

#if DEBUG
{
    char temp[256];
    sprintf(temp, "Added sprite: [%d \"%s\"]", i, animName);
    DEBUG_MSG(temp);
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
    SABRE_GET_SPRITE(dataStore, dataStore->count) = (*(struct SABRE_SpriteStruct *)sprite);
}
