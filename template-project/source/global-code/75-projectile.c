SABRE_ProjectileHitData SABRE_CreateProjectileHit(unsigned char hitType, float dist, SABRE_Projectile *projectile, SABRE_Vector3 hitPosition, SABRE_Entity *entityHit)
{
    SABRE_ProjectileHitData new;

    new.dist = dist;
    new.hitType = hitType;
    new.projectile = projectile;
    new.hitPosition = hitPosition;
    new.entityHit = entityHit;

    return new;
}

void SABRE_SendProjectileHitEvent(SABRE_ProjectileHitData hitData)
{
    SABRE_projectileHitData = hitData;
    SendActivationEvent(SABRE_PROJECTILE_HANDLER_ACTOR);
}

void SABRE_FireProjectile(SABRE_Vector3 dir, float speed, float dropFactor, float radius, SABRE_Vector3 pos, SABRE_Animator animator)
{
    char temp[256];
    SABRE_Projectile new;
    SABRE_ListTypes newListElement;

    new.dir = SABRE_ScaleVector3(SABRE_NormalizeVector3(dir), speed);
    new.speed = speed;
    new.dropFactor = dropFactor;
    sprintf(temp, "projectile.%d", SABRE_CountEntitiesInList());
    new.entity = SABRE_AddEntity(radius, pos, animator, SABRE_ENTITY_NO_COLLISION, temp);

    newListElement.projectile = new;
    SABRE_AddToList(&SABRE_projectiles, newListElement);
}



SABRE_List *SABRE_GetProjectileListObject(SABRE_Projectile *projectile)
{
    SABRE_List *iterator = NULL;

    for (iterator = SABRE_projectiles; iterator != NULL; iterator = iterator->next)
    {
        if (projectile == &iterator->data.projectile)
        {
            return iterator;
        }
    }

    return NULL;
}

SABRE_Vector2 SABRE_Raycast(SABRE_Vector2 rayStartPos, SABRE_Vector2 rayDirection)
{
    SABRE_Vector2 wallHitPosition;

    float rayPosX = rayStartPos.x;
    float rayPosY = rayStartPos.y;
    float rayDirX = rayDirection.x;
    float rayDirY = rayDirection.y;
    float deltaX = (rayDirX == 0) ? SABRE_INFINITY : abs(1 / rayDirX);
    float deltaY = (rayDirY == 0) ? SABRE_INFINITY : abs(1 / rayDirY);
    float distX;
    float distY;

    int rayMapY = (int)rayPosY;
    int rayMapX = (int)rayPosX;
    int unwrappedRayMapY = rayMapY;
    int unwrappedRayMapX = rayMapX;
    int stepX;
    int stepY;
    int rayMapIndex = 0;
    int newRayMapIndex = 0;
    int wallHit = 0;
    int hitSide = 0;
    int wrapCount = 0;

    stepX = 1 - (rayDirX < 0) * 2;
    stepY = 1 - (rayDirY < 0) * 2;

    if (rayDirX < 0) distX = (rayPosX - rayMapX) * deltaX;
    else             distX = (rayMapX + 1 - rayPosX) * deltaX;

    if (rayDirY < 0) distY = (rayPosY - rayMapY) * deltaY;
    else             distY = (rayMapY + 1 - rayPosY) * deltaY;

    while (!wallHit)
    {
        if (distX < distY)
        {
            distX += deltaX;
            rayMapX += stepX;
            unwrappedRayMapX += stepX;
            hitSide = 0;
        }
        else
        {
            distY += deltaY;
            rayMapY += stepY;
            unwrappedRayMapY += stepY;
            hitSide = 1;
        }

        rayMapIndex = rayMapY * SABRE_level.width + rayMapX;

        if (SABRE_graphicsSettings.levelEdgeMode == SABRE_EDGE_MODE_WRAP)
        {
            rayMapY = SABRE_WrapIntValue(rayMapY, SABRE_level.height);
            rayMapX = SABRE_WrapIntValue(rayMapX, SABRE_level.width);
            newRayMapIndex = rayMapY * SABRE_level.width + rayMapX;

            // check if a wrap happened
            if (newRayMapIndex != rayMapIndex)
                wrapCount++;

            rayMapIndex = newRayMapIndex;

            // limit wrap count to avoid infinite loop
            if (wrapCount > SABRE_graphicsSettings.levelEdgeWrapDepth)
                break;
        }
        else
        {
            // check if raycast reached the level edge
            if (rayMapY > SABRE_level.height - 2 || rayMapY < 1 ||
                rayMapX > SABRE_level.width  - 2 || rayMapX < 1)
            {
                break;
            }
        }

        wallHit = (SABRE_level.map[rayMapIndex].texture > 0);
    }

    if (hitSide)
        return SABRE_CreateVector2(rayPosX + ((unwrappedRayMapY - rayPosY + (1 - stepY) / 2.0f) / rayDirY) * rayDirX, unwrappedRayMapY + (stepY < 0));

    return SABRE_CreateVector2(unwrappedRayMapX + (stepX < 0), rayPosY + ((unwrappedRayMapX - rayPosX + (1 - stepX) / 2.0f) / rayDirX) * rayDirY);
}

