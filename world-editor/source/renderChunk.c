#define X 0
#define Y 1

#define RENDER_CHUNK_SIZE 16

#define NO_EDGES 0
#define CHUNK_EDGES 1
#define USED_AREA_EDGES 2
#define CHUNK_AND_USED_AREA_EDGES 3

int mapWidth = 0;
int mapHeight = 0;

int INT_MIN_VALUE = 0;
int INT_MAX_VALUE = 0;

int blockMinX = 0;
int blockMinY = 0;
int blockMaxX = 0;
int blockMaxY = 0;

double spawnPointPosition[2];

int** testMap = NULL;
int** edgeTestMap = NULL;

int renderChunkCount = 0;

typedef struct SWE_EntityTemplateStruct
{
    int sprite;
    float radius;
    unsigned int attributes;
    char name[256];
}SWE_EntityTemplate;

typedef struct SWE_EntityStruct
{
    SWE_EntityTemplate *template;
    struct SWE_EntityStruct *next;
}SWE_Entity;

typedef struct RenderChunkStruct
{
    int x;
    int y;
    int endX;
    int endY;

    int usedStartX;
    int usedStartY;
    int usedEndX;
    int usedEndY;

    int chunkArray[RENDER_CHUNK_SIZE][RENDER_CHUNK_SIZE];
    SWE_Entity *entities;
    struct RenderChunkStruct *next;
}RenderChunk;

RenderChunk *renderChunkHead;
RenderChunk *renderChunkTail;

int exportMapData(void);
void initializeVariables();
void initializeRenderChunk(RenderChunk *this);
void setRenderChunkUsedArea(int newX, int newY, RenderChunk *ptr);
void updateRenderChunkUsedArea(RenderChunk *ptr);
void coordsToRenderChunkCoords(int blockX, int blockY, int renderChunkCoords[2]);
void coordsToMap(double coordX, double coordY, double mapCoords[2]);
void coordsFromMap(double coordX, double coordY, int worldCoords[2]);
RenderChunk *createRenderChunkList(int newX, int newY);
RenderChunk *addRenderChunk(int newX, int newY);
RenderChunk *searchRenderChunk(int chunkX, int chunkY, RenderChunk **prev);
RenderChunk *searchRenderChunkByCoords(int chunkX, int chunkY);
int deleteRenderChunk(int chunkX, int chunkY);
int deleteRenderChunkList();
void plotBlock(int blockX, int blockY, int blockTexture);
void deleteBlock(int blockX, int blockY);
void updateMapDimensions();
int convertChunkDataToMap();
void freeMap();
void testMapNode(int nodeX, int nodeY);
int testMapEdges();
void freeEdgeTestMap();

int exportMapData(void)
{
    FILE *f = NULL;

    if (!testMap) return 1;

    f = fopen("level.c", "w+");

    if (f)
    {
        int i, j;

        fprintf(f, "#define mapWidth %d\n", mapHeight/*mapWidth*/);
        fprintf(f, "#define mapHeight %d\n\n", mapWidth/*mapHeight*/);
        fprintf(f, "int worldMap[mapWidth][mapHeight] =\n{\n");

        for (i = 0; i < mapHeight; i ++)
        {
            fprintf(f, "    ");
            for (j = 0; j < mapWidth; j ++)
            {
                fprintf(f, "%2d%c", testMap[i][j],
                    (i == mapHeight - 1 && j == mapWidth -1)?' ':',');
            }
            fprintf(f, "\n");
        }
        fprintf(f, "};\n");
        fclose(f);

        return 0;
    }
    else return 2;
}

//This function calculates the minimum and maximum values for an integer variable
//and then initializes the edge coordinates to the smallest possible values
//so that when actual blocks are added, they will take over as the blocks that define
//the map's dimensions
void initializeVariables()
{
    // algorithm from: https://www.geeksforgeeks.org/computing-int_max-int_min-bitwise-operations/
    unsigned int notZero = ~0;
    INT_MAX_VALUE = notZero = notZero >> 1;
    INT_MIN_VALUE = ~notZero;

    blockMinX = INT_MAX_VALUE;
    blockMaxX = INT_MIN_VALUE;
    blockMinY = INT_MAX_VALUE;
    blockMaxY = INT_MIN_VALUE;
}

