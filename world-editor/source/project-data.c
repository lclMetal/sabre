typedef struct TextureConfigStruct
{
    unsigned id;
    char name[SWE_TEXTURE_NAME_MAX_LENGTH + 1];
    bool isWindow;
}TextureConfig;


#define SABRE_DATA_STORE_AS_TXRCFG_ARRAY(DATA_STORE) SABRE_DATA_STORE_AS_CAST_ARRAY(DATA_STORE, (TextureConfig *))
#define SABRE_TXRCFG_POINTER_CAST(POINTER) ((TextureConfig *)POINTER)

SABRE_DataStore txrCfgStore;
TextureConfig *textureConfigs = NULL;

int initTextureConfigs();
int addTextureConfig(unsigned id, char name[SWE_TEXTURE_NAME_MAX_LENGTH + 1], bool isWindow);
void addTextureConfigToDataStore(SABRE_DataStore *dataStore, void *tc);
void freeTextureConfigStore();

int initTextureConfigs()
{
    int err = 0;
    unsigned texIndex = 2;
    size_t texNameLen;
    char animName[256];

    SABRE_SetDataStoreAddFunc(&txrCfgStore, addTextureConfigToDataStore);
    txrCfgStore.elemSize = sizeof(TextureConfig);
    err = SABRE_PrepareDataStore(&txrCfgStore);

    if (err) return err;

    textureConfigs = SABRE_DATA_STORE_AS_TXRCFG_ARRAY(txrCfgStore);

    if (strcmp(name, "SABRE_TextureActor") != 0)
    {
        DEBUG_MSG_FROM("This function must be called from an event of SABRE_TextureActor", "initTextureConfigs");
        return 1; // Function called from invalid event
    }

    strcpy(animName, getAnimName(texIndex));
    texNameLen = strlen(animName);

    while (texNameLen)
    {
        err = addTextureConfig(texIndex, animName, SABRE_StringEndsWith(animName, "-window"));

        if (err) return err;

        strcpy(animName, getAnimName(++texIndex));
        texNameLen = strlen(animName);
    }

    return 0;
}

int addTextureConfig(unsigned id, char name[SWE_TEXTURE_NAME_MAX_LENGTH + 1], bool isWindow)
{
    TextureConfig newTxrCfg;

    newTxrCfg.id = id;
    strcpy(newTxrCfg.name, name);
    newTxrCfg.isWindow = isWindow;

    return SABRE_AddToDataStore(&txrCfgStore, &newTxrCfg);
}

void populateTextureConfigWindow(TextureConfig *tc)
{
    char temp[256];
    Window *window = getWindowByTag(SWE_WINDOW_TXR_CONFIG);
    WindowItem *item = NULL;

    if ((item = getItemByTag(window, "title")) && item->type == GEUI_Text)
    {
        sprintf(temp, "\nTexture config: %s", tc->name);
        setTextText(&item->data.text, temp);
    }

    if ((item = getItemByTag(window, "chkAttribIsWindow")) && item->type == GEUI_Checkbox)
    {
        item->data.checkbox.state = tc->isWindow;
    }

    openWindow(SWE_WINDOW_TXR_CONFIG, GEUI_WPOS_SCREEN_CENTER);
}

void applyTextureConfigChanges()
{
    // TODO:
}

void addTextureConfigToDataStore(SABRE_DataStore *dataStore, void *tc)
{
    textureConfigs[dataStore->count] = *SABRE_TXRCFG_POINTER_CAST(tc);
}

void freeTextureConfigStore()
{
    SABRE_FreeDataStore(&txrCfgStore);
    textureConfigs = NULL;
}
