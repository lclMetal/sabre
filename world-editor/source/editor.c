#define blockSize 20
#define R 0
#define G 1
#define B 2
#define PREVIEW 0
#define CREATE 1
#define OFF 0
#define ON 1

int canvasXmouse;
int canvasYmouse;
int canvasPrevXmouse;
int canvasPrevYmouse;

int textureNumber = - 1;

Color bgColor;
Color gridColor;

void setEditorBackgroundColor(int rValue, int gValue, int bValue);
void setEditorGridColor(int rValue, int gValue, int bValue);
void setGuiColor(int rValue, int gValue, int bValue);
void setGuiTextColor(int rValue, int gValue, int bValue);
void setPositionIndicatorColor(int rValue, int gValue, int bValue);
void addTextureIndicatorColor(int rValue, int gValue, int bValue);
void copyTextureIndicatorColor(int colorIndex);
void setSpawnPointPosition(double spawnX, double spawnY);
void drawGrid();
void testScreenTheater(int mode);
void updateViewEndCoordinates();
void mouseCoords();
void coordsToGrid(int inputX, int inputY, int gridCoords[2]);
void convertToGrid(int coords[2]);
    void coordsToScreen(int inputX, int inputY, int screenCoords[2]);
void convertToScreen(int coords[2]);
void coordsToWorld(int inputX, int inputY, int worldCoords[2]);
void convertToWorld(int coords[2]);
    void coordsFromWorld(int inputX, int inputY, int screenCoords[2]);
void convertFromWorld(int coords[2]);
void calculateScreenDimensions();
void putBlock(int blockX, int blockY, int texture, int mode);
void eraseBlock(int blockX, int blockY);
int getBlock(int blockX, int blockY);
void drawVerticalLine(int y1, int y2, int x1, int texture, int mode);
void drawHorizontalLine(int x1, int x2, int y1, int texture, int mode);
void drawRectangle(int x1, int y1, int x2, int y2, int texture, int mode);
void drawLine(int x1, int y1, int x2, int y2, int texture, int mode);
void drawEllipse(int x1, int y1, int x2, int y2, int texture, int mode);
void drawRenderChunkEdges(RenderChunk *ptr);
void drawRenderChunkUsedAreaEdges(RenderChunk *ptr);
void renderChunks(int mode);

//This function sets the color used for drawing the background of the editor
//rValue - the value for the red component of the color
//gValue - the value for the green component of the color
//bValue - the value for the blue component of the color
void setEditorBackgroundColor(int rValue, int gValue, int bValue)
{
    bgColor.r = rValue;
    bgColor.g = gValue;
    bgColor.b = bValue;
}

//This function sets the color used for drawing the grid
//rValue - the value for the red component of the color
//gValue - the value for the green component of the color
//bValue - the value for the blue component of the color
void setEditorGridColor(int rValue, int gValue, int bValue)
{
    gridColor.r = rValue;
    gridColor.g = gValue;
    gridColor.b = bValue;
}

//This function changes the color of the GUI actors
//rValue - the value for the red component of the color
//gValue - the value for the green component of the color
//bValue - the value for the blue component of the color
void setGuiColor(int rValue, int gValue, int bValue)
{
    Actor *a, *a2, *a3;
    a = getclone("bar");
    a2 = getclone("barTop");
    a3 = getclone("screenWindow");

    a->r = a2->r = a3->r = rValue;
    a->g = a2->g = a3->g = gValue;
    a->b = a2->b = a3->b = bValue;
}

//This function changes the color of the GUI text actors
//rValue - the value for the red component of the color
//gValue - the value for the green component of the color
//bValue - the value for the blue component of the color
void setGuiTextColor(int rValue, int gValue, int bValue)
{
    Actor *a, *a2;
    a = getclone("status");
    a2 = getclone("testWindowText");

    a->r = a2->r = rValue;
    a->g = a2->g = gValue;
    a->b = a2->b = bValue;
}

//This function recolors the actors that indicate the current position
//rValue - the value for the red component of the color
//gValue - the value for the green component of the color
//bValue - the value for the blue component of the color
//void setPositionIndicatorColor(int rValue, int gValue, int bValue)
void setPositionIndicatorColor(int rValue, int gValue, int bValue)
{
    Actor *xInd = getclone("xIndicator");
    Actor *yInd = getclone("yIndicator");

    xInd->r = yInd->r = rValue;
    xInd->g = yInd->g = gValue;
    xInd->b = yInd->b = bValue;
}

