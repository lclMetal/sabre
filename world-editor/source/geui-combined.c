// ..\source\geui\01-color.c
typedef struct
{
    double hue;
    double saturation;
    double brightness;

    unsigned char r;
    unsigned char g;
    unsigned char b;

    double alpha;
}Color;

#define COLOR_DEFAULT {   0.0,   0.0, 100.0, 255, 255, 255, 1.0 }
#define COLOR_WHITE   {   0.0,   0.0, 100.0, 255, 255, 255, 1.0 }
#define COLOR_BLACK   {   0.0,   0.0,   0.0,   0,   0,   0, 1.0 }
#define COLOR_RED     {   0.0, 100.0, 100.0, 255,   0,   0, 1.0 }
#define COLOR_GREEN   { 120.0, 100.0, 100.0,   0, 255,   0, 1.0 }
#define COLOR_BLUE    { 240.0, 100.0, 100.0,   0,   0, 255, 1.0 }
#define COLOR_YELLOW  {  60.0, 100.0, 100.0, 255, 255,   0, 1.0 }
#define COLOR_MAGENTA { 300.0, 100.0, 100.0, 255,   0, 255, 1.0 }
#define COLOR_CYAN    { 180.0, 100.0, 100.0,   0, 255, 255, 1.0 }

#define DEFAULT_COLOR   predefinedColors[0]
#define WHITE           predefinedColors[0]
#define BLACK           predefinedColors[1]
#define RED             predefinedColors[2]
#define GREEN           predefinedColors[3]
#define BLUE            predefinedColors[4]
#define YELLOW          predefinedColors[5]
#define MAGENTA         predefinedColors[6]
#define CYAN            predefinedColors[7]

Color predefinedColors[] =
{
    COLOR_WHITE, COLOR_BLACK, COLOR_RED, COLOR_GREEN,
    COLOR_BLUE, COLOR_YELLOW, COLOR_MAGENTA, COLOR_CYAN
};

Color createRGB(int rValue, int gValue, int bValue, double aValue);
Color createHSB(double hValue, double sValue, double bValue, double aValue);
Color HSBtoRGB(double hValue, double sValue, double bValue, double aValue);
Color RGBtoHSB(int rValue, int gValue, int bValue, double aValue);
int compareColors(Color *color1, Color *color2);
Color getActorColor(const char *actorName);
void colorThisActor(Color color);
void colorActor(Actor *pActor, Color color);
void colorActorByName(const char *actorName, Color color);
void colorClones(const char *actorName, long startIndex, long endIndex, Color color);

// This function creates a color with given values of red, green and blue, and then returns the color
// rValue - the value of the red component of the color
// gValue - the value of the green component of the color
// bValue - the value of the blue component of the color
// aValue - the alpha of the color
Color createRGB(int rValue, int gValue, int bValue, double aValue)
{
    Color color;
    Color hsb;

    color.r = rValue;
    color.g = gValue;
    color.b = bValue;
    color.alpha = aValue;

    hsb = RGBtoHSB(rValue, gValue, bValue, color.alpha);

    color.hue = hsb.hue;
    color.saturation = hsb.saturation;
    color.brightness = hsb.brightness;

    return color;
}

// This function creates a color with given values of hue, saturation and brightness, and then
// returns the color
// hValue - the hue for the color
// sValue - the saturation for the color
// bValue - the brightness for the color
// aValue - the alpha of the color
Color createHSB(double hValue, double sValue, double bValue, double aValue)
{
    Color color;
    Color rgb;

    color.hue = hValue;
    color.saturation = sValue;
    color.brightness = bValue;
    color.alpha = aValue;

    rgb = HSBtoRGB(hValue, sValue, bValue, color.alpha);

    color.r = rgb.r;
    color.g = rgb.g;
    color.b = rgb.b;

    return color;
}

// This function converts given HSB color system values to corresponding RGB system color values
// and returns a Color with the r, g and b values
// hValue - the hue value of the color to be converted to RGB
// sValue - the saturation value of the color to be converted to RGB
// bValue - the brightness value of the color to be converted to RGB
// aValue - the alpha of the color
Color HSBtoRGB(double hValue, double sValue, double bValue, double aValue)
{
    Color color;

    double chroma = bValue * sValue;
    double m = bValue - chroma;

    double H = ((double)hValue / 60.0);
    double xValue = chroma * (1 - abs(fmod(H, 2) - 1));

    color.hue = hValue;
    color.saturation = sValue;
    color.brightness = bValue;
    color.alpha = aValue;

    if (H >= 0 && H < 1)
    {
        color.r = (chroma + m) * 255.0;
        color.g = (xValue + m) * 255.0;
        color.b = m * 255.0;
    }
    if (H >= 1 && H < 2)
    {
        color.r = (xValue + m) * 255.0;
        color.g = (chroma + m) * 255.0;
        color.b = m * 255.0;
    }
    if (H >= 2 && H < 3)
    {
        color.r = m * 255.0;
        color.g = (chroma + m) * 255.0;
        color.b = (xValue + m) * 255.0;
    }
    if (H >= 3 && H < 4)
    {
        color.r = m * 255.0;
        color.g = (xValue + m) * 255.0;
        color.b = (chroma + m) * 255.0;
    }
    if (H >= 4 && H < 5)
    {
        color.r = (xValue + m) * 255.0;
        color.g = m * 255.0;
        color.b = (chroma + m) * 255.0;
    }
    if (H >= 5 && H < 6)
    {
        color.r = (chroma + m) * 255.0;
        color.g = m * 255.0;
        color.b = (xValue + m) * 255.0;
    }

    return color;
}

// This function converts given RGB color system values to corresponding HSB system color values
// and returns a Color with the hue, saturation and brightness values
// rValue - the red component of the color to be converted to HSB
// gValue - the green component of the color to be converted to HSB
// bValue - the blue component of the color to be converted to HSB
// aValue - the alpha of the color
Color RGBtoHSB(int rValue, int gValue, int bValue, double aValue)
{
    Color color;

    int biggest = max(rValue, max(gValue, bValue));

    double percentageR = rValue / 255.0;
    double percentageG = gValue / 255.0;
    double percentageB = bValue / 255.0;

    double chromaMax;
    double chromaMin;
    double chroma;

    color.r = rValue;
    color.g = gValue;
    color.b = bValue;
    color.alpha = aValue;

    if (biggest == rValue)
    {
        chromaMax = percentageR;
        chromaMin = min(percentageR, min(percentageG, percentageB));
        chroma = chromaMax - chromaMin;

        color.hue = 60.0 * (fmod(((percentageG - percentageB) / max(chroma, 0.0000001)), 6.0));
        if (color.hue < 0.0)color.hue += 360.0;

        color.saturation = (chromaMax == 0) ? 0 : (chroma / chromaMax);
        color.brightness = chromaMax;
    }
    else if (biggest == gValue)
    {
        chromaMax = percentageG;
        chromaMin = min(percentageR, min(percentageG, percentageB));
        chroma = chromaMax - chromaMin;

        color.hue = 60.0 * (((percentageB - percentageR) / max(chroma, 0.0000001)) + 2.0);
        color.saturation = (chromaMax == 0) ? 0 : (chroma / chromaMax);
        color.brightness = chromaMax;
    }
    else if (biggest == bValue)
    {
        chromaMax = percentageB;
        chromaMin = min(percentageR, min(percentageG, percentageB));
        chroma = chromaMax - chromaMin;

        color.hue = 60.0 * (((percentageR - percentageG) / max(chroma, 0.0000001)) + 4.0);
        color.saturation = (chromaMax == 0) ? 0 : (chroma / chromaMax);
        color.brightness = chromaMax;
    }

    return color;
}

// This function compares two colors
// color1 - the first color to compare
// color2 - the second color to compare
// returns: 1 if colors match, 0 if not, -1 on error
int compareColors(Color *color1, Color *color2)
{
    if (!color1 || !color2) return -1;
    return (color1->r == color2->r && color1->g == color2->g && color1->b == color2->b);
}

// This function retrieves an actor's color
// actorName - the name of the actor
// returns: the retrieved color
Color getActorColor(const char *actorName)
{
    Actor *a = getclone(actorName);

    if (a->cloneindex < 0)
        return createRGB(0, 0, 0, 1.0);
    else
        return createRGB(a->r, a->g, a->b, 1.0 - a->transp);
}

// This function colors the current actor with a given color
// color - the color to color the actor with
void colorThisActor(Color color)
{
    r = color.r;
    g = color.g;
    b = color.b;
    transp = 1.0 - color.alpha;
}

// This function colors the given actor with a given color
// pActor - the pointer to the actor to be colored
// color - the color to color the actor with
void colorActor(Actor *pActor, Color color)
{
    if (!pActor) return;

    pActor->r = color.r;
    pActor->g = color.g;
    pActor->b = color.b;
    pActor->transp = 1.0 - color.alpha;
}

// This function colors the named actor with a given color
// actorName - the name of the actor to be colored
// color - the color to color the actor with
void colorActorByName(const char *actorName, Color color)
{
    Actor *a = getclone(actorName);

    if (a->cloneindex < 0) return;

    a->r = color.r;
    a->g = color.g;
    a->b = color.b;
    a->transp = 1.0 - color.alpha;
}

// This functions colors multiple sequential clones with a given color
// actorName - the base name of the clones
// startIndex - the index of the first clone to color
// endIndex - the index of the last clone to color
// color - the color to color the clones with
void colorClones(const char *actorName, long startIndex, long endIndex, Color color)
{
    long i;
    char cName[256];

    for (i = startIndex; i <= endIndex; i ++)
    {
        sprintf(cName, "%s.%i", actorName, i);
        colorActorByName(cName, color);
    }
}


// ..\source\geui\02-misc.c
#define HALF_WIDTH (width / 2)
#define HALF_HEIGHT (height / 2)
#define HALF_WIDTH_ACTOR(X) (X->width / 2)
#define HALF_HEIGHT_ACTOR(X) (X->height / 2)

int iLimit(int value, int minVal, int maxVal);
short siLimit(short value, short minVal, short maxVal);
float fLimit(float value, float minVal, float maxVal);
double dfLimit(double value, double minVal, double maxVal);
int safeStrcat(char *destination, const char *source, int maxLen);
char *addFileExtension(char *fileName, char *fileExtension);
int actorExists(char *actorName);
int actorExists2(Actor *a);
Actor *gc2(char *actorName, long cloneNum);
void destroyClone(const char *actorName, long cloneNum);
void destroyClones(const char *actorName, long startIndex, long endIndex);
void changeParentOfClones(const char *actorName, long startIndex, long endIndex, const char *parentName);
void disableMouseEvents(char *actorName);
void enableMouseEvents(char *actorName);
int mouseOver(void);
int mouseOverRegion(void);
int mouseOverPartOfRegion(int startX, int startY, int endX, int endY);
int mouseOverActor(const char *actorName);
int mouseOverClones(const char *actorName, long startIndex, long endIndex);

int iLimit(int value, int minVal, int maxVal)
{
    return (int)max(min(minVal, maxVal), min(value, max(maxVal, minVal)));
}

short siLimit(short value, short minVal, short maxVal)
{
    return (short)max(min(minVal, maxVal), min(value, max(maxVal, minVal)));
}

float fLimit(float value, float minVal, float maxVal)
{
    return (float)max(min(minVal, maxVal), min(value, max(maxVal, minVal)));
}

double dfLimit(double value, double minVal, double maxVal)
{
    return max(min(minVal, maxVal), min(value, max(maxVal, minVal)));
}

int safeStrcat(char *destination, const char *source, int maxLen)
{
    int dLen = strlen(destination);                   // Current length of destination string
    int sLen = strlen(source);                        // Current length of source string
    int catLen = sLen - (dLen + sLen - (maxLen - 1)); // Calculate how many characters at most
                                                      // can be concatenated to destination

    strncat(destination, source, catLen); // Concatenate as much as possible

    return (catLen < sLen); // Return 1 if some of the source string had to be left out
}

char tempFullFileName[256];
char *addFileExtension(char *fileName, char *fileExtension)
{
    char *fileExtensionPos;

    fileExtensionPos = strchr(fileName, '.');

    if (fileExtensionPos == NULL) sprintf(tempFullFileName, "%s.%s", fileName, fileExtension);
    else
    {
        int actualFileNameLength = strlen(fileName) - strlen(fileExtensionPos);

        strncpy(tempFullFileName, fileName, actualFileNameLength);
        sprintf(tempFullFileName + actualFileNameLength, ".%s", fileExtension);
    }

    return tempFullFileName;
}

int actorExists(char *actorName)
{
    // GE uses the cloneindex -1 to indicate that an actor is not a valid, existing actor.
    // The actor count needs to be checked because:
    //
    // 1. IF an actor with the given base name exists in the project
    // 2. AND the lowest indexed clone (= the actor itself, if no clones present)
    //      in editor mode has "Create at startup" set to "No"
    // 3. AND the actor currently has no alive instances in the game
    //
    // ..getclone will return an invalid Actor* that has the cloneindex of that specific
    // clone / actor over in editor mode despite it not existing in the game.
    // And that would break this function without the ActorCount check.
    Actor *a = getclone(actorName);
    return (ActorCount(a->name) > 0 && a->cloneindex != -1);
}

// ----- WARNING! -----
//
// Using this function on a stale pointer is undefined behavior!
// Use this function ONLY for checking the result of getclone() right after fetching it.
//
// If you didn't fully understand the above warnings, DO NOT USE THIS FUNCTION!
// Use the above version instead (actorExists())
//
// This function only exists for optimizing performance in cases where a fresh actor pointer
// is already at hand.
int actorExists2(Actor *a)
{
    // GE uses the cloneindex -1 to indicate that an actor is not a valid, existing actor.
    // The actor count needs to be checked because:
    //
    // 1. IF an actor with the given base name exists in the project
    // 2. AND the lowest indexed clone (= the actor itself, if no clones present)
    //      in editor mode has "Create at startup" set to "No"
    // 3. AND the actor currently has no alive instances in the game
    //
    // ..getclone will return an invalid Actor* that has the cloneindex of that specific
    // clone / actor over in editor mode despite it not existing in the game.
    // And that would break this function without the ActorCount check.
    return (ActorCount(a->name) > 0 && a->cloneindex != -1);
}

Actor *gc2(char *actorName, long cloneNum)
{
    char cName[256];
    sprintf(cName, "%s.%d", actorName, cloneNum);
    return getclone(cName);
}

void destroyClone(const char *actorName, long cloneNum)
{
    char cName[256];
    sprintf(cName, "%s.%d", actorName, cloneNum);
    DestroyActor(cName);
}

void destroyClones(const char *actorName, long startIndex, long endIndex)
{
    long i;
    char cName[256];

    if (startIndex < 0 || endIndex < 0) return;

    for (i = startIndex; i <= endIndex; i ++)
    {
        sprintf(cName, "%s.%d", actorName, i);
        DestroyActor(cName);
    }
}

void changeParentOfClones(const char *actorName, long startIndex, long endIndex, const char *parentName)
{
    long i;
    char cName[256];

    if (startIndex < 0 || endIndex < 0) return;

    for (i = startIndex; i <= endIndex; i ++)
    {
        sprintf(cName, "%s.%d", actorName, i);
        ChangeParent(cName, parentName);
    }
}

void disableMouseEvents(char *actorName)
{
    EventDisable(actorName, EVENTMOUSEBUTTONDOWN);
    EventDisable(actorName, EVENTMOUSEBUTTONUP);
    EventDisable(actorName, EVENTMOUSEENTER);
    EventDisable(actorName, EVENTMOUSELEAVE);
}

void enableMouseEvents(char *actorName)
{
    EventEnable(actorName, EVENTMOUSEBUTTONDOWN);
    EventEnable(actorName, EVENTMOUSEBUTTONUP);
    EventEnable(actorName, EVENTMOUSEENTER);
    EventEnable(actorName, EVENTMOUSELEAVE);
}

int mouseOver(void)
{
    return (xmouse >= xscreen - HALF_WIDTH &&
            xmouse <  xscreen - HALF_WIDTH + width &&
            ymouse >= yscreen - HALF_HEIGHT &&
            ymouse <  yscreen - HALF_HEIGHT + height);
}

int mouseOverRegion(void)
{
    return (xmouse >= xscreen && xmouse < xscreen + width &&
            ymouse >= yscreen && ymouse < yscreen + height);
}

int mouseOverPartOfRegion(int startX, int startY, int endX, int endY)
{
    return (xmouse >= xscreen + startX && xmouse < xscreen + width - endX &&
            ymouse >= yscreen + startY && ymouse < yscreen + height - endY);
}

int mouseOverActor(const char *actorName)
{
    Actor *actor;

    if (!actorExists2(actor = getclone(actorName))) return 0;

    return (xmouse >= actor->xscreen - HALF_WIDTH_ACTOR(actor) &&
            xmouse <  actor->xscreen - HALF_WIDTH_ACTOR(actor) + actor->width &&
            ymouse >= actor->yscreen - HALF_HEIGHT_ACTOR(actor) &&
            ymouse <  actor->yscreen - HALF_HEIGHT_ACTOR(actor) + actor->height);
}

int mouseOverClones(const char *actorName, long startIndex, long endIndex)
{
    long i;
    char cName[256];

    if (startIndex < 0 || endIndex < 0) return 0;

    for (i = startIndex; i <= endIndex; i ++)
    {
        sprintf(cName, "%s.%d", actorName, i);
        if (mouseOverActor(cName)) return 1;
    }

    return 0;
}


// ..\source\geui\03-text_renderer.c
#define CAPACITY_STEP 256

#define ALIGN_LEFT 0
#define ALIGN_CENTER 1
#define ALIGN_RIGHT 2

#define SKIP_ESCAPE_SEQUENCE_MARKER 1
#define ESCAPE_SEQUENCE_CHARACTER_LENGTH 1
#define SKIP_COLOR_ESCAPE_SEQUENCE 2
#define SKIP_COLOR_VALUES 5
#define SKIP_COLOR_CHANGE 7
#define COLOR_ESCAPE_SEQUENCE_LENGTH 7
#define SKIP_ENDING_ESCAPE_SEQUENCE 2
#define ENDING_ESCAPE_SEQUENCE_LENGTH 2

