struct SABRE_Vector2Struct
{
    float x, y;
};

struct SABRE_Vector2Struct SABRE_CreateVector2(float x, float y)
{
    struct SABRE_Vector2Struct new;
    new.x = x;
    new.y = y;
    return new;
}

struct SABRE_Vector2Struct SABRE_ScaleVector2(struct SABRE_Vector2Struct vec, float scale)
{
    struct SABRE_Vector2Struct new;
    new.x = vec.x * scale;
    new.y = vec.y * scale;
    return new;
}

void SABRE_ScaleVector2InPlace(struct SABRE_Vector2Struct *vec, float scale)
{
    vec->x *= scale;
    vec->y *= scale;
}

struct SABRE_Vector2Struct SABRE_RotateVector2(struct SABRE_Vector2Struct vec, float rotation)
{
    struct SABRE_Vector2Struct new;
    new.x = vec.x * cos(rotation) - vec.y * sin(rotation);
    new.y = vec.x * sin(rotation) + vec.y * cos(rotation);
    return new;
}

void SABRE_RotateVector2InPlace(struct SABRE_Vector2Struct *vec, float rotation)
{
    float oldX = vec->x;
    vec->x = vec->x * cos(rotation) - vec->y * sin(rotation);
    vec->y = oldX   * sin(rotation) + vec->y * cos(rotation);
}

struct SABRE_Vector2Struct SABRE_AddVector2(struct SABRE_Vector2Struct a, struct SABRE_Vector2Struct b)
{
    struct SABRE_Vector2Struct new;
    new.x = a.x + b.x;
    new.y = a.y + b.y;
    return new;
}

void SABRE_AddVector2InPlace(struct SABRE_Vector2Struct *a, struct SABRE_Vector2Struct b)
{
    a->x += b.x;
    a->y += b.y;
}

float SABRE_DotProductVector2(struct SABRE_Vector2Struct a, struct SABRE_Vector2Struct b)
{
    return a.x * b.x + a.y * b.y;
}

float SABRE_MagnitudeVector2(struct SABRE_Vector2Struct a)
{
    return sqrt(SABRE_DotProductVector2(a, a));
}

struct SABRE_Vector2Struct SABRE_NormalizeVector2(struct SABRE_Vector2Struct vec)
{
    float magnitude = SABRE_MagnitudeVector2(vec);

    if (abs(magnitude) <= 0.0001f)
    {
        DEBUG_MSG_FROM("Unable to normalize: vector magnitude was 0.", "SABRE_NormalizeVector2");
        return SABRE_CreateVector2(0.0f, 0.0f);
    }

    return SABRE_ScaleVector2(vec, 1.0f / magnitude);
}

void SABRE_NormalizeVector2InPlace(struct SABRE_Vector2Struct *vec)
{
    float magnitude = SABRE_MagnitudeVector2(*vec);

    if (abs(magnitude) <= 0.0001f)
    {
        DEBUG_MSG_FROM("Unable to normalize: vector magnitude was 0.", "SABRE_NormalizeVector2InPlace");
        vec->x = 0.0f;
        vec->y = 0.0f;
        return;
    }

    SABRE_ScaleVector2InPlace(vec, 1.0f / magnitude);
}
