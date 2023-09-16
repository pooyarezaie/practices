module execution_stage
	(
		writeback_address_out,
		writeback_en_out,
		writeback_src_out,
		memory_we_out,
		memory_data_out, //memory data
		
		alu_out,
		
		alu_opcode,
		data_in1,
		data_in2,
		
		writeback_address_in,
		writeback_en_in,
		writeback_src_in,
		memory_we_in,
		memory_data_in,
		
		clk,
		rst
	);
output reg [2:0] writeback_address_out;
output reg writeback_en_out;
output reg writeback_src_out;
output reg memory_we_out;
output reg [15:0] memory_data_out; // memory data 

output reg [15:0] alu_out;

input [2:0] alu_opcode;
input [15:0] data_in1;
input [15:0] data_in2;

input [2:0] writeback_address_in;
input writeback_en_in;
input writeback_src_in;
input memory_we_in;
input [15:0] memory_data_in;

input clk;
input rst;

	parameter ADD  = 3'b000;
	parameter SUB  = 3'b001;
	parameter AND  = 3'b010;
	parameter OR   = 3'b011;
	parameter XOR  = 3'b100;
	parameter SL   = 3'b101;
	parameter SR   = 3'b110;
	parameter SRU  = 3'b111;

	reg [15:0] alu_result;
	
	always@(data_in1, data_in2, alu_opcode) begin
		case(alu_opcode)
			ADD: alu_result = data_in1 + data_in2;
			SUB: alu_result = data_in1 - data_in2;
			AND: alu_result = data_in1 & data_in2;
			OR:  alu_result = data_in1 | data_in2;
			XOR: alu_result = data_in1 ^ data_in2;
			SL:  alu_result = data_in1<<data_in2;
			SR:  alu_result = data_in1>>>data_in2;
			SRU: alu_result = data_in1>>data_in2;
			default: alu_result = 16'b0;
		endcase
	end
	
	always@(posedge clk) begin
		if(rst == 1'b1) begin
			alu_out <= 16'b0;
			writeback_address_out <= 3'b0;
			writeback_en_out <= 1'b0;
			writeback_src_out <= 1'b0;
			memory_we_out <= 1'b0;
			memory_data_out <= 16'b0;
		end
		else begin
			alu_out <= alu_result;
			writeback_address_out <= writeback_address_in;
			writeback_en_out <= writeback_en_in;
			writeback_src_out <= writeback_src_in;
			memory_we_out <= memory_we_in;
			memory_data_out <= memory_data_in;
		end
	end
	
endmodule
