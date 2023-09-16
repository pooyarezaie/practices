module memory_stage
	(
		writeback_address_out,
		writeback_en_out,
		writeback_src_out,
		alu_data_out,
		
		mem_data_out,
		// add unbufferd mem_out
		
		
		mem_data_in,
		we,
		
		writeback_address_in,
		writeback_en_in,
		writeback_src_in,
		alu_data_in,
		clk,
		rst
	);
output reg [2:0] writeback_address_out;
output reg writeback_en_out;
output reg writeback_src_out;
output reg [15:0] alu_data_out;

output reg [15:0] mem_data_out;

input [15:0] mem_data_in;
input we;

input [2:0] writeback_address_in;
input writeback_en_in;
input writeback_src_in;
input [15:0] alu_data_in;
input clk;
input rst;

	reg [15:0] data [63:0];
	
	wire [15:0] mem_data;
	
	assign mem_data = data[alu_data_in[5:0]];
	
	integer i;
	always@(posedge clk) begin
		if (rst == 1'b1) begin
			writeback_address_out <= 3'b0;
			writeback_en_out <= 1'b0;
			writeback_src_out <= 1'b0;
			alu_data_out <= 16'b0;
			mem_data_out <= 16'b0;
			
			for (i=0; i<63; i=i+1)
				data[i] <= 16'b0;
		end
		else begin
			writeback_address_out <= writeback_address_in;
			writeback_en_out <= writeback_en_in;
			writeback_src_out <= writeback_src_in;
			alu_data_out <= alu_data_in;
			mem_data_out <= mem_data;
			
			if (we == 1'b1) begin 
				data[alu_data_in[5:0]] <= mem_data_in;
			end
		end
	end
endmodule
