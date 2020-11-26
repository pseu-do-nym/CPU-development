`timescale 1ns / 100ps
`default_nettype none

module test_fsqrt();
   wire [31:0] x,y;
//   wire [23:0] xaaa,nib,hikare,ra;
   wire [47:0] an;
   logic [31:0] xi;
   shortreal    fx,fy;
   int          i,k,it;
   bit [22:0]   m;
   bit [7:0]    e;
   bit [22:0]    dum;
   logic [31:0] fybit;
   int          s;
   logic [23:0] dy;
   bit [22:0] tm;


   assign x = xi;
   
   fsqrt u1(x,y/*,xaaa,nib,hikare,ra,an*/);

   initial begin
      // $dumpfile("test_finv.vcd");
      // $dumpvars(0);

      for (i=0; i<1024*1024*8; i++) begin // 0 ~ 2^23-1
        #1;
        m = i[22:0];
        {s, e, dum} = {1'b0, 8'd99, 23'd1}; //e=odd version
        xi = {s,e,m};

        fx = $bitstoshortreal(xi);
        fy = $sqrt(fx);
        fybit = $shortrealtobits(fy);

        
        #1;

           if (!(y+32'd5 > fybit && y-32'd5 < fybit)) begin
              $display("x      =%e  %b", $bitstoshortreal(x), x);
              $display("true y =%e  %b ", fy, $shortrealtobits(fy));
              $display("my   y =%e  %b \n", $bitstoshortreal(y), y);
/*              $display("nib =%b", nib);
              $display("%b - %b = %b",hikare,xaaa,ra);
              $display("an =%b \n", an);*/
           end
        end
      
      begin: loop
      
      for (i=0; i<1024*1024*8; i++) begin // 0 ~ 2^23-1
        #1;
        m = i[22:0];
        {s, e, dum} = {1'b0, 8'd160, 23'd1};//e=even version
        xi = {s,e,m};

        fx = $bitstoshortreal(xi);
        fy = $sqrt(fx);
        fybit = $shortrealtobits(fy);

        #1;

           if (!(y+32'd5 > fybit && y-32'd5 < fybit)) begin
              $display("x      =%e  %b", $bitstoshortreal(x), x);
              $display("true y =%e  %b ", fy, $shortrealtobits(fy));
              $display("my   y =%e  %b \n", $bitstoshortreal(y), y);
/*              $display("nib =%b", nib);
              $display("%b - %b = %b",hikare,xaaa,ra);
              $display("an =%b \n", an);*/
              
              disable loop;
              
           end
        end
        
        end
        
      $finish;
   end
endmodule

`default_nettype wire