#define ASCII_START 33
#define NULL_ACTOR getclone("-")

#define ABSOLUTE False
#define RELATIVE True

typedef enum { False, True } bool;

#define CHARS 94

typedef struct
{
    int letterSpacing;
    int wordSpacing;
    int lineSpacing;
    int indentation;
    char fontName[256];
    int fontCharWidths[CHARS];
}Font;

typedef struct LetterCloneindexStruct
{
    int index;
    struct LetterCloneindexStruct *next;
}LetterCloneindex;

typedef struct
{
    bool rendered;
    int lastRenderFrame;

    int beginX;
    int beginY;
    bool relative;
    int alignment;

    int textAreaLeftX;
    int textAreaTopY;
    int textAreaRightX;
    int textAreaBottomY;
    int textAreaScrollPosition;
    int textAreaScrollUpLimit;
    int textAreaScrollDownLimit;
    double textAreaHeightPercent;
    double textAreaScrollPercent;
    bool fitInArea;

    size_t capacity;
    char *pString;

    LetterCloneindex *letterIndexHead;
    LetterCloneindex *letterIndexTail;

    int width;
    int height;

    double zDepth;
    char parentCName[256];

    Font *pFont;
    Color color;
}Text;

char typeActorName[2][256] = {"a_typeActor", "a_typeActor2"};

// Functions for use
void fitTextInWidth(Text *pText, int widthLimit);
void fitTextInArea(Text *pText, int topLeftX, int topLeftY, int bottomRightX, int bottomRightY);
void scrollTextByAmount(Text *pText, int scroll);
void setTextScrollByPercent(Text *pText, double scrollPercent);
void readFontDataFile(char *fileName, Font *fontData);
size_t calculateRequiredCapacity(size_t len);
void updateTextDimensions(Text *pText);
Text createText(const char *string, Font *pFont, const char *parentCName, bool relative, int startX, int startY);
void setTextAlignment(Text *pText, int alignment);
void setTextColor(Text *pText, Color color);
void setTextZDepth(Text *pText, double zDepth);
void setTextParent(Text *pText, char *parentCName, bool keepCurrentPosition);
void setTextPosition(Text *pText, int posX, int posY);
void concatenateText(Text *pText, char *string);
void concatenateCharToText(Text *pText, char c);
void setTextText(Text *pText, char *string);
void refreshText(Text *pText);
void eraseText(Text *pText);
void destroyText(Text *pText);

// Internal system functions
int fromASCII(int num);
Color extractHexColor(char *pString);
int calculateTextHeight(Text *pText);
int calculateTextWidth(Text *pText);
int calculateStringWidth(Font *pFont, const char *string);
int getSpecialCharWidth(Text *pText, int pos);
int getCharWidth(Text *pText, int pos);
int isIndentationAllowed(Text *pText);
int getPrevTabStop(Text *pText, int lineWidth);
int getNextTabStopOffset(Text *pText, int lineWidth);
int strCharsLeft(const char *str, int pos);
int isPrintable(const char *str, int pos);
int getLetterSpacing(Text *pText, int lineWidth, int charWidth, int pos);
int calculateLineSubRangeWidth(Text *pText, int start, int end);
int calculateLineTailWidth(Text *pText, int start);
int parseToNextLineStart(Text *pText, int startPosition, Color *pTempColor, bool *pCustomColorPrint);
void replaceAllInstancesOfChar(char *pString, char replace, char new);
void handleEscapeSequences(Text *pText, int *iterator, int *pPrevX, int *pPrevY, Color *pTempColor, bool *pCustomColorPrint, bool *firstOnLine);
void skipEscapeSequences(char *pString, int *iterator, Color *pTempColor, bool *pCustomColorPrint);
void renderText(Text *pText);
int renderCharacter(Text *pText, char printChar, Color color, bool relative, int xPos, int yPos, bool firstOnLine);
LetterCloneindex *newLetterCloneindex(int index);
void storeLetterCloneindex(Text *pText, int index);
void freeLetterCloneindexList(Text *pText);
void execOnLCList(Text *pText, LetterCloneindex *list, LetterCloneindex *(*pFunc)(Text *, LetterCloneindex *));
LetterCloneindex *freeLC(Text *pText, LetterCloneindex *this);
LetterCloneindex *setLCZDepth(Text *pText, LetterCloneindex *this);
LetterCloneindex *setLCParent(Text *pText, LetterCloneindex *this);
LetterCloneindex *eraseLC(Text *pText, LetterCloneindex *this);

int fromASCII(int num)
{
    int value = num - ASCII_START;

    if (value < 0 || value > CHARS - 1) return '?' - ASCII_START;
    else return value;
}

Color extractHexColor(char *pString)
{
    Color tempColor;
    unsigned int rValue;
    unsigned int gValue;
    unsigned int bValue;

    sscanf(pString, "%02x%02x%02x", &rValue, &gValue, &bValue);

    tempColor = createRGB(rValue, gValue, bValue, 1.0);

    return tempColor;
}

int calculateTextHeight(Text *pText)
{
    int i;
    int lineCount = 1;
    int len;
    char *pString = pText->pString;

    len = strlen(pString);

    for (i = 0; i < len; i ++)
        if (pString[i] == '\n' || pString[i] == '\v') lineCount ++;

    return pText->pFont->lineSpacing * lineCount;
}

int calculateTextWidth(Text *pText)
{
    int i;
    int len;
    int lineWidth;
    int maximumLineWidth;
    char *pString = pText->pString;

    maximumLineWidth = calculateLineTailWidth(pText, 0);
    len = strlen(pString);

    for (i = 0; i < len; i ++)
    {
        if (pString[i] == '\n' || pString[i] == '\v')
        {
            lineWidth = calculateLineTailWidth(pText, i + 1);

            if (lineWidth > maximumLineWidth) maximumLineWidth = lineWidth;
        }
    }

    return maximumLineWidth;
}

int calculateStringWidth(Font *pFont, const char *string)
{
    Text tempText = createText(string, pFont, "(none)", False, 0, 0);
    int result = tempText.width;

    destroyText(&tempText);

    return result;
}

int getSpecialCharWidth(Text *pText, int pos)
{
    int letterNum = 0;
    int len = strlen(pText->pString);

    if (pos < len - 1)
    {
        letterNum = fromASCII(pText->pString[pos]);

        switch (pText->pString[pos])
        {
            case '$':   return pText->pFont->fontCharWidths[letterNum];
            case '_':   return pText->pFont->wordSpacing;
            default:    return 0;
        }
    }

    return 0;
}

int getCharWidth(Text *pText, int pos)
{
    int letterNum = fromASCII(pText->pString[pos]);

    switch (pText->pString[pos])
    {
        case '\n':
        case '\v':
        case '\t':  return 0;
        case ' ':   return pText->pFont->wordSpacing;
        case '$':   return getSpecialCharWidth(pText, pos + 1);
        default:    return pText->pFont->fontCharWidths[letterNum];
    }
}

int isIndentationAllowed(Text *pText)
{
    return (pText->pFont->indentation && pText->alignment == ALIGN_LEFT);
}

int getPrevTabStop(Text *pText, int lineWidth)
{
    int tab = pText->pFont->indentation;
    return ceil(lineWidth / tab) * tab;
}

int getNextTabStopOffset(Text *pText, int lineWidth)
{
    int tab = pText->pFont->indentation;

    if (isIndentationAllowed(pText))
        return getPrevTabStop(pText, lineWidth) + tab - lineWidth;

    return 0;
}

int strCharsLeft(const char *str, int pos)
{
    return strlen(str) - pos - 1;
}

int isPrintable(const char *str, int pos)
{
    char c = str[pos];
    int len = strlen(str);

    switch (c)
    {
        case ' ':
        case '\t':
        case '\n':
        case '\v': return 0;

        case '$':
            return (strCharsLeft(str, pos) > 0 && str[pos + 1] == '$');

        default: return 1;
    }

}

int getLetterSpacing(Text *pText, int lineWidth, int charWidth, int pos)
{
    if (isPrintable(pText->pString, pos))
        return pText->pFont->letterSpacing * (lineWidth > charWidth);
    else
        return 0;
}

int calculateLineSubRangeWidth(Text *pText, int start, int end)
{
    int i           = 0;
    int lineWidth   = 0;
    int curCharW    = 0;

    for (i = start; i < end; i++)
    {
        lineWidth += curCharW = getCharWidth(pText, i);
        lineWidth += getLetterSpacing(pText, lineWidth, curCharW, i);

        switch (pText->pString[i])
        {
            case '$':   skipEscapeSequences(pText->pString, &i, NULL, NULL); break;
            case '\t':  lineWidth += getNextTabStopOffset(pText, lineWidth); break;
            case '\n':
            case '\v':  return lineWidth;
            default:    break;
        }
    }

    return lineWidth;
}

int calculateLineTailWidth(Text *pText, int start)
{
    return calculateLineSubRangeWidth(pText, start, strlen(pText->pString));
}

int parseToNextLineStart(Text *pText, int startPosition, Color *pTempColor, bool *pCustomColorPrint)
{
    int i;
    int len;
    char *pString = pText->pString;

    len = strlen(pString);

    for (i = startPosition; i < len; i ++)
    {
        if (pString[i] == '$')
            skipEscapeSequences(pText->pString, &i, pTempColor, pCustomColorPrint);

        if (pString[i] == '\n' || pString[i] == '\v')
        {
            if (i < len - 1) return i + 1;
            else return i;
        }

    }

    return -1;
}

void replaceAllInstancesOfChar(char *pString, char replace, char new)
{
    int i;
    int len = strlen(pString);

    for (i = 0; i < len; i ++)
    {
        if (pString[i] == replace)
            pString[i] = new;
    }
}

void fitTextInWidth(Text *pText, int widthLimit)
{
    int i;
    char *tempLine;
    int lineWidth = 0;
    int lineCharCount = 0;
    int maximumLineWidth = 0;
    int maximumLineCharCount = 0;
    int currentLineStart = 0;
    int len;
    char *pString = pText->pString;

    replaceAllInstancesOfChar(pString, '\n', ' ');

    len = strlen(pString);

    for (i = 0; i < len; i ++)
    {
        if (pString[i] == ' ' || pString[i] == '\v' || i == len -  1)
        {
            int charCount = i - currentLineStart + (i == len - 1);

            tempLine = calloc(charCount + 1, sizeof(char));
            strncpy(tempLine, &pString[currentLineStart], charCount);

            lineWidth = calculateLineSubRangeWidth(pText, currentLineStart, currentLineStart + charCount);
            lineCharCount = strlen(tempLine);

            if (maximumLineWidth == 0 && i < len - 1)
            {
                maximumLineWidth = lineWidth;
                maximumLineCharCount = lineCharCount;
            }

            if (lineWidth > maximumLineWidth && lineWidth <= widthLimit)
            {
                maximumLineWidth = lineWidth;
                maximumLineCharCount = lineCharCount;
            }

            if (lineWidth > widthLimit && maximumLineCharCount > 0)
            {
                int tempCharCount = currentLineStart + maximumLineCharCount;

                pString[tempCharCount] = '\n';

                currentLineStart = tempCharCount + 1;
                i = currentLineStart - 1;

                maximumLineWidth = 0;
                maximumLineCharCount = 0;
            }
            else if (pString[i] == '\v')
            {
                currentLineStart = i + 1;

                maximumLineWidth = 0;
                maximumLineCharCount = 0;
            }

            free(tempLine);
        }
    }

    pText->width = calculateTextWidth(pText);
    pText->height = calculateTextHeight(pText);
}

void fitTextInArea(Text *pText, int topLeftX, int topLeftY, int bottomRightX, int bottomRightY)
{
    int boxWidth = bottomRightX - topLeftX;
    int boxHeight = bottomRightY - topLeftY;

    pText->textAreaLeftX = topLeftX;
    pText->textAreaTopY = topLeftY;
    pText->textAreaRightX = bottomRightX;
    pText->textAreaBottomY = bottomRightY;
    pText->fitInArea = True;

    switch (pText->alignment)
    {
        case ALIGN_LEFT: pText->beginX = topLeftX; break;
        case ALIGN_CENTER: pText->beginX = topLeftX + ceil(boxWidth * 0.5); break;
        case ALIGN_RIGHT: pText->beginX = bottomRightX; break;
    }

    pText->beginY = topLeftY + ceil(pText->pFont->lineSpacing * 0.5);
    pText->textAreaScrollPosition = pText->beginY;
    pText->textAreaScrollPercent = 1.0;

    fitTextInWidth(pText, boxWidth);
    pText->textAreaScrollUpLimit = min(pText->beginY, pText->beginY - ceil((pText->height - boxHeight) / (double)pText->pFont->lineSpacing) * pText->pFont->lineSpacing);
    pText->textAreaScrollDownLimit = pText->beginY;
    pText->textAreaHeightPercent = min((double)boxHeight / (double)pText->height, 1.0);
}

void scrollTextByAmount(Text *pText, int scroll)
{
    int tempScrollHeight = pText->textAreaScrollDownLimit - pText->textAreaScrollUpLimit;

    if ((scroll < 0 && pText->textAreaScrollPosition + scroll >= pText->textAreaScrollUpLimit) ||
        (scroll > 0 && pText->textAreaScrollPosition + scroll <= pText->textAreaScrollDownLimit))
    {
        pText->textAreaScrollPosition += scroll;
        pText->textAreaScrollPercent = (double)(pText->textAreaScrollPosition - pText->textAreaScrollUpLimit) / (double)tempScrollHeight;
        refreshText(pText);
    }
    else if (scroll < 0 && pText->textAreaScrollPosition != pText->textAreaScrollUpLimit)
    {
        pText->textAreaScrollPosition = pText->textAreaScrollUpLimit;
        pText->textAreaScrollPercent = (double)(pText->textAreaScrollPosition - pText->textAreaScrollUpLimit) / (double)tempScrollHeight;
        refreshText(pText);
    }
    else if (scroll > 0 && pText->textAreaScrollPosition != pText->textAreaScrollDownLimit)
    {
        pText->textAreaScrollPosition = pText->textAreaScrollDownLimit;
        pText->textAreaScrollPercent = (double)(pText->textAreaScrollPosition - pText->textAreaScrollUpLimit) / (double)tempScrollHeight;
        refreshText(pText);
    }
}

void setTextScrollByPercent(Text *pText, double scrollPercent)
{
    int tempScrollHeight = pText->textAreaScrollDownLimit - pText->textAreaScrollUpLimit;
    double limitedValue = min(1.0, max(0.0, scrollPercent));

    pText->textAreaScrollPosition = pText->textAreaScrollUpLimit + (limitedValue * (double)tempScrollHeight);
    pText->textAreaScrollPercent = limitedValue;

    refreshText(pText);
}

// TODO: convert to function that returns error codes for how the reading went
void readFontDataFile(char *fileName, Font *fontData)
{
    FILE *f = fopen(addFileExtension(fileName, "fdf"), "r+b");

    if (f)
    {
        fread(fontData, sizeof *fontData, 1, f);
        fclose(f);
    }
}

size_t calculateRequiredCapacity(size_t len)
{
    return ((len / CAPACITY_STEP) + 1) * (size_t)CAPACITY_STEP;
}

Text createText(const char *string, Font *pFont, const char *parentCName, bool relative, int startX, int startY)
{
    Text temp;
    char const *strPtr = NULL;
    char strError[100] = "Warning: invalid value for argument 'string' in createText.";

    temp.rendered = False;
    temp.lastRenderFrame = -1;
    temp.beginX = startX;
    temp.beginY = startY;
    temp.relative = relative;
    temp.alignment = ALIGN_LEFT;
    temp.zDepth = 0.5;
    strcpy(temp.parentCName, parentCName);
    temp.pFont = pFont;
    temp.color = createRGB(255, 255, 255, 1.0);
    temp.textAreaLeftX = 0;
    temp.textAreaTopY = 0;
    temp.textAreaRightX = 0;
    temp.textAreaBottomY = 0;
    temp.textAreaScrollPosition = 0;
    temp.textAreaScrollUpLimit = 0;
    temp.textAreaScrollDownLimit = 0;
    temp.textAreaHeightPercent = 0.0;
    temp.textAreaScrollPercent = 1.0;
    temp.fitInArea = False;
    temp.letterIndexHead = NULL;
    temp.letterIndexTail = NULL;

    strPtr = string;
    if (strPtr == NULL)
        strPtr = strError;

    temp.capacity = calculateRequiredCapacity(strlen(strPtr));
    temp.pString = calloc(temp.capacity + 1, sizeof(char));

    if (temp.pString)
        strcpy(temp.pString, strPtr);
    else
        temp.capacity = 0;

    temp.width = calculateTextWidth(&temp);
    temp.height = calculateTextHeight(&temp);

    return temp;
}

void updateTextDimensions(Text *pText)
{
    pText->width = calculateTextWidth(pText);
    pText->height = calculateTextHeight(pText);
}

void execOnLCList(Text *pText, LetterCloneindex *list, LetterCloneindex *(*pFunc)(Text *, LetterCloneindex *))
{
    LetterCloneindex *iterator = list, *next;

    while (iterator)
    {
        next = (*pFunc)(pText, iterator);
        iterator = next;
    }
}

LetterCloneindex *newLetterCloneindex(int index)
{
    LetterCloneindex *new = malloc(sizeof *new);

    if (!new) return NULL;

    new->index = index;
    new->next = NULL;

    return new;
}

void storeLetterCloneindex(Text *pText, int index)
{
    LetterCloneindex *new = newLetterCloneindex(index);

    if (!pText->letterIndexHead && !pText->letterIndexTail)
    {
        pText->letterIndexHead = pText->letterIndexTail = new;
    }
    else
    {
        pText->letterIndexTail->next = new;
        pText->letterIndexTail = new;
    }
}