//This function initializes all the values of a created render chunk, most importantly setting
//its chunkArray to contain only 0's
//this - the pointer to the render chunk to be initialized
void initializeRenderChunk(RenderChunk *this)
{
    int i, j;
    char temp[256];

    this->endX = this->x + RENDER_CHUNK_SIZE - 1;
    this->endY = this->y + RENDER_CHUNK_SIZE - 1;

    this->usedStartX = this->usedStartY = RENDER_CHUNK_SIZE - 1;
    this->usedEndX = this->usedEndY = 0;

    this->entities = NULL;
    this->next = NULL;

    renderChunkCount ++;

    for (i = 0; i < RENDER_CHUNK_SIZE; i ++)
    {
        for (j = 0; j < RENDER_CHUNK_SIZE; j ++)
        {
            this->chunkArray[i][j] = 0;
        }
    }
}

//This function sets the values to the variables that define which area of the RenderChunk is
//actually being used, i.e. contains blocks
//newX - the x coordinate of the new block to be added to the render chunk
//newY - the y coordinate of the new block to be added to the render chunk
//ptr - the pointer to the render chunk
void setRenderChunkUsedArea(int newX, int newY, RenderChunk *ptr)
{
    if (newX < ptr->usedStartX)ptr->usedStartX = newX;
    if (newY < ptr->usedStartY)ptr->usedStartY = newY;
    if (newX > ptr->usedEndX)ptr->usedEndX = newX;
    if (newY > ptr->usedEndY)ptr->usedEndY = newY;
}

//This function updates the values of the variables that define which area of the render chunk is
//actually being used, i.e. contains blocks
//ptr - the pointer to the render chunk
void updateRenderChunkUsedArea(RenderChunk *ptr)
{
    int i, j;
    int startX;
    int startY;
    int endX;
    int endY;

    startX = startY = RENDER_CHUNK_SIZE - 1;
    endX = endY = 0;

    for (i = ptr->usedStartY; i <= ptr->usedEndY; i ++)
    {
        for (j = ptr->usedStartX; j <= ptr->usedEndX; j ++)
        {
            if (ptr->chunkArray[i][j] > 0)
            {
                if (j < startX)startX = j;
                if (j > endX)endX = j;
                if (i < startY)startY = i;
                if (i > endY)endY = i;
            }
        }
    }

    if (startX == RENDER_CHUNK_SIZE - 1 && endX == 0 &&
        startY == RENDER_CHUNK_SIZE - 1 && endY == 0 &&
        ptr->entities == NULL)
    {
        deleteRenderChunk(ptr->x, ptr->y);
    }
    else
    {
        ptr->usedStartX = startX;
        ptr->usedEndX = endX;
        ptr->usedStartY = startY;
        ptr->usedEndY = endY;
    }
}

//This function finds the relevant render chunk's coordinates for a given point on the world grid
//and stores the coordinates to the integer array pointed to by renderChunkCoords
//blockX - the x coordinate whose relevant render chunk's coordinates are to be found
//blockY - the y coordinate whose relevant render chunk's coordinates are to be found
//renderChunkCoords - the pointer to the integer array where the values are to be stored
void coordsToRenderChunkCoords(int blockX, int blockY, int renderChunkCoords[2])
{
    renderChunkCoords[X] = floor((blockX - ((RENDER_CHUNK_SIZE - 1) * (blockX < 0))) /
             RENDER_CHUNK_SIZE)*RENDER_CHUNK_SIZE;
    renderChunkCoords[Y] = floor((blockY - ((RENDER_CHUNK_SIZE - 1) * (blockY < 0))) /
             RENDER_CHUNK_SIZE)*RENDER_CHUNK_SIZE;
}

