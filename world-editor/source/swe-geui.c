#define DEFAULT_ZDEPTH 0.8 // SABRE WORLD EDITOR (SWE) MODIFICATION, original value: 0.5
#define USE_DEFAULT_STYLE GEUIController.sDefault

#define GEUI_MOUSE_UP 0
#define GEUI_MOUSE_DOWN 1

#define GEUI_CLONENAME_SIZE 42

enum mouseButtonsEnum // Used as array indices, don't touch!
{
    GEUI_MOUSE_LEFT = 0,
    GEUI_MOUSE_RIGHT,
    GEUI_MOUSE_MIDDLE,
    GEUI_MOUSE_WHEEL_UP,
    GEUI_MOUSE_WHEEL_DOWN,
    GEUI_MOUSE_BUTTONS     // Number of supported mouse buttons (5)
};

typedef struct LayoutStruct
{
    short row;
    short col;
    short width;
    short height;
    short startx;
    short starty;
}Layout;

const unsigned long GEUI_TITLE_BAR  = (1 << 0);
const unsigned long GEUI_FAKE_ACTOR = (1 << 1);
const unsigned long GEUI_CLICKED    = (1 << 2);

typedef enum ItemTypeEnum
{
    GEUI_Text,
    GEUI_Button,
    GEUI_Panel,
    GEUI_Embedder
}ItemType;

struct WindowStruct;
struct PanelStruct;

typedef struct WindowItemStruct
{
    int index;          // item index
    char tag[256];      // item identifier tag

    ItemType type;      // item type

    union ItemDataUnion // item data union for different item types
    {
        struct TextItem     { Text text; }text; // TODO: could this be replaced with a plain Text element?
        struct ButtonItem
        {
            Text text;
            char state;
            long bTileStartIndex;
            long bTileEndIndex;
            void (*actionFunction)(struct WindowStruct *, struct WindowItemStruct *);
        }button;
        struct PanelItem // TODO: could this be replaced with a plain Panel element?
        {
            struct PanelStruct *panel;
        }panel;
        struct EmbedderItem
        {
            char actorCName[256];
        }embedder;
    }data;

    Layout layout;
    struct PanelStruct *myPanel;
    struct WindowStruct *parent;     // pointer to parent window
    struct WindowItemStruct *next;   // pointer to next item in list
}WindowItem;

typedef struct PanelStruct
{
    int index;
    int iIndex;
    short rows;
    short cols;
    short width;
    short height;
    struct WindowStruct *parent;
    struct WindowItemStruct *iList;
}Panel;

void updatePanelLayout(Panel *panel);
void setPosition(WindowItem *this, short row, short col);
short getColWidth(Panel *panel, short col);
short getRowHeight(Panel *panel, short row);
short getPanelWidth(Panel *panel);
short getPanelHeight(Panel *panel);

typedef struct WindowStruct
{
    int index;          // window index
    int pIndex;         // next available panel index
    char tag[256];      // window identifier tag
    bool isOpen;        // is window currently open or not
    Style style;        // window style
    double zDepth;      // window z depth
    char parentCName[256]; // clonename of the window parent actor
    long wTileStartIndex;   // cloneindex of the first window tile
    long wTileEndIndex;     // cloneindex of the last window tile
    Panel mainPanel;            // window main panel
    struct WindowStruct *next;  // pointer to next window in list
}Window;

void initGEUI(void);
void quitGEUI(void);

// BEGIN SABRE WORLD EDITOR (SWE) MODIFICATION
void enableGEUIMouse();
void disableGEUIMouse();
int isGEUIActive();
// END SABRE WORLD EDITOR (SWE) MODIFICATION

