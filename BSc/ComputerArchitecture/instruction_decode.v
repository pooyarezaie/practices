module instruction_decode_stage
	(
		alu_opcode,
		branch_en,
		branch_offset,
		writeback_address,
		writeback_en,
		writeback_src,
		memory_we,
		memory_data,
		reg_addr1, 
		reg_addr2,
		ex_src1, 
		ex_src2,
 
		reg_data1,
		reg_data2, 
		instruction, 
		clk, 
		rst
	);

output reg [2:0] alu_opcode;
output reg branch_en;
output [5:0] branch_offset;
output reg [2:0] writeback_address;
output reg writeback_en; 
output reg writeback_src; 
output reg memory_we; // new
output reg [15:0] memory_data;
output [2:0] reg_addr1; 
output [2:0] reg_addr2; 
output reg [15:0] ex_src1; 	
output reg [15:0] ex_src2;	

input [15:0] reg_data1;
input [15:0] reg_data2;
input [15:0] instruction;
input clk;
input rst;
	
	parameter BZ_INSTRUCTION = 4'b1100;
	parameter NOP_INSTRUCTION = 4'b0000;
	parameter LD_INSTRUCTION = 4'b1010;
	parameter ST_INSTRUCTION = 4'b1011;
	
	parameter WRITEBACK_SRC_ALU = 1'b0;
	parameter WRITEBACK_SRC_MEM = 1'b1;
	
	reg [2:0] alu_opcode_wire;
	reg writeback_en_wire;
	reg writeback_src_wire;
	reg [2:0] writeback_address_wire;
	reg [15:0] ex_src1_wire;
	reg [15:0] ex_src2_wire;
	reg memory_we_wire;
	wire [2:0] alu_opcode_generator_output;
	
	reg branch_taken;
  
	assign reg_addr1 = instruction[8:6];
	assign reg_addr2 = (instruction[15:12] == ST_INSTRUCTION) ? 
								instruction[11:9] : instruction[5:3];
	assign branch_offset = instruction[5:0];
	
	generate_alu_opcode
	(
		.alu_opcode(alu_opcode_generator_output),
		.instruction_opcode(instruction[15:12])
	);
		
	always @(posedge clk) begin 
		if (rst == 1'b1) begin 
			alu_opcode <= 3'b0;
			ex_src1 <= 16'b0;
			ex_src2 <= 16'b0;
			branch_taken <= 1'b0;
			writeback_en <= 1'b0;
			writeback_src <= 1'b0;
			writeback_address <= 3'b0;
			memory_we <= 1'b0;
			memory_data <= 16'b0;
		end
		else begin
			alu_opcode <= alu_opcode_wire;
			ex_src1 <= ex_src1_wire;
			ex_src2 <= ex_src2_wire;
			branch_taken <= branch_en;
			writeback_en <= writeback_en_wire;
			writeback_src <= writeback_src_wire;
			writeback_address <= writeback_address_wire;
			memory_we <= memory_we_wire;
			memory_data <= reg_data2;
		end
	end
	
	always @(instruction, alu_opcode_generator_output,
				branch_taken, reg_data1) begin
		branch_en = 1'b0;
		alu_opcode_wire = 3'b0;
		writeback_en_wire = 1'b0;
		writeback_src_wire = 1'b0;
		writeback_address_wire = 3'b0;
		memory_we_wire = 1'b0;
		ex_src1_wire = 1'b0;
		ex_src2_wire = 1'b0;
		
		if (branch_taken == 1'b0) begin
			alu_opcode_wire = alu_opcode_generator_output;
			
			ex_src1_wire = reg_data1;
			ex_src2_wire = reg_data2;
			
			if (instruction[15:12] == NOP_INSTRUCTION) begin
			end
			
			// R-type instructions
			if (instruction[15:12] > 4'd0 &&
				 instruction[15:12] < 4'd9) begin
				writeback_address_wire = instruction[11:9];
				writeback_en_wire = 1'b1;
				writeback_src_wire = WRITEBACK_SRC_ALU;
			end
			
			if (instruction[15:12] == BZ_INSTRUCTION &&
				 reg_data1 == 8'b0) begin
				branch_en = 1'b1;
			end
			
			if (instruction[15:12] == LD_INSTRUCTION || 
				instruction[15:12] == ST_INSTRUCTION) begin 
				
				ex_src1_wire = {{10{instruction[5]}}, instruction[5:0]}; // sign extend 
				
				if (instruction[15:12] == LD_INSTRUCTION) begin
					writeback_src_wire =  WRITEBACK_SRC_MEM;
					writeback_en_wire = 1'b1;
					writeback_address_wire = instruction[11:9];
				end
				else begin
					memory_we_wire = 1'b1;
					ex_src2_wire = reg_data1;
				end
			end
			
		end
	end

endmodule

module generate_alu_opcode
	(
		alu_opcode, 
		instruction_opcode
	);
	
output reg [2:0] alu_opcode; 
input [3:0] instruction_opcode;

	always @(instruction_opcode) begin 
		case (instruction_opcode)
			4'b0001: alu_opcode = 3'b000;
			4'b0010: alu_opcode = 3'b001;
			4'b0011: alu_opcode = 3'b010;
			4'b0100: alu_opcode = 3'b011;
			4'b0101: alu_opcode = 3'b100;
			4'b0110: alu_opcode = 3'b101;
			4'b0111: alu_opcode = 3'b110;
			4'b1000: alu_opcode = 3'b111;
			4'b1001: alu_opcode = 3'b000;
			4'b1010: alu_opcode = 3'b000;
			4'b1011: alu_opcode = 3'b000;
			4'b1010: alu_opcode = 3'b000;
			4'b1011: alu_opcode = 3'b000;
			
			default: alu_opcode = 3'b000;
		endcase
	end
endmodule