//This function transforms world coordinates to coordinates on the map and stores the
//map coordinates to the double array pointed to by mapCoords
//coordX - the x coordinate to transform to map coordinate system
//coordY - the y coordinate to transfrom to map coordinate system
//mapCoords - the pointer to the double array where the values are to be stored
void coordsToMap(double coordX, double coordY, double mapCoords[2])
{
    if (testMap != NULL)
    {
        updateMapDimensions();
        mapCoords[X] = coordX - (double)blockMinX + 0.5;
        mapCoords[Y] = coordY - (double)blockMinY + 0.5;
    }
    else
    {
        mapCoords[X] = mapCoords[Y] = 0;
    }
}

//This function transforms map coordinates to world coordinates and stores the world
//coordinates to the integer array pointed to by worldCoords
//coordX - the x coordinate to transform to world coordinate system
//coordY - the y coordinate to transform to world coordinate system
//worldCoords - the pointer to the integer array where the values are to be stored
void coordsFromMap(double coordX, double coordY, int worldCoords[2])
{
    if (testMap != NULL)
    {
        updateMapDimensions();
        worldCoords[X] = floor(coordX) + blockMinX;
        worldCoords[Y] = floor(coordY) + blockMinY;
    }
    else
    {
        worldCoords[X] = worldCoords[Y] = 0;
    }
}

//This function creates the linked list for the render chunks to be stored in
//newX - the x position of the render chunk that will create the list
//newY - the y position of the render chunk that will create the list
RenderChunk *createRenderChunkList(int newX, int newY)
{
    RenderChunk *ptr = malloc(sizeof *ptr);

    if (ptr == NULL)
    {
        showExceptionMessage(ERROR, "Not enough memory, createChunkList failed!");
        return NULL;
    }

    ptr->x = newX;
    ptr->y = newY;
    initializeRenderChunk(ptr);
    ptr->next = NULL;

    renderChunkHead = renderChunkTail = ptr;

    return ptr;
}

//This function adds a render chunk to the linked list
//newX - the x position of the render chunk to be added to the list
//newY - the y position of the render chunk to be added to the list
RenderChunk *addRenderChunk(int newX, int newY)
{
    RenderChunk *ptr;

    if (renderChunkHead == NULL)return createRenderChunkList(newX, newY);

    ptr = malloc(sizeof *ptr);

    if (ptr == NULL)
    {
        showExceptionMessage(ERROR, "Not enough memory, addRenderChunk failed!");
        return NULL;
    }

    ptr->x = newX;
    ptr->y = newY;
    initializeRenderChunk(ptr);
    ptr->next = NULL;

    renderChunkTail->next = ptr;
    renderChunkTail = ptr;

    return ptr;
}

//This function searches the list for a render chunk by its coordinates and also gives a pointer
//to the render chunk in the list before the render chunk being searched
//(which is handy when deleting render chunks from the list)
//chunkX - the x coordinate of the render chunk we want to find
//chunkY - the y coordinate of the render chunk we want to find
//prev - a pointer to a pointer to a render chunk, the address of the render chunk in the list
//       before the render chunk being searched will be stored to the pointer this pointer
//       points to
RenderChunk *searchRenderChunk(int chunkX, int chunkY, RenderChunk **prev)
{
    RenderChunk *ptr = renderChunkHead;
    RenderChunk *temp = NULL;

    while (ptr != NULL)
    {
        if (ptr->x == chunkX && ptr->y == chunkY)
        {
            if (prev)*prev = temp;
            return ptr;
        }
        else
        {
            temp = ptr;
            ptr = ptr->next;
        }
    }

    return NULL;
}

//This function searches the list for a render chunk by its coordinates
//chunkX - the x coordinate of the render chunk we want to find
//chunkY - the y coordinate of the render chunk we want to find
RenderChunk *searchRenderChunkByCoords(int chunkX, int chunkY)
{
    RenderChunk *ptr = renderChunkHead;

    while (ptr != NULL)
    {
        if (ptr->x == chunkX && ptr->y == chunkY)
        {
            return ptr;
        }
        ptr = ptr->next;
    }

    return NULL;
}

void deleteEntitiesInRenderChunk(RenderChunk *chunk)
{
    SWE_Entity *ptr = NULL;
    SWE_Entity *next = NULL;

    if (chunk)
    {
        ptr = chunk->entities;

        while (ptr != NULL)
        {
            next = ptr->next;
            free(ptr);
            ptr = next;
        }

        chunk->entities = NULL;
    }
}

