enum SABRE_GameStatesEnum
{
    SABRE_UNINITIALIZED = 0,
    SABRE_RUNNING,
    SABRE_FINISHED
}sabreGameState = SABRE_UNINITIALIZED;

struct SABRE_CameraStruct
{
    float posX, posY;
    float dirX, dirY;
    float planeX, planeY;
};

struct SABRE_KeybindStruct
{
    char forward, backward;
    char turnLeft, turnRight;
    char strafeLeft, strafeRight;
}sabreDefaultKeys =
{
    KEY_w, KEY_s, // forward, backward
    KEY_a, KEY_d, // turn left, right
    KEY_q, KEY_e  // strafe left, right
};

struct SABRE_PlayerStruct
{
    float moveSpeed;
    float turnSpeed;
    struct SABRE_KeybindStruct keys;
    struct SABRE_CameraStruct camera;
}sabrePlayer;

struct SABRE_SliceStruct
{
    short textureSlice;
    short textureNumber;
}sabreSlice;

void SABRE_Quit()
{
    if (sabreGameState != SABRE_FINISHED)
    {
        VisibilityState("SABRE_Screen", DISABLE);
        VisibilityState("SABRE_PlayerController", DISABLE);
        VisibilityState("SABRE_TextureSlice", DISABLE);
        VisibilityState("SABRE_SpriteSlice", DISABLE);
        EventDisable("SABRE_Screen", EVENTALL);
        EventDisable("SABRE_PlayerController", EVENTALL);
        EventDisable("SABRE_TextureSlice", EVENTALL);
        EventDisable("SABRE_SpriteSlice", EVENTALL);
        sabreGameState = SABRE_FINISHED;
    }
}
