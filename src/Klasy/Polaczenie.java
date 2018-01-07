/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Klasy;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;

/**
 *
 * @author Anka
 */
public class Polaczenie {
    private Socket socket;

    public Polaczenie(InetAddress ip, int port) {
        try {
            this.socket = new Socket(ip, port);
        } catch (IOException e) {
         System.out.println("Nie udalo sie utworzyc gniazda");
        }
    }  
    public void println(String message) {
        PrintWriter writer;
        try {
            writer = new PrintWriter(new OutputStreamWriter(
                                     socket.getOutputStream()), true);
            writer.println(message);
        } catch (IOException e){ 
            System.out.println("Nie udało sie wyslac");
        }
    }

    public String readLine() {
        BufferedReader reader;
        try {
            reader = new BufferedReader(new InputStreamReader(
                                        socket.getInputStream()));
            
            return reader.readLine();
        } catch (IOException e) {
            // TODO Auto-generated catch block
            System.out.print("Nie udalo sie odberac");
            return "";
        }
    }     
    
    public void close() {
        try {
            if (socket != null && !socket.isClosed())
                socket.close();
        } catch (IOException e) {
            // TODO Auto-generated catch block
           // e.printStackTrace();
        }
    }
    public Socket getSocket() {
        return socket;
    }

    public void setSocket(Socket socket) {
        this.socket = socket;
    }
    
}