LetterCloneindex *freeLC(Text *pText, LetterCloneindex *this)
{
    LetterCloneindex *next = this->next;
    free(this);
    return next;
}

void freeLetterCloneindexList(Text *pText)
{
    execOnLCList(pText, pText->letterIndexHead, &freeLC);
    pText->letterIndexHead = NULL;
    pText->letterIndexTail = NULL;
}

void setTextAlignment(Text *pText, int alignment)
{
    if (pText->alignment != alignment)
    {
        pText->alignment = alignment;

        if (pText->fitInArea) fitTextInArea(pText, pText->textAreaLeftX, pText->textAreaTopY, pText->textAreaRightX, pText->textAreaBottomY);
    }
}

void setTextColor(Text *pText, Color color)
{
    pText->color = color;
}

LetterCloneindex *setLCZDepth(Text *pText, LetterCloneindex *this)
{
    char actorName[256];
    sprintf(actorName, "%s.%i", typeActorName[pText->lastRenderFrame], this->index);
    ChangeZDepth(actorName, pText->zDepth);
    return this->next;
}

void setTextZDepth(Text *pText, double zDepth)
{
    pText->zDepth = zDepth;
    execOnLCList(pText, pText->letterIndexHead, &setLCZDepth);
}

LetterCloneindex *setLCParent(Text *pText, LetterCloneindex *this)
{
    char actorName[256];
    sprintf(actorName, "%s.%i", typeActorName[pText->lastRenderFrame], this->index);
    ChangeParent(actorName, pText->parentCName);
    return this->next;
}

void setTextParent(Text *pText, char *parentCName, bool keepCurrentPosition)
{
    int parentExists;
    Actor *pParent;
    Actor *pPrevParent;

    pParent = getclone(parentCName);
    pPrevParent = getclone(pText->parentCName);

    if (!(parentExists = actorExists2(pParent)))
        strcpy(pText->parentCName, "(none)");
    else
        strcpy(pText->parentCName, parentCName);

    execOnLCList(pText, pText->letterIndexHead, &setLCParent);
    pText->relative = (parentExists) ? True : False;

    if (keepCurrentPosition)
    {
        int parentX = pParent->x * actorExists2(pParent);
        int parentY = pParent->y * actorExists2(pParent);
        int prevParentX = pPrevParent->x * actorExists2(pPrevParent);
        int prevParentY = pPrevParent->y * actorExists2(pPrevParent);

        pText->beginX = prevParentX + pText->beginX - parentX;
        pText->beginY = prevParentY + pText->beginY - parentY;
    }
}

void setTextPosition(Text *pText, int posX, int posY)
{
    pText->beginX = posX;
    pText->beginY = posY;
}

void concatenateText(Text *pText, char *string)
{
    size_t len = strlen(pText->pString) + strlen(string);

    if (len <= pText->capacity)
    {
        strcat(pText->pString, string);
        return;
    }
    else
    {
        size_t reqCapacity = calculateRequiredCapacity(len);
        pText->capacity = reqCapacity;
        pText->pString = realloc(pText->pString, pText->capacity + 1 * sizeof(char));
    }

    if (pText->pString)
        strcat(pText->pString, string);
    else
        pText->capacity = 0;
}

void concatenateCharToText(Text *pText, char c)
{
    char temp[2];

    if (c == '\b')
    {
        pText->pString[strlen(pText->pString) - (strlen(pText->pString) > 0)] = '\0';
        return;
    }
    else if (c == '$')
    {
        concatenateText(pText, "$$");
        return;
    }

    sprintf(temp, "%c", c);
    concatenateText(pText, temp);
}

void setTextText(Text *pText, char *string)
{
    size_t reqCapacity;
    eraseText(pText);

    if (pText->capacity != (reqCapacity = calculateRequiredCapacity(strlen(string))))
    {
        if (pText->pString)
            free(pText->pString);

        pText->capacity = reqCapacity;
        pText->pString = calloc(pText->capacity + 1, sizeof(char));
    }

    if (pText->pString)
        strcpy(pText->pString, string);
    else
        pText->capacity = 0;
}

void refreshText(Text *pText)
{
    renderText(pText);
}

LetterCloneindex *eraseLC(Text *pText, LetterCloneindex *this)
{
    char actorName[256];
    sprintf(actorName, "%s.%i", typeActorName[pText->lastRenderFrame], this->index);
    DestroyActor(actorName);
    this->index = -1;
    return this->next;
}

void eraseText(Text *pText)
{
    if (pText && pText->pString && pText->rendered && pText->lastRenderFrame > -1)
    {
        execOnLCList(pText, pText->letterIndexHead, &eraseLC);
        pText->rendered = False;
        pText->lastRenderFrame = -1;
        freeLetterCloneindexList(pText);
    }
}

void destroyText(Text *pText)
{
    eraseText(pText);

    if (pText->pString)
    {
        free(pText->pString);
        pText->pString = NULL;
    }
}

void handleEscapeSequences(Text *pText, int *iterator, int *pPrevX, int *pPrevY, Color *pTempColor, bool *pCustomColorPrint, bool *firstOnLine)
{
    int len = strlen(pText->pString);

    if (*iterator < len - 1) // If there's still enough characters left for an escape sequence
    {
        switch (pText->pString[*iterator + 1])
        {
            case '$':
                *iterator += SKIP_ESCAPE_SEQUENCE_MARKER;

               if (!*pCustomColorPrint)
                    *pPrevX = renderCharacter(pText, pText->pString[*iterator], pText->color, pText->relative, *pPrevX, *pPrevY, *firstOnLine);
                else
                    *pPrevX = renderCharacter(pText, pText->pString[*iterator], *pTempColor, pText->relative, *pPrevX, *pPrevY, *firstOnLine);

                *firstOnLine = False;
            break;

            case '_':
                (*iterator) ++;
                *pPrevX += pText->pFont->wordSpacing;
            break;

            case 'c': // If there's is still enough characters left for a color sequence
            case 'C':
                if (*iterator < len - COLOR_ESCAPE_SEQUENCE_LENGTH)
                {
                    *iterator += SKIP_COLOR_ESCAPE_SEQUENCE;
                    *pTempColor = extractHexColor(&pText->pString[*iterator]);
                    *pCustomColorPrint = True;
                    *iterator += SKIP_COLOR_VALUES;
                }
                else
                    *iterator += len - *iterator; // Skip to the end of the string
            break;

            case 'x': // Handle ending escape sequences
            case 'X':
                if (*iterator < len - ENDING_ESCAPE_SEQUENCE_LENGTH)
                {
                    switch (pText->pString[*iterator + 2])
                    {
                        case 'c': // Custom color end
                        case 'C':
                            *pCustomColorPrint = False;
                        break;
                    }

                    *iterator += SKIP_ENDING_ESCAPE_SEQUENCE;
                }
                else
                    *iterator += len - *iterator; // Skip to the end of the string
            break;
        }
    }
}

void skipEscapeSequences(char *pString, int *iterator, Color *pTempColor, bool *pCustomColorPrint)
{
    int len = strlen(pString);

    if (*iterator < len - 1) // If there's still enough characters left for an escape sequence
    {
        switch (pString[*iterator + 1])
        {
            case '$':
                *iterator += SKIP_ESCAPE_SEQUENCE_MARKER;
            break;

            case '_':
                (*iterator) ++;
            break;

            case 'c': // If there's is still enough characters left for a color sequence
            case 'C':
                if (*iterator < len - COLOR_ESCAPE_SEQUENCE_LENGTH)
                {
                    *iterator += SKIP_COLOR_ESCAPE_SEQUENCE;
                    if (pTempColor) *pTempColor = extractHexColor(&pString[*iterator]);
                    if (pCustomColorPrint) *pCustomColorPrint = True;
                    *iterator += SKIP_COLOR_VALUES;
                }
                else
                    *iterator += len - *iterator; // Skip to the end of the string
            break;

            case 'x': // Handle ending escape sequences
            case 'X':
                if (*iterator < len - ENDING_ESCAPE_SEQUENCE_LENGTH)
                {
                    switch (pString[*iterator + 2])
                    {
                        case 'c': // Custom color end
                        case 'C':
                            if (pCustomColorPrint) *pCustomColorPrint = False;
                        break;
                    }

                    *iterator += SKIP_ENDING_ESCAPE_SEQUENCE;
                }
                else
                    *iterator += len - *iterator; // Skip to the end of the string
            break;
            default: break;
        }
    }
}

int getLineStartX(Text *pText, int pos)
{

}

void renderText(Text *pText)
{
    int i;
    int len = strlen(pText->pString);
    int currentLine = 0;
    int prevX;
    int prevY;
    Font *pFont = pText->pFont;

    Color tempColor;
    bool printWithCustomColor = False;
    bool firstOnLine = True;

    if (pText->rendered)
        eraseText(pText);

    prevX = getLineStartX(pText, 0);
    switch (pText->alignment)
    {
        case ALIGN_LEFT: prevX = pText->beginX; break;
        case ALIGN_CENTER: prevX = pText->beginX - calculateLineTailWidth(pText, 0) * 0.5; break;
        case ALIGN_RIGHT: prevX = pText->beginX - calculateLineTailWidth(pText, 0); break;
    }

    prevY = (pText->fitInArea) ? pText->textAreaScrollPosition : pText->beginY;

    for (i = 0; i < len; i ++)
    {
        if (pText->fitInArea)
        {
            int skipTo;

            if (prevY < pText->textAreaTopY + pFont->lineSpacing * 0.5)
            {
                skipTo = parseToNextLineStart(pText, i, &tempColor, &printWithCustomColor);

                if (skipTo > -1)
                {
                    i = skipTo - 1;
                    currentLine ++;
                    firstOnLine = True;

                    switch (pText->alignment)
                    {
                        case ALIGN_LEFT: prevX = pText->beginX; break;
                        case ALIGN_CENTER: prevX = pText->beginX - calculateLineTailWidth(pText, skipTo) * 0.5; break;
                        case ALIGN_RIGHT: prevX = pText->beginX - calculateLineTailWidth(pText, skipTo); break;
                    }

                    prevY += pFont->lineSpacing;
                    continue;
                }
            }
            else if (prevY > pText->textAreaBottomY - pFont->lineSpacing * 0.5) break;
        }

        switch (pText->pString[i])
        {
            case ' ': prevX += pFont->wordSpacing; break;

            case '\n':
            case '\v':
                currentLine ++;
                firstOnLine = True;

                switch (pText->alignment)
                {
                    case ALIGN_LEFT: prevX = pText->beginX; break;
                    case ALIGN_CENTER: prevX = pText->beginX - calculateLineTailWidth(pText, i + 1) * 0.5; break;
                    case ALIGN_RIGHT: prevX = pText->beginX - calculateLineTailWidth(pText, i + 1); break;
                }

                prevY += pFont->lineSpacing;
            break;
            case '\t':
                if (pFont->indentation != 0 && pText->alignment == ALIGN_LEFT)
                {
                    int tempIndentation = ceil((prevX - pText->beginX) / pFont->indentation) * pFont->indentation + pFont->indentation + pText->beginX;
                    prevX = tempIndentation;
                }
            break;
            case '$': // Handle custom escape sequences
                handleEscapeSequences(pText, &i, &prevX, &prevY, &tempColor, &printWithCustomColor, &firstOnLine);
            break;

            default:
                if (!printWithCustomColor)
                    prevX = renderCharacter(pText, pText->pString[i], pText->color, pText->relative, prevX, prevY, firstOnLine);
                else
                    prevX = renderCharacter(pText, pText->pString[i], tempColor, pText->relative, prevX, prevY, firstOnLine);

                firstOnLine = False;
            break;
        }
    }

    pText->rendered = True;
    pText->lastRenderFrame = frame % 2;
}

int renderCharacter(Text *pText, char printChar, Color color, bool relative, int xPos, int yPos, bool firstOnLine)
{
    Actor *a, *pParent = getclone(pText->parentCName);
    int letterNum = fromASCII(printChar);
    double prevX = xPos;
    Font *pFont = pText->pFont;

    prevX += floor(pFont->fontCharWidths[letterNum] * 0.5) + pFont->letterSpacing * (!firstOnLine);

    if (!pText->fitInArea || (pText->fitInArea && yPos >= pText->textAreaTopY + pFont->lineSpacing * 0.5 && yPos <= pText->textAreaBottomY - pFont->lineSpacing * 0.5))
    {
        if (pText->relative && actorExists2(pParent))
            a = CreateActor(typeActorName[frame % 2], pFont->fontName, pText->parentCName, "(none)", prevX, yPos, true);
        else
            a = CreateActor(typeActorName[frame % 2], pFont->fontName, "(none)", "(none)", prevX, yPos, true);

        storeLetterCloneindex(pText, a->cloneindex);

        if (actorExists2(pParent))
            ChangeParent(a->clonename, pText->parentCName);

        colorActor(a, color);
        ChangeZDepth(a->clonename, pText->zDepth);

        a->animpos = letterNum;
    }

    prevX += ceil(pFont->fontCharWidths[letterNum] * 0.5);

    return prevX;
}


// ..\source\geui\04-fonts.c
Font defTitleFont =
{
     1, // Letter spacing in pixels
     6, // Word spacing in pixels
    25, // Line spacing in pixels
    28, // Indentation in pixels
    "segoeui20", // Font animation name
    {   // Character widths
         4,  6, 12,  9, 16, 15,  2,  5,  5,  8,
        10,  3,  6,  4,  9, 10,  6, 10,  9, 11,
         9, 10, 10, 10, 10,  4,  4, 10, 10, 10,
         8, 16, 14,  9, 12, 13,  9,  9, 12, 12,
         2,  6, 10,  8, 16, 12, 14,  9, 15, 11,
        10, 10, 12, 13, 20, 12, 12, 12,  5,  8,
         5, 10, 10,  4,  9, 10,  8, 10, 10,  7,
        10, 10,  3,  7,  9,  2, 15, 10, 10, 10,
        10,  6,  8,  6, 10, 10, 14, 10, 10, 10,
         6,  2,  6, 10
    }
};

Font defLabelFont =
{
     1, // Letter spacing in pixels
     4, // Word spacing in pixels
    19, // Line spacing in pixels
    21, // Indentation in pixels
    "segoeui16", // Font animation name
    {   // Character widths
         2,  4, 10,  8, 12, 13,  2,  5,  5,  6,
         8,  3,  6,  2,  8,  8,  5,  8,  8,  9,
         8,  8,  8,  8,  8,  2,  3,  8,  8,  8,
         6, 14, 10,  8, 10, 10,  7,  7, 10, 10,
         2,  5,  9,  7, 12, 10, 12,  8, 13,  9,
         8,  8, 10, 10, 16, 10, 10, 10,  4,  8,
         4,  8,  8,  4,  7,  8,  7,  8,  8,  6,
         8,  8,  2,  5,  7,  2, 12,  8,  8,  8,
         8,  5,  6,  6,  8,  8, 12,  8,  8,  8,
         4,  2,  4,  8
    }
};

Font defTextFont =
{
     1, // Letter spacing in pixels
     4, // Word spacing in pixels
    19, // Line spacing in pixels
    21, // Indentation in pixels
    "segoeui16", // Font animation name
    {   // Character widths
         2,  4, 10,  8, 12, 13,  2,  5,  5,  6,
         8,  3,  6,  2,  8,  8,  5,  8,  8,  9,
         8,  8,  8,  8,  8,  2,  3,  8,  8,  8,
         6, 14, 10,  8, 10, 10,  7,  7, 10, 10,
         2,  5,  9,  7, 12, 10, 12,  8, 13,  9,
         8,  8, 10, 10, 16, 10, 10, 10,  4,  8,
         4,  8,  8,  4,  7,  8,  7,  8,  8,  6,
         8,  8,  2,  5,  7,  2, 12,  8,  8,  8,
         8,  5,  6,  6,  8,  8, 12,  8,  8,  8,
         4,  2,  4,  8
    }
};


// ..\source\geui\05-geui-style.c
typedef struct StyleStruct
{
    char guiAnim[100];

    short tileWidth;
    short tileHeight;

    Font *titleFont;
    Font *labelFont;
    Font *textFont;

    short padding;
    short focusWidth;

    Color titleBgColor;
    Color windowBgColor;
    Color inputBgColor;

    Color titleColor;
    Color labelColor;
    Color textColor;

    Color buttonColor;
    Color buttonHilitColor;
    Color buttonPressedColor;

    Color focusColor;
}Style;

Style defStyle;
Style *setDimensions;

Style createStyle(const char guiAnim[100], Font *titleFont, Font *labelFont, Font *textFont,
                  short padding, short focusWidth, Color titleBgColor, Color windowBgColor,
                  Color inputBgColor, Color titleColor, Color labelColor, Color textColor,
                  Color buttonColor, Color buttonHilitColor, Color buttonPressedColor,
                  Color focusColor);

void getTileDimensions(Style *style)
{
    Actor*a=CreateActor("a_gui", style->guiAnim, "(none)", "(none)", 0, 0, true);
    setDimensions = style;
    SendActivationEvent(a->clonename);
    DestroyActor(a->clonename);
}

void setTileDimensions()
{
    (*setDimensions).tileWidth = width;
    (*setDimensions).tileHeight = height;
    setDimensions = NULL;
}

Style createStyle(const char guiAnim[100], Font *titleFont, Font *labelFont, Font *textFont,
                  short padding, short focusWidth, Color titleBgColor, Color windowBgColor,
                  Color inputBgColor, Color titleColor, Color labelColor, Color textColor,
                  Color buttonColor, Color buttonHilitColor, Color buttonPressedColor,
                  Color focusColor)
{
    Style new;

    strcpy(new.guiAnim, guiAnim);
    new.titleFont = titleFont;
    new.labelFont = labelFont;
    new.textFont = textFont;
    new.padding = padding;
    new.focusWidth = focusWidth;
    new.titleBgColor = titleBgColor;
    new.windowBgColor = windowBgColor;
    new.inputBgColor = inputBgColor;
    new.titleColor = titleColor;
    new.labelColor = labelColor;
    new.textColor = textColor;
    new.buttonColor = buttonColor;
    new.buttonHilitColor = buttonHilitColor;
    new.buttonPressedColor = buttonPressedColor;
    new.focusColor = focusColor;

    return new;
}

