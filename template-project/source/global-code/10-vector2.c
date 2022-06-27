typedef struct SABRE_Vector2Struct
{
    float x, y;
}SABRE_Vector2;

SABRE_Vector2 SABRE_CreateVector2(float x, float y)
{
    SABRE_Vector2 new;
    new.x = x;
    new.y = y;
    return new;
}

SABRE_Vector2 SABRE_ScaleVector2(SABRE_Vector2 vec, float scale)
{
    SABRE_Vector2 new;
    new.x = vec.x * scale;
    new.y = vec.y * scale;
    return new;
}

void SABRE_ScaleVector2InPlace(SABRE_Vector2 *vec, float scale)
{
    vec->x *= scale;
    vec->y *= scale;
}

SABRE_Vector2 SABRE_RotateVector2(SABRE_Vector2 vec, float rotation)
{
    SABRE_Vector2 new;
    new.x = vec.x * cos(rotation) - vec.y * sin(rotation);
    new.y = vec.x * sin(rotation) + vec.y * cos(rotation);
    return new;
}

void SABRE_RotateVector2InPlace(SABRE_Vector2 *vec, float rotation)
{
    float oldX = vec->x;
    vec->x = vec->x * cos(rotation) - vec->y * sin(rotation);
    vec->y = oldX   * sin(rotation) + vec->y * cos(rotation);
}

SABRE_Vector2 SABRE_AddVector2(SABRE_Vector2 a, SABRE_Vector2 b)
{
    SABRE_Vector2 new;
    new.x = a.x + b.x;
    new.y = a.y + b.y;
    return new;
}

void SABRE_AddVector2InPlace(SABRE_Vector2 *a, SABRE_Vector2 b)
{
    a->x += b.x;
    a->y += b.y;
}

float SABRE_DotProductVector2(SABRE_Vector2 a, SABRE_Vector2 b)
{
    return a.x * b.x + a.y * b.y;
}

float SABRE_MagnitudeVector2(SABRE_Vector2 a)
{
    return sqrt(SABRE_DotProductVector2(a, a));
}

SABRE_Vector2 SABRE_NormalizeVector2(SABRE_Vector2 vec)
{
    float magnitude = SABRE_MagnitudeVector2(vec);

    if (abs(magnitude) <= 0.0001f)
    {
        DEBUG_MSG_FROM("Unable to normalize: vector magnitude was 0.", "SABRE_NormalizeVector2");
        return SABRE_CreateVector2(0.0f, 0.0f);
    }

    return SABRE_ScaleVector2(vec, 1.0f / magnitude);
}

void SABRE_NormalizeVector2InPlace(SABRE_Vector2 *vec)
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
