typedef struct SABRE_TextureStruct
{
    short width;
    short height;
    char name[256];
}SABRE_Texture;

SABRE_Texture sabreTextures[2] =
{
    { 256, 256, "bluestone" },
    { 256, 256, "redbrick" }
};
