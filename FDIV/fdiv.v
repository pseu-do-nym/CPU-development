`timescale 1ns / 1ps
`default_nettype none

module fdiv(
  input wire [31:0] x1,
  input wire [31:0] x2,
  output wire [31:0] y,
  output wire ovf/*,
  output wire [9:0] eo,
  output wire [22:0] mo */);
  
  wire [0:0] s1;
  wire [7:0] e1;
  wire [22:0] m1;

  assign s1 = x1[31];
  assign e1 = x1[30:23];
  assign m1 = x1[22:0];
  
  wire [31:0] x2i;

  finv u1(x2,x2i);
  
  wire [0:0] s2i;
  wire [7:0] e2i;
  wire [22:0] m2i;
  
  assign s2i = x2i[31];
  assign e2i = x2i[30:23];
  assign m2i = x2i[22:0];
  
  wire [31:0] ans;
  wire o;
  
  fmul u2(x1,x2i,ans,o);
  
  assign ovf = o;
  assign y = ans;
//  assign eo = eb[9:0];
//  assign mo = mbs[47:25];
endmodule
`default_nettype wire


