// ..\source\global-code\05-color.c
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


// ..\source\global-code\10-vector2.c
#define SABRE_VECTOR2_ZERO SABRE_CreateVector2(0.0f, 0.0f)

typedef struct SABRE_Vector2Struct
{
    float x, y;
}SABRE_Vector2;

SABRE_Vector2 SABRE_CreateVector2(float x, float y)
{
    SABRE_Vector2 new;
    new.x = x;
    new.y = y;
    return new;
}

SABRE_Vector2 SABRE_ScaleVector2(SABRE_Vector2 vec, float scale)
{
    SABRE_Vector2 new;
    new.x = vec.x * scale;
    new.y = vec.y * scale;
    return new;
}

void SABRE_ScaleVector2InPlace(SABRE_Vector2 *vec, float scale)
{
    vec->x *= scale;
    vec->y *= scale;
}

SABRE_Vector2 SABRE_RotateVector2(SABRE_Vector2 vec, float rotation)
{
    SABRE_Vector2 new;
    new.x = vec.x * cos(rotation) - vec.y * sin(rotation);
    new.y = vec.x * sin(rotation) + vec.y * cos(rotation);
    return new;
}

void SABRE_RotateVector2InPlace(SABRE_Vector2 *vec, float rotation)
{
    float oldX = vec->x;
    vec->x = vec->x * cos(rotation) - vec->y * sin(rotation);
    vec->y = oldX   * sin(rotation) + vec->y * cos(rotation);
}

SABRE_Vector2 SABRE_AddVector2(SABRE_Vector2 a, SABRE_Vector2 b)
{
    SABRE_Vector2 new;
    new.x = a.x + b.x;
    new.y = a.y + b.y;
    return new;
}

void SABRE_AddVector2InPlace(SABRE_Vector2 *a, SABRE_Vector2 b)
{
    a->x += b.x;
    a->y += b.y;
}

SABRE_Vector2 SABRE_SubstractVector2(SABRE_Vector2 a, SABRE_Vector2 b)
{
    SABRE_Vector2 new;
    new.x = a.x - b.x;
    new.y = a.y - b.y;
    return new;
}

float SABRE_DotProductVector2(SABRE_Vector2 a, SABRE_Vector2 b)
{
    return a.x * b.x + a.y * b.y;
}

float SABRE_MagnitudeVector2(SABRE_Vector2 a)
{
    return sqrt(SABRE_DotProductVector2(a, a));
}

SABRE_Vector2 SABRE_NormalizeVector2(SABRE_Vector2 vec)
{
    float magnitude = SABRE_MagnitudeVector2(vec);

    if (abs(magnitude) <= 0.0001f)
    {
        DEBUG_MSG_FROM("Unable to normalize: vector magnitude was 0.", "SABRE_NormalizeVector2");
        return SABRE_CreateVector2(0.0f, 0.0f);
    }

    return SABRE_ScaleVector2(vec, 1.0f / magnitude);
}

void SABRE_NormalizeVector2InPlace(SABRE_Vector2 *vec)
{
    float magnitude = SABRE_MagnitudeVector2(*vec);

    if (abs(magnitude) <= 0.0001f)
    {
        DEBUG_MSG_FROM("Unable to normalize: vector magnitude was 0.", "SABRE_NormalizeVector2InPlace");
        vec->x = 0.0f;
        vec->y = 0.0f;
        return;
    }

    SABRE_ScaleVector2InPlace(vec, 1.0f / magnitude);
}

int SABRE_IsZeroVector2(SABRE_Vector2 vec)
{
    return (vec.x == 0 && vec.y == 0);
}


// ..\source\global-code\15-vector3.c
#define SABRE_VECTOR3_ZERO SABRE_CreateVector3(0.0f, 0.0f, 0.0f)

typedef struct SABRE_Vector3Struct
{
    float x, y, z;
}SABRE_Vector3;

SABRE_Vector3 SABRE_CreateVector3(float x, float y, float z)
{
    SABRE_Vector3 new;
    new.x = x;
    new.y = y;
    new.z = z;
    return new;
}

SABRE_Vector3 SABRE_Vector2ToVector3(SABRE_Vector2 vec2, float z)
{
    SABRE_Vector3 new;
    new.x = vec2.x;
    new.y = vec2.y;
    new.z = z;
    return new;
}

SABRE_Vector2 SABRE_Vector3ToVector2WithoutZ(SABRE_Vector3 vec3)
{
    SABRE_Vector2 new;
    new.x = vec3.x;
    new.y = vec3.y;
    return new;
}

SABRE_Vector3 SABRE_ScaleVector3(SABRE_Vector3 vec, float scale)
{
    SABRE_Vector3 new;
    new.x = vec.x * scale;
    new.y = vec.y * scale;
    new.z = vec.z * scale;
    return new;
}

void SABRE_ScaleVector3InPlace(SABRE_Vector3 *vec, float scale)
{
    vec->x *= scale;
    vec->y *= scale;
    vec->z *= scale;
}

SABRE_Vector3 SABRE_AddVector3(SABRE_Vector3 a, SABRE_Vector3 b)
{
    SABRE_Vector3 new;
    new.x = a.x + b.x;
    new.y = a.y + b.y;
    new.z = a.z + b.z;
    return new;
}

void SABRE_AddVector3InPlace(SABRE_Vector3 *a, SABRE_Vector3 b)
{
    a->x += b.x;
    a->y += b.y;
    a->z += b.z;
}