SABRE_ProjectileHitData SABRE_CheckProjectileWallCollision(SABRE_Projectile *projectile)
{
    SABRE_Vector2 prevPos = SABRE_Vector3ToVector2WithoutZ(projectile->entity->pos);
    SABRE_Vector2 nextPos = SABRE_AddVector2(prevPos, SABRE_Vector3ToVector2WithoutZ(projectile->dir));
    SABRE_Vector2 wallHitPosition = SABRE_Raycast(prevPos, SABRE_Vector3ToVector2WithoutZ(projectile->dir));
    SABRE_Vector2 wrappedWallHitPos;
    SABRE_ProjectileHitData hitData = { SABRE_INFINITY };

    float distToWall = distance(prevPos.x, prevPos.y, wallHitPosition.x, wallHitPosition.y);
    float distToNextPos = distance(prevPos.x, prevPos.y, nextPos.x, nextPos.y);

    wrappedWallHitPos.x = SABRE_WrapFloatValue(wallHitPosition.x, SABRE_level.width);
    wrappedWallHitPos.y = SABRE_WrapFloatValue(wallHitPosition.y, SABRE_level.height);

    if (distToWall < distToNextPos)
        hitData = SABRE_CreateProjectileHit(SABRE_PROJECTILE_HIT_WALL, distToWall, projectile, SABRE_Vector2ToVector3(wrappedWallHitPos, projectile->entity->pos.z), NULL);

    return hitData;
}

SABRE_ProjectileHitData SABRE_CheckProjectileEntityCollision(SABRE_Projectile *projectile, SABRE_Entity *entity)
{
    float dist;
    float keepDist;
    float goBackDist;
    float newDist;
    float hitAngle;

    SABRE_Vector2 collisionPos;
    SABRE_Vector2 projection = SABRE_VECTOR2_ZERO;

    SABRE_Vector2 entityPos = SABRE_Vector3ToVector2WithoutZ(entity->pos);

    SABRE_Vector2 prevPos = SABRE_Vector3ToVector2WithoutZ(projectile->entity->pos);
    SABRE_Vector2 nextPos = SABRE_AddVector2(prevPos, SABRE_Vector3ToVector2WithoutZ(projectile->dir));
    SABRE_Vector2 prevToNext = SABRE_SubstractVector2(nextPos, prevPos);
    SABRE_Vector2 prevToColl = SABRE_SubstractVector2(entityPos, prevPos);

    SABRE_ProjectileHitData hitData = { SABRE_INFINITY };

    if (entity->attributes & SABRE_ENTITY_NO_COLLISION)
        return hitData;

    // No need to process entities that are behind the projectile
    if (SABRE_DotProductVector2(prevToColl, prevToNext) < 0)
        return hitData;

    dist = SABRE_PointToLineSegmentDistance(prevPos, nextPos, entityPos, &projection);
    keepDist = projectile->entity->radius + entity->radius;
    goBackDist = sqrt(pow(keepDist, 2) - pow(dist, 2));
    newDist = distance(prevPos.x, prevPos.y, projection.x, projection.y) - goBackDist;

    if (SABRE_LineToPointCollision(prevPos, nextPos, entityPos, projectile->entity->radius, entity->radius))
    {
        collisionPos = SABRE_AddVector2(prevPos, SABRE_ScaleVector2(SABRE_NormalizeVector2(SABRE_Vector3ToVector2WithoutZ(projectile->dir)), newDist));
        hitAngle = degtorad(direction(entityPos.x, entityPos.y, collisionPos.x, collisionPos.y));
        collisionPos.x = entityPos.x + cos(hitAngle) * keepDist;
        collisionPos.y = entityPos.y - sin(hitAngle) * keepDist;

        hitData = SABRE_CreateProjectileHit(SABRE_PROJECTILE_HIT_ENTITY, distance(prevPos.x, prevPos.y, collisionPos.x, collisionPos.y), projectile, SABRE_Vector2ToVector3(collisionPos, projectile->entity->pos.z), entity);
    }

    return hitData;
}