Actor *getTile(long index);
void updateIndexBounds(long *low, long *hi, long val);
int isTopmostItemAtMouse(WindowItem *item);
WindowItem *initNewItem(ItemType type, Window *window, Panel *panel, char tag[256]);
WindowItem *addItemToWindow(WindowItem *ptr);
WindowItem *addText(Window *window, Panel *panel, char tag[256], char *string, short maxWidth);
WindowItem *addButton(Window *window, Panel *panel, char tag[256], char *string, void (*actionFunction)(Window *, WindowItem *));
WindowItem *addPanel(Window *window, Panel *panel, char tag[256]);
WindowItem *addEmbedder(Window *window, Panel *panel, char tag[256], const char *actorName);
WindowItem *getItemFromPanelByTag(Panel *panel, char tag[256]);
WindowItem *getItemByTag(Window *window, char tag[256]);
WindowItem *getItemFromPanelByIndex(Panel *panel, int index);
WindowItem *getItemByIndex(Window *window, int index);
Panel *getPanelByTag(Panel *panel, char tag[256]);
Panel *getPanelByIndex(Panel *panel, int index);
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
void doKeyDown(WindowItem *item, short key);
void doKeyUp(WindowItem *item, short key);
void eraseWindowItem(WindowItem *ptr);
void destroyWindowItem(WindowItem *ptr);
int calculateAnimpos(short w, short h, short i, short j);
Window *createWindow(char tag[256], Style style);
Window *getWindowByTag(char tag[256]);
Window *getWindowByIndex(int index);
void buildItems(Panel *panel);
void buildItem(WindowItem *ptr);
void buildText(WindowItem *ptr);
void buildPanel(WindowItem *ptr);
void buildButtonText(WindowItem *ptr);
void buildButton(WindowItem *ptr);
void buildEmbedder(WindowItem *ptr);
void buildWindow(Window *window);
Window *openWindow(char tag[256]);
Actor *createWindowBaseParent(Window *window);
void setPanelBaseParent(Panel *panel, char *parentName);
void setWindowBaseParent(Window *window, char *parentName);
void bringWindowToFront(Window *window);
void closePanel(Panel *panel);
void closeWindow(Window *window);
void destroyWindow(Window *window);
void destroyWindowList(void);
void destroyPanel(Panel *panel);
short getRowStart(WindowItem *panelItem, Panel *panel, short row);
short getColStart(WindowItem *panelItem, Panel *panel, short col);
void calculateRowsAndCols(Panel *panel);
void updatePanelLayout(WindowItem *panelItem, Panel *panel);

struct GEUIControllerStruct
{
    int wIndex;
    int topIndex;
    Style sDefault;
    Window *wList;

    Actor *mButtonActors[GEUI_MOUSE_BUTTONS];
    char mButtonTopActorCName[GEUI_MOUSE_BUTTONS][GEUI_CLONENAME_SIZE];
    int mButtonActorCount[GEUI_MOUSE_BUTTONS];
    int mButtonState[GEUI_MOUSE_BUTTONS];
    enum mouseButtonsEnum activeButton;

    // BEGIN SABRE WORLD EDITOR (SWE) MODIFICATION
    int openWindowCount;
    // END SABRE WORLD EDITOR (SWE) MODIFICATION
}GEUIController;

void initGEUI(void)
{
    //DEBUG_INIT(); // debug file initialization on startup disabled for now
    // TODO: separate DEBUG_INIT() and DEBUG_INIT_FILE()

    strcpy(defStyle.guiAnim, "gui_sheet_default");  // GUI animation name
    getTileDimensions(&defStyle);
    defStyle.titleFont          = &defTitleFont;
    defStyle.labelFont          = &defLabelFont;
    defStyle.textFont           = &defTextFont;
    defStyle.padding            = 5;
    defStyle.titleBgColor       = DEFAULT_COLOR;
    defStyle.windowBgColor      = DEFAULT_COLOR;
    defStyle.titleColor         = BLACK;
    defStyle.labelColor         = BLACK;
    defStyle.textColor          = BLACK;
    defStyle.buttonColor        = DEFAULT_COLOR;
    defStyle.buttonHilitColor   = CYAN;
    defStyle.buttonPressedColor = BLUE;

    GEUIController.wIndex = 0;
    GEUIController.topIndex = 0;
    GEUIController.sDefault = defStyle;
    GEUIController.wList = NULL;

    // BEGIN SABRE WORLD EDITOR (SWE) MODIFICATION
    GEUIController.openWindowCount = 0;
    disableGEUIMouse();
    // END SABRE WORLD EDITOR (SWE) MODIFICATION
}

