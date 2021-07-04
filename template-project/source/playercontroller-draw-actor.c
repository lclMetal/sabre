char *keyboard = GetKeyState();
struct SABRE_KeybindStruct *keys = &SABRE_keys; // a pointer to the player key binds
struct SABRE_CameraStruct *camera = &SABRE_camera; // a pointer to the camera

float oldDirX = camera->dirX;
float oldPlaneX = camera->planeX;
float rotateSpeed = SABRE_player.turnSpeed;
float moveSpeed = SABRE_player.moveSpeed;

if (keyboard[keys->forward])
{
    if (!map[(short)camera->posY][(short)(camera->posX + camera->dirX * moveSpeed*5)])
    camera->posX += camera->dirX * moveSpeed;
    if (!map[(short)(camera->posY + camera->dirY * moveSpeed*5)][(short)camera->posX])
    camera->posY += camera->dirY * moveSpeed;
}

if (keyboard[keys->backward])
{
    if (!map[(short)camera->posY][(short)(camera->posX - camera->dirX * moveSpeed*5)])
    camera->posX -= camera->dirX * moveSpeed;
    if (!map[(short)(camera->posY - camera->dirY * moveSpeed*5)][(short)camera->posX])
    camera->posY -= camera->dirY * moveSpeed;
}

if (keyboard[keys->turnLeft] && !keyboard[keys->turnRight])
{
    camera->dirX = camera->dirX * cos(-rotateSpeed) - camera->dirY * sin(-rotateSpeed);
    camera->dirY = oldDirX * sin(-rotateSpeed) + camera->dirY * cos(-rotateSpeed);
    camera->planeX = camera->planeX * cos(-rotateSpeed) - camera->planeY * sin(-rotateSpeed);
    camera->planeY = oldPlaneX * sin(-rotateSpeed) + camera->planeY * cos(-rotateSpeed);
}

if (keyboard[keys->turnRight] && !keyboard[keys->turnLeft])
{
    camera->dirX = camera->dirX * cos(rotateSpeed) - camera->dirY * sin(rotateSpeed);
    camera->dirY = oldDirX * sin(rotateSpeed) + camera->dirY * cos(rotateSpeed);
    camera->planeX = camera->planeX * cos(rotateSpeed) - camera->planeY * sin(rotateSpeed);
    camera->planeY = oldPlaneX * sin(rotateSpeed) + camera->planeY * cos(rotateSpeed);
}

if (keyboard[keys->strafeLeft])
{
    if (!map[(short)camera->posY][(short)(camera->posX - camera->planeX * moveSpeed*5)])
    camera->posX -= camera->planeX * moveSpeed;
    if (!map[(short)(camera->posY - camera->planeY * moveSpeed*5)][(short)camera->posX])
    camera->posY -= camera->planeY * moveSpeed;
}

if (keyboard[keys->strafeRight])
{
    if (!map[(short)camera->posY][(short)(camera->posX + camera->planeX * moveSpeed*5)])
    camera->posX += camera->planeX * moveSpeed;
    if (!map[(short)(camera->posY + camera->planeY * moveSpeed*5)][(short)camera->posX])
    camera->posY += camera->planeY * moveSpeed;
}
