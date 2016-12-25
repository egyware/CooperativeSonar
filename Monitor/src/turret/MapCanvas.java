/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package turret;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.List;
import java.awt.geom.AffineTransform;
import java.util.LinkedList;
import javax.swing.JPanel;

/**
 *
 * @author Alumnos
 */
public class MapCanvas extends JPanel
{
    private static final AffineTransform identity = new AffineTransform();
    private AffineTransform transform = new AffineTransform();
    LinkedList<DetectedObject> objects = new LinkedList<>();
    
    public void setMap(String map)
    {
//        objects.clear();
//        map = map.trim();
//        String[] objs = map.split(":");
//        int i=0;
//        for(String obj:objs)
//        {
//            if(obj.isEmpty()) continue;
//            String data[] = obj.split(" ");
//            DetectedObject object = new DetectedObject(Integer.parseInt(data[1]), Integer.parseInt(data[2]));
//            objects.add(object);            
//        }        
    }
    
    @Override
    public void paintComponent(Graphics g)
    {
        Graphics2D g2d = (Graphics2D)g;                
        g2d.setTransform(identity);
        g2d.setColor(getBackground());
        g2d.fillRect(0,0,getWidth(), getHeight());
        
        
        g2d.translate(getWidth()/2-8, getHeight()/2-8);
        g2d.setColor(Color.green);
        g2d.drawOval(0,0,15,15);
        g2d.setColor(Color.black);
        for(DetectedObject obj:objects)
        {
            transform.setToTranslation(getWidth()/2, getHeight()/2);
            transform.rotate(Math.toRadians(obj.angle));
            transform.translate(obj.distance, 0);
            g2d.setTransform(transform);            
            g2d.drawRect(0,0,10,10);transform.setToTranslation(getWidth()/2, getHeight()/2);
        }
    }
    
}
