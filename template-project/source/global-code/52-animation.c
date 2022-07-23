#ifndef SABRE_ANIMATION_DEFINED
typedef struct SABRE_AnimationStruct
{
    float frameRate;
    unsigned int nframes;
    unsigned int sprite;
}SABRE_Animation;
#define SABRE_ANIMATION_DEFINED
#endif

#ifndef SABRE_ANIMATOR_DEFINED
typedef struct SABRE_AnimatorStruct
{
    char state;
    unsigned int animpos;
    float accumulatedAnimpos;
    SABRE_Animation anim;
}SABRE_Animator;
#define SABRE_ANIMATOR_DEFINED
#endif

#define SABRE_ANIMATOR_LITERAL(FPS, NFRAMES, SPRITE) { 0, 0, 0.0f, { FPS, NFRAMES, SPRITE } }

SABRE_Animation SABRE_CreateAnimation(float frameRate, unsigned int sprite)
{
    SABRE_Animation anim;

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
