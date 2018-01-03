/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package sk2projektklient;

/**
 *
 * @author Anka
 */
public class Uzytkownik {
    private String nick;
    private String haslo;
    private boolean czyDostepny;

    public Uzytkownik(String nick, String haslo, boolean czyDostepny) {
        this.nick = nick;
        this.haslo = haslo;
        this.czyDostepny = czyDostepny;
    }

    public String getNick() {
        return nick;
    }

    public void setNick(String nick) {
        this.nick = nick;
    }

    public String getHaslo() {
        return haslo;
    }

    public void setHaslo(String haslo) {
        this.haslo = haslo;
    }

    public boolean isCzyDostepny() {
        return czyDostepny;
    }

    public void setCzyDostepny(boolean czyDostepny) {
        this.czyDostepny = czyDostepny;
    }
    
    
}
