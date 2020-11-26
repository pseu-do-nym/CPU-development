`timescale 1ns / 100ps
`default_nettype none

module test_fmul();
   wire [31:0] x1,x2,y;
   logic [31:0] x1i,x2i;
   shortreal    fx1,fx2,fy;
   int          i,j,k,it,jt;
   bit [22:0]   m1,m2;
   bit [9:0]    dum1,dum2;
   logic [31:0] fybit;
   int          s1,s2;
   logic [23:0] dy;
   bit [22:0] tm;

   assign x1 = x1i;
   assign x2 = x2i;
   
   fmul u1(x1,x2,y);

   initial begin
      // $dumpfile("test_fmul.vcd");
      // $dumpvars(0);

      for (i=1; i<255; i++) begin // x1の指数部 0,255にならないようにした
         for (j=1; j<255; j++) begin // x2の指数部 0,255にならないようにした
            for (s1=0; s1<2; s1++) begin // x1の符号
               for (s2=0; s2<2; s2++) begin // x2の符号
                  for (it=0; it<10; it++) begin
                     for (jt=0; jt<10; jt++) begin
                        #1;

                        case (it) // x1の仮数部
                          0 : m1 = 23'b0;
                          1 : m1 = {22'b0,1'b1};
                          2 : m1 = {21'b0,2'b10};
                          3 : m1 = {1'b0,3'b111,19'b0};
                          4 : m1 = {1'b1,22'b0};
                          5 : m1 = {2'b10,{21{1'b1}}};
                          6 : m1 = {23{1'b1}};
                          default : begin
                             if (i==256) begin
                                {m1,dum1} = 0;
                             end else begin
                                {m1,dum1} = $urandom();
                             end
                          end
                        endcase

                        case (jt) // x2の仮数部
                          0 : m2 = 23'b0;
                          1 : m2 = {22'b0,1'b1};
                          2 : m2 = {21'b0,2'b10};
                          3 : m2 = {1'b0,3'b111,19'b0};
                          4 : m2 = {1'b1,22'b0};
                          5 : m2 = {2'b10,{21{1'b1}}};
                          6 : m2 = {23{1'b1}};
                          default : begin
                             if (i==256) begin
                                {m2,dum2} = 0;
                             end else begin
                                {m2,dum2} = $urandom();
                             end
                          end
                        endcase
                        
                        x1i = {s1[0],i[7:0],m1};
                        x2i = {s2[0],j[7:0],m2};

                        fx1 = $bitstoshortreal(x1i);
                        fx2 = $bitstoshortreal(x2i);
                        fy = fx1 * fx2; // + -> *
                        fybit = $shortrealtobits(fy);
                        
                        #1;

                        if (fybit[30:23] == 0) begin
                           if (y[30:23] !== 0) begin
                              $display("x1, x2 = %b %b", x1, x2);
                              $display("%e %b \n", $bitstoshortreal(y), y);
                           end
                        end else begin
                           if (fybit[30:23]<255 && !(y+32'd1 === fybit || y === fybit || y-32'd1 === fybit)) begin  
                              $display("x1, x2 = %b %b", x1, x2);
                              $display("%e %b ", fy, $shortrealtobits(fy));
                              $display("%e %b \n", $bitstoshortreal(y), y);
                           end
                        end
                     end
                  end
               end
            end
         end
      end

      for (i=1; i<255; i++) begin // 1-254にした
         for (s1=0; s1<2; s1++) begin
            for (s2=0; s2<2; s2++) begin
               for (j=0;j<23;j++) begin
                  repeat(10) begin
                     #1;

                     {m1,dum1} = $urandom();
                     x1i = {s1[0],i[7:0],m1};
                     {m2,dum2} = $urandom();
                     for (k=0;k<j;k++) begin
                        tm[k] = m2[k];
                     end
                     for (k=j;k<23;k++) begin
                        tm[k] = m1[k];
                     end
                     x2i = {s2[0],i[7:0],tm};

                     fx1 = $bitstoshortreal(x1i);
                     fx2 = $bitstoshortreal(x2i);
                     fy = fx1 * fx2; // + -> *
                     fybit = $shortrealtobits(fy);
                     

                     #1;
                     if (fybit[30:23] == 0) begin
                        if (y[30:23] !== 0) begin
                           $display("x1, x2 = %b %b", x1, x2);
                           $display("%e %b \n", $bitstoshortreal(y), y);
                        end
                     end else begin
                        if (!(y+32'd1 === fybit || y === fybit || y-32'd1 === fybit) && fybit[30:23] != 255)  begin
                           $display("x1, x2 = %b %b", x1, x2);
                           $display("%e %b ", fy, $shortrealtobits(fy));
                           $display("%e %b \n", $bitstoshortreal(y), y);
                        end
                     end                  
                  end
               end
            end
         end
      end
      $finish;
   end
endmodule

`default_nettype wire
