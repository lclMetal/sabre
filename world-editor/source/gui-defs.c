#define SWE_WINDOW_WORLD_CONTEXT_MENU   "worldContextMenu"
#define SWE_WINDOW_NEW_PROJECT_ALERT    "newProjectAlert"
#define SWE_WINDOW_PLACE_ENTITY         "placeEntityWindow"
#define SWE_WINDOW_CREATE_ENTITY_TYPE   "createEntityTypeWindow"
#define SWE_WINDOW_PLACE_TRIGGER        "placeTriggerWindow"

void SWE_CreateWindows()
{
    Window  *window;
    Panel   *panel;

    Style contextMenuStyle =
        createStyle("gui_sheet_small",
            &defTitleFont, &defLabelFont, &segoeui12,
            0, 2, GEUI_BUTTON_TEXT_ALIGN_LEFT | GEUI_BUTTON_STRETCH, 0.1f,
            WHITE, WHITE, WHITE, BLACK, BLACK, BLACK, WHITE,
            createRGB(210, 255, 255, 1.0), createRGB(150, 255, 255, 1.0), createRGB(128, 128, 128, 1.0));

    Style sweStyle =
        createStyle("gui_sheet_default",
            &defTitleFont, &defLabelFont, &defTextFont,
            5, 2, 0, 1.0f, WHITE, WHITE, WHITE, BLACK, BLACK, BLACK, createRGB(235, 235, 235, 1.0),
            createRGB(210, 255, 255, 1.0), createRGB(150, 255, 255, 1.0), createRGB(64, 128, 255, 1.0));

    // Define world context menu
    window = createWindow(SWE_WINDOW_WORLD_CONTEXT_MENU, contextMenuStyle);
    getTileDimensions(&window->style);
    panel = getWindowRootPanel(window);
    setPosition(addButton(panel, "placeEntityBtn", "Place entity", createOpenWindowAction(SWE_WINDOW_PLACE_ENTITY, GEUI_WPOS_MOUSE_TOP)), 0, 0);
    setPosition(addButton(panel, "placeTriggerBtn", "Place trigger     ", createOpenWindowAction(SWE_WINDOW_PLACE_TRIGGER, GEUI_WPOS_MOUSE_TOP)), 1, 0);
    // setPosition(addButton(panel, "closeBtn", "Close", createCloseWindowAction(window->tag)), 2, 0);

    // Define "new project" alert window
    window = createWindow(SWE_WINDOW_NEW_PROJECT_ALERT, sweStyle);
    panel = getWindowRootPanel(window);
    setPosition(addText(panel, "title", "\nCreate new project?", 0), 0, 0);
    setPosition(addText(panel, "alertText",
        "Creating a new project will discard the current one. Continue?", 300), 1, 0);
    panel = getPanel(setPosition(addPanel(panel, "buttonsPanel"), 2, 0));
    setPosition(addButton(panel, "okBtn", "Ok", createCloseWindowAction(window->tag)), 0, 0);
    setPosition(addButton(panel, "cancelBtn", "Cancel", createCloseWindowAction(window->tag)), 0, 1);

    // Define "place entity" window
    window = createWindow(SWE_WINDOW_PLACE_ENTITY, sweStyle);
    panel = getWindowRootPanel(window);
    setPosition(addText(panel, "title", "\nPlace entity", 0), 0, 0);
    panel = getPanel(setPosition(addPanel(panel, "buttonsPanel"), 1, 0));
    setPosition(addButton(panel, "okBtn", "Ok", createCloseWindowAction(window->tag)), 0, 0);
    setPosition(addButton(panel, "createNewBtn", "Create new entity type", createOpenWindowAction(SWE_WINDOW_CREATE_ENTITY_TYPE, GEUI_WPOS_SCREEN_CENTER)), 0, 1);

    // Define "create entity type" window
    window = createWindow(SWE_WINDOW_CREATE_ENTITY_TYPE, sweStyle);
    panel = getWindowRootPanel(window);
    setPosition(addText(panel, "title", "\nCreate new entity type\n", 0), 0, 0);
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
    window = createWindow(SWE_WINDOW_PLACE_TRIGGER, sweStyle);
    panel = getWindowRootPanel(window);
    setPosition(addText(panel, "title", "\nPlace trigger", 0), 0, 0);
    setPosition(addButton(panel, "okBtn", "Ok", createCloseWindowAction(window->tag)), 1, 0);
}
