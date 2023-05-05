#define SABRE_VERSION_STR_LEN 50

typedef char SABRE_SWEVersionString[SABRE_VERSION_STR_LEN];

typedef struct SABRE_LevelDataItemSetStruct
{
    int count;
    size_t itemSize;
    long int offset;
}SABRE_LevelDataItemSet;

typedef struct SABRE_LevelHeaderStruct
{
    Color floorColor;
    Color ceilingColor;

    SABRE_Vector2 playerSpawnPoint;

    SABRE_LevelDataItemSet textureData;
    SABRE_LevelDataItemSet spriteData;
    SABRE_LevelDataItemSet entityData;
    SABRE_LevelDataItemSet triggerData;

    int levelWidth;
    int levelHeight;
    long int levelTileDataOffset;
}SABRE_LevelHeader;

SABRE_LevelHeader emptyHeader = { 0 };
SABRE_SWEVersionString sweVersion = "SWE_v0.0.1";

size_t writeLevelHeader(FILE *file, SABRE_LevelHeader *header)
{
    return fwrite(header, sizeof *header, 1, file);
}

size_t readLevelHeader(FILE *file, SABRE_LevelHeader *header)
{
    return fread(header, sizeof *header, 1, file);
}

void setLevelHeaderColorData(SABRE_LevelHeader *header, Color floorColor, Color ceilingColor)
{
    header->floorColor = floorColor;
    header->ceilingColor = ceilingColor;
}

void setLevelHeaderSpawnPoint(SABRE_LevelHeader *header, SABRE_Vector2 playerSpawnPoint)
{
    header->playerSpawnPoint = playerSpawnPoint;
}

void setLevelDataItemSetData(SABRE_LevelDataItemSet *dataItemSet, int count, size_t itemSize, long int offset)
{
    dataItemSet->count = count;
    dataItemSet->itemSize = itemSize;
    dataItemSet->offset = offset;
}

void setLevelHeaderLevelTileData(SABRE_LevelHeader *header, int levelWidth, int levelHeight, long int offset)
{
    header->levelWidth = levelWidth;
    header->levelHeight = levelHeight;
    header->levelTileDataOffset = offset;
}

size_t writeTextureData(FILE *file, size_t count, SABRE_Texture *textures)
{
    return fwrite(textures, sizeof *textures, count, file);
}

size_t writeSpriteData(FILE *file, size_t count, SABRE_Sprite *sprites)
{
    return fwrite(sprites, sizeof *sprites, count, file);
}

size_t writeEntityData(FILE *file, SABRE_List *entities)
{
    size_t count = 0;
    size_t prevCount = 0;
    SABRE_List *iterator = NULL;

    for (iterator = entities; iterator != NULL; iterator = iterator->next)
    {
        count += fwrite(&iterator->data.entity, sizeof iterator->data.entity, 1, file);
        if (count == prevCount)
        {
            return count;
        }
        prevCount = count;
    }

    return count;
}

