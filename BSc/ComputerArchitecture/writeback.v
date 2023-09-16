module writeback_stage
	(
		data, 
		address_out,
		write_en_out,
		
		address_in,
		write_en_in,
		writeback_src,
		alu_data,
		mem_data
	);
	
output [15:0] data;
output [2:0] address_out;
output  write_en_out;

input [2:0] address_in;
input write_en_in;
input writeback_src;
input [15:0] alu_data;
input [15:0] mem_data;

	parameter WRITEBACK_SRC_ALU = 1'b0;
	
	assign address_out = address_in;
	assign write_en_out = write_en_in;
	assign data = (writeback_src == WRITEBACK_SRC_ALU) ? alu_data : mem_data;
	
endmodule
