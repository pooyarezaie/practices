package Xlang;

import Utils.Pair;

import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.stream.Collectors;

public class CodeGenerator {
    private InstructionGenerator ig = new InstructionGenerator();

    public CodeGenerator() {
        ig.comment("start stack start");
        ig.move("s8", "sp");
        ig.addi("s8", "s8", -4);
        ig.comment("end stack start");
    }

    public void popScope(int size) {
        ig.comment("start pop stack");
        ig.addi("sp", "sp", size);
        ig.comment("end pop stack");
    }

    public void shrinkStack(int returnSize, int argSize) {
        if (returnSize == 0)
            popScope(argSize);
    }

    public void updateVariable(int offset, int size) {
        ig.comment("start updating variable");
        ig.loadVariable("t0", size);
        ig.loadVariable("t1", offset);
        ig.subu("t1", "s8", "t1");
        ig.addu("t2", "sp", "t0");
        ig.addi("t2", "t2", -4);
        String label = ig.createLabel("FOR");
        ig.loadWord("t3", "t2", 0);
        ig.storeVariable("t1", "t3", 0);
        ig.addi("t2", "t2", -4);
        ig.addi("t1", "t1", -4);
        ig.addi("t0", "t0", -4);
        ig.branchPositive("t0", label);
        ig.comment("end updating variable") ;
    }

    public void pushVariable(int size) {
        ig.comment("start push variable");
        ig.addi("sp", "sp", -size);
        ig.comment("end push variable");
    }

    public void copyVariableToTop(int offset, int size) {
        ig.comment("start copy to top");
        ig.loadVariable("t0", size);
        ig.loadVariable("t1", offset);
        ig.subu("t1", "s8", "t1"); //todo: why subu instead of sub?
        String label = ig.createLabel("FOR");
        ig.addi("sp", "sp", -4);
        ig.loadWord("t2", "t1", 0);
        ig.storeVariable("sp", "t2", 0);
        ig.addi("t0", "t0", -4);
        ig.addi("t1", "t1", -4);
        ig.branchPositive("t0", label);
        ig.comment("end copy to top");
    }

    public String handleBeginWhile() {
        commit();
        return ig.createLabel("BEGIN_WHILE") ;
    }

    public String handleWhileBranch() {
        commit();
        ig.loadWord("t0", "sp", 0);
        popScope(4);
        String endLabel = ig.createLabel("END_WHILE", false);
        ig.branchZero("t0", endLabel);
        return endLabel;
    }

    public void handleEndWhile(String startLabel, String endLabel) {
        ig.jump(startLabel);
        ig.addLabel(endLabel);
    }

    public void handleBeginForeach(int variableSize) {
        ig.addi("sp", "sp", -variableSize);
    }

    public Pair<String, String> handleForeachBranch(int typeSize, int variableOffset) {
        commit();
        ig.comment("begin foreach");
        String endLabel = ig.createLabel("END_FOR_EACH", false);
        ig.addi("sp", "sp", -4);
        ig.loadVariable("t0", variableOffset);
        ig.sub("t0", "s8", "t0");
        ig.storeVariable("sp", "t0", 0);

        String foreachLabel = ig.createLabel("FOR_EACH");

        ig.loadWord("t0", "sp", 0);
        ig.addi("t0", "t0", -typeSize);
        ig.branchGreaterThenEqual("sp", "t0", endLabel);

        ig.storeVariable("sp", "t0", 0);
        ig.loadVariable("t1", variableOffset);
        ig.sub("t1", "s8", "t1");
        ig.loadVariable("t2", typeSize);
        String label = ig.createLabel("FOR");
        ig.loadWord("t3", "t0", 0);
        ig.storeVariable("t1", "t3", 0);
        ig.addi("t0", "t0", -4);
        ig.addi("t1", "t1", -4);
        ig.addi("t2", "t2", -4);
        ig.branchPositive("t2", label);
        ig.comment("end foreach");

        return new Pair<>(foreachLabel, endLabel);
    }

    public void handleEndForeach(int typeSize, int length, Pair<String, String> labels) {
        ig.loadVariable("t0", typeSize*length + typeSize + 4);
        ig.sub("sp", "sp", "t0");
        ig.jump(labels.getLeft()); // foreach label
        ig.addLabel(labels.getRight()); // end label
        ig.loadVariable("t0", typeSize*length + typeSize + 4);
        ig.add("sp", "sp", "t0");
    }

