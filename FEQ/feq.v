`timescale 1ns / 1ps
`default_nettype none

module feq( //s1==s2ならtrue(1)を返す
  input wire [31:0] x1,
  input wire [31:0] x2,
  output wire v);

  wire [7:0] e1;
  wire [7:0] e2;

  assign e1 = x1[30:23];
  assign e2 = x2[30:23];

  assign v = (x1 == x2 || (e1 == 8'b0 && e2 == 8'b0)) ? 1 : 0; 

endmodule

`default_nettype wire