//This function defines and creates a new style
//windowActor - the name of the actor to be used as the background of the windows
//titleActor - the name of the actor to be used as the title text for the windows
//textActor - the name of the actor to be used as the text actor for the windows' contents
//buttonActor - the name of the actor to be used as the button actor for the windows
//windowAnim - the animation of the windowActor that should be used for the windows
//buttonAnim - the animation of the buttonActor that should be used for the buttons
//borderSize - the amount of pixels for the left, right and bottom borders of the windows
//topBarSize - the amount of pixels for the top border bar of the windows
//bgColor - the color to be used on the windowActor
//titleColor - the color to be used on the titleActor
//textColor - the color to be used on the textActor
//buttonColor - the color to be used on the buttonActor when the button is in idle state
//buttonHilitColor - the color to be used on the buttonActor when the button is highlighted
//buttonPressedColor - the color to be used on the buttonActor when the button is pressed
/*Style createStyle(char *windowActor, char *titleActor, char *textActor, char *buttonActor,
                  char *windowAnim, char *buttonAnim, int borderSize, int topBarSize,
                  Color bgColor, Color titleColor, Color textColor, Color buttonColor,
                  Color buttonHilitColor, Color buttonPressedColor)
{
    Style ptr;

    strcpy(ptr.windowActor, windowActor);
    strcpy(ptr.titleActor, titleActor);
    strcpy(ptr.textActor, textActor);
    strcpy(ptr.buttonActor, buttonActor);

    strcpy(ptr.windowAnim, windowAnim);
    strcpy(ptr.buttonAnim, buttonAnim);

    ptr.borderSize = borderSize;
    ptr.topBarSize = topBarSize;

    ptr.bgColor = bgColor;
    ptr.titleColor = titleColor;
    ptr.textColor = textColor;
    ptr.buttonColor = buttonColor;
    ptr.buttonHilitColor = buttonHilitColor;
    ptr.buttonPressedColor = buttonPressedColor;

    return ptr;
}*/


// ..\source\geui\06-geui-utilities.c
Actor *getTile(long index);
void updateIndexBounds(long *low, long *hi, long val);
int calculateAnimpos(short w, short h, short i, short j);

Actor *getTile(long index)
{
    return gc2("a_gui", index);
}

void updateIndexBounds(long *low, long *hi, long val)
{
    if (*low == -1)
        *low = val;
    if (*hi < val)
        *hi = val;
}

int calculateAnimpos(short w, short h, short i, short j)
{
    short pw = (i > 0) + (i == w - 1); // column 0, 1 or 2
    short ph = (j > 0) + (j == h - 1); // row 0, 1 or 2

    // Array of possible outcomes:
    // 0, 1, 2,
    // 3, 4, 5,
    // 6, 7, 8

    return ph * 3 + pw;

    // The values given to different parts of a window:
    // 0, 1, 1, 1, 2,
    // 3, 4, 4, 4, 5,
    // 3, 4, 4, 4, 5,
    // 6, 7, 7, 7, 8
}


// ..\source\geui\10-geui-header.c
typedef struct LayoutStruct
{
    short row;      // row in a panel
    short col;      // column in a panel
    short width;
    short height;
    short startx;
    short starty;
}Layout;

typedef enum ItemTypeEnum
{
    GEUI_Text,
    GEUI_Button,
    GEUI_Input,
    GEUI_Panel,
    GEUI_Embedder
}ItemType;

struct WindowStruct;
struct PanelStruct;
struct InputFieldStruct;

#define GEUI_DEFAULT_CARET_BLINK_RATE 2

typedef struct BlinkingCaretStruct
{
    float blinkRate;
    float timer;
    short state;
    char actorCName[256];
    Text *pText;
}BlinkingCaret;

typedef struct TileIndicesStruct
{
    long first;
    long last;
}TileIndices;

TileIndices noIndices = { -1, -1 };

typedef enum InputTypeEnum
{
    GEUI_TextInput,
    GEUI_IntInput,
    GEUI_RealInput
}InputType;

typedef struct TextInputSettingsStruct
{
    int empty;
}TextInputSettings;

typedef struct IntInputSettingsStruct
{
    int minVal;
    int maxVal;
    int defaultValue;
}IntInputSettings;

typedef struct RealInputSettingsStruct
{
    float minVal;
    float maxVal;
    float defaultValue;
    short precisionDigits;
}RealInputSettings;

typedef union InputSettingsDataUnion
{
    TextInputSettings textInput;
    IntInputSettings intInput;
    RealInputSettings realInput;
}InputSettingsData;

typedef union InputValueUnion
{
    char *textValue;
    int intValue;
    float realValue;
}InputValue;

typedef struct InputSettingsStruct
{
    InputType type;
    InputSettingsData data;
    void (*settingsFunction)(struct InputFieldStruct *);
    void (*valueFunction)(struct InputFieldStruct *);
}InputSettings;

typedef enum KeyboardLayoutEnum
{
    GEUI_KeyboardUS = 0,
    GEUI_KeyboardFI,
    GEUI_KeyboardLayoutCount
}KeyboardLayout;

typedef struct InputFieldStruct
{
    InputSettings settings;
    InputValue value;

    Text text;
    BlinkingCaret caret;

    TileIndices tiles;
}InputField;

typedef struct WindowItemStruct
{
    int index;          // item index
    char tag[256];      // item identifier tag

    ItemType type;      // item type
    bool focusable;

    union ItemDataUnion // item data union for different item types
    {
        Text text;
        struct ButtonItem
        {
            Text text;
            char state;
            TileIndices tiles;
            void (*actionFunction)(struct WindowStruct *, struct WindowItemStruct *);
        }button;
        InputField input;
        struct PanelStruct *panel;
        struct EmbedderItem
        {
            char actorCName[256];
        }embedder;
    }data;

    Layout layout;
    struct PanelStruct *myPanel;    // pointer to parent panel
    struct WindowStruct *parent;    // pointer to parent window
    struct WindowItemStruct *next;  // pointer to next item in list
}WindowItem;

typedef struct PanelStruct
{
    int index;      // panel index
    int iIndex;     // next available item index
    short rows;     // number of rows in panel
    short cols;     // number of columns in panel
    short width;
    short height;
    struct WindowStruct *parent;
    struct WindowItemStruct *iList;
}Panel;

typedef struct WindowStruct
{
    int index;          // window index
    int pIndex;         // next available panel index
    char tag[256];      // window identifier tag
    bool isOpen;        // is window currently open or not
    Style style;        // window style
    double zDepth;      // window z depth
    char parentCName[256]; // clonename of the window parent actor
    TileIndices tiles;          // cloneindices of the window tiles
    Panel mainPanel;            // window main panel
    struct WindowStruct *next;  // pointer to next window in list
}Window;

#define GEUI_MOUSE_UP 0
#define GEUI_MOUSE_DOWN 1

const unsigned long GEUI_TITLE_BAR  = (1 << 0);
const unsigned long GEUI_FAKE_ACTOR = (1 << 1);
const unsigned long GEUI_CLICKED    = (1 << 2);
const unsigned long GEUI_INPUT_BG   = (1 << 3);
const unsigned long GEUI_CARET      = (1 << 4);

enum mouseButtonsEnum // Used as array indices, don't touch!
{
    GEUI_MOUSE_LEFT = 0,
    GEUI_MOUSE_RIGHT,
    GEUI_MOUSE_MIDDLE,
    GEUI_MOUSE_WHEEL_UP,
    GEUI_MOUSE_WHEEL_DOWN,
    GEUI_MOUSE_BUTTONS     // Number of supported mouse buttons (5)
};

#define GEUI_CLONENAME_SIZE 42

#define DEFAULT_WINDOW_ZDEPTH 0.8 // SABRE WORLD EDITOR (SWE) MODIFICATION, original value: 0.5
#define ACTIVE_WINDOW_ZDEPTH 0.9 // SABRE WORLD EDITOR (SWE) MODIFICATION, original value: 0.6
#define DEFAULT_ITEM_ZDEPTH 0.3
#define WINDOW_TILE_ZDEPTH 0.1
#define FAKE_ACTOR_ZDEPTH 0.2

#define USE_DEFAULT_STYLE GEUIController.sDefault

struct GEUIControllerStruct
{
    int wIndex;
    int topIndex;
    Style sDefault;
    KeyboardLayout kbLayout;
    Window *wList;

    WindowItem *focus;
    TileIndices focusTiles;

    Actor *mButtonActors[GEUI_MOUSE_BUTTONS];
    char mButtonTopActorCName[GEUI_MOUSE_BUTTONS][GEUI_CLONENAME_SIZE];
    int mButtonActorCount[GEUI_MOUSE_BUTTONS];
    int mButtonState[GEUI_MOUSE_BUTTONS];
    enum mouseButtonsEnum activeButton;

    // BEGIN SABRE WORLD EDITOR (SWE) MODIFICATION
    int openWindowCount;
    // END SABRE WORLD EDITOR (SWE) MODIFICATION
}GEUIController;

#define CURRENT_KEYBOARD GEUIController.kbLayout


// ..\source\geui\14-geui-input-caret.c
void setCaretBlinkRate(BlinkingCaret *caret, float blinkRate);

void initializeCaret(BlinkingCaret *caret)
{
    caret->pText = NULL;
    caret->state = 0;
    strcpy(caret->actorCName, "");
    setCaretBlinkRate(caret, GEUI_DEFAULT_CARET_BLINK_RATE);
}

void setCaretActor(BlinkingCaret *caret, char *actorCName, Text *pText, Color color)
{
    strcpy(caret->actorCName, actorCName);
    caret->pText = pText;
    colorActorByName(actorCName, color);
}

void eraseCaret(BlinkingCaret *caret)
{
    DestroyActor(caret->actorCName);
    strcpy(caret->actorCName, "");
}

void setCaretBlinkRate(BlinkingCaret *caret, float blinkRate)
{
    caret->blinkRate = blinkRate;
    caret->timer = 0.0f;
}

void showCaret(BlinkingCaret *caret)
{
    VisibilityState(caret->actorCName, ENABLE);
}

void hideCaret(BlinkingCaret *caret)
{
    VisibilityState(caret->actorCName, DISABLE);
}

void resetCaretBlink(BlinkingCaret *caret)
{
    caret->timer = 0.0f;
    caret->state = 0;
    transp = caret->state;
}

void doCaretBlink(BlinkingCaret *caret)
{
    caret->timer += caret->blinkRate / (float)max(real_fps, 1);

    if (caret->timer >= 1.0f)
    {
        caret->state = !caret->state;
        caret->timer = 0.0f;
    }

    transp = caret->state;
}

void updateCaretPosition(BlinkingCaret *caret)
{
    Actor *a = getclone(caret->actorCName);

    if (actorExists2(a))
    {
        updateTextDimensions(caret->pText);
        a->x = caret->pText->beginX + caret->pText->width + caret->pText->pFont->letterSpacing + ceil(a->width * 0.5);
        a->y = caret->pText->beginY;
        resetCaretBlink(caret);
    }
}


// ..\source\geui\16-geui-input-field.c
#define GEUI_KEYBOARD_MAPPING_SIZE GEUI_KeyboardLayoutCount + 1
#define GEUI_MAX_VALID_KEYS_LIMIT 26

#define GEUI_NUM_STRING_LENGTH 30

const short validKeysForInputType[][GEUI_MAX_VALID_KEYS_LIMIT] =
{
    { -1 },
    {
        KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5,
        KEY_6, KEY_7, KEY_8, KEY_9, KEY_MINUS,
        KEY_PAD_0, KEY_PAD_1, KEY_PAD_2, KEY_PAD_3, KEY_PAD_4, KEY_PAD_5,
        KEY_PAD_6, KEY_PAD_7, KEY_PAD_8, KEY_PAD_9, KEY_PAD_MINUS, KEY_BACKSPACE,
    },
    {
        KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5,
        KEY_6, KEY_7, KEY_8, KEY_9, KEY_MINUS,
        KEY_PAD_0, KEY_PAD_1, KEY_PAD_2, KEY_PAD_3, KEY_PAD_4, KEY_PAD_5,
        KEY_PAD_6, KEY_PAD_7, KEY_PAD_8, KEY_PAD_9, KEY_PAD_MINUS, KEY_BACKSPACE,
        KEY_PERIOD, KEY_PAD_PERIOD, KEY_e
    }
};

int checkKeyValidity(short key, InputType type)
{
    short i;

    if (type == GEUI_TextInput)
        return 1;

    for (i = 0; i < GEUI_MAX_VALID_KEYS_LIMIT; i++)
    {
        if (validKeysForInputType[type][i] == key)
            return 1;
    }

    return 0;
}

int greedyAtoi(const char *str)
{
    int val = 0;
    size_t offset = 0;
    size_t len = strlen(str);

    do
    {
        val = atoi(&str[offset]);
        offset++;
    } while (!val && offset < len);

    return val;
}

float greedyAtof(const char *str, int *readCount)
{
    int count = 0;
    float val = 0;
    size_t offset = 0;
    size_t len = strlen(str);

    do
    {
        count = sscanf(&str[offset], "%f%*s", &val, NULL);
        offset++;
    } while (!val && offset < len);

    if (readCount)
        *readCount = count;

    return val;
}

const short keyboardLocalizations[][GEUI_KEYBOARD_MAPPING_SIZE] =
{
    { KEY_0,            ')',    '='     },
    { KEY_1,            '!',    '!'     },
    { KEY_2,            '@',    '"'     },
    { KEY_3,            '#',    '#'     },
    { KEY_4,            '$',    '_'     },
    { KEY_5,            '%',    '%'     },
    { KEY_6,            '^',    '&'     },
    { KEY_7,            '&',    '/'     },
    { KEY_8,            '*',    '('     },
    { KEY_9,            '(',    ')'     },
    { KEY_EQUALS,       '=',    '+'     },
    { KEY_SLASH,        '/',    '\''    },
    { KEY_PAD_PERIOD,   '.',    '.'     }
};

char getLocalizedKeyboardChar(int key, KeyboardLayout kbLayout)
{
    int i;

    for (i = 0; i < sizeof keyboardLocalizations / sizeof (short[GEUI_KEYBOARD_MAPPING_SIZE]); i++)
    {
        if (keyboardLocalizations[i][0] == key)
        {
            return keyboardLocalizations[i][(size_t)kbLayout + 1];
        }
    }

    return '\0';
}

void refreshValue(InputField *field)
{
    char tempNumText[GEUI_NUM_STRING_LENGTH];

    switch (field->settings.type)
    {
        case GEUI_TextInput: break;
        case GEUI_IntInput:
            sprintf(tempNumText, "%d", field->value.intValue);
            setTextText(&field->text, tempNumText);
        break;
        case GEUI_RealInput:
            sprintf(tempNumText, "%.*f", field->settings.data.realInput.precisionDigits, field->value.realValue);
            field->value.realValue = greedyAtof(tempNumText, NULL);
            setTextText(&field->text, tempNumText);
        break;
    }
}

void refreshInputValue(InputField *field)
{
    refreshValue(field);
    refreshText(&field->text);
    updateCaretPosition(&field->caret);
}

void handleInputFieldInput(InputField *field, short key)
{
    int readCount = 0;
    char tempNumText[GEUI_NUM_STRING_LENGTH];
    char newChar = '\0';
    char rest[30] = "";
    char *keys = GetKeyState();
    short shift = (keys[KEY_LSHIFT] || keys[KEY_RSHIFT]);
    short ctrl  = (keys[KEY_LCTRL] || keys[KEY_RCTRL]);
    short alt   = (keys[KEY_LALT] || keys[KEY_RALT]);

    if (key == KEY_RETURN)
    {
        field->settings.settingsFunction(field);
        field->settings.valueFunction(field);
    }

    if (!checkKeyValidity(key, field->settings.type))
    {
        DEBUG_MSG("key not valid");
        return;
    }

    if (key >= KEY_a && key <= KEY_z)
    {
        newChar = key - ('a' - 'A') * shift;
    }
    else if (key >= KEY_0 && key <= KEY_9)
    {
        newChar = shift ? getLocalizedKeyboardChar(key, CURRENT_KEYBOARD)
                        : key;
    }
    else if (key >= KEY_PAD_0 && key <= KEY_PAD_9)
    {
        newChar = key - (KEY_PAD_0 - KEY_0);
    }
    else
    {
        switch (key)
        {
            case KEY_SPACE:         newChar = ' '; break;
            case KEY_COMMA:         newChar = shift ? ';' : ','; break;
            case KEY_PERIOD:        newChar = shift ? ':' : '.'; break;
            case KEY_MINUS:         newChar = shift ? '_' : '-'; break;
            case KEY_PAD_DIVIDE:    newChar = '/'; break;
            case KEY_PAD_MULTIPLY:  newChar = '*'; break;
            case KEY_PAD_MINUS:     newChar = '-'; break;
            case KEY_PAD_PLUS:      newChar = '+'; break;
            case KEY_TAB:           newChar = '\t'; break;
            case KEY_RETURN:        newChar = '\n'; break;
            case KEY_EQUALS:
            case KEY_SLASH:
            case KEY_PAD_PERIOD:
                newChar = getLocalizedKeyboardChar(key, CURRENT_KEYBOARD);
            break;
            case KEY_BACKSPACE:
                if (ctrl)
                    setTextText(&field->text, "");
                else
                    newChar = '\b';
            break;
        }
    }

    concatenateCharToText(&field->text, newChar);

    switch (field->settings.type)
    {
        case GEUI_IntInput:
        case GEUI_RealInput:
            field->settings.valueFunction(field);
        break;
    }
}


// ..\source\geui\20-geui-window-item.c
// TODO: make functions return error codes instead of just exiting
// without doing anything, which can be difficult to debug

