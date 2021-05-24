module alu_decoder(

    input               [31:0]  inst,   // instruction
   
    output      reg     [6:0]   opcode,
    output      reg     [2:0]   func3,
    output      reg     [6:0]   func7,

    output      reg     [19:0]  imm,

    output      reg     [4:0]   rs1_indx,
    output      reg     [4:0]   rs2_indx,
    output      reg     [4:0]   rd_indx

);

always @ *
begin   
     opcode = inst[6:0];
     func3  = inst[14:12];
     func7  = inst[31:25];

     imm    = inst[31:12];

     rs1_indx    = inst[19:15];
     rs2_indx    = inst[24:20];
     rd_indx     = inst[11:7];
end


endmodule