//This function deletes a render chunk identified by its coordinates
//chunkX - the x coordinate of the render chunk to be deleted
//chunkY - the y coordinate of the render chunk to be deleted
int deleteRenderChunk(int chunkX, int chunkY)
{
    RenderChunk *prev = NULL;
    RenderChunk *del = NULL;

    del = searchRenderChunk(chunkX, chunkY, &prev);

    if (del == NULL)return - 1;
    else
    {
        if (prev != NULL)prev->next = del->next;
        if (del == renderChunkTail)
        {
            if (prev != NULL)
            {
                renderChunkTail = prev;
            }
            else
            {
                renderChunkHead = NULL;
                renderChunkTail = NULL;
            }
        }
        else if (del == renderChunkHead)
        {
            if (del->next)renderChunkHead = del->next;
            else {renderChunkHead = NULL; renderChunkTail = NULL;}
        }
    }

    deleteEntitiesInRenderChunk(del);
    free(del);
    del = NULL;
    renderChunkCount --;

    return 1;
}

//This function deletes all render chunks in the list
int deleteRenderChunkList()
{
    RenderChunk *ptr = renderChunkHead;
    RenderChunk *temp = NULL;

    while (ptr != NULL)
    {
        temp = ptr->next;
        deleteRenderChunk(ptr->x, ptr->y);
        ptr = temp;
    }

    renderChunkHead = renderChunkTail = NULL;

    return 1;
}

//This function add an entity to the world and stores it to the correct render chunk
//entX - the x coordinate for the entity
//entY - the y coordinate for the entity
//entity - pointer to the entity template
void plotEntity(float entX, float entY, SWE_EntityTemplate *entity)
{
    RenderChunk *ptr;
    int chunkCoords[2];

    coordsToRenderChunkCoords((int)entX, (int)entY, chunkCoords);

    ptr = searchRenderChunkByCoords(chunkCoords[X], chunkCoords[Y]);

    if (ptr)
    {
        if (ptr->entities == NULL) // CONTINUE FROM HERE!!!!
        {
            
        }
    }
}

//This function adds a block to the world grid and stores it to the correct render chunk
//blockX - the x coordinate for the block
//blockY - the y coordinate for the block
//blockTexture - the texture of the block to be added
void plotBlock(int blockX, int blockY, int blockTexture)
{
    RenderChunk *ptr;
    int chunkCoords[2];
    int relativeX;
    int relativeY;

    coordsToRenderChunkCoords(blockX, blockY, chunkCoords);

    ptr = searchRenderChunkByCoords(chunkCoords[X], chunkCoords[Y]);

    if (ptr)
    {
        relativeX = abs(blockX - ptr->x);
        relativeY = abs(blockY - ptr->y);
        ptr->chunkArray[relativeY][relativeX] = blockTexture;
        setRenderChunkUsedArea(relativeX, relativeY, ptr);
    }
    else
    {
        ptr = addRenderChunk(chunkCoords[X], chunkCoords[Y]);
        if (ptr != NULL)
        {
            relativeX = abs(blockX - ptr->x);
            relativeY = abs(blockY - ptr->y);
            ptr->chunkArray[relativeY][relativeX] = blockTexture;
            setRenderChunkUsedArea(relativeX, relativeY, ptr);
        }
        else
        showExceptionMessage(ERROR, "Not enough memory, plotBlock failed!");
    }
}

//This function deletes a block from the world grid and from the render chunk - if the render chunk
//is left with no blocks in it, it will be deleted
//blockX - the x coordinate from which a block should be erased
//blockY - the y coordinate from which a block should be erased
void deleteBlock(int blockX, int blockY)
{
    RenderChunk *ptr;
    int chunkCoords[2];
    int relativeX;
    int relativeY;

    coordsToRenderChunkCoords(blockX, blockY, chunkCoords);

    ptr = searchRenderChunkByCoords(chunkCoords[X], chunkCoords[Y]);

    if (ptr)
    {
        relativeX = abs(blockX - ptr->x);
        relativeY = abs(blockY - ptr->y);
        ptr->chunkArray[relativeY][relativeX] = 0;
        updateRenderChunkUsedArea(ptr);
    }
}

