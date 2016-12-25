package turret;

/**
 *
 * @author edgardo.moreno
 */
public class DetectedObject
{
    public int distance;
    public int angle;
    public int angleWidth;

    public DetectedObject(int d, int a, int angleWidth) 
    {
        this.distance = d;
        this.angle = a;        
    }
}
