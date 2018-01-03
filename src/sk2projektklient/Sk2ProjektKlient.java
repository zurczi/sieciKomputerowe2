/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package sk2projektklient;

import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

/**
 *
 * @author Anka
 */
public class Sk2ProjektKlient extends Application {
    
    @Override
    public void start(Stage stage) throws Exception {
        Parent root = FXMLLoader.load(getClass().getResource("FXMLDocument.fxml"));
        
        Scene scene = new Scene(root);
        
        stage.setScene(scene);
        stage.show();
    }

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        launch(args);
        //InetAddress addr=InetAddress.getByName("192.168.0.15");
       InetAddress addr = null;//=InetAddress.getByAddress(new byte[]{(byte)192), (byte)168,(byte)0,(byte)15);
        try {
            Polaczenie polaczenie=new Polaczenie(addr.getByName("192.168.0.15"),1236);
            polaczenie.println("127227");
            polaczenie.readLine();
        } catch (UnknownHostException ex) {
            //Logger.getLogger(Sk2ProjektKlient.class.getName()).log(Level.SEVERE, null, ex);
        }
        
    }
    
}