//This function updates the variables that hold the information of the
//biggest and smallest x and y coordinates used by blocks and then calculates the dimensions of
//the map with those values and updates the variables mapWidth and mapHeight
void updateMapDimensions()
{
    RenderChunk *ptr = renderChunkHead;

    int minX = INT_MAX_VALUE;
    int minY = INT_MAX_VALUE;
    int maxX = INT_MIN_VALUE;
    int maxY = INT_MIN_VALUE;

    while (ptr != NULL)
    {
        minX = min(minX, ptr->x + ptr->usedStartX);
        minY = min(minY, ptr->y + ptr->usedStartY);
        maxX = max(maxX, ptr->x + ptr->usedEndX);
        maxY = max(maxY, ptr->y + ptr->usedEndY);

        ptr = ptr->next;
    }

    blockMinX = minX;
    blockMinY = minY;
    blockMaxX = maxX;
    blockMaxY = maxY;

    mapWidth = abs(blockMaxX - blockMinX) + 1;
    mapHeight = abs(blockMaxY - blockMinY) + 1;
    sabreMapWidth = mapWidth;
    sabreMapHeight = mapHeight;
}

//This function converts the data stored in the render chunks to a dynamically allocated "2D array"
//pointed to by testMap
int convertChunkDataToMap()
{
    int row;
    int i, j;
    int blockX, blockY;

    RenderChunk *ptr = renderChunkHead;

    if (renderChunkHead == NULL)return 0;

    if (ptr != NULL)
    {
        updateMapDimensions();

        testMap = malloc(mapHeight * sizeof(int *));
        if (testMap == NULL)
        {
            showExceptionMessage(ERROR, "Not enough memory, convertChunkDataToMap failed!");
            return 0;
        }

        for (row = 0; row < mapHeight; row ++)
        {
            testMap[row] = malloc(mapWidth * sizeof(int));
            if (testMap[row] == NULL)
            {
                showExceptionMessage(ERROR, "Not enough memory, convertChunkDataToMap failed!");
                return 0;
            }
        }

        while (ptr != NULL)
        {
            for (i = 0; i < RENDER_CHUNK_SIZE; i ++)
            {
                for (j = 0; j < RENDER_CHUNK_SIZE; j ++)
                {
                    blockX = (ptr->x + j) - blockMinX;
                    blockY = (ptr->y + i) - blockMinY;
                    if (blockX >= 0 && blockX < mapWidth && blockY >= 0 && blockY < mapHeight)
                    {
                        testMap[blockY][blockX] = ptr->chunkArray[i][j];
                    }
                }
            }

            ptr = ptr->next;
        }
    }

    return 1;
}

//This function frees the memory allocated for storing the map
void freeMap()
{
    int row;

    if (testMap != NULL)
    {
        for (row = 0; row < mapHeight; row ++)
        {
            free(testMap[row]);
        }

        free(testMap);
        testMap = NULL;
    }
}

//This function starts from the given coordinates, and recursively searches through the
//edgeTestMap, marking the areas that are accessible for the player by walking around
//(This is used in testMapEdges for finding out if there is a leakage allowing the player
//to walk out of the map area)
//nodeX - the starting x coordinate for the recursion (best is to always start from the player
//        spawn point)
//nodeY - the starting y coordinate for the recursion (best is to always start from the player
//        spawn point)
void testMapNode(int nodeX, int nodeY)
{
    if (!edgeTestMap)return;
    if (nodeX < 0 || nodeX > mapWidth - 1 || nodeY < 0 || nodeY > mapHeight - 1)return;
    if (edgeTestMap[nodeY][nodeX] != 0)return;

    edgeTestMap[nodeY][nodeX] = -1;

    testMapNode(nodeX - 1, nodeY);
    testMapNode(nodeX + 1, nodeY);
    testMapNode(nodeX, nodeY - 1);
    testMapNode(nodeX, nodeY + 1);
}

