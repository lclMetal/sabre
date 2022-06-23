#define DEBUG 1 // 0: disable debug, 1: enable debug

#define FPS_AVERAGE 0
#define FPS_RANGE 1
#define FPS_BOTH 2

// -----DEBUG MACROS-----
//
// Can be used if DEBUG above == 1, otherwise the macros will expand to empty
// space. That way it's not necessary to remove them for shipping the game.
//
// Usage:
//     use DEBUG_INIT(); to initialize the timestamp base time and the output file
//         (optional, if omitted, will be initialized on first debug message)
//     use DEBUG_MSG("Message"); to write a debug message to the "debug.log" file
//     use DEBUG_MSG_FROM("Message", "Origin"); to write a debug message
//         and it's origin location to the "debug.log" file
//         example: DEBUG_MSG_FROM("Message", "view Create Actor");

#if DEBUG == 1
    #define DEBUG_INIT() debugCreateFile()
    #define DEBUG_MSG(X) debugMsg((X))
    #define DEBUG_MSG_FROM(X, Y) debugMsgFrom((X), (Y), __LINE__)
#else
    #define DEBUG_INIT()
    #define DEBUG_MSG(X)
    #define DEBUG_MSG_FROM(X, Y)
#endif

struct debugStruct
{
    int fileInitialized;
    unsigned long startTime;

    stTime fpsTime;
    unsigned long fpsLastUpdate;

    int fpsHeap;
    int fpsCount;
    int fpsFindLowest;
    int fpsFindHighest;

    int fpsLowest;
    int fpsHighest;
    double fpsAverage;
}debugController;

void debugPrintFps(int mode);
void debugUpdateFps(void);
int debugCreateFile(void);
void debugMsg(const char *msg);
void debugMsgFrom(const char *msg, const char *label, int line);

void debugPrintFps(int mode)
{
    debugUpdateFps();

    switch (mode)
    {
        default:
        case FPS_AVERAGE:
            sprintf(text, "FPS %.02f", debugController.fpsAverage);
        break;

        case FPS_RANGE:
            sprintf(text, "Min FPS: %i\nMax FPS: %i",
                debugController.fpsLowest, debugController.fpsHighest);
        break;

        case FPS_BOTH:
            sprintf(text, "FPS: %.02f\n+ min: %i\n+ max: %i",
                debugController.fpsAverage, debugController.fpsLowest, debugController.fpsHighest);
        break;
    }
}

void debugUpdateFps(void)
{
    debugController.fpsTime = getTime();

    if (!debugController.fpsLastUpdate)
        debugController.fpsLastUpdate = debugController.fpsTime.sec_utc;

    if (!debugController.fpsCount)
    {
        debugController.fpsFindLowest = 512;
        debugController.fpsFindHighest = 0;
    }

    debugController.fpsHeap += real_fps;
    debugController.fpsCount ++;

    if (real_fps < debugController.fpsFindLowest) debugController.fpsFindLowest = real_fps;
    if (real_fps > debugController.fpsFindHighest) debugController.fpsFindHighest = real_fps;

    if (debugController.fpsTime.sec_utc != debugController.fpsLastUpdate)
    {
        debugController.fpsAverage = debugController.fpsHeap / (double) debugController.fpsCount;
        debugController.fpsLowest = debugController.fpsFindLowest;
        debugController.fpsHighest = debugController.fpsFindHighest;
        debugController.fpsHeap = 0;
        debugController.fpsCount = 0;
        debugController.fpsFindLowest = 512;
        debugController.fpsFindHighest = 0;

        debugController.fpsLastUpdate = debugController.fpsTime.sec_utc;
    }
}

// It is recommended to only use this function through the macro DEBUG_INIT()
int debugCreateFile(void)
{
    FILE *f;

    f = fopen("debug.log", "w+");

    if (f)
    {
        stTime t = getTime();
        debugController.startTime = t.sec_utc;

        debugController.fileInitialized = 1;
        fclose(f);
        debugMsgFrom("Debug file initialized.", __FILE__, __LINE__);
        return true;
    }

    return false;
}

// It is recommended to only use this function through the macro DEBUG_MSG()
void debugMsg(const char *msg)
{
    FILE *f = NULL;

    if (!debugController.fileInitialized)
        debugCreateFile();

    f = fopen("debug.log", "a+");

    if (f)
    {
        fprintf(f, "%s\n", msg);
        fclose(f);
    }
}

// It is recommended to use this function only through the macro DEBUG_MSG_FROM()
void debugMsgFrom(const char *msg, const char *label, int line)
{
    char temp[256];
    stTime t = getTime();

    if (!debugController.fileInitialized)
        debugCreateFile();

    sprintf(temp, "[%-25s, line: %4i, time: %3i, frame: %5i]: \"%s\"",
            label, line, t.sec_utc - debugController.startTime, frame, msg);
    debugMsg(temp);
}
