short slice; // slice of screen to be drawn
short wallHit; // was a wall hit or not
short hitSide; // which side was hit: x-side (0) or y-side (1)

float cameraX; // x-coordinate on the camera plane (-1.0 - 1.0)
float wallHitX; // the horizontal position where the ray hit the wall (0.0-1.0)
short wallSliceHeight; // height of the wall slice to draw
float perpWallDist; // perpendicular distance from the wall (ray length)

float rayPosX, rayPosY; // ray position
float rayDirX, rayDirY; // ray direction
short rayMapX, rayMapY; // coordinates of the square the ray is in
short rayStepX, rayStepY; // ray step direction
float sideDistX, sideDistY; // distance from ray position to next x- or y-side TODO: better explanation
float deltaDistX, deltaDistY; // distance ray has to travel from one side to another

short horizontalScalingCompensation;
const float horizontalCompensationThreshold = 0.0315f;
float scale;

erase(0, 0, 0, 1);

for (slice = 0; slice < screenWidth; slice++)
{
    // calculate the position and direction of the ray
    cameraX = 2.0f * slice / (float)screenWidth - 1; // x on the camera plane
    rayPosX = player.posX; // set the begin position of the ray to the player's position
    rayPosY = player.posY;
    rayDirX = player.dirX + player.planeX * cameraX; // set the direction of the ray
    rayDirY = player.dirY + player.planeY * cameraX;

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
        perpWallDist = ((rayMapX - rayPosX + (1 - rayStepX) / 2) / rayDirX);
    else
        perpWallDist = ((rayMapY - rayPosY + (1 - rayStepY) / 2) / rayDirY);

    // calculate the height of the current line of the wall to be drawn
    wallSliceHeight = screenHeight / perpWallDist;

    // calculate the right texture to use
    player.textureNumber = map[rayMapY][rayMapX] - 1;

    // calculate where the wall was hit
    if (hitSide)
        wallHitX = rayPosX + ((rayMapY - rayPosY + (1 - rayStepY) / 2) / rayDirY) * rayDirX;
    else
        wallHitX = rayPosY + ((rayMapX - rayPosX + (1 - rayStepX) / 2) / rayDirX) * rayDirY;

    // get only the decimal part, which represents the hit position along the horizontal
    // axis of the texture
    wallHitX -= (short)wallHitX;

    // calculate which vertical slice from the texture has to be drawn
    player.textureSliceToDraw = (short)(wallHitX * textureWidth);

    // prevent textures from being drawn as mirror images
    if (!hitSide && rayDirX > 0) player.textureSliceToDraw = textureWidth - player.textureSliceToDraw - 1;
    if (hitSide && rayDirY < 0) player.textureSliceToDraw = textureWidth - player.textureSliceToDraw - 1;

    scale = wallSliceHeight / (float)textureHeight;
    horizontalScalingCompensation = (short)floor(scale - horizontalCompensationThreshold) + 1;

    SendActivationEvent("SABRE_TextureSlice");
    draw_from("SABRE_TextureSlice", slice + horizontalScalingCompensation, height * 0.5f, scale);

    slice += (short)max(floor(scale) - 1, 0) + horizontalScalingCompensation * (scale > 1.0f - horizontalCompensationThreshold);
}
