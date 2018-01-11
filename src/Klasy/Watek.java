/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Klasy;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import javafx.application.Platform;
import javafx.collections.FXCollections;
import javafx.fxml.FXML;
import javafx.scene.control.Label;
import javafx.scene.control.ListView;
import javafx.scene.control.TextArea;
import javafx.scene.layout.AnchorPane;

/**
 *
 * @author Anka
 */
public class Watek extends Thread {

    private Polaczenie polaczenie;
    private Thread watek;
    @FXML
    private Label informacje;
    @FXML
    private ListView listaUzytkownikow;

    public Watek(Polaczenie polaczenie, Label oknoWiadomosci, ListView listaUzyt) {
        this.polaczenie = polaczenie;
        this.informacje = oknoWiadomosci;
        this.listaUzytkownikow = listaUzyt;
        //throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    public Thread getWatek() {
        return watek;
    }

    public void setWatek(Thread watek) {
        this.watek = watek;
    }

    public Watek(Polaczenie polaczenie) {
        this.polaczenie = polaczenie;
    }

    public Polaczenie getPolaczenie() {
        return polaczenie;
    }

    public void setPolaczenie(Polaczenie polaczenie) {
        this.polaczenie = polaczenie;

    }

    @Override
    public void run() {
        while (!Thread.currentThread().isInterrupted()) {
            try {
                System.out.println("Hello from thread");
                String odp = "";
                String wiadomosc = this.polaczenie.readLine();
                System.out.println("To jest wiadomosc: "+wiadomosc);
                String[] wybor = wiadomosc.split("\t");
                String x = wybor[0];
                int y = Integer.parseInt(x);
                switch (y) {
                    case 2:
                        ArrayList<String> uzytkownicy = new ArrayList<>(Arrays.asList(wybor));
                        uzytkownicy.remove(0);
                        Platform.runLater(() -> {
                            this.listaUzytkownikow.setItems(FXCollections.observableArrayList(uzytkownicy));
                        });
                        break;
                    case 6:
                        if(wybor[1].equals(1)){
                        this.informacje.setText("Udało się wsyłać");
                        }
                        break;
                    default:
                        this.informacje.setText("Nie rozpoznano");
                        
                }

                Thread.sleep(3000);
            } catch (InterruptedException ex) {
            }
        }
        // throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

}
