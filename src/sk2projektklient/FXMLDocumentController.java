/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package sk2projektklient;

import Klasy.Funkcje;
import Klasy.Grupa;
import Klasy.Polaczenie;
import Klasy.Uzytkownik;
import Klasy.Watek;
import java.awt.event.MouseEvent;
import java.net.InetAddress;
import java.net.URL;
import java.net.UnknownHostException;
import java.util.Arrays;
import java.util.List;
import java.util.ResourceBundle;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
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
import javafx.scene.control.TextArea;
import javafx.scene.control.TextField;
import javafx.stage.Stage;
import javafx.stage.WindowEvent;

/**
 * Główne okno klienta
 * @author Anka
 */
public class FXMLDocumentController implements Initializable {
    private Polaczenie polaczenie;
    private Watek watek;
    private Grupa grupa;
    private Uzytkownik uzytkownik;
    @FXML
    private Label informacje;
    @FXML
    private Button button;
    @FXML
    private ListView listaUzytkownikow;
    @FXML
    private ListView listaPokoi;
    @FXML
    private Label label;
    @FXML
    private Label obecnaGrupa;
    @FXML
    private Button odswierzUzytkownikow;
    @FXML
    private Button utworzPokoj;
    @FXML
    private TextField nickText;
    @FXML
    private Button WylogujSie;
   
    @FXML
    private TextArea trescWiadomosci;
    @FXML
    private TextArea oknoWiadomosci;
    @FXML
    private Button przyciskWyslij;
    private Funkcje funkcje;
    Watek nowyWatek;
    public FXMLDocumentController() throws UnknownHostException {
      ///  this.polaczenie=new Polaczenie(InetAddress.getByName("192.168.0.15"),1234);
        this.polaczenie=null;
       // this.watek=new Watek();
    }
    
    @FXML 
    void handleWylogujSie(ActionEvent event){
        this.polaczenie.println("7");
        String odbior=this.polaczenie.readLine();
        if(odbior.equals("11")){
            this.polaczenie.close();
            nowyWatek.getWatek().interrupt();
            nowyWatek.getWatek().stop();}
    }
    @FXML 
    void handleWyslijWiadomosc(ActionEvent event){
     Polaczenie polaczenie = null;
        try {
                polaczenie = new Polaczenie(InetAddress.getByName("192.168.0.15"),1234);
                polaczenie.println("6");
               // System.out.println("\""+this.uzytkownik.getNick()+"\"");
                String odbior=polaczenie.readLine(); 
                System.out.println(odbior);
                if (odbior.equals("11")) {
                    System.out.println("jestem w if");
                    System.out.println("\""+this.nickText.getText()+"\"");
                    polaczenie.println("\""+this.nickText.getText()+"\"");
                    System.out.println("czesc");
                    odbior=polaczenie.readLine();
                    
                                    if(odbior.equals("12")){
                                    System.out.println("odbior2");
                                    polaczenie.println("\""+this.obecnaGrupa.getText()+".txt\"");
                                    odbior=polaczenie.readLine();
                                    
                                    if(odbior.equals("13")){
                                        System.out.println("odbior3");
                                        this.oknoWiadomosci.appendText(this.nickText.getText()+" : "+this.trescWiadomosci.getText());
                                        this.polaczenie.println("\""+this.trescWiadomosci.getText()+"\"");
                                        
                                    }
                                }
                    this.trescWiadomosci.getText();
                        }
              polaczenie.close();
                }
         catch (Exception ex) {
             polaczenie.close();
        }   
    }
    