//This function adds a new color for indicating a different texture
//rValue - the value for the red component of the color
//gValue - the value for the green component of the color
//bValue - the value for the blue component of the color
void addTextureIndicatorColor(int rValue, int gValue, int bValue)
{
    Actor *a = CreateActor("block", "block", "(none)", "(none)", -470, -310, true);
    a->r = rValue;
    a->g = gValue;
    a->b = bValue;

    textureNumber ++;
}

//This function copies the texture indicator color to the current actor by the color's index
//colorIndex - index of the color to copy
void copyTextureIndicatorColor(int colorIndex)
{
    Actor *a;

    a = gc2("block", colorIndex - 1);

    r = a->r;
    g = a->g;
    b = a->b;
}

//This function sets the spawning point position
//spawnX - the x coordinate of the spawning point
//spawnY - the y coordinate of the spawning point
void setSpawnPointPosition(double spawnX, double spawnY)
{
    spawnPointPosition[X] = spawnX;
    spawnPointPosition[Y] = spawnY;
}

//This function draws the grid scaled to the size of the zoom variable
void drawGrid()
{
    int i;

    int squareSize = blockSize * zoom;

    int wLim = (screenWidth / squareSize);
    int hLim = (screenHeight / squareSize);

    int wFix = (screenWidth % squareSize) / 2;
    int hFix = (screenHeight % squareSize) / 2;

    erase(bgColor.r, bgColor.g, bgColor.b, 0);

    setpen(gridColor.r, gridColor.g, gridColor.b, 0, 1);

    for (i = 0; i <= wLim; i ++)
    {
        moveto(wFix + i * squareSize, 0);
        lineto(wFix + i * squareSize, screenHeight);
    }

    for (i = 0; i <= hLim; i ++)
    {
        moveto(0, hFix + i * squareSize);
        lineto(screenWidth, hFix + i * squareSize);
    }
}

//This function creates the actor that darkens the screen during level testing and disables
//the darkening after testing is finished
//mode - the variable that tells whether to turn the darkening on or off
void testScreenTheater(int mode)
{
    Actor *a = getclone("darkenCanvas");

    switch (mode)
    {
        case OFF:
            a->var = 2;
        break;

        case ON:
            CreateActor("darkenCanvas", "icon", "(none)", "(none)", view.x, view.y, true);
        break;
    }
}

//This function updates the values of the variables viewEndX and viewEndY, which hold the view's
//bottom right corner's coordinates on the world grid
void updateViewEndCoordinates()
{
    int viewEndCoords[2];

    coordsToGrid(screenWidth, screenHeight, viewEndCoords);
    convertToWorld(viewEndCoords);
    viewEndX = viewEndCoords[X];
    viewEndY = viewEndCoords[Y];
}

//This function calculates the mouse coordinates relative to the canvas and stores
//the coordinates to variables canvasXmouse and canvasYmouse
//Before updating the coordinates the previous coordinates are stored
//to variables canvasPrevXmouse and canvasPrevYmouse
void mouseCoords()
{
    canvasPrevXmouse = canvasXmouse;
    canvasPrevYmouse = canvasYmouse;
    canvasXmouse = xmouse - abs(view.x - gridCanvas.x);
    canvasYmouse = ymouse - abs(view.y - gridCanvas.y);
}

//This function converts the given x and y cooridnates to squares on the grid and stores
//the converted values to the integer array pointed to by gridCoords
//inputX - the x coordinate to convert to a square on the grid
//inputY - the y coordinate to convert to a square on the grid
//gridCoords - the pointer to the integer array where the converted values are to be stored
void coordsToGrid(int inputX, int inputY, int gridCoords[2])
{
    int squareSize = blockSize * zoom;

    int wFix = (screenWidth % squareSize) / 2;
    int hFix = (screenHeight % squareSize) / 2;

    int maxXsize = ceil((double)(screenWidth + wFix) / (double)squareSize) - 1 * (!wFix);
    int maxYsize = ceil((double)(screenHeight + hFix) / (double)squareSize) - 1 * (!hFix);

    int tempX = ((inputX - wFix) / squareSize) + (wFix > 0);
    int tempY = ((inputY - hFix) / squareSize) + (hFix > 0);

    if (inputX < wFix)gridCoords[X] = 0;
    else (tempX <= maxXsize)?(gridCoords[X] = tempX):(gridCoords[X] = maxXsize);
    if (inputY < hFix)gridCoords[Y] = 0;
    else (tempY <= maxYsize)?(gridCoords[Y] = tempY):(gridCoords[Y] = maxYsize);
}

