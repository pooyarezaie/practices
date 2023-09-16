module instruction_fetch_stage
	(
		instruction,
		pc_offset,
		branch_en,
		clk, 
		rst
	);

output reg [15:0] instruction;
input [5:0] pc_offset;
input branch_en;
input clk;
input rst;

	
	reg [7:0] pc;
	wire [15:0] instruction_wire;
	
	instruction_memory instruction_rom(instruction_wire, pc);
	
	always @(posedge clk) begin 
		if (rst == 1'b1) begin
			pc <= 8'b0;
			instruction <= 16'b0;
		end
		else begin
			if (branch_en == 1'b0) begin
				pc <= pc + 8'b1;
			end
			else begin 
				pc <= pc + {{2{pc_offset[5]}}, pc_offset}; // sign extend 
			end
			instruction <= instruction_wire;
		end
	end
endmodule

module instruction_memory(output reg [15:0] data_out, input [7:0] addr_in);
	always@(addr_in) begin
		case(addr_in)
			8'd0: data_out = 16'b0001_011_001_011_000;
			8'd1: data_out = 16'b0010_001_001_010_000;
			8'd2: data_out = 16'b0000_000_000_000_000;
			8'd3: data_out = 16'b0000_000_000_000_000;
			8'd4: data_out = 16'b1100_000_001_000_111;
			8'd5: data_out = 16'b1100_000_000_111_010;
			8'd6: data_out = 16'b0010_011_011_010_000;
			8'd12: data_out = 16'b1011_011_000_000_000;
			8'd13: data_out = 16'b0000_000_000_000_000;
			8'd14: data_out = 16'b0000_000_000_000_000;
			8'd15: data_out = 16'b1010_001_000_000_000;
			default: data_out = 16'b0;
		endcase
	end
endmodule
