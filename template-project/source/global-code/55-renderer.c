const float SABRE_heightUnit = 480.0f;
const float SABRE_halfHeightUnit = SABRE_heightUnit * 0.5f;
float SABRE_screenWidth = 640.0f, SABRE_screenHeight = 480.0f;

void SABRE_InitializeFrame()
{
    int i, j, k;
    size_t index = 0;

    for (j = 0; j < SABRE_level.height; j++)
    {
        for (i = 0; i < SABRE_level.width; i++)
        {
            index = j * SABRE_level.width + i;

            for (k = 0; k <= SABRE_MAX_LEVEL_EDGE_WRAP_DEPTH; k++)
            {
                SABRE_level.map[index].renderObject[k] = NULL;
            }
        }
    }

#if DEBUG
    traversals = 0;
    maxTraversals = 0;
#endif

    SABRE_ROListManager.reusableCache = SABRE_ConcatenateROList(SABRE_ROListManager.reusableCache, SABRE_ROListManager.head);
    SABRE_ROListManager.head = NULL;
    SABRE_ROListManager.curr = NULL;
}

void SABRE_RenderObjects()
{
    int horizontalPosition = 0;
    float verticalPosition = height * 0.5f;
    SABRE_RenderObject *iterator = NULL;
    float verticalResolutionFactor = SABRE_screenHeight / SABRE_heightUnit;
    const float horizontalCompensationThreshold = 0.0315f; // threshold for growing the compensation

    textureDrawCalls = 0;
    spriteDrawCalls = 0;

    for (iterator = SABRE_ROListManager.head; iterator != NULL; iterator = iterator->next)
    {
        if (iterator->objectType == SABRE_TEXTURE_RO)
        {
            SABRE_slice.anim = iterator->slice.anim;
            SABRE_slice.slice = iterator->slice.slice;
            SendActivationEvent(SABRE_TEXTURE_ACTOR);
            draw_from(SABRE_TEXTURE_ACTOR, iterator->horizontalPosition + iterator->horizontalScalingCompensation, verticalPosition - SABRE_camera.vPos * (iterator->height / SABRE_halfHeightUnit),
                iterator->scale);
            textureDrawCalls++;
        }
        else if (iterator->objectType == SABRE_SPRITE_RO)
        {
            SABRE_slice.anim = iterator->slice.anim;
            SABRE_slice.slice = iterator->slice.slice;
            SendActivationEvent(SABRE_SPRITE_ACTOR);
            draw_from(SABRE_SPRITE_ACTOR, iterator->horizontalPosition,
                verticalPosition + ((SABRE_halfHeightUnit - (float)SABRE_sprites[SABRE_slice.anim - 1].halfHeight - SABRE_camera.vPos * 2) - (iterator->verticalOffset * SABRE_heightUnit)) * iterator->scale * verticalResolutionFactor,
                iterator->scale * verticalResolutionFactor);
            spriteDrawCalls++;
        }
    }
}
