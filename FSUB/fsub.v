`timescale 1ns / 1ps
`default_nettype none

module fsub(
  input wire [31:0] x1,
  input wire [31:0] x2,
  output wire [31:0] y,
  output wire ovf);

  wire [0:0] s1;
  wire [7:0] e1;
  wire [22:0] m1;
  wire [0:0] s2;
  wire [7:0] e2;
  wire [22:0] m2;

  assign s1 = x1[31];
  assign e1 = x1[30:23];
  assign m1 = x1[22:0];
  assign s2 = ~(x2[31]);
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

  wire [7:0] e2ai;

  assign e2ai = ~e2a;

  wire [8:0] te;

  assign te = {1'b0,e1a} + {1'b0,e2ai};

  wire [0:0] ce;
  wire [7:0] tde;
  wire [8:0] tecalc;
  wire [8:0] terev;
  wire [4:0] de;

  assign ce = (te[8] == 1'b1) ? 1'b0 : 1'b1;
  assign tecalc = te + 9'b1;
  assign terev = ~te;
  assign tde = (te[8] == 1'b1) ? (tecalc[7:0]) : (terev[7:0]);
  assign de = (|(tde[7:5])) ? 5'd31 : tde[4:0];

  wire [0:0] sel;

  assign sel = (de == 5'b0) ? ( (m1a > m2a) ? 1'b0 : 1'b1 ) : ce;

  wire [24:0] ms;
  wire [24:0] mi;
  wire [7:0] es;
  wire [7:0] ei;
  wire [0:0] ss;

  assign ms = (sel == 0) ? m1a : m2a;
  assign mi = (sel == 0) ? m2a : m1a;
  assign es = (sel == 0) ? e1a : e2a;
  assign ei = (sel == 0) ? e2a : e1a;
  assign ss = (sel == 0) ? s1 : s2;

  wire [55:0] mie;
  wire [55:0] mia;

  assign mie = {mi,31'b0};
  assign mia = mie >> de;

  wire [0:0] tstck;

  assign tstck = |(mia[28:0]);

  wire [26:0] mye;

  assign mye = (s1 == s2) ? ({ms,2'b0} + mia[55:29]) : ({ms,2'b0} - mia[55:29]);

  wire [7:0] esi;
  wire [7:0] eyd;
  wire [26:0] myd;
  wire [0:0] stck;
  wire [0:0] ovf1;

  assign esi = es + 8'b1;
  assign eyd = (mye[26] == 1'b1) ? ((esi == 8'd255) ? 8'd255 : esi ) : es;
  assign myd = (mye[26] == 1'b1) ? ((esi == 8'd255) ? {2'b01,25'b0} : (mye >> 1) ) : mye;
  assign stck = (mye[26] == 1'b1) ? (tstck || mye[0]) : tstck;
  assign ovf1 = (mye[26] == 1'b1) ? ((esi == 8'd255) ? 1'b1 : 1'b0 ) : 1'b0;

  wire [4:0] se;

  assign se = (myd[25] == 0) ? ((myd[24] == 0) ? ((myd[23] == 0) ? ((myd[22] == 0) ? ((myd[21] == 0) ? ((myd[20] == 0) ? ((myd[19] == 0) ? ((myd[18] == 0) ? ((myd[17] == 0) ? ((myd[16] == 0) ? ((myd[15] == 0) ? ((myd[14] == 0) ? ((myd[13] == 0) ? ((myd[12] == 0) ? ((myd[11] == 0) ? ((myd[10] == 0) ? ((myd[9] == 0) ? ((myd[8] == 0) ? ((myd[7] == 0) ? ((myd[6] == 0) ? ((myd[5] == 0) ? ((myd[4] == 0) ? ((myd[3] == 0) ? ((myd[2] == 0) ? ((myd[1] == 0) ? ((myd[0] == 0) ? 26 : 25) : 24) : 23) : 22) : 21) : 20) : 19) : 18) : 17) : 16) : 15) : 14) : 13) : 12) : 11) : 10) : 9) : 8) : 7) : 6) : 5) : 4) : 3) : 2) : 1) : 0;

  wire signed [8:0] eyf;

  assign eyf = {1'b0,eyd} - {4'b0,se};

  wire [7:0] eyr;
  wire [26:0] myf;

  assign eyr = (eyf > 0) ? eyf[7:0] : 8'b0;
  assign myf = (eyf > 0) ? (myd  << se) : (myd << (eyd[4:0]-5'b1));

  wire [24:0] myr;

  assign myr = (( (myf[1]== 1'b1) && (myf[0] == 1'b0) && (stck[0] == 1'b0) && (myf[2] == 1'b1) ) || ( (myf[1]== 1'b1) && (myf[0] == 1'b0) && (s1 == s2) && (stck[0] == 1'b1) ) || ( (myf[1]== 1'b1) && (myf[0] == 1'b1) )) ? (myf[26:2] + 25'b1) : (myf[26:2]);

  wire [7:0] eyri;

  assign eyri = eyr + 8'b1;

  wire [7:0] ey;
  wire [22:0] my;
  wire [0:0] sy;
  wire [0:0] ovf2;

  assign ey = (myr[24] == 1) ? eyri : ((myr[23:0] == 24'b0) ? 8'b0 : eyr);
  assign my = (myr[24] == 1) ? 23'b0 : ((myr[23:0] == 24'b0) ? 23'b0 : myr[22:0]);
  assign sy = (ey == 8'b0 && my == 23'b0) ? (s1 && s2) : ss;

  assign ovf2 = (myr[24] == 1 && eyri == 8'd255)? 1'b1 : 1'b0;

  wire [0:0] nzm1;
  wire [0:0] nzm2;

  assign nzm1 = |(m1[22:0]);
  assign nzm2 = |(m2[22:0]);

  assign ovf = ((ovf1 ==1 || ovf2 == 1 ) && (e1 != 8'd255 && e2 != 8'd255));

/*  assign y = (e1 == 255 && e2 != 255) ? {s1,8'd255,nzm1,m1[21:0]} :
	     ((e2 == 255 && e1 != 255) ? {s2,8'd255,nzm2,m2[21:0]} :
	     ((e1 == 255 && e2 ==255 && nzm2) ? {s2,8'd255,1'b1,m2[21:0]} :
	     ((e1 == 255 && e2 ==255 && nzm1) ? {s1,8'd255,1'b1,m1[21:0]} :
	     ((e1 == 255 && e2 ==255 && s1 == s2) ? {s1,8'd255,23'b0} :
	     ((e1 == 255 && e2 ==255) ? {1'b1,8'd255,1'b1,22'b0} : {sy,ey,my} ))))); */


  assign y = (e1 == 8'd255) ? ((e2 == 8'd255) ? ( (nzm2)? {s2,8'd255,1'b1,m2[21:0]} : ( (nzm1)? {s1,8'd255,1'b1,m1[21:0]} : ( (s1 == s2) ? {s1,8'd255,23'b0} : {1'b1,8'd255,1'b1,22'b0} ) ) ) : ({s1,8'd255,nzm1,m1[21:0]})) : ((e2 == 255) ? ({s2,8'd255,nzm2,m2[21:0]}) : ({sy,ey,my}));

endmodule

`default_nettype wire


