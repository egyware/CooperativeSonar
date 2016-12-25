package turret;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.HashMap;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Edgardo
 */
public class TurretServer extends Thread
{
    
    private int turrets;
    private final HashMap<String, Turret> turret = new HashMap<>();
    

    @Override
    public void run() 
    {
        ServerSocket server;
        try
        {
            server = new ServerSocket(7500);
        
            while(true)
            {
                Socket socket = server.accept();
                Turret turret = new Turret("Turret"+(turrets++), socket);
                System.out.println(String.format("Se conecta la torreta %s una nueva torreta", turret.getTurretName()));
                turret.start();
            }
        
        }
        catch (IOException ex) 
        {
            Logger.getLogger(TurretServer.class.getName()).log(Level.SEVERE, null, ex);
        }
        

    }    
}
