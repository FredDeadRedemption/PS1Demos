public abstract class GameObject {
    int x;
    int y;
    //int size
    //colo
    int texture = 9;
    public void setPosition(int x, int y) {
        this.x = x;
        this.y = y;
    }

    public void translate(int x, int y) {
        this.x += x;
        this.y += y;
    }

}
