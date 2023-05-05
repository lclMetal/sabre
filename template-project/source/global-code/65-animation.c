SABRE_Animation SABRE_CreateAnimation(float frameRate, unsigned int sprite)
{
    SABRE_Animation anim;
    unsigned int spriteNum = SABRE_ValidateSpriteIndex(sprite + 1); // offset by one because of the "missing sprite" sprite

    if (spriteNum == 0)
    {
        char temp[256];
        sprintf(temp, "Invalid sprite index: %d", sprite);
        DEBUG_MSG_FROM(temp, "SABRE_CreateAnimation");
    }

    anim.frameRate = frameRate;
    anim.nframes = SABRE_sprites != NULL ? SABRE_sprites[spriteNum].nframes : 1;
    anim.sprite = spriteNum;

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
