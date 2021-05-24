module alu_ctrl(
input [6:0] opcode,
input [6:0] func7,
input [2:0] func3,
output [4:0] inst_name,
output ADDorSUB,
output typeI,
output typeSigned,
output typeWord
);

reg [4:0] r_inst;
reg [4:0] i_inst;
reg [4:0] u_inst;

//R-type
always @*
begin
case({opcode,func7[0],func3,func7[5]})
12'b0110011_0_000_0: r_inst=5'b00000;   //0 add
12'b0111011_0_000_0: r_inst=5'b00001;   //1 addw
12'b0110011_0_000_1: r_inst=5'b00010;   //2 sub
12'b0111011_0_000_1: r_inst=5'b00011;   //3 subw
12'b0110011_0_001_0: r_inst=5'b00100;   //4 sll

12'b0111011_0_001_0: r_inst=5'b00101;   //5 sllw
12'b0110011_0_010_0: r_inst=5'b00110;   //6 slt
12'b0110011_0_011_0: r_inst=5'b00111;   //7 sltu
12'b0110011_0_100_0: r_inst=5'b01000;   //8 xor
12'b0110011_0_101_0: r_inst=5'b01001;   //9 srl

12'b0111011_0_101_0: r_inst=5'b01010;   //10 srlw
12'b0110011_0_101_1: r_inst=5'b01011;   //11 sra
12'b0111011_0_101_1: r_inst=5'b01100;   //12 sraw
12'b0110011_0_110_0: r_inst=5'b01101;   //13 or
12'b0110011_0_111_0: r_inst=5'b01110;   //14 and

default r_inst=5'b11111;    //fail value
endcase
end

//I-type
always@*
begin
case({opcode,func3,func7[5]})
11'b0010011_000_0,11'b0010011_000_1: i_inst=5'b01111;  //15 addi
11'b0011011_000_0,11'b0011011_000_1: i_inst=5'b10000;  //16 addiw
11'b0010011_010_0,11'b0010011_010_1: i_inst=5'b10001;  //17 slti
11'b0010011_011_0,11'b0010011_011_1: i_inst=5'b10010;  //18 sltiu
11'b0010011_100_0,11'b0010011_100_1: i_inst=5'b10011;  //19 xori


11'b0010011_110_0,11'b0010011_110_1: i_inst=5'b10100;  //20 ori
11'b0010011_111_0,11'b0010011_111_1: i_inst=5'b10101;  //21 andi

11'b0010011_001_0: i_inst=5'b10110;  //22 slli
11'b0011011_001_0: i_inst=5'b10111;  //23 slliw
11'b0010011_101_0: i_inst=5'b11000;  //24 srli


11'b0011011_101_0: i_inst=5'b11001;  //25 srliw
11'b0010011_101_1: i_inst=5'b11010;  //26 srai
11'b0011011_101_1: i_inst=5'b11011;  //27 sraiw
default i_inst=5'b11111;   //not I type
endcase
end

//U-type
always@*
begin
case(opcode)
7'b0110111: u_inst=5'b11100; //28 lui
7'b0010111: u_inst=5'b11101; //29 auipc
default u_inst=5'b11111;   //not u type
endcase
end

assign inst_name=(r_inst!=5'b11111) ? r_inst:
                 (i_inst!=5'b11111) ? i_inst:    
                 (u_inst!=5'b11111) ? u_inst:
                                    5'b11111;

assign ADDorSUB=(r_inst==5'b00000||r_inst==5'b00001||i_inst==5'b01111||i_inst==5'b10000) ? 1'b1 : 1'b0;

assign typeI=(r_inst==5'b11111) ? 1'b1 : 1'b0;

assign typeSigned = (r_inst==5'b00111||i_inst==5'b10010) ? 1'b0 : 1'b1;

assign typeWord = (r_inst==5'b00001||r_inst==5'b00011||r_inst==5'b00101||r_inst==5'b01010||r_inst==5'b01100
                 ||i_inst==5'b10000||i_inst==5'b10111||i_inst==5'b11001||i_inst==5'b11011) ? 1'b1 : 1'b0;

endmodule                   