float SABRE_DotProductVector3(SABRE_Vector3 a, SABRE_Vector3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float SABRE_MagnitudeVector3(SABRE_Vector3 a)
{
    return sqrt(SABRE_DotProductVector3(a, a));
}

SABRE_Vector3 SABRE_NormalizeVector3(SABRE_Vector3 vec)
{
    float magnitude = SABRE_MagnitudeVector3(vec);

    if (abs(magnitude) <= 0.0001f)
    {
        DEBUG_MSG_FROM("Unable to normalize: vector magnitude was 0.", "SABRE_NormalizeVector3");
        return SABRE_CreateVector3(0.0f, 0.0f, 0.0f);
    }

    return SABRE_ScaleVector3(vec, 1.0f / magnitude);
}

void SABRE_NormalizeVector3InPlace(SABRE_Vector3 *vec)
{
    float magnitude = SABRE_MagnitudeVector3(*vec);

    if (abs(magnitude) <= 0.0001f)
    {
        DEBUG_MSG_FROM("Unable to normalize: vector magnitude was 0.", "SABRE_NormalizeVector3InPlace");
        vec->x = 0.0f;
        vec->y = 0.0f;
        vec->z = 0.0f;
        return;
    }

    SABRE_ScaleVector3InPlace(vec, 1.0f / magnitude);
}


// ..\source\global-code\20-misc-functions.c
float SABRE_LimitValue01(float val)
{
    return max(0, min(1, val));
}

// algorithm by user Grumdrig on stack overflow: https://stackoverflow.com/a/1501725
float SABRE_PointToLineSegmentDistance(SABRE_Vector2 a, SABRE_Vector2 b, SABRE_Vector2 p, SABRE_Vector2 *projectionResult)
{
    float projectionLength;
    SABRE_Vector2 projection;
    SABRE_Vector2 ab = SABRE_SubstractVector2(b, a);
    SABRE_Vector2 ap = SABRE_SubstractVector2(p, a);
    float l2 = pow(a.x - b.x, 2) + pow(a.y - b.y, 2);

    if (l2 == 0.0f) return distance(p.x, p.y, a.x, a.y);

    projectionLength = SABRE_LimitValue01(SABRE_DotProductVector2(ap, ab) / (float)l2);
    projection = SABRE_AddVector2(a, SABRE_ScaleVector2(ab, projectionLength));

    if (projectionResult)
        *projectionResult = projection;

    return distance(p.x, p.y, projection.x, projection.y);
}

int SABRE_LineToPointCollision(SABRE_Vector2 a, SABRE_Vector2 b, SABRE_Vector2 p, float lineWidth, float pointRadius)
{
    float collisionThreshold = lineWidth + pointRadius;
    return SABRE_PointToLineSegmentDistance(a, b, p, NULL) <= collisionThreshold;
}

float SABRE_GetPositiveBiasedSign(float val)
{
    if (val < 0.0f) return -1.0f;
    return 1.0f;
}

float SABRE_RandomBetween(float lim1, float lim2)
{
    return rand(max(lim1, lim2) - min(lim1, lim2)) + min(lim1, lim2);
}

float SABRE_NormalizeAngleTo360(float ang)
{
    float tempAng = ang;

    while (tempAng < 0)
    {
        tempAng += 360;
    }

    while (tempAng > 360.0f)
    {
        tempAng -= 360;
    }

    return tempAng;
}

int SABRE_ActorExists(const char *actorName)
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
int SABRE_ActorExists2(Actor *a)
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

int SABRE_StringEndsWith(const char *str, const char *str2)
{
    size_t len1 = strlen(str);
    size_t len2 = strlen(str2);

    if (len1 < len2)
    {
        return 0;
    }

    return !strcmp(&str[len1 - len2], str2);
}

#define SABRE_ANIM_WIDTH 0
#define SABRE_ANIM_HEIGHT 1
#define SABRE_ANIM_NFRAMES 2

int SABRE_GetAnimationDataValue(const char actorName[256], const char animName[256], unsigned char dataValueType)
{
    int i = 0;
    int dimensionPixels = 0;
    int animFrameCount = 0;
    Actor *animationActor = getclone(actorName);

    if (!SABRE_ActorExists2(animationActor))
    {
        DEBUG_MSG_FROM("The actor doesn't exist.", "SABRE_GetAnimationDimensionInPixels");
        return 1;
    }

    ChangeAnimation(animationActor->clonename, animName, STOPPED);
    SendActivationEvent(actorName); // this "finalizes" the animation change, resetting the actor's animpos
    animFrameCount = animationActor->nframes;

    if (dataValueType == SABRE_ANIM_NFRAMES)
        return animFrameCount;

    for (i = 0; i < animFrameCount; i++)
    {
        animationActor = getclone(animationActor->clonename); // this updates the width and height values

        switch (dataValueType)
        {
            default:
            case SABRE_ANIM_WIDTH:
                if (animationActor->width > dimensionPixels)
                {
                    dimensionPixels = animationActor->width;
                }
                break;

            case SABRE_ANIM_HEIGHT:
                if (animationActor->height > dimensionPixels)
                {
                    dimensionPixels = animationActor->height;
                }
                break;
        }

        animationActor->animpos++;
        // Send activation event to apply the animpos advancement during this frame already.
        // The normal behavior of Game Editor is to update the animpos of an actor in the next
        // frame. This same trick is used for changing the TextureSlice's animpos multiple
        // times per frame when drawing the game view. Notice, however, that if you were to
        // inspect the actor during a frame where this trick is used you still wouldn't see
        // the animpos changing more than once during a single frame. This is because Game Editor
        // only draws the actor on screen once per frame. But behind the scenes the animpos
        // still changes multiple times per frame, affecting the actor's dimensions as well as
        // its appearance if drawn using draw_from().
        SendActivationEvent(actorName);
    }

    return dimensionPixels;
}


// ..\source\global-code\30-data-store.c
#define SABRE_DATA_STORE_DOUBLING_LIMIT 128
#define SABRE_DATA_STORE_GROW_AMOUNT 64 // SABRE_DATA_STORE_DOUBLING_LIMIT / 2

#define SABRE_DATA_STORE_AS_CAST_ARRAY(DATA_STORE, DATA_TYPE) (DATA_TYPE(DATA_STORE).elems)

typedef struct SABRE_DataStoreStruct
{
    size_t capacity; // the maximum amount of elements the store can hold at the moment
    size_t count; // the amount of elements the store actually holds at the moment
    size_t elemSize; // the size of a single element in the store
    void *elems; // pointer to the elements
    void (*addFunc)(struct SABRE_DataStoreStruct*, void*);
}SABRE_DataStore;

void SABRE_SetDataStoreAddFunc(SABRE_DataStore *dataStore, void (*addDataFunc)(SABRE_DataStore*, void*));
int SABRE_InitDataStore(SABRE_DataStore *dataStore, size_t elemSize);
int SABRE_GrowDataStore(SABRE_DataStore *dataStore);
int SABRE_PrepareDataStore(SABRE_DataStore *dataStore);
int SABRE_AddToDataStore(SABRE_DataStore *dataStore, void *elem);
void SABRE_FreeDataStore(SABRE_DataStore *dataStore);

void SABRE_SetDataStoreAddFunc(SABRE_DataStore *dataStore, void (*addDataFunc)(SABRE_DataStore*, void*))
{
    dataStore->addFunc = addDataFunc;
}

int SABRE_InitDataStore(SABRE_DataStore *dataStore, size_t elemSize)
{
    dataStore->capacity = 16;
    dataStore->count = 0;
    dataStore->elemSize = elemSize;
    dataStore->elems = calloc(dataStore->capacity, dataStore->elemSize);

    if (!dataStore->elems)
    {
        DEBUG_MSG_FROM("Memory allocation failed!", "SABRE_InitDataStore");
        return 1;
    }

    return 0;
}

int SABRE_GrowDataStore(SABRE_DataStore *dataStore)
{
    void *newElems = NULL;

    // double the data store size or grow it by SABRE_DATA_STORE_GROW_AMOUNT
    if (dataStore->capacity < SABRE_DATA_STORE_DOUBLING_LIMIT) dataStore->capacity *= 2;
    else dataStore->capacity += SABRE_DATA_STORE_GROW_AMOUNT;

    newElems = realloc(dataStore->elems, dataStore->capacity);

    if (!newElems)
    {
        DEBUG_MSG_FROM("Memory allocation failed!", "SABRE_GrowDataStore");
        return 1;
    }

    dataStore->elems = newElems;

    return 0;
}

int SABRE_PrepareDataStore(SABRE_DataStore *dataStore)
{
    // the data store has not been initialized, initialize it and make sure no errors occurred
    if (!dataStore->capacity && SABRE_InitDataStore(dataStore, dataStore->elemSize) != 0)
    {
        DEBUG_MSG_FROM("Unable to initialize data store.", "SABRE_PrepareDataStore");
        return 1;
    }
    // the data store is full, grow it and make sure no errors occurred
    else if (dataStore->count == dataStore->capacity && SABRE_GrowDataStore(dataStore) != 0)
    {
        DEBUG_MSG_FROM("Unable to grow data store.", "SABRE_PrepareDataStore");
        return 2;
    }
    // otherwise no-op

    return 0;
}

int SABRE_AddToDataStore(SABRE_DataStore *dataStore, void *elem)
{
    int err = 0;

    err = SABRE_PrepareDataStore(dataStore);

    if (err != 0) return err;

    dataStore->addFunc(dataStore, elem);
    dataStore->count++; // new element has been added, increment count

    return 0;
}

void SABRE_FreeDataStore(SABRE_DataStore *dataStore)
{
    if (dataStore->elems)
    {
        free(dataStore->elems);
        dataStore->capacity = 0;
        dataStore->count = 0;
        dataStore->elemSize = 0;
        dataStore->elems = NULL;
    }
}


// ..\source\global-code\32-list.c
#ifndef SABRE_ANIMATION_DEFINED
typedef struct SABRE_AnimationStruct
{
    float frameRate;
    unsigned int nframes;
    unsigned int sprite;
}SABRE_Animation;
#define SABRE_ANIMATION_DEFINED
#endif

#ifndef SABRE_ANIMATOR_DEFINED
typedef struct SABRE_AnimatorStruct
{
    char state;
    unsigned int animpos;
    float accumulatedAnimpos;
    SABRE_Animation anim;
}SABRE_Animator;
#define SABRE_ANIMATOR_DEFINED
#endif

#ifndef SABRE_ENTITY_DEFINED
typedef struct SABRE_EntityStruct
{
    float radius;
    SABRE_Vector3 pos;
    unsigned char attributes;
    SABRE_Animator animator;
    char name[256];
}SABRE_Entity;
#define SABRE_ENTITY_DEFINED
#endif

#ifndef SABRE_PROJECTILE_DEFINED
typedef struct SABRE_ProjectileStruct
{
    float speed;
    float dropFactor;
    SABRE_Vector3 dir;
    SABRE_Entity *entity;
}SABRE_Projectile;
#define SABRE_PROJECTILE_DEFINED
#endif

typedef union SABRE_ListTypesUnion
{
    struct SABRE_EntityStruct entity;
    struct SABRE_ProjectileStruct projectile;
}SABRE_ListTypes;

typedef struct SABRE_ListStruct
{
    struct SABRE_ListStruct *next;
    struct SABRE_ListStruct *prev;
    SABRE_ListTypes data;
}SABRE_List;

SABRE_List *SABRE_AddToList(SABRE_List **list, SABRE_ListTypes elem);
void SABRE_FreeList(SABRE_List *list);

SABRE_List *SABRE_AddToList(SABRE_List **list, SABRE_ListTypes elem)
{
    SABRE_List *new = malloc(sizeof *new);

    if (!new)
    {
        DEBUG_MSG_FROM("Malloc failed!", "SABRE_AddToList");
        return NULL;
    }

    new->data = elem;
    new->next = NULL;
    new->prev = NULL;

    if (list && !(*list))
    {
        new->next = NULL;
        new->prev = NULL;
        *list = new;
        return *list;
    }
    else if (!list)
    {
        DEBUG_MSG_FROM("Invalid list pointer.", "SABRE_AddToList");
        free(new);
        return NULL;
    }

    new->next = *list;
    new->prev = NULL;
    new->next->prev = new;
    *list = new;
    return *list;
}

void SABRE_RemoveFromList(SABRE_List **list, SABRE_List *remove)
{
    if (!list || !(*list) || !remove) return;

    if (remove->prev)
        remove->prev->next = remove->next;
    else
        *list = remove->next;

    if (remove->next)
        remove->next->prev = remove->prev;

    if (!remove->prev && !remove->next)
    {
        *list = NULL;
    }

    free(remove);
}

void SABRE_FreeList(SABRE_List *list)
{
    SABRE_List *temp = NULL;
    SABRE_List *iterator = list;

    while (iterator)
    {
        temp = iterator->next;
        free(iterator);
        iterator = temp;
    }
}


// ..\source\global-code\35-engine.c
#define SABRE_CEILING_ACTOR "SABRE_Ceiling"
#define SABRE_FLOOR_ACTOR "SABRE_Floor"

typedef struct SABRE_SliceStruct
{
    short anim;
    short slice;
}SABRE_Slice;

#ifndef SABRE_RENDER_OBJECT_DEFINED
enum SABRE_RenderObjectTypeEnum
{
    SABRE_TEXTURE_RO,
    SABRE_SPRITE_RO
};

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
    char validated;

    unsigned width;
    unsigned height;
    SABRE_LevelTile *map;
}SABRE_Level;
#define SABRE_LEVEL_DEFINED
#endif

enum SABRE_GameStatesEnum
{
    SABRE_UNINITIALIZED = 0,
    SABRE_TEXTURES_ADDED,
    SABRE_SPRITES_ADDED,
    SABRE_RUNNING,
    SABRE_FINISHED
}SABRE_gameState = SABRE_UNINITIALIZED;

typedef struct SABRE_CameraStruct
{
    SABRE_Vector2 prevPos;
    SABRE_Vector2 pos;
    SABRE_Vector2 prevDir;
    SABRE_Vector2 dir;
    SABRE_Vector2 plane;
    float vPos;
}SABRE_Camera;

const float SABRE_heightUnit = 480.0f;
const float SABRE_halfHeightUnit = SABRE_heightUnit * 0.5f;
float SABRE_screenWidth = 640.0f, SABRE_screenHeight = 480.0f;

SABRE_Camera SABRE_camera;

struct SABRE_KeybindStruct
{
    short forward, backward;
    short turnLeft, turnRight;
    short strafeLeft, strafeRight;
    short crouch;
    short interact;
}SABRE_binds =
{
    KEY_w, KEY_s, // forward, backward
    KEY_a, KEY_d, // turn left, right
    KEY_q, KEY_e, // strafe left, right
    KEY_LCTRL,    // crouch
    KEY_r         // interact
};