    public void roleBack() {
        ig.roleBack();
    }

    public void manualCommit() {
        ig.setAutoCommit(false);
    }

    public void commit() {
        ig.commitAll();
        ig.setAutoCommit(true);
    }

    public void writeConsole(int length) {
        ig.comment("start write console");
        ig.loadVariable("t7", length - 1);
        writeConsole();
        ig.comment("end write console");
    }

    public void writeConsole() {
        ig.loadVariable("v0", 4);
        ig.multiply("t1", "v0", "t7");
        ig.addu("a0", "sp", "t1");
        String label = ig.createLabel("PRINT_CHAR");
        String end = ig.createLabel("END", false);
        ig.branchZero("t7", end);
        ig.systemCall();
        ig.addi("t7", "t7", -1);
        ig.addi("a0", "a0", -4);
        ig.branchPositive("t7", label);
        ig.addLabel(end);
        ig.systemCall();
    }

    public void readConsole() {
        ig.comment("start read console");
        ig.loadWord("t0", "sp", 0);
        ig.move("t7", "t0");
        ig.addi("sp", "sp", 4);
        ig.loadVariable("v0", 8);
        ig.loadVariable("a1", 2);
        ig.addi("sp", "sp", -4);
        String label = ig.createLabel("READ_CHAR");
        ig.move("a0", "sp");
        ig.addi("sp", "sp", -4);
        ig.systemCall();
        ig.addi("t0", "t0", -1);
        ig.branchPositive("t0", label);
        ig.comment("end read console");
    }

    public void loadIntegerConstant(int value) {
        ig.comment(String.format("start load integer %d", value));
        ig.addi("sp", "sp", -4);
        ig.loadVariable("t0", value);
        ig.storeVariable("sp", "t0", 0);
        ig.comment(String.format("end load integer %d", value));
    }

    public void loadCharacterConstant(char value) {
        ig.comment(String.format("start load char %c", value));
        ig.addi("sp", "sp", -4);
        ig.loadVariable("t0", value);
        ig.storeVariable("sp", "t0", 0);
        ig.comment(String.format("end load char %c", value));

    }

    public void loadStringConstant(String value) {
        for (int i = 0; i < value.length(); ++i) {
            loadCharacterConstant(value.charAt(i));
        }
    }

    public void addOperation() {
        ig.comment("start adding");
        ig.loadWord("t0", "sp", 0);
        ig.addi("sp", "sp", 4);
        ig.loadWord("t1", "sp", 0);
        ig.add("t0", "t1", "t0");
        ig.storeVariable("sp", "t0", 0);
        ig.comment("end adding");
    }

    public void subOperation() {
        ig.comment("start subbing");
        ig.loadWord("t0", "sp", 0);
        ig.addi("sp", "sp", 4);
        ig.loadWord("t1", "sp", 0);
        ig.sub("t0", "t1", "t0");
        ig.storeVariable("sp", "t0", 0);
        ig.comment("end subbing");
    }

    public void multiplyOperation() {
        ig.comment("start multiplying");
        ig.loadWord("t0", "sp", 0);
        ig.addi("sp", "sp", 4);
        ig.loadWord("t1", "sp", 0);
        ig.multiply("t0", "t0", "t1");
        ig.storeVariable("sp", "t0", 0);
        ig.comment("end multiplying");
    }

    public void divideOperation() {
        ig.comment("start dividing");
        ig.loadWord("t0", "sp", 0);
        ig.addi("sp", "sp", 4);
        ig.loadWord("t1", "sp", 0);
        ig.divide("t0", "t1", "t0");
        ig.storeVariable("sp", "t0", 0);
        ig.comment("end dividing");
    }

    public void andOperation() {
        ig.comment("start and op");
        ig.loadWord("t0", "sp", 0);
        ig.addi("sp", "sp", 4);
        ig.loadWord("t1", "sp", 0);
        ig.and("t0", "t1", "t0");
        ig.storeVariable("sp", "t0", 0);
        ig.comment("end and op");
    }

