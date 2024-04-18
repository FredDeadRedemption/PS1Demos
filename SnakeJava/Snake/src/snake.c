#define MAX_OBJECTS

int number_of_classes = 2;

//PADTYPE *pad;
//pad = (PADTYPE*)padbuff[0];

typedef struct GameObject {
    int x;
    int y;
    int size;

} GameObject_t;

GameObject_t* gameObject_list[256];

gameObject_list_size = 0;

void GameObject_Add(GameObject_t* object) {
    gameObject_list[gameObject_list_size] = object; // Store the pointer to the object
    gameObject_list_size++;
}

void GameObject_Translate(GameObject_t* object, int x, int y) {
    object->x = x;
    object->y = y;
}

int GameObject_Is_Colliding(GameObject_t object1, GameObject_t object2) {
    //check collision with size and x and y
    return 0;
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
    // if( pad->stat == 0 ) {
    //     if( ( pad->type == 0x4 ) ||
    //         ( pad->type == 0x5 ) ||
    //         ( pad->type == 0x7 ) ) {
    //         if (!(pad->btn & PAD_UP)) {
    //             GameObject_Translate(&snake->gameObject, -16, 0);
    //         }
    //     }
    // }
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
    food->gameObject.x = 16;
    food->gameObject.y = 16;
    food->gameObject.size = 16;

}

void Food_Update(Food_t* food) {
    if (GameObject_Is_Colliding(food->gameObject, food->snake.gameObject)) {
        food->gameObject.x = 1;
        food->gameObject.y = 1;
    }

}

void Food_Initialize(Food_t* food) {
    food->snake = Snake_Constructor(8, 8);
    
}

//main lifecycle functions
void Start() {
    Snake_t snake;
    GameObject_Add(&snake.gameObject);

    Food_t food;
    GameObject_Add(&food.gameObject);

    food.snake = snake;
    snake.food = food;

}



void Update() {
    

}

void GameObject_Update() {

}