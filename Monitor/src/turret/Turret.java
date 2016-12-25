package turret;

import java.awt.Graphics;
import java.io.IOException;
import java.io.PrintStream;
import java.net.InetAddress;
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
  
    public Turret(String name, Socket socket) 
    {
        super(name);
        this.name = name;
        this.socket = socket;
        
    }

    @Override
    public void run() 
    {
        try
        {
            scanner = new Scanner(socket.getInputStream());        
            print = new PrintStream(socket.getOutputStream());
            
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
    
    public void paint(Graphics g)
    {
        
    }

    public String getTurretName() 
    {
        return name;
    }
}