void quitGEUI(void)
{
    int mb;

    for (mb = 0; mb < GEUI_MOUSE_BUTTONS; mb ++)
    {
        if (GEUIController.mButtonActors[mb])
            free(GEUIController.mButtonActors[mb]);
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

WindowItem *initNewItem(ItemType type, Window *window, Panel *panel, char tag[256])
{
    WindowItem *ptr = NULL;

    if (!panel) { DEBUG_MSG_FROM("panel is NULL", "initNewItem"); return NULL; }

    ptr = malloc(sizeof *ptr);

    if (!ptr) { DEBUG_MSG_FROM("memory allocation failed", "initNewItem"); return NULL; }

    ptr->type = type;
    ptr->index = panel->iIndex ++;
    strcpy(ptr->tag, tag);
    ptr->myPanel = panel;
    ptr->parent = window;

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

    ptr->data.text.text = createText(string, window->style.textFont, "(none)", ABSOLUTE, 0, 0);
    setTextColor(&ptr->data.text.text, window->style.textColor);
    setTextZDepth(&ptr->data.text.text, 0.6);

    if (maxWidth > 0)
        fitTextInWidth(&ptr->data.text.text, maxWidth);

    ptr->layout.row = 0;
    ptr->layout.col = 0;
    ptr->layout.width = ptr->data.text.text.width;
    ptr->layout.height = ptr->data.text.text.height;
    ptr->layout.startx = 0;
    ptr->layout.starty = 0;

    return addItemToWindow(ptr);
}

WindowItem *addButton(Window *window, Panel *panel, char tag[256], char *string, void (*actionFunction)(Window *, WindowItem *))
{
    WindowItem *ptr = initNewItem(GEUI_Button, window, panel, tag);
    if (!ptr) { DEBUG_MSG_FROM("item is NULL", "addButton"); return NULL; }

    ptr->data.button.text = createText(string, window->style.textFont, "(none)", ABSOLUTE, 0, 0);
    setTextColor(&ptr->data.button.text, window->style.textColor);
    setTextZDepth(&ptr->data.button.text, 0.5);
    ptr->data.button.state = 0;
    ptr->data.button.bTileStartIndex = -1;
    ptr->data.button.bTileEndIndex = -1;
    ptr->data.button.actionFunction = actionFunction;

    ptr->layout.row = 0;
    ptr->layout.col = 0;
    ptr->layout.width = ptr->data.button.text.width + ptr->parent->style.tileWidth * 2;
    ptr->layout.height = ptr->parent->style.tileHeight;
    ptr->layout.startx = 0;
    ptr->layout.starty = 0;

    return addItemToWindow(ptr);
}

WindowItem *addPanel(Window *window, Panel *panel, char tag[256])
{
    WindowItem *ptr = initNewItem(GEUI_Panel, window, panel, tag);
    if (!ptr) { DEBUG_MSG_FROM("item is NULL", "addPanel"); return NULL; }

    ptr->data.panel.panel = malloc(sizeof *ptr->data.panel.panel);
    if (!ptr->data.panel.panel)
    {
        free(ptr);
        DEBUG_MSG_FROM("memory allocation failed", "addPanel");
        return NULL;
    }

    ptr->data.panel.panel->index = window->pIndex++;
    ptr->data.panel.panel->iIndex = 0;
    ptr->data.panel.panel->rows = 0;
    ptr->data.panel.panel->cols = 0;
    ptr->data.panel.panel->width = 0;
    ptr->data.panel.panel->height = 0;
    ptr->data.panel.panel->parent = window;

    ptr->layout.row = 0;
    ptr->layout.col = 0;
    ptr->layout.width = 0;
    ptr->layout.height = 0;
    ptr->layout.startx = 0;
    ptr->layout.starty = 0;

    return addItemToWindow(ptr);
}

WindowItem *addEmbedder(Window *window, Panel *panel, char tag[256], const char *actorName)
{
    Actor *actor;
    WindowItem *ptr = initNewItem(GEUI_Embedder, window, panel, tag);
    if (!ptr) { DEBUG_MSG_FROM("item is NULL", "addEmbedder"); return NULL; }

    if (!actorExists2(actor = getclone(actorName))) { DEBUG_MSG_FROM("actor doesn't exist", "addEmbedder"); free(ptr); return NULL; }

    strcpy(ptr->data.embedder.actorCName, actor->clonename);
    VisibilityState(ptr->data.embedder.actorCName, DONT_DRAW_ONLY);

    ptr->layout.row = 0;
    ptr->layout.col = 0;
    ptr->layout.width = actor->width;
    ptr->layout.height = actor->height;
    ptr->layout.startx = 0;
    ptr->layout.starty = 0;

    return addItemToWindow(ptr);
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
            result = getItemFromPanelByTag(ptr->data.panel.panel, tag);

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

Panel *getPanelByTag(Panel *panel, char tag[256])
{
    WindowItem *ptr;

    if (!panel) { DEBUG_MSG_FROM("panel is NULL", "getPanelByTag"); return NULL; }

    for (ptr = panel->iList; ptr != NULL; ptr = ptr->next)
    {
        if (!strcmp(ptr->tag, tag))
            return ptr->data.panel.panel;
        if (ptr->type == GEUI_Panel)
        {
            Panel *p = getPanelByTag(ptr->data.panel.panel, tag);
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

            if (ptr->data.panel.panel->index == index)
                return ptr->data.panel.panel;

            p = getPanelByIndex(ptr->data.panel.panel, index);
            if (p) return p;
        }
    }

    return NULL;
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
                long start = item->data.button.bTileStartIndex;
                long end   = item->data.button.bTileEndIndex;

                if (item->data.button.state)
                    colorClones("a_gui", start, end, item->parent->style.buttonPressedColor);
                else
                    colorClones("a_gui", start, end, item->parent->style.buttonHilitColor);
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
        { DEBUG_MSG_FROM("actor doesn't exist", "doMouseLeave"); return; }
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
                long start = item->data.button.bTileStartIndex;
                long end   = item->data.button.bTileEndIndex;

                if (item->data.button.state)
                    colorClones("a_gui", start, end, item->parent->style.buttonPressedColor);
                else
                    colorClones("a_gui", start, end, item->parent->style.buttonColor);
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
        ChangeZDepth(actor->clonename, 0.6);

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
        ChangeZDepth(fake->clonename, 0.2);
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
            colorClones("a_gui",
                item->data.button.bTileStartIndex,
                item->data.button.bTileEndIndex, window->style.buttonPressedColor);
            item->data.button.state = 1;
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
        ChangeZDepth(actor->clonename, 0.1);

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
            long start = item->data.button.bTileStartIndex;
            long end   = item->data.button.bTileEndIndex;

            if (isTopmostItemAtMouse(item))
            {
                colorClones("a_gui", start, end, window->style.buttonHilitColor);
                if (item->data.button.state && item->data.button.actionFunction)
                    item->data.button.actionFunction(window, item);
            }
            else
            {
                colorClones("a_gui", start, end, window->style.buttonColor);
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

void doKeyDown(WindowItem *item, short key)
{
    // TODO: implement doKeyDown
}

void doKeyUp(WindowItem *item, short key)
{
    // TODO: implement doKeyUp
}

void eraseWindowItem(WindowItem *ptr)
{
    if (!ptr) { DEBUG_MSG_FROM("item is NULL", "eraseWindowItem"); return; }

    switch (ptr->type)
    {
        case GEUI_Text:
            eraseText(&ptr->data.text.text);
            setTextParent(&ptr->data.text.text, "(none)", False);
        break;
        case GEUI_Button:
            eraseText(&ptr->data.button.text);
            if (ptr->data.button.bTileStartIndex > -1)
            {
                destroyClones("a_gui", ptr->data.button.bTileStartIndex, ptr->data.button.bTileEndIndex);
                ptr->data.button.bTileStartIndex = -1;
                ptr->data.button.bTileEndIndex = -1;
            }
        break;
        case GEUI_Panel:
            closePanel(ptr->data.panel.panel);
        break;
        case GEUI_Embedder:
            VisibilityState(ptr->data.embedder.actorCName, DONT_DRAW_ONLY);
        break;

        default: break;
    }
}

void destroyWindowItem(WindowItem *ptr)
{
    if (!ptr) { DEBUG_MSG_FROM("item is NULL", "destroyWindowItem"); return; }

    switch (ptr->type)
    {
        case GEUI_Text: destroyText(&ptr->data.text.text); break;
        case GEUI_Button:
            destroyText(&ptr->data.button.text);
            if (ptr->data.button.bTileStartIndex > -1)
            {
                destroyClones("a_gui", ptr->data.button.bTileStartIndex, ptr->data.button.bTileEndIndex);
                ptr->data.button.bTileStartIndex = -1;
                ptr->data.button.bTileEndIndex = -1;
            }
        break;
        case GEUI_Panel:
            destroyPanel(ptr->data.panel.panel);
            free(ptr->data.panel.panel);
        break;
        case GEUI_Embedder:
            DestroyActor(ptr->data.embedder.actorCName);
        break;

        default: break;
    }
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

Window *createWindow(char tag[256], Style style)
{
    Window *ptr = malloc(sizeof *ptr);

    if (!ptr) { DEBUG_MSG_FROM("memory allocation failed", "createWindow"); return NULL; }

    ptr->index = GEUIController.wIndex ++;
    ptr->pIndex = 0;
    strcpy(ptr->tag, tag);
    ptr->isOpen = False;
    ptr->style = style;
    ptr->zDepth = DEFAULT_ZDEPTH;
    strcpy(ptr->parentCName, "");
    ptr->wTileStartIndex = -1;
    ptr->wTileEndIndex = -1;
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
        case GEUI_Panel: buildPanel(ptr); break;
        case GEUI_Embedder: buildEmbedder(ptr); break;
    }
}

void buildText(WindowItem *ptr)
{
    if (ptr->type != GEUI_Text) { DEBUG_MSG_FROM("item is not a valid Text item", "buildText"); return; }

    setTextZDepth(&ptr->data.text.text, 0.3);
    // TODO: layout / positioning
    setTextPosition(&ptr->data.text.text,
        ptr->layout.startx + ptr->parent->style.padding,
        ptr->layout.starty + ptr->data.text.text.pFont->lineSpacing * 0.5 + ptr->parent->style.padding);
    refreshText(&ptr->data.text.text);
}

void buildPanel(WindowItem *ptr)
{
    if (ptr->type != GEUI_Panel) { DEBUG_MSG_FROM("item is not a valid Panel item", "buildPanel"); return; }

    buildItems(ptr->data.panel.panel);
}

void buildButtonText(WindowItem *ptr)
{
    long start = ptr->data.button.bTileStartIndex;
    long end = ptr->data.button.bTileEndIndex;

    Text *buttonText = &ptr->data.button.text;

    colorClones("a_gui", start, end, ptr->parent->style.buttonColor);
    setTextZDepth(buttonText, 0.4);
    setTextAlignment(buttonText, ALIGN_CENTER);
    setTextPosition(buttonText,
        ceil((getTile(end)->x - getTile(start)->x) * 0.5) + getTile(start)->x, getTile(start)->y);
    refreshText(buttonText);
}

void buildButton(WindowItem *ptr)
{
    short i;
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
        Actor *a;
        a = CreateActor("a_gui", ptr->parent->style.guiAnim, ptr->parent->parentCName, "(none)", 0, 0, true);
        // TODO: layout / positioning
        a->x = ptr->layout.startx + tileWidth + i * tileWidth + (i >= 2 && i >= tilesHorizontal - 2) * (buttonWidth - tilesHorizontal * tileWidth)-tileWidth/2;// + (ptr->layout.col > 0); // TODO: make nicer
        a->x += ptr->parent->style.padding;
        a->y = ptr->layout.starty + tileHeight-tileWidth/2;// + (ptr->layout.row > 0);
        a->y += ptr->parent->style.padding;
        a->myWindow = ptr->parent->index;
        a->myPanel  = ptr->myPanel->index;
        a->myIndex  = ptr->index;
        ChangeZDepth(a->clonename, 0.35); // TODO: change back to 0.3 after testing
        a->animpos = 9 + (i > 0) + (i == tilesHorizontal - 1) + (i > 0 && i == tilesHorizontal - 2 && buttonWidth < tileWidth * 2.5);// TODO: make nicer

        updateIndexBounds(&ptr->data.button.bTileStartIndex, &ptr->data.button.bTileEndIndex, a->cloneindex);
    }

    buildButtonText(ptr);
}

void buildEmbedder(WindowItem *ptr)
{
    Actor *actor;
    if (ptr->type != GEUI_Embedder) { DEBUG_MSG_FROM("item is not a valid Embedder item", "buildEmbedder"); return; }

    if (!actorExists2(actor = getclone(ptr->data.embedder.actorCName))) { DEBUG_MSG_FROM("actor doesn't exist", "buildEmbedder"); return; }

    ChangeZDepth(ptr->data.embedder.actorCName, 0.3);
    ChangeParent(ptr->data.embedder.actorCName, "(none)");
    actor->x = 0;
    actor->y = 0;
    ChangeParent(ptr->data.embedder.actorCName, ptr->parent->parentCName);
    actor->x = ptr->layout.startx + ptr->parent->style.padding + actor->width / 2;
    actor->y = ptr->layout.starty + ptr->parent->style.padding + actor->height / 2;
    VisibilityState(ptr->data.embedder.actorCName, ENABLE);

    {
        char temp[256];
        sprintf(temp, "%d, %d - %d, %d", ptr->layout.startx + ptr->parent->style.padding + actor->width / 2,
                                ptr->layout.starty + ptr->parent->style.padding + actor->height / 2,
                                (int)actor->x,
                                (int)actor->y);
        DEBUG_MSG_FROM(temp, "buildEmbedder");
    }
}

void buildWindow(Window *window)
{
    short i, j;
    Actor *tile;

    short tileWidth, tileHeight;
    short windowWidth, windowHeight;
    short tilesHorizontal, tilesVertical;

    setWindowBaseParent(window, createWindowBaseParent(window)->clonename);

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
            ChangeZDepth(tile->clonename, 0.1);
            EventDisable(tile->clonename, EVENTCOLLISION);
            EventDisable(tile->clonename, EVENTCOLLISIONFINISH);

            if (j == 0) tile->myProperties = GEUI_TITLE_BAR; // part of the window title bar

            updateIndexBounds(&window->wTileStartIndex, &window->wTileEndIndex, tile->cloneindex);
        }
    }
}

Window *openWindow(char tag[256])
{
    Window *window = getWindowByTag(tag);

    if (!window) { DEBUG_MSG_FROM("window is NULL", "openWindow"); return NULL; }
    if (window->isOpen) { DEBUG_MSG_FROM("window is already open", "openWindow"); return window; }

    updatePanelLayout(NULL, &window->mainPanel);
    buildWindow(window);
    buildItems(&window->mainPanel);

    window->isOpen = True;
    bringWindowToFront(window);

    // BEGIN SABRE WORLD EDITOR (SWE) MODIFICATION
    if (!GEUIController.openWindowCount++)
        enableGEUIMouse();
    // END SABRE WORLD EDITOR (SWE) MODIFICATION

    return window;
}

Actor *createWindowBaseParent(Window *window)
{
    Actor *baseParent;

    baseParent = CreateActor("a_gui", window->style.guiAnim, "(none)", "(none)", 0, 0, true);
    baseParent->animpos = 0;
    ChangeZDepth(baseParent->clonename, window->zDepth);
    CollisionState(baseParent->clonename, DISABLE);

    return baseParent;
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
            case GEUI_Text: setTextParent(&ptr->data.text.text, parentName, True); break;
            case GEUI_Button:
                setTextParent(&ptr->data.button.text, parentName, True);
                if (ptr->data.button.bTileStartIndex > -1)
                    changeParentOfClones("a_gui", ptr->data.button.bTileStartIndex, ptr->data.button.bTileEndIndex, parentName);
                break;
             case GEUI_Panel: setPanelBaseParent(ptr->data.panel.panel,  parentName); break;
             case GEUI_Embedder: ChangeParent(ptr->data.embedder.actorCName, parentName); break;

            default: break;
        }

        ptr = ptr->next;
    }
}