SABRE_ProjectileHitData SABRE_CheckProjectileEntityCollisions(SABRE_Projectile *projectile)
{
    SABRE_List *iterator = NULL;
    SABRE_List *next = NULL;

    float lowestDistance = SABRE_INFINITY;
    SABRE_ProjectileHitData hitData = { SABRE_INFINITY };
    SABRE_ProjectileHitData tempHitData = { SABRE_INFINITY };

    for (iterator = SABRE_entities; iterator != NULL; iterator = next)
    {
        next = iterator->next;

        tempHitData = SABRE_CheckProjectileEntityCollision(projectile, &iterator->data.entity);

        if (tempHitData.dist < lowestDistance)
        {
            hitData = tempHitData;
            lowestDistance = hitData.dist;
        }
    }

    return hitData;
}

void SABRE_StopProjectileAtPosition(SABRE_Projectile *projectile, SABRE_Vector3 position)
{
    projectile->entity->pos = position;
    projectile->dir = SABRE_VECTOR3_ZERO;
}

void SABRE_UpdateProjectiles()
{
    SABRE_List *next = NULL;
    SABRE_List *iterator = NULL;

    SABRE_ProjectileHitData wallHitData;
    SABRE_ProjectileHitData entityHitData;

    for (iterator = SABRE_projectiles; iterator != NULL; iterator = next)
    {
        next = iterator->next;

        if (iterator->data.projectile.dir.x == 0 && iterator->data.projectile.dir.y == 0 && iterator->data.projectile.dir.z == 0)
            continue;

        wallHitData = SABRE_CheckProjectileWallCollision(&iterator->data.projectile);
        entityHitData = SABRE_CheckProjectileEntityCollisions(&iterator->data.projectile);

        if (entityHitData.dist < wallHitData.dist)
        {
            iterator->data.projectile.entity->pos = entityHitData.hitPosition;
            SABRE_StopProjectileAtPosition(&iterator->data.projectile, entityHitData.hitPosition);
            SABRE_SendProjectileHitEvent(entityHitData);
        }
        else if (wallHitData.dist < entityHitData.dist)
        {
            SABRE_StopProjectileAtPosition(&iterator->data.projectile, wallHitData.hitPosition);
            SABRE_SendProjectileHitEvent(wallHitData);
        }
        else
        {
            if (iterator->data.projectile.entity->pos.z - iterator->data.projectile.dropFactor <= 0)
            {
                iterator->data.projectile.entity->pos.z = 0;
                iterator->data.projectile.entity->pos.x = SABRE_WrapFloatValue(iterator->data.projectile.entity->pos.x, SABRE_level.width);
                iterator->data.projectile.entity->pos.y = SABRE_WrapFloatValue(iterator->data.projectile.entity->pos.y, SABRE_level.height);
                iterator->data.projectile.dir = SABRE_VECTOR3_ZERO;
                SABRE_SendProjectileHitEvent(SABRE_CreateProjectileHit(SABRE_PROJECTILE_HIT_FLOOR, 0, &(iterator->data.projectile), iterator->data.projectile.entity->pos, NULL));
            }
            else
            {
                SABRE_AddVector3InPlace(&iterator->data.projectile.entity->pos, iterator->data.projectile.dir);
                iterator->data.projectile.dir.z -= iterator->data.projectile.dropFactor;
            }
        }
    }
}

void SABRE_FreeProjectileList()
{
    SABRE_FreeList(SABRE_projectiles);
    SABRE_projectiles = NULL;
}
