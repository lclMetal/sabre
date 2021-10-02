char *keyboard = GetKeyState();
struct SABRE_KeybindStruct *keys = &SABRE_keys; // a pointer to the player key binds
struct SABRE_CameraStruct *camera = &SABRE_camera; // a pointer to the camera

struct SABRE_Vector2Struct oldDir   = camera->dir;
struct SABRE_Vector2Struct oldPlane = camera->plane;
float rotateSpeed = SABRE_player.turnSpeed;
float moveSpeed = SABRE_player.moveSpeed;

if (keyboard[keys->forward] && !keyboard[keys->backward])
{
    SABRE_AddVector2InPlace(&camera->pos, SABRE_ScaleVector2(camera->dir, moveSpeed));
}
else if (keyboard[keys->backward] && !keyboard[keys->forward])
{
    SABRE_AddVector2InPlace(&camera->pos, SABRE_ScaleVector2(camera->dir, -moveSpeed));
}

if (keyboard[keys->turnLeft] && !keyboard[keys->turnRight])
{
    SABRE_RotateVector2InPlace(&camera->dir,    -rotateSpeed);
    SABRE_RotateVector2InPlace(&camera->plane,  -rotateSpeed);
}
else if (keyboard[keys->turnRight] && !keyboard[keys->turnLeft])
{
    SABRE_RotateVector2InPlace(&camera->dir,    rotateSpeed);
    SABRE_RotateVector2InPlace(&camera->plane,  rotateSpeed);
}

if (keyboard[keys->strafeLeft] && !keyboard[keys->strafeRight])
{
    SABRE_AddVector2InPlace(&camera->pos, SABRE_ScaleVector2(camera->plane, -moveSpeed));
}
else if (keyboard[keys->strafeRight] && !keyboard[keys->strafeLeft])
{
    SABRE_AddVector2InPlace(&camera->pos, SABRE_ScaleVector2(camera->plane, moveSpeed));
}

{
    float posX = camera->pos.x, posY = camera->pos.y;
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

    camera->pos = SABRE_CreateVector2(posX, posY);
}