int writeCurrentLevelProjectData(const char *fileName)
{
    int err = 0;
    int stepCount = 0;
    long int headerStartPos = 0;
    size_t writeCount = 0;
    SABRE_LevelHeader header = emptyHeader;
    FILE *f;

    if (SABRE_gameState != SABRE_INITIALIZED && SABRE_gameState != SABRE_RUNNING)
    {
        DEBUG_MSG_FROM("SABRE not initialized, can't write level project data!", "writeCurrentLevelProjectData");
        return -1;
    }

    f = fopen(fileName, "wb");

    if (f)
    {
        do
        {
            stepCount++;
            writeCount = fwrite(&sweVersion, sizeof sweVersion, 1, f);
            if (writeCount != 1)
                break;

            fwrite("hello", 5, 1, f); // TESTTESTETSESTESTESTESTESTESTESTESTESTESTTESTTESTTEST

            stepCount++;
            headerStartPos = ftell(f);
            writeCount = writeLevelHeader(f, &emptyHeader);
            if (writeCount != 1)
                break;

            fwrite("hejjo", 5, 1, f); // TESTTESTETSESTESTESTESTESTESTESTESTESTESTTESTTESTTEST

            stepCount++;
            setLevelDataItemSetData(&header.textureData, SABRE_textureStore.count, sizeof SABRE_textures[0], ftell(f));
            writeCount = writeTextureData(f, SABRE_textureStore.count, &SABRE_textures[0]);
            if (writeCount != SABRE_textureStore.count)
                break;

            fwrite("hebbo", 5, 1, f); // TESTTESTETSESTESTESTESTESTESTESTESTESTESTTESTTESTTEST

            stepCount++;
            setLevelDataItemSetData(&header.spriteData, SABRE_spriteStore.count, sizeof SABRE_sprites[0], ftell(f));
            writeCount = writeSpriteData(f, SABRE_spriteStore.count, &SABRE_sprites[0]);
            if (writeCount != SABRE_spriteStore.count)
                break;

            fwrite("hewwo", 5, 1, f); // TESTTESTETSESTESTESTESTESTESTESTESTESTESTTESTTESTTEST

            stepCount++;
            setLevelDataItemSetData(&header.entityData, SABRE_CountEntitiesInList(), sizeof (SABRE_Entity), ftell(f));
            writeCount = writeEntityData(f, SABRE_entities);
            if (writeCount != SABRE_CountEntitiesInList())
                break;

            fwrite("heggo", 5, 1, f); // TESTTESTETSESTESTESTESTESTESTESTESTESTESTTESTTESTTEST

            stepCount++;
            setLevelHeaderColorData(&header, SABRE_defaultFloor, SABRE_defaultCeiling);
            setLevelHeaderSpawnPoint(&header, SABRE_CreateVector2(0, 0));
            fseek(f, headerStartPos, SEEK_SET);
            writeCount = writeLevelHeader(f, &header);
            if (writeCount != 1)
                break;

            fwrite("herro", 5, 1, f); // TESTTESTETSESTESTESTESTESTESTESTESTESTESTTESTTESTTEST

            stepCount = 0;
        }while (0);

        if (stepCount)
            err = stepCount + 1;

        fclose(f);
    }
    else
    {
        err = 1;
    }

    switch (err)
    {
        case 0: break;
        case 1: DEBUG_MSG_FROM("Could not create file!", "writeCurrentLevelProjectData");                       break;
        case 2: DEBUG_MSG_FROM("SWE version write failed!", "writeCurrentLevelProjectData");                    break;
        case 3: DEBUG_MSG_FROM("Level header initialization write failed!", "writeCurrentLevelProjectData");    break;
        case 4: DEBUG_MSG_FROM("Texture data write failed!", "writeCurrentLevelProjectData");                   break;
        case 5: DEBUG_MSG_FROM("Sprite data write failed!", "writeCurrentLevelProjectData");                    break;
        case 6: DEBUG_MSG_FROM("Entity data write failed!", "writeCurrentLevelProjectData");                    break;
        case 7: DEBUG_MSG_FROM("Level header data write failed!", "writeCurrentLevelProjectData");              break;
    }

    return err;
}


void saveTextureConfigs(FILE *file)
{
    fwrite(&SABRE_textures[0], sizeof SABRE_textures[0], SABRE_textureStore.count, file);
}

void loadTextureConfigs(FILE *file)
{
    fread(&SABRE_textures[0], sizeof SABRE_textures[0], SABRE_textureStore.count, file);
}

void saveProjectData(const char *fileName)
{
    FILE *f = fopen(fileName, "wb");

    if (f)
    {
        saveTextureConfigs(f);
        fclose(f);
    }
}

void loadProjectData(const char *fileName)
{
    FILE *f = fopen(fileName, "rb");

    if (f)
    {
        loadTextureConfigs(f);
        fclose(f);
    }
}