// from geui-panel.c
void closePanel(Panel *panel);
void destroyPanel(Panel *panel);
Panel *getPanelByIndex(Panel *panel, int index);

void updateGuiTileIndices(TileIndices *indices, long newIndex);
void eraseGuiTiles(TileIndices *indices);
void colorGuiTiles(TileIndices indices, Color color);
WindowItem *initNewItem(ItemType type, Window *window, Panel *panel, char tag[256]);
WindowItem *addItemToWindow(WindowItem *ptr);
WindowItem *addText(Window *window, Panel *panel, char tag[256], char *string, short maxWidth);
WindowItem *addButton(Window *window, Panel *panel, char tag[256], char *string, void (*actionFunction)(Window *, WindowItem *));
WindowItem *addInputField(Window *window, Panel *panel, char tag[256], const char *string, InputSettings settings, short maxWidth);
WindowItem *addPanel(Window *window, Panel *panel, char tag[256]);
WindowItem *addEmbedder(Window *window, Panel *panel, char tag[256], const char *actorName);
void setPosition(WindowItem *this, short row, short col);
WindowItem *getItemFromPanelByTag(Panel *panel, char tag[256]);
WindowItem *getItemByTag(Window *window, char tag[256]);
WindowItem *getItemFromPanelByIndex(Panel *panel, int index);
WindowItem *getItemByIndex(Window *window, int index);
WindowItem *getNextFocusableItem(WindowItem *ptr);
WindowItem *focusItem(WindowItem *ptr);
void blurItem(WindowItem *ptr);
void buildFocus(WindowItem *ptr);
void eraseFocus();
void buildItems(Panel *panel);
void buildItem(WindowItem *ptr);
void buildText(WindowItem *ptr);
void buildPanel(WindowItem *ptr);
void buildButtonText(WindowItem *ptr);
void buildButton(WindowItem *ptr);
Actor *buildCaret(WindowItem *ptr, Text *pText, BlinkingCaret *caret);
void buildInputFieldBackground(WindowItem *ptr, TileIndices *tiles);
void buildInputField(WindowItem *ptr);
void buildEmbedder(WindowItem *ptr);
void eraseWindowItem(WindowItem *ptr);
void destroyWindowItem(WindowItem *ptr);

void updateGuiTileIndices(TileIndices *indices, long newIndex)
{
    updateIndexBounds(&indices->first, &indices->last, newIndex);
}

void eraseGuiTiles(TileIndices *indices)
{
    destroyClones("a_gui", indices->first, indices->last);
    *indices = noIndices;
}

void colorGuiTiles(TileIndices indices, Color color)
{
    colorClones("a_gui", indices.first, indices.last, color);
}

WindowItem *initNewItem(ItemType type, Window *window, Panel *panel, char tag[256])
{
    WindowItem *ptr = NULL;

    if (!panel) { DEBUG_MSG_FROM("panel is NULL", "initNewItem"); return NULL; }

    ptr = malloc(sizeof *ptr);

    if (!ptr) { DEBUG_MSG_FROM("memory allocation failed", "initNewItem"); return NULL; }

    ptr->type = type;
    ptr->focusable = False;
    ptr->index = panel->iIndex ++;
    strcpy(ptr->tag, tag);
    ptr->myPanel = panel;
    ptr->parent = window;
    ptr->layout.row = 0;
    ptr->layout.col = 0;
    ptr->layout.width = 0;
    ptr->layout.height = 0;
    ptr->layout.startx = 0;
    ptr->layout.starty = 0;

    return ptr;
}

WindowItem *addItemToWindow(WindowItem *ptr)
{
    if (!ptr) { DEBUG_MSG_FROM("item is NULL", "addItemToWindow"); return NULL; }

    ptr->next = ptr->myPanel->iList;
    ptr->myPanel->iList = ptr;

    return ptr;
}

WindowItem *addText(Window *window, Panel *panel, char tag[256], char *string, short maxWidth)
{
    WindowItem *ptr = initNewItem(GEUI_Text, window, panel, tag);
    if (!ptr) { DEBUG_MSG_FROM("item is NULL", "addText"); return NULL; }

    ptr->data.text = createText(string, window->style.textFont, "(none)", ABSOLUTE, 0, 0);
    setTextColor(&ptr->data.text, window->style.textColor);
    setTextZDepth(&ptr->data.text, DEFAULT_ITEM_ZDEPTH);

    if (maxWidth > 0)
        fitTextInWidth(&ptr->data.text, maxWidth);

    ptr->layout.width = ptr->data.text.width;
    ptr->layout.height = ptr->data.text.height;

    return addItemToWindow(ptr);
}

WindowItem *addButton(Window *window, Panel *panel, char tag[256], char *string, void (*actionFunction)(Window *, WindowItem *))
{
    WindowItem *ptr = initNewItem(GEUI_Button, window, panel, tag);
    if (!ptr) { DEBUG_MSG_FROM("item is NULL", "addButton"); return NULL; }

    ptr->focusable = True;
    ptr->data.button.text = createText(string, window->style.textFont, "(none)", ABSOLUTE, 0, 0);
    setTextColor(&ptr->data.button.text, window->style.textColor);
    setTextZDepth(&ptr->data.button.text, DEFAULT_ITEM_ZDEPTH);
    ptr->data.button.state = 0;
    ptr->data.button.tiles = noIndices;
    ptr->data.button.actionFunction = actionFunction;

    ptr->layout.width = ptr->data.button.text.width + ptr->parent->style.tileWidth * 2;
    ptr->layout.height = ptr->parent->style.tileHeight;

    return addItemToWindow(ptr);
}

void enforceTextInputSettings(InputField *input)
{
    return;
}

void updateTextInputValue(InputField *input)
{
    return;
}

InputSettings createTextInputSettings()
{
    InputSettings settings;

    settings.type = GEUI_TextInput;
    (settings.settingsFunction = enforceTextInputSettings);
    (settings.valueFunction = updateTextInputValue);

    settings.data.textInput.empty = 0;

    return settings;
}

void enforceIntInputSettings(InputField *input)
{
    input->value.intValue = iLimit(input->value.intValue, input->settings.data.intInput.minVal, input->settings.data.intInput.maxVal);
    refreshInputValue(input);
}

void updateIntInputValue(InputField *input)
{
    if (greedyAtoi(input->text.pString) != 0)
    {
        input->value.intValue = greedyAtoi(input->text.pString);
        refreshInputValue(input);
    }
}

InputSettings createIntInputSettings(int minVal, int maxVal, int defaultValue)
{
    InputSettings settings;

    settings.type = GEUI_IntInput;
    (settings.settingsFunction = enforceIntInputSettings);
    (settings.valueFunction = updateIntInputValue);

    settings.data.intInput.minVal = minVal;
    settings.data.intInput.maxVal = maxVal;
    settings.data.intInput.defaultValue = iLimit(defaultValue, minVal, maxVal);

    return settings;
}

void enforceRealInputSettings(InputField *input)
{
    input->value.realValue = fLimit(input->value.realValue, input->settings.data.realInput.minVal, input->settings.data.realInput.maxVal);
    refreshInputValue(input);
}

void updateRealInputValue(InputField *input)
{
    int readCount = 0;

    if (greedyAtof(input->text.pString, &readCount) != 0)
    {
        input->value.realValue = greedyAtof(input->text.pString, NULL);

        if (readCount > 1)
            refreshInputValue(input);
    }
}

InputSettings createRealInputSettings(float minVal, float maxVal, float defaultValue, short precisionDigits)
{
    InputSettings settings;

    settings.type = GEUI_RealInput;
    (settings.settingsFunction = enforceRealInputSettings);
    (settings.valueFunction = updateRealInputValue);

    settings.data.realInput.minVal = minVal;
    settings.data.realInput.maxVal = maxVal;
    settings.data.realInput.defaultValue = dfLimit(defaultValue, minVal, maxVal);
    settings.data.realInput.precisionDigits = siLimit(precisionDigits, 0, 10);

    return settings;
}

WindowItem *addInputField(Window *window, Panel *panel, char tag[256], const char *string, InputSettings settings, short maxWidth)
{
    WindowItem *ptr = initNewItem(GEUI_Input, window, panel, tag);
    if (!ptr) { DEBUG_MSG_FROM("item is NULL", "addInputField"); return NULL; }

    ptr->focusable = True;
    initializeCaret(&ptr->data.input.caret);
    ptr->data.input.text = createText(string, window->style.textFont, "(none)", ABSOLUTE, 0, 0);
    setTextColor(&ptr->data.input.text, window->style.textColor);
    setTextZDepth(&ptr->data.input.text, DEFAULT_ITEM_ZDEPTH);
    ptr->data.input.settings = settings;

    switch (settings.type)
    {
        case GEUI_TextInput: ptr->data.input.value.textValue = ptr->data.input.text.pString; break;
        case GEUI_IntInput: ptr->data.input.value.intValue = settings.data.intInput.defaultValue; break;
        case GEUI_RealInput: ptr->data.input.value.realValue = settings.data.realInput.defaultValue; break;
    }

    refreshValue(&ptr->data.input);

    ptr->data.input.tiles = noIndices;

    ptr->layout.width = maxWidth + ptr->parent->style.tileWidth * 2;
    ptr->layout.height = ptr->parent->style.tileHeight;

    return addItemToWindow(ptr);
}

WindowItem *addPanel(Window *window, Panel *panel, char tag[256])
{
    WindowItem *ptr = initNewItem(GEUI_Panel, window, panel, tag);
    if (!ptr) { DEBUG_MSG_FROM("item is NULL", "addPanel"); return NULL; }

    ptr->data.panel = malloc(sizeof *ptr->data.panel);
    if (!ptr->data.panel)
    {
        free(ptr);
        DEBUG_MSG_FROM("memory allocation failed", "addPanel");
        return NULL;
    }

    ptr->data.panel->index = window->pIndex++;
    ptr->data.panel->iIndex = 0;
    ptr->data.panel->rows = 0;
    ptr->data.panel->cols = 0;
    ptr->data.panel->width = 0;
    ptr->data.panel->height = 0;
    ptr->data.panel->parent = window;

    return addItemToWindow(ptr);
}

WindowItem *addEmbedder(Window *window, Panel *panel, char tag[256], const char *actorName)
{
    Actor *actor;
    WindowItem *ptr = initNewItem(GEUI_Embedder, window, panel, tag);
    if (!ptr) { DEBUG_MSG_FROM("item is NULL", "addEmbedder"); return NULL; }

    if (!actorExists2(actor = getclone(actorName)))
    {
        DEBUG_MSG_FROM("actor doesn't exist", "addEmbedder");
        free(ptr);
        return NULL;
    }

    strcpy(ptr->data.embedder.actorCName, actor->clonename);
    VisibilityState(ptr->data.embedder.actorCName, DISABLE);

    ptr->layout.width = actor->width;
    ptr->layout.height = actor->height;

    return addItemToWindow(ptr);
}

void setPosition(WindowItem *this, short row, short col)
{
    if (!this) { DEBUG_MSG_FROM("item is NULL", "setPosition"); return; }

    this->layout.row = row;
    this->layout.col = col;
}

WindowItem *getItemFromPanelByTag(Panel *panel, char tag[256])
{
    WindowItem *ptr;
    WindowItem *result = NULL;

    if (!panel) { DEBUG_MSG_FROM("panel is NULL", "getItemFromPanelByTag"); return NULL; }

    ptr = panel->iList;

    while (ptr)
    {
        if (!strcmp(ptr->tag, tag))
            return ptr;

        if (ptr->type == GEUI_Panel)
        {
            result = getItemFromPanelByTag(ptr->data.panel, tag);

            if (result)
                return result;
        }

        ptr = ptr->next;
    }

    return NULL;
}

WindowItem *getItemByTag(Window *window, char tag[256])
{
    WindowItem *ptr;

    if (!window) { DEBUG_MSG_FROM("panel is NULL", "getItemByTag"); return NULL; }

    ptr = getItemFromPanelByTag(&window->mainPanel, tag);

    if (ptr)
        return ptr;

    return NULL;
}

WindowItem *getItemFromPanelByIndex(Panel *panel, int index)
{
    WindowItem *ptr;
    WindowItem *result = NULL;

    if (!panel) { DEBUG_MSG_FROM("panel is NULL", "getItemFromPanelByIndex"); return NULL; }

    ptr = panel->iList;

    while (ptr)
    {
        if (ptr->index == index)
            return ptr;

        ptr = ptr->next;
    }

    return NULL;
}

WindowItem *getItemByIndex(Window *window, int index)
{
    WindowItem *ptr;

    if (!window) { DEBUG_MSG_FROM("window is NULL", "getItemByIndex"); return NULL; }

    ptr = getItemFromPanelByIndex(&window->mainPanel, index);

    if (ptr)
        return ptr;

    return NULL;
}

WindowItem *getNextFocusableItem(WindowItem *ptr)
{
    Panel *panel = ptr->myPanel;
    Window *window = ptr->parent;
    WindowItem *next = getItemFromPanelByIndex(panel, ptr->index + 1);

    // If there was a next item in the same panel
    if (next)
    {
        if (next->focusable)
            return next;
        return getNextFocusableItem(next);
    }

    // Otherwise get the next panel in this window
    panel = getPanelByIndex(&window->mainPanel, panel->index + 1);

    // If there was a next panel in the same window
    if (panel)
    {
        next = getItemFromPanelByIndex(panel, 0);

        // Panel had an item inside with index 0
        if (next)
        {
            if (next->focusable)
                return next;
            return getNextFocusableItem(next);
        }
    }

    // Otherwise use the main panel (always has index 0) of the window
    panel = getPanelByIndex(&window->mainPanel, 0);

    if (panel)
    {
        next = getItemFromPanelByIndex(panel, 0);

        if (next)
        {
            if (next->focusable)
                return next;
            return getNextFocusableItem(next);
        }
    }

    return NULL;
}

WindowItem *focusItem(WindowItem *ptr)
{
    if (ptr->focusable)
    {
        if (GEUIController.focus != ptr)
        {
            blurItem(GEUIController.focus);
            GEUIController.focus = ptr;
            buildFocus(ptr);

            switch (ptr->type)
            {
                case GEUI_Input:
                    showCaret(&ptr->data.input.caret);
                    updateCaretPosition(&ptr->data.input.caret);
                break;
            }
        }

        return ptr;
    }

    return NULL;
}

void blurItem(WindowItem *ptr)
{
    if (GEUIController.focus != NULL && GEUIController.focus == ptr)
    {
        eraseFocus(ptr);
        GEUIController.focus = NULL;

        if (ptr->type == GEUI_Button)
        {
            ptr->data.button.state = 0;
            colorGuiTiles(ptr->data.button.tiles, ptr->parent->style.buttonColor);
        }
        else if (ptr->type == GEUI_Input)
        {
            ptr->data.input.settings.settingsFunction(&ptr->data.input);
            ptr->data.input.settings.valueFunction(&ptr->data.input);
            updateCaretPosition(&ptr->data.input.caret);
            hideCaret(&ptr->data.input.caret);
        }
    }
}

void buildFocus(WindowItem *ptr)
{
    short i, j;
    Actor *tile;
    short tempAnimpos;
    short focusWidth;
    short focusHeight;
    short focusLineWidth = ptr->parent->style.focusWidth;
    short tilesHorizontal;
    short tilesVertical;
    short tileWidth = ptr->parent->style.tileWidth;
    short tileHeight = ptr->parent->style.tileHeight;

    focusWidth = ptr->layout.width + focusLineWidth * 2;
    tilesHorizontal = ceil(focusWidth / (float)tileWidth);
    focusHeight = ptr->layout.height + focusLineWidth * 2;
    tilesVertical = ceil(focusHeight / (float)tileHeight);

    if (tilesVertical < 3)
    {
        for (i = 0; i < tilesHorizontal; i++)
        {
            tile = CreateActor("a_gui", ptr->parent->style.guiAnim,
                               ptr->parent->parentCName, "(none)", 0, 0, true);
            tile->x = ptr->layout.startx + tileWidth + i * tileWidth  + (i >= 2 && i >= tilesHorizontal - 2) * (focusWidth  - tilesHorizontal * tileWidth)-tileWidth/2;
            tile->x += ptr->parent->style.padding - focusLineWidth;
            tile->y = ptr->layout.starty + tileHeight -tileHeight/2;
            tile->y += ptr->parent->style.padding - focusLineWidth;
            tile->animpos = 15 + (i > 0) + (i == tilesHorizontal - 1);
            tile->myWindow = -1;
            tile->myPanel = -1;
            tile->myIndex = -1;
            colorActor(tile, ptr->parent->style.focusColor);
            ChangeZDepth(tile->clonename, DEFAULT_ITEM_ZDEPTH);
            EventDisable(tile->clonename, EVENTCOLLISION);
            EventDisable(tile->clonename, EVENTCOLLISIONFINISH);
            updateGuiTileIndices(&GEUIController.focusTiles, tile->cloneindex);

            tile = CreateActor("a_gui", ptr->parent->style.guiAnim,
                           ptr->parent->parentCName, "(none)", 0, 0, true);
            tile->x = ptr->layout.startx + tileWidth + i * tileWidth  + (i >= 2 && i >= tilesHorizontal - 2) * (focusWidth  - tilesHorizontal * tileWidth)-tileWidth/2;
            tile->x += ptr->parent->style.padding - focusLineWidth;
            tile->y = ptr->layout.starty + tileHeight -tileHeight/2 + ptr->parent->style.padding - focusLineWidth + focusHeight - tileHeight;
            tile->animpos = 21 + (i > 0) + (i == tilesHorizontal - 1);
            tile->myWindow = -1;
            tile->myPanel = -1;
            tile->myIndex = -1;
            colorActor(tile, ptr->parent->style.focusColor);
            ChangeZDepth(tile->clonename, DEFAULT_ITEM_ZDEPTH);
            EventDisable(tile->clonename, EVENTCOLLISION);
            EventDisable(tile->clonename, EVENTCOLLISIONFINISH);
            updateGuiTileIndices(&GEUIController.focusTiles, tile->cloneindex);
        }
    }
    else
    {
        for (j = 0; j < tilesVertical; j++)
        {
            for (i = 0; i < tilesHorizontal; i++)
            {
                tempAnimpos = calculateAnimpos(tilesHorizontal, tilesVertical, i, j) + 15;

                if (tempAnimpos == 19)
                    continue;

                tile = CreateActor("a_gui", ptr->parent->style.guiAnim,
                                   ptr->parent->parentCName, "(none)", 0, 0, true);
                tile->x = ptr->layout.startx + tileWidth + i * tileWidth  + (i >= 2 && i >= tilesHorizontal - 2) * (focusWidth  - tilesHorizontal * tileWidth)-tileWidth/2;
                tile->x += ptr->parent->style.padding - focusLineWidth;
                tile->y = ptr->layout.starty + tileHeight + j * tileHeight + (j >= 2 && j >= tilesVertical - 2) * (focusHeight - tilesVertical * tileHeight)-tileHeight/2;
                tile->y += ptr->parent->style.padding - focusLineWidth;
                tile->animpos = tempAnimpos;

                tile->myWindow = -1;
                tile->myPanel = -1;
                tile->myIndex = -1;
                colorActor(tile, ptr->parent->style.focusColor);
                ChangeZDepth(tile->clonename, DEFAULT_ITEM_ZDEPTH);
                EventDisable(tile->clonename, EVENTCOLLISION);
                EventDisable(tile->clonename, EVENTCOLLISIONFINISH);
                updateGuiTileIndices(&GEUIController.focusTiles, tile->cloneindex);
            }
        }
    }
}

