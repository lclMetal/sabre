void close(Window *win, WindowItem *item)
{
    if (item->type == GEUI_Button)
        closeWindow(win);
}

void openOtherWindow(Window *win, WindowItem *item)
{
    openWindow("second", GEUI_XY_MOUSE_POSITION);
}

void setRandomColor(Window *win, WindowItem *item)
{
    colorActorByName("f", createRGB(rand(256), rand(256), rand(256), 1));
}

void toggleTransparency(Window *win, WindowItem *item)
{
    if (getclone(win->parentCName)->transp > 0)
        getclone(win->parentCName)->transp = 0;
    else
        getclone(win->parentCName)->transp = 0.5;

    if (getclone(getWindowByTag("tag")->parentCName)->transp > 0)
        getclone(getWindowByTag("tag")->parentCName)->transp = 0;
    else
        getclone(getWindowByTag("tag")->parentCName)->transp = 0.5;


    if (getclone(getWindowByTag("win3")->parentCName)->transp > 0)
        getclone(getWindowByTag("win3")->parentCName)->transp = 0;
    else
        getclone(getWindowByTag("win3")->parentCName)->transp = 0.5;
}

void movePacman(Window *win, WindowItem *item)
{
    getclone("pac")->xvelocity = 0.5;
}

void startTestMode(Window *win, WindowItem *item)
{
    getclone("gridCanvas")->var = !getclone("gridCanvas")->var;
if (getclone("gridCanvas")->var)
{
    if (convertChunkDataToMap())
    {
        if (testMapEdges())
        {
            guiItemFloatActor("screenWindow", UPPER_LEFT, getclone("view")->width * 0.5, getclone("view")->height * 0.5 + 10);
            CreateActor("testScreen", "icon", "(none)", "(none)", getclone("screenWindow")->x, getclone("screenWindow")->y, true);
            testScreenTheater(ON);
        }
        else
        {
            getclone("gridCanvas")->var = 0;
            freeMap();
        }
    }
    else
    getclone("gridCanvas")->var = 0;
}
else
{
    freeMap();
    guiItemFloatActor("screenWindow", UPPER_LEFT, -(getclone("screenWindow")->width * 0.5 + getclone("view")->width * 0.75), getclone("view")->height * 0.5 + 10);
    DestroyActor("testScreen");
    testScreenTheater(OFF);
}

closeWindow(win);

}

void exit(Window *win, WindowItem *item)
{
    quitGEUI();
    deleteRenderChunkList();
    freeMap();
    refresh = 1;
    ExitGame();
}

void exitos(Window *win, WindowItem *item)
{
    destroyWindowList();
    ExitGame();
}

void setSP(Window *win, WindowItem *item)
{
    WindowItem *input = getItemByTag(win, "embed");
 
    if (input)
    {
    setSpawnPointPosition(0.0f, atof(getclone(input->data.embedder.actorCName)->text));
    refresh = 1;
    }
    closeWindow(win);
}