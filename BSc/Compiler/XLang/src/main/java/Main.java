import Antlr.XLexer;
import Antlr.XParser;
import org.antlr.v4.runtime.*;

public class Main {
	public static void main(String[] args) throws Exception {

		CharStream input = new ANTLRFileStream(args[0]);
		XLexer lexer = new XLexer(input);
		CommonTokenStream ts = new CommonTokenStream(lexer);
		XParser parser = new XParser(ts);
		XParser.ProgramContext program_ctx = parser.program();
 	}
}
