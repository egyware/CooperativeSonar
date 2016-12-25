package turret;

/**
 *
 * @author Edgardo
 */
public class Program 
{
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) 
    {
        System.setProperty("line.separator", "\r\n");
        
        TurretServer server = new TurretServer();
        server.start();
        
        /* Create and display the form */
        java.awt.EventQueue.invokeLater(() -> {
            new DistanceMapViewer().setVisible(true);
        });
    }
    
}
