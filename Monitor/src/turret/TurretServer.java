package turret;

import turret.listeners.TurretServerListener;
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
    
    private int turretCounter;
    private final HashMap<String, Turret> turrets = new HashMap<>();
    private TurretServerListener listener;
    
    public TurretServer(TurretServerListener listener)
    {
        this.listener = listener;
    }

    @Override
    public void run() 
    {
        ServerSocket server;
        try
        {
            server = new ServerSocket(7500);
            
            listener.connectedTurret(new Turret(this, "asdasda", null));
        
            while(true)
            {
                Socket socket = server.accept();
                Turret turret = new Turret(this, "Turret"+(turretCounter++), socket);
                System.out.println(String.format("Se conecta la torreta %s una nueva torreta", turret.getTurretName()));
                turrets.put(turret.getTurretName(), turret);
                if(listener != null) listener.connectedTurret(turret);
                turret.start();
            }
        
        }
        catch (IOException ex) 
        {
            Logger.getLogger(TurretServer.class.getName()).log(Level.SEVERE, null, ex);
        }
        

    }    
}
