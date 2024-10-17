int SABRE_ValidateSpriteIndex(int index)
{
    // 0 is reserved for the "sprite missing" sprite
    if (index > 0 && index < SABRE_spriteStore.count)
        return index;

    return 0;
}

void SABRE_UpdateSpriteShortcutPointer(void)
{
    SABRE_sprites = SABRE_DATA_STORE_AS_SPRITE_ARRAY(SABRE_spriteStore);
}

void SABRE_AddSpriteToDataStore(SABRE_DataStore *dataStore, void *sprite)
{
    SABRE_sprites[dataStore->count] = *SABRE_SPRITE_POINTER_CAST(sprite);
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

int SABRE_AutoAddSprites()
{
    int i = 1; // Start from 1, don't add project management label as a texture
    int err = 0;
    char animName[256];

    strcpy(animName, getAnimName(i));

    while (strcmp(animName, "") != 0)
    {
        err = SABRE_AddSprite(animName);

        if (err) return err;

#if DEBUG
{
    char temp[256];
    sprintf(temp, "Added sprite: [%d \"%s\" %d]", i - 2, animName, SABRE_sprites[SABRE_spriteStore.count-1].nframes);
    DEBUG_MSG_FROM(temp, "SABRE_AutoAddSprites");
}
#endif

        strcpy(animName, getAnimName(++i));
    }

    return 0;
}

void SABRE_FreeSpriteStore()
{
    SABRE_FreeDataStore(&SABRE_spriteStore);
    SABRE_sprites = NULL;
}