    public void orOperation() {
        ig.comment("start or op");
        ig.loadWord("t0", "sp", 0);
        ig.addi("sp", "sp", 4);
        ig.loadWord("t1", "sp", 0);
        ig.or("t0", "t1", "t0");
        ig.storeVariable("sp", "t0", 0);
        ig.comment("end or op");
    }

    public void gtComparison() {
        ig.comment("start gt comparison");
        ig.loadWord("t0", "sp", 0);
        ig.addi("sp", "sp", 4);
        ig.loadWord("t1", "sp", 0);
        ig.gt("t0", "t1", "t0");
        ig.storeVariable("sp", "t0", 0);
        ig.comment("end gt comparison");
    }

    public void ltComparison() {
        ig.comment("start lt comparison");
        ig.loadWord("t0", "sp", 0);
        ig.addi("sp", "sp", 4);
        ig.loadWord("t1", "sp", 0);
        ig.lt("t0", "t1", "t0");
        ig.storeVariable("sp", "t0", 0);
        ig.comment("end lt comparison");
    }

    public void eqComparison() {
        ig.comment("start eq comparison");
        ig.loadWord("t0", "sp", 0);
        ig.addi("sp", "sp", 4);
        ig.loadWord("t1", "sp", 0);
        ig.eq("t0", "t1", "t0");
        ig.storeVariable("sp", "t0", 0);
        ig.comment("end eq comparison");
    }

    public void neqComparison() {
        ig.comment("start neq comparison");
        ig.loadWord("t0", "sp", 0);
        ig.addi("sp", "sp", 4);
        ig.loadWord("t1", "sp", 0);
        ig.neq("t0", "t1", "t0");
        ig.storeVariable("sp", "t0", 0);
        ig.comment("end neq comparison");
    }

    public void invertOperation(){
        ig.comment("start invert operation");
        ig.loadWord("t0", "sp", 0);
        ig.sub("t0", "zero", "t0");
        ig.storeVariable("sp", "t0", 0);
        ig.comment("end invert operation");
    }

    public void notOperation() {
        ig.comment("start not operation");
        ig.loadWord("t0", "sp", 0);
        ig.not("t0", "t0");
        ig.storeVariable("sp", "t0", 0);
        ig.comment("end not operation");
    }

    public void listAccessOperation(int listSize, int baseOffset, int resultSize) {
        ig.comment("start list access operation");
        /*System.out.println("list size : " + listSize);
        System.out.println("base offset : " + baseOffset);
        System.out.println("result size : " + resultSize);*/
        ig.addi("sp", "sp", listSize);
        ig.move("t1", "sp");
        ig.loadVariable("t0", baseOffset);
        ig.subu("sp", "sp", "t0");

        ig.loadVariable("t0", resultSize);

        String label = ig.createLabel("LIST_ACCESS");
        ig.loadWord("t2", "sp", 0);
        ig.storeVariable("t1", "t2", 0);
        ig.addi("t0", "t0", -4);
        ig.addi("t1", "t1", -4);
        ig.addi("sp", "sp", -4);
        ig.branchPositive("t0", label);
        ig.move("sp", "t1");
        ig.addi("sp", "sp", 4);
        ig.comment("end list access operation");
    }

    public String handleBeginIf() {
        String label = ig.createLabel("END_IF", false);
        ig.loadWord("t0", "sp", 0);
        popScope(4);
        ig.branchZero("t0", label);
        return label;
    }

    public Pair<String, String> handleBeginIfElse() {
        String ifLabel = ig.createLabel("END_IF", false);
        String elseLabel = ig.createLabel("END_ELSE", false);
        ig.loadWord("t0", "sp", 0);
        popScope(4);
        ig.branchZero("t0", ifLabel);
        return new Pair<>(ifLabel, elseLabel);
    }


    public void handleBeginElse(String ifLabel, String elseLabel) {
        ig.jump(elseLabel);
        ig.addLabel(ifLabel);
    }

    public void handleEndIf(String label) {
        ig.addLabel(label);
    }

    public String showProgram() {
        return ig.getInstructions().stream().collect(Collectors.joining("\n"));
    }

    public void saveProgram() throws FileNotFoundException {
        commit();
        try(PrintWriter out = new PrintWriter("outputs/program.asm")){
            out.println(showProgram());
        }
    }
}
