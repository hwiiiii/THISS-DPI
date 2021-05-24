module alu_reg (

input clk,

input [6:0] opcode,
input [6:0] func7,
input [2:0] func3,
input [63:0] rs1, //from reg_file
input [63:0] rs2, //from reg_file
input [19:0] imm,
input [63:0] pc,
input i_valid,

input [4:0] i_rd_indx, //added


output reg [6:0] r_opcode,
output reg [6:0] r_func7,
output reg [2:0] r_func3,
output reg [63:0] r_rs1, 
output reg [63:0] r_rs2, 
output reg [19:0] r_imm,
output reg [63:0] r_pc,
output reg r_i_valid,

output reg [4:0] r_i_rd_indx //added


);


always@(posedge clk)
begin

 r_opcode <= opcode;
 r_func7<=func7;
 r_func3<=func3;
 r_rs1<=rs1;
 r_rs2 <=rs2;
 r_imm<=imm;
 r_pc<=pc;
 r_i_valid<=i_valid;

 r_i_rd_indx<=i_rd_indx;

end



endmodule
