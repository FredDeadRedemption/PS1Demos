public class Main {

    public static void main(String[] args) {
        new LifeCycle();
    }
}

class LifeCycle {

    private Snake snake;



    private Food food;
    private Renderer renderer;
    private GameObject[] gameObjectsList = new GameObject[2];

    public LifeCycle() {
        renderer = new Renderer(gameObjectsList);
        Start();
        Update();

    }

    public void Start() {
        Snake snake = new Snake(1, 1);
        Food food = new Food(2, 2);

        gameObjectsList[0] = snake;
        //System.out.println(snake.x);


        gameObjectsList[1] = food;

        renderer.renderGame();
        System.out.println();

        snake.direction = Direction.UP;
        snake.move();

        renderer.renderGame();


    }
    public void Update() {

        final int TARGET_FPS = 60;
        final long OPTIMAL_TIME = 1000000000 / TARGET_FPS; // Nanoseconds per frame

        long lastLoopTime = System.nanoTime();

        while (true) {
            long now = System.nanoTime();
            long updateLength = now - lastLoopTime;
            lastLoopTime = now;
            double delta = updateLength / ((double)OPTIMAL_TIME);

            // Your loop logic goes here

















            // Adjust loop timing
            try {
                Thread.sleep((lastLoopTime - System.nanoTime() + OPTIMAL_TIME) / 1000000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

    }

    private void addGameObject(GameObject gameObject) {

    }


}
