#define MENU_ACTIONS 3
#define NONE 0
#define PEN 1
#define LINE 2
#define RECTANGLE 3
#define ELLIPSE 4
#define BORDER_TOOL 5
#define NOT_CLICKED 0
#define CLICKED 1
#define HUE 0
#define SATURATION 1
#define BRIGHTNESS 2

int clickX;
int clickY;
int clicked;
int firstClick = NOT_CLICKED;
int secondClick = NOT_CLICKED;
int screenWidth;
int screenHeight;
// |-- SABRE template project related not crucial variables starts here...
int textureDrawCalls = 0;
int spriteDrawCalls = 0;
char nightmareMode = 0;
// ...and ends here --|

int draw;
int refresh;
int wait;
int currentTexture = 0;
int paintTexture = 0;
int selectedTool;
int renderMode = 0;
int HSBIndicatorIndex[3] = {-1, -1, -1};

double zoom;
double viewX;
double viewY;
double viewEndX;
double viewEndY;
double moveSpeed = 1.0;
double waitTimeMultiplier = 0.5;
double darkenTransp = 0;

char tools[6][256] =
{
    "None",
    "Pen",
    "Line",
    "Rectangle",
    "Ellipse",
    "Border tool"
};

//SABRE engine variables
int sabreMapWidth, sabreMapHeight;
float sabreScreenWidth, sabreScreenHeight; //These represent the dimensions of the screen.

int quality; //This variable is used to adjust the quality by decreasing the amount of skips.
float EPSILON; //This is used as a minimum number for some dividings to avoid illegal division by 0.
float walkSpeed; //This represents the moving speed.
float rotateSpeed; //This represents the rotation speed.
float cameraX; //This represents the x-coordinate on the camera plane.
float perpWallDist; //This represents the perpendicular distance from the wall (length of the ray).
float lineHeight; //This represents the height of the current line of wall to be drawn.
float wallX; //This represents the exact x position where the wall was hit.
float posX, posY; //These represent the position of the player.
float dirX, dirY; //These represent the direction of the player.
float planeX, planeY; //These represent the camera plane of the player.
float rayPosX, rayPosY; //These represent the position of the ray.
float rayDirX, rayDirY; //These represent the direction of the ray.
float deltaDistX, deltaDistY; //These represent the distance from previous x- or y-side to the next.
float sideDistX, sideDistY; //These represent the distance from the ray's current position to the
                            //next x-side and y-side
double walkSpeedSet;
double rotateSpeedSet;

int checkX; //This represents the current x position of the check.
float wallHit; //This represents if a wall has been hit or not.
float hitSide; //This represents from what side the wall was hit (0 for x-side, 1 for y-side).
float texNum; //This represents the number of the texture to use.
float texX; //This represents the vertical line of the texture to be drawn.
float texWidth; //This represents the width of the texture.
float rayMapX, rayMapY; //These represent the coordinates of the square which the ray is in.
float rayStepX, rayStepY; //These represent the direction to step into on x or y-direction (-1 or +1).
