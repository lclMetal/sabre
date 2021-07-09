char *keyboard = GetKeyState();
struct SABRE_KeybindStruct *keys = &SABRE_keys; // a pointer to the player key binds
struct SABRE_CameraStruct *camera = &SABRE_camera; // a pointer to the camera

float oldDirX = camera->dirX;
float oldPlaneX = camera->planeX;
float rotateSpeed = SABRE_player.turnSpeed;
float moveSpeed = SABRE_player.moveSpeed;

if (keyboard[keys->forward] && !keyboard[keys->backward])
{
    camera->posX += camera->dirX * moveSpeed;
    camera->posY += camera->dirY * moveSpeed;
}
else if (keyboard[keys->backward] && !keyboard[keys->forward])
{
    camera->posX -= camera->dirX * moveSpeed;
    camera->posY -= camera->dirY * moveSpeed;
}

if (keyboard[keys->turnLeft] && !keyboard[keys->turnRight])
{
    camera->dirX = camera->dirX * cos(-rotateSpeed) - camera->dirY * sin(-rotateSpeed);
    camera->dirY = oldDirX * sin(-rotateSpeed) + camera->dirY * cos(-rotateSpeed);
    camera->planeX = camera->planeX * cos(-rotateSpeed) - camera->planeY * sin(-rotateSpeed);
    camera->planeY = oldPlaneX * sin(-rotateSpeed) + camera->planeY * cos(-rotateSpeed);
}
else if (keyboard[keys->turnRight] && !keyboard[keys->turnLeft])
{
    camera->dirX = camera->dirX * cos(rotateSpeed) - camera->dirY * sin(rotateSpeed);
    camera->dirY = oldDirX * sin(rotateSpeed) + camera->dirY * cos(rotateSpeed);
    camera->planeX = camera->planeX * cos(rotateSpeed) - camera->planeY * sin(rotateSpeed);
    camera->planeY = oldPlaneX * sin(rotateSpeed) + camera->planeY * cos(rotateSpeed);
}

if (keyboard[keys->strafeLeft] && !keyboard[keys->strafeRight])
{
    camera->posX -= camera->planeX * moveSpeed;
    camera->posY -= camera->planeY * moveSpeed;
}
else if (keyboard[keys->strafeRight] && !keyboard[keys->strafeLeft])
{
    camera->posX += camera->planeX * moveSpeed;
    camera->posY += camera->planeY * moveSpeed;
}

{
    float posX = camera->posX, posY = camera->posY;
    int coll = SABRE_GetSurroundingWalls(&posX, &posY, map);
    float radius = 0.4f;

    if ((coll & SABRE_TOP_L_MASK) == SABRE_TOP_L)
        SABRE_KeepDistance(&posX, &posY, (int)posX, (int)posY, radius);
    if ((coll & SABRE_TOP_MASK) == SABRE_TOP)
        SABRE_KeepDistance(&posX, &posY, posX, (int)posY, radius);
    if ((coll & SABRE_TOP_R_MASK) == SABRE_TOP_R)
        SABRE_KeepDistance(&posX, &posY, (int)posX + 1, (int)posY, radius);
    if ((coll & SABRE_LEFT_MASK) == SABRE_LEFT)
        SABRE_KeepDistance(&posX, &posY, (int)posX, posY, radius);
    if ((coll & SABRE_RIGHT_MASK) == SABRE_RIGHT)
        SABRE_KeepDistance(&posX, &posY, (int)posX +  1, posY, radius);
    if ((coll & SABRE_LOW_L_MASK) == SABRE_LOW_L)
        SABRE_KeepDistance(&posX, &posY, (int)posX, (int)posY + 1, radius);
    if ((coll & SABRE_LOW_MASK) == SABRE_LOW)
        SABRE_KeepDistance(&posX, &posY, posX, (int)posY + 1, radius);
    if ((coll & SABRE_LOW_R_MASK) == SABRE_LOW_R)
        SABRE_KeepDistance(&posX, &posY, (int)posX + 1, (int)posY + 1, radius);

    camera->posX = posX;
    camera->posY = posY;
}
