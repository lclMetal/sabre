void populateTextureConfigWindow(SABRE_Texture *txr)
{
    char temp[256];
    Window *window = getWindowByTag(SWE_WINDOW_TXR_CONFIG);
    WindowItem *item = NULL;

    if ((item = getItemByTag(window, "title")) && item->type == GEUI_Text)
    {
        sprintf(temp, "\nTexture \"%s\" configuration", txr->name);
        setTextContent(item, temp);
        // setTextText(&item->data.text, temp);
    }

    if ((item = getItemByTag(window, "chkAttribIsWindow")) && item->type == GEUI_Checkbox)
    {
        setCheckboxState(item, txr->isWindow);
        // item->data.checkbox.state = txr->isWindow;
    }

    openWindow(SWE_WINDOW_TXR_CONFIG, GEUI_WPOS_SCREEN_CENTER);
}

void applyTextureConfigChanges()
{
    SABRE_Texture *txr = NULL;
    txr->isWindow = getCheckboxState(item);
}