//This function checks if the boundaries of the level are closed, i.e. there is no way for the
//player to walk out of the map area, which would cause undefined behavior. In case there is a
//leakage like that, the function returns 0 and gives an warning message, in case of an error
//with the memory allocation for the function, the function returns 0 and gives an error message,
//and in case the edges are continuous and there is no leakage, the function returns 1
int testMapEdges()
{
    int row;
    int i, j;
    char temp[256];
    double mapCoords[2];

    if (testMap)
    {
        edgeTestMap = malloc(mapHeight * sizeof(int *));
        if (edgeTestMap == NULL)
        {
            showExceptionMessage(ERROR, "Not enough memory, testMapEdges failed!");
            return 0;
        }

        for (row = 0; row < mapHeight; row ++)
        {
            edgeTestMap[row] = malloc(mapWidth * sizeof(int));
            if (edgeTestMap[row] == NULL)
            {
                showExceptionMessage(ERROR, "Not enough memory, testMapEdges failed!");
                return 0;
            }
        }

        for (i = 0; i < mapHeight; i ++)
        {
            for (j = 0; j < mapWidth; j ++)
            {
                edgeTestMap[i][j] = (testMap[i][j] > 0);
            }
        }

        coordsToMap(spawnPointPosition[X], spawnPointPosition[Y], mapCoords);
        if (mapCoords[X] < 0 || mapCoords[X] > mapWidth ||
            mapCoords[Y] < 0 || mapCoords[Y] > mapHeight)
        {
            showExceptionMessage(WARNING, "Player spawn point is outside of the map edges!");
            freeEdgeTestMap();
            return 0;
        }
        if (edgeTestMap[(int)mapCoords[Y]][(int)mapCoords[X]] != 0)
        {
            showExceptionMessage(WARNING, "Player spawn point is inside a wall!");
            freeEdgeTestMap();
            return 0;
        }
        testMapNode((int)mapCoords[X], (int)mapCoords[Y]);

        for (i = 0; i < mapWidth; i ++)
        {
            if (edgeTestMap[0][i] == -1)
            {
                int coords[2];

                coordsFromMap(i, 0, coords);

                sprintf(temp,
                "Detected a leak in the level edges, edge was accessed at (%i, %i)!",
                coords[X], coords[Y]);
                showExceptionMessage(WARNING, temp);
                freeEdgeTestMap();
                return 0;
            }
            if (edgeTestMap[mapHeight - 1][i] == -1)
            {
                int coords[2];

                coordsFromMap(i, mapHeight - 1, coords);

                sprintf(temp,
                "Detected a leak in the level edges, edge was accessed at (%i, %i)!",
                coords[X], coords[Y]);
                showExceptionMessage(WARNING, temp);
                freeEdgeTestMap();
                return 0;
            }
        }

        for (i = 0; i < mapHeight; i ++)
        {
            if (edgeTestMap[i][0] == -1)
            {
                int coords[2];

                coordsFromMap(0, i, coords);

                sprintf(temp,
                "Detected a leak in the level edges, edge was accessed at (%i, %i)!",
                coords[X], coords[Y]);
                showExceptionMessage(WARNING, temp);
                freeEdgeTestMap();
                return 0;
            }
            if (edgeTestMap[i][mapWidth - 1] == -1)
            {
                int coords[2];

                coordsFromMap(mapWidth - 1, i, coords);

                sprintf(temp,
                "Detected a leak in the level edges, edge was accessed at (%i, %i)!",
                coords[X], coords[Y]);
                showExceptionMessage(WARNING, temp);
                freeEdgeTestMap();
                return 0;
            }
        }
    }

    freeEdgeTestMap();

    return 1;
}

//This function frees the memory allocated for the testing of the edges of the map
void freeEdgeTestMap()
{
    int row;

    if (edgeTestMap != NULL)
    {
        for (row = 0; row < mapHeight; row ++)
        {
            free(edgeTestMap[row]);
        }

        free(edgeTestMap);
        edgeTestMap = NULL;
    }
}
