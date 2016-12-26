package turret;

import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.geom.AffineTransform;
import java.io.IOException;
import java.io.PrintStream;
import java.net.Socket;
import java.util.Scanner;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Edgardo
 */
public class Turret extends Thread
{
    private String name;
    private Socket socket;
    private Scanner scanner;
    private PrintStream print;
    private int x=100,y=100;
    private int angle;
    private AffineTransform t;
    private final TurretServer server;
  
    public Turret(TurretServer server, String name, Socket socket) 
    {
        super(name);
        this.server = server;
        this.name = name;
        this.socket = socket;
        t = new AffineTransform();
        
    }

    @Override
    public void run() 
    {
        try
        {
            scanner = new Scanner(socket.getInputStream());        
            print = new PrintStream(socket.getOutputStream());
            
            print.println(":NOMBRE "+name);
            while(scanner.hasNextLine())
            {
                String command = scanner.nextLine();
                System.out.println(command);
            }
        }
        catch (IOException ex)
        {
            Logger.getLogger(Turret.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    
    //public methods
    public void setPosition(int x, int y)
    {
        this.x = x;
        this.y = y;
        print.println(":POSICION " + x + " " + y);
    }
    
    public void setRotation(int angle)
    {
        this.angle = angle;
        print.println(":ROTACION "+angle);
    }
    
    public void setTurretName(String name)
    {
        this.name = name;
        print.println(":NOMBRE " + x + " " + y);
    }
    
    public void paint(Graphics2D g)
    {
        t.setToIdentity();
        t.translate(x, y);
        t.rotate(Math.toRadians(angle));        
        t.translate(-16, -16);
        g.setTransform(t);
        
        g.setColor(Color.green);
        g.drawOval(8,8, 16,16);
        g.setColor(Color.blue);
        g.drawLine(16, -8, 16, 24);
        g.drawLine(8, 16, 24, 16);
        g.setColor((Color.black));        
        g.drawString(name, -g.getFontMetrics().stringWidth(name)/2+16, 40);
        
    }
    
    public boolean isClicked(int mx, int my)
    {        
        return (mx-x+8)*(mx-x+8) + (my-y+8)*(my-y+8) <= 64;        
    }

    public String getTurretName() 
    {
        return name;
    }

    public int getX() 
    {
        return x;
    }

    public int getY() 
    {
        return y;
    }

    public int getRotation() 
    {
        return angle;
    }
}
