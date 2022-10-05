short slice; // slice of screen to be drawn
short wallHit; // was a wall hit or not
short hitSide; // which side was hit: x-side (0) or y-side (1)

float cameraX; // x-coordinate on the camera plane (-1.0 - 1.0)
float wallHitX; // the horizontal position where the ray hit the wall (0.0-1.0)
float wallSliceHeight; // height of the wall slice to draw
float perpWallDist; // perpendicular distance from the wall (ray length)

float rayPosX, rayPosY; // ray position
float rayDirX, rayDirY; // ray direction
short rayMapX, rayMapY; // coordinates of the square the ray is in
short unwrappedRayMapX, unwrappedRayMapY;
short rayStepX, rayStepY; // ray step direction
float sideDistX, sideDistY; // distance from ray position to next x- or y-side TODO: better explanation
float deltaDistX, deltaDistY; // distance ray has to travel from one side to another

float scale;
float scaledHalfWidth;
short horizontalScalingCompensation = 0; // amount of pixels to shift the drawing position to right
                                         // to compensate for the bigger width resulting from scaling
const float horizontalCompensationThreshold = 0.0315f; // threshold for growing the compensation

SABRE_Texture *texture = NULL;
SABRE_Entity *entity = NULL;
SABRE_Sprite *sprite = NULL;
SABRE_Camera *camera = &SABRE_camera; // a pointer to the camera

float spriteX, spriteY;
float invDet;
float transformX, transformY;
float spriteScreenX;

size_t rayMapIndex = 0;
size_t newRayMapIndex = 0;
SABRE_LevelTile *tile = NULL;

int sliceWidth = 0;
char willRenderAWindow = 0;
char windowCount = 0;
char wrapCount = 0;
char rayIsInsideWindow = 0;
short prevWindowTexture = 0;

char solidWallHit = 0;
char levelEdgeHit = 0;

SABRE_List *iterator = NULL; // pointer to the entity to process

