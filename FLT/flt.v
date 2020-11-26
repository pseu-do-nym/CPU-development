`timescale 1ns / 1ps
`default_nettype none

module flt( //s1<s2ならtrue(1)を返す
  input wire [31:0] x1,
  input wire [31:0] x2,
  output wire v);

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

  wire [24:0] m1a;
  wire [24:0] m2a;

  assign m1a = (e1 == 8'b0) ? {2'b0,m1[22:0]} : {1'b0,1'b1,m1[22:0]};
  assign m2a = (e2 == 8'b0) ? {2'b0,m2[22:0]} : {1'b0,1'b1,m2[22:0]};

  wire [7:0] e1a;
  wire [7:0] e2a;

  assign e1a = (e1 == 8'b0) ? 8'b1 : e1[7:0];
  assign e2a = (e2 == 8'b0) ? 8'b1 : e2[7:0];

  wire [1:0] sel;
  wire [1:0] ce;

  assign sel = (e1a < e2a) ? 0 : ((e1a > e2a) ? 1 : 2);
  assign ce = (m1a < m2a) ? 0 : ((m1a > m2a) ? 1 : 2);

  assign v = (s1 == s2) ? ((s1 == 0) ? ((sel == 0 || (sel == 2 && ce == 0) ? 1 : 0 )) :
                                       ((sel == 1 || (sel == 2 && ce == 1)) ? 1 : 0 ))
                                        : ((s1 == 1 && ({e1,m1} != 31'b0 || {e2,m2} != 31'b0)) ? 1 : 0 );

endmodule

`default_nettype wire


