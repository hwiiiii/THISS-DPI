
module alu_tb ();
//longint unsigned cinpc=0;

import "DPI-C" context function longint main(input longint rs1,input longint rs2, input longint unsigned cinpc); //(input longint unsigned cinpc);
//import "DPI-C" function void main();
integer instcnt=37;

//import "DPI-C" function int hello();


//import "DPI-C" function int inst_fetch();
//import "DPI-C" function void inst_exe();

//import "DPI-C" function void inst_exe();
//import "DPI-C" function unsigned long long  sext64();
//import "DPI-C" function unsigned long long  sext32();
//import "DPI-C" function void inst_exe();




//import "DPI-C" function int main(1, 'R');
//import "DPI-C" function int main(1, 'R');
//import "DPI-C" function void hello2();

reg [31:0] instver;
reg reset_n;
reg clk = 0;
reg [63:0] pc;
reg [31:0] inst_array_ver [0:50];

reg i_valid = 0; //from Issue Queue actually

reg [63:0] x [0:31];

wire [63:0] result;

///////////////////////

wire     [6:0]   opcode;
wire     [2:0]   func3;
wire     [6:0]   func7;

wire     [19:0]  imm;

wire     [4:0]   rs1_indx;
wire     [4:0]   rs2_indx;
wire     [4:0]   rd_indx; //from alu_decoder to alutop

wire     [4:0]   o_rd_indx; //from alutop to reg_file
//

wire     [6:0]   r_opcode;
wire     [2:0]   r_func3;
wire     [6:0]   r_func7;
wire     [63:0]  r_rd_data_1;
wire     [63:0]  r_rd_data_2;
wire     [19:0]  r_imm;
wire     [63:0]  r_pc;

wire             r_i_valid; 

wire     [4:0]   r_rd_indx; 







//assign fu_result = result;

alu_decoder  alu_decoder (
    
//input
    .inst           (instver),                     // instruction
//output -> input to MUl unit   
    .opcode         (opcode),
    .func3          (func3),
    .func7          (func7),
//output -> ALU unit (for I, U type inst
    .imm            (imm),
//output -> input to register file
    .rs1_indx            (rs1_indx),
    .rs2_indx            (rs2_indx),
//output -> input to alutop
    .rd_indx             (rd_indx)         

);






//general purpose register ( ARF )

wire    [63:0]  rd_data_1;
wire    [63:0]  rd_data_2;

reg_file  x_reg_file(

    .clk            (clk),
    .reset_n        (reset_n),
// input 
    .rs1_idx        (rs1_indx),         // from decode stg
    .rs2_idx        (rs2_indx),         // from decode stg

    .rd_idx         (o_rd_indx),       // from ALU stg
    .wr_en          (o_valid),      // from ALU unit
    .wr_data        (result),         // from ALU unit
// output -> to ALU unit
    .rd_data_1      (rd_data_1),
    .rd_data_2      (rd_data_2)

); 

//added start

alu_reg alu_register(

    .clk(clk),

    .opcode (opcode),   //input from alu_decoder
    .func7(func7),
    .func3(func3),
    .rs1(rd_data_1),    //input from reg_file
    .rs2(rd_data_2),
    .imm(imm),
    .pc(pc),            //input from ??
    .i_valid(i_valid),  //input from Iq
    
    .i_rd_indx(rd_indx),//input from alu_decoder


    .r_opcode (r_opcode), //output to alutop
    .r_func7(r_func7),
    .r_func3(r_func3),
    .r_rs1(r_rd_data_1),
    .r_rs2(r_rd_data_2),
    .r_imm(r_imm),
    .r_pc(r_pc), 
    .r_i_valid(r_i_valid), 
    
    .r_i_rd_indx(r_rd_indx) 

);
//added end



alutop alutop(

    .opcode (r_opcode),
    .func7(r_func7),
    .func3(r_func3),
    .rs1(r_rd_data_1),
    .rs2(r_rd_data_2),

    .imm(r_imm),
    .pc(r_pc), //from ??
    .i_valid(r_i_valid), //from Issue Queue (all operand ready)
    
    .i_rd_indx(r_rd_indx), //input from alu_decoder
    .o_rd_indx(o_rd_indx), //output to reg_file

    .o_valid(o_valid), //to ROB (reslut ready)
    .rd(result)

);




initial begin

reset_n = 0;
#7;
reset_n = 1;

end




always #5   clk  <= ~clk;


initial begin // get instructions

$readmemh ("alu_test.hex" , inst_array_ver);

end
longint unsigned i;
longint res;
integer j;
initial begin
    instver = 32'b0;
    pc = 64'hffff_0000_2432_2317;
    //i_valid = 1;
#45;
//    i_valid = 1;
    for (i=0; i<=instcnt ; i=i+1) begin
    
        #10;
        
        i_valid = 1;
        instver = inst_array_ver [i];  
 
     if(i>=1) begin
         res=main(rd_data_1, rd_data_2, 4*(i-1));
       // main();
         $display("instver=%d           GPR[%d]=%h\n\n",i,rd_indx, result);  
        j=(res==result);
        $display ("same?= %d\n\n\n",j);
        end 
    end

    #(1000);
    $finish;
end




//initial begin
//    #20
   //  $display("%h",main());  
  // $display("%d",c);

//    $dumpfile("alu_tb.vcd");
//   $dumpvars(0, alu_tb);
 //   $shm_open("alusim");
//    $shm_probe("ACM");
//end

endmodule





