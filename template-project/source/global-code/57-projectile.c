#define SABRE_PROJECTILE_HANDLER_ACTOR "SABRE_ProjectileHandler"

#define SABRE_PROJECTILE_HIT_WALL 1
#define SABRE_PROJECTILE_HIT_ENTITY 2
#define SABRE_PROJECTILE_HIT_FLOOR 3

#ifndef SABRE_PROJECTILE_DEFINED
typedef struct SABRE_ProjectileStruct
{
    float speed;
    float dropFactor;
    SABRE_Vector3 dir;
    SABRE_Entity *entity;
}SABRE_Projectile;
#define SABRE_PROJECTILE_DEFINED
#endif

// Not really anything like infinity, merely a very high value,
// but the word infinity is shorter than "very high value" :D
#define SABRE_INFINITY 1e30

SABRE_List *SABRE_projectiles = NULL;

typedef struct SABRE_ProjectileHitDataStruct
{
    unsigned char hitType;
    SABRE_Projectile *projectile;
    SABRE_Vector3 hitPosition;
    SABRE_Entity *entityHit;
}SABRE_ProjectileHitData;

SABRE_ProjectileHitData SABRE_projectileHitData;

SABRE_ProjectileHitData SABRE_CreateProjectileHit(unsigned char hitType, SABRE_Projectile *projectile, SABRE_Vector3 hitPosition, SABRE_Entity *entityHit)
{
    SABRE_ProjectileHitData new;

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

#if DEBUG
int projectilesTravelling = 0;
#endif

void SABRE_FireProjectile(SABRE_Vector3 dir, float speed, float dropFactor, float radius, SABRE_Vector3 pos, unsigned int sprite)
{
    char temp[256];
    SABRE_Projectile new;
    SABRE_ListTypes newListElement;

    new.dir = SABRE_ScaleVector3(SABRE_NormalizeVector3(dir), speed);
    new.speed = speed;
    new.dropFactor = dropFactor;
    sprintf(temp, "projectile.%d", SABRE_CountEntitiesInList());
    new.entity = SABRE_AddEntity(radius, pos, sprite, SABRE_ENTITY_NO_COLLISION, temp);

#if DEBUG
    projectilesTravelling++;
#endif

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

void SABRE_UpdateProjectiles()
{
    SABRE_List *next = NULL;
    SABRE_List *next2 = NULL;
    SABRE_List *iterator = NULL;
    SABRE_List *iterator2 = NULL;

    float rayPosX, rayPosY;
    float rayDirX, rayDirY;
    float deltaX, deltaY;
    float distX, distY;
    int rayMapX, rayMapY;
    int stepX, stepY;
    int wallHit = 0;
    int hitSide = 0;
    float wallHitPosition = 0;

    SABRE_Vector3 prevPos;
    SABRE_Vector3 nextPos;
    SABRE_Vector2 collWallPos;
    SABRE_Vector2 collEntityPos;
    SABRE_Vector2 entityCollPos;
    SABRE_Vector2 projection;
    SABRE_Vector2 prevToCollVec;
    SABRE_Vector2 prevToNextVec;

    float dist = 0;
    float keepDist = 0;
    float goBackDist = 0;
    float newDist = 0;
    float entityHitAngle = 0;

    float wallHitDistance = 0;
    float entityHitDistance = 0;
    float lowestEntityHitDistance = 0;

    for (iterator = SABRE_projectiles; iterator != NULL; iterator = next)
    {
        SABRE_ProjectileHitData wallHitData = { 0 };
        SABRE_ProjectileHitData entityHitData = { 0 };

        wallHitDistance = SABRE_INFINITY;
        entityHitDistance = SABRE_INFINITY;
        lowestEntityHitDistance = SABRE_INFINITY;

        next = iterator->next;

        if (iterator->data.projectile.dir.x == 0 && iterator->data.projectile.dir.y == 0)
            continue;

        rayPosX = iterator->data.projectile.entity->pos.x;
        rayPosY = iterator->data.projectile.entity->pos.y;
        rayDirX = iterator->data.projectile.dir.x;
        rayDirY = iterator->data.projectile.dir.y;
        rayMapX = (int)rayPosX;
        rayMapY = (int)rayPosY;
        deltaX = (rayDirX == 0) ? SABRE_INFINITY : abs(1 / rayDirX);
        deltaY = (rayDirY == 0) ? SABRE_INFINITY : abs(1 / rayDirY);
        wallHit = 0;

        if (rayDirX < 0)
        {
            stepX = -1;
            distX = (rayPosX - rayMapX) * deltaX;
        }
        else
        {
            stepX = 1;
            distX = (rayMapX + 1 - rayPosX) * deltaX;
        }

        if (rayDirY < 0)
        {
            stepY = -1;
            distY = (rayPosY - rayMapY) * deltaY;
        }
        else
        {
            stepY = 1;
            distY = (rayMapY + 1 - rayPosY) * deltaY;
        }

        while (!wallHit)
        {
            if (distX < distY)
            {
                distX += deltaX;
                rayMapX += stepX;
                hitSide = 0;
            }
            else
            {
                distY += deltaY;
                rayMapY += stepY;
                hitSide = 1;
            }

            if (rayMapY >= LEVEL_HEIGHT) wallHit = 1;
            if (rayMapX >= LEVEL_WIDTH) wallHit = 1;
            if (rayMapY < 0 || rayMapX < 0) wallHit = 1;

            // ATTENTION!!! projectiles going outside of the level trigger these
            // if (rayMapY >= LEVEL_HEIGHT) DEBUG_MSG_FROM("erorr", "rayMapY >= level height");//wallHit = 1;
            // if (rayMapX >= LEVEL_WIDTH) DEBUG_MSG_FROM("erorr", "rayMapX >= level width");//wallHit = 1;
            // if (rayMapY < 0 || rayMapX < 0) DEBUG_MSG_FROM("erorr", "rayMapY < 0 || rayMapX < 0");//wallHit = 1;

            if (!wallHit)
                wallHit = (map[rayMapY][rayMapX] > 0);
        }

        prevPos = iterator->data.projectile.entity->pos;

        if (hitSide)
        {
            wallHitPosition = rayPosX + ((rayMapY - rayPosY + (1 - stepY) / 2.0f) / rayDirY) * rayDirX;
            collWallPos = SABRE_CreateVector2(wallHitPosition, rayMapY + (stepY < 0));
        }
        else
        {
            wallHitPosition = rayPosY + ((rayMapX - rayPosX + (1 - stepX) / 2.0f) / rayDirX) * rayDirY;
            collWallPos = SABRE_CreateVector2(rayMapX + (stepX < 0), wallHitPosition);
        }

        nextPos = SABRE_AddVector3(prevPos, iterator->data.projectile.dir);

        if (distance(prevPos.x, prevPos.y, collWallPos.x, collWallPos.y) < distance(prevPos.x, prevPos.y, nextPos.x, nextPos.y))
        {
            iterator->data.projectile.entity->pos = SABRE_Vector2ToVector3(collWallPos, iterator->data.projectile.entity->pos.z);

            wallHitData = SABRE_CreateProjectileHit(SABRE_PROJECTILE_HIT_WALL, &iterator->data.projectile, iterator->data.projectile.entity->pos, NULL);
        }

        for (iterator2 = SABRE_entities; iterator2 != NULL; iterator2 = next2)
        {
            next2 = iterator2->next;

            if (iterator2->data.entity.attributes & SABRE_ENTITY_NO_COLLISION)
                continue;

            collEntityPos = SABRE_Vector3ToVector2WithoutZ(iterator2->data.entity.pos);

            prevToCollVec = SABRE_SubstractVector2(collEntityPos, SABRE_Vector3ToVector2WithoutZ(prevPos));
            prevToNextVec = SABRE_SubstractVector2(SABRE_Vector3ToVector2WithoutZ(nextPos), SABRE_Vector3ToVector2WithoutZ(prevPos));

            if (SABRE_DotProductVector2(prevToCollVec, prevToNextVec) < 0)
                continue;

            projection = SABRE_VECTOR2_ZERO;
            dist = SABRE_PointToLineSegmentDistance(SABRE_Vector3ToVector2WithoutZ(prevPos), SABRE_Vector3ToVector2WithoutZ(nextPos), collEntityPos, &projection);
            keepDist = iterator->data.projectile.entity->radius + iterator2->data.entity.radius;
            goBackDist = sqrt(pow(keepDist, 2) - pow(dist, 2));
            newDist = distance(prevPos.x, prevPos.y, projection.x, projection.y) - goBackDist;

            if (SABRE_LineToPointCollision(SABRE_Vector3ToVector2WithoutZ(prevPos), SABRE_Vector3ToVector2WithoutZ(nextPos), collEntityPos, iterator->data.projectile.entity->radius, iterator2->data.entity.radius))
            {
                entityCollPos = SABRE_AddVector2(SABRE_Vector3ToVector2WithoutZ(prevPos), SABRE_ScaleVector2(SABRE_NormalizeVector2(SABRE_Vector3ToVector2WithoutZ(iterator->data.projectile.dir)), newDist));

                entityHitAngle = degtorad(direction(collEntityPos.x, collEntityPos.y, entityCollPos.x, entityCollPos.y));
                entityCollPos.x = collEntityPos.x + cos(entityHitAngle) * keepDist;
                entityCollPos.y = collEntityPos.y - sin(entityHitAngle) * keepDist;

                entityHitDistance = distance(prevPos.x, prevPos.y, entityCollPos.x, entityCollPos.y);

                if (entityHitDistance < lowestEntityHitDistance)
                {
                    iterator->data.projectile.entity->pos = SABRE_Vector2ToVector3(entityCollPos, iterator->data.projectile.entity->pos.z);
                    entityHitData = SABRE_CreateProjectileHit(SABRE_PROJECTILE_HIT_ENTITY, &(iterator->data.projectile), iterator->data.projectile.entity->pos, &(iterator2->data.entity));
                    lowestEntityHitDistance = entityHitDistance;
                }
            }
        }

        if (wallHitData.hitType == SABRE_PROJECTILE_HIT_WALL)
            wallHitDistance = distance(prevPos.x, prevPos.y, wallHitData.hitPosition.x, wallHitData.hitPosition.y);
        if (entityHitData.hitType == SABRE_PROJECTILE_HIT_ENTITY)
            entityHitDistance = distance(prevPos.x, prevPos.y, entityHitData.hitPosition.x, entityHitData.hitPosition.y);

        if (wallHitDistance < SABRE_INFINITY && entityHitDistance < SABRE_INFINITY)
        {
            if (entityHitDistance < wallHitDistance)
            {
                SABRE_SendProjectileHitEvent(entityHitData);
#if DEBUG
                projectilesTravelling--;
#endif
                continue;
            }
            else
            {
                SABRE_SendProjectileHitEvent(wallHitData);
#if DEBUG
                projectilesTravelling--;
#endif
                continue;
            }
        }
        else if (wallHitDistance < SABRE_INFINITY)
        {
            SABRE_SendProjectileHitEvent(wallHitData);
#if DEBUG
                projectilesTravelling--;
#endif
            continue;
        }
        else if (entityHitDistance < SABRE_INFINITY)
        {
            SABRE_SendProjectileHitEvent(entityHitData);
#if DEBUG
                projectilesTravelling--;
#endif
            continue;
        }
        else
        {
            if (iterator->data.projectile.entity->pos.z - iterator->data.projectile.dropFactor <= 0)
            {
                iterator->data.projectile.entity->pos.z = 0;
                SABRE_SendProjectileHitEvent(SABRE_CreateProjectileHit(SABRE_PROJECTILE_HIT_FLOOR, &(iterator->data.projectile), iterator->data.projectile.entity->pos, NULL));
#if DEBUG
                projectilesTravelling--;
#endif
            continue;
            }
            else
            {
                iterator->data.projectile.entity->pos = nextPos;
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