void setWindowBaseParent(Window *window, char *parentName)
{
    WindowItem *ptr = NULL;

    if (!window) { DEBUG_MSG_FROM("window is NULL", "setWindowBaseParent"); return; }
    if (!actorExists(parentName)) { DEBUG_MSG_FROM("actor doesn't exist", "setWindowBaseParent"); return; }

    strcpy(window->parentCName, parentName);

    if (window->wTileStartIndex > -1)
        changeParentOfClones("a_gui", window->wTileStartIndex, window->wTileEndIndex, parentName);

    setPanelBaseParent(&window->mainPanel, parentName);
}

void bringWindowToFront(Window *window)
{
    Window *ptr = NULL;

    if (!window) { DEBUG_MSG_FROM("window is NULL", "bringWindowToFront"); return; }

    ptr = GEUIController.wList;

    while (ptr)
    {
        if (ptr->index == window->index)
        {
            ptr->zDepth = 0.85; // SABRE WORLD EDITOR MODIFICATION, original value: 0.6
            ChangeZDepth(ptr->parentCName, 0.85); // SABRE WORLD EDITOR MODIFICATION, original value: 0.6
            GEUIController.topIndex = window->index;
        }
        else
        {
            ptr->zDepth = DEFAULT_ZDEPTH;
            ChangeZDepth(ptr->parentCName, DEFAULT_ZDEPTH);
        }

        ptr = ptr->next;
    }
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

void closeWindow(Window *window)
{
    WindowItem *ptr = NULL;

    if (!window) { DEBUG_MSG_FROM("window is NULL", "closeWindow"); return; }

    window->isOpen = False;

    DestroyActor(window->parentCName);
    strcpy(window->parentCName, "(none)");

    if (window->wTileStartIndex > -1)
    {
        destroyClones("a_gui", window->wTileStartIndex, window->wTileEndIndex);
        window->wTileStartIndex = -1;
        window->wTileEndIndex = -1;
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

void setPosition(WindowItem *this, short row, short col)
{
    if (!this) { DEBUG_MSG_FROM("item is NULL", "setPosition"); return; }

    this->layout.row = row;
    this->layout.col = col;
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
        if (item->type == GEUI_Panel) updatePanelLayout(item, item->data.panel.panel);
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
        if (item->type == GEUI_Panel) updatePanelLayout(item, item->data.panel.panel);
        if (item->layout.row == row && item->layout.height > height)
            height = item->layout.height;
    }

    return height + panel->parent->style.padding * (row < panel->rows - 1);
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