//This function converts the given x and y coordinates to squares on the grid
//The function modifies the actual values of the given array, so after using the function, the
//values have changed!
//coords - the array containing the x and y coordinates to be converted to squares of the grid
void convertToGrid(int coords[2])
{
    int squareSize = blockSize * zoom;

    int wFix = (screenWidth % squareSize) / 2;
    int hFix = (screenHeight % squareSize) / 2;

    int maxXsize = ceil((double)(screenWidth + wFix) / (double)squareSize) - 1 * (!wFix);
    int maxYsize = ceil((double)(screenHeight + hFix) / (double)squareSize) - 1 * (!hFix);

    int tempX = ((coords[X] - wFix) / squareSize) + (wFix > 0);
    int tempY = ((coords[Y] - hFix) / squareSize) + (hFix > 0);

    if (coords[X] < wFix)coords[X] = 0;
    else (tempX <= maxXsize)?(coords[X] = tempX):(coords[X] = maxXsize);
    if (coords[Y] < hFix)coords[Y] = 0;
    else (tempY <= maxYsize)?(coords[Y] = tempY):(coords[Y] = maxYsize);
}

//This function converts the given x and y coordinates of a square on the grid to the corresponding
//coordinates on the canvas and stores the converted values to the integer array pointed to by
//screenCords
//inputX - the x coordinate of the square on the grid to be converted to a canvas coordinate
//inputY - the y coordinate of the square on the grid to be converted to a canvas coordinate
//screenCoords - the pointer to the integer array where the converted values are to be stored
void coordsToScreen(int inputX, int inputY, int screenCoords[2])
{
    int squareSize = blockSize * zoom;

    int wFix = (screenWidth % squareSize) / 2;
    int hFix = (screenHeight % squareSize) / 2;

    if (inputX == 0)(wFix > 0)?(screenCoords[X] = wFix - floor(squareSize * 0.5)):(screenCoords[X] = ceil(squareSize * 0.5));
    else screenCoords[X] = wFix + ceil(squareSize * 0.5) + ceil(squareSize * (inputX - (wFix > 0)));
    if (inputY == 0)(hFix > 0)?(screenCoords[Y] = hFix - floor(squareSize * 0.5)):(screenCoords[Y] = ceil(squareSize * 0.5));
    else screenCoords[Y] = hFix + ceil(squareSize * 0.5) + ceil(squareSize * (inputY - (hFix > 0)));
}

//This function converts the given x and y coordinates of a square on the grid to the corresponding
//coordinates on the canvas
//The function modifies the actual values of the given array, so after using the function, the
//values have changed!
//coords - the array containing the x and y coordinates to be converted to coordinates on the canvas
void convertToScreen(int coords[2])
{
    int squareSize = blockSize * zoom;

    int wFix = (screenWidth % squareSize) / 2;
    int hFix = (screenHeight % squareSize) / 2;

    if (coords[X] == 0)(wFix > 0)?(coords[X] = wFix - floor(squareSize * 0.5)):(coords[X] = ceil(squareSize * 0.5));
    else coords[X] = wFix + ceil(squareSize * 0.5) + ceil(squareSize * (coords[X] - (wFix > 0)));
    if (coords[Y] == 0)(hFix > 0)?(coords[Y] = hFix - floor(squareSize * 0.5)):(coords[Y] = ceil(squareSize * 0.5));
    else coords[Y] = hFix + ceil(squareSize * 0.5) + ceil(squareSize * (coords[Y] - (hFix > 0)));
}

