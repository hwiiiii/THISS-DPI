module alu_ver2 (

//input reset_n,

input [63:0] rs1,
input [63:0] rs2,
input [19:0] imm, //20bit imm
input [63:0] pc,

input [4:0] inst_name,

input ADDorSUB, //1->ADD, 0->SUB
input typeI, //    1->IMM, 0->R
input typeSigned, //1->signed, 0->unsigned
input typeWord, //1->word, 0->64bit

//input [4:0]i_rd_indx,
//output [4:0]o_rd_indx,


output wire [63:0] rd


);

localparam ADD = 5'b00000;
localparam ADDW = 5'b00001;
localparam SUB = 5'b00010;
localparam SUBW = 5'b00011;
localparam SLL = 5'b00100;

localparam SLLW = 5'b00101;
localparam SLT = 5'b00110;
localparam SLTU = 5'b00111;
localparam XOR = 5'b01000;
localparam SRL = 5'b01001;

localparam SRLW = 5'b01010;
localparam SRA = 5'b01011;
localparam SRAW = 5'b01100;
localparam OR = 5'b01101;
localparam AND = 5'b01110;

localparam ADDI = 5'b01111;
localparam ADDIW = 5'b10000;
localparam SLTI = 5'b10001;
localparam SLTIU = 5'b10010;
localparam XORI = 5'b10011;

localparam ORI = 5'b10100;
localparam ANDI = 5'b10101;
localparam SLLI = 5'b10110;
localparam SLLIW = 5'b10111;
localparam SRLI = 5'b11000;

localparam SRLIW = 5'b11001;
localparam SRAI = 5'b11010;
localparam SRAIW = 5'b11011;
localparam LUI = 5'b11100;
localparam AUIPC = 5'b11101;

reg [63:0] alu_result;

// signed rs1 for arithmetic shift, slt
wire signed [63:0] rs1_signed;

assign             rs1_signed = rs1;

//imm assign

wire [11:0] imm_I;
wire [19:0] imm_U;

assign imm_I = imm [19:8];
assign imm_U = imm;


//add, sub
wire [63:0] result_addsub;
wire [63:0] op2_addsub;
wire [63:0] addsub_6result;

assign      op2_addsub = typeI ? {{52{imm_I[11]}},imm_I} :
                                (ADDorSUB ? rs2 : (~rs2 + 1) ); 

assign      result_addsub = rs1 + op2_addsub ;

assign      addsub_6result = (typeWord) ? { {32{result_addsub[31]}}, result_addsub[31:0]} :  result_addsub;


//slt,sltu

wire        [63:0] op2_slt;
wire signed [63:0] op2_slt_signed;
wire               less;

assign    op2_slt = typeI ? {{52{imm_I[11]}},imm_I} : rs2;

assign    op2_slt_signed = op2_slt;

assign    less = (typeSigned) ? (rs1_signed < op2_slt_signed) : (rs1 < op2_slt) ;


//shift

//sll
wire [63:0] sll_result;

assign      sll_result = rs1 << rs2[5:0];

//slli legal --when shamt[5] = 0
wire [63:0] slli_result;

assign      slli_result = imm_I[5] ? 64'b0 : rs1 << imm_I[5:0];

//sllw
wire [63:0] sllw_shifted;
wire [31:0] sllw_shifted_32;
wire [63:0] sllw_result;

assign      sllw_shifted = rs1 << rs2[4:0];

assign      sllw_shifted_32 = sllw_shifted[31:0];

assign      sllw_result = {{32{sllw_shifted_32[31]}},sllw_shifted_32};

//slliw--when shamt[5] = 0
wire [63:0] slliw_shifted;
wire [31:0] slliw_shifted_32;
wire [63:0] slliw_result;

assign      slliw_shifted = rs1 << imm_I[4:0];

assign      slliw_shifted_32 = slliw_shifted[31:0];

assign      slliw_result = imm_I[5] ? 64'b0 : {{32{slliw_shifted_32[31]}},slliw_shifted_32};

//left logical result confirm
wire [63:0] sll_4result;
assign      sll_4result = typeI ? 
                          (typeWord ? slliw_result : slli_result) : 
                          (typeWord ? sllw_result : sll_result);

//////////////////////////////////////////////////////////

//srl
wire [63:0] srl_result;