typedef struct SABRE_KeyboardStateStruct
{
    char pressedForward,     releasedForward,     forward,     prevForward;
    char pressedBackward,    releasedBackward,    backward,    prevBackward;
    char pressedTurnLeft,    releasedTurnLeft,    turnLeft,    prevTurnLeft;
    char pressedTurnRight,   releasedTurnRight,   turnRight,   prevTurnRight;
    char pressedStrafeLeft,  releasedStrafeLeft,  strafeLeft,  prevStrafeLeft;
    char pressedStrafeRight, releasedStrafeRight, strafeRight, prevStrafeRight;
    char pressedCrouch,      releasedCrouch,      crouch,      prevCrouch;
    char pressedInteract,    releasedInteract,    interact,    prevInteract;
}SABRE_KeyboardState;

SABRE_KeyboardState SABRE_keys;

struct SABRE_PlayerStruct
{
    float moveSpeed;
    float turnSpeed;
    float crouchSpeed;
    float crouchHeightChange;
    float radius;
}SABRE_player = { 0.05f, 0.05f, 5.0f, 50.0f, 0.2f };

SABRE_Slice SABRE_slice;
SABRE_Color SABRE_defaultCeiling = { 215.0, 54.0, 91.0, 106, 158, 231, 1.0 };
SABRE_Color SABRE_defaultFloor   = {  86.0, 76.0, 62.0, 106, 158,  38, 1.0 };

// x = player
// 000     abc
// 0x0  => d e
// 000     fgh

#define SABRE_COLLISION_MASK_SIZE 7
#define SABRE_TOP_L      0x80 // a
#define SABRE_TOP_L_MASK 0xD0 // a && b && d
#define SABRE_TOP        0x40 // b
#define SABRE_TOP_MASK   0x40 // b
#define SABRE_TOP_R      0x20 // c
#define SABRE_TOP_R_MASK 0x68 // c && b && e
#define SABRE_LEFT       0x10 // d
#define SABRE_LEFT_MASK  0x10 // d
#define SABRE_RIGHT      0x08 // e
#define SABRE_RIGHT_MASK 0x08 // e
#define SABRE_LOW_L      0x04 // f
#define SABRE_LOW_L_MASK 0x16 // f && d && g
#define SABRE_LOW        0x02 // g
#define SABRE_LOW_MASK   0x02 // g
#define SABRE_LOW_R      0x01 // h
#define SABRE_LOW_R_MASK 0x0B // h && e && g

void SABRE_SetCeilingColor(SABRE_Color color)
{
    SABRE_ColorActorByName(SABRE_CEILING_ACTOR, color);
}

void SABRE_SetFloorColor(SABRE_Color color)
{
    SABRE_ColorActorByName(SABRE_FLOOR_ACTOR, color);
}

int SABRE_GetSurroundingWalls(float *px, float *py, SABRE_Level*level)
{
    int i, j, rows = 3, cols = 3, mid = 0, collisions = 0;

    for (j = 0; j < rows; j++)
    {
        for (i = 0; i < cols; i++)
        {
            if (j == 1 && i == 1) mid = 1;
            else
            {
                int row = (int)*py - 1 + j;
                int col = (int)*px - 1 + i;

                collisions += (level->map[row * level->width + col].texture > 0) << SABRE_COLLISION_MASK_SIZE - (j * cols + i - mid);
            }
        }
    }

    return collisions;
}

void SABRE_KeepDistance(float *x1, float *y1, float x2, float y2, float dist)
{
    float len = distance(*x1, *y1, x2, y2);
    float ang = degtorad(direction(x2, y2, *x1, *y1));

    if (len < dist)
    {
        // multiply the results by 1.001 to make the above condition evaluate
        // to false if the player hasn't moved since their position was last
        // modified by this function
        *x1 = x2 + cos(ang) * dist * 1.001f;
        *y1 = y2 + -sin(ang) * dist * 1.001f;
    }
}

void SABRE_UpdateKeyboardState()
{
    char *keys = GetKeyState();

    SABRE_keys.prevForward      = SABRE_keys.forward;
    SABRE_keys.prevBackward     = SABRE_keys.backward;
    SABRE_keys.prevTurnLeft     = SABRE_keys.turnLeft;
    SABRE_keys.prevTurnRight    = SABRE_keys.turnRight;
    SABRE_keys.prevStrafeLeft   = SABRE_keys.strafeLeft;
    SABRE_keys.prevStrafeRight  = SABRE_keys.strafeRight;
    SABRE_keys.prevCrouch       = SABRE_keys.crouch;
    SABRE_keys.prevInteract     = SABRE_keys.interact;

    SABRE_keys.forward          = keys[SABRE_binds.forward];
    SABRE_keys.backward         = keys[SABRE_binds.backward];
    SABRE_keys.turnLeft         = keys[SABRE_binds.turnLeft];
    SABRE_keys.turnRight        = keys[SABRE_binds.turnRight];
    SABRE_keys.strafeLeft       = keys[SABRE_binds.strafeLeft];
    SABRE_keys.strafeRight      = keys[SABRE_binds.strafeRight];
    SABRE_keys.crouch           = keys[SABRE_binds.crouch];
    SABRE_keys.interact         = keys[SABRE_binds.interact];

    SABRE_keys.pressedForward       = !SABRE_keys.prevForward       && SABRE_keys.forward;
    SABRE_keys.pressedBackward      = !SABRE_keys.prevBackward      && SABRE_keys.backward;
    SABRE_keys.pressedTurnLeft      = !SABRE_keys.prevTurnLeft      && SABRE_keys.turnLeft;
    SABRE_keys.pressedTurnRight     = !SABRE_keys.prevTurnRight     && SABRE_keys.turnRight;
    SABRE_keys.pressedStrafeLeft    = !SABRE_keys.prevStrafeLeft    && SABRE_keys.strafeLeft;
    SABRE_keys.pressedStrafeRight   = !SABRE_keys.prevStrafeRight   && SABRE_keys.strafeRight;
    SABRE_keys.pressedCrouch        = !SABRE_keys.prevCrouch        && SABRE_keys.crouch;
    SABRE_keys.pressedInteract      = !SABRE_keys.prevInteract      && SABRE_keys.interact;

    SABRE_keys.releasedForward      = SABRE_keys.prevForward        && !SABRE_keys.forward;
    SABRE_keys.releasedBackward     = SABRE_keys.prevBackward       && !SABRE_keys.backward;
    SABRE_keys.releasedTurnLeft     = SABRE_keys.prevTurnLeft       && !SABRE_keys.turnLeft;
    SABRE_keys.releasedTurnRight    = SABRE_keys.prevTurnRight      && !SABRE_keys.turnRight;
    SABRE_keys.releasedStrafeLeft   = SABRE_keys.prevStrafeLeft     && !SABRE_keys.strafeLeft;
    SABRE_keys.releasedStrafeRight  = SABRE_keys.prevStrafeRight    && !SABRE_keys.strafeRight;
    SABRE_keys.releasedCrouch       = SABRE_keys.prevCrouch         && !SABRE_keys.crouch;
    SABRE_keys.releasedInteract     = SABRE_keys.prevInteract       && !SABRE_keys.interact;
}

void SABRE_DisableActor(const char *actorName)
{
    VisibilityState(actorName, DISABLE);
    EventDisable(actorName, EVENTALL);
}

void SABRE_EnableActor(const char *actorName)
{
    VisibilityState(actorName, ENABLE);
    EventEnable(actorName, EVENTALL);
}

void SABRE_DisableActors()
{
    SABRE_DisableActor("SABRE_Screen");
    SABRE_DisableActor("SABRE_PlayerController");
    SABRE_DisableActor("SABRE_ProjectileHandler");
    SABRE_DisableActor("SABRE_TriggerHandler");
    SABRE_DisableActor("SABRE_TextureActor");
    SABRE_DisableActor("SABRE_SpriteActor");
    SABRE_DisableActor("SABRE_Ceiling");
    SABRE_DisableActor("SABRE_Floor");
}

void SABRE_EnableActors()
{
    SABRE_EnableActor("SABRE_Screen");
    SABRE_EnableActor("SABRE_PlayerController");
    SABRE_EnableActor("SABRE_ProjectileHandler");
    SABRE_EnableActor("SABRE_TriggerHandler");
    SABRE_EnableActor("SABRE_TextureActor");
    SABRE_EnableActor("SABRE_SpriteActor");
    SABRE_EnableActor("SABRE_Ceiling");
    SABRE_EnableActor("SABRE_Floor");
}

// from level.c
int SABRE_ValidateCurrentLevel();

// from entity.c
void SABRE_SetEntities();

void SABRE_Quit();

#define SABRE_STRINGIFY(X) #X
#define SABRE_EXPAND_STRINGIFY(X) SABRE_STRINGIFY(X)
#define SABRE_PROCESS_STEP_LABEL(NAME, STAGE, STAGES) "[" NAME " (" SABRE_STRINGIFY(STAGE) "/" SABRE_EXPAND_STRINGIFY(STAGES) ")] "

#define SABRE_INIT_STAGES 6
#define SABRE_INIT_STEP_LABEL(STAGE) SABRE_PROCESS_STEP_LABEL("init", STAGE, SABRE_INIT_STAGES)

#define SABRE_QUIT_STAGES 7
#define SABRE_QUIT_STEP_LABEL(STAGE) SABRE_PROCESS_STEP_LABEL("quit", STAGE, SABRE_QUIT_STAGES)

