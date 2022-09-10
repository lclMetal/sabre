double limit(double value, double mi, double ma);
double random(double i, double j);
double tieToFps(double value, double fps);
void shakerSort(int* orderArray, double* inputArray, int arrayCount);

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
