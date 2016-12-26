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
        
        MonitorWindow view = new MonitorWindow();
        
        TurretServer server = new TurretServer(view);
        server.start();
        
        /* Create and display the form */
        java.awt.EventQueue.invokeLater(() -> {
            view.setVisible(true);
        });
    }
    
}
