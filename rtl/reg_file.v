// --=========================================================================--
// Copyright (c) 2021 DSAL, EWHA Womans University. All rights reserved
//                     DSAL Confidential Proprietary
//  ----------------------------------------------------------------------------
//        This confidential and proprietary software may be used only as
//            authorised by a licensing agreement from DSAL.
//
//         The entire notice above must be reproduced on all authorised
//          copies and copies may only be made to the extent permitted
//                by a licensing agreement from DSAL.
//
//      The entire notice above must be reproduced on all authorized copies.
//
// -----------------------------------------------------------------------------
// FILE NAME       : reg_file.v
// DEPARTMENT      : Digital System Architecture Lab.
// AUTHOR          : Sohyeon Kim
// AUTHOR'S EMAIL  : ee.shkim95@gmail.com
// -----------------------------------------------------------------------------
// RELEASE HISTORY
// VERSION DATE         AUTHOR         DESCRIPTION
// 1.0     2021-04-12   Sohyeon Kim    module for Architectural register file
// -----------------------------------------------------------------------------
// PURPOSE      :  module for Architectural register file
// -----------------------------------------------------------------------------

module reg_file #(parameter DWIDTH=64) (

    input               clk,
    input               reset_n,

    //read operand
    input   [4:0]       rs1_idx,
    input   [4:0]       rs2_idx,

    input   [4:0]        rd_idx,
    input                wr_en,
    input   [DWIDTH-1:0] wr_data,

    output [DWIDTH-1:0] rd_data_1, //rd : read data of src operand1
    output [DWIDTH-1:0] rd_data_2  //rd : read data of src operand2

//    output reg [DWIDTH-1:0] x[0:31]     
); 

integer i;

//Architectural Register File
reg     [DWIDTH-1:0]    x[0:31];


always@(posedge clk or negedge reset_n) // write
begin
    if(!reset_n)
    begin
        for (i = 0; i < 32; i = i+1)
            x[i] <= 64'h0;
    end

    else if(wr_en) 
    begin
        x[rd_idx] <= wr_data;        
    end

end


//combinational read
assign rd_data_1 = x[rs1_idx];
assign rd_data_2 = x[rs2_idx];


endmodule
