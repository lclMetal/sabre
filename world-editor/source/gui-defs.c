#define SWE_WINDOW_WORLD_CONTEXT_MENU   "worldContextMenu"
#define SWE_WINDOW_NEW_PROJECT_ALERT    "newProjectAlert"
#define SWE_WINDOW_OPEN_PROJECT         "openProjectWindow"
#define SWE_WINDOW_SAVE_PROJECT         "saveProjectWindow"
#define SWE_WINDOW_PLACE_ENTITY         "placeEntityWindow"
#define SWE_WINDOW_CREATE_ENTITY_TYPE   "createEntityTypeWindow"
#define SWE_WINDOW_PLACE_TRIGGER        "placeTriggerWindow"
#define SWE_WINDOW_TXR_CONFIG           "configureTexture"
#define SWE_WINDOW_LEVEL_PROPERTIES     "levelProperties"

Style sweStyle;
Style contextMenuStyle;

Font titleFontYes =
{
     1, // Letter spacing in pixels
     6, // Word spacing in pixels
    25, // Line spacing in pixels
     4, // Baseline offset in pixels
    28, // Indentation in pixels
    "segoeui20", // Font animation name
    {   // Character widths
         4,  6, 12,  9, 16, 15,  2,  5,  5,  8,
        10,  3,  6,  4,  9, 10,  6, 10,  9, 11,
         9, 10, 10, 10, 10,  4,  4, 10, 10, 10,
         8, 16, 14,  9, 12, 13,  9,  9, 12, 12,
         2,  6, 10,  8, 16, 12, 14,  9, 15, 11,
        10, 10, 12, 13, 20, 12, 12, 12,  5,  8,
         5, 10, 10,  4,  9, 10,  8, 10, 10,  7,
        10, 10,  3,  7,  9,  2, 15, 10, 10, 10,
        10,  6,  8,  6, 10, 10, 14, 10, 10, 10,
         6,  2,  6, 10
    }
};

Font labelFontYes =
{
     1, // Letter spacing in pixels
     4, // Word spacing in pixels
    19, // Line spacing in pixels
     3, // Baseline offset in pixels
    21, // Indentation in pixels
    "segoeui16", // Font animation name
    {   // Character widths
         2,  4, 10,  8, 12, 13,  2,  5,  5,  6,
         8,  3,  6,  2,  8,  8,  5,  8,  8,  9,
         8,  8,  8,  8,  8,  2,  3,  8,  8,  8,
         6, 14, 10,  8, 10, 10,  7,  7, 10, 10,
         2,  5,  9,  7, 12, 10, 12,  8, 13,  9,
         8,  8, 10, 10, 16, 10, 10, 10,  4,  8,
         4,  8,  8,  4,  7,  8,  7,  8,  8,  6,
         8,  8,  2,  5,  7,  2, 12,  8,  8,  8,
         8,  5,  6,  6,  8,  8, 12,  8,  8,  8,
         4,  2,  4,  8
    }
};

Font textFontYes =
{
     1, // Letter spacing in pixels
     4, // Word spacing in pixels
    19, // Line spacing in pixels
     3, // Baseline offset in pixels
    21, // Indentation in pixels
    "segoeui16", // Font animation name
    {   // Character widths
         2,  4, 10,  8, 12, 13,  2,  5,  5,  6,
         8,  3,  6,  2,  8,  8,  5,  8,  8,  9,
         8,  8,  8,  8,  8,  2,  3,  8,  8,  8,
         6, 14, 10,  8, 10, 10,  7,  7, 10, 10,
         2,  5,  9,  7, 12, 10, 12,  8, 13,  9,
         8,  8, 10, 10, 16, 10, 10, 10,  4,  8,
         4,  8,  8,  4,  7,  8,  7,  8,  8,  6,
         8,  8,  2,  5,  7,  2, 12,  8,  8,  8,
         8,  5,  6,  6,  8,  8, 12,  8,  8,  8,
         4,  2,  4,  8
    }
};

void applyTextureConfig(GUIAction *action)
{
    SABRE_Texture *txr = getBoundData(action->window, "texture");

    if (txr)
    {
        txr->isWindow = getCheckboxState(getItemByTag(action->window, "chkAttribIsWindow"), False);
    }

    closeWindow(action->window);
}