//This function converts the given x and y coordinates of a square on the screen grid to the x
//and y coordinates of a square on the world grid and stores the converted values to the integer
//array pointed to by fromWorldCoords
//inputX - the x coordinate of the square on the screen grid to be converted to the world grid
//inputY - the y coordinate of the square on the screen grid to be converted to the world grid
//worldCoords - the pointer to the integer array where the converted values are to be stored
void coordsToWorld(int inputX, int inputY, int worldCoords[2])
{
    worldCoords[X] = inputX + floor(viewX);
    worldCoords[Y] = inputY + floor(viewY);
}

//This function converts the given x and y coordinates of a square on the screen grid to the x
//and y coordinates of a square on the world grid
//The function modifies the actual values of the given array, so after using the function, the
//values have changed!
//coords - the array containing the x and y coordinates to be converted to the world grid
void convertToWorld(int coords[2])
{
    coords[X] = coords[X] + floor(viewX);
    coords[Y] = coords[Y] + floor(viewY);
}

//This function converts the given x and y coordinates of a square on the world grid to the x and y
//coordinates of a square on the screen grid and stores the converted values to the integer array
//pointed to by worldCoords
//inputX - the x coordinate of the square on the world grid to be converted to the screen grid
//inputY - the y coordinate of the square on the world grid to be converted to the screen grid
//screenCoords - the pointer to the integer array where the converted values are to be stored
void coordsFromWorld(int inputX, int inputY, int screenCoords[2])
{
    screenCoords[X] = inputX - floor(viewX);
    screenCoords[Y] = inputY - floor(viewY);
}

//This function converts the given x and y coordinates of a square on the world grid to the x and y
//coordinates of a square on the screen grid
//The function modifies the actual values of the given array, so after using the function, the
//values have changed!
//coords - the array containing the x and y coordinates to be converted to the screen grid
void convertFromWorld(int coords[2])
{
    coords[X] = coords[X] - floor(viewX);
    coords[Y] = coords[Y] - floor(viewY);
}

//This function calculates the dimensions of the area on the gridCanvas actor that is being used
//with the current resolution
void calculateScreenDimensions()
{
    screenWidth = view.width;
    screenHeight = view.height - barTop.height - bar.height;
}

//This function either draws a preview block or adds an actual block to the desired point
//in the world grid
//blockX - the x coordinate for the block
//blockY - the y coordinate for the block
//texture - the texture for the block
//mode - whether to preview a block in that position or to actually add one to that position
void putBlock(int blockX, int blockY, int texture, int mode)
{
    if (mode == CREATE)
    {
        int blockOnGrid[2];
        int blockOnScreen[2];

        plotBlock(blockX, blockY, texture+1);

        coordsFromWorld(blockX, blockY, blockOnGrid);
        coordsToScreen(blockOnGrid[X], blockOnGrid[Y], blockOnScreen);
        if (blockOnGrid[X] >= 0 && blockOnGrid[X] <= width &&
            blockOnGrid[Y] >= 0 &&blockOnGrid[Y] <= height)
        {
            draw_from(gc2("block", texture)->clonename, blockOnScreen[X], blockOnScreen[Y], zoom);
        }
    }
    else if (mode == PREVIEW)
    {
        int blockOnGrid[2];
        int blockOnScreen[2];

        coordsFromWorld(blockX, blockY, blockOnGrid);
        coordsToScreen(blockOnGrid[X], blockOnGrid[Y], blockOnScreen);
        if (blockOnGrid[X] >= 0 && blockOnGrid[X] <= width &&
            blockOnGrid[Y] >= 0 && blockOnGrid[Y] <= height)
        {
            draw_from("previewBlock", blockOnScreen[X], blockOnScreen[Y], zoom);
        }
    }
}

//This function both deletes and erases a given block
//By also erasing the block from the screen instead of just deleting it, a full screen update
//is be avoided
//blockX - the x coordinate of the block to be deleted and erased from the screen
//blockY - the y coordinate of the block to be deleted and erased from the screen
void eraseBlock(int blockX, int blockY)
{
    int blockOnScreen[2];

    if (getBlock(blockX, blockY) > -1)
    {
        deleteBlock(blockX, blockY);

        coordsFromWorld(blockX, blockY, blockOnScreen);
        convertToScreen(blockOnScreen);
        draw_from("transparentBlock", blockOnScreen[X], blockOnScreen[Y], zoom);
    }
}

