#define SWE_WINDOW_WORLD_CONTEXT_MENU   "worldContextMenu"
#define SWE_WINDOW_NEW_PROJECT_ALERT    "newProjectAlert"
#define SWE_WINDOW_PLACE_ENTITY         "placeEntityWindow"
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
            5, 2, 0, 1.0f, WHITE, WHITE, WHITE, BLACK, BLACK, BLACK, WHITE,
            createRGB(210, 255, 255, 1.0), createRGB(150, 255, 255, 1.0), createRGB(128, 128, 128, 1.0));

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
    setPosition(addButton(panel, "okBtn", "Ok", createCloseWindowAction(window->tag)), 1, 0);

    // Define "place trigger" window
    window = createWindow(SWE_WINDOW_PLACE_TRIGGER, sweStyle);
    panel = getWindowRootPanel(window);
    setPosition(addText(panel, "title", "\nPlace trigger", 0), 0, 0);
    setPosition(addButton(panel, "okBtn", "Ok", createCloseWindowAction(window->tag)), 1, 0);
}