void eraseFocus()
{
    eraseGuiTiles(&GEUIController.focusTiles);
}

void buildItems(Panel *panel)
{
    WindowItem *ptr;

    ptr = panel->iList;

    while (ptr)
    {
        buildItem(ptr);
        ptr = ptr->next;
    }
}

void buildItem(WindowItem *ptr)
{
    switch (ptr->type)
    {
        case GEUI_Text: buildText(ptr); break;
        case GEUI_Button: buildButton(ptr); break;
        case GEUI_Input: buildInputField(ptr); break;
        case GEUI_Panel: buildPanel(ptr); break;
        case GEUI_Embedder: buildEmbedder(ptr); break;
    }
}

void buildText(WindowItem *ptr)
{
    if (ptr->type != GEUI_Text) { DEBUG_MSG_FROM("item is not a valid Text item", "buildText"); return; }

    setTextZDepth(&ptr->data.text, DEFAULT_ITEM_ZDEPTH);
    // TODO: layout / positioning
    setTextPosition(&ptr->data.text,
        ptr->layout.startx + ptr->parent->style.padding,
        ptr->layout.starty + ptr->data.text.pFont->lineSpacing * 0.5 + ptr->parent->style.padding);
    refreshText(&ptr->data.text);
}

void buildPanel(WindowItem *ptr)
{
    if (ptr->type != GEUI_Panel) { DEBUG_MSG_FROM("item is not a valid Panel item", "buildPanel"); return; }

    buildItems(ptr->data.panel);
}

void buildButtonText(WindowItem *ptr)
{
    long start = ptr->data.button.tiles.first;
    long end = ptr->data.button.tiles.last;

    Text *buttonText = &ptr->data.button.text;

    colorClones("a_gui", start, end, ptr->parent->style.buttonColor);
    setTextZDepth(buttonText, DEFAULT_ITEM_ZDEPTH);
    setTextAlignment(buttonText, ALIGN_CENTER);
    setTextPosition(buttonText,
        ceil((getTile(end)->x - getTile(start)->x) * 0.5) + getTile(start)->x, getTile(start)->y);
    refreshText(buttonText);
}

void buildButton(WindowItem *ptr)
{
    short i;
    Actor *a;
    Text *buttonText;
    long start, end;
    short buttonWidth;
    short tilesHorizontal;
    short tileWidth = ptr->parent->style.tileWidth;
    short tileHeight = ptr->parent->style.tileHeight;

    if (ptr->type != GEUI_Button) { DEBUG_MSG_FROM("item is not a valid Button item", "buildButton"); return; }

    buttonWidth = ptr->layout.width; //ptr->data.button.text.width + ptr->parent->style.padding * 2;
    tilesHorizontal = ceil(buttonWidth / (float)tileWidth);

    for (i = 0; i < tilesHorizontal; i ++)
    {
        a = CreateActor("a_gui", ptr->parent->style.guiAnim, ptr->parent->parentCName, "(none)", 0, 0, true);
        // TODO: layout / positioning
        a->x = ptr->layout.startx + tileWidth + i * tileWidth + (i >= 2 && i >= tilesHorizontal - 2) * (buttonWidth - tilesHorizontal * tileWidth)-tileWidth/2;// + (ptr->layout.col > 0); // TODO: make nicer
        a->x += ptr->parent->style.padding;
        a->y = ptr->layout.starty + tileHeight-tileWidth/2;// + (ptr->layout.row > 0);
        a->y += ptr->parent->style.padding;
        a->myWindow = ptr->parent->index;
        a->myPanel  = ptr->myPanel->index;
        a->myIndex  = ptr->index;
        ChangeZDepth(a->clonename, DEFAULT_ITEM_ZDEPTH);
        a->animpos = 9 + (i > 0) + (i == tilesHorizontal - 1) + (i > 0 && i == tilesHorizontal - 2 && buttonWidth < tileWidth * 2.5);// TODO: make nicer

        updateGuiTileIndices(&ptr->data.button.tiles, a->cloneindex);
    }

    buildButtonText(ptr);
}

Actor *buildCaret(WindowItem *ptr, Text *pText, BlinkingCaret *caret)
{
    Actor *a = CreateActor("a_gui", ptr->parent->style.guiAnim, ptr->parent->parentCName, "(none)", 0, 0, true);
    a->animpos = 19;
    a->myWindow = ptr->parent->index,
    a->myPanel = ptr->myPanel->index;
    a->myIndex = ptr->index;
    a->myProperties = GEUI_CARET;
    SendActivationEvent(a->clonename);
    ChangeZDepth(a->clonename, DEFAULT_ITEM_ZDEPTH);
    strcpy(caret->actorCName, a->clonename);
    caret->pText = pText;
    colorActorByName(caret->actorCName, ptr->parent->style.textColor);
    updateCaretPosition(caret);

    return a;
}

void buildInputFieldBackground(WindowItem *ptr, TileIndices *tiles)
{
    short i;
    Actor *a;
    short fieldWidth;
    short tilesHorizontal;
    short tileWidth = ptr->parent->style.tileWidth;
    short tileHeight = ptr->parent->style.tileHeight;

    fieldWidth = ptr->layout.width;
    tilesHorizontal = ceil(fieldWidth / (float)tileWidth);

    for (i = 0; i < tilesHorizontal; i++)
    {
        a = CreateActor("a_gui", ptr->parent->style.guiAnim, ptr->parent->parentCName, "(none)", 0, 0, true);
        a->x = ptr->layout.startx + tileWidth + i * tileWidth + (i >= 2 && i >= tilesHorizontal - 2) * (fieldWidth - tilesHorizontal * tileWidth)-tileWidth/2;
        a->x += ptr->parent->style.padding;
        a->y = ptr->layout.starty + tileHeight-tileWidth/2;
        a->y += ptr->parent->style.padding;
        a->myWindow = ptr->parent->index;
        a->myPanel  = ptr->myPanel->index;
        a->myIndex  = ptr->index;
        a->myProperties = GEUI_INPUT_BG;
        a->animpos = 12 + (i > 0) + (i == tilesHorizontal - 1) + (i > 0 && i == tilesHorizontal - 2 && fieldWidth < tileWidth * 2.5);
        SendActivationEvent(a->clonename);
        ChangeZDepth(a->clonename, DEFAULT_ITEM_ZDEPTH);

        updateGuiTileIndices(tiles, a->cloneindex);
    }
}

void buildInputField(WindowItem *ptr)
{
    if (ptr->type != GEUI_Input) { DEBUG_MSG_FROM("item is not a valid InputText item", "buildInputText"); return; }

    buildInputFieldBackground(ptr, &ptr->data.input.tiles);
    colorGuiTiles(ptr->data.input.tiles, ptr->parent->style.inputBgColor);

    setTextZDepth(&ptr->data.input.text, DEFAULT_ITEM_ZDEPTH);
    setTextPosition(&ptr->data.input.text,
        getTile(ptr->data.input.tiles.first)->x,
        getTile(ptr->data.input.tiles.last)->y);
    refreshText(&ptr->data.input.text);

    buildCaret(ptr, &ptr->data.input.text, &ptr->data.input.caret);
    hideCaret(&ptr->data.input.caret);
}

void buildEmbedder(WindowItem *ptr)
{
    Actor *actor;
    if (ptr->type != GEUI_Embedder) { DEBUG_MSG_FROM("item is not a valid Embedder item", "buildEmbedder"); return; }

    if (!actorExists2(actor = getclone(ptr->data.embedder.actorCName))) { DEBUG_MSG_FROM("actor doesn't exist", "buildEmbedder"); return; }

    ChangeZDepth(ptr->data.embedder.actorCName, DEFAULT_ITEM_ZDEPTH);
    ChangeParent(ptr->data.embedder.actorCName, "(none)");
    actor->x = 0;
    actor->y = 0;
    ChangeParent(ptr->data.embedder.actorCName, ptr->parent->parentCName);
    actor->x = ptr->layout.startx + ptr->parent->style.padding + actor->width / 2;
    actor->y = ptr->layout.starty + ptr->parent->style.padding + actor->height / 2;
    VisibilityState(ptr->data.embedder.actorCName, ENABLE);

    // {
        // char temp[256];
        // sprintf(temp, "%d, %d - %d, %d", ptr->layout.startx + ptr->parent->style.padding + actor->width / 2,
                                // ptr->layout.starty + ptr->parent->style.padding + actor->height / 2,
                                // (int)actor->x,
                                // (int)actor->y);
        // DEBUG_MSG_FROM(temp, "buildEmbedder");
    // }
}

void eraseWindowItem(WindowItem *ptr)
{
    if (!ptr) { DEBUG_MSG_FROM("item is NULL", "eraseWindowItem"); return; }

    if (GEUIController.focus == ptr)
    {
        eraseFocus();
        GEUIController.focus = NULL;
    }

    switch (ptr->type)
    {
        case GEUI_Text:
            eraseText(&ptr->data.text);
            setTextParent(&ptr->data.text, "(none)", False);
        break;
        case GEUI_Button:
            eraseText(&ptr->data.button.text);
            eraseGuiTiles(&ptr->data.button.tiles);
        break;
        case GEUI_Input:
            eraseText(&ptr->data.input.text);
            eraseCaret(&ptr->data.input.caret);
            setTextParent(&ptr->data.input.text, "(none)", False);
            eraseGuiTiles(&ptr->data.input.tiles);
        break;
        case GEUI_Panel:
            closePanel(ptr->data.panel);
        break;
        case GEUI_Embedder:
            VisibilityState(ptr->data.embedder.actorCName, DISABLE);
        break;

        default: break;
    }
}

void destroyWindowItem(WindowItem *ptr)
{
    if (!ptr) { DEBUG_MSG_FROM("item is NULL", "destroyWindowItem"); return; }

    if (GEUIController.focus == ptr)
        eraseFocus();

    switch (ptr->type)
    {
        case GEUI_Text: destroyText(&ptr->data.text); break;
        case GEUI_Button:
            destroyText(&ptr->data.button.text);
            eraseGuiTiles(&ptr->data.button.tiles);
        break;
        case GEUI_Input:
            destroyText(&ptr->data.input.text);
            eraseCaret(&ptr->data.input.caret);
            eraseGuiTiles(&ptr->data.input.tiles);
        break;
        case GEUI_Panel:
            destroyPanel(ptr->data.panel);
            free(ptr->data.panel);
        break;
        case GEUI_Embedder:
            DestroyActor(ptr->data.embedder.actorCName);
        break;

        default: break;
    }
}


// ..\source\geui\30-geui-panel.c
// TODO: make functions return error codes instead of just exiting
// without doing anything, which can be difficult to debug

Panel *getPanelByTag(Panel *panel, char tag[256]);
Panel *getPanelByIndex(Panel *panel, int index);
void calculateRowsAndCols(Panel *panel);
short getColWidth(Panel *panel, short col);
short getRowHeight(Panel *panel, short row);
short getRowStart(WindowItem *panelItem, Panel *panel, short row);
short getColStart(WindowItem *panelItem, Panel *panel, short col);
short getPanelWidth(Panel *panel);
short getPanelHeight(Panel *panel);
void setPanelBaseParent(Panel *panel, char *parentName);
void updatePanelLayout(WindowItem *panelItem, Panel *panel);
void closePanel(Panel *panel);
void destroyPanel(Panel *panel);

Panel *getPanelByTag(Panel *panel, char tag[256])
{
    WindowItem *ptr;

    if (!panel) { DEBUG_MSG_FROM("panel is NULL", "getPanelByTag"); return NULL; }

    for (ptr = panel->iList; ptr != NULL; ptr = ptr->next)
    {
        if (!strcmp(ptr->tag, tag))
            return ptr->data.panel;
        if (ptr->type == GEUI_Panel)
        {
            Panel *p = getPanelByTag(ptr->data.panel, tag);
            if (p) return p;
        }
    }

    return NULL;
}

Panel *getPanelByIndex(Panel *panel, int index)
{
    WindowItem *ptr;

    if (!panel) { DEBUG_MSG_FROM("panel is NULL", "getPanelByIndex"); return NULL; }

    if (panel->index == index) return panel;

    for (ptr = panel->iList; ptr != NULL; ptr = ptr->next)
    {
        if (ptr->type == GEUI_Panel)
        {
            Panel *p;

            if (ptr->data.panel->index == index)
                return ptr->data.panel;

            p = getPanelByIndex(ptr->data.panel, index);
            if (p) return p;
        }
    }

    return NULL;
}

void calculateRowsAndCols(Panel *panel)
{
    WindowItem *item;

    if (!panel) { DEBUG_MSG_FROM("panel is NULL", "calculateRowsAndCols"); return; }

    for (item = panel->iList; item != NULL; item = item->next)
    {
        if (item->layout.row >= panel->rows)
            panel->rows = item->layout.row + 1;
        if (item->layout.col >= panel->cols)
            panel->cols = item->layout.col + 1;
    }
}

short getColWidth(Panel *panel, short col)
{
    short width = 0;
    WindowItem *item;

    if (!panel || !panel->cols)
        { DEBUG_MSG_FROM("panel is NULL or has no columns", "getColWidth"); return -1; }
    if (col < 0 || col >= panel->cols)
        { DEBUG_MSG_FROM("column number is invalid", "getColWidth"); return -2; }

    for (item = panel->iList; item != NULL; item = item->next)
    {
        if (item->type == GEUI_Panel) updatePanelLayout(item, item->data.panel);
        if (item->layout.col == col && item->layout.width > width)
            width = item->layout.width;
    }

    return width + panel->parent->style.padding * (col < panel->cols - 1);
}

short getRowHeight(Panel *panel, short row)
{
    short height = 0;
    WindowItem *item;

    if (!panel || !panel->rows)
        { DEBUG_MSG_FROM("panel is NULL or has no rows", "getRowHeight"); return -1; }
    if (row < 0 || row >= panel->rows)
        { DEBUG_MSG_FROM("row number is invalid", "getRowHeight"); return -2; }

    for (item = panel->iList; item != NULL; item = item->next)
    {
        if (item->type == GEUI_Panel) updatePanelLayout(item, item->data.panel);
        if (item->layout.row == row && item->layout.height > height)
            height = item->layout.height;
    }

    return height + panel->parent->style.padding * (row < panel->rows - 1);
}

short getRowStart(WindowItem *panelItem, Panel *panel, short row)
{
    WindowItem *ptr;

    if (!panel || !panel->iList) { DEBUG_MSG_FROM("panel is NULL or has no items", "getRowStart"); return 0; }

    if (row >= panel->rows)
        return panel->height + getRowStart(panelItem, panel, 0) + panel->parent->style.padding;

    for (ptr = panel->iList; ptr != NULL; ptr = ptr->next)
    {
        if (ptr->layout.row == row)
        {
            return ptr->layout.starty + ptr->parent->style.padding / 2;
        }
    }

    return 0;
}

short getColStart(WindowItem *panelItem, Panel *panel, short col)
{
    WindowItem *ptr;

    if (!panel || !panel->iList) { DEBUG_MSG_FROM("panel is NULL or has no items", "getColStart"); return 0; }

    if (col >= panel->cols)
        return panel->width + getColStart(panelItem, panel, 0) + panel->parent->style.padding;

    for (ptr = panel->iList; ptr != NULL; ptr = ptr->next)
    {
        if (ptr->layout.col == col)
        {
            return ptr->layout.startx + ptr->parent->style.padding / 2;
        }
    }

    return 0;
}

short getPanelWidth(Panel *panel)
{
    short col;
    short width = 0;
    short tempWidth;

    if (!panel) { DEBUG_MSG_FROM("panel is NULL", "getPanelWidth"); return -1; }

    if (!panel->cols) { DEBUG_MSG_FROM("panel has no columns", "getPanelWidth"); return -2; }

    for (col = 0; col < panel->cols; col ++)
    {
        if ((tempWidth = getColWidth(panel, col)) >= 0)
            width += tempWidth;
    }

    return width + panel->cols - (panel->cols > 0);
}

short getPanelHeight(Panel *panel)
{
    short row;
    short height = 0;
    short tempHeight;

    if (!panel) { DEBUG_MSG_FROM("panel is NULL", "getPanelHeight"); return -1; }

    if (!panel->rows) { DEBUG_MSG_FROM("panel has no rows", "getPanelHeight"); return -2; }

    for (row = 0; row < panel->rows; row ++)
    {
        if ((tempHeight = getRowHeight(panel, row)) >= 0)
            height += tempHeight;
    }

    return height + panel->rows - (panel->rows > 0);
}