//This function gets the value of the given block on the world grid and returns it
//If there is no block in that position, returns -1
//blockX - the x coordinate of the block on the world grid
//blockY - the y coordinate of the block on the world grid
int getBlock(int blockX, int blockY)
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
        return ptr->chunkArray[relativeY][relativeX] - 1;
    }

    return -1;
}

//This function draws a vertical line of blocks
//y1 - the y coordinate for the first end of the line
//y2 - the y coordinate for the second end of the line
//x1 - the x coordinate for the line
//texture - the texture to be used for the blocks forming the line
//mode - should the line actually be created or will it just be previewed
void drawVerticalLine(int y1, int y2, int x1, int texture, int mode)
{
    int i;

    for (i = min(y1, y2); i <= max(y1, y2); i ++)
    {
        putBlock(x1, i, texture, mode);
    }
}

//This function draws a horizontal line of blocks
//x1 - the x coordinate for the first end of the line
//x2 - the x coordinate for the second end of the line
//y1 - the y coordinate for the line
//texture - the texture to be used for the blocks forming the line
//mode - should the line actually be created or will it just be previewed
void drawHorizontalLine(int x1, int x2, int y1, int texture, int mode)
{
    int i;

    for (i = min(x1, x2); i <= max(x1, x2); i ++)
    {
        putBlock(i, y1, texture, mode);
    }
}

//This function draws a rectangle of blocks
//x1 - the x coordinate for the top left corner of the rectangle
//y1 - the y coordinate for the top left corner of the rectangle
//x2 - the x coordinate for the bottom right corner of the rectangle
//y2 - the y coordinate for the bottom right corner of the rectangle
//texture - the texture to be used for the blocks forming the rectangle
//mode - should the rectangle actually be created or will it just be previewed
void drawRectangle(int x1, int y1, int x2, int y2, int texture, int mode)
{
    int topLeftX = min(x1, x2);
    int topLeftY = min(y1, y2);
    int bottomRightX = max(x1, x2);
    int bottomRightY = max(y1, y2);

    drawVerticalLine(topLeftY, bottomRightY, topLeftX, texture, mode);
    drawVerticalLine(topLeftY, bottomRightY, bottomRightX, texture, mode);
    drawHorizontalLine(topLeftX, bottomRightX, topLeftY, texture, mode);
    drawHorizontalLine(topLeftX, bottomRightX, bottomRightY, texture, mode);
}

//This function draws a line of blocks between any two points using
//Bresenham's line drawing algorithm
//x1 - the first x coordinate
//y1 - the first y coordiante
//x2 - the second x coordinate
//y2 - the second y coordinate
//texture - the texture to be used for the blocks forming the line
//mode - should the line actually be created or will it just be previewed
void drawLine(int x1, int y1, int x2, int y2, int texture, int mode)
{
    int dX = x2 - x1;
    int dY = y2 - y1;

    int xx;
    int yy;
    int change;

    double err = 0;
    double deltaError;

    if (y1 == y2){drawHorizontalLine(x1, x2, y1, texture, mode); return;}
    if (x1 == x2){drawVerticalLine(y1, y2, x1, texture, mode); return;}

    if (abs(dX) >= abs(dY))
    {
        int changeX = - 1 + (2 * (x2 > x1));
        deltaError = abs(dY) / max(abs(dX), 0.000001);

        yy = y1;
        change = - 1 + (2 * (y2 > y1));

        for (xx = x1; xx != x2 + changeX; xx += changeX)
        {
            putBlock(xx, yy, texture, mode);
            err += deltaError;

            if (err >= 0.5)
            {
                yy += change;
                err -= 1.0;
            }
        }

        return;
    }
    else if (abs(dY) > abs(dX))
    {
        int changeY = - 1 + (2 * (y2 > y1));
        deltaError = abs(dX) / max(abs(dY), 0.000001);

        xx = x1;
        change = - 1 + (2 * (x2 > x1));

        for (yy = y1; yy != y2 + changeY; yy += changeY)
        {
            putBlock(xx, yy, texture, mode);
            err += deltaError;

            if (err >= 0.5)
            {
                xx += change;
                err -= 1.0;
            }
        }

        return;
    }
}