// only the 1st clone (cloneindex 0) will execute this code, as the other ones are just going
// to inherit everything drawn on the first clone, due to how cloned canvases work in GE
if (!cloneindex && SABRE_gameState == SABRE_RUNNING)
{
    erase(0, 0, 0, 1);
    SABRE_InitializeFrame();

    invDet = 1.0f / (float)(camera->plane.x * camera->dir.y - camera->dir.x * camera->plane.y);

    for (iterator = SABRE_entities; iterator != NULL; iterator = iterator->next)
    {
        // Skip to next entity if current one is hidden
        if (iterator->data.entity.attributes & SABRE_ENTITY_HIDDEN)
            continue;

        entity = &iterator->data.entity;
        sprite = &SABRE_sprites[entity->animator.anim.sprite];
        spriteX = entity->pos.x - camera->pos.x;
        spriteY = entity->pos.y - camera->pos.y;

        transformX = invDet * (camera->dir.y * spriteX - camera->dir.x * spriteY);
        transformY = invDet * (-camera->plane.y * spriteX + camera->plane.x * spriteY);

        if (transformY < 0.05f)
        {
            transformY = -1;
        }

        spriteScreenX = (SABRE_screenWidth / 2.0f) * (1 + transformX / transformY);
        scale = 1.0f / transformY;
        scaledHalfWidth = sprite->halfWidth * scale;

        if (transformY > 0 && spriteScreenX > 0 - scaledHalfWidth && spriteScreenX < SABRE_screenWidth + scaledHalfWidth)
        {
            // +1 to compensate for the existence of the project management label
            SABRE_slice.anim = entity->animator.anim.sprite + 1;

            if (entity->animator.anim.nframes > 1)
            {
                SABRE_UpdateAnimation(&entity->animator);
                SABRE_slice.slice = entity->animator.animpos;
            }
            else
            {
                SABRE_slice.slice = 0;
            }

            SABRE_AddSpriteRO(transformY, scale, spriteScreenX, entity->pos.z, SABRE_slice);
        }
    }

    for (slice = 0; slice < SABRE_screenWidth; slice++)
    {
        // calculate the position and direction of the ray
        cameraX = 2.0f * (float)slice / (float)SABRE_screenWidth - 1; // x on the camera plane
        rayPosX = camera->pos.x; // set the begin position of the ray to the player's position
        rayPosY = camera->pos.y;
        rayDirX = camera->dir.x + camera->plane.x * cameraX; // set the direction of the ray
        rayDirY = camera->dir.y + camera->plane.y * cameraX;

        // set the square the ray starts from
        rayMapX = (short)rayPosX;
        rayMapY = (short)rayPosY;
        unwrappedRayMapX = rayMapX;
        unwrappedRayMapY = rayMapY;

        // distance ray has to travel from one side to another
        deltaDistX = (rayDirX == 0) ? 1e30 : abs(1 / rayDirX);
        deltaDistY = (rayDirY == 0) ? 1e30 : abs(1 / rayDirY);

        wallHit = 0;
        hitSide = 0;
        solidWallHit = 0;
        levelEdgeHit = 0;
        windowCount = 0;
        wrapCount = 0;
        willRenderAWindow = 0;
        rayIsInsideWindow = 0;
        prevWindowTexture = 0;
        texture = NULL;
        horizontalScalingCompensation = 0;

        // calculate the step and the initial sideDistX and sideDistY
        if (rayDirX < 0) // if the ray is moving left
        {
            rayStepX = -1;
            sideDistX = (rayPosX - rayMapX) * deltaDistX;
        }
        else // if the ray is moving right insted
        {
            rayStepX = 1;
            sideDistX = (rayMapX + 1 - rayPosX) * deltaDistX;
        }
        if (rayDirY < 0) // if the ray is moving up
        {
            rayStepY = -1;
            sideDistY = (rayPosY - rayMapY) * deltaDistY;
        }
        else // if the ray is moving down instead
        {
            rayStepY = 1;
            sideDistY = (rayMapY + 1 - rayPosY) * deltaDistY;
        }

        while (!solidWallHit)
        {
            // perform Digital Differential Analysis (DDA) for finding the wall
            while (!wallHit) // loop until a wall has been hit
            {
                // step to next map square in x- or y-direction, depending on which one is closer
                if (sideDistX < sideDistY)
                {
                    sideDistX += deltaDistX;
                    rayMapX += rayStepX;
                    unwrappedRayMapX += rayStepX;
                    hitSide = 0;
                }
                else
                {
                    sideDistY += deltaDistY;
                    rayMapY += rayStepY;
                    unwrappedRayMapY += rayStepY;
                    hitSide = 1;
                }

                rayMapIndex = rayMapY * SABRE_level.width + rayMapX;

                // level edge wrap
                if (SABRE_graphicsSettings.levelEdgeMode == SABRE_EDGE_MODE_WRAP)
                {
                    rayMapY = SABRE_WrapIntValue(rayMapY, SABRE_level.height);
                    rayMapX = SABRE_WrapIntValue(rayMapX, SABRE_level.width);
                    newRayMapIndex = rayMapY * SABRE_level.width + rayMapX;

                    // check if a wrap happened
                    if (newRayMapIndex != rayMapIndex)
                        wrapCount++;

                    rayMapIndex = newRayMapIndex;
                    tile = &SABRE_level.map[rayMapIndex];

                    // limit wrap count to avoid infinite loop
                    if (wrapCount > SABRE_graphicsSettings.levelEdgeWrapDepth)
                    {
                        wallHit = 1;
                        levelEdgeHit = 3;
                        break;
                    }
                }
                else
                {
                    // check if raycast reached the level edge
                    if (rayMapY > SABRE_level.height - 2 || rayMapY < 1 ||
                        rayMapX > SABRE_level.width  - 2 || rayMapX < 1)
                    {
                        levelEdgeHit = 2;
                        tile = &SABRE_level.map[rayMapIndex];
                        break;
                    }
                }

                tile = &SABRE_level.map[rayMapIndex];

                willRenderAWindow = 0;

                // check if a wall has been hit
                if (tile->texture > 0)
                {
                    if (tile->properties & SABRE_IS_WINDOW)
                    {
                        if (rayIsInsideWindow || (SABRE_graphicsSettings.windowRenderDepth && windowCount >= SABRE_graphicsSettings.windowRenderDepth)) // Already a window, no need to render anything
                        {
                            prevWindowTexture = tile->texture;
                            continue;
                        }
                        else
                        {
                            windowCount++;
                            rayIsInsideWindow = 1;
                            wallHit = 1;

                            willRenderAWindow = 1;
                        }
                    }
                    else
                    {
                        rayIsInsideWindow = 0;
                        wallHit = 1;
                    }
                }
                else if (rayIsInsideWindow && (SABRE_graphicsSettings.windowRenderDepth == 0 || windowCount < SABRE_graphicsSettings.windowRenderDepth))
                {
                    windowCount++;
                    rayIsInsideWindow = 0;
                    wallHit = 1;

                    willRenderAWindow = 1;
                }

                // calculate the perpendicular distance between the wall and the camera plane
                if (!hitSide)
                    perpWallDist = (sideDistX - deltaDistX);
                else
                    perpWallDist = (sideDistY - deltaDistY);

                if (wallHit && willRenderAWindow && tile->renderObject[wrapCount] && abs(perpWallDist - tile->renderObject[wrapCount]->sortValue) < 1 && slice < (tile->renderObject[wrapCount]->horizontalPosition + tile->renderObject[wrapCount]->width + 1))
                {
                    wallHit = 0;
                    prevWindowTexture = tile->texture;
                }
            }

            if (levelEdgeHit && tile->texture == 0 && SABRE_graphicsSettings.levelEdgeMode == SABRE_EDGE_MODE_NO_RENDER ||
                levelEdgeHit == 3 && SABRE_graphicsSettings.levelEdgeMode == SABRE_EDGE_MODE_WRAP)
            {
                break;
            }

            // calculate the perpendicular distance between the wall and the camera plane
            if (!hitSide)
                perpWallDist = (sideDistX - deltaDistX);
            else
                perpWallDist = (sideDistY - deltaDistY);

            // calculate the height of the current line of the wall to be drawn
            wallSliceHeight = (float)SABRE_screenHeight / (float)perpWallDist;

            // calculate the right texture to use
            SABRE_slice.anim = tile->texture;
            if (SABRE_slice.anim == 0)
            {
                if (prevWindowTexture)
                    SABRE_slice.anim = prevWindowTexture;
                if (levelEdgeHit)
                    SABRE_slice.anim = SABRE_graphicsSettings.levelEdgeTextureIndex;
            }

            texture = &SABRE_textures[SABRE_slice.anim - 1];

            // calculate where the wall was hit
            if (hitSide)
                wallHitX = rayPosX + ((unwrappedRayMapY - rayPosY + (1 - rayStepY) / 2.0f) / rayDirY) * rayDirX;
            else
                wallHitX = rayPosY + ((unwrappedRayMapX - rayPosX + (1 - rayStepX) / 2.0f) / rayDirX) * rayDirY;

            // get only the decimal part, which represents the hit position along the horizontal
            // axis of the texture
            wallHitX -= (short)wallHitX;
            wallHitX = (wallHitX < 0) ? wallHitX + 1 : wallHitX;

            // calculate which vertical slice from the texture has to be drawn
            SABRE_slice.slice = (short)(wallHitX * texture->width);

            // prevent textures from being drawn as mirror images
            if (!hitSide && rayDirX < 0) SABRE_slice.slice = texture->width - SABRE_slice.slice - 1;
            if (hitSide && rayDirY > 0) SABRE_slice.slice = texture->width - SABRE_slice.slice - 1;

            scale = wallSliceHeight / (float)texture->height;
            horizontalScalingCompensation = (short)floor(scale - horizontalCompensationThreshold) + 1;

            // If a part of this wall has already been drawn this frame, set the render object list manager's
            // curr pointer to point to that render object in the list, as the next slice will be somewhere
            // very near to that in depth
            if (tile->renderObject[wrapCount])
            {
                SABRE_ROListManager.curr = tile->renderObject[wrapCount];
            }

            sliceWidth = (short)max(floor(scale) - 1, 0) + horizontalScalingCompensation * (scale > 1.0f - (horizontalCompensationThreshold - 0.0001f));

            SABRE_AddTextureRO(perpWallDist, scale, sliceWidth, wallSliceHeight, slice, horizontalScalingCompensation, SABRE_slice);

            // Set the last used render object pointer for this wall
            tile->renderObject[wrapCount] = SABRE_ROListManager.curr;

            if (!texture->isWindow || levelEdgeHit == 2)
            {
                solidWallHit = 1;
            }
            else if (rayIsInsideWindow)
            {
                prevWindowTexture = SABRE_slice.anim;
            }

            if (levelEdgeHit)
                break;

            wallHit = 0;
        }

        if (!windowCount)
            slice += (short)max(floor(scale) - 1, 0) + horizontalScalingCompensation * (scale > 1.0f - (horizontalCompensationThreshold - 0.0001f));
    }

    SABRE_RenderObjects();
}
