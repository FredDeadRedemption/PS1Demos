public class Main {



    public static void main(String[] args) {
        LifeCycle lifeCycle = new LifeCycle();
        
    }
}


public class LifeCycle {
    public void Start() {
        System.out.print("Hej");
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
            System.out.println("hello 2");

            // Adjust loop timing
            try {
                Thread.sleep((lastLoopTime - System.nanoTime() + OPTIMAL_TIME) / 1000000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

    }

    public LifeCycle() {
        Start();
        Update();
    }
}