//CURRENTLY UNFINISHED!
//This function draws an ellipse of blocks
//x1 - the x coordinate for the top left corner of the rectangle that limits the ellipse's edges
//y1 - the y coordinate for the top left corner of the rectangle that limits the ellipse's edges
//x2 - the x coordinate for the bottom right corner of the rectangle that limits the ellipse's edges
//y2 - the y coordinate for the bottom right corner of the rectangle that limits the ellipse's edges
//texture - the texture to be used for the blocks forming the ellipse
//mode - should the ellipse actually be created or will it just be previewed
void drawEllipse(int x1, int y1, int x2, int y2, int texture, int mode)
{
    int hDiam = abs(max(x1, x2) - min(x1, x2));
    int vDiam = abs(max(y1, y2) - min(y1, y2));

    int hRadius = round(hDiam * 0.5);
    int vRadius = round(vDiam * 0.5);

    int h2 = (hRadius * 2) * (hRadius * 2);
    int v2 = (vRadius * 2) * (vRadius * 2);

    int fh2 = 4 * h2;
    int fv2 = 4 * v2;

    int midX = min(x1, x2) + hRadius;
    int midY = min(y1, y2) + vRadius;

    int xx, yy, sigma;

    if (x1 == x2 ||y1 == y2)
    {
        putBlock(x1, y1, texture, mode);
        return;
    }

    for (xx = 0, yy = vRadius, sigma = 2 * v2 + h2 * (1 - 2 * vRadius); v2 * xx <= h2 * yy; xx ++)
    {
        putBlock(midX + xx, midY + yy, texture, mode);
        putBlock(midX - xx, midY + yy, texture, mode);
        putBlock(midX + xx, midY - yy, texture, mode);
        putBlock(midX - xx, midY - yy, texture, mode);

        if (sigma >= 0)
        {
            sigma += fh2 * (1 - yy);
            yy --;
        }

        sigma += v2 * ((4 * xx) + 6);
    }

    for (xx = hRadius, yy = 0, sigma = 2 * h2 + v2 * (1 - 2 * hRadius); h2 * yy <= v2 * xx; yy ++)
    {
        putBlock(midX + xx, midY + yy, texture, mode);
        putBlock(midX - xx, midY + yy, texture, mode);
        putBlock(midX + xx, midY - yy, texture, mode);
        putBlock(midX - xx, midY - yy, texture, mode);

        if (sigma >= 0)
        {
            sigma += fv2 * (1 - xx);
            xx --;
        }

        sigma += h2 * ((4 * yy) + 6);
    }
}

void DrawEllipse (int x0, int y0, int width, int height, int texture, int mode)
{
    int a2 = width * width;
    int b2 = height * height;
    int fa2 = 4 * a2, fb2 = 4 * b2;
    int x, y, sigma;

    if (width == 0 || height == 0)
    {
        putBlock(x0, y0, texture, mode);
        return;
    }

    for (x = 0, y = height, sigma = 2*b2+a2*(1-2*height); b2*x <= a2*y; x++)
    {
        putBlock (x0 + x, y0 + y, texture, mode);
        putBlock (x0 - x, y0 + y, texture, mode);
        putBlock (x0 + x, y0 - y, texture, mode);
        putBlock (x0 - x, y0 - y, texture, mode);
        if (sigma >= 0)
        {
            sigma += fa2 * (1 - y);
            y--;
        }
        sigma += b2 * ((4 * x) + 6);
    }
    for (x = width, y = 0, sigma = 2*a2+b2*(1-2*width); a2*y <= b2*x; y++)
    {
        putBlock (x0 + x, y0 + y, texture, mode);
        putBlock (x0 - x, y0 + y, texture, mode);
        putBlock (x0 + x, y0 - y, texture, mode);
        putBlock (x0 - x, y0 - y, texture, mode);
        if (sigma >= 0)
        {
            sigma += fb2 * (1 - x);
            x--;
        }
        sigma += a2 * ((4 * y) + 6);
    }
}

