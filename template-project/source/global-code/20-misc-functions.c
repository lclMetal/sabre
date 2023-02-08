float SABRE_LimitValue01(float val)
{
    return max(0, min(1, val));
}

int SABRE_LimitIntValue(int val, int minVal, int maxVal)
{
    return max(min(minVal, maxVal), min(val, max(minVal, maxVal)));
}

int SABRE_WrapIntValue(int val, int limit)
{
    int mod = val % limit;
    return (mod < 0) ? mod + limit : mod;
}

float SABRE_WrapFloatValue(float val, int limit)
{
    int mod = (int)val % limit;
    float decimals = val - (int)val;

    return (mod + decimals < 0) ? mod + decimals + limit : mod + decimals;
}

Actor *SABRE_gc2(char *actorName, long cloneNum)
{
    char cName[256];
    sprintf(cName, "%s.%d", actorName, cloneNum);
    return getclone(cName);
}

// algorithm by user Grumdrig on stack overflow: https://stackoverflow.com/a/1501725
float SABRE_PointToLineSegmentDistance(SABRE_Vector2 a, SABRE_Vector2 b, SABRE_Vector2 p, SABRE_Vector2 *projectionResult)
{
    float projectionLength;
    SABRE_Vector2 projection;
    SABRE_Vector2 ab = SABRE_SubstractVector2(b, a);
    SABRE_Vector2 ap = SABRE_SubstractVector2(p, a);
    float l2 = pow(a.x - b.x, 2) + pow(a.y - b.y, 2);

    if (l2 == 0.0f) return distance(p.x, p.y, a.x, a.y);

    projectionLength = SABRE_LimitValue01(SABRE_DotProductVector2(ap, ab) / (float)l2);
    projection = SABRE_AddVector2(a, SABRE_ScaleVector2(ab, projectionLength));

    if (projectionResult)
        *projectionResult = projection;

    return distance(p.x, p.y, projection.x, projection.y);
}

int SABRE_LineToPointCollision(SABRE_Vector2 a, SABRE_Vector2 b, SABRE_Vector2 p, float lineWidth, float pointRadius)
{
    float collisionThreshold = lineWidth + pointRadius;
    return SABRE_PointToLineSegmentDistance(a, b, p, NULL) <= collisionThreshold;
}

float SABRE_GetPositiveBiasedSign(float val)
{
    if (val < 0.0f) return -1.0f;
    return 1.0f;
}

float SABRE_RandomBetween(float lim1, float lim2)
{
    return rand(max(lim1, lim2) - min(lim1, lim2)) + min(lim1, lim2);
}

float SABRE_NormalizeAngleTo360(float ang)
{
    float tempAng = ang;

    while (tempAng < 0)
    {
        tempAng += 360;
    }

    while (tempAng > 360.0f)
    {
        tempAng -= 360;
    }

    return tempAng;
}

float SABRE_NormalizeAngleTo90(float ang)
{
    float tempAng = ang;

    while (tempAng < 0)
    {
        tempAng += 90;
    }

    while (tempAng > 90.0f)
    {
        tempAng -= 90;
    }

    return tempAng;
}

int SABRE_ActorExists(const char *actorName)
{
    // GE uses the cloneindex -1 to indicate that an actor is not a valid, existing actor.
    // The actor count needs to be checked because:
    //
    // 1. IF an actor with the given base name exists in the project
    // 2. AND the lowest indexed clone (= the actor itself, if no clones present)
    //      in editor mode has "Create at startup" set to "No"
    // 3. AND the actor currently has no alive instances in the game
    //
    // ..getclone will return an invalid Actor* that has the cloneindex of that specific
    // clone / actor over in editor mode despite it not existing in the game.
    // And that would break this function without the ActorCount check.
    Actor *a = getclone(actorName);
    return (ActorCount(a->name) > 0 && a->cloneindex != -1);
}

// ----- WARNING! -----
//
// Using this function on a stale pointer is undefined behavior!
// Use this function ONLY for checking the result of getclone() right after fetching it.
//
// If you didn't fully understand the above warnings, DO NOT USE THIS FUNCTION!
// Use the above version instead (actorExists())
//
// This function only exists for optimizing performance in cases where a fresh actor pointer
// is already at hand.
int SABRE_ActorExists2(Actor *a)
{
    // GE uses the cloneindex -1 to indicate that an actor is not a valid, existing actor.
    // The actor count needs to be checked because:
    //
    // 1. IF an actor with the given base name exists in the project
    // 2. AND the lowest indexed clone (= the actor itself, if no clones present)
    //      in editor mode has "Create at startup" set to "No"
    // 3. AND the actor currently has no alive instances in the game
    //
    // ..getclone will return an invalid Actor* that has the cloneindex of that specific
    // clone / actor over in editor mode despite it not existing in the game.
    // And that would break this function without the ActorCount check.
    return (ActorCount(a->name) > 0 && a->cloneindex != -1);
}

int SABRE_StringEndsWith(const char *str, const char *str2)
{
    size_t len1 = strlen(str);
    size_t len2 = strlen(str2);

    if (len1 < len2)
    {
        return 0;
    }

    return !strcmp(&str[len1 - len2], str2);
}

#define SABRE_ANIM_WIDTH 0
#define SABRE_ANIM_HEIGHT 1
#define SABRE_ANIM_NFRAMES 2

int SABRE_GetAnimationDataValue(const char actorName[256], const char animName[256], unsigned char dataValueType)
{
    int i = 0;
    int dimensionPixels = 0;
    int animFrameCount = 0;
    Actor *animationActor = getclone(actorName);

    if (!SABRE_ActorExists2(animationActor))
    {
        DEBUG_MSG_FROM("The actor doesn't exist.", "SABRE_GetAnimationDimensionInPixels");
        return 1;
    }

    ChangeAnimation(animationActor->clonename, animName, STOPPED);
    SendActivationEvent(actorName); // this "finalizes" the animation change, resetting the actor's animpos
    animFrameCount = animationActor->nframes;

    if (dataValueType == SABRE_ANIM_NFRAMES)
        return animFrameCount;

    for (i = 0; i < animFrameCount; i++)
    {
        animationActor = getclone(animationActor->clonename); // this updates the width and height values

        switch (dataValueType)
        {
            default:
            case SABRE_ANIM_WIDTH:
                if (animationActor->width > dimensionPixels)
                {
                    dimensionPixels = animationActor->width;
                }
                break;

            case SABRE_ANIM_HEIGHT:
                if (animationActor->height > dimensionPixels)
                {
                    dimensionPixels = animationActor->height;
                }
                break;
        }

        animationActor->animpos++;
        // Send activation event to apply the animpos advancement during this frame already.
        // The normal behavior of Game Editor is to update the animpos of an actor in the next
        // frame. This same trick is used for changing the TextureSlice's animpos multiple
        // times per frame when drawing the game view. Notice, however, that if you were to
        // inspect the actor during a frame where this trick is used you still wouldn't see
        // the animpos changing more than once during a single frame. This is because Game Editor
        // only draws the actor on screen once per frame. But behind the scenes the animpos
        // still changes multiple times per frame, affecting the actor's dimensions as well as
        // its appearance if drawn using draw_from().
        SendActivationEvent(actorName);
    }

    return dimensionPixels;
}
