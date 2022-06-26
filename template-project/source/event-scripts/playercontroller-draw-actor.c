struct SABRE_KeyboardState *keys = &SABRE_keys; // a pointer to the player key binds
struct SABRE_CameraStruct *camera = &SABRE_camera; // a pointer to the camera

struct SABRE_Vector2Struct oldDir   = camera->dir;
struct SABRE_Vector2Struct oldPlane = camera->plane;
float rotateSpeed = SABRE_player.turnSpeed;
float moveSpeed = SABRE_player.moveSpeed;

struct SABRE_Vector2Struct prevPos = camera->pos;
struct SABRE_Vector2Struct prevDir = camera->dir;
struct SABRE_Vector2Struct newPos = SABRE_CreateVector2(0.0f, 0.0f);
struct SABRE_Vector2Struct normalizedForwardDir;
struct SABRE_Vector2Struct normalizedRightDir;

SABRE_UpdateKeyboardState();

if (keys->turnLeft && !keys->turnRight)
{
    SABRE_RotateVector2InPlace(&camera->dir,    -rotateSpeed);
    SABRE_RotateVector2InPlace(&camera->plane,  -rotateSpeed);
}
else if (keys->turnRight && !keys->turnLeft)
{
    SABRE_RotateVector2InPlace(&camera->dir,    rotateSpeed);
    SABRE_RotateVector2InPlace(&camera->plane,  rotateSpeed);
}

normalizedForwardDir = SABRE_NormalizeVector2(camera->dir);
normalizedRightDir = SABRE_NormalizeVector2(camera->plane);

if (keys->forward && !keys->backward)
{
    SABRE_AddVector2InPlace(&newPos, SABRE_ScaleVector2(normalizedForwardDir, moveSpeed));
}
else if (keys->backward && !keys->forward)
{
    SABRE_AddVector2InPlace(&newPos, SABRE_ScaleVector2(normalizedForwardDir, -moveSpeed));
}

if (keys->strafeLeft && !keys->strafeRight)
{
    SABRE_AddVector2InPlace(&newPos, SABRE_ScaleVector2(normalizedRightDir, -moveSpeed));
}
else if (keys->strafeRight && !keys->strafeLeft)
{
    SABRE_AddVector2InPlace(&newPos, SABRE_ScaleVector2(normalizedRightDir, moveSpeed));
}

if (newPos.x != 0 || newPos.y != 0)
{
    SABRE_NormalizeVector2InPlace(&newPos);
    SABRE_ScaleVector2InPlace(&newPos, moveSpeed);

    {
        unsigned int i;
        float posX = newPos.x + camera->pos.x, posY = newPos.y + camera->pos.y;
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

        for (i = 0; i < SABRE_ENTITY_COUNT; i++)
        {
            // Skip to next entity if current one is hidden
            if (entities[i].attributes & SABRE_ENTITY_HIDDEN)
                continue;

            SABRE_KeepDistance(&posX, &posY, entities[i].pos.x, entities[i].pos.y, entities[i].radius);
        }

        camera->pos = SABRE_CreateVector2(posX, posY);
    }
}

if (prevPos.x != camera->pos.x || prevPos.y != camera->pos.y)
{
    camera->prevPos = prevPos;
}

if (prevDir.x != camera->dir.x || prevDir.y != camera->dir.y)
{
    camera->prevDir = prevDir;
}

SABRE_UpdateEvents();
