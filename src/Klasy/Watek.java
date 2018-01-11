/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Klasy;

import javafx.fxml.FXML;
import javafx.scene.control.TextArea;

/**
 *
 * @author Anka
 */
public class Watek extends Thread {
    private Polaczenie polaczenie;
    private Thread watek;
    @FXML 
    private TextArea okno;

    public Watek(Polaczenie polaczenie, TextArea oknoWiadomosci) {
        this.polaczenie=polaczenie;
        this.okno=oknoWiadomosci;
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

    public void setPolaczenie(Polaczenie polaczenie,TextArea x) {
        this.polaczenie = polaczenie;
        this.okno=x;
    }
    
    @Override
    public void run() {
        while(!Thread.currentThread().isInterrupted()){
     try{
         System.out.println("Hello from thread");
        // String wiadomosc=this.polaczenie.readLine();
       //  this.okno.appendText(wiadomosc);
       Thread.sleep(30);
     } catch (InterruptedException ex) { 
     }
     }
       // throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }
    
}
