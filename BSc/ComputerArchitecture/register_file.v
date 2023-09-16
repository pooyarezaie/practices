module register_file
	(
		data_out1,
		data_out2,
		
		addr_read1,
		addr_read2,
		addr_write,
		data_in,
		we,
		clk,
		rst
	);
output [15:0] data_out1;
output [15:0] data_out2;

input [2:0] addr_read1;
input [2:0] addr_read2;
input [2:0] addr_write;
input [15:0] data_in;
input we;
input clk;
input rst;
	
	reg [15:0] data [7:0];
	
	assign data_out1 = data[addr_read1];
	assign data_out2 = data[addr_read2];
	
	integer i; // for set zero data in reset  
	always@(negedge clk) begin
		if(rst == 1'b1) begin
			data[0] <= 16'd0;
			data[1] <= 16'd5;
			data[2] <= 16'd1;
			data[3] <= 16'd0;
			data[4] <= 16'd0;
			data[5] <= 16'd0;
			data[6] <= 16'd0;
			data[7] <= 16'd0;
		end
		else begin
			if(we == 1'b1 && addr_write != 3'b0) begin
				data[addr_write] <= data_in;
			end
		end
	end
endmodule
