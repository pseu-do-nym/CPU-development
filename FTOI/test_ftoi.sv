`timescale 1ns / 100ps
`default_nettype none

module test_ftoi();
   wire [31:0] x,y;
   logic [31:0] xi;
   real         fx;
   int          i,it,fy;
   bit [22:0]   m;
   bit [9:0]    dum;
   logic [31:0] fybit;
   int          s;
   logic [23:0] dy;

   assign x = xi;
   
   ftoi u1(x,1'b0,y);

   initial begin
      // $dumpfile("test_finv.vcd");
      // $dumpvars(0);

      for (i=1; i<255; i++) begin // x1の指数部 0,255にならないようにした
            for (s=0; s<2; s++) begin // x1の符号
                  for (it=0; it<10; it++) begin
                        #1;

                        case (it) // x1の仮数部
                          0 : m = 23'b0;
                          1 : m = {22'b0,1'b1};
                          2 : m = {21'b0,2'b10};
                          3 : m = {1'b0,3'b111,19'b0};
                          4 : m = {1'b1,22'b0};
                          5 : m = {2'b10,{21{1'b1}}};
                          6 : m = {23{1'b1}};
                          default : begin
                             if (i==256) begin
                                {m,dum} = 0;
                             end else begin
                                {m,dum} = $urandom();
                             end
                          end
                        endcase
                        
                        xi = {s[0],i[7:0],m};

                        fx = $bitstoreal(xi);
                        fy = $rtoi(fx);
                        
                        #1;

                        if (1 == 1) begin
                            $display("x = %b ", x);
                            $display("%b %b \n", fy, y);
                        end
                 end
            end
      end

      for (i=1; i<255; i++) begin // 1-254にした
         for (s=0; s<2; s++) begin
                  repeat(10) begin
                     #1;

                     {m,dum} = $urandom();
                     xi = {s[0],i[7:0],m};

                     fx = $bitstoreal(xi);
                     fy = $rtoi(fx);
                        
                     #1;

                     if (1 == 1) begin
                        $display("x = %b ", x);
                        $display("%b %b \n", fy, y);
                     end
           
                  end
         end
      end
      $finish;
   end
endmodule

`default_nettype wire
