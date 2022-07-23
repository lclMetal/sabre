typedef struct SABRE_ColorStruct
{
    double hue;
    double saturation;
    double brightness;

    unsigned char r;
    unsigned char g;
    unsigned char b;

    double alpha;
}SABRE_Color;

#define SABRE_COLOR_DEFAULT {   0.0,   0.0, 100.0, 255, 255, 255, 1.0 }
#define SABRE_COLOR_WHITE   {   0.0,   0.0, 100.0, 255, 255, 255, 1.0 }
#define SABRE_COLOR_BLACK   {   0.0,   0.0,   0.0,   0,   0,   0, 1.0 }
#define SABRE_COLOR_RED     {   0.0, 100.0, 100.0, 255,   0,   0, 1.0 }
#define SABRE_COLOR_GREEN   { 120.0, 100.0, 100.0,   0, 255,   0, 1.0 }
#define SABRE_COLOR_BLUE    { 240.0, 100.0, 100.0,   0,   0, 255, 1.0 }
#define SABRE_COLOR_YELLOW  {  60.0, 100.0, 100.0, 255, 255,   0, 1.0 }
#define SABRE_COLOR_MAGENTA { 300.0, 100.0, 100.0, 255,   0, 255, 1.0 }
#define SABRE_COLOR_CYAN    { 180.0, 100.0, 100.0,   0, 255, 255, 1.0 }

#define SABRE_DEFAULT_COLOR   SABRE_predefinedColors[0]
#define SABRE_WHITE           SABRE_predefinedColors[0]
#define SABRE_BLACK           SABRE_predefinedColors[1]
#define SABRE_RED             SABRE_predefinedColors[2]
#define SABRE_GREEN           SABRE_predefinedColors[3]
#define SABRE_BLUE            SABRE_predefinedColors[4]
#define SABRE_YELLOW          SABRE_predefinedColors[5]
#define SABRE_MAGENTA         SABRE_predefinedColors[6]
#define SABRE_CYAN            SABRE_predefinedColors[7]

SABRE_Color SABRE_predefinedColors[] =
{
    SABRE_COLOR_WHITE, SABRE_COLOR_BLACK, SABRE_COLOR_RED, SABRE_COLOR_GREEN,
    SABRE_COLOR_BLUE, SABRE_COLOR_YELLOW, SABRE_COLOR_MAGENTA, SABRE_COLOR_CYAN
};

SABRE_Color SABRE_CreateRGB(int rValue, int gValue, int bValue, double aValue);
SABRE_Color SABRE_CreateHSB(double hValue, double sValue, double bValue, double aValue);
SABRE_Color SABRE_HSBtoRGB(double hValue, double sValue, double bValue, double aValue);
SABRE_Color SABRE_RGBtoHSB(int rValue, int gValue, int bValue, double aValue);
int SABRE_CompareColors(SABRE_Color *color1, SABRE_Color *color2);
SABRE_Color SABRE_GetActorColor(const char *actorName);
void SABRE_ColorThisActor(SABRE_Color color);
void SABRE_ColorActor(Actor *pActor, SABRE_Color color);
void SABRE_ColorActorByName(const char *actorName, SABRE_Color color);
void SABRE_ColorClones(const char *actorName, long startIndex, long endIndex, SABRE_Color color);

// This function creates a color with given values of red, green and blue, and then returns the color
// rValue - the value of the red component of the color
// gValue - the value of the green component of the color
// bValue - the value of the blue component of the color
// aValue - the alpha of the color
SABRE_Color SABRE_CreateRGB(int rValue, int gValue, int bValue, double aValue)
{
    SABRE_Color color;
    SABRE_Color hsb;

    color.r = rValue;
    color.g = gValue;
    color.b = bValue;
    color.alpha = aValue;

    hsb = SABRE_RGBtoHSB(rValue, gValue, bValue, color.alpha);

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
SABRE_Color SABRE_CreateHSB(double hValue, double sValue, double bValue, double aValue)
{
    SABRE_Color color;
    SABRE_Color rgb;

    color.hue = hValue;
    color.saturation = sValue;
    color.brightness = bValue;
    color.alpha = aValue;

    rgb = SABRE_HSBtoRGB(hValue, sValue, bValue, color.alpha);

    color.r = rgb.r;
    color.g = rgb.g;
    color.b = rgb.b;

    return color;
}

// This function converts given HSB color system values to corresponding RGB system color values
// and returns a SABRE_Color with the r, g and b values
// hValue - the hue value of the color to be converted to RGB
// sValue - the saturation value of the color to be converted to RGB
// bValue - the brightness value of the color to be converted to RGB
// aValue - the alpha of the color
SABRE_Color SABRE_HSBtoRGB(double hValue, double sValue, double bValue, double aValue)
{
    SABRE_Color color;

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
// and returns a SABRE_Color with the hue, saturation and brightness values
// rValue - the red component of the color to be converted to HSB
// gValue - the green component of the color to be converted to HSB
// bValue - the blue component of the color to be converted to HSB
// aValue - the alpha of the color
SABRE_Color SABRE_RGBtoHSB(int rValue, int gValue, int bValue, double aValue)
{
    SABRE_Color color;

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
int SABRE_CompareColors(SABRE_Color *color1, SABRE_Color *color2)
{
    if (!color1 || !color2) return -1;
    return (color1->r == color2->r && color1->g == color2->g && color1->b == color2->b);
}

// This function retrieves an actor's color
// actorName - the name of the actor
// returns: the retrieved color
SABRE_Color SABRE_GetActorColor(const char *actorName)
{
    Actor *a = getclone(actorName);

    if (a->cloneindex < 0)
        return SABRE_CreateRGB(0, 0, 0, 1.0);
    else
        return SABRE_CreateRGB(a->r, a->g, a->b, 1.0 - a->transp);
}

// This function colors the current actor with a given color
// color - the color to color the actor with
void SABRE_ColorThisActor(SABRE_Color color)
{
    r = color.r;
    g = color.g;
    b = color.b;
    transp = 1.0 - color.alpha;
}

// This function colors the given actor with a given color
// pActor - the pointer to the actor to be colored
// color - the color to color the actor with
void SABRE_ColorActor(Actor *pActor, SABRE_Color color)
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
void SABRE_ColorActorByName(const char *actorName, SABRE_Color color)
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
void SABRE_ColorClones(const char *actorName, long startIndex, long endIndex, SABRE_Color color)
{
    long i;
    char cName[256];

    for (i = startIndex; i <= endIndex; i ++)
    {
        sprintf(cName, "%s.%i", actorName, i);
        SABRE_ColorActorByName(cName, color);
    }
}
