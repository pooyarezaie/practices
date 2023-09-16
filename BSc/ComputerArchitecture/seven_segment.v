module seven_segment_decoder(output reg [6:0]hex, input [3:0]num);
	always@(num) begin
		case(num)
			4'd0: hex = 7'b1000000;
			4'd1: hex = 7'b1111001;
			4'd2: hex = 7'b0100100;
			4'd3: hex = 7'b0110000;
			4'd4: hex = 7'b0011001;
			4'd5: hex = 7'b0010010;
			4'd6: hex = 7'b0000010;
			4'd7: hex = 7'b1111000;
			4'd8: hex = 7'b0000000;
			4'd9: hex = 7'b0010000;
			default: hex = 7'b0110110;
		endcase
	end
endmodule