void setPanelBaseParent(Panel *panel, char *parentName)
{
    WindowItem *ptr;

    if (!panel) { DEBUG_MSG_FROM("panel is NULL", "setPanelBaseParent"); return; }
    if (!actorExists(parentName)) { DEBUG_MSG_FROM("actor doesn't exist", "setPanelBaseParent"); return; }

    ptr = panel->iList;

    while (ptr)
    {
        switch (ptr->type)
        {
            case GEUI_Text: setTextParent(&ptr->data.text, parentName, True); break;
            case GEUI_Button:
                setTextParent(&ptr->data.button.text, parentName, True);
                changeParentOfClones("a_gui", ptr->data.button.tiles.first, ptr->data.button.tiles.last, parentName);
                break;
            case GEUI_Input: setTextParent(&ptr->data.input.text, parentName, True); break;
            case GEUI_Panel: setPanelBaseParent(ptr->data.panel,  parentName); break;
            case GEUI_Embedder: ChangeParent(ptr->data.embedder.actorCName, parentName); break;

            default: break;
        }

        ptr = ptr->next;
    }
}

void updatePanelLayout(WindowItem *panelItem, Panel *panel)
{
    short i;
    short startx = 0;
    short starty = 0;
    short origx=0;
    short origy=0;
    short *rowValues;
    short *colValues;
    WindowItem *item;

    if (!panel) { DEBUG_MSG_FROM("panel is NULL", "updatePanelLayout"); return; }

    if (panelItem && panelItem->type == GEUI_Panel)
    {
        origx=startx = panelItem->layout.startx;
        origy=starty = panelItem->layout.starty;
    }
    else panelItem = NULL;

    calculateRowsAndCols(panel);

    if (!panel->rows || !panel->cols)
        { DEBUG_MSG_FROM("panel has no rows or columns", "updatePanelLayout"); return; }

    rowValues = malloc((panel->rows + 1) * sizeof *rowValues);
    if (!rowValues) { DEBUG_MSG_FROM("memory allocation failed", "updatePanelLayout"); return; }
    colValues = malloc((panel->cols + 1) * sizeof *colValues);
    if (!colValues) { free(rowValues); DEBUG_MSG_FROM("memory allocation failed", "updatePanelLayout"); return; }

    rowValues[0] = origy;
    colValues[0] = origx;
    for (i = 0; i < panel->rows; i++)
        starty = rowValues[i+1] = starty + getRowHeight(panel, i) + 1;// + (i > 0);
    for (i = 0; i < panel->cols; i++)
        startx = colValues[i+1] = startx + getColWidth(panel, i) + 1;// + (i > 0);

    for (item = panel->iList; item != NULL; item = item->next)
    {
        item->layout.startx = colValues[item->layout.col];
        item->layout.starty = rowValues[item->layout.row];
    }

    panel->width = getPanelWidth(panel);
    panel->height = getPanelHeight(panel);

    if (panelItem)
    {
        panelItem->layout.width = panel->width;
        panelItem->layout.height = panel->height;
    }

    free(rowValues);
    free(colValues);
}

void closePanel(Panel *panel)
{
    WindowItem *ptr;

    if (!panel) { DEBUG_MSG_FROM("panel is NULL", "closePanel"); return; }

    ptr = panel->iList;

    while (ptr)
    {
        eraseWindowItem(ptr);
        ptr = ptr->next;
    }
}

void destroyPanel(Panel *panel)
{
    WindowItem *temp;
    WindowItem *ptr;

    if (!panel) { DEBUG_MSG_FROM("panel is NULL", "destroyPanel"); return; }

    ptr = panel->iList;

    while (ptr)
    {
        destroyWindowItem(ptr);
        temp = ptr->next;
        free(ptr);
        ptr = temp;
    }

    panel->iList = NULL;
    panel->iIndex = 0;
}


// ..\source\geui\40-geui-window.c
// TODO: make functions return error codes instead of just exiting
// without doing anything, which can be difficult to debug

#define GEUI_XY_SCREEN_CENTER -1, -1
#define GEUI_XY_MOUSE_POSITION xmouse, ymouse

Window *createWindow(char tag[256], Style style);
Window *getWindowByTag(char tag[256]);
Window *getWindowByIndex(int index);
Window *getFirstOpenWindow();
Window *openWindow(char tag[256], float startX, float startY);
void buildWindow(Window *window, float startX, float startY);
Actor *createWindowBaseParent(Window *window, float startX, float startY);
void setWindowBaseParent(Window *window, char *parentName);
void bringWindowToFront(Window *window);
void closeWindow(Window *window);
void destroyWindow(Window *window);

// BEGIN SABRE WORLD EDITOR (SWE) MODIFICATION
void enableGEUIMouse();
void disableGEUIMouse();
int isGEUIActive();
// END SABRE WORLD EDITOR (SWE) MODIFICATION

Window *createWindow(char tag[256], Style style)
{
    Window *ptr = malloc(sizeof *ptr);

    if (!ptr) { DEBUG_MSG_FROM("memory allocation failed", "createWindow"); return NULL; }

    ptr->index = GEUIController.wIndex ++;
    ptr->pIndex = 0;
    strcpy(ptr->tag, tag);
    ptr->isOpen = False;
    ptr->style = style;
    ptr->zDepth = DEFAULT_WINDOW_ZDEPTH;
    strcpy(ptr->parentCName, "");
    ptr->tiles = noIndices;
    ptr->mainPanel.index = ptr->pIndex++;
    ptr->mainPanel.iIndex = 0;
    ptr->mainPanel.rows = 0;
    ptr->mainPanel.cols = 0;
    ptr->mainPanel.width = -1;
    ptr->mainPanel.height = -1;
    ptr->mainPanel.parent = ptr;
    ptr->mainPanel.iList = NULL;
    ptr->next = GEUIController.wList;

    GEUIController.wList = ptr;

    getTileDimensions(&ptr->style);
    ptr->style.tileWidth = defStyle.tileWidth;
    ptr->style.tileHeight = defStyle.tileHeight;

    return ptr;
}

Window *getWindowByTag(char tag[256])
{
    Window *ptr = GEUIController.wList;

    while (ptr)
    {
        if (!strcmp(ptr->tag, tag))
            return ptr;

        ptr = ptr->next;
    }

    return NULL;
}

Window *getWindowByIndex(int index)
{
    Window *ptr = GEUIController.wList;

    while (ptr)
    {
        if (ptr->index == index)
            return ptr;

        ptr = ptr->next;
    }

    return NULL;
}

Window *getFirstOpenWindow()
{
    Window *ptr = GEUIController.wList;

    while (ptr)
    {
        if (ptr->isOpen)
        {
            return ptr;
        }

        ptr = ptr->next;
    }

    return NULL;
}

Window *openWindow(char tag[256], float startX, float startY)
{
    Window *window = getWindowByTag(tag);

    if (!window) { DEBUG_MSG_FROM("window is NULL", "openWindow"); return NULL; }
    if (window->isOpen) { DEBUG_MSG_FROM("window is already open", "openWindow"); return window; }

    updatePanelLayout(NULL, &window->mainPanel);
    buildWindow(window, startX, startY);
    buildItems(&window->mainPanel);

    window->isOpen = True;
    bringWindowToFront(window);

    // BEGIN SABRE WORLD EDITOR (SWE) MODIFICATION
    if (!GEUIController.openWindowCount++)
        enableGEUIMouse();
    // END SABRE WORLD EDITOR (SWE) MODIFICATION

    return window;
}

void buildWindow(Window *window, float startX, float startY)
{
    short i, j;
    Actor *tile;

    short tileWidth, tileHeight;
    short windowWidth, windowHeight;
    short tilesHorizontal, tilesVertical;

    setWindowBaseParent(window, createWindowBaseParent(window, startX, startY)->clonename);

    tileWidth = window->style.tileWidth;
    tileHeight = window->style.tileHeight;

    windowWidth = window->mainPanel.width + window->style.tileWidth + window->style.padding * 2;
    windowHeight = window->mainPanel.height + window->style.tileHeight + window->style.padding * 2;

    tilesHorizontal = ceil(windowWidth / (float)tileWidth);
    tilesVertical = ceil(windowHeight / (float)tileHeight);

    for (j = 0; j < tilesVertical; j ++)
    {
        for (i = 0; i < tilesHorizontal; i ++)
        {
            tile = CreateActor("a_gui", window->style.guiAnim,
                               window->parentCName, "(none)", 0, 0, true);
            // TODO: actual positioning
            tile->x = i * tileWidth  + (i >= 2 && i >= tilesHorizontal - 2) * (windowWidth  - tilesHorizontal * tileWidth);
            tile->y = j * tileHeight + (j >= 2 && j >= tilesVertical - 2) * (windowHeight - tilesVertical * tileHeight);
            tile->myWindow = window->index;
            tile->myPanel = window->mainPanel.index;
            tile->myIndex = -1;
            tile->animpos = calculateAnimpos(tilesHorizontal, tilesVertical, i, j);
            colorActor(tile, window->style.windowBgColor);
            ChangeZDepth(tile->clonename, WINDOW_TILE_ZDEPTH);
            EventDisable(tile->clonename, EVENTCOLLISION);
            EventDisable(tile->clonename, EVENTCOLLISIONFINISH);

            if (j == 0) tile->myProperties = GEUI_TITLE_BAR; // part of the window title bar

            updateGuiTileIndices(&window->tiles, tile->cloneindex);
        }
    }
}

Actor *createWindowBaseParent(Window *window, float startX, float startY)
{
    Actor *baseParent;
    float posX = startX;
    float posY = startY;

    // Magic values to indicate that the window should be centered
    if (startX == -1 && startY == -1)
    {
        posX = view.width * 0.5f - window->mainPanel.width * 0.5f;
        posY = view.height * 0.5f - window->mainPanel.height * 0.5f;
    }

    baseParent = CreateActor("a_gui", window->style.guiAnim, "(none)", "(none)", view.x + posX, view.y + posY, true);
    baseParent->animpos = 0;
    baseParent->myWindow = window->index;
    baseParent->myPanel = -1;
    baseParent->myIndex = -1;
    ChangeZDepth(baseParent->clonename, window->zDepth);
    CollisionState(baseParent->clonename, DISABLE);

    return baseParent;
}

void setWindowBaseParent(Window *window, char *parentName)
{
    WindowItem *ptr = NULL;

    if (!window) { DEBUG_MSG_FROM("window is NULL", "setWindowBaseParent"); return; }
    if (!actorExists(parentName)) { DEBUG_MSG_FROM("actor doesn't exist", "setWindowBaseParent"); return; }

    strcpy(window->parentCName, parentName);

    changeParentOfClones("a_gui", window->tiles.first, window->tiles.last, parentName);
    setPanelBaseParent(&window->mainPanel, parentName);
}

void bringWindowToFront(Window *window)
{
    Window *ptr = NULL;
    Window *prev = NULL;

    if (!window) { DEBUG_MSG_FROM("window is NULL", "bringWindowToFront"); return; }

    ptr = GEUIController.wList;
    blurItem(GEUIController.focus);

    while (ptr)
    {
        if (ptr->index == window->index)
        {
            ptr->zDepth = ACTIVE_WINDOW_ZDEPTH;
            ChangeZDepth(ptr->parentCName, ptr->zDepth);
            GEUIController.topIndex = window->index;

            if (prev)
            {
                prev->next = ptr->next;
                ptr->next = GEUIController.wList;
                GEUIController.wList = ptr;
                ptr = prev;
            }
        }
        else
        {
            ptr->zDepth = DEFAULT_WINDOW_ZDEPTH;
            ChangeZDepth(ptr->parentCName, ptr->zDepth);
        }

        prev = ptr;
        ptr = ptr->next;
    }
}

void closeWindow(Window *window)
{
    WindowItem *ptr = NULL;

    if (!window) { DEBUG_MSG_FROM("window is NULL", "closeWindow"); return; }

    window->isOpen = False;

    DestroyActor(window->parentCName);
    strcpy(window->parentCName, "(none)");

    eraseGuiTiles(&window->tiles);

    if (window->index == GEUIController.topIndex)
    {
        Window *nextTopWindow = getFirstOpenWindow();

        if (nextTopWindow)
        {
            GEUIController.topIndex = nextTopWindow->index;
        }
        else
        {
            GEUIController.topIndex = -1;
        }
    }

    // BEGIN SABRE WORLD EDITOR (SWE) MODIFICATION
    GEUIController.openWindowCount--;

    if (!GEUIController.openWindowCount)
        disableGEUIMouse();
    // END SABRE WORLD EDITOR (SWE) MODIFICATION

    closePanel(&window->mainPanel);
}

void destroyWindow(Window *window)
{
    closeWindow(window);
    destroyPanel(&window->mainPanel);
    free(window);
}


// ..\source\geui\50-geui-input.c
// TODO: make functions return error codes instead of just exiting
// without doing anything, which can be difficult to debug

int isTopmostItemAtMouse(WindowItem *item);
void focusNextItemInWindow();
void doMouseEnter(const char *actorName);
void doMouseLeave(const char *actorName);
void doMouseButtonDown(const char *actorName, enum mouseButtonsEnum mButtonNumber);
void doMouseButtonUp(const char *actorName, enum mouseButtonsEnum mButtonNumber);
int onMouseClick(enum mouseButtonsEnum mButtonNumber);
int onMouseRelease(enum mouseButtonsEnum mButtonNumber);
int onMouseButtonDown(enum mouseButtonsEnum mButtonNumber);
int onMouseButtonUp(enum mouseButtonsEnum mButtonNumber);
int isMouseButtonDown(enum mouseButtonsEnum mButtonNumber);
int isMouseButtonUp(enum mouseButtonsEnum mButtonNumber);
int updateMouseButtonDown(enum mouseButtonsEnum mButtonNumber);
void updateMouseButtonUp(enum mouseButtonsEnum mButtonNumber);
void doKeyDown(WindowItem *item, int key);
void doKeyUp(WindowItem *item, int key);

int isTopmostItemAtMouse(WindowItem *item)
{
    int count;
    Actor *actors;

    actors = getAllActorsInCollision("a_geuiMouse.0", &count);

    if (actors)
    {
        int i;

        for (i = count - 1; i >= 0; i --)
        {
            if (!strcmp(actors[i].name, "a_gui"))
            {
                return (actors[i].myIndex > -1 &&
                        actors[i].myWindow == item->parent->index &&
                        actors[i].myPanel == item->myPanel->index &&
                        actors[i].myIndex == item->index);
            }
        }
    }

    return 0;
}

void focusNextItemInWindow()
{
    Window *window = getWindowByIndex(GEUIController.topIndex);
    WindowItem *nextFocus = NULL;

    if (GEUIController.focus && GEUIController.focus->parent->index == GEUIController.topIndex)
    {
        nextFocus = getNextFocusableItem(GEUIController.focus);
    }
    else
    {
        window = getWindowByIndex(GEUIController.topIndex);
        if (window && window->isOpen)
        {
            nextFocus = getItemFromPanelByIndex(&window->mainPanel, 0);
            if (nextFocus && nextFocus->focusable == False)
            {
                nextFocus = getNextFocusableItem(nextFocus);
            }
        }
    }

    if (nextFocus)
    {
        focusItem(nextFocus);
    }
}

void doMouseEnter(const char *actorName)
{
    Actor *actor;
    Window *window;
    WindowItem *item;

    if (!actorExists2(actor = getclone(actorName)))
        { DEBUG_MSG_FROM("actor doesn't exist", "doMouseEnter"); return; }
    if (actor->myWindow < 0 || actor->myPanel < 0 || actor->myIndex < 0)
        { DEBUG_MSG_FROM("actor window, panel or index is invalid", "doMouseEnter"); return; }
    if (!(window = getWindowByIndex(actor->myWindow)))
        { DEBUG_MSG_FROM("window is NULL", "doMouseEnter"); return; }
    if (!(item = getItemFromPanelByIndex(getPanelByIndex(&window->mainPanel, actor->myPanel), actor->myIndex)))
        { DEBUG_MSG_FROM("item is NULL", "doMouseEnter"); return; }

    switch (item->type)
    {
        case GEUI_Button:
            if (isTopmostItemAtMouse(item))
            {
                if (item->data.button.state)
                    colorGuiTiles(item->data.button.tiles, item->parent->style.buttonPressedColor);
                else
                    colorGuiTiles(item->data.button.tiles, item->parent->style.buttonHilitColor);
            }
            else doMouseLeave(actorName);
        break;
    }
}

void doMouseLeave(const char *actorName)
{
    Actor *actor;
    Window *window;
    WindowItem *item;

    if (!actorExists2(actor = getclone(actorName)))
        { DEBUG_MSG_FROM(actorName, "doMouseLeave"); DEBUG_MSG_FROM("actor doesn't exist", "doMouseLeave"); return; }
    if (actor->myWindow < 0 || actor->myPanel < 0 || actor->myIndex < 0)
        { DEBUG_MSG_FROM("actor window, panel or index is invalid", "doMouseLeave"); return; }
    if (!(window = getWindowByIndex(actor->myWindow)))
        { DEBUG_MSG_FROM("window is NULL", "doMouseLeave"); return; }
    if (!(item = getItemFromPanelByIndex(getPanelByIndex(&window->mainPanel, actor->myPanel), actor->myIndex)))
        { DEBUG_MSG_FROM("item is NULL", "doMouseLeave"); return; }

    switch (item->type)
    {
        case GEUI_Button:
            if (!isTopmostItemAtMouse(item))
            {
                if (item->data.button.state)
                    colorGuiTiles(item->data.button.tiles, item->parent->style.buttonPressedColor);
                else
                    colorGuiTiles(item->data.button.tiles, item->parent->style.buttonColor);
            }
        break;
    }
}

