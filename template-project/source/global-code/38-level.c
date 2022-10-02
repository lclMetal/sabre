const unsigned long SABRE_NORMAL    = 0;
const unsigned long SABRE_IS_WINDOW = (1 << 0);

#ifndef SABRE_RENDER_OBJECT_DEFINED
typedef struct SABRE_RenderObjectStruct
{
    float sortValue;
    enum SABRE_RenderObjectTypeEnum objectType;

    float scale;
    int width;
    int height;
    float verticalOffset;
    int horizontalPosition;
    int horizontalScalingCompensation;
    SABRE_Slice slice;

    struct SABRE_RenderObjectStruct *prev;
    struct SABRE_RenderObjectStruct *next;
}SABRE_RenderObject;
#define SABRE_RENDER_OBJECT_DEFINED
#endif

#ifndef SABRE_LEVEL_DEFINED
#define SABRE_MAX_LEVEL_EDGE_WRAP_DEPTH 5
typedef struct SABRE_LevelTileStruct
{
    unsigned texture;
    unsigned long properties;
    struct SABRE_RenderObjectStruct *renderObject[SABRE_MAX_LEVEL_EDGE_WRAP_DEPTH + 1];
}SABRE_LevelTile;

typedef struct SABRE_LevelStruct
{
    char validated;

    unsigned width;
    unsigned height;
    SABRE_LevelTile *map;
}SABRE_Level;
#define SABRE_LEVEL_DEFINED
#endif

// from texture.c
int SABRE_ValidateTextureIndex(int index);
int SABRE_IsWindowTexture(int index);

SABRE_Level SABRE_level;

int SABRE_FreeLevel();
int SABRE_PrintLevel(SABRE_Level *level);
int SABRE_InitLevel(SABRE_Level *level, unsigned width, unsigned height);
int SABRE_FreeLevelData(SABRE_Level *level);
int SABRE_AllocLevel(SABRE_Level *level);
int SABRE_ValidateLevel(SABRE_Level *level);
int SABRE_ValidateCurrentLevel();
int SABRE_SetLevelDataFromArray(SABRE_Level *level, unsigned width, unsigned height, unsigned arr[]);

int  SABRE_PrintLevel(SABRE_Level *level)
{
    unsigned i, j;
    char temp[256] = "";
    char temp2[256] = "";
    unsigned width = 0;
    unsigned height = 0;

    if (!level) return 1; // 1: invalid pointer
    if (!level->map) return 2; // 2: no map data to free

    width = level->width;
    height = level->height;

    DEBUG_MSG_FROM("Printing level:", "SABRE_PrintLevel");

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            sprintf(temp2, "%d, ", level->map[i * width + j].texture);
            strcat(temp, temp2);
        }

        DEBUG_MSG_FROM(temp, "SABRE_PrintLevel");
        strcpy(temp, "");
    }

    return 0;
}

int SABRE_InitLevel(SABRE_Level *level, unsigned width, unsigned height)
{
    unsigned levelSize = width * height;
    if (!level) return 1; // 1: invalid pointer
    if (levelSize < min(width, height)) return 2; // 2: detected unsigned integer wrap around
                                                  // e.g. width and/or height is too big (very unlikely :P)

    level->validated = 0;
    level->width = width;
    level->height = height;
    level->map = NULL;

    return 0;
}

int SABRE_FreeLevel()
{
    return SABRE_FreeLevelData(&SABRE_level);
}

int SABRE_FreeLevelData(SABRE_Level *level)
{
    if (!level) return 1; // 1: invalid pointer
    if (!level->map) return 2; // 2: no map data to free

    free(level->map);
    level->map = NULL;

    return 0;
}

int SABRE_AllocLevel(SABRE_Level *level)
{
    unsigned mapSize;

    if (!level) return 1; // 1: invalid pointer

    SABRE_FreeLevel(level);

    mapSize = level->width * level->height;
    level->map = malloc(mapSize * (sizeof *(level->map)));

    if (!level->map) return 2; // 2: allocation failed

    return 0;
}

// Iterate over the map and validate each tile's texture against the textureStore contents.
// Any invalid texture values are modified to the value representing a missing texture.
// Returns a value indicating whether invalid textures were found (1) or not (0).
int SABRE_ValidateLevel(SABRE_Level *level)
{
    int result = 0;
    size_t index = 0;
    unsigned i, j;
    unsigned textureIndex = 0;
    unsigned width = level->width;
    unsigned height = level->height;
    unsigned validatedIndex = 0;

    if (level->validated)
        return 0; // 0: level already validated

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            index = i * width + j;

            textureIndex = level->map[index].texture;

            if (textureIndex == 0) // current tile is empty
            {
                level->map[index].properties = SABRE_NORMAL;
                continue;
            }

            validatedIndex = SABRE_ValidateTextureIndex(textureIndex);
            level->map[index].properties = (SABRE_IsWindowTexture(validatedIndex)) ? SABRE_IS_WINDOW : SABRE_NORMAL;
            level->map[index].texture = validatedIndex + 1; // offset by one because the first texture index
                                                            // is reserved for the "texture missing" texture

            if (validatedIndex == 0)
                result = 1; // 1: level has at least one invalid texture index
        }
    }

    level->validated = 1;

    return result; // 0: no errors, 1: see above
}

int SABRE_ValidateCurrentLevel()
{
    return SABRE_ValidateLevel(&SABRE_level);
}

int SABRE_SetLevelDataFromArray(SABRE_Level *level, unsigned width, unsigned height, unsigned arr[])
{
    if (!level) return 1; // 1: invalid pointer
    if (!arr) return 2; // 2: invalid map data array pointer

    SABRE_FreeLevel(level);
    if (SABRE_InitLevel(level, width, height) == 2) return 3; // invalid map dimensions

    if (SABRE_AllocLevel(level) == 0)
    {
        unsigned i, j, k;
        size_t index = 0;

        for (i = 0; i < height; i++)
        {
            for (j = 0; j < width; j++)
            {
                index = i * width + j;

                level->map[index].properties = SABRE_NORMAL;
                level->map[index].texture = arr[index];

                for (k = 0; k <= SABRE_MAX_LEVEL_EDGE_WRAP_DEPTH; k++)
                {
                    level->map[index].renderObject[k] = NULL;
                }
            }
        }

        return 0;
    }

    return 3; // 3: level allocation failed
}

int SABRE_SetLevelDataFrom2DIntArray(SABRE_Level *level, unsigned width, unsigned height, int **arr)
{
    if (!level) return 1; // 1: invalid pointer
    if (!arr) return 2; // 2: invalid map data array pointer

    SABRE_FreeLevel(level);
    if (SABRE_InitLevel(level, width, height) == 2) return 3; // invalid map dimensions

    if (SABRE_AllocLevel(level) == 0)
    {
        unsigned i, j, k;
        size_t index = 0;

        for (i = 0; i < height; i++)
        {
            for (j = 0; j < width; j++)
            {
                index = i * width + j;

                level->map[index].properties = SABRE_NORMAL;
                level->map[index].texture = arr[i][j];

                for (k = 0; k <= SABRE_MAX_LEVEL_EDGE_WRAP_DEPTH; k++)
                {
                    level->map[index].renderObject[k] = NULL;
                }
            }
        }

        return 0;
    }

    return 3; // 3: level allocation failed
}
