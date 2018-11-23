Actor *gc2();
void disableMouseEvents();
void enableMouseEvents();
double limit();
int mouseOver();
int mouseOverRegion();
int mouseOverPartOfRegion();
double random();
double tieToFps();
void shakerSort();

//This function gets a clone by its name and cloneindex and returns an Actor pointer to it
//actorName - the name of the actor
//actorIndex - the index of the specific clone
Actor *gc2(char actorName[128], int actorIndex)
{
    char temp[256];
 
    sprintf(temp, "%s.%i", actorName, actorIndex);
 
    return getclone(temp);
}

//This function disables all mouse events of the actor by its name or clonename
//actorName - the name or clonename of the actor whose mouse events are being disabled
void disableMouseEvents(char *actorName)
{
    char tempName[256];
 
    if (!strcmp(actorName, "Event Actor"))sprintf(tempName, "%s", clonename);
    else if (!strcmp(actorName, "Collide Actor"))sprintf(tempName, "%s", collide.clonename);
    else if (!strcmp(actorName, "Parent Actor"))sprintf(tempName, "%s", parent.clonename);
    else if (!strcmp(actorName, "Creator Actor"))sprintf(tempName, "%s", creator.clonename);

    EventDisable(tempName, EVENTMOUSEBUTTONDOWN);
    EventDisable(tempName, EVENTMOUSEBUTTONUP);
    EventDisable(tempName, EVENTMOUSEENTER);
    EventDisable(tempName, EVENTMOUSELEAVE);
}

//This function enables all mouse events of the actor by its name or clonename
//actorName - the name or clonename of the actor whose mouse events are being enabled
void enableMouseEvents(char *actorName)
{
    char tempName[256];
 
    if (!strcmp(actorName, "Event Actor"))sprintf(tempName, "%s", clonename);
    else if (!strcmp(actorName, "Collide Actor"))sprintf(tempName, "%s", collide.clonename);
    else if (!strcmp(actorName, "Parent Actor"))sprintf(tempName, "%s", parent.clonename);
    else if (!strcmp(actorName, "Creator Actor"))sprintf(tempName, "%s", creator.clonename);

    EventEnable(tempName, EVENTMOUSEBUTTONDOWN);
    EventEnable(tempName, EVENTMOUSEBUTTONUP);
    EventEnable(tempName, EVENTMOUSEENTER);
    EventEnable(tempName, EVENTMOUSELEAVE);
}

//This function limits the value given to it to always stay between the given limiter values
//value - the current value of the variable being limited
//mi - the minimum value the variable should have
//ma - the maximum value the variable should have
double limit(double value, double mi, double ma)
{
    return max(min(mi, ma), min(value, max(ma, mi)));
}

//This function returns 1 if the mouse cursor is being held over the rectangle surrounding the actor
int mouseOver()
{
    return (xmouse > xscreen - width*0.5 && xmouse < xscreen + width*0.5 &&
            ymouse > yscreen - height*0.5 && ymouse < yscreen + height*0.5);
}

//This function returns 1 if the mouse cursor is being held over the region actor in case
int mouseOverRegion()
{
    return (xmouse > xscreen && xmouse < xscreen + width && ymouse > yscreen && ymouse < yscreen + height);
}
int mouseOverPartOfRegion(int startX, int startY, int endX, int endY)
{
    return (xmouse > xscreen + startX && xmouse < xscreen + width - endX && ymouse > yscreen + startY && ymouse < yscreen + height - endY);
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
