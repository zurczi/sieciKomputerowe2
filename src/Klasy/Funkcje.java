/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Klasy;

import java.util.ArrayList;

/**
 *
 * @author Anka
 */
public class Funkcje {
public String[] funkcja(String wejscie){
    System.out.println("dupka1");
    String[] wyjscie;
    wejscie.replace(" ", "");
    System.out.println("dupka");
    System.out.println(wejscie);
    wyjscie=wejscie.split("\t");
    return wyjscie;
}
}
