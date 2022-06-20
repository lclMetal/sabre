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

#define SABRE_DIMENSION_X 0
#define SABRE_DIMENSION_Y 1

int SABRE_GetAnimationDimensionInPixels(const char actorName[256], const char animName[256], int dimension)
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

    for (i = 0; i < animFrameCount; i++)
    {
        animationActor = getclone(animationActor->clonename); // this updates the width and height values

        switch (dimension)
        {
            default:
            case SABRE_DIMENSION_X:
                if (animationActor->width > dimensionPixels)
                {
                    dimensionPixels = animationActor->width;
                }
                break;

            case SABRE_DIMENSION_Y:
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
