#define SABRE_SPRITE_ACTOR "SABRE_SpriteActor"

typedef struct SABRE_SpriteStruct
{
    unsigned int width;
    unsigned int height;
    unsigned int halfWidth;
    unsigned int halfHeight;
    unsigned int nframes;
    char name[256];
}SABRE_Sprite;

#define SABRE_DATA_STORE_AS_SPRITE_ARRAY(DATA_STORE) SABRE_DATA_STORE_AS_CAST_ARRAY(DATA_STORE, (SABRE_Sprite *))
#define SABRE_SPRITE_POINTER_CAST(POINTER) ((SABRE_Sprite *)POINTER)

// Actual sprite data store
SABRE_DataStore SABRE_spriteStore;
// A shortcut pointer to access the data from the store
// without having to cast pointers all the time
SABRE_Sprite *SABRE_sprites = NULL;

int SABRE_AutoAddSprites();
int SABRE_AddSprite(const char spriteName[256]);

void SABRE_AddSpriteToDataStore(SABRE_DataStore *dataStore, void *sprite);
void SABRE_FreeSpriteStore();

int SABRE_AutoAddSprites()
{
    int i = 1; // Start from 1, don't add project management label as a texture
    int err = 0;
    char animName[256];

    strcpy(animName, getAnimName(i));
    SABRE_SetDataStoreAddFunc(&SABRE_spriteStore, SABRE_AddSpriteToDataStore);
    SABRE_spriteStore.elemSize = sizeof(SABRE_Sprite);
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
    sprintf(temp, "Added sprite: [%d \"%s\" %d]", i - 1, animName, SABRE_sprites[SABRE_spriteStore.count-1].nframes);
    DEBUG_MSG_FROM(temp, "SABRE_AutoAddSprites");
}
#endif

        strcpy(animName, getAnimName(++i));
    }

    return 0;
}

int SABRE_AddSprite(const char spriteName[256])
{
    SABRE_Sprite newSprite;

    strcpy(newSprite.name, spriteName);
    newSprite.width = SABRE_GetAnimationDataValue(SABRE_SPRITE_ACTOR, spriteName, SABRE_ANIM_WIDTH);
    newSprite.height = SABRE_GetAnimationDataValue(SABRE_SPRITE_ACTOR, spriteName, SABRE_ANIM_HEIGHT);
    newSprite.halfWidth = newSprite.width * 0.5f;
    newSprite.halfHeight = newSprite.height * 0.5f;
    newSprite.nframes = SABRE_GetAnimationDataValue(SABRE_SPRITE_ACTOR, spriteName, SABRE_ANIM_NFRAMES);

    return SABRE_AddToDataStore(&SABRE_spriteStore, &newSprite);
}

void SABRE_AddSpriteToDataStore(SABRE_DataStore *dataStore, void *sprite)
{
    SABRE_sprites[dataStore->count] = *SABRE_SPRITE_POINTER_CAST(sprite);
}

void SABRE_FreeSpriteStore()
{
    SABRE_FreeDataStore(&SABRE_spriteStore);
    SABRE_sprites = NULL;
}
