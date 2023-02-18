#define SWE_TEXTURE_CONF_FILE "textures.conf"
#define SWE_TEXTURE_NAME_MAX_LENGTH 30

int stringIsAlphaNumericHyphenUnderscore(const char *str)
{
    char c;
    size_t i;
    size_t len = strlen(str);

    for (i = 0; i < len; i++)
    {
        c = str[i];

        if (!(c >= 'A' && c <= 'Z' ||
              c >= 'a' && c <= 'z' ||
              c >= '0' && c <= '9' ||
              c == '-' || c == '_'))
        {
            return 0;
        }
    }

    return 1;
}

void clearTextureConfFile()
{
    FILE *f;
    f = fopen(SWE_TEXTURE_CONF_FILE, "w");
    if (f) fclose(f);
}

// Must be called from an event of SABRE_TextureActor
int validateTextureNames()
{
    int texIndex = 2; // Ignore project-management animation and "missing texture" texture
    size_t texNameLen;
    char animName[256];

    if (strcmp(name, "SABRE_TextureActor") != 0)
    {
        DEBUG_MSG_FROM("This function must be called from an event of SABRE_TextureActor", "validateTextureNameLengths");
        return 1; // Function called from invalid event
    }

    strcpy(animName, getAnimName(texIndex));
    texNameLen = strlen(animName);

    while (texNameLen)
    {
        // Name length greater than 30, can't guarantee correct matching to texture file
        if (texNameLen > SWE_TEXTURE_NAME_MAX_LENGTH || !stringIsAlphaNumericHyphenUnderscore(animName))
        {
            clearTextureConfFile();
            return texIndex;
        }

        strcpy(animName, getAnimName(++texIndex));
        texNameLen = strlen(animName);
    }

    return 0;
}

// Must be called from an event of SABRE_TextureActor
int checkTextureNameChanges()
{
    FILE *f;
    int texIndex = 2; // Ignore project-management animation and "missing texture" texture
    size_t texNameLen = strlen(getAnimName(texIndex));
    char readName[SWE_TEXTURE_NAME_MAX_LENGTH + 1];
    char format[30];
    char temp[256];

    if (strcmp(name, "SABRE_TextureActor") != 0)
    {
        DEBUG_MSG_FROM("This function must be called from an event of SABRE_TextureActor", "checkTextureNameChanges");
        return 1; // Function called from invalid event
    }

    f = fopen(SWE_TEXTURE_CONF_FILE, "r");
    if (!f) return 2; // Couldn't open file

    sprintf(format, "%%*04d %%%ds\n", SWE_TEXTURE_NAME_MAX_LENGTH);

    while (texNameLen)
    {
        if (fscanf(f, format, readName) != 2 || strcmp(getAnimName(texIndex), readName) != 0)
        {
            sprintf(temp, "Texture name: %s,\tName read from file: %s", getAnimName(texIndex), readName);
            DEBUG_MSG_FROM(temp, "checkTextureNameChanges");

            fclose(f);
            return -texIndex; // Texture name wasn't found in the file
        }

        texNameLen = strlen(getAnimName(++texIndex));
    }


    fclose(f);
    return 0;
}

// Must be called from an event of SABRE_TextureActor
int printTextureNames()
{
    FILE *f;
    int texIndex = 2; // Ignore project-management animation and "missing texture" texture
    size_t texNameLen = strlen(getAnimName(texIndex));

    if (strcmp(name, "SABRE_TextureActor") != 0)
    {
        DEBUG_MSG_FROM("This function must be called from an event of SABRE_TextureActor", "printTextureNames");
        return 1; // Function called from invalid event
    }

    f = fopen(SWE_TEXTURE_CONF_FILE, "w");
    if (!f) return 2; // Couldn't open file

    while (texNameLen)
    {
        fprintf(f, "%04d %s\n", texIndex - 1, getAnimName(texIndex));
        texNameLen = strlen(getAnimName(++texIndex));
    }

    fclose(f);
    return 0;
}
