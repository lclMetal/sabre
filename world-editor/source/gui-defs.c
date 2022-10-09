#define SWE_WINDOW_WORLD_CONTEXT_MENU "worldContextMenu"
#define SWE_WINDOW_NEW_PROJECT_ALERT "newProjectAlert"

void SWE_CreateWindows()
{
    Window  *win;
    Panel   *pan;

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
    win = createWindow("worldContextMenu", contextMenuStyle);
    getTileDimensions(&win->style);
    pan = &win->mainPanel;
    setPosition(addButton(win, pan, "addEntityBtn", "Add entity", NULL), 0, 0);
    setPosition(addButton(win, pan, "addTriggerBtn", "Add trigger     ", NULL), 1, 0);
    setPosition(addButton(win, pan, "closeBtn", "Close", close), 2, 0);

    // Define new project alert win
    win = createWindow("newProjectAlert", sweStyle);
    pan = &win->mainPanel;
    setPosition(addText(win, pan, "title", "Create new project?", 0), 0, 0);
    setPosition(addText(win, pan, "alertText",
        "Creating a new project will discard the current one. Continue?", 300), 1, 0);
    setPosition(addPanel(win, pan, "buttonsPanel"), 2, 0);
    pan = getItemByTag(win, "buttonsPanel")->data.panel;
    setPosition(addButton(win, pan, "okBtn", "Ok", close), 0, 0);
    setPosition(addButton(win, pan, "cancelBtn", "Cancel", close), 0, 1);
}
