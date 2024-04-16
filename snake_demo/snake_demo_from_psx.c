#include <sys/types.h>	// This provides typedefs needed by libgte.h and libgpu.h
#include <stdio.h>	// Not necessary but include it anyway
#include <libetc.h>	// Includes some functions that controls the display
#include <libgte.h>	// GTE header, not really used but libgpu.h depends on it
#include <libgpu.h>	// GPU library header
#include <libapi.h>     // API header, has InitPAD() and StartPAD() defs
#include <stdlib.h>


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


//default stuff
//global pad, figure out how to add more then 1
PADTYPE *pad;
pad = (PADTYPE*)padbuff[0];

typedef struct GameObject {
    int x;
    int y;
    int size;

} GameObject_t;

void GameObject_Translate(GameObject_t* object, int x, int y) {
    object->x = x;
    object->y = y;
}



//snake stuff
//struct prototypes
typedef struct Snake Snake_t;
typedef struct Food Food_t;


struct Snake {
    Food_t food;
    int length;
    GameObject_t gameObject;
};

void Snake_moveSegments() {
    //move them
}

void Snake_Start(Snake_t* snake) {
    snake->gameObject.x = 16;
    snake->gameObject.y = 16;
    snake->gameObject.size = 16;

}

void Snake_Update(Snake_t* snake) {
    if( pad->stat == 0 ) {
        if( ( pad->type == 0x4 ) ||
            ( pad->type == 0x5 ) ||
            ( pad->type == 0x7 ) ) {
            if (!(pad->btn & PAD_UP)) {
                GameObject_Translate(&snake->gameObject, -16, 0);
            }
        }
    }
    Snake_moveSegments();
}

void Snake_Initialize(Snake_t* snake) {
    snake->length = 10;
}

Snake_t Snake_Constructor(int length, int size) {
    Snake_t* snake;
    snake->gameObject.size = size;
    snake->length = length;
}

typedef struct Food {
    Snake_t snake;
    GameObject_t gameObject;
} Food_t;

void Food_Start(Food_t* food) {

}

void Food_Update(Food_t* food) {

}

void Food_Initialize(Food_t* food) {

}







void start() {





}


void renderGameObject(gameObject_t* object) {
    tile = (TILE*)nextpri;      // Cast next primitive
    setTile(tile);              // Initialize the primitive (very important)

    setXY0(tile, object->position.x, object->position.y);       // Set primitive (x,y) position
    setWH(tile, object->textureSize.width, object->textureSize.height);        // Set primitive size
    setRGB0(tile, object->color.r, object->color.g, object->color.b); // Set color yellow
    addPrim(ot[db], tile);      // Add primitive to the ordering table

    nextpri += sizeof(TILE);    // Advance the next primitive pointer
}

void renderGameObjects(gameObject_t* objects[], int count) {
    for(int i = 0; i < count; i++) {
        renderGameObject(objects[i]);
    }
}



void renderSnake(Snake_t* snake) {
    for(int i = 0; i < snake->length; i++) {
        color_t current_color;
        if (i % 2 == 1) {
            current_color = WHITE;
        } else {
            current_color = GREEN;
        }
        gameObject_t gameObject = {snake->segments[i].current.x, snake->segments[i].current.y, snake->base.textureSize.width, snake->base.textureSize.height, current_color};
        renderGameObject(&gameObject);
    }
}



int main() {
    int pos_x,pos_y;


    init();

    // Main loop
    while(1) {
        counter++;
        // Parse controller input




        // Only parse inputs when a controller is connected
        if( pad->stat == 0 ) {
            // Only parse when a digital pad,
            // dual-analog and dual-shock is connected
            if( ( pad->type == 0x4 ) ||
                ( pad->type == 0x5 ) ||
                ( pad->type == 0x7 ) ) {
                if (!(pad->btn & PAD_UP) && direction != DOWN) {
                    direction = UP;
                } else if (!(pad->btn & PAD_DOWN) && direction != UP) {
                    direction = DOWN;
                }

                if (!(pad->btn & PAD_LEFT) && direction != RIGHT) {
                    direction = LEFT;
                } else if (!(pad->btn & PAD_RIGHT) && direction != LEFT) {
                    direction = RIGHT;
                }
                if( !(pad->btn&PAD_CROSS) && (counter % 16 == 0)) { // test CROSS

                    growSnake(&snake);
                    //printf("CROSS \n");
                }
            }
        }

        if (counter % 16 == 0) {
            counter = 0;
            switch (direction) {
                case UP:
                    //snake->base.position.y -= speed;
                    gameObjects[0]->position.y -= speed;
                    //moveSnake(&snake, 0, -speed);
                    break;
                case DOWN:
                    //snake->base.position.y += speed;
                    gameObjects[0]->position.y += speed;
                    //moveSnake(&snake, 0, speed);
                    break;
                case RIGHT:
                    //snake->base.position.x += speed;
                    gameObjects[0]->position.x += speed;
                    //moveSnake(&snake, speed, 0);
                    break;
                case LEFT:
                    //snake->base.position.x -= speed;
                    gameObjects[0]->position.x -= speed;
                    //moveSnake(&snake, -speed, 0);
                    break;
                case NONE:
                    pos_x = pos_y = 48;
                    break;
            }

            if (foodCollision(&snake, gameObjects, size_gameObjects)) {
                growSnake(&snake);
                moveFood(&food);
            }
            moveSnake(&snake);
        }



        ClearOTagR(ot[db], OTLEN);  // Clear ordering table

        renderGameObjects(gameObjects, size_gameObjects);

        FntFlush(-1);
        display();
    }
    return 0;
}



