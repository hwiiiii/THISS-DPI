module alutop (


input [6:0] opcode,
input [6:0] func7,
input [2:0] func3,
input [63:0] rs1,
input [63:0] rs2,
input [19:0] imm,
input [63:0] pc,
input i_valid,

input [4:0] i_rd_indx, //added
output [4:0] o_rd_indx, //to reg_file 

output o_valid, //to reg_file
output [63:0] rd //to reg_file
);

assign o_valid=i_valid;
assign o_rd_indx = i_rd_indx; //to reg_file with rd(result) at next edge


wire [4:0] inst_name;
wire ADDorSUB;
wire typeI;
wire typeSigned;
wire typeWord;


alu_ver2 ALU1 (


//    .reset_n(reset_n),//
    
    .rs1(rs1),
    .rs2(rs2),
    .imm(imm),
    .pc(pc),
    
    .inst_name(inst_name),
    
    .ADDorSUB(ADDorSUB),
    .typeI(typeI),
    .typeSigned(typeSigned),
    .typeWord(typeWord),
    
    .rd(rd)
    
);

alu_ctrl ALUCTRL (
    .opcode(opcode),
    .func7(func7),
    .func3(func3),

    .inst_name(inst_name),
    .ADDorSUB(ADDorSUB),
    .typeI(typeI),
    .typeSigned(typeSigned),
    .typeWord(typeWord) 
);


endmodule
