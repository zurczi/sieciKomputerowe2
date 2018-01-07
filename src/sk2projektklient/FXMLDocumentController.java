/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package sk2projektklient;

import Klasy.Funkcje;
import Klasy.Polaczenie;
import java.net.InetAddress;
import java.net.URL;
import java.net.UnknownHostException;
import java.util.Arrays;
import java.util.List;
import java.util.ResourceBundle;
import javafx.collections.FXCollections;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.ListView;
import javafx.stage.Stage;
import javafx.stage.WindowEvent;

/**
 *
 * @author Anka
 */
public class FXMLDocumentController implements Initializable {
    
    @FXML
    private Button button;
    @FXML
    private ListView listaUzytkownikow;
    @FXML
    private Label label;
    @FXML
    private Button odswierzUzytkownikow;
    private Funkcje funkcje;
    @FXML
    void handleZaladujListeUzytkownikow(ActionEvent event) {
        Polaczenie polaczenie = null;
        try {
                polaczenie = new Polaczenie(InetAddress.getByName("192.168.0.15"),1234);
                System.out.println("Polaczenie");
                polaczenie.println("2\n");
                System.out.println("wyslano");
                String odbior=polaczenie.readLine();
                if (odbior.equals("11")) {
                    System.out.println("jestem w if");
                    String uzyt = "";
                        System.out.println("xxxx");
                        uzyt=polaczenie.readLine();
                        System.out.println("odebralem: "+uzyt);
                        System.out.println("siemandero");
                    //String [] uzyt2;
                    //uzyt2=funkcje.funkcja(uzyt);
                    //System.out.println("heheszi");
                    //System.out.println(uzyt2);
                    //List<String> uzyt3=Arrays.asList(uzyt2);
                        String uzyt2=  uzyt.replace(" ","");
                        String [] uzyt3=uzyt2.split("\t");
                        List<String> uzyt4=Arrays.asList(uzyt3);
                        listaUzytkownikow.setItems(FXCollections.observableArrayList(uzyt4));
                        }
                System.out.println("zaraz wylacze");
              polaczenie.close();
                }
         catch (Exception ex) {
             polaczenie.close();
           // Logger.getLogger(Sk2ProjektKlient.class.getName()).log(Level.SEVERE, null, ex);
        }
    } 
    
    @FXML
    void handleButtonAction(ActionEvent event) {
                try{
            //Parent root = FXMLLoader.load(getClass().getResource("fxmldocument2.fxml"));
            FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("Logowanie.fxml"));
            //FXMLLoader fxmlLoader=FXMLLoader.load(getClass().getResource("fxmlLogowanie.fxml"));
            Parent root1=(Parent) fxmlLoader.load();
            Stage stage=new Stage();
            stage.setTitle("Logowanie");
            stage.setScene(new Scene(root1));
            stage.show();
        }catch (Exception e){
            System.out.println("Nie można załadować okna");
        }
    }
  
    @Override
    public void initialize(URL url, ResourceBundle rb) {
        // TODO
    }    
    
}
