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
short rayStepX, rayStepY; // ray step direction
float sideDistX, sideDistY; // distance from ray position to next x- or y-side TODO: better explanation
float deltaDistX, deltaDistY; // distance ray has to travel from one side to another

float scale;
short horizontalScalingCompensation; // amount of pixels to shift the drawing position to right
                                     // to compensate for the bigger width resulting from scaling
const float horizontalCompensationThreshold = 0.0315f; // threshold for growing the compensation

struct SABRE_TextureStruct *texture = NULL;
struct SABRE_CameraStruct *camera = &SABRE_camera; // a pointer to the camera

// only the 1st clone (cloneindex 0) will execute this code, as the other ones are just going
// to inherit everything drawn on the first clone, due to how cloned canvases work in GE
if (!cloneindex)
{
    erase(0, 0, 0, 1);

    drawCalls = 0;

    for (slice = 0; slice < screenWidth; slice++)
    {
        // calculate the position and direction of the ray
        cameraX = 2.0f * slice / (float)screenWidth - 1; // x on the camera plane
        rayPosX = camera->posX; // set the begin position of the ray to the player's position
        rayPosY = camera->posY;
        rayDirX = camera->dirX + camera->planeX * cameraX; // set the direction of the ray
        rayDirY = camera->dirY + camera->planeY * cameraX;

        // set the square the ray starts from
        rayMapX = (short)rayPosX;
        rayMapY = (short)rayPosY;

        // distance ray has to travel from one side to another
        deltaDistX = (rayDirY == 0) ? 0 : ((rayDirX == 0) ? 1 : abs(1.0f / rayDirX));
        deltaDistY = (rayDirX == 0) ? 0 : ((rayDirY == 0) ? 1 : abs(1.0f / rayDirY));

        wallHit = 0;
        hitSide = 0;

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

        // perform Digital Differential Analysis (DDA) for finding the wall
        while (!wallHit) // loop until a wall has been hit
        {
            // step to next map square in x- or y-direction, depending on which one is closer
            if (sideDistX < sideDistY)
            {
                sideDistX += deltaDistX;
                rayMapX += rayStepX;
                hitSide = 0;
            }
            else
            {
                sideDistY += deltaDistY;
                rayMapY += rayStepY;
                hitSide = 1;
            }

            // check if a wall has been hit
            if (map[rayMapY][rayMapX] > 0) wallHit = 1;
        }

        // calculate the perpendicular distance between the wall and the camera plane
        if (!hitSide)
            perpWallDist = ((rayMapX - rayPosX + (1 - rayStepX) / 2.0f) / rayDirX);
        else
            perpWallDist = ((rayMapY - rayPosY + (1 - rayStepY) / 2.0f) / rayDirY);

        // calculate the height of the current line of the wall to be drawn
        wallSliceHeight = screenHeight / perpWallDist;

        // calculate the right texture to use
        SABRE_slice.anim = map[rayMapY][rayMapX] - 1;
        texture = &(SABRE_textureStore.textures[SABRE_slice.anim]);

        // calculate where the wall was hit
        if (hitSide)
            wallHitX = rayPosX + ((rayMapY - rayPosY + (1 - rayStepY) / 2.0f) / rayDirY) * rayDirX;
        else
            wallHitX = rayPosY + ((rayMapX - rayPosX + (1 - rayStepX) / 2.0f) / rayDirX) * rayDirY;

        // get only the decimal part, which represents the hit position along the horizontal
        // axis of the texture
        wallHitX -= (short)wallHitX;

        // calculate which vertical slice from the texture has to be drawn
        SABRE_slice.slice = (short)(wallHitX * texture->width);

        // prevent textures from being drawn as mirror images
        if (!hitSide && rayDirX < 0) SABRE_slice.slice = texture->width - SABRE_slice.slice - 1;
        if (hitSide && rayDirY > 0) SABRE_slice.slice = texture->width - SABRE_slice.slice - 1;

        scale = wallSliceHeight / (float)texture->height;
        horizontalScalingCompensation = (short)floor(scale - horizontalCompensationThreshold) + 1;

        SendActivationEvent("SABRE_TextureSlice");
        draw_from("SABRE_TextureSlice", slice + horizontalScalingCompensation, height * 0.5f, scale);

        slice += (short)max(floor(scale) - 1, 0) + horizontalScalingCompensation * (scale > 1.0f - (horizontalCompensationThreshold - 0.0001f));
        drawCalls++;
    }
}
