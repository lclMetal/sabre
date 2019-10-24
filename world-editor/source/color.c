typedef struct ColorStruct
{
    double hue;
    double saturation;
    double brightness;

    unsigned char r;
    unsigned char g;
    unsigned char b;
}Color;

Color HSBtoRGB();
Color RGBtoHSB();

//This function converts given HSB color system values to corresponding RGB system color values
//and returns a Color with the r, g and b values
//hValue - the hue value of the color to be converted to RGB
//sValue - the saturation value of the color to be converted to RGB
//bValue - the brightness value of the color to be converted to RGB
Color HSBtoRGB(double hValue, double sValue, double bValue)
{
    Color color;

    double chroma = bValue * sValue;
    double m = bValue - chroma;

    double H = ((double)hValue / 60.0);
    double xValue = chroma * (1 - abs(fmod(H, 2) - 1));

    color.hue = hValue;
    color.saturation = sValue;
    color.brightness = bValue;

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

//This function converts given RGB color system values to corresponding HSB system color values
//and returns a Color with the hue, saturation and brightness values
//rValue - the red component of the color to be converted to HSB
//gValue - the green component of the color to be converted to HSB
//bValue - the blue component of the color to be converted to HSB
Color RGBtoHSB(int rValue, int gValue, int bValue)
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
