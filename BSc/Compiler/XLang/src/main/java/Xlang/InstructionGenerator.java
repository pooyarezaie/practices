package Xlang;

import java.util.ArrayList;

@SuppressWarnings("SameParameterValue")
public class InstructionGenerator {
    private ArrayList<String> instructions;

    private int labelNum = 0;

    private boolean autoCommit = true;
    private ArrayList<String> unCommittedInstructions;

    public InstructionGenerator() {
        instructions = new ArrayList<>();
        unCommittedInstructions = new ArrayList<>();
        addInstruction(".globl main");
        addInstruction("main:");
    }

    public ArrayList<String> getInstructions() {
        ArrayList<String> program = new ArrayList<>(instructions);
        program.add("jr $ra");
        return program;
    }

    public void addInstruction(String instruction) {
        if (autoCommit) {
            commitAll();
            instructions.add(instruction);
        }
        else
            unCommittedInstructions.add(instruction);
    }

    public void commitAll() {
        instructions.addAll(unCommittedInstructions);
        unCommittedInstructions.clear();
    }

    public void roleBack() {
        unCommittedInstructions.clear();
    }

    public void setAutoCommit(boolean autoCommit) {
        this.autoCommit = autoCommit;
    }

    public String createLabel(String labelName, boolean commit) {
        String labelUniqueName = String.format("%s_%d", labelName, labelNum++);
        if (commit)
            addLabel(labelUniqueName);
        return labelUniqueName;
    }

    public String createLabel(String labelName) {
        return createLabel(labelName, true);
    }

    public void addLabel(String labelName) {
        addInstruction(String.format("%s:", labelName));
    }

    public void loadVariable(String register, int value) {
        addInstruction(String.format("li $%s, %d", register, value));
    }

    public void addi(String dest, String arg, int value) {
        int MAX_IMMEDIATE = 32767;
        while(value > MAX_IMMEDIATE) {
            addInstruction(String.format("addi $%s, $%s, +%d", dest, arg, MAX_IMMEDIATE));
            value -= MAX_IMMEDIATE;
        }
        if (value != 0)
            addInstruction(String.format("addi $%s, $%s, %d", dest, arg, value));
    }

    public void addu(String dest, String arg1, String arg2) {
        addInstruction(String.format("addu $%s, $%s, $%s", dest, arg1, arg2));
    }

    public void add(String dest, String arg1, String arg2) {
        addInstruction(String.format("add $%s, $%s, $%s", dest, arg1, arg2));
    }

    public void subu(String dest, String arg1, String arg2) {
        addInstruction(String.format("subu $%s, $%s, $%s", dest, arg1, arg2));
    }

    public void sub(String dest, String arg1, String arg2) {
        addInstruction(String.format("sub $%s, $%s, $%s", dest, arg1, arg2));
    }

    public void multiply(String dest, String arg1, String arg2) {
        addInstruction(String.format("mul $%s, $%s, $%s", dest, arg1, arg2));
    }

    public void divide(String dest, String arg1, String arg2) {
        addInstruction(String.format("div $%s, $%s, $%s", dest, arg1, arg2));
    }

    public void and(String dest, String arg1, String arg2) {
        String zeroAnsLabel = createLabel("ZERO_ANS", false);
        String endLabel = createLabel("END", false);
        branchZero(arg1, zeroAnsLabel);
        branchZero(arg2, zeroAnsLabel);
        loadVariable(dest, 1);
        jump(endLabel);
        addLabel(zeroAnsLabel);
        loadVariable(dest, 0);
        addLabel(endLabel);
    }

    public void or(String dest, String arg1, String arg2) {
        String oneAnsLabel = createLabel("ONE_ANS", false);
        String endLabel = createLabel("END", false);
        branchNotZero(arg1, oneAnsLabel);
        branchNotZero(arg2, oneAnsLabel);
        loadVariable(dest, 0);
        jump(endLabel);
        addLabel(oneAnsLabel);
        loadVariable(dest, 1);
        addLabel(endLabel);
    }

    public void gt(String dest, String arg1, String arg2) {
        String gtLabel = createLabel("GT", false);
        String endLabel = createLabel("END", false);
        branchGreaterThan(arg1, arg2, gtLabel);
        loadVariable(dest, 0);
        jump(endLabel);
        addLabel(gtLabel);
        loadVariable(dest, 1);
        addLabel(endLabel);
    }

    public void eq(String dest, String arg1, String arg2) {
        String eqLabel = createLabel("EQ", false);
        String endLabel = createLabel("END", false);
        branchEqual(arg1, arg2, eqLabel);
        loadVariable(dest, 0);
        jump(endLabel);
        addLabel(eqLabel);
        loadVariable(dest, 1);
        addLabel(endLabel);
    }

    public void neq(String dest, String arg1, String arg2) {
        String eqLabel = createLabel("NEQ", false);
        String endLabel = createLabel("END", false);
        branchEqual(arg1, arg2, eqLabel);
        loadVariable(dest, 1);
        jump(endLabel);
        addLabel(eqLabel);
        loadVariable(dest, 0);
        addLabel(endLabel);
    }

    public void not(String dest, String arg) {
        String zeroLabel = createLabel("ZERO", false);
        String endLabel = createLabel("END", false);
        branchZero(arg, zeroLabel);
        loadVariable(dest, 0);
        jump(endLabel);
        addLabel(zeroLabel);
        loadVariable(dest, 1);
        addLabel(endLabel);
    }

    public void lt(String dest, String arg1, String arg2) {
        addInstruction(String.format("slt $%s, $%s, $%s", dest, arg1, arg2));
    }

    public void comment(String comment) {
        addInstruction("\n# " + comment + '\n');
    }

    public void systemCall() {
        addInstruction("syscall");
    }

    public void branchPositive(String reg, String label) {
        addInstruction(String.format("bgtz $%s, %s", reg, label));
    }

    public void branchZero(String reg, String label) {
        addInstruction(String.format("beqz $%s, %s", reg, label));
    }

    public void branchNotZero(String reg, String label) {
        addInstruction(String.format("bnez $%s, %s", reg, label));
    }

    public void branchGreaterThan(String arg1, String arg2, String label) {
        addInstruction(String.format("bgt $%s, $%s %s", arg1, arg2, label));
    }

    public void branchGreaterThenEqual(String arg1, String arg2, String label) {
        addInstruction(String.format("bge $%s, $%s %s", arg1, arg2, label));
    }

    public void branchEqual(String arg1, String arg2, String label) {
        addInstruction(String.format("beq $%s, $%s %s", arg1, arg2, label));
    }

    public void jump(String label) {
        addInstruction(String.format("j %s", label));
    }

    public void storeVariable(String dest, String reg, int offset) {
        addInstruction(String.format("sw $%s, %d($%s)", reg, offset, dest));

    }

    public void loadWord(String dest, String reg, int offset) {
        addInstruction(String.format("lw $%s, %d($%s)", dest, offset, reg));
    }

    public void move(String dest, String arg) {
        addInstruction(String.format("move $%s, $%s", dest, arg));
    }

}
