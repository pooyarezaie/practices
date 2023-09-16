module mips
	(
		clk,
		rst
	);
input clk;
input rst;
	
	wire [15:0] s1_instruction;
	
	wire [5:0] s2_branch_offset;
	wire s2_branch_en;
	wire [2:0] s2_alu_opcode;
	wire [2:0] s2_writeback_address;
	wire s2_writeback_en;
	wire s2_writeback_src;
	wire [2:0] s2_reg_addr1;
	wire [2:0] s2_reg_addr2;
	wire [15:0] s2_ex_src1;
	wire [15:0] s2_ex_src2;
	wire s2_memory_we;
	wire [15:0] s2_memory_data;
	
	wire [15:0] register_file_reg_out1;
	wire [15:0] register_file_reg_out2;
	
	wire [2:0] s3_writeback_address;
	wire s3_writeback_en;
	wire s3_writeback_src;
	wire [15:0] s3_alu_out;
	wire s3_memory_we_out;
	wire [15:0] s3_memory_data_out;
	
	wire [2:0] s4_writeback_address;
	wire s4_writeback_en;
	wire s4_writeback_src;
	wire [15:0] s4_alu_data_out;
	wire [15:0] s4_mem_data_out;
	
	wire [15:0] s5_data;
	wire [2:0] s5_address;
	wire s5_write_en;
	
	instruction_fetch_stage stage1
	(
		.instruction(s1_instruction),
		
		.pc_offset(s2_branch_offset),
		.branch_en(s2_branch_en),
		.clk(clk),
		.rst(rst)
	);
	
	instruction_decode_stage stage2
	(
		.alu_opcode(s2_alu_opcode), 
		.branch_en(s2_branch_en),
		.branch_offset(s2_branch_offset),
		.writeback_address(s2_writeback_address),
		.writeback_en(s2_writeback_en),
		.writeback_src(s2_writeback_src),
		.memory_we(s2_memory_we),
		.memory_data(s2_memory_data),
		.reg_addr1(s2_reg_addr1), 
		.reg_addr2(s2_reg_addr2), 
		.ex_src1(s2_ex_src1), 
		.ex_src2(s2_ex_src2),
		
		.reg_data1(register_file_reg_out1),
		.reg_data2(register_file_reg_out2), 
		.instruction(s1_instruction), 
		.clk(clk), 
		.rst(rst)
	);
	
	register_file regfile
	(
		.data_out1(register_file_reg_out1),
		.data_out2(register_file_reg_out2),
		
		.addr_read1(s2_reg_addr1),
		.addr_read2(s2_reg_addr2),
		.addr_write(s5_address),
		.data_in(s5_data),
		.we(s5_write_en),
		.clk(clk),
		.rst(rst)
	);
	
	execution_stage stage3
	(
		.writeback_address_out(s3_writeback_address),
		.writeback_en_out(s3_writeback_en),
		.writeback_src_out(s3_writeback_src),
		.memory_we_out(s3_memory_we_out),
		.memory_data_out(s3_memory_data_out), // memory data
		
		.alu_out(s3_alu_out),
		
		.alu_opcode(s2_alu_opcode),
		.data_in1(s2_ex_src1),
		.data_in2(s2_ex_src2),
		
		.writeback_address_in(s2_writeback_address),
		.writeback_en_in(s2_writeback_en),
		.writeback_src_in(s2_writeback_src),
		.memory_we_in(s2_memory_we),
		.memory_data_in(s2_memory_data),
		
		.clk(clk),
		.rst(rst)
	);
	
	memory_stage stage4
	(
		.writeback_address_out(s4_writeback_address),
		.writeback_en_out(s4_writeback_en),
		.writeback_src_out(s4_writeback_src),
		.alu_data_out(s4_alu_data_out),
		
		.mem_data_out(s4_mem_data_out),
		
		.mem_data_in(s3_memory_data_out),
		.we(s3_memory_we_out),
		
		.writeback_address_in(s3_writeback_address),
		.writeback_en_in(s3_writeback_en),
		.writeback_src_in(s3_writeback_src),
		.alu_data_in(s3_alu_out),
		.clk(clk),
		.rst(rst)
	);
	
	writeback_stage stage5
	(
		.data(s5_data), 
		.address_out(s5_address),
		.write_en_out(s5_write_en),
		
		.address_in(s4_writeback_address),
		.write_en_in(s4_writeback_en),
		.writeback_src(s4_writeback_src),
		.alu_data(s4_alu_data_out),
		.mem_data(s4_mem_data_out)
	);
	
endmodule
