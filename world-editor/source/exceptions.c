#define WARNING 0
#define ERROR 1

int lowestExceptionIndex = 0;
int highestExceptionIndex = -1;

void showExceptionMessage(int type, char message[256]);
void hideExceptionMessage(int index);

//This function shows an exception message at the bottom left corner of the screen
//Up to ten messages can be shown on screen at the same time
//type - the type of the exeption message, is it a warning or an error
//message - the message to be shown
void showExceptionMessage(int type, char message[256])
{
    int i;
    Actor *a;
    char exceptionTypeNames[2][10] =
    {
        "WARNING! ",
        "ERROR! "
    };

    if (highestExceptionIndex > -1)
    {
        Actor *compareThis;
        char compareToThis[256];

        sprintf(compareToThis, "%s%s", exceptionTypeNames[type], message);

        compareThis = gc2("exceptionTextActor", highestExceptionIndex);

        if (!strcmp(compareToThis, compareThis->text))return;
    }

    a = CreateActor("exceptionTextActor", "icon", "(none)", "(none)", 0, 0, true);
    sprintf(a->text, "%s%s", exceptionTypeNames[type], message);
    guiItemFloatActor(a->clonename, BOTTOM_LEFT, 16, 37);
    //guiItemFloatActor(a->clonename, UPPER_RIGHT, a->width, 40);
    a->var = type;

    if (a->cloneindex > highestExceptionIndex)highestExceptionIndex = a->cloneindex;

    for (i = 0; i < highestExceptionIndex; i ++)
    {
        Actor *moveThis;

        moveThis = gc2("exceptionTextActor", i);

        if (moveThis->cloneindex > -1)moveThis->y -= 12;
        //if (moveThis->cloneindex > -1)moveThis->y += 12;
    }

    if (ActorCount("exceptionTextActor") > 10)
    {
        Actor *nextLowest = NULL;

        DestroyActor(gc2("exceptionTextActor", lowestExceptionIndex)->clonename);

        while (nextLowest == NULL || nextLowest->cloneindex < 0)
        {
            lowestExceptionIndex ++;
            nextLowest = gc2("exceptionTextActor", lowestExceptionIndex);

            if (lowestExceptionIndex > highestExceptionIndex)
            {
                lowestExceptionIndex = 0;
                highestExceptionIndex = -1;
                break;
            }
        }
    }
}

//This function deletes an exception message and rearranges the other exception messages correctly
//index - the index of the exception message to be removed
void hideExceptionMessage(int index)
{
    int i;
    int missingExceptions = 0;
    Actor *current = NULL;

    for (i = highestExceptionIndex; i >= lowestExceptionIndex; i --)
    {
        current = gc2("exceptionTextActor", i);

        if (current->cloneindex < 0)continue;
        else
        {
            if (i == index)
            {
                DestroyActor(current->clonename);
                missingExceptions ++;

                if (index == highestExceptionIndex)
                {
                    Actor *nextHighest = NULL;

                    while (nextHighest == NULL || nextHighest->cloneindex < 0)
                    {
                        highestExceptionIndex --;
                        nextHighest = gc2("exceptionTextActor", highestExceptionIndex);

                        if (highestExceptionIndex < lowestExceptionIndex)
                        {
                            highestExceptionIndex = -1;
                            lowestExceptionIndex = 0;
                            break;
                        }
                    }
                }

                if (index == lowestExceptionIndex)
                {
                    Actor *nextLowest = NULL;

                    while (nextLowest == NULL || nextLowest->cloneindex < 0)
                    {
                        lowestExceptionIndex ++;
                        nextLowest = gc2("exceptionTextActor", lowestExceptionIndex);

                        if (lowestExceptionIndex > highestExceptionIndex)
                        {
                            highestExceptionIndex = -1;
                            lowestExceptionIndex = 0;
                            break;
                        }
                    }
                }
            }
            else
            {
                current->y += 12 * missingExceptions;
            }
        }
    }
}
