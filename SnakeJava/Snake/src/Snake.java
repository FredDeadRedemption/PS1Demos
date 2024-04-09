import java.util.ArrayList;
import java.util.List;

public class Snake extends GameObject {
    Segment[] segmentList = new Segment[100];

    Direction direction = Direction.NONE;

    public Snake(int startPosX, int startPosY) {
        this.x = startPosX;
        this.y = startPosY;
        this.texture = 1;

        segmentList[0] = new Segment(startPosX, startPosY);
    }

    public void grow() {

    }

    public void move() {
        switch (direction) {
            case NONE:
                return;
            case UP:
                this.translate(0, -1);
                break;
            case DOWN:
                this.translate(0, 1);
                break;
            case LEFT:
                this.translate(-1, 0);
                break;
            case RIGHT:
                this.translate(1, 0);
                break;

        }
        //moveSegments();
        return;
    }

    public void moveSegments() {
        if (segmentList.length == 0) {
            return;
        }

        //set head segment to snakes coordinates
        segmentList[0].x = this.x;
        segmentList[0].y = this.y;

        //set rest of segments
        for (int i = segmentList.length-1; i > 0; i--) {
            segmentList[i].x = segmentList[i-1].x;
            segmentList[i].y = segmentList[i-1].y;
        }

    }

}