void saveProject(GUIAction *action)
{
    // saveProjectData(addFileExtension(readTextInput(getItemByTag(action->window, "fileName")), "swp"));
    writeCurrentLevelProjectData(addFileExtension(readTextInput(getItemByTag(action->window, "fileName")), "swp"));
    closeWindow(action->window);
}

void loadProject(GUIAction *action)
{
    loadProjectData(addFileExtension(readTextInput(getItemByTag(action->window, "fileName")), "swp"));
    closeWindow(action->window);
}

void populateTextureConfigWindow(SABRE_Texture *txr)
{
    char temp[256];
    Window *window = getWindowByTag(SWE_WINDOW_TXR_CONFIG);
    WindowItem *item = NULL;

    if ((item = getItemByTag(window, "title")) && item->type == GEUI_Text)
    {
        sprintf(temp, "\nTexture \"%s\" configuration", txr->name);
        setTextContent(item, temp);
    }

    if ((item = getItemByTag(window, "chkAttribIsWindow")) && item->type == GEUI_Checkbox)
    {
        setCheckboxState(item, txr->isWindow);
    }

    updateDataBind(window, "texture", txr);
    openWindow(SWE_WINDOW_TXR_CONFIG, GEUI_WPOS_SCREEN_CENTER);
}

void SWE_CreateWindows(Style *ctxMenuStyle, Style *sweMainStyle)
{
    Window  *window;
    Panel   *panel;

    // Define world context menu
    window = createWindow(SWE_WINDOW_WORLD_CONTEXT_MENU, GEUI_NO_TITLE, ctxMenuStyle);
    getTileDimensions(window->style);
    panel = getWindowRootPanel(window);
    setPosition(addButton(panel, "placeEntityBtn", "Place entity", createOpenWindowAction(SWE_WINDOW_PLACE_ENTITY, GEUI_WPOS_MOUSE_TOP)), 0, 0);
    setPosition(addButton(panel, "placeTriggerBtn", "Place trigger     ", createOpenWindowAction(SWE_WINDOW_PLACE_TRIGGER, GEUI_WPOS_MOUSE_TOP)), 1, 0);
    // setPosition(addButton(panel, "closeBtn", "Close", createCloseWindowAction(window->tag)), 2, 0);

    // Define "new project" alert window
    window = createWindow(SWE_WINDOW_NEW_PROJECT_ALERT, "Create new project?", sweMainStyle);
    panel = getWindowRootPanel(window);
    setPosition(addText(panel, "alertText",
        "Creating a new project will discard the current one. Continue?", 300), 0, 0);
    panel = getPanel(setPosition(addPanel(panel, "buttonsPanel"), 1, 0));
    setPosition(addButton(panel, "okBtn", "Ok", createCloseWindowAction(window->tag)), 0, 0);
    setPosition(addButton(panel, "cancelBtn", "Cancel", createCloseWindowAction(window->tag)), 0, 1);

    // Define "open project" window
    window = createWindow(SWE_WINDOW_OPEN_PROJECT, "Open project", sweMainStyle);
    panel = getWindowRootPanel(window);
    setPosition(addText(panel, "title", "Project file name", 0), 0, 0);
    setPosition(addInputField(panel, "fileName", "untitled", createTextInputSettings(), 200), 1, 0);
    panel = getPanel(setPosition(addPanel(panel, "buttonsPanel"), 2, 0));
    setPosition(addButton(panel, "openBtn", "Open", createAction(loadProject)), 0, 0);
    setPosition(addButton(panel, "cancelBtn", "Cancel", createCloseWindowAction(window->tag)), 0, 1);

    // Define "save project" window
    window = createWindow(SWE_WINDOW_SAVE_PROJECT, "Save project", sweMainStyle);
    panel = getWindowRootPanel(window);
    setPosition(addText(panel, "title", "Project file name", 0), 0, 0);
    setPosition(addInputField(panel, "fileName", "untitled", createTextInputSettings(), 200), 1, 0);
    panel = getPanel(setPosition(addPanel(panel, "buttonsPanel"), 2, 0));
    setPosition(addButton(panel, "saveBtn", "Save", createAction(saveProject)), 0, 0);
    setPosition(addButton(panel, "cancelBtn", "Cancel", createCloseWindowAction(window->tag)), 0, 1);

    // Define "place entity" window
    window = createWindow(SWE_WINDOW_PLACE_ENTITY, "Place entity", sweMainStyle);
    panel = getWindowRootPanel(window);
    setPosition(addText(panel, "title", "Place entity", 0), 0, 0);
    panel = getPanel(setPosition(addPanel(panel, "buttonsPanel"), 1, 0));
    setPosition(addButton(panel, "okBtn", "Ok", createCloseWindowAction(window->tag)), 0, 0);
    setPosition(addButton(panel, "createNewBtn", "Create new entity type", createOpenWindowAction(SWE_WINDOW_CREATE_ENTITY_TYPE, GEUI_WPOS_SCREEN_CENTER)), 0, 1);

    // Define "create entity type" window
    window = createWindow(SWE_WINDOW_CREATE_ENTITY_TYPE, "Create new entity type", sweMainStyle);
    panel = getWindowRootPanel(window);
    setPosition(addText(panel, "title", "Entity settings\n", 0), 0, 0);
    panel = getPanel(setPosition(addPanel(panel, "propertiesPanel"), 1, 0));
    setPosition(addText(panel, "labelName", "Name", 200), 0, 0);
    setPosition(addInputField(panel, "inputName", "Name", createTextInputSettings(), 200), 0, 1);
    setPosition(addText(panel, "labelRadius", "Hitbox radius", 200), 1, 0);
    setPosition(addInputField(panel, "inputName2", "0.2", createDecimalInputSettings(0.01f, 10.0f, 0.2f, 3), 200), 1, 1);
    setPosition(addText(panel, "labelAttributes", "Attributes", 200), 2, 0);
    panel = getPanel(setPosition(addPanel(panel, "attributesPanel"), 2, 1));
    setPosition(addCheckbox(panel, "chkAttribHidden", False), 0, 0);
    setPosition(addText(panel, "labelAttribHidden", "Hidden", 200), 0, 1);
    setPosition(addCheckbox(panel, "chkAttribNoColl", False), 1, 0);
    setPosition(addText(panel, "labelAttribNoColl", "No collision", 200), 1, 1);
    panel = getPanel(setPosition(addPanel(getWindowRootPanel(window), "buttonsPanel"), 2, 0));
    setPosition(addButton(panel, "doneBtn", "Done", createCloseWindowAction(window->tag)), 0, 0);
    setPosition(addButton(panel, "cancelBtn", "Cancel", createCloseWindowAction(window->tag)), 0, 1);

    // Define "place trigger" window
    window = createWindow(SWE_WINDOW_PLACE_TRIGGER, "Place trigger", sweMainStyle);
    panel = getWindowRootPanel(window);
    setPosition(addText(panel, "title", "Place trigger", 0), 0, 0);
    setPosition(addButton(panel, "okBtn", "Ok", createCloseWindowAction(window->tag)), 1, 0);

    // Define "texture config" window
    window = createWindow(SWE_WINDOW_TXR_CONFIG, "Configure texture", sweMainStyle);
    panel = getWindowRootPanel(window);
    addDataBind(window, "texture", NULL);
    setPosition(addText(panel, "title", "Texture settings:", 0), 0, 0);
    // panel = getPanel(setPosition(addPanel(panel, "propertiesPanel"), 1, 0));
    // setPosition(addText(panel, "labelName", "Name", 200), 0, 0);
    // setPosition(addText(panel, "valueName", "Name", 200), 0, 1);
    setPosition(addText(panel, "labelAttributes", "Attributes", 200), 1, 0);
    panel = getPanel(setPosition(addPanel(getWindowRootPanel(window), "attributesPanel"), 2, 0));
    setPosition(addCheckbox(panel, "chkAttribIsWindow", False), 0, 0);
    setPosition(addText(panel, "labelAttribIsWindow", "Is window", 200), 0, 1);
    panel = getPanel(setPosition(addPanel(getWindowRootPanel(window), "buttonsPanel"), 3, 0));
    setPosition(addButton(panel, "doneBtn", "Done", createAction(applyTextureConfig)), 0, 0);
    setPosition(addButton(panel, "cancelBtn", "Cancel", createCloseWindowAction(window->tag)), 0, 1);

    // Define the "level properties" window
    window = createWindow(SWE_WINDOW_LEVEL_PROPERTIES, "Edit level properties", sweMainStyle);
    panel = getWindowRootPanel(window);
    setPosition(addText(panel, "title", "Level properties:", 0), 0, 0);
    setPosition(addText(panel, "labelColors", "Colors", 200), 1, 0);
    panel = getPanel(setPosition(addPanel(getWindowRootPanel(window), "colorsPanel"), 2, 0));
    //setPosition(addInputField(panel, ""))
}
