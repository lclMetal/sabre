#define UPPER_LEFT 0
#define UPPER_RIGHT 1
#define BOTTOM_LEFT 2
#define BOTTOM_RIGHT 3
#define CENTER_TOP 4

void guiItemFloat();
void guiItemFloatActor();
void guiItemFloatRelativeToActor();
void showTooltip();

//This function can be used to place the current actor on the screen in relation to the specified
//corner of the screen
//corner - the corner the actor's position will be relative to
//xOffset - the actor's desired offset from the corner on the x-axis
//yOffset - the actor's desired offset from the corner on the y-axis
void guiItemFloat(int corner, int xOffset, int yOffset)
{
    switch(corner)
    {
        case UPPER_LEFT:
            x = view.x + xOffset;
            y = view.y + yOffset;
        break;
 
        case UPPER_RIGHT:
            x = view.x + view.width - xOffset;
            y = view.y + yOffset;
        break;
 
        case BOTTOM_LEFT:
            x = view.x + xOffset;
            y = view.y + view.height - yOffset;
        break;
 
        case BOTTOM_RIGHT:
            x = view.x + view.width - xOffset;
            y = view.y + view.height - yOffset;
        break;
    }
}

//This function can be used to place a given actor on the screen in relation to the specified
//corner of the screen
//actorName - the name or the clonename of the actor to be positioned in relation to a corner of
//            the screen
//corner - the corner the actor's position will be relative to
//xOffset - the actor's desired offset from the corner on the x-axis
//yOffset - the actor's desired offset from the corner on the y-axis
void guiItemFloatActor(char actorName[256], int corner, int xOffset, int yOffset)
{
    Actor *a = getclone(actorName);
 
    switch(corner)
    {
        case UPPER_LEFT:
            a->x = view.x + xOffset;
            a->y = view.y + yOffset;
        break;
 
        case UPPER_RIGHT:
            a->x = view.x + view.width - xOffset;
            a->y = view.y + yOffset;
        break;
 
        case BOTTOM_LEFT:
            a->x = view.x + xOffset;
            a->y = view.y + view.height - yOffset;
        break;
 
        case BOTTOM_RIGHT:
            a->x = view.x + view.width - xOffset;
            a->y = view.y + view.height - yOffset;
        break;
    }
}

//This function can be used to make the current actor's position relative to a specified
//actors position
//actorName - the name of the another actor the current actor's position should be relative to
//corner - the corner of the another actor the actor's position will be relative to
//xOffset - the actor's desired offset from the corner on the x-axis
//yOffset - the actor's desired offset from the corner on the y-axis
void guiItemFloatRelativeToActor(char actorName[256], int corner, int xOffset, int yOffset)
{
    Actor *a = getclone(actorName);
 
    switch(corner)
    {
        case UPPER_LEFT:
            x = a->x - a->width * 0.5 + xOffset;
            y = a->y - a->height * 0.5 + yOffset;
        break;
 
        case UPPER_RIGHT:
            x = a->x + a->width * 0.5 - xOffset;
            y = a->y - a->height * 0.5 + yOffset;
        break;
 
        case BOTTOM_LEFT:
            x = a->x - a->width + xOffset;
            y = a->y + a->height * 0.5 - yOffset;
        break;
 
        case BOTTOM_RIGHT:
            x = a->x + a->width * 0.5 - xOffset;
            y = a->y + a->height * 0.5 - yOffset;
        break;
 
        case CENTER_TOP:
            x = a->x -xOffset;
            y = a->y - a->height * 0.5 + yOffset;
        break;
    }
}

//This function is used for showing a tooltip in the bottom left corner of the screen
//tooltipText - the text to be shown
void showTooltip(char *tooltipText)
{
    sprintf(getclone("status")->text, "%s", tooltipText);
}
