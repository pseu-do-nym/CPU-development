`timescale 1ns / 1ps
`default_nettype none

module ftoi(
  input wire [31:0] x,
  input wire rm,
  output wire [31:0] y);

  wire [0:0] s;
  wire [7:0] e;
  wire [22:0] m;

  assign s = x[31];
  assign e = x[30:23];
  assign m = x[22:0];
  
  wire [7:0]  d;
  wire [31:0] i;
  wire [30:0] round;
  wire [30:0] ir;
  
  assign d = (e > 8'd127 && e < 8'd159) ? 8'd158 - e : 8'b0; // ずらす桁
  assign i = {1'b1,m,8'b0} >> d;
  assign round = (rm == 0 || s[0] == 0) ? i[31:0] : (i[31:0]) - 1; // rm == 0 で0丸め(端数無視)、 rm == 1 で最近接丸め(絶対値四捨五入)
//  assign round = (rm == 0 || i[0] == 0) ? i[31:0] : (i[31:0]) + 1; // rm == 0 で0丸め(端数無視)、 rm == 1 で最近接丸め(絶対値四捨五入)
  assign ir = (s == 0) ? round : (~round) + 1; // bit反転(補数)
  
  assign y = (e < 8'd127) ? 32'b0 : //指数部が0以下
             ((e > 8'd159) ? 32'b11111111111111111111111111111111 : //inf
             {s,ir} // その他
             );

endmodule

`default_nettype wire