// TODO: test without actorName, with just clonename
void doMouseButtonDown(const char *actorName, enum mouseButtonsEnum mButtonNumber)
{
    Actor *actor;
    Window *window;
    WindowItem *item;

    if (!actorExists2(actor = getclone(actorName)))
        { DEBUG_MSG_FROM("actor doesn't exist", "doMouseButtonDown"); return; }
    if (actor->myWindow < 0)
        { DEBUG_MSG_FROM("actor window is invalid", "doMouseButtonDown"); return; }
    if (!(window = getWindowByIndex(actor->myWindow)))
        { DEBUG_MSG_FROM("window is NULL", "doMouseButtonDown"); return; }

    if (actor->myProperties & GEUI_TITLE_BAR)
    {
        Actor *fake;
        Actor *wParent;
        int xDist, yDist;

        ChangeParent(actor->clonename, "(none)");
        ChangeZDepth(actor->clonename, ACTIVE_WINDOW_ZDEPTH);

        // store the current color of the event actor
        actor->myColorR = actor->r;
        actor->myColorG = actor->g;
        actor->myColorB = actor->b;
        //VisibilityState(actor->clonename, DONT_DRAW_ONLY);

        // make the event actor white to keep children's colors unchanged after parenting
        actor->r = actor->g = actor->b = 255;

        // create fake actor to cover the now white event actor
        fake = CreateActor("a_gui", window->style.guiAnim, window->parentCName, "(none)", 0, 0, false);
        fake->myWindow = window->index;
        fake->myPanel = -1;
        fake->myIndex = -1;
        fake->myProperties = GEUI_FAKE_ACTOR;
        actor->myFakeIndex = fake->cloneindex;
        ChangeZDepth(fake->clonename, FAKE_ACTOR_ZDEPTH);
        SendActivationEvent(getTile(actor->myFakeIndex)->clonename);

        wParent = getclone(window->parentCName);

        // calculate the window base parent's coordinates relative to the event actor
        xDist = ceil(wParent->x - x);
        yDist = ceil(wParent->y - y);

        // make event actor the parent of the window base parent
        ChangeParent(wParent->clonename, actor->clonename);
        wParent->x = xDist;
        wParent->y = yDist;

        actor->myProperties |= GEUI_CLICKED;
        FollowMouse(actor->clonename, BOTH_AXIS);
    }

    bringWindowToFront(window);

    if (actor->myIndex < 0) return;
    if (!(item = getItemFromPanelByIndex(getPanelByIndex(&window->mainPanel, actor->myPanel), actor->myIndex)))
        { DEBUG_MSG_FROM("item is NULL", "doMouseButtonDown"); return; }

    switch (item->type)
    {
        case GEUI_Button:
            focusItem(item);
            colorGuiTiles(item->data.button.tiles, window->style.buttonPressedColor);
            item->data.button.state = 1;
        break;
        case GEUI_Input:
            focusItem(item);
        break;
    }
}

// TODO: test without actorName, with just clonename
void doMouseButtonUp(const char *actorName, enum mouseButtonsEnum mButtonNumber)
{
    Actor *actor;
    Window *window;
    WindowItem *item;

    if (!actorExists2(actor = getclone(actorName)))
        { DEBUG_MSG_FROM("actor doesn't exist", "doMouseButtonUp"); return; }
    if (actor->myWindow < 0)
        { DEBUG_MSG_FROM("actor window is invalid", "doMouseButtonUp"); return; }
    if (!(window = getWindowByIndex(actor->myWindow)))
        { DEBUG_MSG_FROM("window is NULL", "doMouseButtonUp"); return; }

    if (actor->myProperties & GEUI_TITLE_BAR && actor->myProperties & GEUI_CLICKED)
    {
        Actor *fake;
        Actor *wParent;
        int xDist, yDist;

        wParent = getclone(window->parentCName);

        // reset window base parent's parent to none
        ChangeParent(wParent->clonename, "(none)");
        ChangeZDepth(actor->clonename, WINDOW_TILE_ZDEPTH);

        // calculate the event actor's coordinates relative to window the base parent
        xDist = ceil(actor->x - wParent->x);
        yDist = ceil(actor->y - wParent->y);

        // make the window base parent the parent of the event actor
        ChangeParent(actor->clonename, wParent->clonename);
        actor->x = xDist;
        actor->y = yDist;

        actor->myProperties &= ~GEUI_CLICKED;
        FollowMouse(actor->clonename, NONE_AXIS);

        // restore the event actor's original color
        actor->r = actor->myColorR;
        actor->g = actor->myColorG;
        actor->b = actor->myColorB;
        //VisibilityState(actor->clonename, ENABLE);

        // destroy the fake actor
        DestroyActor(getTile(actor->myFakeIndex)->clonename);
    }

    //ChangeZDepth(window->parentCName, 0.5);

    if (actor->myIndex < 0) return;
    if (!(item = getItemFromPanelByIndex(getPanelByIndex(&window->mainPanel, actor->myPanel), actor->myIndex)))
        { DEBUG_MSG_FROM("item is NULL", "doMouseButtonUp"); return; }

    switch (item->type)
    {
        case GEUI_Button:
        {
            if (isTopmostItemAtMouse(item))
            {
                colorGuiTiles(item->data.button.tiles, window->style.buttonHilitColor);
                if (item->data.button.state && item->data.button.actionFunction)
                    item->data.button.actionFunction(window, item);
            }
            else
            {
                colorGuiTiles(item->data.button.tiles, window->style.buttonColor);
            }
            item->data.button.state = 0;
        }
        break;
    }
}

int onMouseClick(enum mouseButtonsEnum mButtonNumber)
{
    return (!strcmp(GEUIController.mButtonTopActorCName[mButtonNumber], clonename) &&
            GEUIController.mButtonState[mButtonNumber] &&
            GEUIController.activeButton == mButtonNumber);
}

int onMouseRelease(enum mouseButtonsEnum mButtonNumber)
{
    return (!strcmp(GEUIController.mButtonTopActorCName[mButtonNumber], clonename) &&
            !GEUIController.mButtonState[mButtonNumber] &&
            GEUIController.activeButton == mButtonNumber);
}

int onMouseButtonDown(enum mouseButtonsEnum mButtonNumber)
{
    return (GEUIController.mButtonState[mButtonNumber] &&
            GEUIController.activeButton == mButtonNumber);
}

int onMouseButtonUp(enum mouseButtonsEnum mButtonNumber)
{
    return (!GEUIController.mButtonState[mButtonNumber] &&
            GEUIController.activeButton == mButtonNumber);
}

int isMouseButtonDown(enum mouseButtonsEnum mButtonNumber)
{
    return (GEUIController.mButtonState[mButtonNumber]);
}

int isMouseButtonUp(enum mouseButtonsEnum mButtonNumber)
{
    return (!GEUIController.mButtonState[mButtonNumber]);
}

int updateMouseButtonDown(enum mouseButtonsEnum mButtonNumber)
{
    int i;
    int count;
    Actor *actors = NULL;

    GEUIController.mButtonState[mButtonNumber] = GEUI_MOUSE_DOWN;

    // Get the actors currently in collision with the mouse actor, and their count
    actors = getAllActorsInCollision("a_geuiMouse.0", &count);

    if (!actors) { DEBUG_MSG_FROM("no actors at mouse position", "updateMouseButtonDown"); return 1; }

    // If there currently is an array of actors stored for the mouse button, it needs to be emptied
    if (GEUIController.mButtonActors[mButtonNumber])
    {
        free(GEUIController.mButtonActors[mButtonNumber]);
        GEUIController.mButtonActors[mButtonNumber] = NULL;
        GEUIController.mButtonActorCount[mButtonNumber] = 0;
    }

    // Reset the variable used to store the top actor's name
    strcpy(GEUIController.mButtonTopActorCName[mButtonNumber], "");

    // Allocate memory for the array of actors in the current mouse actor position
    GEUIController.mButtonActors[mButtonNumber] = malloc(count * sizeof *actors);
    GEUIController.mButtonActorCount[mButtonNumber] = count; // Store the count of actors

    if (GEUIController.mButtonActors[mButtonNumber]) // If the memory allocation succeeded
    {
        // Copy the contents of the array returned by getAllActorsInCollision to
        // the allocated chunk of memory
        memcpy(GEUIController.mButtonActors[mButtonNumber], actors, count * sizeof *actors);
    }
    else // If the memory allocation failed
        { DEBUG_MSG_FROM("memory allocation failed", "updateMouseButtonDown"); return 2; }

    GEUIController.activeButton = mButtonNumber;

    // Of the clicked actors, find the one with the highest z depth
    if (GEUIController.mButtonActorCount[mButtonNumber] > 0)
        strcpy(GEUIController.mButtonTopActorCName[mButtonNumber],
            GEUIController.mButtonActors[mButtonNumber][GEUIController.mButtonActorCount[mButtonNumber] - 1].clonename);

    // Iterate through the array of actors and send an activation event to each of them
    for (i = 0; i < GEUIController.mButtonActorCount[mButtonNumber]; i ++)
    {
        SendActivationEvent(GEUIController.mButtonActors[mButtonNumber][i].clonename);
    }

    return 0;
}

void updateMouseButtonUp(enum mouseButtonsEnum mButtonNumber)
{
    int i;

    GEUIController.mButtonState[mButtonNumber] = GEUI_MOUSE_UP;
    GEUIController.activeButton = mButtonNumber;

    // If a top actor exists
    if (strlen(GEUIController.mButtonTopActorCName[mButtonNumber]) > 0)
        // Send activation event to the top actor
        SendActivationEvent(GEUIController.mButtonTopActorCName[mButtonNumber]);

    // Reset the variable used to store the top actor's name
    strcpy(GEUIController.mButtonTopActorCName[mButtonNumber], "");

    // If there is an array actors stored for the mouse button
    if (GEUIController.mButtonActors[mButtonNumber])
    {
        // Iterate through the array of actors and send an activation event to each of them
        for (i = 0; i < GEUIController.mButtonActorCount[mButtonNumber]; i ++)
        {
            SendActivationEvent(GEUIController.mButtonActors[mButtonNumber][i].clonename);
        }

        free(GEUIController.mButtonActors[mButtonNumber]);
        GEUIController.mButtonActors[mButtonNumber] = NULL;
        GEUIController.mButtonActorCount[mButtonNumber] = 0;
    }
}

void doKeyDown(WindowItem *item, int key)
{
    if (item)
    {
        switch (item->type)
        {
            case GEUI_Button:
                if (key == KEY_RETURN || key == KEY_SPACE)
                {
                    colorGuiTiles(item->data.button.tiles, item->parent->style.buttonPressedColor);
                    item->data.button.state = 1;
                }
            break;
            case GEUI_Input:
                handleInputFieldInput(&item->data.input, key);
                refreshText(&item->data.input.text);
                updateCaretPosition(&item->data.input.caret);
            break;
        }
    }

    switch (key)
    {
        case KEY_TAB:
            focusNextItemInWindow();
        break;
    }
}

void doKeyUp(WindowItem *item, int key)
{
    if (item)
    {
        switch (item->type)
        {
            case GEUI_Button:
                if ((key == KEY_RETURN || key == KEY_SPACE) && item->data.button.state == 1)
                {
                    if (item->data.button.actionFunction)
                        item->data.button.actionFunction(item->parent, item);

                    colorGuiTiles(item->data.button.tiles, item->parent->style.buttonColor);
                    item->data.button.state = 0;
                }
            break;
        }
    }
}


// ..\source\geui\60-geui-controller.c
void initGEUI(KeyboardLayout kbLayout);
void quitGEUI(void);
void destroyWindowList(void);

// BEGIN SABRE WORLD EDITOR (SWE) MODIFICATION
void enableGEUIMouse();
void disableGEUIMouse();
int isGEUIActive();
// END SABRE WORLD EDITOR (SWE) MODIFICATION

void initGEUI(KeyboardLayout kbLayout)
{
    //DEBUG_INIT(); // debug file initialization on startup disabled for now
    // TODO: separate DEBUG_INIT() and DEBUG_INIT_FILE()

    strcpy(defStyle.guiAnim, "gui_sheet_default");  // GUI animation name
    getTileDimensions(&defStyle);
    defStyle.titleFont          = &defTitleFont;
    defStyle.labelFont          = &defLabelFont;
    defStyle.textFont           = &defTextFont;
    defStyle.padding            = 5;
    defStyle.focusWidth         = 2;
    defStyle.titleBgColor       = DEFAULT_COLOR;
    defStyle.windowBgColor      = DEFAULT_COLOR;
    defStyle.inputBgColor       = DEFAULT_COLOR;
    defStyle.titleColor         = BLACK;
    defStyle.labelColor         = BLACK;
    defStyle.textColor          = BLACK;
    defStyle.buttonColor        = DEFAULT_COLOR;
    defStyle.buttonHilitColor   = CYAN;
    defStyle.buttonPressedColor = BLUE;
    defStyle.focusColor         = createRGB(255, 0, 128, 1.0);

    GEUIController.wIndex = 0;
    GEUIController.topIndex = -1;
    GEUIController.sDefault = defStyle;
    GEUIController.kbLayout = kbLayout;
    GEUIController.wList = NULL;
    GEUIController.focus = NULL;
    GEUIController.focusTiles = noIndices;

    // BEGIN SABRE WORLD EDITOR (SWE) MODIFICATION
    GEUIController.openWindowCount = 0;
    disableGEUIMouse();
    // END SABRE WORLD EDITOR (SWE) MODIFICATION
}

void setKeyboardLayout(KeyboardLayout kbLayout)
{
    GEUIController.kbLayout = kbLayout;
}

void quitGEUI(void)
{
    int mb;

    for (mb = 0; mb < GEUI_MOUSE_BUTTONS; mb ++)
    {
        if (GEUIController.mButtonActors[mb])
        {
            free(GEUIController.mButtonActors[mb]);
            GEUIController.mButtonActors[mb] = NULL;
        }
    }

    destroyWindowList();
}

// BEGIN SABRE WORLD EDITOR (SWE) MODIFICATION
void enableGEUIMouse()
{
    enableMouseEvents("a_mouseSensor");

    ChangeParent("a_mouseSensor", "view");
    getclone("a_mouseSensor")->x = 0;
    getclone("a_mouseSensor")->y = 0;
    ChangeZDepth("a_mouseSensor", 1.0);

    SendActivationEvent("status"); // reset tooltip text
}

void disableGEUIMouse()
{
    disableMouseEvents("a_mouseSensor");

    ChangeParent("a_mouseSensor", "(none)");
    getclone("a_mouseSensor")->x = getclone("view")->x;
    getclone("a_mouseSensor")->y = getclone("view")->y;
    ChangeZDepth("a_mouseSensor", 0.0);
}

int isGEUIActive()
{
    return GEUIController.openWindowCount > 0;
}
// END SABRE WORLD EDITOR (SWE) MODIFICATION

void destroyWindowList(void)
{
    Window *temp = NULL;
    Window *ptr = GEUIController.wList;

    while (ptr)
    {
        temp = ptr->next;
        destroyWindow(ptr);
        ptr = temp;
    }

    GEUIController.wList = NULL;
    GEUIController.wIndex = 0;

    // BEGIN SABRE WORLD EDITOR (SWE) MODIFICATION
    GEUIController.openWindowCount = 0;
    disableGEUIMouse();
    // END SABRE WORLD EDITOR (SWE) MODIFICATION
}


// ..\source\geui\99-geui_layout_visualization.c
char geuiDebugIconAnimpos;
Color geuiDebugIconColor;

void visualize(Window *window, WindowItem *panelItem, Color color)
{
    if (window && window->isOpen)
    {
        long x, y, xOrigin = 0, yOrigin = 0;
        short row, col;
        Actor *parent;
        Panel *panel;

        if (panelItem)
            panel = panelItem->data.panel;
        else
            panel = &window->mainPanel;

        parent = getclone(window->parentCName);

        if (actorExists2(parent))
        {
            xOrigin = parent->xscreen;
            yOrigin = parent->yscreen;
        }

        geuiDebugIconColor = color;

        for (row = 0; row <= panel->rows; row++)
        {
            for (col = 0; col <= panel->cols; col++)
            {
                y = getRowStart(panelItem, panel, row)-(row > 0);
                x = getColStart(panelItem, panel, col)-(col > 0);
                geuiDebugIconAnimpos = calculateAnimpos(panel->cols + 1, panel->rows + 1, col, row);
                SendActivationEvent(a_debugIcon.clonename);
                SendActivationEvent(a_debugIcon.clonename); // coloring doesn't work properly
                                                            // without the second activation event
                draw_from("a_debugIcon", xOrigin + x, yOrigin + y, 1);
            }
        }
    }
}

void printVisualizationData(Window *window, WindowItem *panelItem)
{
    if (window && window->isOpen)
    {
        long x, y;
        short row, col;
        char temp[256];
        char panelTag[50];
        Panel *panel;

        if (panelItem)
            panel = panelItem->data.panel;
        else
            panel = &window->mainPanel;

        for (row = 0; row <= panel->rows; row++)
        {
            for (col = 0; col <= panel->cols; col++)
            {
                y = getRowStart(panelItem, panel, row);
                x = getColStart(panelItem, panel, col);

                if (panelItem)
                    sprintf(panelTag, "%s.%d w: %d h: %d cw: %d rh: %d", panelItem->tag, window->index, getPanelWidth(panelItem->data.panel), getPanelHeight(panelItem->data.panel), getColWidth(panelItem->data.panel, col), getRowHeight(panelItem->data.panel, row));
                else
                    sprintf(panelTag, "mainPanel.%d w: %d h: %d cw: %d rh: %d", window->index, getPanelWidth(&window->mainPanel), getPanelHeight(&window->mainPanel), getColWidth(&window->mainPanel, col), getRowHeight(&window->mainPanel, row));
                sprintf(temp, "%s row: %d, col: %d, x: %d, y: %d, rows: %d", panelTag, row, col, x, y, window->mainPanel.rows);
                DEBUG_MSG(temp);
            }
        }
    }
}


