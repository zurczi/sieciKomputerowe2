/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package sk2projektklient;

import Klasy.Polaczenie;
import java.net.InetAddress;
import java.net.URL;
import java.net.UnknownHostException;
import java.util.ResourceBundle;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.control.TextField;

/**
 * FXML Controller class
 *
 * @author Anka
 */
public class LogowanieController implements Initializable {
    @FXML
    private Button zalogujSie2;
    @FXML
    private TextField nickDoLogowania;
    @FXML
    void handleZalogujSie2(ActionEvent event) {
               InetAddress addr = null;//=InetAddress.getByAddress(new byte[]{(byte)192), (byte)168,(byte)0,(byte)15);
        try {
            Polaczenie polaczenie=new Polaczenie(addr.getByName("192.168.0.15"),1234);
            
            polaczenie.println(nickDoLogowania.getText());
            
            System.out.println( polaczenie.readLine());
           
        
        } catch (UnknownHostException ex) {
            //Logger.getLogger(Sk2ProjektKlient.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    /**
     * Initializes the controller class.
     */
    @Override
    public void initialize(URL url, ResourceBundle rb) {
        // TODO
    }    
    
}
