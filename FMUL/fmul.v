`timescale 1ns / 1ps
`default_nettype none

module fmul(
  input wire [31:0] x1,
  input wire [31:0] x2,
  output wire [31:0] y,
  output wire ovf/*,
  output wire [9:0] eo,
  output wire [22:0] mo */);

  wire [0:0] s1;
  wire [7:0] e1;
  wire [22:0] m1;
  wire [0:0] s2;
  wire [7:0] e2;
  wire [22:0] m2;

  assign s1 = x1[31];
  assign e1 = x1[30:23];
  assign m1 = x1[22:0];
  assign s2 = x2[31];
  assign e2 = x2[30:23];
  assign m2 = x2[22:0];

  wire [23:0] m1a;
  wire [23:0] m2a;

  assign m1a = (e1 == 8'b0) ? {1'b0,m1[22:0]} : {1'b1,m1[22:0]};
  assign m2a = (e2 == 8'b0) ? {1'b0,m2[22:0]} : {1'b1,m2[22:0]};

  wire [7:0] e1a;
  wire [7:0] e2a;

  assign e1a = (e1 == 8'b0) ? 8'b1 : e1[7:0];
  assign e2a = (e2 == 8'b0) ? 8'b1 : e2[7:0];
  
  wire signed [8:0] e1b;
  wire signed [8:0] e2b;

  assign e1b = {1'b0,e1a[7:0]} - 127;
  assign e2b = {1'b0,e2a[7:0]} - 127;

  wire [0:0] ss;
  wire signed [9:0] ea;
  
  assign ss = s1 ^ s2;
  assign ea = e1b + e2b + 127;
  
  wire [47:0] myd;
  
  assign myd =  m1a * m2a;
  
  wire [5:0] se;
  
  assign se = (myd[47] == 0) ? ((myd[46] == 0) ? ((myd[45] == 0) ? ((myd[44] == 0) ? ((myd[43] == 0) ? ((myd[42] == 0) ? ((myd[41] == 0) ? ((myd[40] == 0) ? ((myd[39] == 0) ? ((myd[38] == 0) ? ((myd[37] == 0) ? ((myd[36] == 0) ? ((myd[35] == 0) ? ((myd[34] == 0) ? ((myd[33] == 0) ? ((myd[32] == 0) ? ((myd[31] == 0) ? ((myd[30] == 0) ? ((myd[29] == 0) ? ((myd[28] == 0) ? ((myd[27] == 0) ? ((myd[26] == 0) ? ((myd[25] == 0) ? ((myd[24] == 0) ? ((myd[23] == 0) ? ((myd[22] == 0) ? ((myd[21] == 0) ? ((myd[20] == 0) ? ((myd[19] == 0) ? ((myd[18] == 0) ? ((myd[17] == 0) ? ((myd[16] == 0) ? ((myd[15] == 0) ? ((myd[14] == 0) ? ((myd[13] == 0) ? ((myd[12] == 0) ? ((myd[11] == 0) ? ((myd[10] == 0) ? ((myd[9] == 0) ? ((myd[8] == 0) ? ((myd[7] == 0) ? ((myd[6] == 0) ? ((myd[5] == 0) ? ((myd[4] == 0) ? ((myd[3] == 0) ? ((myd[2] == 0) ? ((myd[1] == 0) ? ((myd[0] == 0) ? 48 : 47): 46) : 45) : 44) : 43) : 42) : 41) : 40) : 39) : 38) : 37) : 36) : 35) : 34) : 33) : 32) : 31) : 30) : 29) : 28) : 27) : 26) : 25) : 24) : 23) : 22) : 21) : 20) : 19) : 18) : 17) : 16) : 15) : 14) : 13) : 12) : 11) : 10) : 9) : 8) : 7) : 6) : 5) : 4) : 3) : 2) : 1) : 0;
  
  wire [47:0] mb;
  wire [47:0] mbs;
  wire signed [9:0] eb;
  wire signed [9:0] ebs;
  
  assign mb = myd << (se + 1);
  assign eb = ea - {5'b0,se} + 1;
  assign ebs = -eb + 1;
  assign mbs = (eb[9] == 1) ? mb >> ebs : mb;
  
  assign ovf = (eb[9:8] == 2'b01);
  assign y = (eb[9] == 1) ? ((eb > -23) ? {ss,8'b0,mbs[47:25]} : {ss,31'b0} ) : ((eb[8] == 1) ? {ss,8'd255,23'b0} : {ss,eb[7:0],mbs[47:25]} );
//  assign eo = eb[9:0];
//  assign mo = mbs[47:25];
endmodule
`default_nettype wire


