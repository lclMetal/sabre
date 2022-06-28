enum SABRE_EntityAttributeFlags
{
    SABRE_ENTITY_HIDDEN = 1
};

typedef struct SABRE_EntityStruct
{
    float radius;
    SABRE_Vector2 pos;
    unsigned int sprite;
    unsigned char attributes;
}SABRE_Entity;