void SABRE_Start()
{
    SABRE_EnableActors();

    DEBUG_MSG_FROM(SABRE_INIT_STEP_LABEL(1) "Signal textureActor to start adding textures.", "SABRE_Start");
    SendActivationEvent("SABRE_TextureActor");
    if (SABRE_gameState == SABRE_TEXTURES_ADDED)
    {
        DEBUG_MSG_FROM(SABRE_INIT_STEP_LABEL(2) "Texture addition successful.", "SABRE_Start");
        DEBUG_MSG_FROM(SABRE_INIT_STEP_LABEL(3) "Signal spriteActor to start adding sprites.", "SABRE_Start");
        SendActivationEvent("SABRE_SpriteActor");
        if (SABRE_gameState == SABRE_SPRITES_ADDED)
        {
            DEBUG_MSG_FROM(SABRE_INIT_STEP_LABEL(4) "Sprite addition successful.", "SABRE_Start");

            if (SABRE_ValidateCurrentLevel() == 0)
                DEBUG_MSG_FROM(SABRE_INIT_STEP_LABEL(5) "Level validation done, no errors.", "SABRE_Start");
            else
                DEBUG_MSG_FROM(SABRE_INIT_STEP_LABEL(5) "Warning! At least one missing texture was detected.", "SABRE_Start");

            CreateActor("SABRE_Screen", "icon", "(none)", "(none)", view.x, view.y, true);
            CreateActor("SABRE_Ceiling", "background", "(none)", "(none)", view.x + view.width * 0.5, view.y + view.height * 0.5 - 270, true);
            CreateActor("SABRE_Floor", "background", "(none)", "(none)", view.x + view.width * 0.5, view.y + view.height * 0.5 + 270, true);
            SABRE_SetCeilingColor(SABRE_defaultCeiling);
            SABRE_SetFloorColor(SABRE_defaultFloor);
            SABRE_gameState = SABRE_RUNNING;
            DEBUG_MSG_FROM(SABRE_INIT_STEP_LABEL(6) "SABRE initialization complete.", "SABRE_Start");

            SABRE_SetEntities(); // Set the test entities
        }
        else
        {
            DEBUG_MSG_FROM(SABRE_INIT_STEP_LABEL(4) "Error! Sprite addition failed.", "SABRE_Start");
            SABRE_Quit();
        }
    }
    else
    {
        DEBUG_MSG_FROM(SABRE_INIT_STEP_LABEL(2) "Error! Texture addition failed.", "SABRE_Start");
        SABRE_Quit();
    }
}

int SABRE_FreeLevel();              // from level.c
void SABRE_FreeTextureStore();      // from texture.c
void SABRE_FreeSpriteStore();       // from sprite.c
void SABRE_FreeRenderObjectList();  // from render.c
void SABRE_FreeEntityList();        // from entity.c
void SABRE_FreeProjectileList();    // from projectile.c

void SABRE_Quit()
{
    if (SABRE_gameState != SABRE_FINISHED)
    {
        SABRE_DisableActors();

        SABRE_FreeLevel();
        DEBUG_MSG_FROM(SABRE_QUIT_STEP_LABEL(1) "Freed level data memory.", "SABRE_Quit");

        SABRE_FreeTextureStore();
        DEBUG_MSG_FROM(SABRE_QUIT_STEP_LABEL(2) "Freed texture store memory.", "SABRE_Quit");

        SABRE_FreeSpriteStore();
        DEBUG_MSG_FROM(SABRE_QUIT_STEP_LABEL(3) "Freed sprite store memory.", "SABRE_Quit");

        SABRE_FreeProjectileList();
        DEBUG_MSG_FROM(SABRE_QUIT_STEP_LABEL(4) "Freed projectile list memory.", "SABRE_Quit");

        SABRE_FreeEntityList();
        DEBUG_MSG_FROM(SABRE_QUIT_STEP_LABEL(5) "Freed entity list memory.", "SABRE_Quit");

        SABRE_FreeRenderObjectList();
        DEBUG_MSG_FROM(SABRE_QUIT_STEP_LABEL(6) "Freed render object list memory.", "SABRE_Quit");

        SABRE_gameState = SABRE_FINISHED;
        DEBUG_MSG_FROM(SABRE_QUIT_STEP_LABEL(7) "SABRE cleanup complete.", "SABRE_Quit");
    }
}


// ..\source\global-code\37-event.c
#define SABRE_EVENT_PERMANENT_CHANGE      0
#define SABRE_EVENT_PRESENCE_TOGGLE       1
#define SABRE_EVENT_PRESENCE_TOGGLE_ONCE  2
#define SABRE_EVENT_SIGNAL                3
#define SABRE_EVENT_SIGNAL_ONCE           4

#define SABRE_EVENT_FLAG_ACTIVE 1
#define SABRE_EVENT_FLAG_LOCKED 2

#define SABRE_TRIGGER_HANDLER "SABRE_TriggerHandler"

#define SABRE_EVENT_ACTIVATE(EVENT) do { EVENT->state |= SABRE_EVENT_FLAG_ACTIVE;   \
                                         SABRE_updatedTrigger = EVENT;              \
                                         SendActivationEvent(SABRE_TRIGGER_HANDLER); } while (0)
#define SABRE_EVENT_DEACTIVATE(EVENT) do { EVENT->state &= (~SABRE_EVENT_FLAG_ACTIVE);  \
                                           SABRE_updatedTrigger = EVENT;                \
                                           SendActivationEvent(SABRE_TRIGGER_HANDLER); } while (0)

#define SABRE_EVENT_LOCK(EVENT) do { EVENT->state |= SABRE_EVENT_FLAG_LOCKED; } while (0)
#define SABRE_EVENT_UNLOCK(EVENT) do { EVENT->state &= (~SABRE_EVENT_FLAG_LOCKED); } while (0)

#define SABRE_EVENT_ACTIVE(EVENT) (EVENT->state & SABRE_EVENT_FLAG_ACTIVE)
#define SABRE_EVENT_NOT_ACTIVE(EVENT) (EVENT->state & SABRE_EVENT_FLAG_ACTIVE) == 0

#define SABRE_EVENT_LOCKED(EVENT) (EVENT->state & SABRE_EVENT_FLAG_LOCKED)
#define SABRE_EVENT_NOT_LOCKED(EVENT) (EVENT->state & SABRE_EVENT_FLAG_LOCKED) == 0

typedef enum SABRE_EventNameEnum
{
    LVL1_CASH_MACHINE_EVENT = 0,
    LVL1_WINDOW_EVENT
}SABRE_EventName;

typedef struct SABRE_EventTriggerStruct
{
    unsigned char type;
    SABRE_EventName event;
    SABRE_Vector2 p1;
    SABRE_Vector2 p2;
    float facingDir;
    float facingFov;
    unsigned char state;
}SABRE_EventTrigger;

SABRE_EventTrigger event1 =
{
    SABRE_EVENT_PRESENCE_TOGGLE,
    LVL1_CASH_MACHINE_EVENT,
    { 1, 8 },
    { 2, 9 },
    0.0f, 45.0f,
    0
};

SABRE_EventTrigger event2 =
{
    SABRE_EVENT_PRESENCE_TOGGLE,
    LVL1_WINDOW_EVENT,
    { 0, 5 },
    { 2.6, 6 },
    0.0f, -30.0f
};

SABRE_EventTrigger *SABRE_updatedTrigger = NULL;

void UpdateEvents();
void SABRE_UpdateEvent(SABRE_EventTrigger *event);
void SABRE_EnterEventTrigger(SABRE_EventTrigger *event);
void SABRE_StayInEventTrigger(SABRE_EventTrigger *event);
void SABRE_LeaveEventTrigger(SABRE_EventTrigger *event);

#if DEBUG
void SABRE_ResetEvent(SABRE_EventTrigger *event);
void SABRE_CycleEventType(SABRE_EventTrigger *event);
#endif

void SABRE_UpdateEvent(SABRE_EventTrigger *event)
{
    SABRE_Vector2 camPos = SABRE_camera.pos;
    SABRE_Vector2 prevPos = SABRE_camera.prevPos;

    float deltaTo180 = 180.0f - event->facingDir;
    float normalizedDir = SABRE_NormalizeAngleTo360(direction(0, 0, SABRE_camera.dir.x, SABRE_camera.dir.y) + deltaTo180);
    float normalizedPrevDir = SABRE_NormalizeAngleTo360(direction(0, 0, SABRE_camera.prevDir.x, SABRE_camera.prevDir.y) + deltaTo180);

    short isFacingRightDirection = (event->facingFov < 0 || (abs(normalizedDir - 180.0f) <= event->facingFov / 2.0f));
    short wasFacingRightDirection = (event->facingFov < 0 || (abs(normalizedPrevDir - 180.0f) <= event->facingFov / 2.0f));

    short isInsideTrigger = (camPos.x >= event->p1.x &&
                                camPos.x <= event->p2.x &&
                                camPos.y >= event->p1.y &&
                                camPos.y <= event->p2.y);
    short wasInsideTrigger = (prevPos.x >= event->p1.x &&
                                prevPos.x <= event->p2.x &&
                                prevPos.y >= event->p1.y &&
                                prevPos.y <= event->p2.y);

    if (isFacingRightDirection && isInsideTrigger && (!wasFacingRightDirection && isFacingRightDirection || !wasInsideTrigger && isInsideTrigger))
    {
        SABRE_EnterEventTrigger(event);
    }
    else if ((!isFacingRightDirection || !isInsideTrigger) && (wasFacingRightDirection && wasInsideTrigger))
    {
        SABRE_LeaveEventTrigger(event);
    }
    else if (wasFacingRightDirection && isFacingRightDirection && wasInsideTrigger && isInsideTrigger)
    {
        SABRE_StayInEventTrigger(event);
    }
}

void SABRE_UpdateEvents()
{
    // TODO: Function that updates the events associated with the tile the player is standing in.
    SABRE_UpdateEvent(&event1);
    SABRE_UpdateEvent(&event2);
}

#if DEBUG
void SABRE_ResetEvent(SABRE_EventTrigger *event)
{
    event->state = 0;
}

void SABRE_CycleEventType(SABRE_EventTrigger *event)
{
    event->type++;
    if (event->type > 4) event->type = 0;
}
#endif

