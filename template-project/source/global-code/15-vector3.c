#define SABRE_VECTOR3_ZERO SABRE_CreateVector3(0.0f, 0.0f, 0.0f)

Vector SABRE_CreateVector3(float x, float y, float z)
{
    Vector new;
    new.x = x;
    new.y = y;
    new.z = z;
    return new;
}

Vector SABRE_Vector2ToVector3(SABRE_Vector2 vec2, float z)
{
    Vector new;
    new.x = vec2.x;
    new.y = vec2.y;
    new.z = z;
    return new;
}

SABRE_Vector2 SABRE_Vector3ToVector2WithoutZ(Vector vec3)
{
    SABRE_Vector2 new;
    new.x = vec3.x;
    new.y = vec3.y;
    return new;
}

Vector SABRE_ScaleVector3(Vector vec, float scale)
{
    Vector new;
    new.x = vec.x * scale;
    new.y = vec.y * scale;
    new.z = vec.z * scale;
    return new;
}

void SABRE_ScaleVector3InPlace(Vector *vec, float scale)
{
    vec->x *= scale;
    vec->y *= scale;
    vec->z *= scale;
}

Vector SABRE_AddVector3(Vector a, Vector b)
{
    Vector new;
    new.x = a.x + b.x;
    new.y = a.y + b.y;
    new.z = a.z + b.z;
    return new;
}

void SABRE_AddVector3InPlace(Vector *a, Vector b)
{
    a->x += b.x;
    a->y += b.y;
    a->z += b.z;
}

float SABRE_DotProductVector3(Vector a, Vector b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float SABRE_MagnitudeVector3(Vector a)
{
    return sqrt(SABRE_DotProductVector3(a, a));
}

Vector SABRE_NormalizeVector3(Vector vec)
{
    float magnitude = SABRE_MagnitudeVector3(vec);

    if (abs(magnitude) <= 0.0001f)
    {
        DEBUG_MSG_FROM("Unable to normalize: vector magnitude was 0.", "SABRE_NormalizeVector3");
        return SABRE_CreateVector3(0.0f, 0.0f, 0.0f);
    }

    return SABRE_ScaleVector3(vec, 1.0f / magnitude);
}

void SABRE_NormalizeVector3InPlace(Vector *vec)
{
    float magnitude = SABRE_MagnitudeVector3(*vec);

    if (abs(magnitude) <= 0.0001f)
    {
        DEBUG_MSG_FROM("Unable to normalize: vector magnitude was 0.", "SABRE_NormalizeVector3InPlace");
        vec->x = 0.0f;
        vec->y = 0.0f;
        vec->z = 0.0f;
        return;
    }

    SABRE_ScaleVector3InPlace(vec, 1.0f / magnitude);
}
