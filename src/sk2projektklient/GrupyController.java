/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package sk2projektklient;

import Klasy.Polaczenie;
import java.net.InetAddress;
import java.net.URL;
import java.util.Arrays;
import java.util.List;
import java.util.ResourceBundle;
import javafx.collections.FXCollections;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.control.ListView;
import javafx.scene.control.TextField;

/**
 * FXML Controller class
 *
 * @author Anka
 */
public class GrupyController implements Initializable {
    @FXML
    private Button dodajDoPokoju;

    @FXML
    private ListView uzytkownicyNalezacy;

    @FXML
    private Button zatwierdz;

    @FXML
    private TextField nazwaPokoju;

    @FXML
    private ListView uzytkownicy;

    @FXML
    private TextField nickAdministratora;
    @FXML
    void handleDodaj(ActionEvent event) {}
    @FXML 
    void handleZatwierdz (ActionEvent event){}

    /**
     * Initializes the controller class.
     */
    @Override
    public void initialize(URL url, ResourceBundle rb) {
        // TODO
        Polaczenie polaczenie = null;
        try {
                polaczenie = new Polaczenie(InetAddress.getByName("192.168.0.15"),1234);
                System.out.println("Polaczenie");
                polaczenie.println("2\n");
                String odbior=polaczenie.readLine();
                if (odbior.equals("11")) {
                    System.out.println("jestem w if");
                    String uzyt = "";
                        System.out.println("xxxx");
                        uzyt=polaczenie.readLine();
                        System.out.println("odebralem: "+uzyt);
                        System.out.println("siemandero");
                        String uzyt2=  uzyt.replace(" ","");
                        String [] uzyt3=uzyt2.split("\t");
                        List<String> uzyt4=Arrays.asList(uzyt3);
                        uzytkownicy.setItems(FXCollections.observableArrayList(uzyt4));
                        }
                System.out.println("zaraz wylacze");
              polaczenie.close();
                }
         catch (Exception ex) {
             polaczenie.close();
           // Logger.getLogger(Sk2ProjektKlient.class.getName()).log(Level.SEVERE, null, ex);
        }
    }    
    
}