void SABRE_EnterEventTrigger(SABRE_EventTrigger *event)
{
    switch (event->type)
    {
        case SABRE_EVENT_PERMANENT_CHANGE:
            if (SABRE_EVENT_NOT_ACTIVE(event))
            {
                SABRE_EVENT_ACTIVATE(event);
            }
            break;
        case SABRE_EVENT_PRESENCE_TOGGLE:
            SABRE_EVENT_ACTIVATE(event);
            break;
        case SABRE_EVENT_PRESENCE_TOGGLE_ONCE:
            if (SABRE_EVENT_NOT_LOCKED(event))
            {
                SABRE_EVENT_ACTIVATE(event);
                SABRE_EVENT_LOCK(event);
            }
            break;
        case SABRE_EVENT_SIGNAL:
            if (SABRE_EVENT_NOT_LOCKED(event))
            {
                SABRE_EVENT_ACTIVATE(event);
                SABRE_EVENT_LOCK(event);
            }
            else
            {
                SABRE_EVENT_DEACTIVATE(event);
            }
            break;
        case SABRE_EVENT_SIGNAL_ONCE:
            if (SABRE_EVENT_NOT_LOCKED(event))
            {
                SABRE_EVENT_ACTIVATE(event);
                SABRE_EVENT_LOCK(event);
            }
            else
            {
                SABRE_EVENT_DEACTIVATE(event);
            }
            break;
    }
}

void SABRE_StayInEventTrigger(SABRE_EventTrigger *event)
{
    switch (event->type)
    {
        case SABRE_EVENT_SIGNAL:
        case SABRE_EVENT_SIGNAL_ONCE:
            SABRE_EnterEventTrigger(event);
            break;
        default:
            break;
    }
}

void SABRE_LeaveEventTrigger(SABRE_EventTrigger *event)
{
    switch (event->type)
    {
        case SABRE_EVENT_PRESENCE_TOGGLE:
            SABRE_EVENT_DEACTIVATE(event);
            break;
        case SABRE_EVENT_PRESENCE_TOGGLE_ONCE:
            SABRE_EVENT_DEACTIVATE(event);
            break;
        case SABRE_EVENT_SIGNAL:
            if (SABRE_EVENT_LOCKED(event))
            {
                SABRE_EVENT_UNLOCK(event);
            }
            break;
        case SABRE_EVENT_SIGNAL_ONCE:
            if (SABRE_EVENT_LOCKED(event))
            {
                SABRE_EVENT_DEACTIVATE(event);
            }
            break;
    }
}


// ..\source\global-code\38-level.c
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
    char validated;

    unsigned width;
    unsigned height;
    SABRE_LevelTile *map;
}SABRE_Level;
#define SABRE_LEVEL_DEFINED
#endif

// from texture.c
int SABRE_ValidateTextureIndex(int index);

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
    unsigned i, j;
    unsigned index = 0;
    unsigned width = level->width;
    unsigned height = level->height;
    unsigned validatedIndex = 0;

    if (level->validated)
        return 0; // 0: level already validated

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            index = level->map[i * width + j].texture;

            if (index == 0) // current tile is empty
                continue;

            validatedIndex = SABRE_ValidateTextureIndex(index);
            level->map[i * width + j].texture = validatedIndex + 1; // offset by one because the first texture index
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

int SABRE_SetLevelDataFrom2DIntArray(SABRE_Level *level, unsigned width, unsigned height, int **arr)
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
                level->map[i * width + j].texture = arr[i][j];
                level->map[i * width + j].renderObject = NULL;
            }
        }

        return 0;
    }

    return 3; // 3: level allocation failed
}


// ..\source\global-code\40-texture.c
#define SABRE_TEXTURE_ACTOR "SABRE_TextureActor"

typedef struct SABRE_TextureStruct
{
    int width;
    int height;
    short slices;
    char isWindow;
    char name[256];
}SABRE_Texture;

#define SABRE_DATA_STORE_AS_TEXTURE_ARRAY(DATA_STORE) SABRE_DATA_STORE_AS_CAST_ARRAY(DATA_STORE, (SABRE_Texture *))
#define SABRE_TEXTURE_POINTER_CAST(POINTER) ((SABRE_Texture *)POINTER)

// Actual texture data store
SABRE_DataStore SABRE_textureStore;
// A shortcut pointer to access the data from the store
// without having to cast pointers all the time
SABRE_Texture *SABRE_textures = NULL;

int SABRE_ValidateTextureIndex(int index);

int SABRE_AutoAddTextures();
int SABRE_AddTexture(const char textureName[256]);
int SABRE_CalculateTextureWidth(SABRE_Texture *texture);
int SABRE_CalculateTextureHeight(SABRE_Texture *texture);

void SABRE_AddTextureToDataStore(SABRE_DataStore *dataStore, void *texture);
void SABRE_FreeTextureStore();

int SABRE_ValidateTextureIndex(int index)
{
    if (index > 0 && index < SABRE_textureStore.count)
        return index; // offset by one because the first texture index
                      // is reserved for the "texture missing" texture

    return 0;
}

// only works for non-animated textures
int SABRE_AutoAddTextures()
{
    int i = 1; // Start from 1, don't add project management label as a texture
    int err = 0;
    char animName[256];

    strcpy(animName, getAnimName(i));
    SABRE_SetDataStoreAddFunc(&SABRE_textureStore, SABRE_AddTextureToDataStore);
    SABRE_textureStore.elemSize = sizeof(SABRE_Texture);
    SABRE_PrepareDataStore(&SABRE_textureStore);

    // Set the shortcut pointer to allow easier access to texture data
    SABRE_textures = SABRE_DATA_STORE_AS_TEXTURE_ARRAY(SABRE_textureStore);

    while (strcmp(animName, "") != 0)
    {
        err = SABRE_AddTexture(animName);

        if (err) return err;

#if DEBUG
{
    char temp[256];
    sprintf(temp, "Added texture: [%d \"%s\"]", i - 2, animName);
    DEBUG_MSG_FROM(temp, "SABRE_AutoAddTextures");
}
#endif

        strcpy(animName, getAnimName(++i));
    }

    return 0;
}

int SABRE_AddTexture(const char textureName[256])
{
    SABRE_Texture newTexture;

    strcpy(newTexture.name, textureName);
    newTexture.width = SABRE_CalculateTextureWidth(&newTexture);
    newTexture.height = SABRE_CalculateTextureHeight(&newTexture);
    newTexture.isWindow = SABRE_StringEndsWith(newTexture.name, "-window");

    return SABRE_AddToDataStore(&SABRE_textureStore, &newTexture);
}

int SABRE_CalculateTextureWidth(SABRE_Texture *texture)
{
    // TODO: make a check for if the animation actually exists, use getAnimIndex(), if -1, doesn't exist
    ChangeAnimation(SABRE_TEXTURE_ACTOR, texture->name, STOPPED);
    return getclone(SABRE_TEXTURE_ACTOR)->nframes;
}

int SABRE_CalculateTextureHeight(SABRE_Texture *texture)
{
    return SABRE_GetAnimationDataValue(SABRE_TEXTURE_ACTOR, texture->name, SABRE_ANIM_HEIGHT);
}

void SABRE_AddTextureToDataStore(SABRE_DataStore *dataStore, void *texture)
{
    SABRE_textures[dataStore->count] = *SABRE_TEXTURE_POINTER_CAST(texture);
}

void SABRE_FreeTextureStore()
{
    SABRE_FreeDataStore(&SABRE_textureStore);
    SABRE_textures = NULL;
}


// ..\source\global-code\50-sprite.c
#define SABRE_SPRITE_ACTOR "SABRE_SpriteActor"

typedef struct SABRE_SpriteStruct
{
    unsigned int width;
    unsigned int height;
    unsigned int halfWidth;
    unsigned int halfHeight;
    unsigned int nframes;
    char name[256];
}SABRE_Sprite;

#define SABRE_DATA_STORE_AS_SPRITE_ARRAY(DATA_STORE) SABRE_DATA_STORE_AS_CAST_ARRAY(DATA_STORE, (SABRE_Sprite *))
#define SABRE_SPRITE_POINTER_CAST(POINTER) ((SABRE_Sprite *)POINTER)

// Actual sprite data store
SABRE_DataStore SABRE_spriteStore;
// A shortcut pointer to access the data from the store
// without having to cast pointers all the time
SABRE_Sprite *SABRE_sprites = NULL;

int SABRE_AutoAddSprites();
int SABRE_AddSprite(const char spriteName[256]);

void SABRE_AddSpriteToDataStore(SABRE_DataStore *dataStore, void *sprite);
void SABRE_FreeSpriteStore();

int SABRE_AutoAddSprites()
{
    int i = 1; // Start from 1, don't add project management label as a texture
    int err = 0;
    char animName[256];

    strcpy(animName, getAnimName(i));
    SABRE_SetDataStoreAddFunc(&SABRE_spriteStore, SABRE_AddSpriteToDataStore);
    SABRE_spriteStore.elemSize = sizeof(SABRE_Sprite);
    SABRE_PrepareDataStore(&SABRE_spriteStore);

    // Set the shortcut pointer to allow easier access to sprite data
    SABRE_sprites = SABRE_DATA_STORE_AS_SPRITE_ARRAY(SABRE_spriteStore);

    while (strcmp(animName, "") != 0)
    {
        err = SABRE_AddSprite(animName);

        if (err) return err;

#if DEBUG
{
    char temp[256];
    sprintf(temp, "Added sprite: [%d \"%s\" %d]", i - 1, animName, SABRE_sprites[SABRE_spriteStore.count-1].nframes);
    DEBUG_MSG_FROM(temp, "SABRE_AutoAddSprites");
}
#endif

        strcpy(animName, getAnimName(++i));
    }

    return 0;
}

int SABRE_AddSprite(const char spriteName[256])
{
    SABRE_Sprite newSprite;

    strcpy(newSprite.name, spriteName);
    newSprite.width = SABRE_GetAnimationDataValue(SABRE_SPRITE_ACTOR, spriteName, SABRE_ANIM_WIDTH);
    newSprite.height = SABRE_GetAnimationDataValue(SABRE_SPRITE_ACTOR, spriteName, SABRE_ANIM_HEIGHT);
    newSprite.halfWidth = newSprite.width * 0.5f;
    newSprite.halfHeight = newSprite.height * 0.5f;
    newSprite.nframes = SABRE_GetAnimationDataValue(SABRE_SPRITE_ACTOR, spriteName, SABRE_ANIM_NFRAMES);

    return SABRE_AddToDataStore(&SABRE_spriteStore, &newSprite);
}

