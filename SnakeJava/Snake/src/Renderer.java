public class Renderer {
    private final int width = 20;
    private final int height = 15;

    public GameObject[] gameObjectsList;

    int noget = 0;


    public Renderer(GameObject[] objects) {
        gameObjectsList = objects;
    }

    private void updateBoard() {

    }

    public void renderGame(){

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++){
                boolean objectFound = false;

                for (GameObject gameObject : gameObjectsList) {


                        if (gameObject.x == x && gameObject.y == y) {
                            System.out.print(" " + gameObject.texture + " ");
                            objectFound = true;
                            break;
                        }

                }
                if(!objectFound) {
                    System.out.print(" " + noget + " ");
                }

            }
            System.out.println();
        }
    }

}
