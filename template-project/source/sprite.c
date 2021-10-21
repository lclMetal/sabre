#define SABRE_SPRITE_ACTOR "SABRE_SpriteActor"

struct SABRE_SpriteStruct
{
    struct SABRE_Vector2Struct pos;
    float radius;
    unsigned int sprite;
};

#define SABRE_SPRITE_COUNT 11

struct SABRE_SpriteStruct sprites[SABRE_SPRITE_COUNT] =
{
    { { 8.5f, 1.5f }, 0.4f, 1 },
    { { 7.5f, 1.5f }, 0.4f, 1 },
    { { 6.5f, 1.5f }, 0.4f, 2 },
    { { 8.5f, 2.5f }, 0.4f, 1 },
    { { 7.5f, 2.5f }, 0.4f, 0 },
    { { 6.5f, 2.5f }, 0.4f, 1 },
    { { 6.5f, 3.5f }, 0.4f, 3 },
    { { 7.0f, 3.5f }, 0.4f, 3 },
    { { 7.5f, 3.5f }, 0.4f, 3 },
    { { 8.0f, 3.5f }, 0.4f, 3 },
    { { 8.5f, 3.5f }, 0.4f, 3 }
};
