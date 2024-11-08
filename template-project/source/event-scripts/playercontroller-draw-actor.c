SABRE_KeyboardState *keys = NULL; // a pointer to the player key binds
SABRE_Camera *camera = NULL; // a pointer to the camera

SABRE_Vector2 oldDir;
SABRE_Vector2 oldPlane;
float rotateSpeed;
float moveSpeed;

SABRE_Vector2 prevPos;
SABRE_Vector2 prevDir;
SABRE_Vector2 newPos;
SABRE_Vector2 normalizedForwardDir;
SABRE_Vector2 normalizedRightDir;

int coll;
float posX;
float posY;
float radius;
unsigned int i;
SABRE_List *iterator = NULL;
SABRE_Entity *entity = NULL;

if (SABRE_gameState == SABRE_RUNNING)
{
    keys = &SABRE_keys; // a pointer to the player key binds
    camera = &SABRE_camera; // a pointer to the camera

    oldDir   = camera->dir;
    oldPlane = camera->plane;
    rotateSpeed = SABRE_player.turnSpeed;
    moveSpeed = SABRE_player.moveSpeed;

    prevPos = camera->pos;
    prevDir = camera->dir;
    newPos = SABRE_CreateVector2(0.0f, 0.0f);

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

    if (keys->crouch && camera->vPos < SABRE_player.crouchHeightChange)
    {
        camera->vPos += SABRE_player.crouchSpeed;
    }
    else if (!keys->crouch && camera->vPos > 0.0f)
    {
        camera->vPos -= SABRE_player.crouchSpeed;
    }
    else if (keys->crouch)
    {
        camera->vPos = SABRE_player.crouchHeightChange;
    }
    else
    {
        camera->vPos = 0.0f;
    }

    // if (keys->releasedInteract)
    if (keys->interact)
    {
        if (frame % 5 == 0)
            SABRE_FireProjectile(SABRE_Vector2ToVector3(camera->dir, 0.2f), 0.1f, 0.002f, 0.01f, SABRE_Vector2ToVector3(camera->pos, 0.3f), SABRE_CreateAnimator(SABRE_CreateAnimation(0, 6)));
    }

    if (newPos.x != 0 || newPos.y != 0)
    {
        SABRE_NormalizeVector2InPlace(&newPos);
        SABRE_ScaleVector2InPlace(&newPos, moveSpeed);

        posX = newPos.x + camera->pos.x;
        posY = newPos.y + camera->pos.y;
        coll = SABRE_GetSurroundingWalls(&posX, &posY, &SABRE_level);
        radius = SABRE_player.radius;

        posX = fmod(posX, SABRE_level.width);
        if (posX < 0) posX += SABRE_level.width;
        posY = fmod(posY, SABRE_level.height);
        if (posY < 0) posY += SABRE_level.height;

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

        for (iterator = SABRE_entities; iterator != NULL; iterator = iterator->next)
        {
            entity = &iterator->data.entity;
            if (entity->attributes & SABRE_ENTITY_NO_COLLISION)
                continue;

            SABRE_KeepDistance(&posX, &posY, entity->pos.x, entity->pos.y, entity->radius + radius);
        }

        camera->pos = SABRE_CreateVector2(posX, posY);
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
}