assign      srl_result = rs1 >> rs2[5:0];

//srli--when shamt[5] = 0
wire [63:0] srli_result;

assign      srli_result =  imm_I[5] ? 64'b0 : rs1 >> imm_I[5:0];

//srlw
wire [31:0] srlw_shifted;
wire [63:0] srlw_result;

assign      srlw_shifted = rs1[31:0] >> rs2[4:0];

assign      srlw_result = {{32{srlw_shifted[31]}},srlw_shifted};

//srliw--when shamt[5] = 0
wire [31:0] srliw_shifted;
wire [63:0] srliw_result;

assign      srliw_shifted = (rs1[31:0] >> imm_I[4:0]);

assign      srliw_result =  imm_I[5] ? 64'b0 : {{32{srliw_shifted[31]}},srliw_shifted};

//right logical result confirm

wire [63:0] srl_4result;

assign      srl_4result = typeI ? 
                         (typeWord ? srliw_result : srli_result) : 
                         (typeWord ? srlw_result : srl_result);


////////////////////////////////////////////////////////////
//arithmetic shift needs signed operands

//sra
wire [63:0] sra_result;

assign      sra_result = rs1_signed>>> rs2[5:0];

//srai--when shamt[5] = 0
wire signed [63:0] srai_shifted;
wire [63:0] srai_result;

assign      srai_shifted = rs1_signed >>> imm_I[5:0];

assign      srai_result = imm_I[5] ? 64'b0 : srai_shifted;

//sraw
wire        [31:0] sraw_shifted;
wire signed [31:0] rs1_signed_cut;
wire        [63:0] sraw_result;

assign      rs1_signed_cut = rs1_signed[31:0];

assign      sraw_shifted = rs1_signed_cut >>> rs2[4:0];

assign      sraw_result = {{32{sraw_shifted[31]}},sraw_shifted};

//sraiw
wire [31:0] sraiw_shifted;
wire [63:0] sraiw_result;

assign      sraiw_shifted = (rs1_signed_cut >>> imm_I[4:0]);

assign      sraiw_result = imm_I[5] ? 64'b0: {{32{sraiw_shifted[31]}},sraiw_shifted}; //--when shamt[5] = 0

//right arithmetic result confirm
wire [63:0] sra_4result;

assign      sra_4result = typeI ? 
                         (typeWord ? sraiw_result : srai_result) : 
                         (typeWord ? sraw_result : sra_result);

//xor, xori

wire [63:0] xor_2result;

assign      xor_2result = (typeI)? rs1 ^ {{52{imm_I[11]}},imm_I} :  rs1 ^ rs2; //64-12=52

//or, ori

wire [63:0] or_2result;

assign      or_2result = (typeI)? rs1 | {{52{imm_I[11]}},imm_I}  :  rs1 | rs2;

//and, andi

wire [63:0] and_2result;

assign      and_2result = (typeI)? rs1 & {{52{imm_I[11]}},imm_I}  :  rs1 & rs2;


//lui, auipc

wire [63:0] lui_result;
wire [63:0] auipc_result;

assign lui_result = {{32{imm_U[19]}}, imm_U, 12'b0};

assign auipc_result = pc + lui_result;



always @*
begin
//
//    if(!reset_n)
//        alu_result = 64'b0;
//    else begin

//      

    case(inst_name)
    
        ADD, ADDW, SUB, SUBW, ADDI, ADDIW :
            alu_result = addsub_6result;
        
        SLT, SLTU, SLTI, SLTIU :     
            alu_result = {{63{1'b0}}, less};
        
        SLL, SLLW, SLLI, SLLIW :     
            alu_result = sll_4result;
        
        SRL, SRLW, SRLI, SRLIW : 
            alu_result = srl_4result;
        
        SRA, SRAW, SRAI, SRAIW : 
            alu_result = sra_4result;
        
        XOR, XORI :  
            alu_result = xor_2result;
        
        OR,ORI  : 
            alu_result = or_2result;
        
        AND, ANDI : 
            alu_result = and_2result;
        
        LUI : 
            alu_result = lui_result; 
        
        AUIPC : 
            alu_result = auipc_result;
        
        default : 
            alu_result = 64'h0;
        
    endcase
//    end
end 

assign rd = alu_result;

endmodule
