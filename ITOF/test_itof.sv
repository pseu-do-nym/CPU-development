`timescale 1ns / 100ps
`default_nettype none

module test_itof();
   wire [31:0] x,y;
   logic [31:0] xi;
   int          i,it;
   bit [22:0]   m;
   bit [9:0]    dum;
   logic [31:0] fybit;
   int          s;
   logic [23:0] dy;

   assign x = xi;
   
   itof u1(x,y);

   initial begin
      // $dumpfile("test_finv.vcd");
      // $dumpvars(0);

      for (i=1; i<255; i++) begin // x1の指数部 0,255にならないようにした
                        
          #1;
          xi = $random();
          $display("x = %b ", x);
          $display("%b \n", y);

      end
   end
   
endmodule

`default_nettype wire
