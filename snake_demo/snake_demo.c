#include <sys/types.h>	// This provides typedefs needed by libgte.h and libgpu.h
#include <stdio.h>	// Not necessary but include it anyway
#include <libetc.h>	// Includes some functions that controls the display
#include <libgte.h>	// GTE header, not really used but libgpu.h depends on it
#include <libgpu.h>	// GPU library header
#include <libapi.h>     // API header, has InitPAD() and StartPAD() defs

#define OTLEN 8         // Ordering table length (recommended to set as a define
// so it can be changed easily)

// Define environment pairs and buffer counter
DISPENV disp[2];
DRAWENV draw[2];
int db = 0;

u_long ot[2][OTLEN]; // Ordering table length
char pribuff[2][32768]; // Primitive buffer
char *nextpri;          // Next primitive pointer


TILE *tile;                     // Primitive pointer

// Pad stuff (omit when using PSn00bSDK)
#define PAD_SELECT      1
#define PAD_L3          2
#define PAD_R3          4
#define PAD_START       8
#define PAD_UP          16
#define PAD_RIGHT       32
#define PAD_DOWN        64
#define PAD_LEFT        128
#define PAD_L2          256
#define PAD_R2          512
#define PAD_L1          1024
#define PAD_R1          2048
#define PAD_TRIANGLE    4096
#define PAD_CIRCLE      8192
#define PAD_CROSS       16384
#define PAD_SQUARE      32768

typedef struct _PADTYPE
{
    unsigned char	stat;
    unsigned char	len:4;
    unsigned char	type:4;
    unsigned short	btn;
    unsigned char	rs_x,rs_y;
    unsigned char	ls_x,ls_y;
} PADTYPE;

// pad buffer arrays
u_char padbuff[2][34];

void init(void) {

    // Reset GPU and enable interrupts
    ResetGraph(0);


    // Configures the pair of DISPENVs for 320x240 mode (NTSC)
    SetDefDispEnv(&disp[0], 0, 0, 320, 240);
    SetDefDispEnv(&disp[1], 0, 240, 320, 240);


    // Configures the pair of DRAWENVs for the DISPENVs
    SetDefDrawEnv(&draw[0], 0, 240, 320, 240);
    SetDefDrawEnv(&draw[1], 0, 0, 320, 240);

    // Specifies the clear color of the DRAWENV
    setRGB0(&draw[0], 63, 0, 127);
    setRGB0(&draw[1], 63, 0, 127);
    // Enable background clear
    draw[0].isbg = 1;
    draw[1].isbg = 1;

    // Apply environments
    PutDispEnv(&disp[0]);
    PutDrawEnv(&draw[0]);


    nextpri = pribuff[0];

    // Initialize the pads
    InitPAD( padbuff[0], 34, padbuff[1], 34 );

    // Begin polling
    StartPAD();

    // To avoid VSync Timeout error, may not be defined in PsyQ
    ChangeClearPAD( 1 );

    // Load the internal font texture
    FntLoad(960, 0);
    // Create the text stream
    FntOpen(0, 16, 320, 224, 0, 100);
}

void display(void) {
    DrawSync(0);
    VSync(0);


    // Apply environments
    PutDispEnv(&disp[db]);
    PutDrawEnv(&draw[db]);

    // Enable display
    SetDispMask(1);

    DrawOTag(ot[db]+OTLEN-1);   // Draw the ordering table

    // Flip buffer counter
    db = !db;
    nextpri = pribuff[db];      // Reset next primitive pointer
}

enum Direction {
    NONE,
    UP,
    RIGHT,
    DOWN,
    LEFT
};



typedef struct position {
    int x;
    int y;
} position_t;

typedef struct textureSize {
    int width;
    int height;
} textureSize_t;

typedef struct color{
    int r;
    int g;
    int b;
} color_t;
color_t RED = {255, 0, 0};
color_t BLUE = {0, 0, 255};
color_t GREEN = {0, 255, 0};
color_t YELLOW = {255, 255, 0};

typedef struct gameObject {
    position_t position;
    textureSize_t textureSize;
    color_t color;
} gameObject_t;

typedef struct snakeBodyPart {
    gameObject_t base;

} snakeBodyPart_t;

typedef struct snake {
    gameObject_t base;
    int length;
    snakeBodyPart_t snakeBodyPart[length];
} snake_t;



typedef struct food {
    gameObject_t base;
    int value : 1;
} food_t;

void renderGameObject(gameObject_t* object) {
    tile = (TILE*)nextpri;      // Cast next primitive
    setTile(tile);              // Initialize the primitive (very important)

    setXY0(tile, object->position.x, object->position.y);       // Set primitive (x,y) position
    setWH(tile, object->textureSize.width, object->textureSize.height);        // Set primitive size
    setRGB0(tile, object->color.r, object->color.g, object->color.b); // Set color yellow
    addPrim(ot[db], tile);      // Add primitive to the ordering table

    nextpri += sizeof(TILE);    // Advance the next primitive pointer
}

void renderGameObjects(gameObject_t objects[], int count) {
    for(int i = 0; i < count; i++) {
        renderGameObject(&objects[i]);
    }
}


int main()
{

    int pos_x,pos_y;
    PADTYPE *pad;
    init();
    enum Direction direction;

    pos_x = pos_y = 64;

    int counter = 0;
    int speed = 16;


    food_t f1 = {32, 32, 8,8, RED};
    food_t f2 = {128, 160, 12,12, RED};

    snake_t snake = {128,128, 16,16, GREEN};
    gameObject_t gameObjects[3];
    gameObjects[0] = snake.base;
    gameObjects[1] = f1.base;
    gameObjects[2] = f2.base;


    // Main loop
    while(1) {
        counter++;
        // Parse controller input
        pad = (PADTYPE*)padbuff[0];

        // Only parse inputs when a controller is connected
        if( pad->stat == 0 ) {
            // Only parse when a digital pad,
            // dual-analog and dual-shock is connected
            if( ( pad->type == 0x4 ) ||
                ( pad->type == 0x5 ) ||
                ( pad->type == 0x7 ) ) {
                if( !(pad->btn&PAD_UP) ) { // test UP
                    direction = UP;
                }
                else if( !(pad->btn&PAD_DOWN) )       // test DOWN
                {
                    direction = DOWN;
                }
                if( !(pad->btn&PAD_LEFT) )          // test LEFT
                {
                    direction = LEFT;
                }
                else if( !(pad->btn&PAD_RIGHT) )    // test RIGHT
                {
                    direction = RIGHT;
                }
            }
        }

        if (counter % 16 == 0) {
            counter = 0;
            switch (direction) {
                case UP:
                    gameObjects[0].position.y -= speed;
                    break;
                case DOWN:
                    gameObjects[0].position.y += speed;
                    break;
                case RIGHT:
                    gameObjects[0].position.x += speed;
                    break;
                case LEFT:
                    gameObjects[0].position.x -= speed;
                    break;
                case NONE:
                    pos_x = pos_y = 48;
                    break;
            }

        }

        ClearOTagR(ot[db], OTLEN);  // Clear ordering table


        renderGameObjects(gameObjects, 3);

        display();
    }
    return 0;
}



