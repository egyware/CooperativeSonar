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
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.geom.AffineTransform;
import java.util.LinkedList;
import javax.swing.JPanel;
import turret.listeners.MapCanvasListener;

/**
 *
 * @author Alumnos
 */
public class MapCanvas extends JPanel
implements MouseListener
{
    private static final AffineTransform identity = new AffineTransform();
    private AffineTransform transform = new AffineTransform();
    private LinkedList<DetectedObject> objects = new LinkedList<>();
    private LinkedList<Turret> turrets = new LinkedList<>();
    private MapCanvasListener listener;
    
    public MapCanvas(MapCanvasListener listener)
    {
        this.listener = listener;
        this.addMouseListener(this);
    }
    
    @Override
    public void paintComponent(Graphics g)
    {
        Graphics2D g2d = (Graphics2D)g;                
        g2d.setTransform(identity);
        
        for(Turret turret:turrets)
        {
            turret.paint(g2d);
        }
    }
    
    
        
//        g2d.setColor(getBackground());
//        g2d.fillRect(0,0,getWidth(), getHeight());
//        
//        
//        g2d.translate(getWidth()/2-8, getHeight()/2-8);
//        g2d.setColor(Color.green);
//        g2d.drawOval(0,0,15,15);
//        g2d.setColor(Color.black);
//        for(DetectedObject obj:objects)
//        {
//            transform.setToTranslation(getWidth()/2, getHeight()/2);
//            transform.rotate(Math.toRadians(obj.angle));
//            transform.translate(obj.distance, 0);
//            g2d.setTransform(transform);            
//            g2d.drawRect(0,0,10,10);transform.setToTranslation(getWidth()/2, getHeight()/2);
//        }
//    }

    

    public void addTurret(Turret turret) 
    {
        turrets.add(turret);
    }

    public void removeTurret(Turret turret) 
    {
        turrets.remove(turret);
    }

    @Override
    public void mouseClicked(MouseEvent e) 
    {    
        for(Turret turret: turrets)
        {
            if(turret.isClicked(e.getX(), e.getY()))
            {
                if(listener != null) listener.turretClicked(turret);
                return;
            }            
        }        
    }

    @Override
    public void mousePressed(MouseEvent e) 
    {    
    }

    @Override
    public void mouseReleased(MouseEvent e) 
    {    
    }

    @Override
    public void mouseEntered(MouseEvent e) 
    {    
    }

    @Override
    public void mouseExited(MouseEvent e) 
    {    
    }
    
}
