package Xlang;

import Antlr.XParser;

public class ErrorManager {
    private XParser parser;
    private boolean hasError;

    public ErrorManager() {hasError = false;}

    public boolean hasError() {return hasError;}

    public void setParser(XParser parser) {this.parser = parser;}

    public int getLine() {return parser.getCurrentToken().getLine();}

    public String getErrorMessage(String msg) {
        return String.format("[ERROR] Line %d : %s%n", getLine(), msg);
    }

    public void error(String msg) {
        System.out.print(getErrorMessage(msg));
        hasError = true;
    }

    public void log(String msg) {System.out.format("[LOG] Line %d : %s%n", getLine(), msg);}
}