    /**
     * Funkcja obsługująca klawisz "Odśwież listę pokoi" 
     * @param event 
     */
    @FXML
    void handleZaladujListePokoi(ActionEvent event) {
        Polaczenie polaczenie = null;
        try {
                polaczenie = new Polaczenie(InetAddress.getByName("192.168.0.15"),1234);
                polaczenie.println("3");
                String odbior=polaczenie.readLine();
                if (odbior.equals("11")) {
                    String uzyt = "";
                        uzyt=polaczenie.readLine();
                        String uzyt2=  uzyt.replace(" ","");
                        String [] uzyt3=uzyt2.split("\t");
                        List<String> uzyt4=Arrays.asList(uzyt3);
                        listaPokoi.setItems(FXCollections.observableArrayList(uzyt4));
                        }
             polaczenie.close();
                }
         catch (Exception ex) {
             polaczenie.close();
        }
    } 
    /**
     * Funkcja obsługująca klawisz "Utwórz pokój" - otwiera Grupy.fxml wewnętrzne okno
     * @param event 
     */
    @FXML
    void handleUtworzPokoj(ActionEvent event) {
            try{
            FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("Grupy.fxml"));
            Parent root1=(Parent) fxmlLoader.load();
            Stage stage=new Stage();
            stage.setTitle("Pokoje");
            stage.setScene(new Scene(root1));
            stage.show();
        }catch (Exception e){
            System.out.println("Nie można załadować okna");
        }
    }
    /**
     * Funkcja obsługująca klawisz "Odśwież listę klientów" 
     * @param event 
     */
    @FXML
    void handleZaladujListeUzytkownikow(ActionEvent event) {
        Polaczenie polaczenie = null;
        try {
               polaczenie = new Polaczenie(InetAddress.getByName("192.168.0.15"),1234);
                polaczenie.println("2");
                String odbior=polaczenie.readLine();
                if (odbior.equals("11")) {
                    String uzyt = "";
                        uzyt=polaczenie.readLine();
                        String uzyt2=  uzyt.replace(" ","");
                        String [] uzyt3=uzyt2.split("\t");
                        List<String> uzyt4=Arrays.asList(uzyt3);
                        listaUzytkownikow.setItems(FXCollections.observableArrayList(uzyt4));
                        }
             polaczenie.close();
                }
         catch (Exception ex) {
             polaczenie.close();
        }
    } 
   
    //zaloguj sie
    @FXML
    void handleButtonAction(ActionEvent event) {
       // Polaczenie polaczenie = null;
        try {
               this.polaczenie = new Polaczenie(InetAddress.getByName("192.168.0.15"),1234);
           this.polaczenie.println("1");
            String czyOdbior= this.polaczenie.readLine();
            if (czyOdbior.equals("11")) {
                this.polaczenie.println("\""+nickText.getText()+"\"");
                String odbior=this.polaczenie.readLine();
                if(odbior.equals("100")){
                    informacje.setText("Nie udało się zalogować. Taki nick juz istnieje");
                }else if(odbior.equals("101")) {
                    informacje.setText("Udało się zalogować poprawnie");
                    this.uzytkownik.setNick(nickText.getText());
                    nowyWatek=new Watek(this.polaczenie,this.oknoWiadomosci);
                    Thread th=new Thread(nowyWatek);
                    th.start();
                    nowyWatek.setWatek(th);
                }
                }
           // polaczenie.close();
            
        } catch (Exception ex) {
           this.polaczenie.close();
    }}
      public Uzytkownik getUzytkownik() {
        return uzytkownik;
    }
    
    public void setUzytkownik(Uzytkownik uzytkownik) {
        this.uzytkownik = uzytkownik;
    }
    @Override
    public void initialize(URL url, ResourceBundle rb) {
        //this.uzytkownik=new Uzytkownik("");
        listaPokoi.getSelectionModel().selectedItemProperty().addListener(new ChangeListener<String>(){
            @Override
            public void changed(ObservableValue<? extends String> observable, String oldValue, String newValue) {
                //System.out.println(newValue);
                obecnaGrupa.setText(newValue);
                //System.out.println(newValue);
                Polaczenie polaczenie = null;
        try {
              polaczenie = new Polaczenie(InetAddress.getByName("192.168.0.15"),1234);
               polaczenie.println("4");
               String odbior=polaczenie.readLine();
                if (odbior.equals("11")) {
                    polaczenie.println("\""+newValue+".txt\"");
                    String uzytPok = "";
                        uzytPok=polaczenie.readLine();
                        String uzytPok2=  uzytPok.replace(" ","");
                        String [] uzytPok3=uzytPok2.split("\t");
                        List<String> uzytPok4=Arrays.asList(uzytPok3);
                       // System.out.println(uzytPok4);
                        listaUzytkownikow.setItems(FXCollections.observableArrayList(uzytPok4));
                        }
              //  System.out.println("zaraz wylacze");
              polaczenie.close();}
         catch (Exception ex) {
             polaczenie.close();
        }    
            }
        });
    }    
    
}
