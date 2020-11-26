`timescale 1ns / 1ps
`default_nettype none

module itof(
  input wire [31:0] x,
  output wire [31:0] y);

  wire [31:0] abs;
  
  assign abs = (x[31] == 0) ? x : (~x)+1 ;
  
  wire [4:0] shift;
  
  assign shift = (abs[30] == 1) ? 1 :
                ((abs[29] == 1) ? 2 :
                ((abs[28] == 1) ? 3 :
                ((abs[27] == 1) ? 4 :
                ((abs[26] == 1) ? 5 :
                ((abs[25] == 1) ? 6 :
                ((abs[24] == 1) ? 7 :
                ((abs[23] == 1) ? 8 :
                ((abs[22] == 1) ? 9 :
                ((abs[21] == 1) ? 10 :
                ((abs[20] == 1) ? 11 :
                ((abs[19] == 1) ? 12 :
                ((abs[18] == 1) ? 13 :
                ((abs[17] == 1) ? 14 :
                ((abs[16] == 1) ? 15 :
                ((abs[15] == 1) ? 16 :
                ((abs[14] == 1) ? 17 :
                ((abs[13] == 1) ? 18 :
                ((abs[12] == 1) ? 19 :
                ((abs[11] == 1) ? 20 :
                ((abs[10] == 1) ? 21 :
                ((abs[9] == 1) ? 22 :
                ((abs[8] == 1) ? 23 :
                ((abs[7] == 1) ? 24 :
                ((abs[6] == 1) ? 25 :
                ((abs[5] == 1) ? 26 :
                ((abs[4] == 1) ? 27 :
                ((abs[3] == 1) ? 28 :
                ((abs[2] == 1) ? 29 :
                ((abs[1] == 1) ? 30 : 
                ((abs[0] == 1) ? 31 : 0
                )))))))))))))))))))))))))))))); //最初に出た1が消えるまでシフト。
                
  wire [4:0] rs;
  wire [30:0] rx;
  
  assign rs = 30 - shift; //rs == 31でもしてくれる？？
  assign rx = (abs[30:0]) << shift;
  assign y = (shift == 0) ? {31'b0} : // x == 0
            ((shift == 31) ? {x[31],1'b0,7'b1111111,23'b0} : // x == 1 or -1
            {x[31],3'b100,rs,rx[30:8]}
            );
endmodule

`default_nettype wire


