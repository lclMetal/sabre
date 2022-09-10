#ifndef SABRE_RENDER_OBJECT_DEFINED
typedef struct SABRE_RenderObjectStruct
{
    float sortValue;
    enum SABRE_RenderObjectTypeEnum objectType;

    float scale;
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
typedef struct SABRE_LevelTileStruct
{
    unsigned texture;
    struct SABRE_RenderObjectStruct *renderObject;
}SABRE_LevelTile;

typedef struct SABRE_LevelStruct
{
    unsigned width;
    unsigned height;
    SABRE_LevelTile *map;
}SABRE_Level;
#define SABRE_LEVEL_DEFINED
#endif

SABRE_Level SABRE_level;

int SABRE_FreeLevel();
int SABRE_InitLevel(SABRE_Level *level, unsigned width, unsigned height);
int SABRE_FreeLevelData(SABRE_Level *level);
int SABRE_AllocLevel(SABRE_Level *level);
int SABRE_SetLevelDataFromArray(SABRE_Level *level, unsigned width, unsigned height, unsigned arr[]);

int SABRE_InitLevel(SABRE_Level *level, unsigned width, unsigned height)
{
    unsigned levelSize = width * height;
    if (!level) return 1; // 1: invalid pointer
    if (levelSize < min(width, height)) return 2; // 2: detected unsigned integer wrap around
                                                  // e.g. width and/or height is too big (very unlikely :P)

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

int SABRE_SetLevelDataFromArray(SABRE_Level *level, unsigned width, unsigned height, unsigned arr[])
{
    if (!level) return 1; // 1: invalid pointer
    if (!arr) return 2; // 2: invalid map data array pointer

    SABRE_FreeLevel(level);
    if (SABRE_InitLevel(level, width, height) == 2) return 3; // invalid map dimensions

    if (SABRE_AllocLevel(level) == 0)
    {
        unsigned i, j;

        for (i = 0; i < height; i++)
        {
            for (j = 0; j < width; j++)
            {
                level->map[i * width + j].texture = arr[i * width + j];
                level->map[i * width + j].renderObject = NULL;
            }
        }

        return 0;
    }

    return 3; // 3: level allocation failed
}