void SABRE_AddSpriteToDataStore(SABRE_DataStore *dataStore, void *sprite)
{
    SABRE_sprites[dataStore->count] = *SABRE_SPRITE_POINTER_CAST(sprite);
}

void SABRE_FreeSpriteStore()
{
    SABRE_FreeDataStore(&SABRE_spriteStore);
    SABRE_sprites = NULL;
}


// ..\source\global-code\52-animation.c
#ifndef SABRE_ANIMATION_DEFINED
typedef struct SABRE_AnimationStruct
{
    float frameRate;
    unsigned int nframes;
    unsigned int sprite;
}SABRE_Animation;
#define SABRE_ANIMATION_DEFINED
#endif

#ifndef SABRE_ANIMATOR_DEFINED
typedef struct SABRE_AnimatorStruct
{
    char state;
    unsigned int animpos;
    float accumulatedAnimpos;
    SABRE_Animation anim;
}SABRE_Animator;
#define SABRE_ANIMATOR_DEFINED
#endif

#define SABRE_ANIMATOR_LITERAL(FPS, NFRAMES, SPRITE) { 0, 0, 0.0f, { FPS, NFRAMES, SPRITE } }

SABRE_Animation SABRE_CreateAnimation(float frameRate, unsigned int sprite)
{
    SABRE_Animation anim;

    anim.frameRate = frameRate;
    anim.nframes = SABRE_sprites != NULL ? SABRE_sprites[sprite].nframes : 1;
    anim.sprite = sprite;

    return anim;
}

void SABRE_ChangeAnimation(SABRE_Animator *animator, SABRE_Animation anim, char state)
{
    animator->state = state;
    animator->animpos = 0;
    animator->accumulatedAnimpos = 0.0f;
    animator->anim = anim;
}

void SABRE_ChangeAnimationDirection(SABRE_Animator *animator, char state)
{
    animator->state = state;
}

SABRE_Animator SABRE_CreateAnimator(SABRE_Animation anim)
{
    SABRE_Animator animator;

    animator.state = FORWARD;
    animator.animpos = 0;
    animator.accumulatedAnimpos = 0.0f;
    animator.anim = anim;

    return animator;
}

void SABRE_UpdateAnimation(SABRE_Animator *animator)
{
    if (!animator)
    {
        DEBUG_MSG_FROM("Invalid animator pointer", "SABRE_UpdateAnimation");
        return;
    }

    if (animator->state == STOPPED)
        return;

    animator->accumulatedAnimpos += animator->anim.frameRate / max(real_fps, 1);

    if (animator->accumulatedAnimpos >= 1.0f)
    {
        animator->animpos = (animator->animpos + (int)animator->accumulatedAnimpos) % (int)max(animator->anim.nframes, 1);
        animator->accumulatedAnimpos = max(0, animator->accumulatedAnimpos - (int)animator->accumulatedAnimpos);
    }
}


// ..\source\global-code\55-entity.c
#ifndef SABRE_ANIMATION_DEFINED
typedef struct SABRE_AnimationStruct
{
    float frameRate;
    unsigned int nframes;
    unsigned int sprite;
}SABRE_Animation;
#define SABRE_ANIMATION_DEFINED
#endif

#ifndef SABRE_ANIMATOR_DEFINED
typedef struct SABRE_AnimatorStruct
{
    char state;
    unsigned int animpos;
    float accumulatedAnimpos;
    SABRE_Animation anim;
}SABRE_Animator;
#define SABRE_ANIMATOR_DEFINED
#endif

enum SABRE_EntityAttributeFlags
{
    SABRE_ENTITY_HIDDEN         = 1,
    SABRE_ENTITY_NO_COLLISION   = 2
};

#ifndef SABRE_ENTITY_DEFINED
typedef struct SABRE_EntityStruct
{
    float radius;
    SABRE_Vector3 pos;
    unsigned char attributes;
    SABRE_Animator animator;
    char name[256];
}SABRE_Entity;
#define SABRE_ENTITY_DEFINED
#endif

SABRE_List *SABRE_entities = NULL;

SABRE_Entity *SABRE_AddEntity(float radius, SABRE_Vector3 pos, SABRE_Animator animator, unsigned char attributes, const char name[256]);
SABRE_Entity *SABRE_GetEntity(const char name[256]);
int SABRE_CountEntitiesInList();
void SABRE_FreeEntityList();

SABRE_Entity *movableFlowerPot = NULL;
SABRE_Entity *hiddenFlowerPot = NULL;
SABRE_Entity *inCorner = NULL;

#define SABRE_ENTITY_COUNT 12

void SABRE_SetEntities()
{
    int i;
    SABRE_ListTypes data;

    SABRE_Entity tempEntities[SABRE_ENTITY_COUNT] =
    {
        { 0.15f, { 8.5f, 1.5f }, 0, SABRE_ANIMATOR_LITERAL(0, 1, 1), "barrel.0" },
        { 0.15f, { 7.5f, 1.5f }, 0, SABRE_ANIMATOR_LITERAL(0, 1, 1), "barrel.1" },
        { 0.15f, { 6.5f, 1.5f }, 0, SABRE_ANIMATOR_LITERAL(0, 1, 2), "terra-cotta-planter.0" },
        { 0.15f, { 8.5f, 2.5f }, 0, SABRE_ANIMATOR_LITERAL(0, 1, 1), "barrel.2" },
        { 0.14f, { 2.5f, 2.5f }, 0, SABRE_ANIMATOR_LITERAL(0, 1, 0), "pillar.0" },
        { 0.15f, { 6.5f, 2.5f }, 0, SABRE_ANIMATOR_LITERAL(0, 1, 1), "barrel.3" },
        { 0.19f, { 6.5f, 3.5f }, 0, SABRE_ANIMATOR_LITERAL(0, 1, 3), "flower-pot.0" },
        { 0.19f, { 7.0f, 3.5f }, 0, SABRE_ANIMATOR_LITERAL(0, 1, 3), "flower-pot.1" },
        { 0.19f, { 7.5f, 3.5f }, 0, SABRE_ANIMATOR_LITERAL(0, 1, 3), "flower-pot.2" },
        { 0.19f, { 8.0f, 3.5f }, 0, SABRE_ANIMATOR_LITERAL(0, 1, 3), "flower-pot.3" },
        { 0.19f, { 8.5f, 3.5f }, 0, SABRE_ANIMATOR_LITERAL(0, 1, 3), "flower-pot.4" },
        // { 0.15f, { 2.5f, 8.5f }, 0, SABRE_ANIMATOR_LITERAL(0, 1, 5), "tree.4" }
        { 0.15f, { 2.5f, 8.5f }, 0, SABRE_ANIMATOR_LITERAL(10, 1, 7), "dude.0" }
    };

    for (i = 0; i < SABRE_ENTITY_COUNT; i++)
    {
        data.entity = tempEntities[i];
        data.entity.animator = SABRE_CreateAnimator(SABRE_CreateAnimation(tempEntities[i].animator.anim.frameRate, tempEntities[i].animator.anim.sprite));

        if (SABRE_AddToList(&SABRE_entities, data))
            DEBUG_MSG_FROM("Added new entity", "SABRE_SetEntities");
    }
}

SABRE_Entity *SABRE_AddEntity(float radius, SABRE_Vector3 pos, SABRE_Animator animator, unsigned char attributes, const char name[256])
{
    SABRE_Entity new;
    SABRE_ListTypes newListElement;

    new.radius = radius;
    new.pos = pos;
    new.attributes = attributes;
    new.animator = animator;

    if (strlen(name) > 0)
        strcpy(new.name, name);
    else
        sprintf(new.name, "entity.%d", SABRE_CountEntitiesInList());

#if DEBUG
// {
    // char temp[300];
    // sprintf(temp, "Added entity: %s", name);
    // DEBUG_MSG_FROM(temp, "SABRE_AddEntity");
// }
#endif

    newListElement.entity = new;
    return &(SABRE_AddToList(&SABRE_entities, newListElement)->data.entity);
}

SABRE_Entity *SABRE_GetEntity(const char name[256])
{
    SABRE_List *iterator = NULL;

    for (iterator = SABRE_entities; iterator != NULL; iterator = iterator->next)
    {
        if (strcmp(name, iterator->data.entity.name) == 0)
        {
            return &(iterator->data.entity);
        }
    }

    return NULL;
}

SABRE_List *SABRE_GetEntityListObject(SABRE_Entity *entity)
{
    SABRE_List *iterator = NULL;

    for (iterator = SABRE_entities; iterator != NULL; iterator = iterator->next)
    {
        if (entity == &iterator->data.entity)
        {
            return iterator;
        }
    }

    return NULL;
}

int SABRE_CountEntitiesInList()
{
    int count = 0;
    SABRE_List *iterator = NULL;

    for (iterator = SABRE_entities; iterator != NULL; iterator = iterator->next)
    {
        count++;
    }

    return count;
}

void SABRE_FreeEntityList()
{
    SABRE_FreeList(SABRE_entities);
    SABRE_entities = NULL;
}


// ..\source\global-code\57-projectile.c
#define SABRE_PROJECTILE_HANDLER_ACTOR "SABRE_ProjectileHandler"

#define SABRE_PROJECTILE_HIT_WALL 1
#define SABRE_PROJECTILE_HIT_ENTITY 2
#define SABRE_PROJECTILE_HIT_FLOOR 3

#ifndef SABRE_PROJECTILE_DEFINED
typedef struct SABRE_ProjectileStruct
{
    float speed;
    float dropFactor;
    SABRE_Vector3 dir;
    SABRE_Entity *entity;
}SABRE_Projectile;
#define SABRE_PROJECTILE_DEFINED
#endif

// Not really anything like infinity, merely a very high value,
// but the word infinity is shorter than "very high value" :D
#define SABRE_INFINITY 1e30

