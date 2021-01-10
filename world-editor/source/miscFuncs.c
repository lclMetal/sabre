#define HALF_WIDTH (width / 2)
#define HALF_HEIGHT (height / 2)
#define HALF_WIDTH_ACTOR(X) (X->width / 2)
#define HALF_HEIGHT_ACTOR(X) (X->height / 2)

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
double limit(double value, double mi, double ma);
double random(double i, double j);
double tieToFps(double value, double fps);
void shakerSort(int* orderArray, double* inputArray, int arrayCount);

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

double limit(double value, double mi, double ma)
{
    return max(min(mi, ma), min(value, max(ma, mi)));
}

//This function returns a random value between the limiting values i and j
//(The function is originally made by Bee-Ant)
//i - the lower limit of the value
//j - the upper limit of the value
double random(double i, double j)
{
    double ii = rand(max(i ,j) - min (i, j));
    return ii + min (i, j);
}

//This function can be used to tie a variables value to the real_fps so that the value stays
//approximately the same for every full second. This is meant to be used for variables that are
//used in some sort of movement or other cases where a value is being constantly modified
//by some other value, this function makes the effect of the constant modification to remain
//almost the same even if the real_fps of the game changes
//value - the value that would be used normally once in every frame with the framerate of fps
//fps - the framerate the value would be normally used by
double tieToFps(double value, double fps)
{
    return (value * fps) / (double)max(real_fps, 1);
}

//This function sorts the given array so that the smallest values are first and the largest
//ones the last
//order_array - the pointer to the array which is used for storing the indexes of the items
//              in the actual array
//input_array - the actual array which is to be sorted
//array_count - the count of the items in the array
void shakerSort(int* orderArray, double* inputArray, int arrayCount)
{
    int i;
    int swap;
    int orderTemp;
    double temp;

    do
    {
        swap = 0; //no values have been swapped

        for (i = arrayCount - 1; i > 0; i --) //go through the array starting from the end
        {
            if (inputArray[i - 1] > inputArray[i]) //if the value of the smaller index element is
            {                                    //higher than the value of the current element

                temp = inputArray[i - 1];       //swap the values
                inputArray[i - 1] = inputArray[i];
                inputArray[i] = temp;

                orderTemp = orderArray[i - 1];
                orderArray[i - 1] = orderArray[i];
                orderArray[i] = orderTemp;

                swap = 1; //some values have been swapped
            }
        }

        for (i = 1; i < arrayCount; i ++) //go through the array starting from the beginning
        {
            if (inputArray[i - 1] > inputArray[i]) //if the value of the smaller index element is
            {                                    //higher than the value of the current element

                temp = inputArray[i - 1];         //swap the values
                inputArray[i - 1] = inputArray[i];
                inputArray[i] = temp;

                orderTemp = orderArray[i - 1];
                orderArray[i - 1] = orderArray[i];
                orderArray[i] = orderTemp;

                swap = 1; //some values have been swapped
            }
        }
    }
    while(swap); //continue the do loop when swap is equal to 1 (-> some values have been swapped)
}
