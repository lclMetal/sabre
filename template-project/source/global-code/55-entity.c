enum SABRE_EntityAttributeFlags
{
    SABRE_ENTITY_HIDDEN = 1
};

struct SABRE_EntityStruct
{
    float radius;
    struct SABRE_Vector2Struct pos;
    // struct SABRE_SpriteStruct *sprite;
    unsigned int sprite;
    unsigned char attributes;
};

#define SABRE_ENTITY_COUNT 11

struct SABRE_EntityStruct entities[SABRE_ENTITY_COUNT] =
{
    { 0.4f, { 8.5f, 1.5f }, 1 },
    { 0.4f, { 7.5f, 1.5f }, 1 },
    { 0.4f, { 6.5f, 1.5f }, 2 },
    { 0.4f, { 8.5f, 2.5f }, 1 },
    { 0.4f, { 7.5f, 2.5f }, 0 },
    { 0.4f, { 6.5f, 2.5f }, 1 },
    { 0.4f, { 6.5f, 3.5f }, 3 },
    { 0.4f, { 7.0f, 3.5f }, 3 },
    { 0.4f, { 7.5f, 3.5f }, 3 },
    { 0.4f, { 8.0f, 3.5f }, 3 },
    { 0.4f, { 8.5f, 3.5f }, 3 }
};
