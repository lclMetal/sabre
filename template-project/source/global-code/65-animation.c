SABRE_Animation SABRE_CreateAnimation(float frameRate, unsigned int sprite)
{
    SABRE_Animation anim;

    if (sprite >= SABRE_spriteStore.count)
    {
        DEBUG_MSG_FROM("Trying to create an animation with an invalid sprite index.", "SABRE_CreateAnimation");
        anim.frameRate = anim.nframes = anim.sprite = 0;
    }

    anim.frameRate = frameRate;
    anim.nframes = SABRE_sprites != NULL ? SABRE_sprites[sprite].nframes : 1;
    anim.sprite = sprite;

    return anim;
}

void SABRE_ChangeAnimation(SABRE_Animator *animator, SABRE_Animation anim, char state)
{
    animator->state = state;
    animator->animpos = 0;
    animator->accumulatedAnimpos = 0.0f;
    animator->anim = anim;
}

void SABRE_ChangeAnimationDirection(SABRE_Animator *animator, char state)
{
    animator->state = state;
}

SABRE_Animator SABRE_CreateAnimator(SABRE_Animation anim)
{
    SABRE_Animator animator;

    animator.state = FORWARD;
    animator.animpos = 0;
    animator.accumulatedAnimpos = 0.0f;
    animator.anim = anim;

    return animator;
}

void SABRE_UpdateAnimation(SABRE_Animator *animator)
{
    if (!animator)
    {
        DEBUG_MSG_FROM("Invalid animator pointer", "SABRE_UpdateAnimation");
        return;
    }

    if (animator->state == STOPPED)
        return;

    animator->accumulatedAnimpos += animator->anim.frameRate / max(real_fps, 1);

    if (animator->accumulatedAnimpos >= 1.0f)
    {
        animator->animpos = (animator->animpos + (int)animator->accumulatedAnimpos) % (int)max(animator->anim.nframes, 1);
        animator->accumulatedAnimpos = max(0, animator->accumulatedAnimpos - (int)animator->accumulatedAnimpos);
    }
}