SABRE_List *SABRE_projectiles = NULL;

typedef struct SABRE_ProjectileHitDataStruct
{
    float dist;
    unsigned char hitType;
    SABRE_Projectile *projectile;
    SABRE_Vector3 hitPosition;
    SABRE_Entity *entityHit;
}SABRE_ProjectileHitData;

SABRE_ProjectileHitData SABRE_projectileHitData;

SABRE_ProjectileHitData SABRE_CreateProjectileHit(unsigned char hitType, float dist, SABRE_Projectile *projectile, SABRE_Vector3 hitPosition, SABRE_Entity *entityHit)
{
    SABRE_ProjectileHitData new;

    new.dist = dist;
    new.hitType = hitType;
    new.projectile = projectile;
    new.hitPosition = hitPosition;
    new.entityHit = entityHit;

    return new;
}

void SABRE_SendProjectileHitEvent(SABRE_ProjectileHitData hitData)
{
    SABRE_projectileHitData = hitData;
    SendActivationEvent(SABRE_PROJECTILE_HANDLER_ACTOR);
}

void SABRE_FireProjectile(SABRE_Vector3 dir, float speed, float dropFactor, float radius, SABRE_Vector3 pos, SABRE_Animator animator)
{
    char temp[256];
    SABRE_Projectile new;
    SABRE_ListTypes newListElement;

    new.dir = SABRE_ScaleVector3(SABRE_NormalizeVector3(dir), speed);
    new.speed = speed;
    new.dropFactor = dropFactor;
    sprintf(temp, "projectile.%d", SABRE_CountEntitiesInList());
    new.entity = SABRE_AddEntity(radius, pos, animator, SABRE_ENTITY_NO_COLLISION, temp);

    newListElement.projectile = new;
    SABRE_AddToList(&SABRE_projectiles, newListElement);
}



SABRE_List *SABRE_GetProjectileListObject(SABRE_Projectile *projectile)
{
    SABRE_List *iterator = NULL;

    for (iterator = SABRE_projectiles; iterator != NULL; iterator = iterator->next)
    {
        if (projectile == &iterator->data.projectile)
        {
            return iterator;
        }
    }

    return NULL;
}

SABRE_Vector2 SABRE_Raycast(SABRE_Vector2 rayStartPos, SABRE_Vector2 rayDirection)
{
    SABRE_Vector2 wallHitPosition;

    float rayPosX = rayStartPos.x;
    float rayPosY = rayStartPos.y;
    float rayDirX = rayDirection.x;
    float rayDirY = rayDirection.y;
    float deltaX = (rayDirX == 0) ? SABRE_INFINITY : abs(1 / rayDirX);
    float deltaY = (rayDirY == 0) ? SABRE_INFINITY : abs(1 / rayDirY);
    float distX;
    float distY;

    int rayMapY = (int)rayPosY;
    int rayMapX = (int)rayPosX;
    int stepX;
    int stepY;
    int wallHit = 0;
    int hitSide = 0;

    stepX = 1 - (rayDirX < 0) * 2;
    stepY = 1 - (rayDirY < 0) * 2;

    if (rayDirX < 0) distX = (rayPosX - rayMapX) * deltaX;
    else             distX = (rayMapX + 1 - rayPosX) * deltaX;

    if (rayDirY < 0) distY = (rayPosY - rayMapY) * deltaY;
    else             distY = (rayMapY + 1 - rayPosY) * deltaY;

    while (!wallHit)
    {
        if (distX < distY)
        {
            distX += deltaX;
            rayMapX += stepX;
            hitSide = 0;
        }
        else
        {
            distY += deltaY;
            rayMapY += stepY;
            hitSide = 1;
        }

        if (!wallHit)
            wallHit = (SABRE_level.map[rayMapY * SABRE_level.width + rayMapX].texture > 0);
    }

    if (hitSide)
        return SABRE_CreateVector2(rayPosX + ((rayMapY - rayPosY + (1 - stepY) / 2.0f) / rayDirY) * rayDirX, rayMapY + (stepY < 0));

    return SABRE_CreateVector2(rayMapX + (stepX < 0), rayPosY + ((rayMapX - rayPosX + (1 - stepX) / 2.0f) / rayDirX) * rayDirY);
}

SABRE_ProjectileHitData SABRE_CheckProjectileWallCollision(SABRE_Projectile *projectile)
{
    SABRE_Vector2 prevPos = SABRE_Vector3ToVector2WithoutZ(projectile->entity->pos);
    SABRE_Vector2 nextPos = SABRE_AddVector2(prevPos, SABRE_Vector3ToVector2WithoutZ(projectile->dir));
    SABRE_Vector2 wallHitPosition = SABRE_Raycast(prevPos, SABRE_Vector3ToVector2WithoutZ(projectile->dir));
    SABRE_ProjectileHitData hitData = { SABRE_INFINITY };

    float distToWall = distance(prevPos.x, prevPos.y, wallHitPosition.x, wallHitPosition.y);
    float distToNextPos = distance(prevPos.x, prevPos.y, nextPos.x, nextPos.y);

    if (distToWall < distToNextPos)
        hitData = SABRE_CreateProjectileHit(SABRE_PROJECTILE_HIT_WALL, distToWall, projectile, SABRE_Vector2ToVector3(wallHitPosition, projectile->entity->pos.z), NULL);

    return hitData;
}

SABRE_ProjectileHitData SABRE_CheckProjectileEntityCollision(SABRE_Projectile *projectile, SABRE_Entity *entity)
{
    float dist;
    float keepDist;
    float goBackDist;
    float newDist;
    float hitAngle;

    SABRE_Vector2 collisionPos;
    SABRE_Vector2 projection = SABRE_VECTOR2_ZERO;

    SABRE_Vector2 entityPos = SABRE_Vector3ToVector2WithoutZ(entity->pos);

    SABRE_Vector2 prevPos = SABRE_Vector3ToVector2WithoutZ(projectile->entity->pos);
    SABRE_Vector2 nextPos = SABRE_AddVector2(prevPos, SABRE_Vector3ToVector2WithoutZ(projectile->dir));
    SABRE_Vector2 prevToNext = SABRE_SubstractVector2(nextPos, prevPos);
    SABRE_Vector2 prevToColl = prevToColl = SABRE_SubstractVector2(entityPos, prevPos);

    SABRE_ProjectileHitData hitData = { SABRE_INFINITY };

    if (entity->attributes & SABRE_ENTITY_NO_COLLISION)
        return hitData;

    // No need to process entities that are behind the projectile
    if (SABRE_DotProductVector2(prevToColl, prevToNext) < 0)
        return hitData;

    dist = SABRE_PointToLineSegmentDistance(prevPos, nextPos, entityPos, &projection);
    keepDist = projectile->entity->radius + entity->radius;
    goBackDist = sqrt(pow(keepDist, 2) - pow(dist, 2));
    newDist = distance(prevPos.x, prevPos.y, projection.x, projection.y) - goBackDist;

    if (SABRE_LineToPointCollision(prevPos, nextPos, entityPos, projectile->entity->radius, entity->radius))
    {
        collisionPos = SABRE_AddVector2(prevPos, SABRE_ScaleVector2(SABRE_NormalizeVector2(SABRE_Vector3ToVector2WithoutZ(projectile->dir)), newDist));
        hitAngle = degtorad(direction(entityPos.x, entityPos.y, collisionPos.x, collisionPos.y));
        collisionPos.x = entityPos.x + cos(hitAngle) * keepDist;
        collisionPos.y = entityPos.y - sin(hitAngle) * keepDist;

        hitData = SABRE_CreateProjectileHit(SABRE_PROJECTILE_HIT_ENTITY, distance(prevPos.x, prevPos.y, collisionPos.x, collisionPos.y), projectile, SABRE_Vector2ToVector3(collisionPos, projectile->entity->pos.z), entity);
    }

    return hitData;
}

SABRE_ProjectileHitData SABRE_CheckProjectileEntityCollisions(SABRE_Projectile *projectile)
{
    SABRE_List *iterator = NULL;
    SABRE_List *next = NULL;

    float lowestDistance = SABRE_INFINITY;
    SABRE_ProjectileHitData hitData = { SABRE_INFINITY };
    SABRE_ProjectileHitData tempHitData = { SABRE_INFINITY };

    for (iterator = SABRE_entities; iterator != NULL; iterator = next)
    {
        next = iterator->next;

        tempHitData = SABRE_CheckProjectileEntityCollision(projectile, &iterator->data.entity);

        if (tempHitData.dist < lowestDistance)
        {
            hitData = tempHitData;
            lowestDistance = hitData.dist;
        }
    }

    return hitData;
}

void SABRE_StopProjectileAtPosition(SABRE_Projectile *projectile, SABRE_Vector3 position)
{
    projectile->entity->pos = position;
    projectile->dir = SABRE_VECTOR3_ZERO;
}

void SABRE_UpdateProjectiles()
{
    SABRE_List *next = NULL;
    SABRE_List *iterator = NULL;

    SABRE_ProjectileHitData wallHitData;
    SABRE_ProjectileHitData entityHitData;

    for (iterator = SABRE_projectiles; iterator != NULL; iterator = next)
    {
        next = iterator->next;

        if (iterator->data.projectile.dir.x == 0 && iterator->data.projectile.dir.y == 0 && iterator->data.projectile.dir.z == 0)
            continue;

        wallHitData = SABRE_CheckProjectileWallCollision(&iterator->data.projectile);
        entityHitData = SABRE_CheckProjectileEntityCollisions(&iterator->data.projectile);

        if (entityHitData.dist < wallHitData.dist)
        {
            iterator->data.projectile.entity->pos = entityHitData.hitPosition;
            SABRE_StopProjectileAtPosition(&iterator->data.projectile, entityHitData.hitPosition);
            SABRE_SendProjectileHitEvent(entityHitData);
        }
        else if (wallHitData.dist < entityHitData.dist)
        {
            SABRE_StopProjectileAtPosition(&iterator->data.projectile, wallHitData.hitPosition);
            SABRE_SendProjectileHitEvent(wallHitData);
        }
        else
        {
            if (iterator->data.projectile.entity->pos.z - iterator->data.projectile.dropFactor <= 0)
            {
                iterator->data.projectile.entity->pos.z = 0;
                iterator->data.projectile.dir = SABRE_VECTOR3_ZERO;
                SABRE_SendProjectileHitEvent(SABRE_CreateProjectileHit(SABRE_PROJECTILE_HIT_FLOOR, 0, &(iterator->data.projectile), iterator->data.projectile.entity->pos, NULL));
            }
            else
            {
                SABRE_AddVector3InPlace(&iterator->data.projectile.entity->pos, iterator->data.projectile.dir);
                iterator->data.projectile.dir.z -= iterator->data.projectile.dropFactor;
            }
        }
    }
}

