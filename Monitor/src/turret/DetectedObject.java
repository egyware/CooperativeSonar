package turret;

/**
 *
 * @author edgardo.moreno
 */
public class DetectedObject
{
    public int distance;
    public int angle;

    DetectedObject(int d, int a) 
    {
        this.distance = d;
        this.angle = a;        
    }
}
