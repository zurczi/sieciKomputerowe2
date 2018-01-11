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
import javafx.collections.ObservableList;
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
   // private Watek watek;
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
    private Button wybierzPokoj;
    @FXML
    private TextField nazwaPokoju;
    @FXML 
    private Button dodajDoPokoju;
    @FXML
    private Button zatwierdz;
    @FXML
    private TextArea trescWiadomosci;
    @FXML
    private TextArea oknoWiadomosci;
    @FXML
    private Button przyciskWyslij;
    @FXML 
    private Label pokoje;
    @FXML
    private Label uzytkownicy;
    private Funkcje funkcje;
   // Watek nowyWatek;
    
    public String przygotujWiadomosc(String case1,String nickUzyt){
        String wiadomosc = "";
        String [] sklejanie= new String[125];
               sklejanie[0]=case1;
               String [] nick = nickUzyt.split("");
               int k=100;
               for(int i=1;i<4;i++){
                   sklejanie[i]=String.valueOf(nickUzyt.length()%k);
                           k=k/10;
               }
               for(int i=4;i<nickUzyt.length();i++){
                   sklejanie[i]=nick[i];
               }
               for(int i=nickUzyt.length();i<125;i++){
                   sklejanie[i]=" ";
               }
               wiadomosc=sklejanie.toString();
        return wiadomosc;
    }
    public String przygotujWiadomosc2(String case1){
          String wiadomosc = case1;
      //  String [] sklejanie= new String[125];
               //sklejanie[0]=case1;
               
               for(int i=1;i<125;i++){
                   wiadomosc=wiadomosc+" ";
               }
              // wiadomosc=sklejanie.toString();
        return wiadomosc;
    }
    public FXMLDocumentController() throws UnknownHostException {
      ///  this.polaczenie=new Polaczenie(InetAddress.getByName("192.168.0.15"),1234);
        this.polaczenie=null;
       // this.watek=new Watek();
    }
    /**
     * DODAJ DO POKOJU CASE 5
     */
    @FXML
    void handleDodajDoPokoju(ActionEvent event){
        listaUzytkownikow.getItems().add(listaPokoi.getSelectionModel().getSelectedItem());
        listaPokoi.getItems().remove(listaPokoi.getSelectionModel().getSelectedItem());
    }
    /**
     * POTWIERDZ UTWORZENIE POKOJU CASE 5
     * @param event 
     */
    @FXML
    void handleZatwierdz(ActionEvent event){
        try{
         ObservableList lista=listaUzytkownikow.getItems();
         String listaUzyt;
         listaUzyt=lista.toString();
         String replace = listaUzyt.replace("[","");
         replace = replace.replace("]","");
         replace = replace.replace(",","");
         replace = replace.replace(" ", "\t");
         replace=replace+"\n";
         System.out.println(replace);
        
        String wiadomosc="";
            wiadomosc=wiadomosc+"5";
          String x=this.nazwaPokoju.getText()+".txt";
           
               int dl=x.length();
               String str=String.valueOf(dl);
               int dlstr=str.length();
               if(dlstr==1){
                 wiadomosc=wiadomosc+"00"+str;
               }
                 else{
                  wiadomosc=wiadomosc+"0"+str;
               }
             wiadomosc=wiadomosc+x;
             String nazwaAdmina=this.nickText.getText();
             dl=nazwaAdmina.length();
               str=String.valueOf(dl);
               dlstr=str.length();
               if(dlstr==1){
                 wiadomosc=wiadomosc+"00"+str;
               }
                 else{
                  wiadomosc=wiadomosc+"0"+str;
               }
             wiadomosc=wiadomosc+nazwaAdmina;
             
             
               dl=replace.length();
               str=String.valueOf(dl);
               dlstr=str.length();
               if(dlstr==1){
                 wiadomosc=wiadomosc+"00"+str;
               }
                 else{
                  wiadomosc=wiadomosc+"0"+str;
               }
             wiadomosc=wiadomosc+replace;
             int dlwiadomosci=wiadomosc.length();
               for(int i=dlwiadomosci;i<125;i++){
                   wiadomosc=wiadomosc+" ";
               }
               System.out.println(wiadomosc);
               this.polaczenie.println(wiadomosc);
               System.out.println("wyslalam");
               String odbior=this.polaczenie.readLine();
                    if(odbior.equals("5")){
                        this.informacje.setText("Utworzono nowa grupe");
                        this.nazwaPokoju.setVisible(false);
                        this.dodajDoPokoju.setVisible(false);
                        this.zatwierdz.setVisible(false);
                        this.pokoje.setText("Pokoje");
                        this.pokoje.setText("Uzytkownicy");
                    }else this.informacje.setText("Nie udało się utworzyc nowej grupy");
        
    }catch(Exception e){
        this.polaczenie.close();
    }}
    /**
     * 
     */
    /**
     * WYBOR POKOJU CASE 4
     * @param event 
     */
    @FXML
    void handleWybierzPokoj(ActionEvent event){
        try{
            String wybrany=listaPokoi.getSelectionModel().selectedItemProperty().getValue().toString();
            this.obecnaGrupa.setText(wybrany);
            String wiadomosc="";
            wiadomosc=wiadomosc+"4";
            wybrany=wybrany+".txt";
               int dl=wybrany.length();
               String str=String.valueOf(dl);
               int dlstr=str.length();
               if(dlstr==1){
                 wiadomosc=wiadomosc+"00"+str;
               }
                 else{
                  wiadomosc=wiadomosc+"0"+str;
               }
             wiadomosc=wiadomosc+wybrany;
             int dlwiadomosci=wiadomosc.length();
               for(int i=dlwiadomosci;i<125;i++){
                   wiadomosc=wiadomosc+" ";
               }
               System.out.println(wiadomosc);
               this.polaczenie.println(wiadomosc);
               System.out.println("wyslalam");
               //String odbior=this.polaczenie.readLine();
            //System.out.println(wybrany);
        }catch(Exception e){
            this.polaczenie.close();
        }
    }
    @FXML 
    void handleWylogujSie(ActionEvent event){
       // this.polaczenie.println("7");
       // String odbior=this.polaczenie.readLine();
       // if(odbior.equals("11")){
       String wiad=przygotujWiadomosc("7", nickText.getText());
       this.polaczenie.println(wiad);            
       this.polaczenie.close();
          //  nowyWatek.getWatek().interrupt();
           // nowyWatek.getWatek().stop();}
    }
    @FXML 
    void handleWyslijWiadomosc(ActionEvent event){
            try{

        String wiadomosc="";
            wiadomosc=wiadomosc+"6";
          
            
            
            String x=this.nickText.getText();
           
               int dl=x.length();
               String str=String.valueOf(dl);
               int dlstr=str.length();
               if(dlstr==1){
                 wiadomosc=wiadomosc+"00"+str;
               }
                 else{
                  wiadomosc=wiadomosc+"0"+str;
               }
             wiadomosc=wiadomosc+x;
             this.obecnaGrupa.setText("klub");
             String nazwaGrupy=this.obecnaGrupa.getText()+".txt";
             dl=nazwaGrupy.length();
               str=String.valueOf(dl);
               dlstr=str.length();
               if(dlstr==1){
                 wiadomosc=wiadomosc+"00"+str;
               }
                 else{
                  wiadomosc=wiadomosc+"0"+str;
               }
             wiadomosc=wiadomosc+nazwaGrupy;
             
             
               dl=this.trescWiadomosci.getText().length();
               str=String.valueOf(dl);
               dlstr=str.length();
               if(dlstr==1){
                 wiadomosc=wiadomosc+"00"+str;
               }
               else if(dlstr==2){
                  wiadomosc=wiadomosc+"0"+str;
               }
               else if(dlstr==3){
                  wiadomosc=wiadomosc+str;
               }
             wiadomosc=wiadomosc+trescWiadomosci.getText();
             int dlwiadomosci=wiadomosc.length();
               for(int i=dlwiadomosci;i<125;i++){
                   wiadomosc=wiadomosc+" ";
               }
               System.out.println(wiadomosc);
               this.polaczenie.println(wiadomosc);
               System.out.println("wyslalam");
            
    }catch(Exception e){
        this.polaczenie.close();
    }}
    
    
    /**
     * ODSWIEZ LISTE POKOI CASE 3
     * @param event 
     */
    @FXML
    void handleZaladujListePokoi(ActionEvent event) {
            try{
                System.out.println("jestem");
               String wiadomosc = "3";
               for(int i=1;i<125;i++){
                   wiadomosc=wiadomosc+" ";
               }
               System.out.println(wiadomosc);
               this.polaczenie.println(wiadomosc);
                String uzyt=this.polaczenie.readLine();
                String uzyt2=  uzyt.replace(" ","");
                String [] uzyt3=uzyt2.split("\t");
                List<String> uzyt4=Arrays.asList(uzyt3);
                listaPokoi.setItems(FXCollections.observableArrayList(uzyt4));
                }    
         catch (Exception ex) {
             this.polaczenie.close();
        }
    } 
    /**
     * Funkcja obsługująca klawisz "Utwórz pokój" 
     * @param event 
     */
    @FXML
    void handleUtworzPokoj(ActionEvent event) {
           try {
               this.nazwaPokoju.setVisible(true);
               this.dodajDoPokoju.setVisible(true);
               this.zatwierdz.setVisible(true);
               this.pokoje.setText("Uzytkownicy do dodania");
               this.pokoje.setText("Uzytkownicy dodani");
               String wiadomosc = "2";
               for(int i=1;i<125;i++){
                   wiadomosc=wiadomosc+" ";
               }
               this.polaczenie.println(wiadomosc); 
               String uzyt=this.polaczenie.readLine();
                String uzyt2=  uzyt.replace(" ","");
                String [] uzyt3=uzyt2.split("\t");
                List<String> uzyt4=Arrays.asList(uzyt3);
                listaPokoi.setItems(FXCollections.observableArrayList(uzyt4));
           }catch(Exception e){
                this.polaczenie.close();
                }
        //TODO Pokazywanie sie i znikanie przyciskow
        
        
        
           /* try{
            FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("Grupy.fxml"));
            Parent root1=(Parent) fxmlLoader.load();
            Stage stage=new Stage();
            stage.setTitle("Pokoje");
            stage.setScene(new Scene(root1));
            stage.show();
        }catch (Exception e){
            System.out.println("Nie można załadować okna");
        }*/
    }
    /**
     * ODSWIEZ LISTE KLIENTOW CASE 2
     * @param event 
     */
    @FXML
    void handleZaladujListeUzytkownikow(ActionEvent event) {
        
        try {
            System.out.println("jestem");
               String wiadomosc = "2";
               for(int i=1;i<125;i++){
                   wiadomosc=wiadomosc+" ";
               }
               System.out.println(wiadomosc);
               this.polaczenie.println(wiadomosc); 
               String uzyt = "";
                        uzyt=this.polaczenie.readLine();
                        String uzyt2=  uzyt.replace(" ","");
                        String [] uzyt3=uzyt2.split("\t");
                        List<String> uzyt4=Arrays.asList(uzyt3);
                        listaUzytkownikow.setItems(FXCollections.observableArrayList(uzyt4));
                }
         catch (Exception ex) {
             polaczenie.close();
        }
    } 
   
    /**
     * ZALOGUJ SIE CASE 1 GLOWNE POLACZENIE 
     * @param event 
     */
    
    @FXML
    void handleZalogujSie(ActionEvent event) {
       String wiadomosc="";
       //String [] sklejanie= new String[125];
        try {
               this.polaczenie = new Polaczenie(InetAddress.getByName("192.168.0.15"),1234);
               System.out.println("polaczylem");
               wiadomosc=wiadomosc+"1";
               String [] nick = nickText.getText().split("");
               int k=100;
               int dl=nickText.getText().length();
               String str=String.valueOf(dl);
               int dlstr=str.length();
               if(dlstr==1){
                 wiadomosc=wiadomosc+"00"+str;
               }
                 else{
                  wiadomosc=wiadomosc+"0"+str;
               }
             wiadomosc=wiadomosc+nickText.getText();
             int dlwiadomosci=wiadomosc.length();
               for(int i=dlwiadomosci;i<125;i++){
                   wiadomosc=wiadomosc+" ";
               }
               System.out.println(wiadomosc);
               this.polaczenie.println(wiadomosc);
               System.out.println("wyslalam");
               String odbior=this.polaczenie.readLine();
               if(odbior.equals("1")){
                   informacje.setText("Udało się zalogować");
               }else if(odbior.equals("2")){
                   informacje.setText("Nie udało się zalogować. Nazwa jest zajęta");
                   this.polaczenie.close();
               }
              
                  /*  nowyWatek=new Watek(this.polaczenie,this.oknoWiadomosci);
                    Thread th=new Thread(nowyWatek);
                    th.start();
                    synchronized(th){
                          try {
                            System.out.println("Wait for list.");
                             th.wait();
                            }catch (InterruptedException e){
                                System.out.println("Error in wait thread.");
                                e.printStackTrace();
                            }
                    }
                   // nowyWatek.setWatek(th);
                }
                }*/
           
            
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
       this.polaczenie=null;
               this.nazwaPokoju.setVisible(false);
               this.dodajDoPokoju.setVisible(false);
               this.zatwierdz.setVisible(false);
             //  this.pokoje.setText("Uzytkownicy do dodania");
              // this.pokoje.setText("Uzytkownicy dodani");
    }    
    
}