void SABRE_FreeProjectileList()
{
    SABRE_FreeList(SABRE_projectiles);
    SABRE_projectiles = NULL;
}


// ..\source\global-code\60-renderer.c
#ifndef SABRE_RENDER_OBJECT_DEFINED
enum SABRE_RenderObjectTypeEnum
{
    SABRE_TEXTURE_RO,
    SABRE_SPRITE_RO
};

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

struct SABRE_RenderObjectListManagerStruct
{
    SABRE_RenderObject *head;
    SABRE_RenderObject *curr;
    SABRE_RenderObject *reusableCache;
}SABRE_ROListManager;

#if DEBUG
int allocations = 0;
int traversals = 0;
int singleSliceTraversals = 0;
int maxTraversals = 0;
size_t allocatedMemory = 0;
#endif

void SABRE_FreeRenderObjectList()
{
    SABRE_RenderObject *iterator = NULL;
    SABRE_RenderObject *next = NULL;

    for (iterator = SABRE_ROListManager.head; iterator != NULL; iterator = next)
    {
        next = iterator->next;
        free(iterator);

#if DEBUG
        allocations--;
        allocatedMemory -= sizeof *iterator;
#endif
    }

    SABRE_ROListManager.head = NULL;
    SABRE_ROListManager.curr = NULL;
    SABRE_ROListManager.reusableCache = NULL;
}

SABRE_RenderObject *SABRE_GetLastROInList(SABRE_RenderObject *list)
{
    SABRE_RenderObject *iterator = NULL;

    for (iterator = list; iterator != NULL; iterator = iterator->next)
    {
        if (!iterator->next)
        {
            return iterator;
        }
    }

    return NULL;
}

SABRE_RenderObject *SABRE_ConcatenateROList(SABRE_RenderObject *dest, SABRE_RenderObject *src)
{
    SABRE_RenderObject *tail = SABRE_GetLastROInList(dest);
    SABRE_RenderObject *result = NULL;

    if (tail)
    {
        tail->next = src;
        result = dest;
    }
    else
    {
        result = src;
    }

    return result;
}

void SABRE_InitializeFrame()
{
    int i, j;

    for (j = 0; j < SABRE_level.height; j++)
    {
        for (i = 0; i < SABRE_level.width; i++)
        {
            SABRE_level.map[j * SABRE_level.width + i].renderObject = NULL;
        }
    }

#if DEBUG
    traversals = 0;
    maxTraversals = 0;
#endif

    SABRE_ROListManager.reusableCache = SABRE_ConcatenateROList(SABRE_ROListManager.reusableCache, SABRE_ROListManager.head);
    SABRE_ROListManager.head = NULL;
    SABRE_ROListManager.curr = NULL;
}

int SABRE_InsertRO(SABRE_RenderObject *object)
{
    SABRE_RenderObject *iterator = NULL;
    SABRE_RenderObject *prev = NULL;
    SABRE_RenderObject *next = NULL;

    if (!object)
    {
        return 1;
    }

    if (!SABRE_ROListManager.head || !SABRE_ROListManager.curr)
    {
        SABRE_ROListManager.head = object;
        SABRE_ROListManager.curr = object;
        return 0;
    }

    iterator = SABRE_ROListManager.curr;

#if DEBUG
    singleSliceTraversals = 0;
#endif

    if (object->sortValue <= iterator->sortValue)
    {
        while (iterator && object->sortValue <= iterator->sortValue)
        {
            prev = iterator;
            iterator = iterator->next;
#if DEBUG
            traversals++;
            singleSliceTraversals++;
#endif
        }

        if (iterator)
        {
            object->prev = iterator->prev;
            object->next = iterator;
            if (iterator->prev)
            {
                iterator->prev->next = object;
            }
            iterator->prev = object;

        }
        else
        {
            object->prev = prev;
            object->next = NULL;
            prev->next = object;
        }
        SABRE_ROListManager.curr = object;
    }
    else
    {
        while (iterator && object->sortValue > iterator->sortValue)
        {
            next = iterator;
            iterator = iterator->prev;
#if DEBUG
            traversals++;
            singleSliceTraversals++;
#endif
        }

        if (iterator)
        {
            object->prev = iterator;
            object->next = iterator->next;
            if (iterator->next)
            {
                iterator->next->prev = object;
            }
            iterator->next = object;
        }
        else
        {
            object->prev = NULL;
            object->next = next;
            next->prev = object;
            SABRE_ROListManager.head = object;
        }
        SABRE_ROListManager.curr = object;
    }

#if DEBUG
    if (singleSliceTraversals > maxTraversals) maxTraversals = singleSliceTraversals;
#endif

    return 0;
}

SABRE_RenderObject *SABRE_GetNextUnusedRO()
{
    if (SABRE_ROListManager.reusableCache)
    {
        SABRE_RenderObject *new = SABRE_ROListManager.reusableCache;
        SABRE_ROListManager.reusableCache = SABRE_ROListManager.reusableCache->next;
        return new;
    }
    else
    {
        SABRE_RenderObject *new = malloc(sizeof *new);

        if (!new)
        {
            DEBUG_MSG_FROM("Memory allocation failed!", "SABRE_GetNextUnusedRO");
            return NULL;
        }

#if DEBUG
    allocations++;
    allocatedMemory += sizeof *new;
#endif

        return new;
    }
}

SABRE_RenderObject *SABRE_AddTextureRO(float sortValue, float scale, int horizontalPosition, int compensation, SABRE_Slice slice)
{
    int err = 0;
    SABRE_RenderObject *new = SABRE_GetNextUnusedRO();

    if (!new)
    {
        return NULL;
    }

    new->sortValue = sortValue;
    new->objectType = SABRE_TEXTURE_RO;
    new->scale = scale;
    new->verticalOffset = 0;
    new->horizontalPosition = horizontalPosition;
    new->horizontalScalingCompensation = compensation;
    new->slice = slice;
    new->prev = NULL;
    new->next = NULL;

    err = SABRE_InsertRO(new);

    return new;
}

SABRE_RenderObject *SABRE_AddSpriteRO(float sortValue, float scale, int horizontalPosition, float verticalOffset, SABRE_Slice slice)
{
    int err = 0;
    SABRE_RenderObject *new = SABRE_GetNextUnusedRO();

    if (!new)
    {
        return NULL;
    }

    new->sortValue = sortValue;
    new->objectType = SABRE_SPRITE_RO;
    new->scale = scale;
    new->verticalOffset = verticalOffset;
    new->horizontalPosition = horizontalPosition;
    new->horizontalScalingCompensation = 0;
    new->slice = slice;
    new->prev = NULL;
    new->next = NULL;

    err = SABRE_InsertRO(new);

    return new;
}

void SABRE_PrintROList()
{
    int counter = 0;
    char temp[256];
    struct SABRE_RenderObjectStruct *iterator = NULL;

    sprintf(temp, "head: %f, curr: %f", SABRE_ROListManager.head->sortValue, SABRE_ROListManager.curr->sortValue);
    DEBUG_MSG(temp);

    for (iterator = SABRE_ROListManager.head; iterator != NULL; iterator = iterator->next)
    {
        sprintf(temp, "frame: %3d render object %3d: [sortValue: %f, scale: %f, hpos: %d, compensation: %d, slice: [anim: %d, slice: %d]]",
            frame, counter++, iterator->sortValue, iterator->scale, iterator->horizontalPosition, iterator->horizontalScalingCompensation,
            iterator->slice.anim, iterator->slice.slice);
        DEBUG_MSG(temp);
    }
}

void SABRE_RenderObjects()
{
    int horizontalPosition = 0;
    float verticalPosition = height * 0.5f;
    SABRE_RenderObject *iterator = NULL;
    float verticalResolutionFactor = SABRE_screenHeight / SABRE_heightUnit;
    const float horizontalCompensationThreshold = 0.0315f; // threshold for growing the compensation

    textureDrawCalls = 0;
    spriteDrawCalls = 0;

    for (iterator = SABRE_ROListManager.head; iterator != NULL; iterator = iterator->next)
    {
        if (iterator->objectType == SABRE_TEXTURE_RO)
        {
            SABRE_slice.anim = iterator->slice.anim;
            SABRE_slice.slice = iterator->slice.slice;
            SendActivationEvent(SABRE_TEXTURE_ACTOR);
            draw_from(SABRE_TEXTURE_ACTOR, iterator->horizontalPosition + iterator->horizontalScalingCompensation, verticalPosition - SABRE_camera.vPos * iterator->scale, iterator->scale);
            textureDrawCalls++;
        }
        else if (iterator->objectType == SABRE_SPRITE_RO)
        {
            SABRE_slice.anim = iterator->slice.anim;
            SABRE_slice.slice = iterator->slice.slice;
            SendActivationEvent(SABRE_SPRITE_ACTOR);
            draw_from(SABRE_SPRITE_ACTOR, iterator->horizontalPosition,
                verticalPosition + ((SABRE_halfHeightUnit - (float)SABRE_sprites[SABRE_slice.anim - 1].halfHeight - SABRE_camera.vPos * 2) - (iterator->verticalOffset * SABRE_heightUnit)) * iterator->scale * verticalResolutionFactor,
                iterator->scale * verticalResolutionFactor);
            spriteDrawCalls++;
        }
    }
}