//This function draws the edges of a given render chunk
//ptr - the pointer to the render chunk
void drawRenderChunkEdges(RenderChunk *ptr)
{
    int start[2], startOnScreen[2], end[2], endOnScreen[2];

    setpen(100, 100, 100, 0, 20*zoom);
    coordsFromWorld(ptr->x, ptr->y, start);
    coordsToScreen(start[X], start[Y], startOnScreen);
    moveto(startOnScreen[X], startOnScreen[Y]);
    coordsFromWorld(ptr->endX, ptr->y, end);
    coordsToScreen(end[X], end[Y], endOnScreen);
    lineto(endOnScreen[X], endOnScreen[Y]);
    coordsFromWorld(ptr->endX, ptr->endY, end);
    coordsToScreen(end[X], end[Y], endOnScreen);
    lineto(endOnScreen[X], endOnScreen[Y]);
    coordsFromWorld(ptr->x, ptr->endY, end);
    coordsToScreen(end[X], end[Y], endOnScreen);
    lineto(endOnScreen[X], endOnScreen[Y]);
    coordsFromWorld(ptr->x, ptr->y, end);
    coordsToScreen(end[X], end[Y], endOnScreen);
    lineto(endOnScreen[X], endOnScreen[Y]);
}

//This function draws the edges of the used area of a given render chunk
//ptr - the pointer to the render chunk
void drawRenderChunkUsedAreaEdges(RenderChunk *ptr)
{
    int start[2], startOnScreen[2], end[2], endOnScreen[2];

    setpen(150, 150, 150, 0, 20*zoom);
    coordsFromWorld(ptr->x + ptr->usedStartX, ptr->y + ptr->usedStartY, start);
    coordsToScreen(start[X], start[Y], startOnScreen);
    moveto(startOnScreen[X], startOnScreen[Y]);
    coordsFromWorld(ptr->x + ptr->usedEndX, ptr->y + ptr->usedStartY, end);
    coordsToScreen(end[X], end[Y], endOnScreen);
    lineto(endOnScreen[X], endOnScreen[Y]);
    coordsFromWorld(ptr->x + ptr->usedEndX, ptr->y + ptr->usedEndY, end);
    coordsToScreen(end[X], end[Y], endOnScreen);
    lineto(endOnScreen[X], endOnScreen[Y]);
    coordsFromWorld(ptr->x + ptr->usedStartX, ptr->y + ptr->usedEndY, end);
    coordsToScreen(end[X], end[Y], endOnScreen);
    lineto(endOnScreen[X], endOnScreen[Y]);
    coordsFromWorld(ptr->x + ptr->usedStartX, ptr->y + ptr->usedStartY, end);
    coordsToScreen(end[X], end[Y], endOnScreen);
    lineto(endOnScreen[X], endOnScreen[Y]);
}

//This function renders the render chunks, i.e. draws the blocks from the render chunks
//to the screen
void renderChunks(int mode)
{
    int blockOnGrid[2];
    int blockOnScreen[2];
    int viewPositionRenderChunk[2];
    int viewEndPositionRenderChunk[2];
    RenderChunk *ptr = renderChunkHead;

    coordsToRenderChunkCoords(viewX, viewY, viewPositionRenderChunk);
    coordsToRenderChunkCoords(viewEndX, viewEndY, viewEndPositionRenderChunk);

    while (ptr != NULL)
    {
        int i, j;

        if (mode)
        {
            switch (mode)
            {
                case 1: drawRenderChunkEdges(ptr); break;
                case 2: drawRenderChunkUsedAreaEdges(ptr); break;
                case 3: drawRenderChunkEdges(ptr); drawRenderChunkUsedAreaEdges(ptr); break;
            }
        }

        if (ptr->endX >= viewX && ptr->endY >= viewY && ptr->x <= viewEndX && ptr->y <= viewEndY)
        {
            for (i = ptr->usedStartY; i <= ptr->usedEndY; i ++)
            {
                for (j = ptr->usedStartX; j <= ptr->usedEndX; j ++)
                {
                    if (ptr->chunkArray[i][j] > 0)
                    {
                        coordsFromWorld(ptr->x + j, ptr->y + i, blockOnGrid);
                        coordsToScreen(blockOnGrid[X], blockOnGrid[Y], blockOnScreen);
                        currentTexture = ptr->chunkArray[i][j] - 1;
                        draw_from(gc2("block", currentTexture)->clonename, blockOnScreen[X], blockOnScreen[Y], zoom);
                    }
                }
            }
        }

        ptr = ptr->next;
    }
}
