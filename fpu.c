#include<stdio.h>
unsigned int get(unsigned int a,int n,int m){
	unsigned int b;
	unsigned int c;
	b=a>>m;
	c=(1<<(n-m+1))-1;
	b&=c;
	return b;
}
unsigned int lget(unsigned long int a,int n,int m){
	unsigned long int b;
	unsigned long int c;
	b=a>>m;
	c=(1<<(n-m+1))-1;
	b&=c;
	return (unsigned int)b;
}
void print(unsigned int a){
	for (int i=0;i<32;i++)
		printf("%d",(a>>(31-i))&1);
	printf("\n");
	return;
}
void lprint(unsigned long int a){
	for(int i=0;i<64;i++)
		printf("%d",(-i+93)%10);
	printf("\n");
	for (int i=0;i<64;i++)
		printf("%lu",(a>>(63-i))&1);
	printf("\n");
	return;
}

unsigned int flt(unsigned int x1,unsigned int x2){
	
	unsigned int v;
  unsigned int s1;
  unsigned int e1;
  unsigned int m1;
  unsigned int s2;
  unsigned int e2;
  unsigned int m2;

   s1 = get(x1,31,31);
   e1 = get(x1,30,23);
   m1 = get(x1,22,0);
   s2 = get(x2,31,31);
   e2 = get(x2,30,23);
   m2 = get(x2,22,0);

  unsigned int  m1a;
  unsigned int  m2a;

   m1a = (e1 == 0) ? get(m1,22,0) : (1<<23)|get(m1,22,0);
   m2a = (e2 == 0) ? get(m2,22,0) : (1<<23)|get(m2,22,0);

  unsigned int e1a;
  unsigned int e2a;

   e1a = (e1 == 0) ? 1 : get(e1,7,0);
   e2a = (e2 == 0) ? 1 : get(e2,7,0);

  unsigned int  sel;
  unsigned int  ce;

   sel = (e1a < e2a) ? 0 : ((e1a > e2a) ? 1 : 2);
   ce = (m1a < m2a) ? 0 : ((m1a > m2a) ? 1 : 2);

   v = (s1 == s2) ? ((s1 == 0) ? ((sel == 0 || (sel == 2 && ce == 0) ? 1 : 0 )) :
	 ((sel == 1 || (sel == 2 && ce == 1)) ? 1 : 0 )): 
	 ((s1 == 0 && (((e1<<23)|m1) != 0 || ((e2<<23)|m2) != 0)) ? 0 : 1 );

return v;
}
unsigned int feq(unsigned int x1,unsigned int x2){
  unsigned int v;

  unsigned int e1;
  unsigned int e2;

  e1 = get(x1,30,23);
  e2 = get(x2,30,23);

  v = (x1 == x2 || (e1 == 0 && e2 == 0)) ? 1 : 0; 

return v;
}
unsigned int fmul(unsigned int x1,unsigned int x2){
	unsigned int y;
	unsigned int ovf;
  unsigned int s1;
  unsigned int e1;
  unsigned int m1;
  unsigned int s2;
  unsigned int e2;
  unsigned int m2;

   s1 = get(x1,31,31);
   e1 = get(x1,30,23);
   m1 = get(x1,22,0);
   s2 = get(x2,31,31);
   e2 = get(x2,30,23);
   m2 = get(x2,22,0);

  unsigned int  m1a;
  unsigned int  m2a;

   m1a = (e1 == 0) ? get(m1,22,0) : (1<<23)|get(m1,22,0);
   m2a = (e2 == 0) ? get(m2,22,0) : (1<<23)|get(m2,22,0);

  unsigned int  e1a;
  unsigned int  e2a;

   e1a = (e1 == 0) ? 0x01 : get(e1,7,0);
   e2a = (e2 == 0) ? 0x01 : get(e2,7,0);
  
  int  e1b;
  int  e2b;
   e1b = get(e1a,7,0) - 127;
   e2b = get(e2a,7,0) - 127;
//printf("e1a=%d,e1b=%d",e1a,e1b);

  unsigned int  ss;
  int  ea;
  
   ss = s1 ^ s2;
   ea = get(e1b + e2b + 127,9,0);
  
  unsigned long int  myd;
  
   myd =  (unsigned long int)m1a * (unsigned long int)m2a;
  
	//printf("m1a=%u,m2a=%u\n",m1a,m2a);
  unsigned int  se=48;
 for(int i=0;i<48;i++){
	if(lget(myd,47-i,47-i)==1){
		se=i;
		break;
	}
 }
  
  unsigned long int  mb;
  unsigned long int  mbs;
  int  eb;
  int  ebs;
  
   mb = (myd << (se + 1))&0xffffffffffff;//12*4=48bit
   eb = ea - se + 1;
   ebs = -eb + 1;
   mbs = (get(eb,9,9) == 1) ? (mb >> ebs)&0xffffffffffff : mb&0xffffffffffff;
   ovf = (get(eb,9,8) == 1);
   y = (get(eb,9,9) == 1) ? ((eb > -23) ? (ss<<31)|lget(mbs,47,25) : (ss<<31)):
	 ((get(eb,8,8) == 1) ? (ss<<31)|(0xff<<23) : 
	 (ss<<31)|(get(eb,7,0)<<23)|lget(mbs,47,25));

	 return y;
}
unsigned int fle(unsigned int x1,unsigned int x2){
  unsigned int v;

  unsigned int s1;
  unsigned int e1;
  unsigned int m1;
  unsigned int s2;
  unsigned int e2;
  unsigned int m2;

   s1 = get(x1,31,31);
   e1 = get(x1,30,23);
   m1 = get(x1,22,0);
   s2 = get(x2,31,31);
   e2 = get(x2,30,23);
   m2 = get(x2,22,0);

  unsigned int  m1a;
  unsigned int  m2a;

   m1a = (e1 == 0) ? get(m1,22,0) : (1<<23)|get(m1,22,0);
   m2a = (e2 == 0) ? get(m2,22,0) : (1<<23)|get(m2,22,0);

  unsigned int  e1a;
  unsigned int  e2a;

   e1a = (e1 == 0) ? 1 : get(e1,7,0);
   e2a = (e2 == 0) ? 1 : get(e2,7,0);

  unsigned int  sel;
  unsigned int  ce;

   sel = (e1a < e2a) ? 0 : ((e1a > e2a) ? 1 : 2);
   ce = (m1a < m2a) ? 0 : ((m1a > m2a) ? 1 : 2);

   v = (s1 == s2) ? ((s1 == 0) ? ((sel == 0 || (sel == 2 && ce != 1) ? 1 : 0 )) :
            ((sel == 1 || (sel == 2 && ce != 0)) ? 1 : 0 ))
             : ((s1 == 0 && (((e1<<23)|m1) != 0 || ((e2<<23)|m2) != 0)) ? 0 : 1 );

return v;
}










unsigned int ftoi(unsigned int x,unsigned int rm){
  unsigned int y;

  unsigned int  s;
  unsigned int  e;
  unsigned int  m;

   s = get(x,31,31);
   e = get(x,30,23);
   m = get(x,22,0);
  
  unsigned int d;
  unsigned int i;
  unsigned int round;
  unsigned int ir;
  unsigned int roundm;
  
   d = (e >= 127 && e < 158) ? 158 - e : 0; // ずらす桁
	 roundm=(rm == 0||get(s,0,0)== 0) ? (1<<31)|(m<<8) :((1<<31)|(m<<8)-1);
   i = roundm >> d;
   round = ((i<<1)>>1); // rm == 0 で0丸め(端数無視)、 rm == 1 で最近接丸め(絶対値四捨五入)
//   round = (rm == 0 || i[0] == 0) ? i[31:0] : (i[31:0]) + 1; // rm == 0 で0丸め(端数無視)、 rm == 1 で最近接丸め(絶対値四捨五入)
   ir = (s == 0) ? round : ((rm==0) ? (~round) + 1: (~round)); // bit反転(補数)
  
  y = (e < 127) ? ((rm == 0 || get(s,0,0) == 0 ) ? 0 : 0xffffffff) : //指数部が0以下
             ((e >= 158) ? ((s==0) ? 0x7fffffff : 0x80000000) : //inf
             (((ir==0)&&(s==1))?0x00000000:((s<<31)|ir)) // その他
             );
return y;


}

unsigned int itof(unsigned int x){
  unsigned int  y;

  unsigned int abs;
  
   abs = (get(x,31,31) == 0) ? x : (~x)+1 ;
  
  unsigned int  shift=0;
  for(int i=1;i<32;i++){
		if (get(abs,31-i,31-i)==1){
			shift=i;
			break;
		}
	}


                
  unsigned int  rs;
  unsigned int  rx;
  
   rs = 30 - shift; //rs == 31でもしてくれる？？
   rx = (get(abs,30,0)) << shift;
   y = (shift == 0) ? 0 : // x == 0
            ((shift == 31) ? (get(x,31,31)<<31)|(0x7f<<23) : // x == 1 or -1
            (get(x,31,31)<<31)|(1<<30)|(rs<<23)|get(rx,30,8)
            );
return y;
}

// ここから追加分(finv,fdiv,fsqrt)です

unsigned int finv(unsigned int x){
  unsigned int y;
  
  unsigned int s;
  unsigned int e;
  unsigned int m;

   s = get(x,31,31);
   e = get(x,30,23);
   m = get(x,22,0);

  unsigned int  ma;

   ma = (e == 0) ? get(m,22,0) : (1<<23)|get(m,22,0);
   
  unsigned int x0;
  unsigned int x2;
  
  switch(get(m,22,12)){
  
    case 0 :
        x0 = 8384515;
        x2 = 8380423;
        break;
    case 1 :
        x0 = 8376331;
        x2 = 8364063;
        break;
    case 2 :
        x0 = 8368155;
        x2 = 8347727;
        break;
    case 3 :
        x0 = 8359987;
        x2 = 8331415;
        break;
    case 4 :
        x0 = 8351827;
        x2 = 8315127;
        break;
    case 5 :
        x0 = 8343675;
        x2 = 8298862;
        break;
    case 6 :
        x0 = 8335530;
        x2 = 8282620;
        break;
    case 7 :
        x0 = 8327394;
        x2 = 8266403;
        break;
    case 8 :
        x0 = 8319266;
        x2 = 8250211;
        break;
    case 9 :
        x0 = 8311146;
        x2 = 8234042;
        break;
    case 10 :
        x0 = 8303032;
        x2 = 8217892;
        break;
    case 11 :
        x0 = 8294928;
        x2 = 8201771;
        break;
    case 12 :
        x0 = 8286832;
        x2 = 8185673;
        break;
    case 13 :
        x0 = 8278742;
        x2 = 8169595;
        break;
    case 14 :
        x0 = 8270661;
        x2 = 8153543;
        break;
    case 15 :
        x0 = 8262588;
        x2 = 8137515;
        break;
    case 16 :
        x0 = 8254522;
        x2 = 8121508;
        break;
    case 17 :
        x0 = 8246464;
        x2 = 8105524;
        break;
    case 18 :
        x0 = 8238415;
        x2 = 8089567;
        break;
    case 19 :
        x0 = 8230372;
        x2 = 8073628;
        break;
    case 20 :
        x0 = 8222338;
        x2 = 8057716;
        break;
    case 21 :
        x0 = 8214312;
        x2 = 8041827;
        break;
    case 22 :
        x0 = 8206292;
        x2 = 8025957;
        break;
    case 23 :
        x0 = 8198282;
        x2 = 8010115;
        break;
    case 24 :
        x0 = 8190278;
        x2 = 7994293;
        break;
    case 25 :
        x0 = 8182283;
        x2 = 7978495;
        break;
    case 26 :
        x0 = 8174295;
        x2 = 7962720;
        break;
    case 27 :
        x0 = 8166315;
        x2 = 7946967;
        break;
    case 28 :
        x0 = 8158342;
        x2 = 7931236;
        break;
    case 29 :
        x0 = 8150378;
        x2 = 7915531;
        break;
    case 30 :
        x0 = 8142420;
        x2 = 7899845;
        break;
    case 31 :
        x0 = 8134471;
        x2 = 7884184;
        break;
    case 32 :
        x0 = 8126529;
        x2 = 7868544;
        break;
    case 33 :
        x0 = 8118594;
        x2 = 7852926;
        break;
    case 34 :
        x0 = 8110668;
        x2 = 7837332;
        break;
    case 35 :
        x0 = 8102749;
        x2 = 7821761;
        break;
    case 36 :
        x0 = 8094838;
        x2 = 7806212;
        break;
    case 37 :
        x0 = 8086934;
        x2 = 7790684;
        break;
    case 38 :
        x0 = 8079038;
        x2 = 7775180;
        break;
    case 39 :
        x0 = 8071148;
        x2 = 7759695;
        break;
    case 40 :
        x0 = 8063268;
        x2 = 7744237;
        break;
    case 41 :
        x0 = 8055394;
        x2 = 7728798;
        break;
    case 42 :
        x0 = 8047528;
        x2 = 7713382;
        break;
    case 43 :
        x0 = 8039670;
        x2 = 7697989;
        break;
    case 44 :
        x0 = 8031818;
        x2 = 7682616;
        break;
    case 45 :
        x0 = 8023975;
        x2 = 7667267;
        break;
    case 46 :
        x0 = 8016139;
        x2 = 7651939;
        break;
    case 47 :
        x0 = 8008310;
        x2 = 7636632;
        break;
    case 48 :
        x0 = 8000489;
        x2 = 7621349;
        break;
    case 49 :
        x0 = 7992676;
        x2 = 7606088;
        break;
    case 50 :
        x0 = 7984870;
        x2 = 7590848;
        break;
    case 51 :
        x0 = 7977070;
        x2 = 7575627;
        break;
    case 52 :
        x0 = 7969279;
        x2 = 7560431;
        break;
    case 53 :
        x0 = 7961496;
        x2 = 7545257;
        break;
    case 54 :
        x0 = 7953718;
        x2 = 7530101;
        break;
    case 55 :
        x0 = 7945950;
        x2 = 7514971;
        break;
    case 56 :
        x0 = 7938188;
        x2 = 7499860;
        break;
    case 57 :
        x0 = 7930434;
        x2 = 7484772;
        break;
    case 58 :
        x0 = 7922686;
        x2 = 7469703;
        break;
    case 59 :
        x0 = 7914947;
        x2 = 7454659;
        break;
    case 60 :
        x0 = 7907214;
        x2 = 7439633;
        break;
    case 61 :
        x0 = 7899490;
        x2 = 7424632;
        break;
    case 62 :
        x0 = 7891772;
        x2 = 7409649;
        break;
    case 63 :
        x0 = 7884062;
        x2 = 7394689;
        break;
    case 64 :
        x0 = 7876359;
        x2 = 7379750;
        break;
    case 65 :
        x0 = 7868663;
        x2 = 7364832;
        break;
    case 66 :
        x0 = 7860974;
        x2 = 7349934;
        break;
    case 67 :
        x0 = 7853293;
        x2 = 7335058;
        break;
    case 68 :
        x0 = 7845620;
        x2 = 7320206;
        break;
    case 69 :
        x0 = 7837952;
        x2 = 7305369;
        break;
    case 70 :
        x0 = 7830293;
        x2 = 7290558;
        break;
    case 71 :
        x0 = 7822641;
        x2 = 7275766;
        break;
    case 72 :
        x0 = 7814996;
        x2 = 7260996;
        break;
    case 73 :
        x0 = 7807358;
        x2 = 7246246;
        break;
    case 74 :
        x0 = 7799728;
        x2 = 7231518;
        break;
    case 75 :
        x0 = 7792104;
        x2 = 7216808;
        break;
    case 76 :
        x0 = 7784488;
        x2 = 7202121;
        break;
    case 77 :
        x0 = 7776879;
        x2 = 7187455;
        break;
    case 78 :
        x0 = 7769277;
        x2 = 7172809;
        break;
    case 79 :
        x0 = 7761682;
        x2 = 7158183;
        break;
    case 80 :
        x0 = 7754094;
        x2 = 7143577;
        break;
    case 81 :
        x0 = 7746514;
        x2 = 7128994;
        break;
    case 82 :
        x0 = 7738941;
        x2 = 7114431;
        break;
    case 83 :
        x0 = 7731374;
        x2 = 7099887;
        break;
    case 84 :
        x0 = 7723815;
        x2 = 7085364;
        break;
    case 85 :
        x0 = 7716263;
        x2 = 7070862;
        break;
    case 86 :
        x0 = 7708718;
        x2 = 7056380;
        break;
    case 87 :
        x0 = 7701180;
        x2 = 7041919;
        break;
    case 88 :
        x0 = 7693649;
        x2 = 7027477;
        break;
    case 89 :
        x0 = 7686125;
        x2 = 7013056;
        break;
    case 90 :
        x0 = 7678608;
        x2 = 6998655;
        break;
    case 91 :
        x0 = 7671098;
        x2 = 6984274;
        break;
    case 92 :
        x0 = 7663596;
        x2 = 6969915;
        break;
    case 93 :
        x0 = 7656100;
        x2 = 6955574;
        break;
    case 94 :
        x0 = 7648611;
        x2 = 6941253;
        break;
    case 95 :
        x0 = 7641130;
        x2 = 6926955;
        break;
    case 96 :
        x0 = 7633654;
        x2 = 6912672;
        break;
    case 97 :
        x0 = 7626187;
        x2 = 6898413;
        break;
    case 98 :
        x0 = 7618726;
        x2 = 6884173;
        break;
    case 99 :
        x0 = 7611272;
        x2 = 6869952;
        break;
    case 100 :
        x0 = 7603825;
        x2 = 6855752;
        break;
    case 101 :
        x0 = 7596384;
        x2 = 6841569;
        break;
    case 102 :
        x0 = 7588952;
        x2 = 6827410;
        break;
    case 103 :
        x0 = 7581526;
        x2 = 6813269;
        break;
    case 104 :
        x0 = 7574106;
        x2 = 6799147;
        break;
    case 105 :
        x0 = 7566694;
        x2 = 6785045;
        break;
    case 106 :
        x0 = 7559288;
        x2 = 6770962;
        break;
    case 107 :
        x0 = 7551890;
        x2 = 6756901;
        break;
    case 108 :
        x0 = 7544498;
        x2 = 6742858;
        break;
    case 109 :
        x0 = 7537112;
        x2 = 6728832;
        break;
    case 110 :
        x0 = 7529734;
        x2 = 6714828;
        break;
    case 111 :
        x0 = 7522363;
        x2 = 6700844;
        break;
    case 112 :
        x0 = 7514998;
        x2 = 6686878;
        break;
    case 113 :
        x0 = 7507641;
        x2 = 6672933;
        break;
    case 114 :
        x0 = 7500290;
        x2 = 6659007;
        break;
    case 115 :
        x0 = 7492946;
        x2 = 6645099;
        break;
    case 116 :
        x0 = 7485609;
        x2 = 6631212;
        break;
    case 117 :
        x0 = 7478278;
        x2 = 6617342;
        break;
    case 118 :
        x0 = 7470954;
        x2 = 6603492;
        break;
    case 119 :
        x0 = 7463638;
        x2 = 6589664;
        break;
    case 120 :
        x0 = 7456328;
        x2 = 6575853;
        break;
    case 121 :
        x0 = 7449024;
        x2 = 6562060;
        break;
    case 122 :
        x0 = 7441727;
        x2 = 6548287;
        break;
    case 123 :
        x0 = 7434437;
        x2 = 6534533;
        break;
    case 124 :
        x0 = 7427154;
        x2 = 6520798;
        break;
    case 125 :
        x0 = 7419877;
        x2 = 6507081;
        break;
    case 126 :
        x0 = 7412607;
        x2 = 6493384;
        break;
    case 127 :
        x0 = 7405344;
        x2 = 6479706;
        break;
    case 128 :
        x0 = 7398087;
        x2 = 6466046;
        break;
    case 129 :
        x0 = 7390838;
        x2 = 6452407;
        break;
    case 130 :
        x0 = 7383594;
        x2 = 6438784;
        break;
    case 131 :
        x0 = 7376358;
        x2 = 6425182;
        break;
    case 132 :
        x0 = 7369128;
        x2 = 6411597;
        break;
    case 133 :
        x0 = 7361904;
        x2 = 6398031;
        break;
    case 134 :
        x0 = 7354688;
        x2 = 6384485;
        break;
    case 135 :
        x0 = 7347478;
        x2 = 6370957;
        break;
    case 136 :
        x0 = 7340274;
        x2 = 6357446;
        break;
    case 137 :
        x0 = 7333077;
        x2 = 6343954;
        break;
    case 138 :
        x0 = 7325886;
        x2 = 6330480;
        break;
    case 139 :
        x0 = 7318702;
        x2 = 6317025;
        break;
    case 140 :
        x0 = 7311526;
        x2 = 6303592;
        break;
    case 141 :
        x0 = 7304355;
        x2 = 6290173;
        break;
    case 142 :
        x0 = 7297190;
        x2 = 6276773;
        break;
    case 143 :
        x0 = 7290033;
        x2 = 6263393;
        break;
    case 144 :
        x0 = 7282882;
        x2 = 6250030;
        break;
    case 145 :
        x0 = 7275738;
        x2 = 6236687;
        break;
    case 146 :
        x0 = 7268600;
        x2 = 6223361;
        break;
    case 147 :
        x0 = 7261468;
        x2 = 6210052;
        break;
    case 148 :
        x0 = 7254343;
        x2 = 6196763;
        break;
    case 149 :
        x0 = 7247224;
        x2 = 6183490;
        break;
    case 150 :
        x0 = 7240112;
        x2 = 6170237;
        break;
    case 151 :
        x0 = 7233007;
        x2 = 6157003;
        break;
    case 152 :
        x0 = 7225908;
        x2 = 6143786;
        break;
    case 153 :
        x0 = 7218815;
        x2 = 6130586;
        break;
    case 154 :
        x0 = 7211729;
        x2 = 6117405;
        break;
    case 155 :
        x0 = 7204650;
        x2 = 6104243;
        break;
    case 156 :
        x0 = 7197576;
        x2 = 6091097;
        break;
    case 157 :
        x0 = 7190509;
        x2 = 6077969;
        break;
    case 158 :
        x0 = 7183448;
        x2 = 6064858;
        break;
    case 159 :
        x0 = 7176394;
        x2 = 6051767;
        break;
    case 160 :
        x0 = 7169346;
        x2 = 6038692;
        break;
    case 161 :
        x0 = 7162305;
        x2 = 6025637;
        break;
    case 162 :
        x0 = 7155270;
        x2 = 6012598;
        break;
    case 163 :
        x0 = 7148241;
        x2 = 5999576;
        break;
    case 164 :
        x0 = 7141219;
        x2 = 5986574;
        break;
    case 165 :
        x0 = 7134202;
        x2 = 5973586;
        break;
    case 166 :
        x0 = 7127194;
        x2 = 5960621;
        break;
    case 167 :
        x0 = 7120190;
        x2 = 5947669;
        break;
    case 168 :
        x0 = 7113193;
        x2 = 5934736;
        break;
    case 169 :
        x0 = 7106202;
        x2 = 5921820;
        break;
    case 170 :
        x0 = 7099218;
        x2 = 5908922;
        break;
    case 171 :
        x0 = 7092240;
        x2 = 5896042;
        break;
    case 172 :
        x0 = 7085268;
        x2 = 5883178;
        break;
    case 173 :
        x0 = 7078302;
        x2 = 5870331;
        break;
    case 174 :
        x0 = 7071344;
        x2 = 5857505;
        break;
    case 175 :
        x0 = 7064390;
        x2 = 5844692;
        break;
    case 176 :
        x0 = 7057444;
        x2 = 5831899;
        break;
    case 177 :
        x0 = 7050503;
        x2 = 5819122;
        break;
    case 178 :
        x0 = 7043569;
        x2 = 5806363;
        break;
    case 179 :
        x0 = 7036641;
        x2 = 5793620;
        break;
    case 180 :
        x0 = 7029720;
        x2 = 5780897;
        break;
    case 181 :
        x0 = 7022804;
        x2 = 5768188;
        break;
    case 182 :
        x0 = 7015894;
        x2 = 5755496;
        break;
    case 183 :
        x0 = 7008991;
        x2 = 5742822;
        break;
    case 184 :
        x0 = 7002094;
        x2 = 5730165;
        break;
    case 185 :
        x0 = 6995203;
        x2 = 5717525;
        break;
    case 186 :
        x0 = 6988318;
        x2 = 5704902;
        break;
    case 187 :
        x0 = 6981440;
        x2 = 5692296;
        break;
    case 188 :
        x0 = 6974568;
        x2 = 5679708;
        break;
    case 189 :
        x0 = 6967702;
        x2 = 5667136;
        break;
    case 190 :
        x0 = 6960841;
        x2 = 5654579;
        break;
    case 191 :
        x0 = 6953988;
        x2 = 5642042;
        break;
    case 192 :
        x0 = 6947140;
        x2 = 5629520;
        break;
    case 193 :
        x0 = 6940298;
        x2 = 5617015;
        break;
    case 194 :
        x0 = 6933462;
        x2 = 5604526;
        break;
    case 195 :
        x0 = 6926632;
        x2 = 5592053;
        break;
    case 196 :
        x0 = 6919809;
        x2 = 5579599;
        break;
    case 197 :
        x0 = 6912992;
        x2 = 5567162;
        break;
    case 198 :
        x0 = 6906180;
        x2 = 5554739;
        break;
    case 199 :
        x0 = 6899375;
        x2 = 5542334;
        break;
    case 200 :
        x0 = 6892576;
        x2 = 5529946;
        break;
    case 201 :
        x0 = 6885782;
        x2 = 5517572;
        break;
    case 202 :
        x0 = 6878996;
        x2 = 5505219;
        break;
    case 203 :
        x0 = 6872214;
        x2 = 5492878;
        break;
    case 204 :
        x0 = 6865440;
        x2 = 5480557;
        break;
    case 205 :
        x0 = 6858670;
        x2 = 5468249;
        break;
    case 206 :
        x0 = 6851908;
        x2 = 5455961;
        break;
    case 207 :
        x0 = 6845150;
        x2 = 5443686;
        break;
    case 208 :
        x0 = 6838399;
        x2 = 5431429;
        break;
    case 209 :
        x0 = 6831654;
        x2 = 5419188;
        break;
    case 210 :
        x0 = 6824915;
        x2 = 5406963;
        break;
    case 211 :
        x0 = 6818182;
        x2 = 5394755;
        break;
    case 212 :
        x0 = 6811454;
        x2 = 5382561;
        break;
    case 213 :
        x0 = 6804734;
        x2 = 5370388;
        break;
    case 214 :
        x0 = 6798018;
        x2 = 5358226;
        break;
    case 215 :
        x0 = 6791309;
        x2 = 5346083;
        break;
    case 216 :
        x0 = 6784606;
        x2 = 5333956;
        break;
    case 217 :
        x0 = 6777908;
        x2 = 5321844;
        break;
    case 218 :
        x0 = 6771216;
        x2 = 5309747;
        break;
    case 219 :
        x0 = 6764530;
        x2 = 5297667;
        break;
    case 220 :
        x0 = 6757850;
        x2 = 5285603;
        break;
    case 221 :
        x0 = 6751177;
        x2 = 5273557;
        break;
    case 222 :
        x0 = 6744509;
        x2 = 5261525;
        break;
    case 223 :
        x0 = 6737847;
        x2 = 5249509;
        break;
    case 224 :
        x0 = 6731190;
        x2 = 5237508;
        break;
    case 225 :
        x0 = 6724540;
        x2 = 5225525;
        break;
    case 226 :
        x0 = 6717895;
        x2 = 5213555;
        break;
    case 227 :
        x0 = 6711256;
        x2 = 5201602;
        break;
    case 228 :
        x0 = 6704624;
        x2 = 5189667;
        break;
    case 229 :
        x0 = 6697996;
        x2 = 5177744;
        break;
    case 230 :
        x0 = 6691376;
        x2 = 5165841;
        break;
    case 231 :
        x0 = 6684760;
        x2 = 5153950;
        break;
    case 232 :
        x0 = 6678150;
        x2 = 5142075;
        break;
    case 233 :
        x0 = 6671546;
        x2 = 5130216;
        break;
    case 234 :
        x0 = 6664948;
        x2 = 5118374;
        break;
    case 235 :
        x0 = 6658356;
        x2 = 5106547;
        break;
    case 236 :
        x0 = 6651769;
        x2 = 5094734;
        break;
    case 237 :
        x0 = 6645188;
        x2 = 5082937;
        break;
    case 238 :
        x0 = 6638614;
        x2 = 5071158;
        break;
    case 239 :
        x0 = 6632044;
        x2 = 5059391;
        break;
    case 240 :
        x0 = 6625480;
        x2 = 5047640;
        break;
    case 241 :
        x0 = 6618923;
        x2 = 5035907;
        break;
    case 242 :
        x0 = 6612371;
        x2 = 5024188;
        break;
    case 243 :
        x0 = 6605824;
        x2 = 5012483;
        break;
    case 244 :
        x0 = 6599284;
        x2 = 5000795;
        break;
    case 245 :
        x0 = 6592749;
        x2 = 4989122;
        break;
    case 246 :
        x0 = 6586220;
        x2 = 4977464;
        break;
    case 247 :
        x0 = 6579696;
        x2 = 4965820;
        break;
    case 248 :
        x0 = 6573178;
        x2 = 4954192;
        break;
    case 249 :
        x0 = 6566666;
        x2 = 4942580;
        break;
    case 250 :
        x0 = 6560159;
        x2 = 4930982;
        break;
    case 251 :
        x0 = 6553658;
        x2 = 4919399;
        break;
    case 252 :
        x0 = 6547164;
        x2 = 4907834;
        break;
    case 253 :
        x0 = 6540674;
        x2 = 4896282;
        break;
    case 254 :
        x0 = 6534190;
        x2 = 4884744;
        break;
    case 255 :
        x0 = 6527711;
        x2 = 4873221;
        break;
    case 256 :
        x0 = 6521239;
        x2 = 4861715;
        break;
    case 257 :
        x0 = 6514772;
        x2 = 4850223;
        break;
    case 258 :
        x0 = 6508310;
        x2 = 4838745;
        break;
    case 259 :
        x0 = 6501854;
        x2 = 4827283;
        break;
    case 260 :
        x0 = 6495404;
        x2 = 4815836;
        break;
    case 261 :
        x0 = 6488960;
        x2 = 4804405;
        break;
    case 262 :
        x0 = 6482520;
        x2 = 4792986;
        break;
    case 263 :
        x0 = 6476086;
        x2 = 4781582;
        break;
    case 264 :
        x0 = 6469659;
        x2 = 4770196;
        break;
    case 265 :
        x0 = 6463236;
        x2 = 4758822;
        break;
    case 266 :
        x0 = 6456820;
        x2 = 4747465;
        break;
    case 267 :
        x0 = 6450408;
        x2 = 4736120;
        break;
    case 268 :
        x0 = 6444002;
        x2 = 4724791;
        break;
    case 269 :
        x0 = 6437602;
        x2 = 4713477;
        break;
    case 270 :
        x0 = 6431208;
        x2 = 4702178;
        break;
    case 271 :
        x0 = 6424818;
        x2 = 4690892;
        break;
    case 272 :
        x0 = 6418434;
        x2 = 4679621;
        break;
    case 273 :
        x0 = 6412056;
        x2 = 4668365;
        break;
    case 274 :
        x0 = 6405684;
        x2 = 4657125;
        break;
    case 275 :
        x0 = 6399316;
        x2 = 4645896;
        break;
    case 276 :
        x0 = 6392954;
        x2 = 4634684;
        break;
    case 277 :
        x0 = 6386598;
        x2 = 4623486;
        break;
    case 278 :
        x0 = 6380248;
        x2 = 4612304;
        break;
    case 279 :
        x0 = 6373902;
        x2 = 4601134;
        break;
    case 280 :
        x0 = 6367562;
        x2 = 4589979;
        break;
    case 281 :
        x0 = 6361228;
        x2 = 4578839;
        break;
    case 282 :
        x0 = 6354898;
        x2 = 4567711;
        break;
    case 283 :
        x0 = 6348575;
        x2 = 4556601;
        break;
    case 284 :
        x0 = 6342257;
        x2 = 4545504;
        break;
    case 285 :
        x0 = 6335944;
        x2 = 4534420;
        break;
    case 286 :
        x0 = 6329636;
        x2 = 4523350;
        break;
    case 287 :
        x0 = 6323334;
        x2 = 4512295;
        break;
    case 288 :
        x0 = 6317038;
        x2 = 4501256;
        break;
    case 289 :
        x0 = 6310746;
        x2 = 4490228;
        break;
    case 290 :
        x0 = 6304461;
        x2 = 4479217;
        break;
    case 291 :
        x0 = 6298180;
        x2 = 4468218;
        break;
    case 292 :
        x0 = 6291906;
        x2 = 4457236;
        break;
    case 293 :
        x0 = 6285636;
        x2 = 4446265;
        break;
    case 294 :
        x0 = 6279371;
        x2 = 4435308;
        break;
    case 295 :
        x0 = 6273112;
        x2 = 4424366;
        break;
    case 296 :
        x0 = 6266858;
        x2 = 4413438;
        break;
    case 297 :
        x0 = 6260610;
        x2 = 4402524;
        break;
    case 298 :
        x0 = 6254368;
        x2 = 4391626;
        break;
    case 299 :
        x0 = 6248130;
        x2 = 4380739;
        break;
    case 300 :
        x0 = 6241897;
        x2 = 4369866;
        break;
    case 301 :
        x0 = 6235670;
        x2 = 4359008;
        break;
    case 302 :
        x0 = 6229448;
        x2 = 4348163;
        break;
    case 303 :
        x0 = 6223232;
        x2 = 4337333;
        break;
    case 304 :
        x0 = 6217020;
        x2 = 4326515;
        break;
    case 305 :
        x0 = 6210815;
        x2 = 4315714;
        break;
    case 306 :
        x0 = 6204614;
        x2 = 4304924;
        break;
    case 307 :
        x0 = 6198418;
        x2 = 4294147;
        break;
    case 308 :
        x0 = 6192229;
        x2 = 4283388;
        break;
    case 309 :
        x0 = 6186044;
        x2 = 4272639;
        break;
    case 310 :
        x0 = 6179864;
        x2 = 4261904;
        break;
    case 311 :
        x0 = 6173690;
        x2 = 4251184;
        break;
    case 312 :
        x0 = 6167520;
        x2 = 4240476;
        break;
    case 313 :
        x0 = 6161357;
        x2 = 4229784;
        break;
    case 314 :
        x0 = 6155198;
        x2 = 4219103;
        break;
    case 315 :
        x0 = 6149044;
        x2 = 4208436;
        break;
    case 316 :
        x0 = 6142896;
        x2 = 4197783;
        break;
    case 317 :
        x0 = 6136754;
        x2 = 4187146;
        break;
    case 318 :
        x0 = 6130615;
        x2 = 4176518;
        break;
    case 319 :
        x0 = 6124482;
        x2 = 4165905;
        break;
    case 320 :
        x0 = 6118355;
        x2 = 4155307;
        break;
    case 321 :
        x0 = 6112232;
        x2 = 4144721;
        break;
    case 322 :
        x0 = 6106116;
        x2 = 4134150;
        break;
    case 323 :
        x0 = 6100003;
        x2 = 4123590;
        break;
    case 324 :
        x0 = 6093896;
        x2 = 4113044;
        break;
    case 325 :
        x0 = 6087794;
        x2 = 4102512;
        break;
    case 326 :
        x0 = 6081698;
        x2 = 4091994;
        break;
    case 327 :
        x0 = 6075606;
        x2 = 4081488;
        break;
    case 328 :
        x0 = 6069520;
        x2 = 4070996;
        break;
    case 329 :
        x0 = 6063439;
        x2 = 4060517;
        break;
    case 330 :
        x0 = 6057363;
        x2 = 4050052;
        break;
    case 331 :
        x0 = 6051292;
        x2 = 4039599;
        break;
    case 332 :
        x0 = 6045226;
        x2 = 4029159;
        break;
    case 333 :
        x0 = 6039165;
        x2 = 4018733;
        break;
    case 334 :
        x0 = 6033110;
        x2 = 4008321;
        break;
    case 335 :
        x0 = 6027058;
        x2 = 3997918;
        break;
    case 336 :
        x0 = 6021013;
        x2 = 3987532;
        break;
    case 337 :
        x0 = 6014972;
        x2 = 3977157;
        break;
    case 338 :
        x0 = 6008937;
        x2 = 3966797;
        break;
    case 339 :
        x0 = 6002907;
        x2 = 3956450;
        break;
    case 340 :
        x0 = 5996882;
        x2 = 3946116;
        break;
    case 341 :
        x0 = 5990861;
        x2 = 3935792;
        break;
    case 342 :
        x0 = 5984846;
        x2 = 3925484;
        break;
    case 343 :
        x0 = 5978836;
        x2 = 3915188;
        break;
    case 344 :
        x0 = 5972830;
        x2 = 3904904;
        break;
    case 345 :
        x0 = 5966830;
        x2 = 3894634;
        break;
    case 346 :
        x0 = 5960836;
        x2 = 3884378;
        break;
    case 347 :
        x0 = 5954845;
        x2 = 3874132;
        break;
    case 348 :
        x0 = 5948860;
        x2 = 3863901;
        break;
    case 349 :
        x0 = 5942880;
        x2 = 3853682;
        break;
    case 350 :
        x0 = 5936904;
        x2 = 3843475;
        break;
    case 351 :
        x0 = 5930934;
        x2 = 3833282;
        break;
    case 352 :
        x0 = 5924969;
        x2 = 3823101;
        break;
    case 353 :
        x0 = 5919009;
        x2 = 3812934;
        break;
    case 354 :
        x0 = 5913054;
        x2 = 3802779;
        break;
    case 355 :
        x0 = 5907103;
        x2 = 3792635;
        break;
    case 356 :
        x0 = 5901158;
        x2 = 3782506;
        break;
    case 357 :
        x0 = 5895218;
        x2 = 3772390;
        break;
    case 358 :
        x0 = 5889282;
        x2 = 3762284;
        break;
    case 359 :
        x0 = 5883351;
        x2 = 3752191;
        break;
    case 360 :
        x0 = 5877426;
        x2 = 3742113;
        break;
    case 361 :
        x0 = 5871504;
        x2 = 3732044;
        break;
    case 362 :
        x0 = 5865589;
        x2 = 3721991;
        break;
    case 363 :
        x0 = 5859678;
        x2 = 3711949;
        break;
    case 364 :
        x0 = 5853772;
        x2 = 3701919;
        break;
    case 365 :
        x0 = 5847870;
        x2 = 3691901;
        break;
    case 366 :
        x0 = 5841974;
        x2 = 3681897;
        break;
    case 367 :
        x0 = 5836083;
        x2 = 3671905;
        break;
    case 368 :
        x0 = 5830196;
        x2 = 3661925;
        break;
    case 369 :
        x0 = 5824315;
        x2 = 3651958;
        break;
    case 370 :
        x0 = 5818438;
        x2 = 3642003;
        break;
    case 371 :
        x0 = 5812566;
        x2 = 3632060;
        break;
    case 372 :
        x0 = 5806700;
        x2 = 3622131;
        break;
    case 373 :
        x0 = 5800837;
        x2 = 3612212;
        break;
    case 374 :
        x0 = 5794980;
        x2 = 3602307;
        break;
    case 375 :
        x0 = 5789128;
        x2 = 3592414;
        break;
    case 376 :
        x0 = 5783280;
        x2 = 3582532;
        break;
    case 377 :
        x0 = 5777436;
        x2 = 3572662;
        break;
    case 378 :
        x0 = 5771599;
        x2 = 3562806;
        break;
    case 379 :
        x0 = 5765766;
        x2 = 3552962;
        break;
    case 380 :
        x0 = 5759937;
        x2 = 3543129;
        break;
    case 381 :
        x0 = 5754113;
        x2 = 3533308;
        break;
    case 382 :
        x0 = 5748294;
        x2 = 3523499;
        break;
    case 383 :
        x0 = 5742480;
        x2 = 3513703;
        break;
    case 384 :
        x0 = 5736671;
        x2 = 3503920;
        break;
    case 385 :
        x0 = 5730866;
        x2 = 3494147;
        break;
    case 386 :
        x0 = 5725067;
        x2 = 3484388;
        break;
    case 387 :
        x0 = 5719272;
        x2 = 3474640;
        break;
    case 388 :
        x0 = 5713482;
        x2 = 3464905;
        break;
    case 389 :
        x0 = 5707696;
        x2 = 3455180;
        break;
    case 390 :
        x0 = 5701916;
        x2 = 3445469;
        break;
    case 391 :
        x0 = 5696140;
        x2 = 3435769;
        break;
    case 392 :
        x0 = 5690368;
        x2 = 3426080;
        break;
    case 393 :
        x0 = 5684602;
        x2 = 3416404;
        break;
    case 394 :
        x0 = 5678840;
        x2 = 3406740;
        break;
    case 395 :
        x0 = 5673082;
        x2 = 3397086;
        break;
    case 396 :
        x0 = 5667330;
        x2 = 3387445;
        break;
    case 397 :
        x0 = 5661583;
        x2 = 3377818;
        break;
    case 398 :
        x0 = 5655840;
        x2 = 3368201;
        break;
    case 399 :
        x0 = 5650102;
        x2 = 3358596;
        break;
    case 400 :
        x0 = 5644368;
        x2 = 3349002;
        break;
    case 401 :
        x0 = 5638639;
        x2 = 3339420;
        break;
    case 402 :
        x0 = 5632915;
        x2 = 3329850;
        break;
    case 403 :
        x0 = 5627195;
        x2 = 3320291;
        break;
    case 404 :
        x0 = 5621480;
        x2 = 3310745;
        break;
    case 405 :
        x0 = 5615770;
        x2 = 3301210;
        break;
    case 406 :
        x0 = 5610064;
        x2 = 3291686;
        break;
    case 407 :
        x0 = 5604364;
        x2 = 3282176;
        break;
    case 408 :
        x0 = 5598667;
        x2 = 3272675;
        break;
    case 409 :
        x0 = 5592976;
        x2 = 3263188;
        break;
    case 410 :
        x0 = 5587288;
        x2 = 3253709;
        break;
    case 411 :
        x0 = 5581606;
        x2 = 3244245;
        break;
    case 412 :
        x0 = 5575928;
        x2 = 3234790;
        break;
    case 413 :
        x0 = 5570255;
        x2 = 3225348;
        break;
    case 414 :
        x0 = 5564586;
        x2 = 3215917;
        break;
    case 415 :
        x0 = 5558922;
        x2 = 3206498;
        break;
    case 416 :
        x0 = 5553263;
        x2 = 3197091;
        break;
    case 417 :
        x0 = 5547608;
        x2 = 3187694;
        break;
    case 418 :
        x0 = 5541958;
        x2 = 3178309;
        break;
    case 419 :
        x0 = 5536313;
        x2 = 3168937;
        break;
    case 420 :
        x0 = 5530672;
        x2 = 3159575;
        break;
    case 421 :
        x0 = 5525035;
        x2 = 3150223;
        break;
    case 422 :
        x0 = 5519403;
        x2 = 3140884;
        break;
    case 423 :
        x0 = 5513776;
        x2 = 3131556;
        break;
    case 424 :
        x0 = 5508153;
        x2 = 3122239;
        break;
    case 425 :
        x0 = 5502534;
        x2 = 3112932;
        break;
    case 426 :
        x0 = 5496921;
        x2 = 3103639;
        break;
    case 427 :
        x0 = 5491312;
        x2 = 3094357;
        break;
    case 428 :
        x0 = 5485708;
        x2 = 3085086;
        break;
    case 429 :
        x0 = 5480107;
        x2 = 3075824;
        break;
    case 430 :
        x0 = 5474511;
        x2 = 3066574;
        break;
    case 431 :
        x0 = 5468920;
        x2 = 3057336;
        break;
    case 432 :
        x0 = 5463334;
        x2 = 3048111;
        break;
    case 433 :
        x0 = 5457752;
        x2 = 3038895;
        break;
    case 434 :
        x0 = 5452174;
        x2 = 3029690;
        break;
    case 435 :
        x0 = 5446601;
        x2 = 3020496;
        break;
    case 436 :
        x0 = 5441032;
        x2 = 3011313;
        break;
    case 437 :
        x0 = 5435468;
        x2 = 3002142;
        break;
    case 438 :
        x0 = 5429909;
        x2 = 2992983;
        break;
    case 439 :
        x0 = 5424354;
        x2 = 2983834;
        break;
    case 440 :
        x0 = 5418802;
        x2 = 2974694;
        break;
    case 441 :
        x0 = 5413256;
        x2 = 2965567;
        break;
    case 442 :
        x0 = 5407714;
        x2 = 2956451;
        break;
    case 443 :
        x0 = 5402177;
        x2 = 2947346;
        break;
    case 444 :
        x0 = 5396644;
        x2 = 2938252;
        break;
    case 445 :
        x0 = 5391116;
        x2 = 2929169;
        break;
    case 446 :
        x0 = 5385592;
        x2 = 2920097;
        break;
    case 447 :
        x0 = 5380072;
        x2 = 2911035;
        break;
    case 448 :
        x0 = 5374557;
        x2 = 2901985;
        break;
    case 449 :
        x0 = 5369046;
        x2 = 2892945;
        break;
    case 450 :
        x0 = 5363540;
        x2 = 2883916;
        break;
    case 451 :
        x0 = 5358038;
        x2 = 2874898;
        break;
    case 452 :
        x0 = 5352540;
        x2 = 2865890;
        break;
    case 453 :
        x0 = 5347047;
        x2 = 2856894;
        break;
    case 454 :
        x0 = 5341558;
        x2 = 2847908;
        break;
    case 455 :
        x0 = 5336074;
        x2 = 2838934;
        break;
    case 456 :
        x0 = 5330594;
        x2 = 2829970;
        break;
    case 457 :
        x0 = 5325118;
        x2 = 2821016;
        break;
    case 458 :
        x0 = 5319647;
        x2 = 2812074;
        break;
    case 459 :
        x0 = 5314180;
        x2 = 2803142;
        break;
    case 460 :
        x0 = 5308718;
        x2 = 2794221;
        break;
    case 461 :
        x0 = 5303260;
        x2 = 2785311;
        break;
    case 462 :
        x0 = 5297806;
        x2 = 2776411;
        break;
    case 463 :
        x0 = 5292356;
        x2 = 2767520;
        break;
    case 464 :
        x0 = 5286911;
        x2 = 2758642;
        break;
    case 465 :
        x0 = 5281470;
        x2 = 2749774;
        break;
    case 466 :
        x0 = 5276034;
        x2 = 2740917;
        break;
    case 467 :
        x0 = 5270602;
        x2 = 2732070;
        break;
    case 468 :
        x0 = 5265174;
        x2 = 2723233;
        break;
    case 469 :
        x0 = 5259750;
        x2 = 2714407;
        break;
    case 470 :
        x0 = 5254331;
        x2 = 2705592;
        break;
    case 471 :
        x0 = 5248916;
        x2 = 2696787;
        break;
    case 472 :
        x0 = 5243506;
        x2 = 2687993;
        break;
    case 473 :
        x0 = 5238098;
        x2 = 2679207;
        break;
    case 474 :
        x0 = 5232697;
        x2 = 2670435;
        break;
    case 475 :
        x0 = 5227299;
        x2 = 2661671;
        break;
    case 476 :
        x0 = 5221906;
        x2 = 2652919;
        break;
    case 477 :
        x0 = 5216516;
        x2 = 2644176;
        break;
    case 478 :
        x0 = 5211132;
        x2 = 2635446;
        break;
    case 479 :
        x0 = 5205750;
        x2 = 2626722;
        break;
    case 480 :
        x0 = 5200374;
        x2 = 2618011;
        break;
    case 481 :
        x0 = 5195002;
        x2 = 2609311;
        break;
    case 482 :
        x0 = 5189634;
        x2 = 2600620;
        break;
    case 483 :
        x0 = 5184270;
        x2 = 2591939;
        break;
    case 484 :
        x0 = 5178911;
        x2 = 2583270;
        break;
    case 485 :
        x0 = 5173556;
        x2 = 2574611;
        break;
    case 486 :
        x0 = 5168204;
        x2 = 2565960;
        break;
    case 487 :
        x0 = 5162858;
        x2 = 2557322;
        break;
    case 488 :
        x0 = 5157516;
        x2 = 2548694;
        break;
    case 489 :
        x0 = 5152176;
        x2 = 2540072;
        break;
    case 490 :
        x0 = 5146842;
        x2 = 2531464;
        break;
    case 491 :
        x0 = 5141512;
        x2 = 2522865;
        break;
    case 492 :
        x0 = 5136187;
        x2 = 2514278;
        break;
    case 493 :
        x0 = 5130865;
        x2 = 2505700;
        break;
    case 494 :
        x0 = 5125548;
        x2 = 2497132;
        break;
    case 495 :
        x0 = 5120235;
        x2 = 2488574;
        break;
    case 496 :
        x0 = 5114926;
        x2 = 2480027;
        break;
    case 497 :
        x0 = 5109621;
        x2 = 2471489;
        break;
    case 498 :
        x0 = 5104320;
        x2 = 2462960;
        break;
    case 499 :
        x0 = 5099024;
        x2 = 2454443;
        break;
    case 500 :
        x0 = 5093732;
        x2 = 2445936;
        break;
    case 501 :
        x0 = 5088442;
        x2 = 2437436;
        break;
    case 502 :
        x0 = 5083158;
        x2 = 2428948;
        break;
    case 503 :
        x0 = 5077879;
        x2 = 2420472;
        break;
    case 504 :
        x0 = 5072603;
        x2 = 2412004;
        break;
    case 505 :
        x0 = 5067332;
        x2 = 2403547;
        break;
    case 506 :
        x0 = 5062064;
        x2 = 2395099;
        break;
    case 507 :
        x0 = 5056800;
        x2 = 2386660;
        break;
    case 508 :
        x0 = 5051541;
        x2 = 2378232;
        break;
    case 509 :
        x0 = 5046286;
        x2 = 2369814;
        break;
    case 510 :
        x0 = 5041035;
        x2 = 2361406;
        break;
    case 511 :
        x0 = 5035788;
        x2 = 2353008;
        break;
    case 512 :
        x0 = 5030545;
        x2 = 2344619;
        break;
    case 513 :
        x0 = 5025306;
        x2 = 2336240;
        break;
    case 514 :
        x0 = 5020072;
        x2 = 2327872;
        break;
    case 515 :
        x0 = 5014841;
        x2 = 2319512;
        break;
    case 516 :
        x0 = 5009614;
        x2 = 2311162;
        break;
    case 517 :
        x0 = 5004392;
        x2 = 2302823;
        break;
    case 518 :
        x0 = 4999174;
        x2 = 2294494;
        break;
    case 519 :
        x0 = 4993960;
        x2 = 2286174;
        break;
    case 520 :
        x0 = 4988748;
        x2 = 2277861;
        break;
    case 521 :
        x0 = 4983542;
        x2 = 2269561;
        break;
    case 522 :
        x0 = 4978340;
        x2 = 2261270;
        break;
    case 523 :
        x0 = 4973142;
        x2 = 2252988;
        break;
    case 524 :
        x0 = 4967948;
        x2 = 2244717;
        break;
    case 525 :
        x0 = 4962758;
        x2 = 2236455;
        break;
    case 526 :
        x0 = 4957572;
        x2 = 2228202;
        break;
    case 527 :
        x0 = 4952390;
        x2 = 2219959;
        break;
    case 528 :
        x0 = 4947212;
        x2 = 2211726;
        break;
    case 529 :
        x0 = 4942038;
        x2 = 2203502;
        break;
    case 530 :
        x0 = 4936868;
        x2 = 2195288;
        break;
    case 531 :
        x0 = 4931702;
        x2 = 2187083;
        break;
    case 532 :
        x0 = 4926540;
        x2 = 2178888;
        break;
    case 533 :
        x0 = 4921382;
        x2 = 2170703;
        break;
    case 534 :
        x0 = 4916229;
        x2 = 2162528;
        break;
    case 535 :
        x0 = 4911079;
        x2 = 2154361;
        break;
    case 536 :
        x0 = 4905933;
        x2 = 2146204;
        break;
    case 537 :
        x0 = 4900791;
        x2 = 2138057;
        break;
    case 538 :
        x0 = 4895653;
        x2 = 2129918;
        break;
    case 539 :
        x0 = 4890519;
        x2 = 2121790;
        break;
    case 540 :
        x0 = 4885389;
        x2 = 2113671;
        break;
    case 541 :
        x0 = 4880263;
        x2 = 2105561;
        break;
    case 542 :
        x0 = 4875140;
        x2 = 2097459;
        break;
    case 543 :
        x0 = 4870022;
        x2 = 2089368;
        break;
    case 544 :
        x0 = 4864908;
        x2 = 2081287;
        break;
    case 545 :
        x0 = 4859798;
        x2 = 2073215;
        break;
    case 546 :
        x0 = 4854692;
        x2 = 2065152;
        break;
    case 547 :
        x0 = 4849590;
        x2 = 2057099;
        break;
    case 548 :
        x0 = 4844491;
        x2 = 2049054;
        break;
    case 549 :
        x0 = 4839396;
        x2 = 2041018;
        break;
    case 550 :
        x0 = 4834306;
        x2 = 2032993;
        break;
    case 551 :
        x0 = 4829219;
        x2 = 2024976;
        break;
    case 552 :
        x0 = 4824136;
        x2 = 2016968;
        break;
    case 553 :
        x0 = 4819057;
        x2 = 2008970;
        break;
    case 554 :
        x0 = 4813982;
        x2 = 2000981;
        break;
    case 555 :
        x0 = 4808911;
        x2 = 1993002;
        break;
    case 556 :
        x0 = 4803844;
        x2 = 1985031;
        break;
    case 557 :
        x0 = 4798781;
        x2 = 1977071;
        break;
    case 558 :
        x0 = 4793721;
        x2 = 1969117;
        break;
    case 559 :
        x0 = 4788666;
        x2 = 1961175;
        break;
    case 560 :
        x0 = 4783614;
        x2 = 1953241;
        break;
    case 561 :
        x0 = 4778566;
        x2 = 1945316;
        break;
    case 562 :
        x0 = 4773522;
        x2 = 1937400;
        break;
    case 563 :
        x0 = 4768482;
        x2 = 1929493;
        break;
    case 564 :
        x0 = 4763446;
        x2 = 1921596;
        break;
    case 565 :
        x0 = 4758414;
        x2 = 1913708;
        break;
    case 566 :
        x0 = 4753386;
        x2 = 1905830;
        break;
    case 567 :
        x0 = 4748360;
        x2 = 1897957;
        break;
    case 568 :
        x0 = 4743340;
        x2 = 1890097;
        break;
    case 569 :
        x0 = 4738323;
        x2 = 1882245;
        break;
    case 570 :
        x0 = 4733310;
        x2 = 1874402;
        break;
    case 571 :
        x0 = 4728300;
        x2 = 1866566;
        break;
    case 572 :
        x0 = 4723295;
        x2 = 1858742;
        break;
    case 573 :
        x0 = 4718293;
        x2 = 1850925;
        break;
    case 574 :
        x0 = 4713296;
        x2 = 1843119;
        break;
    case 575 :
        x0 = 4708302;
        x2 = 1835320;
        break;
    case 576 :
        x0 = 4703310;
        x2 = 1827528;
        break;
    case 577 :
        x0 = 4698324;
        x2 = 1819748;
        break;
    case 578 :
        x0 = 4693342;
        x2 = 1811977;
        break;
    case 579 :
        x0 = 4688363;
        x2 = 1804214;
        break;
    case 580 :
        x0 = 4683388;
        x2 = 1796460;
        break;
    case 581 :
        x0 = 4678417;
        x2 = 1788715;
        break;
    case 582 :
        x0 = 4673449;
        x2 = 1780978;
        break;
    case 583 :
        x0 = 4668486;
        x2 = 1773251;
        break;
    case 584 :
        x0 = 4663526;
        x2 = 1765532;
        break;
    case 585 :
        x0 = 4658570;
        x2 = 1757822;
        break;
    case 586 :
        x0 = 4653616;
        x2 = 1750119;
        break;
    case 587 :
        x0 = 4648668;
        x2 = 1742427;
        break;
    case 588 :
        x0 = 4643723;
        x2 = 1734743;
        break;
    case 589 :
        x0 = 4638782;
        x2 = 1727069;
        break;
    case 590 :
        x0 = 4633844;
        x2 = 1719401;
        break;
    case 591 :
        x0 = 4628911;
        x2 = 1711745;
        break;
    case 592 :
        x0 = 4623981;
        x2 = 1704096;
        break;
    case 593 :
        x0 = 4619054;
        x2 = 1696454;
        break;
    case 594 :
        x0 = 4614132;
        x2 = 1688824;
        break;
    case 595 :
        x0 = 4609214;
        x2 = 1681202;
        break;
    case 596 :
        x0 = 4604298;
        x2 = 1673586;
        break;
    case 597 :
        x0 = 4599387;
        x2 = 1665981;
        break;
    case 598 :
        x0 = 4594480;
        x2 = 1658385;
        break;
    case 599 :
        x0 = 4589576;
        x2 = 1650797;
        break;
    case 600 :
        x0 = 4584676;
        x2 = 1643217;
        break;
    case 601 :
        x0 = 4579779;
        x2 = 1635645;
        break;
    case 602 :
        x0 = 4574886;
        x2 = 1628082;
        break;
    case 603 :
        x0 = 4569997;
        x2 = 1620528;
        break;
    case 604 :
        x0 = 4565112;
        x2 = 1612984;
        break;
    case 605 :
        x0 = 4560230;
        x2 = 1605446;
        break;
    case 606 :
        x0 = 4555352;
        x2 = 1597918;
        break;
    case 607 :
        x0 = 4550478;
        x2 = 1590398;
        break;
    case 608 :
        x0 = 4545606;
        x2 = 1582885;
        break;
    case 609 :
        x0 = 4540740;
        x2 = 1575384;
        break;
    case 610 :
        x0 = 4535876;
        x2 = 1567888;
        break;
    case 611 :
        x0 = 4531016;
        x2 = 1560402;
        break;
    case 612 :
        x0 = 4526160;
        x2 = 1552924;
        break;
    case 613 :
        x0 = 4521308;
        x2 = 1545456;
        break;
    case 614 :
        x0 = 4516459;
        x2 = 1537994;
        break;
    case 615 :
        x0 = 4511614;
        x2 = 1530542;
        break;
    case 616 :
        x0 = 4506772;
        x2 = 1523098;
        break;
    case 617 :
        x0 = 4501934;
        x2 = 1515662;
        break;
    case 618 :
        x0 = 4497100;
        x2 = 1508235;
        break;
    case 619 :
        x0 = 4492270;
        x2 = 1500817;
        break;
    case 620 :
        x0 = 4487443;
        x2 = 1493406;
        break;
    case 621 :
        x0 = 4482619;
        x2 = 1486003;
        break;
    case 622 :
        x0 = 4477800;
        x2 = 1478610;
        break;
    case 623 :
        x0 = 4472984;
        x2 = 1471225;
        break;
    case 624 :
        x0 = 4468171;
        x2 = 1463847;
        break;
    case 625 :
        x0 = 4463362;
        x2 = 1456478;
        break;
    case 626 :
        x0 = 4458556;
        x2 = 1449116;
        break;
    case 627 :
        x0 = 4453754;
        x2 = 1441763;
        break;
    case 628 :
        x0 = 4448956;
        x2 = 1434419;
        break;
    case 629 :
        x0 = 4444162;
        x2 = 1427084;
        break;
    case 630 :
        x0 = 4439371;
        x2 = 1419756;
        break;
    case 631 :
        x0 = 4434584;
        x2 = 1412437;
        break;
    case 632 :
        x0 = 4429800;
        x2 = 1405126;
        break;
    case 633 :
        x0 = 4425019;
        x2 = 1397821;
        break;
    case 634 :
        x0 = 4420242;
        x2 = 1390526;
        break;
    case 635 :
        x0 = 4415469;
        x2 = 1383239;
        break;
    case 636 :
        x0 = 4410700;
        x2 = 1375961;
        break;
    case 637 :
        x0 = 4405934;
        x2 = 1368691;
        break;
    case 638 :
        x0 = 4401171;
        x2 = 1361427;
        break;
    case 639 :
        x0 = 4396412;
        x2 = 1354173;
        break;
    case 640 :
        x0 = 4391656;
        x2 = 1346925;
        break;
    case 641 :
        x0 = 4386904;
        x2 = 1339687;
        break;
    case 642 :
        x0 = 4382156;
        x2 = 1332457;
        break;
    case 643 :
        x0 = 4377411;
        x2 = 1325235;
        break;
    case 644 :
        x0 = 4372670;
        x2 = 1318021;
        break;
    case 645 :
        x0 = 4367932;
        x2 = 1310815;
        break;
    case 646 :
        x0 = 4363198;
        x2 = 1303617;
        break;
    case 647 :
        x0 = 4358467;
        x2 = 1296427;
        break;
    case 648 :
        x0 = 4353740;
        x2 = 1289245;
        break;
    case 649 :
        x0 = 4349016;
        x2 = 1282071;
        break;
    case 650 :
        x0 = 4344296;
        x2 = 1274905;
        break;
    case 651 :
        x0 = 4339579;
        x2 = 1267746;
        break;
    case 652 :
        x0 = 4334866;
        x2 = 1260597;
        break;
    case 653 :
        x0 = 4330156;
        x2 = 1253454;
        break;
    case 654 :
        x0 = 4325450;
        x2 = 1246320;
        break;
    case 655 :
        x0 = 4320747;
        x2 = 1239193;
        break;
    case 656 :
        x0 = 4316048;
        x2 = 1232075;
        break;
    case 657 :
        x0 = 4311352;
        x2 = 1224965;
        break;
    case 658 :
        x0 = 4306660;
        x2 = 1217862;
        break;
    case 659 :
        x0 = 4301970;
        x2 = 1210766;
        break;
    case 660 :
        x0 = 4297285;
        x2 = 1203680;
        break;
    case 661 :
        x0 = 4292603;
        x2 = 1196600;
        break;
    case 662 :
        x0 = 4287924;
        x2 = 1189528;
        break;
    case 663 :
        x0 = 4283249;
        x2 = 1182465;
        break;
    case 664 :
        x0 = 4278578;
        x2 = 1175410;
        break;
    case 665 :
        x0 = 4273910;
        x2 = 1168363;
        break;
    case 666 :
        x0 = 4269245;
        x2 = 1161322;
        break;
    case 667 :
        x0 = 4264584;
        x2 = 1154290;
        break;
    case 668 :
        x0 = 4259926;
        x2 = 1147266;
        break;
    case 669 :
        x0 = 4255271;
        x2 = 1140248;
        break;
    case 670 :
        x0 = 4250620;
        x2 = 1133239;
        break;
    case 671 :
        x0 = 4245972;
        x2 = 1126237;
        break;
    case 672 :
        x0 = 4241328;
        x2 = 1119244;
        break;
    case 673 :
        x0 = 4236687;
        x2 = 1112258;
        break;
    case 674 :
        x0 = 4232050;
        x2 = 1105280;
        break;
    case 675 :
        x0 = 4227416;
        x2 = 1098309;
        break;
    case 676 :
        x0 = 4222785;
        x2 = 1091346;
        break;
    case 677 :
        x0 = 4218158;
        x2 = 1084391;
        break;
    case 678 :
        x0 = 4213534;
        x2 = 1077443;
        break;
    case 679 :
        x0 = 4208914;
        x2 = 1070504;
        break;
    case 680 :
        x0 = 4204297;
        x2 = 1063571;
        break;
    case 681 :
        x0 = 4199684;
        x2 = 1056648;
        break;
    case 682 :
        x0 = 4195073;
        x2 = 1049730;
        break;
    case 683 :
        x0 = 4190466;
        x2 = 1042820;
        break;
    case 684 :
        x0 = 4185862;
        x2 = 1035917;
        break;
    case 685 :
        x0 = 4181262;
        x2 = 1029023;
        break;
    case 686 :
        x0 = 4176666;
        x2 = 1022138;
        break;
    case 687 :
        x0 = 4172072;
        x2 = 1015257;
        break;
    case 688 :
        x0 = 4167482;
        x2 = 1008386;
        break;
    case 689 :
        x0 = 4162896;
        x2 = 1001523;
        break;
    case 690 :
        x0 = 4158312;
        x2 = 994665;
        break;
    case 691 :
        x0 = 4153732;
        x2 = 987816;
        break;
    case 692 :
        x0 = 4149156;
        x2 = 980975;
        break;
    case 693 :
        x0 = 4144582;
        x2 = 974140;
        break;
    case 694 :
        x0 = 4140012;
        x2 = 967314;
        break;
    case 695 :
        x0 = 4135446;
        x2 = 960495;
        break;
    case 696 :
        x0 = 4130882;
        x2 = 953683;
        break;
    case 697 :
        x0 = 4126322;
        x2 = 946878;
        break;
    case 698 :
        x0 = 4121766;
        x2 = 940083;
        break;
    case 699 :
        x0 = 4117212;
        x2 = 933292;
        break;
    case 700 :
        x0 = 4112662;
        x2 = 926510;
        break;
    case 701 :
        x0 = 4108115;
        x2 = 919735;
        break;
    case 702 :
        x0 = 4103572;
        x2 = 912969;
        break;
    case 703 :
        x0 = 4099032;
        x2 = 906209;
        break;
    case 704 :
        x0 = 4094495;
        x2 = 899456;
        break;
    case 705 :
        x0 = 4089962;
        x2 = 892712;
        break;
    case 706 :
        x0 = 4085431;
        x2 = 885973;
        break;
    case 707 :
        x0 = 4080904;
        x2 = 879242;
        break;
    case 708 :
        x0 = 4076380;
        x2 = 872519;
        break;
    case 709 :
        x0 = 4071860;
        x2 = 865804;
        break;
    case 710 :
        x0 = 4067343;
        x2 = 859095;
        break;
    case 711 :
        x0 = 4062830;
        x2 = 852395;
        break;
    case 712 :
        x0 = 4058319;
        x2 = 845701;
        break;
    case 713 :
        x0 = 4053812;
        x2 = 839014;
        break;
    case 714 :
        x0 = 4049308;
        x2 = 832335;
        break;
    case 715 :
        x0 = 4044806;
        x2 = 825661;
        break;
    case 716 :
        x0 = 4040309;
        x2 = 818997;
        break;
    case 717 :
        x0 = 4035814;
        x2 = 812338;
        break;
    case 718 :
        x0 = 4031324;
        x2 = 805689;
        break;
    case 719 :
        x0 = 4026836;
        x2 = 799046;
        break;
    case 720 :
        x0 = 4022351;
        x2 = 792409;
        break;
    case 721 :
        x0 = 4017870;
        x2 = 785780;
        break;
    case 722 :
        x0 = 4013392;
        x2 = 779159;
        break;
    case 723 :
        x0 = 4008917;
        x2 = 772544;
        break;
    case 724 :
        x0 = 4004446;
        x2 = 765938;
        break;
    case 725 :
        x0 = 3999977;
        x2 = 759336;
        break;
    case 726 :
        x0 = 3995512;
        x2 = 752743;
        break;
    case 727 :
        x0 = 3991050;
        x2 = 746157;
        break;
    case 728 :
        x0 = 3986591;
        x2 = 739578;
        break;
    case 729 :
        x0 = 3982136;
        x2 = 733007;
        break;
    case 730 :
        x0 = 3977684;
        x2 = 726443;
        break;
    case 731 :
        x0 = 3973234;
        x2 = 719884;
        break;
    case 732 :
        x0 = 3968788;
        x2 = 713333;
        break;
    case 733 :
        x0 = 3964346;
        x2 = 706791;
        break;
    case 734 :
        x0 = 3959906;
        x2 = 700254;
        break;
    case 735 :
        x0 = 3955470;
        x2 = 693725;
        break;
    case 736 :
        x0 = 3951037;
        x2 = 687203;
        break;
    case 737 :
        x0 = 3946606;
        x2 = 680686;
        break;
    case 738 :
        x0 = 3942180;
        x2 = 674179;
        break;
    case 739 :
        x0 = 3937756;
        x2 = 667677;
        break;
    case 740 :
        x0 = 3933336;
        x2 = 661183;
        break;
    case 741 :
        x0 = 3928918;
        x2 = 654695;
        break;
    case 742 :
        x0 = 3924504;
        x2 = 648215;
        break;
    case 743 :
        x0 = 3920094;
        x2 = 641743;
        break;
    case 744 :
        x0 = 3915686;
        x2 = 635276;
        break;
    case 745 :
        x0 = 3911281;
        x2 = 628816;
        break;
    case 746 :
        x0 = 3906880;
        x2 = 622364;
        break;
    case 747 :
        x0 = 3902482;
        x2 = 615919;
        break;
    case 748 :
        x0 = 3898086;
        x2 = 609479;
        break;
    case 749 :
        x0 = 3893694;
        x2 = 603047;
        break;
    case 750 :
        x0 = 3889306;
        x2 = 596624;
        break;
    case 751 :
        x0 = 3884920;
        x2 = 590205;
        break;
    case 752 :
        x0 = 3880537;
        x2 = 583794;
        break;
    case 753 :
        x0 = 3876158;
        x2 = 577390;
        break;
    case 754 :
        x0 = 3871781;
        x2 = 570992;
        break;
    case 755 :
        x0 = 3867408;
        x2 = 564601;
        break;
    case 756 :
        x0 = 3863038;
        x2 = 558218;
        break;
    case 757 :
        x0 = 3858671;
        x2 = 551841;
        break;
    case 758 :
        x0 = 3854307;
        x2 = 545471;
        break;
    case 759 :
        x0 = 3849946;
        x2 = 539107;
        break;
    case 760 :
        x0 = 3845588;
        x2 = 532750;
        break;
    case 761 :
        x0 = 3841234;
        x2 = 526401;
        break;
    case 762 :
        x0 = 3836882;
        x2 = 520058;
        break;
    case 763 :
        x0 = 3832534;
        x2 = 513722;
        break;
    case 764 :
        x0 = 3828188;
        x2 = 507391;
        break;
    case 765 :
        x0 = 3823846;
        x2 = 501069;
        break;
    case 766 :
        x0 = 3819508;
        x2 = 494755;
        break;
    case 767 :
        x0 = 3815172;
        x2 = 488446;
        break;
    case 768 :
        x0 = 3810838;
        x2 = 482142;
        break;
    case 769 :
        x0 = 3806508;
        x2 = 475846;
        break;
    case 770 :
        x0 = 3802182;
        x2 = 469558;
        break;
    case 771 :
        x0 = 3797858;
        x2 = 463275;
        break;
    case 772 :
        x0 = 3793537;
        x2 = 456999;
        break;
    case 773 :
        x0 = 3789220;
        x2 = 450731;
        break;
    case 774 :
        x0 = 3784905;
        x2 = 444468;
        break;
    case 775 :
        x0 = 3780594;
        x2 = 438213;
        break;
    case 776 :
        x0 = 3776285;
        x2 = 431963;
        break;
    case 777 :
        x0 = 3771980;
        x2 = 425721;
        break;
    case 778 :
        x0 = 3767678;
        x2 = 419486;
        break;
    case 779 :
        x0 = 3763378;
        x2 = 413255;
        break;
    case 780 :
        x0 = 3759082;
        x2 = 407033;
        break;
    case 781 :
        x0 = 3754788;
        x2 = 400816;
        break;
    case 782 :
        x0 = 3750498;
        x2 = 394607;
        break;
    case 783 :
        x0 = 3746212;
        x2 = 388406;
        break;
    case 784 :
        x0 = 3741927;
        x2 = 382208;
        break;
    case 785 :
        x0 = 3737646;
        x2 = 376019;
        break;
    case 786 :
        x0 = 3733368;
        x2 = 369836;
        break;
    case 787 :
        x0 = 3729093;
        x2 = 363659;
        break;
    case 788 :
        x0 = 3724821;
        x2 = 357489;
        break;
    case 789 :
        x0 = 3720552;
        x2 = 351326;
        break;
    case 790 :
        x0 = 3716286;
        x2 = 345169;
        break;
    case 791 :
        x0 = 3712022;
        x2 = 339017;
        break;
    case 792 :
        x0 = 3707762;
        x2 = 332873;
        break;
    case 793 :
        x0 = 3703506;
        x2 = 326737;
        break;
    case 794 :
        x0 = 3699252;
        x2 = 320606;
        break;
    case 795 :
        x0 = 3695000;
        x2 = 314480;
        break;
    case 796 :
        x0 = 3690752;
        x2 = 308362;
        break;
    case 797 :
        x0 = 3686508;
        x2 = 302251;
        break;
    case 798 :
        x0 = 3682266;
        x2 = 296146;
        break;
    case 799 :
        x0 = 3678026;
        x2 = 290046;
        break;
    case 800 :
        x0 = 3673790;
        x2 = 283954;
        break;
    case 801 :
        x0 = 3669557;
        x2 = 277868;
        break;
    case 802 :
        x0 = 3665327;
        x2 = 271789;
        break;
    case 803 :
        x0 = 3661100;
        x2 = 265716;
        break;
    case 804 :
        x0 = 3656875;
        x2 = 259648;
        break;
    case 805 :
        x0 = 3652654;
        x2 = 253588;
        break;
    case 806 :
        x0 = 3648436;
        x2 = 247535;
        break;
    case 807 :
        x0 = 3644220;
        x2 = 241486;
        break;
    case 808 :
        x0 = 3640008;
        x2 = 235445;
        break;
    case 809 :
        x0 = 3635798;
        x2 = 229409;
        break;
    case 810 :
        x0 = 3631592;
        x2 = 223382;
        break;
    case 811 :
        x0 = 3627388;
        x2 = 217359;
        break;
    case 812 :
        x0 = 3623188;
        x2 = 211343;
        break;
    case 813 :
        x0 = 3618990;
        x2 = 205333;
        break;
    case 814 :
        x0 = 3614795;
        x2 = 199330;
        break;
    case 815 :
        x0 = 3610603;
        x2 = 193332;
        break;
    case 816 :
        x0 = 3606414;
        x2 = 187341;
        break;
    case 817 :
        x0 = 3602228;
        x2 = 181357;
        break;
    case 818 :
        x0 = 3598045;
        x2 = 175378;
        break;
    case 819 :
        x0 = 3593865;
        x2 = 169407;
        break;
    case 820 :
        x0 = 3589688;
        x2 = 163441;
        break;
    case 821 :
        x0 = 3585513;
        x2 = 157481;
        break;
    case 822 :
        x0 = 3581342;
        x2 = 151528;
        break;
    case 823 :
        x0 = 3577173;
        x2 = 145580;
        break;
    case 824 :
        x0 = 3573008;
        x2 = 139640;
        break;
    case 825 :
        x0 = 3568845;
        x2 = 133705;
        break;
    case 826 :
        x0 = 3564685;
        x2 = 127776;
        break;
    case 827 :
        x0 = 3560528;
        x2 = 121854;
        break;
    case 828 :
        x0 = 3556374;
        x2 = 115937;
        break;
    case 829 :
        x0 = 3552223;
        x2 = 110028;
        break;
    case 830 :
        x0 = 3548075;
        x2 = 104124;
        break;
    case 831 :
        x0 = 3543930;
        x2 = 98227;
        break;
    case 832 :
        x0 = 3539787;
        x2 = 92335;
        break;
    case 833 :
        x0 = 3535647;
        x2 = 86449;
        break;
    case 834 :
        x0 = 3531510;
        x2 = 80569;
        break;
    case 835 :
        x0 = 3527376;
        x2 = 74696;
        break;
    case 836 :
        x0 = 3523246;
        x2 = 68830;
        break;
    case 837 :
        x0 = 3519117;
        x2 = 62968;
        break;
    case 838 :
        x0 = 3514992;
        x2 = 57113;
        break;
    case 839 :
        x0 = 3510869;
        x2 = 51264;
        break;
    case 840 :
        x0 = 3506750;
        x2 = 45422;
        break;
    case 841 :
        x0 = 3502633;
        x2 = 39585;
        break;
    case 842 :
        x0 = 3498519;
        x2 = 33754;
        break;
    case 843 :
        x0 = 3494408;
        x2 = 27930;
        break;
    case 844 :
        x0 = 3490300;
        x2 = 22111;
        break;
    case 845 :
        x0 = 3486194;
        x2 = 16298;
        break;
    case 846 :
        x0 = 3482092;
        x2 = 10492;
        break;
    case 847 :
        x0 = 3477992;
        x2 = 4691;
        break;
    case 848 :
        x0 = 3473896;
        x2 = 8386404;
        break;
    case 849 :
        x0 = 3469801;
        x2 = 8374825;
        break;
    case 850 :
        x0 = 3465710;
        x2 = 8363260;
        break;
    case 851 :
        x0 = 3461622;
        x2 = 8351708;
        break;
    case 852 :
        x0 = 3457536;
        x2 = 8340166;
        break;
    case 853 :
        x0 = 3453453;
        x2 = 8328636;
        break;
    case 854 :
        x0 = 3449374;
        x2 = 8317122;
        break;
    case 855 :
        x0 = 3445296;
        x2 = 8305614;
        break;
    case 856 :
        x0 = 3441222;
        x2 = 8294121;
        break;
    case 857 :
        x0 = 3437150;
        x2 = 8282639;
        break;
    case 858 :
        x0 = 3433082;
        x2 = 8271171;
        break;
    case 859 :
        x0 = 3429016;
        x2 = 8259713;
        break;
    case 860 :
        x0 = 3424952;
        x2 = 8248264;
        break;
    case 861 :
        x0 = 3420892;
        x2 = 8236831;
        break;
    case 862 :
        x0 = 3416834;
        x2 = 8225407;
        break;
    case 863 :
        x0 = 3412780;
        x2 = 8213999;
        break;
    case 864 :
        x0 = 3408728;
        x2 = 8202600;
        break;
    case 865 :
        x0 = 3404678;
        x2 = 8191210;
        break;
    case 866 :
        x0 = 3400632;
        x2 = 8179836;
        break;
    case 867 :
        x0 = 3396588;
        x2 = 8168471;
        break;
    case 868 :
        x0 = 3392548;
        x2 = 8157121;
        break;
    case 869 :
        x0 = 3388510;
        x2 = 8145781;
        break;
    case 870 :
        x0 = 3384474;
        x2 = 8134450;
        break;
    case 871 :
        x0 = 3380442;
        x2 = 8123135;
        break;
    case 872 :
        x0 = 3376412;
        x2 = 8111829;
        break;
    case 873 :
        x0 = 3372385;
        x2 = 8100535;
        break;
    case 874 :
        x0 = 3368361;
        x2 = 8089253;
        break;
    case 875 :
        x0 = 3364339;
        x2 = 8077981;
        break;
    case 876 :
        x0 = 3360320;
        x2 = 8066722;
        break;
    case 877 :
        x0 = 3356304;
        x2 = 8055474;
        break;
    case 878 :
        x0 = 3352290;
        x2 = 8044236;
        break;
    case 879 :
        x0 = 3348280;
        x2 = 8033013;
        break;
    case 880 :
        x0 = 3344272;
        x2 = 8021799;
        break;
    case 881 :
        x0 = 3340268;
        x2 = 8010601;
        break;
    case 882 :
        x0 = 3336265;
        x2 = 7999409;
        break;
    case 883 :
        x0 = 3332266;
        x2 = 7988232;
        break;
    case 884 :
        x0 = 3328268;
        x2 = 7977061;
        break;
    case 885 :
        x0 = 3324274;
        x2 = 7965906;
        break;
    case 886 :
        x0 = 3320283;
        x2 = 7954762;
        break;
    case 887 :
        x0 = 3316294;
        x2 = 7943629;
        break;
    case 888 :
        x0 = 3312308;
        x2 = 7932507;
        break;
    case 889 :
        x0 = 3308326;
        x2 = 7921400;
        break;
    case 890 :
        x0 = 3304344;
        x2 = 7910297;
        break;
    case 891 :
        x0 = 3300366;
        x2 = 7899209;
        break;
    case 892 :
        x0 = 3296392;
        x2 = 7888136;
        break;
    case 893 :
        x0 = 3292419;
        x2 = 7877069;
        break;
    case 894 :
        x0 = 3288449;
        x2 = 7866015;
        break;
    case 895 :
        x0 = 3284482;
        x2 = 7854973;
        break;
    case 896 :
        x0 = 3280518;
        x2 = 7843942;
        break;
    case 897 :
        x0 = 3276556;
        x2 = 7832921;
        break;
    case 898 :
        x0 = 3272597;
        x2 = 7821913;
        break;
    case 899 :
        x0 = 3268641;
        x2 = 7810916;
        break;
    case 900 :
        x0 = 3264688;
        x2 = 7799931;
        break;
    case 901 :
        x0 = 3260736;
        x2 = 7788953;
        break;
    case 902 :
        x0 = 3256788;
        x2 = 7777989;
        break;
    case 903 :
        x0 = 3252842;
        x2 = 7767035;
        break;
    case 904 :
        x0 = 3248900;
        x2 = 7756096;
        break;
    case 905 :
        x0 = 3244959;
        x2 = 7745163;
        break;
    case 906 :
        x0 = 3241022;
        x2 = 7734245;
        break;
    case 907 :
        x0 = 3237086;
        x2 = 7723333;
        break;
    case 908 :
        x0 = 3233154;
        x2 = 7712437;
        break;
    case 909 :
        x0 = 3229225;
        x2 = 7701552;
        break;
    case 910 :
        x0 = 3225298;
        x2 = 7690676;
        break;
    case 911 :
        x0 = 3221374;
        x2 = 7679813;
        break;
    case 912 :
        x0 = 3217452;
        x2 = 7668958;
        break;
    case 913 :
        x0 = 3213533;
        x2 = 7658116;
        break;
    case 914 :
        x0 = 3209617;
        x2 = 7647285;
        break;
    case 915 :
        x0 = 3205703;
        x2 = 7636464;
        break;
    case 916 :
        x0 = 3201792;
        x2 = 7625655;
        break;
    case 917 :
        x0 = 3197884;
        x2 = 7614857;
        break;
    case 918 :
        x0 = 3193978;
        x2 = 7604069;
        break;
    case 919 :
        x0 = 3190074;
        x2 = 7593290;
        break;
    case 920 :
        x0 = 3186174;
        x2 = 7582526;
        break;
    case 921 :
        x0 = 3182276;
        x2 = 7571770;
        break;
    case 922 :
        x0 = 3178380;
        x2 = 7561024;
        break;
    case 923 :
        x0 = 3174488;
        x2 = 7550293;
        break;
    case 924 :
        x0 = 3170598;
        x2 = 7539570;
        break;
    case 925 :
        x0 = 3166710;
        x2 = 7528857;
        break;
    case 926 :
        x0 = 3162826;
        x2 = 7518158;
        break;
    case 927 :
        x0 = 3158944;
        x2 = 7507469;
        break;
    case 928 :
        x0 = 3155064;
        x2 = 7496788;
        break;
    case 929 :
        x0 = 3151188;
        x2 = 7486123;
        break;
    case 930 :
        x0 = 3147313;
        x2 = 7475463;
        break;
    case 931 :
        x0 = 3143441;
        x2 = 7464815;
        break;
    case 932 :
        x0 = 3139572;
        x2 = 7454180;
        break;
    case 933 :
        x0 = 3135706;
        x2 = 7443555;
        break;
    case 934 :
        x0 = 3131842;
        x2 = 7432940;
        break;
    case 935 :
        x0 = 3127980;
        x2 = 7422335;
        break;
    case 936 :
        x0 = 3124122;
        x2 = 7411743;
        break;
    case 937 :
        x0 = 3120265;
        x2 = 7401158;
        break;
    case 938 :
        x0 = 3116412;
        x2 = 7390587;
        break;
    case 939 :
        x0 = 3112560;
        x2 = 7380023;
        break;
    case 940 :
        x0 = 3108712;
        x2 = 7369473;
        break;
    case 941 :
        x0 = 3104866;
        x2 = 7358933;
        break;
    case 942 :
        x0 = 3101022;
        x2 = 7348401;
        break;
    case 943 :
        x0 = 3097182;
        x2 = 7337883;
        break;
    case 944 :
        x0 = 3093344;
        x2 = 7327375;
        break;
    case 945 :
        x0 = 3089508;
        x2 = 7316876;
        break;
    case 946 :
        x0 = 3085676;
        x2 = 7306391;
        break;
    case 947 :
        x0 = 3081844;
        x2 = 7295910;
        break;
    case 948 :
        x0 = 3078017;
        x2 = 7285445;
        break;
    case 949 :
        x0 = 3074192;
        x2 = 7274990;
        break;
    case 950 :
        x0 = 3070368;
        x2 = 7264541;
        break;
    case 951 :
        x0 = 3066548;
        x2 = 7254106;
        break;
    case 952 :
        x0 = 3062730;
        x2 = 7243681;
        break;
    case 953 :
        x0 = 3058916;
        x2 = 7233269;
        break;
    case 954 :
        x0 = 3055102;
        x2 = 7222862;
        break;
    case 955 :
        x0 = 3051292;
        x2 = 7212468;
        break;
    case 956 :
        x0 = 3047484;
        x2 = 7202084;
        break;
    case 957 :
        x0 = 3043680;
        x2 = 7191713;
        break;
    case 958 :
        x0 = 3039877;
        x2 = 7181349;
        break;
    case 959 :
        x0 = 3036077;
        x2 = 7170997;
        break;
    case 960 :
        x0 = 3032280;
        x2 = 7160656;
        break;
    case 961 :
        x0 = 3028484;
        x2 = 7150322;
        break;
    case 962 :
        x0 = 3024692;
        x2 = 7140001;
        break;
    case 963 :
        x0 = 3020902;
        x2 = 7129690;
        break;
    case 964 :
        x0 = 3017115;
        x2 = 7119390;
        break;
    case 965 :
        x0 = 3013330;
        x2 = 7109099;
        break;
    case 966 :
        x0 = 3009548;
        x2 = 7098820;
        break;
    case 967 :
        x0 = 3005768;
        x2 = 7088549;
        break;
    case 968 :
        x0 = 3001990;
        x2 = 7078287;
        break;
    case 969 :
        x0 = 2998216;
        x2 = 7068040;
        break;
    case 970 :
        x0 = 2994444;
        x2 = 7057801;
        break;
    case 971 :
        x0 = 2990674;
        x2 = 7047571;
        break;
    case 972 :
        x0 = 2986906;
        x2 = 7037350;
        break;
    case 973 :
        x0 = 2983142;
        x2 = 7027144;
        break;
    case 974 :
        x0 = 2979379;
        x2 = 7016943;
        break;
    case 975 :
        x0 = 2975619;
        x2 = 7006754;
        break;
    case 976 :
        x0 = 2971862;
        x2 = 6996576;
        break;
    case 977 :
        x0 = 2968106;
        x2 = 6986405;
        break;
    case 978 :
        x0 = 2964354;
        x2 = 6976247;
        break;
    case 979 :
        x0 = 2960604;
        x2 = 6966098;
        break;
    case 980 :
        x0 = 2956857;
        x2 = 6955961;
        break;
    case 981 :
        x0 = 2953112;
        x2 = 6945833;
        break;
    case 982 :
        x0 = 2949370;
        x2 = 6935716;
        break;
    case 983 :
        x0 = 2945629;
        x2 = 6925605;
        break;
    case 984 :
        x0 = 2941892;
        x2 = 6915508;
        break;
    case 985 :
        x0 = 2938156;
        x2 = 6905417;
        break;
    case 986 :
        x0 = 2934424;
        x2 = 6895341;
        break;
    case 987 :
        x0 = 2930694;
        x2 = 6885273;
        break;
    case 988 :
        x0 = 2926966;
        x2 = 6875213;
        break;
    case 989 :
        x0 = 2923240;
        x2 = 6865163;
        break;
    case 990 :
        x0 = 2919518;
        x2 = 6855127;
        break;
    case 991 :
        x0 = 2915798;
        x2 = 6845099;
        break;
    case 992 :
        x0 = 2912080;
        x2 = 6835080;
        break;
    case 993 :
        x0 = 2908364;
        x2 = 6825069;
        break;
    case 994 :
        x0 = 2904652;
        x2 = 6815073;
        break;
    case 995 :
        x0 = 2900940;
        x2 = 6805080;
        break;
    case 996 :
        x0 = 2897232;
        x2 = 6795101;
        break;
    case 997 :
        x0 = 2893526;
        x2 = 6785131;
        break;
    case 998 :
        x0 = 2889824;
        x2 = 6775175;
        break;
    case 999 :
        x0 = 2886122;
        x2 = 6765222;
        break;
    case 1000 :
        x0 = 2882424;
        x2 = 6755283;
        break;
    case 1001 :
        x0 = 2878728;
        x2 = 6745352;
        break;
    case 1002 :
        x0 = 2875034;
        x2 = 6735431;
        break;
    case 1003 :
        x0 = 2871344;
        x2 = 6725523;
        break;
    case 1004 :
        x0 = 2867654;
        x2 = 6715618;
        break;
    case 1005 :
        x0 = 2863968;
        x2 = 6705728;
        break;
    case 1006 :
        x0 = 2860284;
        x2 = 6695846;
        break;
    case 1007 :
        x0 = 2856602;
        x2 = 6685973;
        break;
    case 1008 :
        x0 = 2852924;
        x2 = 6676113;
        break;
    case 1009 :
        x0 = 2849246;
        x2 = 6666257;
        break;
    case 1010 :
        x0 = 2845572;
        x2 = 6656415;
        break;
    case 1011 :
        x0 = 2841900;
        x2 = 6646581;
        break;
    case 1012 :
        x0 = 2838231;
        x2 = 6636759;
        break;
    case 1013 :
        x0 = 2834564;
        x2 = 6626945;
        break;
    case 1014 :
        x0 = 2830899;
        x2 = 6617140;
        break;
    case 1015 :
        x0 = 2827237;
        x2 = 6607346;
        break;
    case 1016 :
        x0 = 2823577;
        x2 = 6597560;
        break;
    case 1017 :
        x0 = 2819920;
        x2 = 6587786;
        break;
    case 1018 :
        x0 = 2816264;
        x2 = 6578018;
        break;
    case 1019 :
        x0 = 2812612;
        x2 = 6568263;
        break;
    case 1020 :
        x0 = 2808961;
        x2 = 6558515;
        break;
    case 1021 :
        x0 = 2805313;
        x2 = 6548777;
        break;
    case 1022 :
        x0 = 2801668;
        x2 = 6539051;
        break;
    case 1023 :
        x0 = 2798024;
        x2 = 6529330;
        break;
    case 1024 :
        x0 = 2794384;
        x2 = 6519624;
        break;
    case 1025 :
        x0 = 2790745;
        x2 = 6509923;
        break;
    case 1026 :
        x0 = 2787109;
        x2 = 6500233;
        break;
    case 1027 :
        x0 = 2783475;
        x2 = 6490552;
        break;
    case 1028 :
        x0 = 2779844;
        x2 = 6480882;
        break;
    case 1029 :
        x0 = 2776214;
        x2 = 6471217;
        break;
    case 1030 :
        x0 = 2772588;
        x2 = 6461567;
        break;
    case 1031 :
        x0 = 2768963;
        x2 = 6451922;
        break;
    case 1032 :
        x0 = 2765341;
        x2 = 6442289;
        break;
    case 1033 :
        x0 = 2761722;
        x2 = 6432666;
        break;
    case 1034 :
        x0 = 2758104;
        x2 = 6423049;
        break;
    case 1035 :
        x0 = 2754490;
        x2 = 6413447;
        break;
    case 1036 :
        x0 = 2750876;
        x2 = 6403847;
        break;
    case 1037 :
        x0 = 2747266;
        x2 = 6394261;
        break;
    case 1038 :
        x0 = 2743658;
        x2 = 6384683;
        break;
    case 1039 :
        x0 = 2740053;
        x2 = 6375116;
        break;
    case 1040 :
        x0 = 2736450;
        x2 = 6365558;
        break;
    case 1041 :
        x0 = 2732848;
        x2 = 6356006;
        break;
    case 1042 :
        x0 = 2729250;
        x2 = 6346467;
        break;
    case 1043 :
        x0 = 2725654;
        x2 = 6336936;
        break;
    case 1044 :
        x0 = 2722060;
        x2 = 6327414;
        break;
    case 1045 :
        x0 = 2718468;
        x2 = 6317901;
        break;
    case 1046 :
        x0 = 2714879;
        x2 = 6308398;
        break;
    case 1047 :
        x0 = 2711292;
        x2 = 6298904;
        break;
    case 1048 :
        x0 = 2707708;
        x2 = 6289421;
        break;
    case 1049 :
        x0 = 2704125;
        x2 = 6279943;
        break;
    case 1050 :
        x0 = 2700545;
        x2 = 6270477;
        break;
    case 1051 :
        x0 = 2696967;
        x2 = 6261018;
        break;
    case 1052 :
        x0 = 2693392;
        x2 = 6251571;
        break;
    case 1053 :
        x0 = 2689818;
        x2 = 6242130;
        break;
    case 1054 :
        x0 = 2686248;
        x2 = 6232702;
        break;
    case 1055 :
        x0 = 2682680;
        x2 = 6223282;
        break;
    case 1056 :
        x0 = 2679113;
        x2 = 6213868;
        break;
    case 1057 :
        x0 = 2675549;
        x2 = 6204465;
        break;
    case 1058 :
        x0 = 2671988;
        x2 = 6195073;
        break;
    case 1059 :
        x0 = 2668428;
        x2 = 6185687;
        break;
    case 1060 :
        x0 = 2664872;
        x2 = 6176314;
        break;
    case 1061 :
        x0 = 2661316;
        x2 = 6166944;
        break;
    case 1062 :
        x0 = 2657764;
        x2 = 6157588;
        break;
    case 1063 :
        x0 = 2654214;
        x2 = 6148240;
        break;
    case 1064 :
        x0 = 2650666;
        x2 = 6138900;
        break;
    case 1065 :
        x0 = 2647120;
        x2 = 6129569;
        break;
    case 1066 :
        x0 = 2643577;
        x2 = 6120248;
        break;
    case 1067 :
        x0 = 2640036;
        x2 = 6110936;
        break;
    case 1068 :
        x0 = 2636498;
        x2 = 6101634;
        break;
    case 1069 :
        x0 = 2632960;
        x2 = 6092336;
        break;
    case 1070 :
        x0 = 2629426;
        x2 = 6083051;
        break;
    case 1071 :
        x0 = 2625895;
        x2 = 6073777;
        break;
    case 1072 :
        x0 = 2622365;
        x2 = 6064508;
        break;
    case 1073 :
        x0 = 2618838;
        x2 = 6055251;
        break;
    case 1074 :
        x0 = 2615312;
        x2 = 6045998;
        break;
    case 1075 :
        x0 = 2611790;
        x2 = 6036760;
        break;
    case 1076 :
        x0 = 2608268;
        x2 = 6027524;
        break;
    case 1077 :
        x0 = 2604750;
        x2 = 6018302;
        break;
    case 1078 :
        x0 = 2601234;
        x2 = 6009088;
        break;
    case 1079 :
        x0 = 2597720;
        x2 = 5999882;
        break;
    case 1080 :
        x0 = 2594208;
        x2 = 5990684;
        break;
    case 1081 :
        x0 = 2590699;
        x2 = 5981498;
        break;
    case 1082 :
        x0 = 2587192;
        x2 = 5972319;
        break;
    case 1083 :
        x0 = 2583686;
        x2 = 5963146;
        break;
    case 1084 :
        x0 = 2580184;
        x2 = 5953986;
        break;
    case 1085 :
        x0 = 2576684;
        x2 = 5944834;
        break;
    case 1086 :
        x0 = 2573185;
        x2 = 5935688;
        break;
    case 1087 :
        x0 = 2569689;
        x2 = 5926553;
        break;
    case 1088 :
        x0 = 2566195;
        x2 = 5917426;
        break;
    case 1089 :
        x0 = 2562704;
        x2 = 5908309;
        break;
    case 1090 :
        x0 = 2559214;
        x2 = 5899198;
        break;
    case 1091 :
        x0 = 2555728;
        x2 = 5890101;
        break;
    case 1092 :
        x0 = 2552242;
        x2 = 5881006;
        break;
    case 1093 :
        x0 = 2548760;
        x2 = 5871925;
        break;
    case 1094 :
        x0 = 2545279;
        x2 = 5862849;
        break;
    case 1095 :
        x0 = 2541801;
        x2 = 5853784;
        break;
    case 1096 :
        x0 = 2538325;
        x2 = 5844727;
        break;
    case 1097 :
        x0 = 2534851;
        x2 = 5835678;
        break;
    case 1098 :
        x0 = 2531380;
        x2 = 5826639;
        break;
    case 1099 :
        x0 = 2527910;
        x2 = 5817607;
        break;
    case 1100 :
        x0 = 2524442;
        x2 = 5808582;
        break;
    case 1101 :
        x0 = 2520978;
        x2 = 5799570;
        break;
    case 1102 :
        x0 = 2517515;
        x2 = 5790564;
        break;
    case 1103 :
        x0 = 2514054;
        x2 = 5781566;
        break;
    case 1104 :
        x0 = 2510596;
        x2 = 5772579;
        break;
    case 1105 :
        x0 = 2507140;
        x2 = 5763600;
        break;
    case 1106 :
        x0 = 2503686;
        x2 = 5754629;
        break;
    case 1107 :
        x0 = 2500234;
        x2 = 5745666;
        break;
    case 1108 :
        x0 = 2496784;
        x2 = 5736710;
        break;
    case 1109 :
        x0 = 2493337;
        x2 = 5727766;
        break;
    case 1110 :
        x0 = 2489892;
        x2 = 5718829;
        break;
    case 1111 :
        x0 = 2486448;
        x2 = 5709898;
        break;
    case 1112 :
        x0 = 2483008;
        x2 = 5700980;
        break;
    case 1113 :
        x0 = 2479568;
        x2 = 5692065;
        break;
    case 1114 :
        x0 = 2476132;
        x2 = 5683164;
        break;
    case 1115 :
        x0 = 2472698;
        x2 = 5674270;
        break;
    case 1116 :
        x0 = 2469266;
        x2 = 5665384;
        break;
    case 1117 :
        x0 = 2465835;
        x2 = 5656503;
        break;
    case 1118 :
        x0 = 2462407;
        x2 = 5647633;
        break;
    case 1119 :
        x0 = 2458982;
        x2 = 5638774;
        break;
    case 1120 :
        x0 = 2455558;
        x2 = 5629920;
        break;
    case 1121 :
        x0 = 2452137;
        x2 = 5621077;
        break;
    case 1122 :
        x0 = 2448718;
        x2 = 5612241;
        break;
    case 1123 :
        x0 = 2445300;
        x2 = 5603411;
        break;
    case 1124 :
        x0 = 2441886;
        x2 = 5594594;
        break;
    case 1125 :
        x0 = 2438472;
        x2 = 5585780;
        break;
    case 1126 :
        x0 = 2435062;
        x2 = 5576979;
        break;
    case 1127 :
        x0 = 2431654;
        x2 = 5568186;
        break;
    case 1128 :
        x0 = 2428247;
        x2 = 5559398;
        break;
    case 1129 :
        x0 = 2424843;
        x2 = 5550620;
        break;
    case 1130 :
        x0 = 2421441;
        x2 = 5541851;
        break;
    case 1131 :
        x0 = 2418041;
        x2 = 5533089;
        break;
    case 1132 :
        x0 = 2414643;
        x2 = 5524336;
        break;
    case 1133 :
        x0 = 2411248;
        x2 = 5515593;
        break;
    case 1134 :
        x0 = 2407854;
        x2 = 5506855;
        break;
    case 1135 :
        x0 = 2404462;
        x2 = 5498125;
        break;
    case 1136 :
        x0 = 2401074;
        x2 = 5489408;
        break;
    case 1137 :
        x0 = 2397686;
        x2 = 5480694;
        break;
    case 1138 :
        x0 = 2394302;
        x2 = 5471993;
        break;
    case 1139 :
        x0 = 2390918;
        x2 = 5463295;
        break;
    case 1140 :
        x0 = 2387538;
        x2 = 5454609;
        break;
    case 1141 :
        x0 = 2384159;
        x2 = 5445929;
        break;
    case 1142 :
        x0 = 2380782;
        x2 = 5437257;
        break;
    case 1143 :
        x0 = 2377408;
        x2 = 5428595;
        break;
    case 1144 :
        x0 = 2374036;
        x2 = 5419941;
        break;
    case 1145 :
        x0 = 2370666;
        x2 = 5411295;
        break;
    case 1146 :
        x0 = 2367298;
        x2 = 5402657;
        break;
    case 1147 :
        x0 = 2363932;
        x2 = 5394026;
        break;
    case 1148 :
        x0 = 2360568;
        x2 = 5385404;
        break;
    case 1149 :
        x0 = 2357206;
        x2 = 5376789;
        break;
    case 1150 :
        x0 = 2353846;
        x2 = 5368182;
        break;
    case 1151 :
        x0 = 2350489;
        x2 = 5359585;
        break;
    case 1152 :
        x0 = 2347134;
        x2 = 5350997;
        break;
    case 1153 :
        x0 = 2343780;
        x2 = 5342413;
        break;
    case 1154 :
        x0 = 2340429;
        x2 = 5333840;
        break;
    case 1155 :
        x0 = 2337080;
        x2 = 5325274;
        break;
    case 1156 :
        x0 = 2333732;
        x2 = 5316714;
        break;
    case 1157 :
        x0 = 2330388;
        x2 = 5308167;
        break;
    case 1158 :
        x0 = 2327044;
        x2 = 5299622;
        break;
    case 1159 :
        x0 = 2323704;
        x2 = 5291091;
        break;
    case 1160 :
        x0 = 2320366;
        x2 = 5282567;
        break;
    case 1161 :
        x0 = 2317028;
        x2 = 5274045;
        break;
    case 1162 :
        x0 = 2313694;
        x2 = 5265537;
        break;
    case 1163 :
        x0 = 2310362;
        x2 = 5257036;
        break;
    case 1164 :
        x0 = 2307031;
        x2 = 5248541;
        break;
    case 1165 :
        x0 = 2303702;
        x2 = 5240053;
        break;
    case 1166 :
        x0 = 2300376;
        x2 = 5231575;
        break;
    case 1167 :
        x0 = 2297052;
        x2 = 5223106;
        break;
    case 1168 :
        x0 = 2293730;
        x2 = 5214644;
        break;
    case 1169 :
        x0 = 2290410;
        x2 = 5206189;
        break;
    case 1170 :
        x0 = 2287092;
        x2 = 5197743;
        break;
    case 1171 :
        x0 = 2283776;
        x2 = 5189304;
        break;
    case 1172 :
        x0 = 2280462;
        x2 = 5180873;
        break;
    case 1173 :
        x0 = 2277150;
        x2 = 5172449;
        break;
    case 1174 :
        x0 = 2273841;
        x2 = 5164036;
        break;
    case 1175 :
        x0 = 2270533;
        x2 = 5155628;
        break;
    case 1176 :
        x0 = 2267227;
        x2 = 5147228;
        break;
    case 1177 :
        x0 = 2263924;
        x2 = 5138838;
        break;
    case 1178 :
        x0 = 2260622;
        x2 = 5130453;
        break;
    case 1179 :
        x0 = 2257322;
        x2 = 5122075;
        break;
    case 1180 :
        x0 = 2254025;
        x2 = 5113708;
        break;
    case 1181 :
        x0 = 2250730;
        x2 = 5105349;
        break;
    case 1182 :
        x0 = 2247436;
        x2 = 5096994;
        break;
    case 1183 :
        x0 = 2244145;
        x2 = 5088650;
        break;
    case 1184 :
        x0 = 2240856;
        x2 = 5080314;
        break;
    case 1185 :
        x0 = 2237568;
        x2 = 5071982;
        break;
    case 1186 :
        x0 = 2234283;
        x2 = 5063661;
        break;
    case 1187 :
        x0 = 2231000;
        x2 = 5055348;
        break;
    case 1188 :
        x0 = 2227718;
        x2 = 5047039;
        break;
    case 1189 :
        x0 = 2224440;
        x2 = 5038743;
        break;
    case 1190 :
        x0 = 2221162;
        x2 = 5030450;
        break;
    case 1191 :
        x0 = 2217887;
        x2 = 5022167;
        break;
    case 1192 :
        x0 = 2214614;
        x2 = 5013892;
        break;
    case 1193 :
        x0 = 2211342;
        x2 = 5005621;
        break;
    case 1194 :
        x0 = 2208074;
        x2 = 4997364;
        break;
    case 1195 :
        x0 = 2204806;
        x2 = 4989109;
        break;
    case 1196 :
        x0 = 2201542;
        x2 = 4980866;
        break;
    case 1197 :
        x0 = 2198278;
        x2 = 4972626;
        break;
    case 1198 :
        x0 = 2195018;
        x2 = 4964399;
        break;
    case 1199 :
        x0 = 2191758;
        x2 = 4956174;
        break;
    case 1200 :
        x0 = 2188502;
        x2 = 4947962;
        break;
    case 1201 :
        x0 = 2185246;
        x2 = 4939752;
        break;
    case 1202 :
        x0 = 2181994;
        x2 = 4931555;
        break;
    case 1203 :
        x0 = 2178742;
        x2 = 4923361;
        break;
    case 1204 :
        x0 = 2175494;
        x2 = 4915179;
        break;
    case 1205 :
        x0 = 2172246;
        x2 = 4906999;
        break;
    case 1206 :
        x0 = 2169002;
        x2 = 4898832;
        break;
    case 1207 :
        x0 = 2165758;
        x2 = 4890668;
        break;
    case 1208 :
        x0 = 2162518;
        x2 = 4882516;
        break;
    case 1209 :
        x0 = 2159278;
        x2 = 4874367;
        break;
    case 1210 :
        x0 = 2156042;
        x2 = 4866230;
        break;
    case 1211 :
        x0 = 2152806;
        x2 = 4858096;
        break;
    case 1212 :
        x0 = 2149574;
        x2 = 4849975;
        break;
    case 1213 :
        x0 = 2146342;
        x2 = 4841855;
        break;
    case 1214 :
        x0 = 2143114;
        x2 = 4833749;
        break;
    case 1215 :
        x0 = 2139886;
        x2 = 4825645;
        break;
    case 1216 :
        x0 = 2136661;
        x2 = 4817551;
        break;
    case 1217 :
        x0 = 2133438;
        x2 = 4809464;
        break;
    case 1218 :
        x0 = 2130216;
        x2 = 4801382;
        break;
    case 1219 :
        x0 = 2126998;
        x2 = 4793313;
        break;
    case 1220 :
        x0 = 2123780;
        x2 = 4785247;
        break;
    case 1221 :
        x0 = 2120565;
        x2 = 4777190;
        break;
    case 1222 :
        x0 = 2117352;
        x2 = 4769141;
        break;
    case 1223 :
        x0 = 2114140;
        x2 = 4761096;
        break;
    case 1224 :
        x0 = 2110931;
        x2 = 4753062;
        break;
    case 1225 :
        x0 = 2107723;
        x2 = 4745033;
        break;
    case 1226 :
        x0 = 2104518;
        x2 = 4737013;
        break;
    case 1227 :
        x0 = 2101314;
        x2 = 4728999;
        break;
    case 1228 :
        x0 = 2098113;
        x2 = 4720995;
        break;
    case 1229 :
        x0 = 2094913;
        x2 = 4712995;
        break;
    case 1230 :
        x0 = 2091716;
        x2 = 4705006;
        break;
    case 1231 :
        x0 = 2088520;
        x2 = 4697021;
        break;
    case 1232 :
        x0 = 2085326;
        x2 = 4689044;
        break;
    case 1233 :
        x0 = 2082134;
        x2 = 4681074;
        break;
    case 1234 :
        x0 = 2078944;
        x2 = 4673112;
        break;
    case 1235 :
        x0 = 2075756;
        x2 = 4665157;
        break;
    case 1236 :
        x0 = 2072570;
        x2 = 4657209;
        break;
    case 1237 :
        x0 = 2069386;
        x2 = 4649269;
        break;
    case 1238 :
        x0 = 2066204;
        x2 = 4641336;
        break;
    case 1239 :
        x0 = 2063024;
        x2 = 4633411;
        break;
    case 1240 :
        x0 = 2059846;
        x2 = 4625493;
        break;
    case 1241 :
        x0 = 2056670;
        x2 = 4617582;
        break;
    case 1242 :
        x0 = 2053495;
        x2 = 4609677;
        break;
    case 1243 :
        x0 = 2050322;
        x2 = 4601778;
        break;
    case 1244 :
        x0 = 2047152;
        x2 = 4593890;
        break;
    case 1245 :
        x0 = 2043984;
        x2 = 4586009;
        break;
    case 1246 :
        x0 = 2040816;
        x2 = 4578130;
        break;
    case 1247 :
        x0 = 2037652;
        x2 = 4570264;
        break;
    case 1248 :
        x0 = 2034490;
        x2 = 4562405;
        break;
    case 1249 :
        x0 = 2031328;
        x2 = 4554549;
        break;
    case 1250 :
        x0 = 2028170;
        x2 = 4546704;
        break;
    case 1251 :
        x0 = 2025012;
        x2 = 4538862;
        break;
    case 1252 :
        x0 = 2021857;
        x2 = 4531030;
        break;
    case 1253 :
        x0 = 2018704;
        x2 = 4523206;
        break;
    case 1254 :
        x0 = 2015552;
        x2 = 4515386;
        break;
    case 1255 :
        x0 = 2012403;
        x2 = 4507576;
        break;
    case 1256 :
        x0 = 2009256;
        x2 = 4499773;
        break;
    case 1257 :
        x0 = 2006110;
        x2 = 4491975;
        break;
    case 1258 :
        x0 = 2002966;
        x2 = 4484185;
        break;
    case 1259 :
        x0 = 1999824;
        x2 = 4476401;
        break;
    case 1260 :
        x0 = 1996684;
        x2 = 4468625;
        break;
    case 1261 :
        x0 = 1993546;
        x2 = 4460857;
        break;
    case 1262 :
        x0 = 1990410;
        x2 = 4453095;
        break;
    case 1263 :
        x0 = 1987276;
        x2 = 4445341;
        break;
    case 1264 :
        x0 = 1984144;
        x2 = 4437594;
        break;
    case 1265 :
        x0 = 1981014;
        x2 = 4429855;
        break;
    case 1266 :
        x0 = 1977884;
        x2 = 4422118;
        break;
    case 1267 :
        x0 = 1974758;
        x2 = 4414393;
        break;
    case 1268 :
        x0 = 1971634;
        x2 = 4406675;
        break;
    case 1269 :
        x0 = 1968510;
        x2 = 4398960;
        break;
    case 1270 :
        x0 = 1965390;
        x2 = 4391257;
        break;
    case 1271 :
        x0 = 1962270;
        x2 = 4383556;
        break;
    case 1272 :
        x0 = 1959153;
        x2 = 4375865;
        break;
    case 1273 :
        x0 = 1956038;
        x2 = 4368181;
        break;
    case 1274 :
        x0 = 1952924;
        x2 = 4360502;
        break;
    case 1275 :
        x0 = 1949812;
        x2 = 4352830;
        break;
    case 1276 :
        x0 = 1946703;
        x2 = 4345168;
        break;
    case 1277 :
        x0 = 1943595;
        x2 = 4337510;
        break;
    case 1278 :
        x0 = 1940489;
        x2 = 4329860;
        break;
    case 1279 :
        x0 = 1937384;
        x2 = 4322215;
        break;
    case 1280 :
        x0 = 1934282;
        x2 = 4314579;
        break;
    case 1281 :
        x0 = 1931182;
        x2 = 4306951;
        break;
    case 1282 :
        x0 = 1928084;
        x2 = 4299329;
        break;
    case 1283 :
        x0 = 1924986;
        x2 = 4291710;
        break;
    case 1284 :
        x0 = 1921892;
        x2 = 4284104;
        break;
    case 1285 :
        x0 = 1918798;
        x2 = 4276499;
        break;
    case 1286 :
        x0 = 1915708;
        x2 = 4268907;
        break;
    case 1287 :
        x0 = 1912618;
        x2 = 4261316;
        break;
    case 1288 :
        x0 = 1909531;
        x2 = 4253736;
        break;
    case 1289 :
        x0 = 1906445;
        x2 = 4246160;
        break;
    case 1290 :
        x0 = 1903362;
        x2 = 4238594;
        break;
    case 1291 :
        x0 = 1900280;
        x2 = 4231032;
        break;
    case 1292 :
        x0 = 1897200;
        x2 = 4223478;
        break;
    case 1293 :
        x0 = 1894122;
        x2 = 4215931;
        break;
    case 1294 :
        x0 = 1891045;
        x2 = 4208389;
        break;
    case 1295 :
        x0 = 1887970;
        x2 = 4200853;
        break;
    case 1296 :
        x0 = 1884898;
        x2 = 4193328;
        break;
    case 1297 :
        x0 = 1881827;
        x2 = 4185807;
        break;
    case 1298 :
        x0 = 1878758;
        x2 = 4178293;
        break;
    case 1299 :
        x0 = 1875691;
        x2 = 4170786;
        break;
    case 1300 :
        x0 = 1872626;
        x2 = 4163287;
        break;
    case 1301 :
        x0 = 1869562;
        x2 = 4155792;
        break;
    case 1302 :
        x0 = 1866500;
        x2 = 4148304;
        break;
    case 1303 :
        x0 = 1863440;
        x2 = 4140823;
        break;
    case 1304 :
        x0 = 1860382;
        x2 = 4133350;
        break;
    case 1305 :
        x0 = 1857326;
        x2 = 4125884;
        break;
    case 1306 :
        x0 = 1854272;
        x2 = 4118424;
        break;
    case 1307 :
        x0 = 1851220;
        x2 = 4110972;
        break;
    case 1308 :
        x0 = 1848168;
        x2 = 4103522;
        break;
    case 1309 :
        x0 = 1845120;
        x2 = 4096084;
        break;
    case 1310 :
        x0 = 1842072;
        x2 = 4088648;
        break;
    case 1311 :
        x0 = 1839027;
        x2 = 4081222;
        break;
    case 1312 :
        x0 = 1835984;
        x2 = 4073803;
        break;
    case 1313 :
        x0 = 1832942;
        x2 = 4066389;
        break;
    case 1314 :
        x0 = 1829902;
        x2 = 4058981;
        break;
    case 1315 :
        x0 = 1826864;
        x2 = 4051581;
        break;
    case 1316 :
        x0 = 1823828;
        x2 = 4044188;
        break;
    case 1317 :
        x0 = 1820794;
        x2 = 4036801;
        break;
    case 1318 :
        x0 = 1817760;
        x2 = 4029417;
        break;
    case 1319 :
        x0 = 1814730;
        x2 = 4022045;
        break;
    case 1320 :
        x0 = 1811701;
        x2 = 4014678;
        break;
    case 1321 :
        x0 = 1808674;
        x2 = 4007318;
        break;
    case 1322 :
        x0 = 1805648;
        x2 = 3999962;
        break;
    case 1323 :
        x0 = 1802624;
        x2 = 3992613;
        break;
    case 1324 :
        x0 = 1799603;
        x2 = 3985274;
        break;
    case 1325 :
        x0 = 1796582;
        x2 = 3977937;
        break;
    case 1326 :
        x0 = 1793564;
        x2 = 3970609;
        break;
    case 1327 :
        x0 = 1790548;
        x2 = 3963288;
        break;
    case 1328 :
        x0 = 1787534;
        x2 = 3955975;
        break;
    case 1329 :
        x0 = 1784520;
        x2 = 3948663;
        break;
    case 1330 :
        x0 = 1781509;
        x2 = 3941361;
        break;
    case 1331 :
        x0 = 1778500;
        x2 = 3934066;
        break;
    case 1332 :
        x0 = 1775492;
        x2 = 3926776;
        break;
    case 1333 :
        x0 = 1772486;
        x2 = 3919493;
        break;
    case 1334 :
        x0 = 1769482;
        x2 = 3912216;
        break;
    case 1335 :
        x0 = 1766480;
        x2 = 3904947;
        break;
    case 1336 :
        x0 = 1763480;
        x2 = 3897684;
        break;
    case 1337 :
        x0 = 1760480;
        x2 = 3890424;
        break;
    case 1338 :
        x0 = 1757484;
        x2 = 3883176;
        break;
    case 1339 :
        x0 = 1754489;
        x2 = 3875932;
        break;
    case 1340 :
        x0 = 1751496;
        x2 = 3868695;
        break;
    case 1341 :
        x0 = 1748504;
        x2 = 3861463;
        break;
    case 1342 :
        x0 = 1745514;
        x2 = 3854237;
        break;
    case 1343 :
        x0 = 1742526;
        x2 = 3847019;
        break;
    case 1344 :
        x0 = 1739540;
        x2 = 3839807;
        break;
    case 1345 :
        x0 = 1736554;
        x2 = 3832598;
        break;
    case 1346 :
        x0 = 1733572;
        x2 = 3825400;
        break;
    case 1347 :
        x0 = 1730591;
        x2 = 3818207;
        break;
    case 1348 :
        x0 = 1727612;
        x2 = 3811021;
        break;
    case 1349 :
        x0 = 1724634;
        x2 = 3803840;
        break;
    case 1350 :
        x0 = 1721658;
        x2 = 3796665;
        break;
    case 1351 :
        x0 = 1718684;
        x2 = 3789497;
        break;
    case 1352 :
        x0 = 1715712;
        x2 = 3782337;
        break;
    case 1353 :
        x0 = 1712742;
        x2 = 3775183;
        break;
    case 1354 :
        x0 = 1709772;
        x2 = 3768031;
        break;
    case 1355 :
        x0 = 1706806;
        x2 = 3760891;
        break;
    case 1356 :
        x0 = 1703840;
        x2 = 3753753;
        break;
    case 1357 :
        x0 = 1700877;
        x2 = 3746624;
        break;
    case 1358 :
        x0 = 1697915;
        x2 = 3739500;
        break;
    case 1359 :
        x0 = 1694955;
        x2 = 3732383;
        break;
    case 1360 :
        x0 = 1691996;
        x2 = 3725270;
        break;
    case 1361 :
        x0 = 1689040;
        x2 = 3718167;
        break;
    case 1362 :
        x0 = 1686085;
        x2 = 3711068;
        break;
    case 1363 :
        x0 = 1683132;
        x2 = 3703976;
        break;
    case 1364 :
        x0 = 1680180;
        x2 = 3696888;
        break;
    case 1365 :
        x0 = 1677231;
        x2 = 3689810;
        break;
    case 1366 :
        x0 = 1674283;
        x2 = 3682736;
        break;
    case 1367 :
        x0 = 1671336;
        x2 = 3675667;
        break;
    case 1368 :
        x0 = 1668392;
        x2 = 3668607;
        break;
    case 1369 :
        x0 = 1665450;
        x2 = 3661554;
        break;
    case 1370 :
        x0 = 1662508;
        x2 = 3654502;
        break;
    case 1371 :
        x0 = 1659569;
        x2 = 3647461;
        break;
    case 1372 :
        x0 = 1656631;
        x2 = 3640423;
        break;
    case 1373 :
        x0 = 1653695;
        x2 = 3633392;
        break;
    case 1374 :
        x0 = 1650761;
        x2 = 3626369;
        break;
    case 1375 :
        x0 = 1647828;
        x2 = 3619349;
        break;
    case 1376 :
        x0 = 1644898;
        x2 = 3612339;
        break;
    case 1377 :
        x0 = 1641968;
        x2 = 3605331;
        break;
    case 1378 :
        x0 = 1639041;
        x2 = 3598332;
        break;
    case 1379 :
        x0 = 1636116;
        x2 = 3591340;
        break;
    case 1380 :
        x0 = 1633192;
        x2 = 3584353;
        break;
    case 1381 :
        x0 = 1630270;
        x2 = 3577372;
        break;
    case 1382 :
        x0 = 1627348;
        x2 = 3570393;
        break;
    case 1383 :
        x0 = 1624430;
        x2 = 3563426;
        break;
    case 1384 :
        x0 = 1621513;
        x2 = 3556464;
        break;
    case 1385 :
        x0 = 1618598;
        x2 = 3549508;
        break;
    case 1386 :
        x0 = 1615684;
        x2 = 3542556;
        break;
    case 1387 :
        x0 = 1612772;
        x2 = 3535611;
        break;
    case 1388 :
        x0 = 1609862;
        x2 = 3528673;
        break;
    case 1389 :
        x0 = 1606952;
        x2 = 3521738;
        break;
    case 1390 :
        x0 = 1604046;
        x2 = 3514813;
        break;
    case 1391 :
        x0 = 1601140;
        x2 = 3507891;
        break;
    case 1392 :
        x0 = 1598237;
        x2 = 3500978;
        break;
    case 1393 :
        x0 = 1595335;
        x2 = 3494069;
        break;
    case 1394 :
        x0 = 1592435;
        x2 = 3487167;
        break;
    case 1395 :
        x0 = 1589536;
        x2 = 3480269;
        break;
    case 1396 :
        x0 = 1586640;
        x2 = 3473381;
        break;
    case 1397 :
        x0 = 1583744;
        x2 = 3466494;
        break;
    case 1398 :
        x0 = 1580851;
        x2 = 3459617;
        break;
    case 1399 :
        x0 = 1577959;
        x2 = 3452744;
        break;
    case 1400 :
        x0 = 1575069;
        x2 = 3445877;
        break;
    case 1401 :
        x0 = 1572180;
        x2 = 3439016;
        break;
    case 1402 :
        x0 = 1569294;
        x2 = 3432163;
        break;
    case 1403 :
        x0 = 1566409;
        x2 = 3425314;
        break;
    case 1404 :
        x0 = 1563525;
        x2 = 3418470;
        break;
    case 1405 :
        x0 = 1560644;
        x2 = 3411635;
        break;
    case 1406 :
        x0 = 1557764;
        x2 = 3404805;
        break;
    case 1407 :
        x0 = 1554886;
        x2 = 3397981;
        break;
    case 1408 :
        x0 = 1552008;
        x2 = 3391159;
        break;
    case 1409 :
        x0 = 1549133;
        x2 = 3384346;
        break;
    case 1410 :
        x0 = 1546260;
        x2 = 3377540;
        break;
    case 1411 :
        x0 = 1543388;
        x2 = 3370738;
        break;
    case 1412 :
        x0 = 1540518;
        x2 = 3363943;
        break;
    case 1413 :
        x0 = 1537650;
        x2 = 3357155;
        break;
    case 1414 :
        x0 = 1534782;
        x2 = 3350368;
        break;
    case 1415 :
        x0 = 1531918;
        x2 = 3343593;
        break;
    case 1416 :
        x0 = 1529054;
        x2 = 3336820;
        break;
    case 1417 :
        x0 = 1526192;
        x2 = 3330054;
        break;
    case 1418 :
        x0 = 1523332;
        x2 = 3323294;
        break;
    case 1419 :
        x0 = 1520474;
        x2 = 3316541;
        break;
    case 1420 :
        x0 = 1517616;
        x2 = 3309790;
        break;
    case 1421 :
        x0 = 1514762;
        x2 = 3303050;
        break;
    case 1422 :
        x0 = 1511908;
        x2 = 3296312;
        break;
    case 1423 :
        x0 = 1509056;
        x2 = 3289581;
        break;
    case 1424 :
        x0 = 1506206;
        x2 = 3282857;
        break;
    case 1425 :
        x0 = 1503357;
        x2 = 3276137;
        break;
    case 1426 :
        x0 = 1500510;
        x2 = 3269423;
        break;
    case 1427 :
        x0 = 1497664;
        x2 = 3262714;
        break;
    case 1428 :
        x0 = 1494821;
        x2 = 3256014;
        break;
    case 1429 :
        x0 = 1491978;
        x2 = 3249316;
        break;
    case 1430 :
        x0 = 1489138;
        x2 = 3242626;
        break;
    case 1431 :
        x0 = 1486300;
        x2 = 3235944;
        break;
    case 1432 :
        x0 = 1483462;
        x2 = 3229263;
        break;
    case 1433 :
        x0 = 1480627;
        x2 = 3222591;
        break;
    case 1434 :
        x0 = 1477792;
        x2 = 3215921;
        break;
    case 1435 :
        x0 = 1474960;
        x2 = 3209261;
        break;
    case 1436 :
        x0 = 1472130;
        x2 = 3202606;
        break;
    case 1437 :
        x0 = 1469300;
        x2 = 3195954;
        break;
    case 1438 :
        x0 = 1466473;
        x2 = 3189311;
        break;
    case 1439 :
        x0 = 1463647;
        x2 = 3182672;
        break;
    case 1440 :
        x0 = 1460823;
        x2 = 3176039;
        break;
    case 1441 :
        x0 = 1458000;
        x2 = 3169411;
        break;
    case 1442 :
        x0 = 1455180;
        x2 = 3162791;
        break;
    case 1443 :
        x0 = 1452360;
        x2 = 3156174;
        break;
    case 1444 :
        x0 = 1449542;
        x2 = 3149563;
        break;
    case 1445 :
        x0 = 1446726;
        x2 = 3142959;
        break;
    case 1446 :
        x0 = 1443912;
        x2 = 3136361;
        break;
    case 1447 :
        x0 = 1441099;
        x2 = 3129768;
        break;
    case 1448 :
        x0 = 1438288;
        x2 = 3123181;
        break;
    case 1449 :
        x0 = 1435478;
        x2 = 3116598;
        break;
    case 1450 :
        x0 = 1432670;
        x2 = 3110022;
        break;
    case 1451 :
        x0 = 1429864;
        x2 = 3103453;
        break;
    case 1452 :
        x0 = 1427058;
        x2 = 3096885;
        break;
    case 1453 :
        x0 = 1424255;
        x2 = 3090326;
        break;
    case 1454 :
        x0 = 1421454;
        x2 = 3083774;
        break;
    case 1455 :
        x0 = 1418654;
        x2 = 3077226;
        break;
    case 1456 :
        x0 = 1415854;
        x2 = 3070680;
        break;
    case 1457 :
        x0 = 1413058;
        x2 = 3064145;
        break;
    case 1458 :
        x0 = 1410263;
        x2 = 3057614;
        break;
    case 1459 :
        x0 = 1407469;
        x2 = 3051088;
        break;
    case 1460 :
        x0 = 1404677;
        x2 = 3044568;
        break;
    case 1461 :
        x0 = 1401886;
        x2 = 3038052;
        break;
    case 1462 :
        x0 = 1399098;
        x2 = 3031545;
        break;
    case 1463 :
        x0 = 1396310;
        x2 = 3025040;
        break;
    case 1464 :
        x0 = 1393524;
        x2 = 3018542;
        break;
    case 1465 :
        x0 = 1390740;
        x2 = 3012050;
        break;
    case 1466 :
        x0 = 1387958;
        x2 = 3005564;
        break;
    case 1467 :
        x0 = 1385177;
        x2 = 2999083;
        break;
    case 1468 :
        x0 = 1382397;
        x2 = 2992606;
        break;
    case 1469 :
        x0 = 1379620;
        x2 = 2986137;
        break;
    case 1470 :
        x0 = 1376844;
        x2 = 2979673;
        break;
    case 1471 :
        x0 = 1374068;
        x2 = 2973211;
        break;
    case 1472 :
        x0 = 1371296;
        x2 = 2966759;
        break;
    case 1473 :
        x0 = 1368524;
        x2 = 2960310;
        break;
    case 1474 :
        x0 = 1365754;
        x2 = 2953867;
        break;
    case 1475 :
        x0 = 1362986;
        x2 = 2947431;
        break;
    case 1476 :
        x0 = 1360219;
        x2 = 2940999;
        break;
    case 1477 :
        x0 = 1357454;
        x2 = 2934573;
        break;
    case 1478 :
        x0 = 1354690;
        x2 = 2928151;
        break;
    case 1479 :
        x0 = 1351928;
        x2 = 2921736;
        break;
    case 1480 :
        x0 = 1349168;
        x2 = 2915327;
        break;
    case 1481 :
        x0 = 1346408;
        x2 = 2908920;
        break;
    case 1482 :
        x0 = 1343651;
        x2 = 2902522;
        break;
    case 1483 :
        x0 = 1340895;
        x2 = 2896128;
        break;
    case 1484 :
        x0 = 1338140;
        x2 = 2889738;
        break;
    case 1485 :
        x0 = 1335388;
        x2 = 2883357;
        break;
    case 1486 :
        x0 = 1332637;
        x2 = 2876980;
        break;
    case 1487 :
        x0 = 1329887;
        x2 = 2870607;
        break;
    case 1488 :
        x0 = 1327140;
        x2 = 2864243;
        break;
    case 1489 :
        x0 = 1324392;
        x2 = 2857879;
        break;
    case 1490 :
        x0 = 1321648;
        x2 = 2851525;
        break;
    case 1491 :
        x0 = 1318904;
        x2 = 2845173;
        break;
    case 1492 :
        x0 = 1316162;
        x2 = 2838828;
        break;
    case 1493 :
        x0 = 1313422;
        x2 = 2832489;
        break;
    case 1494 :
        x0 = 1310684;
        x2 = 2826157;
        break;
    case 1495 :
        x0 = 1307946;
        x2 = 2819826;
        break;
    case 1496 :
        x0 = 1305211;
        x2 = 2813504;
        break;
    case 1497 :
        x0 = 1302476;
        x2 = 2807184;
        break;
    case 1498 :
        x0 = 1299744;
        x2 = 2800872;
        break;
    case 1499 :
        x0 = 1297013;
        x2 = 2794565;
        break;
    case 1500 :
        x0 = 1294283;
        x2 = 2788262;
        break;
    case 1501 :
        x0 = 1291556;
        x2 = 2781967;
        break;
    case 1502 :
        x0 = 1288830;
        x2 = 2775676;
        break;
    case 1503 :
        x0 = 1286104;
        x2 = 2769388;
        break;
    case 1504 :
        x0 = 1283381;
        x2 = 2763108;
        break;
    case 1505 :
        x0 = 1280659;
        x2 = 2756832;
        break;
    case 1506 :
        x0 = 1277938;
        x2 = 2750560;
        break;
    case 1507 :
        x0 = 1275220;
        x2 = 2744296;
        break;
    case 1508 :
        x0 = 1272503;
        x2 = 2738037;
        break;
    case 1509 :
        x0 = 1269787;
        x2 = 2731782;
        break;
    case 1510 :
        x0 = 1267072;
        x2 = 2725531;
        break;
    case 1511 :
        x0 = 1264360;
        x2 = 2719289;
        break;
    case 1512 :
        x0 = 1261649;
        x2 = 2713050;
        break;
    case 1513 :
        x0 = 1258940;
        x2 = 2706818;
        break;
    case 1514 :
        x0 = 1256232;
        x2 = 2700590;
        break;
    case 1515 :
        x0 = 1253524;
        x2 = 2694364;
        break;
    case 1516 :
        x0 = 1250820;
        x2 = 2688149;
        break;
    case 1517 :
        x0 = 1248116;
        x2 = 2681935;
        break;
    case 1518 :
        x0 = 1245414;
        x2 = 2675728;
        break;
    case 1519 :
        x0 = 1242714;
        x2 = 2669527;
        break;
    case 1520 :
        x0 = 1240015;
        x2 = 2663331;
        break;
    case 1521 :
        x0 = 1237317;
        x2 = 2657138;
        break;
    case 1522 :
        x0 = 1234622;
        x2 = 2650954;
        break;
    case 1523 :
        x0 = 1231926;
        x2 = 2644769;
        break;
    case 1524 :
        x0 = 1229234;
        x2 = 2638595;
        break;
    case 1525 :
        x0 = 1226542;
        x2 = 2632423;
        break;
    case 1526 :
        x0 = 1223852;
        x2 = 2626257;
        break;
    case 1527 :
        x0 = 1221164;
        x2 = 2620098;
        break;
    case 1528 :
        x0 = 1218477;
        x2 = 2613942;
        break;
    case 1529 :
        x0 = 1215792;
        x2 = 2607793;
        break;
    case 1530 :
        x0 = 1213108;
        x2 = 2601648;
        break;
    case 1531 :
        x0 = 1210426;
        x2 = 2595509;
        break;
    case 1532 :
        x0 = 1207744;
        x2 = 2589372;
        break;
    case 1533 :
        x0 = 1205065;
        x2 = 2583244;
        break;
    case 1534 :
        x0 = 1202387;
        x2 = 2577119;
        break;
    case 1535 :
        x0 = 1199710;
        x2 = 2570998;
        break;
    case 1536 :
        x0 = 1197036;
        x2 = 2564886;
        break;
    case 1537 :
        x0 = 1194362;
        x2 = 2558776;
        break;
    case 1538 :
        x0 = 1191690;
        x2 = 2552672;
        break;
    case 1539 :
        x0 = 1189020;
        x2 = 2546574;
        break;
    case 1540 :
        x0 = 1186351;
        x2 = 2540481;
        break;
    case 1541 :
        x0 = 1183684;
        x2 = 2534393;
        break;
    case 1542 :
        x0 = 1181018;
        x2 = 2528310;
        break;
    case 1543 :
        x0 = 1178353;
        x2 = 2522230;
        break;
    case 1544 :
        x0 = 1175690;
        x2 = 2516157;
        break;
    case 1545 :
        x0 = 1173028;
        x2 = 2510087;
        break;
    case 1546 :
        x0 = 1170368;
        x2 = 2504024;
        break;
    case 1547 :
        x0 = 1167710;
        x2 = 2497967;
        break;
    case 1548 :
        x0 = 1165052;
        x2 = 2491912;
        break;
    case 1549 :
        x0 = 1162397;
        x2 = 2485866;
        break;
    case 1550 :
        x0 = 1159742;
        x2 = 2479821;
        break;
    case 1551 :
        x0 = 1157090;
        x2 = 2473784;
        break;
    case 1552 :
        x0 = 1154439;
        x2 = 2467752;
        break;
    case 1553 :
        x0 = 1151789;
        x2 = 2461723;
        break;
    case 1554 :
        x0 = 1149140;
        x2 = 2455699;
        break;
    case 1555 :
        x0 = 1146494;
        x2 = 2449682;
        break;
    case 1556 :
        x0 = 1143849;
        x2 = 2443670;
        break;
    case 1557 :
        x0 = 1141205;
        x2 = 2437662;
        break;
    case 1558 :
        x0 = 1138562;
        x2 = 2431658;
        break;
    case 1559 :
        x0 = 1135922;
        x2 = 2425662;
        break;
    case 1560 :
        x0 = 1133282;
        x2 = 2419668;
        break;
    case 1561 :
        x0 = 1130644;
        x2 = 2413680;
        break;
    case 1562 :
        x0 = 1128008;
        x2 = 2407698;
        break;
    case 1563 :
        x0 = 1125372;
        x2 = 2401718;
        break;
    case 1564 :
        x0 = 1122739;
        x2 = 2395746;
        break;
    case 1565 :
        x0 = 1120106;
        x2 = 2389776;
        break;
    case 1566 :
        x0 = 1117476;
        x2 = 2383815;
        break;
    case 1567 :
        x0 = 1114847;
        x2 = 2377857;
        break;
    case 1568 :
        x0 = 1112219;
        x2 = 2371904;
        break;
    case 1569 :
        x0 = 1109592;
        x2 = 2365954;
        break;
    case 1570 :
        x0 = 1106968;
        x2 = 2360012;
        break;
    case 1571 :
        x0 = 1104344;
        x2 = 2354073;
        break;
    case 1572 :
        x0 = 1101722;
        x2 = 2348139;
        break;
    case 1573 :
        x0 = 1099102;
        x2 = 2342212;
        break;
    case 1574 :
        x0 = 1096482;
        x2 = 2336286;
        break;
    case 1575 :
        x0 = 1093865;
        x2 = 2330369;
        break;
    case 1576 :
        x0 = 1091248;
        x2 = 2324453;
        break;
    case 1577 :
        x0 = 1088634;
        x2 = 2318546;
        break;
    case 1578 :
        x0 = 1086020;
        x2 = 2312640;
        break;
    case 1579 :
        x0 = 1083408;
        x2 = 2306741;
        break;
    case 1580 :
        x0 = 1080798;
        x2 = 2300847;
        break;
    case 1581 :
        x0 = 1078189;
        x2 = 2294958;
        break;
    case 1582 :
        x0 = 1075582;
        x2 = 2289074;
        break;
    case 1583 :
        x0 = 1072976;
        x2 = 2283195;
        break;
    case 1584 :
        x0 = 1070370;
        x2 = 2277317;
        break;
    case 1585 :
        x0 = 1067767;
        x2 = 2271448;
        break;
    case 1586 :
        x0 = 1065166;
        x2 = 2265584;
        break;
    case 1587 :
        x0 = 1062566;
        x2 = 2259725;
        break;
    case 1588 :
        x0 = 1059966;
        x2 = 2253867;
        break;
    case 1589 :
        x0 = 1057369;
        x2 = 2248017;
        break;
    case 1590 :
        x0 = 1054772;
        x2 = 2242170;
        break;
    case 1591 :
        x0 = 1052178;
        x2 = 2236330;
        break;
    case 1592 :
        x0 = 1049585;
        x2 = 2230494;
        break;
    case 1593 :
        x0 = 1046993;
        x2 = 2224663;
        break;
    case 1594 :
        x0 = 1044402;
        x2 = 2218835;
        break;
    case 1595 :
        x0 = 1041814;
        x2 = 2213015;
        break;
    case 1596 :
        x0 = 1039226;
        x2 = 2207197;
        break;
    case 1597 :
        x0 = 1036640;
        x2 = 2201385;
        break;
    case 1598 :
        x0 = 1034055;
        x2 = 2195577;
        break;
    case 1599 :
        x0 = 1031472;
        x2 = 2189775;
        break;
    case 1600 :
        x0 = 1028890;
        x2 = 2183977;
        break;
    case 1601 :
        x0 = 1026310;
        x2 = 2178185;
        break;
    case 1602 :
        x0 = 1023730;
        x2 = 2172394;
        break;
    case 1603 :
        x0 = 1021152;
        x2 = 2166610;
        break;
    case 1604 :
        x0 = 1018576;
        x2 = 2160831;
        break;
    case 1605 :
        x0 = 1016002;
        x2 = 2155059;
        break;
    case 1606 :
        x0 = 1013428;
        x2 = 2149288;
        break;
    case 1607 :
        x0 = 1010856;
        x2 = 2143524;
        break;
    case 1608 :
        x0 = 1008286;
        x2 = 2137765;
        break;
    case 1609 :
        x0 = 1005716;
        x2 = 2132008;
        break;
    case 1610 :
        x0 = 1003148;
        x2 = 2126257;
        break;
    case 1611 :
        x0 = 1000582;
        x2 = 2120512;
        break;
    case 1612 :
        x0 = 998017;
        x2 = 2114771;
        break;
    case 1613 :
        x0 = 995453;
        x2 = 2109034;
        break;
    case 1614 :
        x0 = 992891;
        x2 = 2103302;
        break;
    case 1615 :
        x0 = 990330;
        x2 = 2097575;
        break;
    case 1616 :
        x0 = 987771;
        x2 = 2091853;
        break;
    case 1617 :
        x0 = 985212;
        x2 = 2086134;
        break;
    case 1618 :
        x0 = 982656;
        x2 = 2080422;
        break;
    case 1619 :
        x0 = 980101;
        x2 = 2074714;
        break;
    case 1620 :
        x0 = 977547;
        x2 = 2069010;
        break;
    case 1621 :
        x0 = 974994;
        x2 = 2063310;
        break;
    case 1622 :
        x0 = 972444;
        x2 = 2057618;
        break;
    case 1623 :
        x0 = 969894;
        x2 = 2051928;
        break;
    case 1624 :
        x0 = 967346;
        x2 = 2046243;
        break;
    case 1625 :
        x0 = 964799;
        x2 = 2040562;
        break;
    case 1626 :
        x0 = 962253;
        x2 = 2034886;
        break;
    case 1627 :
        x0 = 959710;
        x2 = 2029217;
        break;
    case 1628 :
        x0 = 957166;
        x2 = 2023548;
        break;
    case 1629 :
        x0 = 954625;
        x2 = 2017886;
        break;
    case 1630 :
        x0 = 952086;
        x2 = 2012231;
        break;
    case 1631 :
        x0 = 949546;
        x2 = 2006576;
        break;
    case 1632 :
        x0 = 947010;
        x2 = 2000930;
        break;
    case 1633 :
        x0 = 944474;
        x2 = 1995286;
        break;
    case 1634 :
        x0 = 941940;
        x2 = 1989649;
        break;
    case 1635 :
        x0 = 939406;
        x2 = 1984012;
        break;
    case 1636 :
        x0 = 936874;
        x2 = 1978382;
        break;
    case 1637 :
        x0 = 934344;
        x2 = 1972758;
        break;
    case 1638 :
        x0 = 931815;
        x2 = 1967137;
        break;
    case 1639 :
        x0 = 929288;
        x2 = 1961522;
        break;
    case 1640 :
        x0 = 926762;
        x2 = 1955911;
        break;
    case 1641 :
        x0 = 924236;
        x2 = 1950302;
        break;
    case 1642 :
        x0 = 921713;
        x2 = 1944701;
        break;
    case 1643 :
        x0 = 919191;
        x2 = 1939103;
        break;
    case 1644 :
        x0 = 916670;
        x2 = 1933510;
        break;
    case 1645 :
        x0 = 914151;
        x2 = 1927922;
        break;
    case 1646 :
        x0 = 911633;
        x2 = 1922338;
        break;
    case 1647 :
        x0 = 909116;
        x2 = 1916758;
        break;
    case 1648 :
        x0 = 906601;
        x2 = 1911183;
        break;
    case 1649 :
        x0 = 904087;
        x2 = 1905612;
        break;
    case 1650 :
        x0 = 901574;
        x2 = 1900046;
        break;
    case 1651 :
        x0 = 899063;
        x2 = 1894485;
        break;
    case 1652 :
        x0 = 896554;
        x2 = 1888930;
        break;
    case 1653 :
        x0 = 894045;
        x2 = 1883376;
        break;
    case 1654 :
        x0 = 891538;
        x2 = 1877828;
        break;
    case 1655 :
        x0 = 889032;
        x2 = 1872284;
        break;
    case 1656 :
        x0 = 886528;
        x2 = 1866746;
        break;
    case 1657 :
        x0 = 884025;
        x2 = 1861212;
        break;
    case 1658 :
        x0 = 881523;
        x2 = 1855681;
        break;
    case 1659 :
        x0 = 879022;
        x2 = 1850155;
        break;
    case 1660 :
        x0 = 876524;
        x2 = 1844636;
        break;
    case 1661 :
        x0 = 874026;
        x2 = 1839119;
        break;
    case 1662 :
        x0 = 871530;
        x2 = 1833607;
        break;
    case 1663 :
        x0 = 869034;
        x2 = 1828097;
        break;
    case 1664 :
        x0 = 866541;
        x2 = 1822595;
        break;
    case 1665 :
        x0 = 864048;
        x2 = 1817095;
        break;
    case 1666 :
        x0 = 861558;
        x2 = 1811603;
        break;
    case 1667 :
        x0 = 859068;
        x2 = 1806112;
        break;
    case 1668 :
        x0 = 856580;
        x2 = 1800627;
        break;
    case 1669 :
        x0 = 854093;
        x2 = 1795146;
        break;
    case 1670 :
        x0 = 851607;
        x2 = 1789669;
        break;
    case 1671 :
        x0 = 849122;
        x2 = 1784195;
        break;
    case 1672 :
        x0 = 846640;
        x2 = 1778729;
        break;
    case 1673 :
        x0 = 844158;
        x2 = 1773265;
        break;
    case 1674 :
        x0 = 841678;
        x2 = 1767806;
        break;
    case 1675 :
        x0 = 839199;
        x2 = 1762352;
        break;
    case 1676 :
        x0 = 836722;
        x2 = 1756903;
        break;
    case 1677 :
        x0 = 834246;
        x2 = 1751458;
        break;
    case 1678 :
        x0 = 831770;
        x2 = 1746014;
        break;
    case 1679 :
        x0 = 829297;
        x2 = 1740578;
        break;
    case 1680 :
        x0 = 826824;
        x2 = 1735144;
        break;
    case 1681 :
        x0 = 824354;
        x2 = 1729718;
        break;
    case 1682 :
        x0 = 821884;
        x2 = 1724293;
        break;
    case 1683 :
        x0 = 819416;
        x2 = 1718874;
        break;
    case 1684 :
        x0 = 816948;
        x2 = 1713457;
        break;
    case 1685 :
        x0 = 814483;
        x2 = 1708047;
        break;
    case 1686 :
        x0 = 812018;
        x2 = 1702639;
        break;
    case 1687 :
        x0 = 809556;
        x2 = 1697239;
        break;
    case 1688 :
        x0 = 807094;
        x2 = 1691841;
        break;
    case 1689 :
        x0 = 804634;
        x2 = 1686448;
        break;
    case 1690 :
        x0 = 802174;
        x2 = 1681057;
        break;
    case 1691 :
        x0 = 799716;
        x2 = 1675672;
        break;
    case 1692 :
        x0 = 797260;
        x2 = 1670292;
        break;
    case 1693 :
        x0 = 794805;
        x2 = 1664916;
        break;
    case 1694 :
        x0 = 792352;
        x2 = 1659546;
        break;
    case 1695 :
        x0 = 789898;
        x2 = 1654175;
        break;
    case 1696 :
        x0 = 787448;
        x2 = 1648815;
        break;
    case 1697 :
        x0 = 784998;
        x2 = 1643455;
        break;
    case 1698 :
        x0 = 782549;
        x2 = 1638100;
        break;
    case 1699 :
        x0 = 780102;
        x2 = 1632750;
        break;
    case 1700 :
        x0 = 777656;
        x2 = 1627404;
        break;
    case 1701 :
        x0 = 775211;
        x2 = 1622061;
        break;
    case 1702 :
        x0 = 772768;
        x2 = 1616724;
        break;
    case 1703 :
        x0 = 770326;
        x2 = 1611391;
        break;
    case 1704 :
        x0 = 767885;
        x2 = 1606061;
        break;
    case 1705 :
        x0 = 765446;
        x2 = 1600738;
        break;
    case 1706 :
        x0 = 763008;
        x2 = 1595417;
        break;
    case 1707 :
        x0 = 760570;
        x2 = 1590099;
        break;
    case 1708 :
        x0 = 758135;
        x2 = 1584788;
        break;
    case 1709 :
        x0 = 755700;
        x2 = 1579478;
        break;
    case 1710 :
        x0 = 753268;
        x2 = 1574177;
        break;
    case 1711 :
        x0 = 750836;
        x2 = 1568877;
        break;
    case 1712 :
        x0 = 748406;
        x2 = 1563582;
        break;
    case 1713 :
        x0 = 745977;
        x2 = 1558292;
        break;
    case 1714 :
        x0 = 743549;
        x2 = 1553005;
        break;
    case 1715 :
        x0 = 741122;
        x2 = 1547721;
        break;
    case 1716 :
        x0 = 738697;
        x2 = 1542443;
        break;
    case 1717 :
        x0 = 736274;
        x2 = 1537171;
        break;
    case 1718 :
        x0 = 733850;
        x2 = 1531898;
        break;
    case 1719 :
        x0 = 731429;
        x2 = 1526634;
        break;
    case 1720 :
        x0 = 729010;
        x2 = 1521374;
        break;
    case 1721 :
        x0 = 726590;
        x2 = 1516115;
        break;
    case 1722 :
        x0 = 724173;
        x2 = 1510863;
        break;
    case 1723 :
        x0 = 721756;
        x2 = 1505612;
        break;
    case 1724 :
        x0 = 719342;
        x2 = 1500369;
        break;
    case 1725 :
        x0 = 716928;
        x2 = 1495128;
        break;
    case 1726 :
        x0 = 714516;
        x2 = 1489892;
        break;
    case 1727 :
        x0 = 712104;
        x2 = 1484658;
        break;
    case 1728 :
        x0 = 709694;
        x2 = 1479430;
        break;
    case 1729 :
        x0 = 707286;
        x2 = 1474207;
        break;
    case 1730 :
        x0 = 704879;
        x2 = 1468988;
        break;
    case 1731 :
        x0 = 702472;
        x2 = 1463770;
        break;
    case 1732 :
        x0 = 700068;
        x2 = 1458560;
        break;
    case 1733 :
        x0 = 697665;
        x2 = 1453354;
        break;
    case 1734 :
        x0 = 695262;
        x2 = 1448148;
        break;
    case 1735 :
        x0 = 692862;
        x2 = 1442951;
        break;
    case 1736 :
        x0 = 690462;
        x2 = 1437756;
        break;
    case 1737 :
        x0 = 688064;
        x2 = 1432566;
        break;
    case 1738 :
        x0 = 685666;
        x2 = 1427377;
        break;
    case 1739 :
        x0 = 683270;
        x2 = 1422194;
        break;
    case 1740 :
        x0 = 680876;
        x2 = 1417016;
        break;
    case 1741 :
        x0 = 678483;
        x2 = 1411843;
        break;
    case 1742 :
        x0 = 676090;
        x2 = 1406670;
        break;
    case 1743 :
        x0 = 673700;
        x2 = 1401506;
        break;
    case 1744 :
        x0 = 671310;
        x2 = 1396343;
        break;
    case 1745 :
        x0 = 668922;
        x2 = 1391185;
        break;
    case 1746 :
        x0 = 666535;
        x2 = 1386031;
        break;
    case 1747 :
        x0 = 664150;
        x2 = 1380883;
        break;
    case 1748 :
        x0 = 661764;
        x2 = 1375734;
        break;
    case 1749 :
        x0 = 659382;
        x2 = 1370594;
        break;
    case 1750 :
        x0 = 657000;
        x2 = 1365457;
        break;
    case 1751 :
        x0 = 654619;
        x2 = 1360322;
        break;
    case 1752 :
        x0 = 652239;
        x2 = 1355192;
        break;
    case 1753 :
        x0 = 649862;
        x2 = 1350069;
        break;
    case 1754 :
        x0 = 647484;
        x2 = 1344945;
        break;
    case 1755 :
        x0 = 645108;
        x2 = 1339827;
        break;
    case 1756 :
        x0 = 642734;
        x2 = 1334714;
        break;
    case 1757 :
        x0 = 640361;
        x2 = 1329605;
        break;
    case 1758 :
        x0 = 637988;
        x2 = 1324498;
        break;
    case 1759 :
        x0 = 635618;
        x2 = 1319398;
        break;
    case 1760 :
        x0 = 633248;
        x2 = 1314299;
        break;
    case 1761 :
        x0 = 630880;
        x2 = 1309206;
        break;
    case 1762 :
        x0 = 628513;
        x2 = 1304117;
        break;
    case 1763 :
        x0 = 626148;
        x2 = 1299033;
        break;
    case 1764 :
        x0 = 623782;
        x2 = 1293949;
        break;
    case 1765 :
        x0 = 621420;
        x2 = 1288874;
        break;
    case 1766 :
        x0 = 619058;
        x2 = 1283801;
        break;
    case 1767 :
        x0 = 616697;
        x2 = 1278731;
        break;
    case 1768 :
        x0 = 614337;
        x2 = 1273665;
        break;
    case 1769 :
        x0 = 611978;
        x2 = 1268602;
        break;
    case 1770 :
        x0 = 609622;
        x2 = 1263547;
        break;
    case 1771 :
        x0 = 607266;
        x2 = 1258493;
        break;
    case 1772 :
        x0 = 604911;
        x2 = 1253443;
        break;
    case 1773 :
        x0 = 602558;
        x2 = 1248398;
        break;
    case 1774 :
        x0 = 600206;
        x2 = 1243357;
        break;
    case 1775 :
        x0 = 597855;
        x2 = 1238319;
        break;
    case 1776 :
        x0 = 595505;
        x2 = 1233285;
        break;
    case 1777 :
        x0 = 593156;
        x2 = 1228254;
        break;
    case 1778 :
        x0 = 590809;
        x2 = 1223229;
        break;
    case 1779 :
        x0 = 588464;
        x2 = 1218209;
        break;
    case 1780 :
        x0 = 586118;
        x2 = 1213188;
        break;
    case 1781 :
        x0 = 583775;
        x2 = 1208176;
        break;
    case 1782 :
        x0 = 581432;
        x2 = 1203164;
        break;
    case 1783 :
        x0 = 579091;
        x2 = 1198158;
        break;
    case 1784 :
        x0 = 576752;
        x2 = 1193158;
        break;
    case 1785 :
        x0 = 574413;
        x2 = 1188159;
        break;
    case 1786 :
        x0 = 572075;
        x2 = 1183164;
        break;
    case 1787 :
        x0 = 569739;
        x2 = 1178174;
        break;
    case 1788 :
        x0 = 567404;
        x2 = 1173187;
        break;
    case 1789 :
        x0 = 565070;
        x2 = 1168204;
        break;
    case 1790 :
        x0 = 562738;
        x2 = 1163226;
        break;
    case 1791 :
        x0 = 560406;
        x2 = 1158250;
        break;
    case 1792 :
        x0 = 558076;
        x2 = 1153280;
        break;
    case 1793 :
        x0 = 555747;
        x2 = 1148312;
        break;
    case 1794 :
        x0 = 553420;
        x2 = 1143351;
        break;
    case 1795 :
        x0 = 551092;
        x2 = 1138388;
        break;
    case 1796 :
        x0 = 548768;
        x2 = 1133435;
        break;
    case 1797 :
        x0 = 546444;
        x2 = 1128484;
        break;
    case 1798 :
        x0 = 544120;
        x2 = 1123534;
        break;
    case 1799 :
        x0 = 541799;
        x2 = 1118591;
        break;
    case 1800 :
        x0 = 539478;
        x2 = 1113650;
        break;
    case 1801 :
        x0 = 537159;
        x2 = 1108715;
        break;
    case 1802 :
        x0 = 534840;
        x2 = 1103780;
        break;
    case 1803 :
        x0 = 532524;
        x2 = 1098854;
        break;
    case 1804 :
        x0 = 530208;
        x2 = 1093928;
        break;
    case 1805 :
        x0 = 527894;
        x2 = 1089008;
        break;
    case 1806 :
        x0 = 525581;
        x2 = 1084092;
        break;
    case 1807 :
        x0 = 523268;
        x2 = 1079177;
        break;
    case 1808 :
        x0 = 520958;
        x2 = 1074269;
        break;
    case 1809 :
        x0 = 518648;
        x2 = 1069363;
        break;
    case 1810 :
        x0 = 516340;
        x2 = 1064462;
        break;
    case 1811 :
        x0 = 514032;
        x2 = 1059563;
        break;
    case 1812 :
        x0 = 511726;
        x2 = 1054669;
        break;
    case 1813 :
        x0 = 509421;
        x2 = 1049778;
        break;
    case 1814 :
        x0 = 507118;
        x2 = 1044893;
        break;
    case 1815 :
        x0 = 504815;
        x2 = 1040009;
        break;
    case 1816 :
        x0 = 502514;
        x2 = 1035131;
        break;
    case 1817 :
        x0 = 500214;
        x2 = 1030256;
        break;
    case 1818 :
        x0 = 497915;
        x2 = 1025384;
        break;
    case 1819 :
        x0 = 495617;
        x2 = 1020516;
        break;
    case 1820 :
        x0 = 493320;
        x2 = 1015651;
        break;
    case 1821 :
        x0 = 491025;
        x2 = 1010792;
        break;
    case 1822 :
        x0 = 488730;
        x2 = 1005934;
        break;
    case 1823 :
        x0 = 486438;
        x2 = 1001084;
        break;
    case 1824 :
        x0 = 484146;
        x2 = 996234;
        break;
    case 1825 :
        x0 = 481856;
        x2 = 991391;
        break;
    case 1826 :
        x0 = 479566;
        x2 = 986548;
        break;
    case 1827 :
        x0 = 477278;
        x2 = 981711;
        break;
    case 1828 :
        x0 = 474991;
        x2 = 976878;
        break;
    case 1829 :
        x0 = 472704;
        x2 = 972045;
        break;
    case 1830 :
        x0 = 470420;
        x2 = 967220;
        break;
    case 1831 :
        x0 = 468136;
        x2 = 962397;
        break;
    case 1832 :
        x0 = 465854;
        x2 = 957579;
        break;
    case 1833 :
        x0 = 463573;
        x2 = 952764;
        break;
    case 1834 :
        x0 = 461292;
        x2 = 947951;
        break;
    case 1835 :
        x0 = 459014;
        x2 = 943145;
        break;
    case 1836 :
        x0 = 456736;
        x2 = 938340;
        break;
    case 1837 :
        x0 = 454460;
        x2 = 933541;
        break;
    case 1838 :
        x0 = 452184;
        x2 = 928743;
        break;
    case 1839 :
        x0 = 449910;
        x2 = 923950;
        break;
    case 1840 :
        x0 = 447637;
        x2 = 919161;
        break;
    case 1841 :
        x0 = 445366;
        x2 = 914377;
        break;
    case 1842 :
        x0 = 443095;
        x2 = 909595;
        break;
    case 1843 :
        x0 = 440826;
        x2 = 904818;
        break;
    case 1844 :
        x0 = 438557;
        x2 = 900042;
        break;
    case 1845 :
        x0 = 436290;
        x2 = 895271;
        break;
    case 1846 :
        x0 = 434024;
        x2 = 890504;
        break;
    case 1847 :
        x0 = 431759;
        x2 = 885740;
        break;
    case 1848 :
        x0 = 429496;
        x2 = 880982;
        break;
    case 1849 :
        x0 = 427233;
        x2 = 876225;
        break;
    case 1850 :
        x0 = 424972;
        x2 = 871473;
        break;
    case 1851 :
        x0 = 422712;
        x2 = 866725;
        break;
    case 1852 :
        x0 = 420452;
        x2 = 861978;
        break;
    case 1853 :
        x0 = 418194;
        x2 = 857236;
        break;
    case 1854 :
        x0 = 415938;
        x2 = 852500;
        break;
    case 1855 :
        x0 = 413682;
        x2 = 847765;
        break;
    case 1856 :
        x0 = 411428;
        x2 = 843035;
        break;
    case 1857 :
        x0 = 409175;
        x2 = 838309;
        break;
    case 1858 :
        x0 = 406922;
        x2 = 833583;
        break;
    case 1859 :
        x0 = 404672;
        x2 = 828866;
        break;
    case 1860 :
        x0 = 402422;
        x2 = 824149;
        break;
    case 1861 :
        x0 = 400173;
        x2 = 819436;
        break;
    case 1862 :
        x0 = 397926;
        x2 = 814728;
        break;
    case 1863 :
        x0 = 395680;
        x2 = 810024;
        break;
    case 1864 :
        x0 = 393434;
        x2 = 805320;
        break;
    case 1865 :
        x0 = 391190;
        x2 = 800623;
        break;
    case 1866 :
        x0 = 388947;
        x2 = 795928;
        break;
    case 1867 :
        x0 = 386706;
        x2 = 791239;
        break;
    case 1868 :
        x0 = 384465;
        x2 = 786551;
        break;
    case 1869 :
        x0 = 382226;
        x2 = 781868;
        break;
    case 1870 :
        x0 = 379988;
        x2 = 777189;
        break;
    case 1871 :
        x0 = 377750;
        x2 = 772511;
        break;
    case 1872 :
        x0 = 375514;
        x2 = 767838;
        break;
    case 1873 :
        x0 = 373279;
        x2 = 763168;
        break;
    case 1874 :
        x0 = 371046;
        x2 = 758504;
        break;
    case 1875 :
        x0 = 368812;
        x2 = 753839;
        break;
    case 1876 :
        x0 = 366581;
        x2 = 749182;
        break;
    case 1877 :
        x0 = 364350;
        x2 = 744525;
        break;
    case 1878 :
        x0 = 362122;
        x2 = 739876;
        break;
    case 1879 :
        x0 = 359894;
        x2 = 735228;
        break;
    case 1880 :
        x0 = 357667;
        x2 = 730584;
        break;
    case 1881 :
        x0 = 355441;
        x2 = 725943;
        break;
    case 1882 :
        x0 = 353216;
        x2 = 721305;
        break;
    case 1883 :
        x0 = 350992;
        x2 = 716670;
        break;
    case 1884 :
        x0 = 348770;
        x2 = 712041;
        break;
    case 1885 :
        x0 = 346549;
        x2 = 707415;
        break;
    case 1886 :
        x0 = 344328;
        x2 = 702790;
        break;
    case 1887 :
        x0 = 342110;
        x2 = 698172;
        break;
    case 1888 :
        x0 = 339892;
        x2 = 693556;
        break;
    case 1889 :
        x0 = 337675;
        x2 = 688943;
        break;
    case 1890 :
        x0 = 335460;
        x2 = 684335;
        break;
    case 1891 :
        x0 = 333245;
        x2 = 679728;
        break;
    case 1892 :
        x0 = 331032;
        x2 = 675127;
        break;
    case 1893 :
        x0 = 328820;
        x2 = 670529;
        break;
    case 1894 :
        x0 = 326608;
        x2 = 665932;
        break;
    case 1895 :
        x0 = 324398;
        x2 = 661341;
        break;
    case 1896 :
        x0 = 322189;
        x2 = 656753;
        break;
    case 1897 :
        x0 = 319982;
        x2 = 652170;
        break;
    case 1898 :
        x0 = 317775;
        x2 = 647588;
        break;
    case 1899 :
        x0 = 315570;
        x2 = 643011;
        break;
    case 1900 :
        x0 = 313365;
        x2 = 638436;
        break;
    case 1901 :
        x0 = 311162;
        x2 = 633866;
        break;
    case 1902 :
        x0 = 308960;
        x2 = 629299;
        break;
    case 1903 :
        x0 = 306758;
        x2 = 624734;
        break;
    case 1904 :
        x0 = 304558;
        x2 = 620173;
        break;
    case 1905 :
        x0 = 302360;
        x2 = 615618;
        break;
    case 1906 :
        x0 = 300162;
        x2 = 611064;
        break;
    case 1907 :
        x0 = 297965;
        x2 = 606514;
        break;
    case 1908 :
        x0 = 295770;
        x2 = 601968;
        break;
    case 1909 :
        x0 = 293576;
        x2 = 597426;
        break;
    case 1910 :
        x0 = 291382;
        x2 = 592885;
        break;
    case 1911 :
        x0 = 289190;
        x2 = 588350;
        break;
    case 1912 :
        x0 = 286999;
        x2 = 583817;
        break;
    case 1913 :
        x0 = 284808;
        x2 = 579286;
        break;
    case 1914 :
        x0 = 282620;
        x2 = 574762;
        break;
    case 1915 :
        x0 = 280432;
        x2 = 570239;
        break;
    case 1916 :
        x0 = 278246;
        x2 = 565721;
        break;
    case 1917 :
        x0 = 276060;
        x2 = 561205;
        break;
    case 1918 :
        x0 = 273876;
        x2 = 556694;
        break;
    case 1919 :
        x0 = 271692;
        x2 = 552184;
        break;
    case 1920 :
        x0 = 269510;
        x2 = 547679;
        break;
    case 1921 :
        x0 = 267328;
        x2 = 543175;
        break;
    case 1922 :
        x0 = 265148;
        x2 = 538677;
        break;
    case 1923 :
        x0 = 262970;
        x2 = 534184;
        break;
    case 1924 :
        x0 = 260792;
        x2 = 529692;
        break;
    case 1925 :
        x0 = 258615;
        x2 = 525203;
        break;
    case 1926 :
        x0 = 256439;
        x2 = 520717;
        break;
    case 1927 :
        x0 = 254264;
        x2 = 516235;
        break;
    case 1928 :
        x0 = 252091;
        x2 = 511758;
        break;
    case 1929 :
        x0 = 249918;
        x2 = 507282;
        break;
    case 1930 :
        x0 = 247748;
        x2 = 502813;
        break;
    case 1931 :
        x0 = 245578;
        x2 = 498345;
        break;
    case 1932 :
        x0 = 243408;
        x2 = 493879;
        break;
    case 1933 :
        x0 = 241240;
        x2 = 489418;
        break;
    case 1934 :
        x0 = 239073;
        x2 = 484960;
        break;
    case 1935 :
        x0 = 236908;
        x2 = 480507;
        break;
    case 1936 :
        x0 = 234742;
        x2 = 476053;
        break;
    case 1937 :
        x0 = 232578;
        x2 = 471604;
        break;
    case 1938 :
        x0 = 230416;
        x2 = 467161;
        break;
    case 1939 :
        x0 = 228254;
        x2 = 462719;
        break;
    case 1940 :
        x0 = 226094;
        x2 = 458282;
        break;
    case 1941 :
        x0 = 223935;
        x2 = 453848;
        break;
    case 1942 :
        x0 = 221776;
        x2 = 449415;
        break;
    case 1943 :
        x0 = 219620;
        x2 = 444990;
        break;
    case 1944 :
        x0 = 217464;
        x2 = 440565;
        break;
    case 1945 :
        x0 = 215308;
        x2 = 436142;
        break;
    case 1946 :
        x0 = 213154;
        x2 = 431724;
        break;
    case 1947 :
        x0 = 211002;
        x2 = 427311;
        break;
    case 1948 :
        x0 = 208850;
        x2 = 422900;
        break;
    case 1949 :
        x0 = 206699;
        x2 = 418491;
        break;
    case 1950 :
        x0 = 204550;
        x2 = 414088;
        break;
    case 1951 :
        x0 = 202401;
        x2 = 409686;
        break;
    case 1952 :
        x0 = 200254;
        x2 = 405288;
        break;
    case 1953 :
        x0 = 198107;
        x2 = 400893;
        break;
    case 1954 :
        x0 = 195962;
        x2 = 396502;
        break;
    case 1955 :
        x0 = 193818;
        x2 = 392114;
        break;
    case 1956 :
        x0 = 191674;
        x2 = 387728;
        break;
    case 1957 :
        x0 = 189532;
        x2 = 383346;
        break;
    case 1958 :
        x0 = 187391;
        x2 = 378968;
        break;
    case 1959 :
        x0 = 185251;
        x2 = 374593;
        break;
    case 1960 :
        x0 = 183112;
        x2 = 370221;
        break;
    case 1961 :
        x0 = 180974;
        x2 = 365852;
        break;
    case 1962 :
        x0 = 178838;
        x2 = 361489;
        break;
    case 1963 :
        x0 = 176702;
        x2 = 357126;
        break;
    case 1964 :
        x0 = 174567;
        x2 = 352767;
        break;
    case 1965 :
        x0 = 172434;
        x2 = 348413;
        break;
    case 1966 :
        x0 = 170301;
        x2 = 344059;
        break;
    case 1967 :
        x0 = 168170;
        x2 = 339711;
        break;
    case 1968 :
        x0 = 166039;
        x2 = 335364;
        break;
    case 1969 :
        x0 = 163910;
        x2 = 331023;
        break;
    case 1970 :
        x0 = 161782;
        x2 = 326684;
        break;
    case 1971 :
        x0 = 159654;
        x2 = 322347;
        break;
    case 1972 :
        x0 = 157528;
        x2 = 318014;
        break;
    case 1973 :
        x0 = 155403;
        x2 = 313685;
        break;
    case 1974 :
        x0 = 153278;
        x2 = 309357;
        break;
    case 1975 :
        x0 = 151156;
        x2 = 305036;
        break;
    case 1976 :
        x0 = 149034;
        x2 = 300716;
        break;
    case 1977 :
        x0 = 146913;
        x2 = 296399;
        break;
    case 1978 :
        x0 = 144794;
        x2 = 292087;
        break;
    case 1979 :
        x0 = 142674;
        x2 = 287775;
        break;
    case 1980 :
        x0 = 140556;
        x2 = 283467;
        break;
    case 1981 :
        x0 = 138440;
        x2 = 279165;
        break;
    case 1982 :
        x0 = 136324;
        x2 = 274863;
        break;
    case 1983 :
        x0 = 134210;
        x2 = 270567;
        break;
    case 1984 :
        x0 = 132096;
        x2 = 266272;
        break;
    case 1985 :
        x0 = 129984;
        x2 = 261982;
        break;
    case 1986 :
        x0 = 127872;
        x2 = 257693;
        break;
    case 1987 :
        x0 = 125762;
        x2 = 253409;
        break;
    case 1988 :
        x0 = 123652;
        x2 = 249127;
        break;
    case 1989 :
        x0 = 121544;
        x2 = 244849;
        break;
    case 1990 :
        x0 = 119437;
        x2 = 240575;
        break;
    case 1991 :
        x0 = 117330;
        x2 = 236301;
        break;
    case 1992 :
        x0 = 115226;
        x2 = 232035;
        break;
    case 1993 :
        x0 = 113122;
        x2 = 227769;
        break;
    case 1994 :
        x0 = 111018;
        x2 = 223505;
        break;
    case 1995 :
        x0 = 108916;
        x2 = 219246;
        break;
    case 1996 :
        x0 = 106815;
        x2 = 214990;
        break;
    case 1997 :
        x0 = 104716;
        x2 = 210739;
        break;
    case 1998 :
        x0 = 102616;
        x2 = 206487;
        break;
    case 1999 :
        x0 = 100518;
        x2 = 202240;
        break;
    case 2000 :
        x0 = 98422;
        x2 = 197999;
        break;
    case 2001 :
        x0 = 96326;
        x2 = 193758;
        break;
    case 2002 :
        x0 = 94231;
        x2 = 189521;
        break;
    case 2003 :
        x0 = 92138;
        x2 = 185288;
        break;
    case 2004 :
        x0 = 90045;
        x2 = 181057;
        break;
    case 2005 :
        x0 = 87953;
        x2 = 176828;
        break;
    case 2006 :
        x0 = 85862;
        x2 = 172603;
        break;
    case 2007 :
        x0 = 83772;
        x2 = 168381;
        break;
    case 2008 :
        x0 = 81684;
        x2 = 164163;
        break;
    case 2009 :
        x0 = 79596;
        x2 = 159947;
        break;
    case 2010 :
        x0 = 77510;
        x2 = 155736;
        break;
    case 2011 :
        x0 = 75424;
        x2 = 151526;
        break;
    case 2012 :
        x0 = 73340;
        x2 = 147321;
        break;
    case 2013 :
        x0 = 71256;
        x2 = 143117;
        break;
    case 2014 :
        x0 = 69174;
        x2 = 138918;
        break;
    case 2015 :
        x0 = 67093;
        x2 = 134723;
        break;
    case 2016 :
        x0 = 65012;
        x2 = 130528;
        break;
    case 2017 :
        x0 = 62933;
        x2 = 126338;
        break;
    case 2018 :
        x0 = 60854;
        x2 = 122149;
        break;
    case 2019 :
        x0 = 58777;
        x2 = 117966;
        break;
    case 2020 :
        x0 = 56701;
        x2 = 113785;
        break;
    case 2021 :
        x0 = 54626;
        x2 = 109608;
        break;
    case 2022 :
        x0 = 52552;
        x2 = 105433;
        break;
    case 2023 :
        x0 = 50478;
        x2 = 101260;
        break;
    case 2024 :
        x0 = 48406;
        x2 = 97091;
        break;
    case 2025 :
        x0 = 46335;
        x2 = 92926;
        break;
    case 2026 :
        x0 = 44264;
        x2 = 88762;
        break;
    case 2027 :
        x0 = 42196;
        x2 = 84604;
        break;
    case 2028 :
        x0 = 40128;
        x2 = 80448;
        break;
    case 2029 :
        x0 = 38060;
        x2 = 76293;
        break;
    case 2030 :
        x0 = 35994;
        x2 = 72142;
        break;
    case 2031 :
        x0 = 33929;
        x2 = 67995;
        break;
    case 2032 :
        x0 = 31865;
        x2 = 63851;
        break;
    case 2033 :
        x0 = 29802;
        x2 = 59710;
        break;
    case 2034 :
        x0 = 27740;
        x2 = 55572;
        break;
    case 2035 :
        x0 = 25679;
        x2 = 51437;
        break;
    case 2036 :
        x0 = 23618;
        x2 = 47302;
        break;
    case 2037 :
        x0 = 21560;
        x2 = 43175;
        break;
    case 2038 :
        x0 = 19502;
        x2 = 39049;
        break;
    case 2039 :
        x0 = 17445;
        x2 = 34926;
        break;
    case 2040 :
        x0 = 15388;
        x2 = 30804;
        break;
    case 2041 :
        x0 = 13334;
        x2 = 26689;
        break;
    case 2042 :
        x0 = 11280;
        x2 = 22575;
        break;
    case 2043 :
        x0 = 9227;
        x2 = 18464;
        break;
    case 2044 :
        x0 = 7174;
        x2 = 14354;
        break;
    case 2045 :
        x0 = 5124;
        x2 = 10251;
        break;
    case 2046 :
        x0 = 3074;
        x2 = 6149;
        break;
    case 2047 :
        x0 = 1025;
        x2 = 2050;
        break;
  
  }

  unsigned long int ax2;
  unsigned int ax2_2;
  
  /*
  
  printf("m   is   ");
  print(m);
  printf("ma  is   ");
  print(ma);
  printf("1,x2 is  ");
  print((1<<23)|x2);
  
  */
  
  ax2 = ((unsigned long int)ma) * ((unsigned long int)((1<<23)|x2));
  ax2_2 = (lget(ax2,47,47) == 0) ? lget(ax2,46,23) : lget(ax2,47,24);
  
  /*
  
  printf("ax2 is \n");
  lprint(ax2);
  printf("ax2_2 is ");
  print(ax2_2);
  
  */
  
  unsigned int x0_2;
  
  x0_2 = ((1<<23)|x0) << 1;
  
  /*
  
  printf("x_2 is   ");
  print(x0_2);
  
  */
  
  unsigned int ans;
  
  ans = x0_2 - ((1<<23)|ax2_2);
  y = (e == 0 || e == 255) ? (s<<31) : ((e < 253) ? ((s<<31)|((253-e)<<23)|get(ans,22,0)) : (s<<31) );
  
  /*
  
  printf("ans is   ");
  print(ans);
  printf("y   is   ");
  print(y);
  
  */
  
  return y;

}

unsigned int fdiv(unsigned int x1,unsigned int x2){
	unsigned int y;
    unsigned int ovf;
    
  unsigned int s1;
  unsigned int e1;
  unsigned int m1;

   s1 = get(x1,31,31);
   e1 = get(x1,30,23);
   m1 = get(x1,22,0);

  unsigned int x2i;
  x2i = finv(x2);
  
  unsigned int s2i;
  unsigned int e2i;
  unsigned int m2i;

   s2i = get(x2i,31,31);
   e2i = get(x2i,30,23);
   m2i = get(x2i,22,0);
   
  unsigned int ans;

  ans = fmul(x1,x2i);
  y = ans;
  
  return y;
  
}

unsigned int fsqrt(unsigned int x){
    unsigned int y;
    
  unsigned int  s;
  unsigned int  e;
  unsigned int  m;

   s = get(x,31,31);
   e = get(x,30,23);
   m = get(x,22,0);
   
  unsigned int  ma,ea;

   ma = (e == 0) ? get(m,22,0) : (1<<23)|get(m,22,0);
   ea = ((e+1) >> 1) + 63 + (((get(e,0,0) == 0) && (m == 0b11111111111111111111111)) ? 1 : 0 );

  unsigned int nibunno3x;
  unsigned int nibunnox3;
  
  switch((1-(get(e,0,0)))<<11|get(m,22,12)){
    case 0 :
        nibunno3x = 4192770;
        nibunnox3 = 8382470;
        break;
    case 1 :
        nibunno3x = 4189699;
        nibunnox3 = 8370194;
        break;
    case 2 :
        nibunno3x = 4186632;
        nibunnox3 = 8357936;
        break;
    case 3 :
        nibunno3x = 4183566;
        nibunnox3 = 8345690;
        break;
    case 4 :
        nibunno3x = 4180504;
        nibunnox3 = 8333468;
        break;
    case 5 :
        nibunno3x = 4177442;
        nibunnox3 = 8321252;
        break;
    case 6 :
        nibunno3x = 4174384;
        nibunnox3 = 8309054;
        break;
    case 7 :
        nibunno3x = 4171327;
        nibunnox3 = 8296868;
        break;
    case 8 :
        nibunno3x = 4168274;
        nibunnox3 = 8284705;
        break;
    case 9 :
        nibunno3x = 4165222;
        nibunnox3 = 8272549;
        break;
    case 10 :
        nibunno3x = 4162172;
        nibunnox3 = 8260410;
        break;
    case 11 :
        nibunno3x = 4159124;
        nibunnox3 = 8248283;
        break;
    case 12 :
        nibunno3x = 4156080;
        nibunnox3 = 8236174;
        break;
    case 13 :
        nibunno3x = 4153036;
        nibunnox3 = 8224076;
        break;
    case 14 :
        nibunno3x = 4149996;
        nibunnox3 = 8212000;
        break;
    case 15 :
        nibunno3x = 4146956;
        nibunnox3 = 8199930;
        break;
    case 16 :
        nibunno3x = 4143920;
        nibunnox3 = 8187880;
        break;
    case 17 :
        nibunno3x = 4140887;
        nibunnox3 = 8175844;
        break;
    case 18 :
        nibunno3x = 4137854;
        nibunnox3 = 8163822;
        break;
    case 19 :
        nibunno3x = 4134825;
        nibunnox3 = 8151816;
        break;
    case 20 :
        nibunno3x = 4131798;
        nibunnox3 = 8139822;
        break;
    case 21 :
        nibunno3x = 4128773;
        nibunnox3 = 8127846;
        break;
    case 22 :
        nibunno3x = 4125749;
        nibunnox3 = 8115878;
        break;
    case 23 :
        nibunno3x = 4122728;
        nibunnox3 = 8103932;
        break;
    case 24 :
        nibunno3x = 4119709;
        nibunnox3 = 8091994;
        break;
    case 25 :
        nibunno3x = 4116692;
        nibunnox3 = 8080073;
        break;
    case 26 :
        nibunno3x = 4113679;
        nibunnox3 = 8068170;
        break;
    case 27 :
        nibunno3x = 4110666;
        nibunnox3 = 8056273;
        break;
    case 28 :
        nibunno3x = 4107656;
        nibunnox3 = 8044397;
        break;
    case 29 :
        nibunno3x = 4104648;
        nibunnox3 = 8032534;
        break;
    case 30 :
        nibunno3x = 4101642;
        nibunnox3 = 8020684;
        break;
    case 31 :
        nibunno3x = 4098638;
        nibunnox3 = 8008848;
        break;
    case 32 :
        nibunno3x = 4095638;
        nibunnox3 = 7997030;
        break;
    case 33 :
        nibunno3x = 4092638;
        nibunnox3 = 7985223;
        break;
    case 34 :
        nibunno3x = 4089642;
        nibunnox3 = 7973431;
        break;
    case 35 :
        nibunno3x = 4086647;
        nibunnox3 = 7961653;
        break;
    case 36 :
        nibunno3x = 4083654;
        nibunnox3 = 7949886;
        break;
    case 37 :
        nibunno3x = 4080662;
        nibunnox3 = 7938135;
        break;
    case 38 :
        nibunno3x = 4077674;
        nibunnox3 = 7926400;
        break;
    case 39 :
        nibunno3x = 4074689;
        nibunnox3 = 7914681;
        break;
    case 40 :
        nibunno3x = 4071704;
        nibunnox3 = 7902973;
        break;
    case 41 :
        nibunno3x = 4068722;
        nibunnox3 = 7891276;
        break;
    case 42 :
        nibunno3x = 4065742;
        nibunnox3 = 7879596;
        break;
    case 43 :
        nibunno3x = 4062764;
        nibunnox3 = 7867928;
        break;
    case 44 :
        nibunno3x = 4059788;
        nibunnox3 = 7856277;
        break;
    case 45 :
        nibunno3x = 4056816;
        nibunnox3 = 7844641;
        break;
    case 46 :
        nibunno3x = 4053843;
        nibunnox3 = 7833014;
        break;
    case 47 :
        nibunno3x = 4050875;
        nibunnox3 = 7821406;
        break;
    case 48 :
        nibunno3x = 4047908;
        nibunnox3 = 7809810;
        break;
    case 49 :
        nibunno3x = 4044943;
        nibunnox3 = 7798228;
        break;
    case 50 :
        nibunno3x = 4041980;
        nibunnox3 = 7786658;
        break;
    case 51 :
        nibunno3x = 4039020;
        nibunnox3 = 7775104;
        break;
    case 52 :
        nibunno3x = 4036062;
        nibunnox3 = 7763564;
        break;
    case 53 :
        nibunno3x = 4033104;
        nibunnox3 = 7752032;
        break;
    case 54 :
        nibunno3x = 4030150;
        nibunnox3 = 7740520;
        break;
    case 55 :
        nibunno3x = 4027197;
        nibunnox3 = 7729019;
        break;
    case 56 :
        nibunno3x = 4024248;
        nibunnox3 = 7717533;
        break;
    case 57 :
        nibunno3x = 4021299;
        nibunnox3 = 7706061;
        break;
    case 58 :
        nibunno3x = 4018352;
        nibunnox3 = 7694598;
        break;
    case 59 :
        nibunno3x = 4015410;
        nibunnox3 = 7683155;
        break;
    case 60 :
        nibunno3x = 4012467;
        nibunnox3 = 7671722;
        break;
    case 61 :
        nibunno3x = 4009528;
        nibunnox3 = 7660305;
        break;
    case 62 :
        nibunno3x = 4006591;
        nibunnox3 = 7648902;
        break;
    case 63 :
        nibunno3x = 4003655;
        nibunnox3 = 7637507;
        break;
    case 64 :
        nibunno3x = 4000722;
        nibunnox3 = 7626130;
        break;
    case 65 :
        nibunno3x = 3997790;
        nibunnox3 = 7614766;
        break;
    case 66 :
        nibunno3x = 3994861;
        nibunnox3 = 7603414;
        break;
    case 67 :
        nibunno3x = 3991934;
        nibunnox3 = 7592076;
        break;
    case 68 :
        nibunno3x = 3989008;
        nibunnox3 = 7580749;
        break;
    case 69 :
        nibunno3x = 3986086;
        nibunnox3 = 7569442;
        break;
    case 70 :
        nibunno3x = 3983165;
        nibunnox3 = 7558143;
        break;
    case 71 :
        nibunno3x = 3980246;
        nibunnox3 = 7546858;
        break;
    case 72 :
        nibunno3x = 3977329;
        nibunnox3 = 7535588;
        break;
    case 73 :
        nibunno3x = 3974414;
        nibunnox3 = 7524329;
        break;
    case 74 :
        nibunno3x = 3971502;
        nibunnox3 = 7513085;
        break;
    case 75 :
        nibunno3x = 3968590;
        nibunnox3 = 7501851;
        break;
    case 76 :
        nibunno3x = 3965683;
        nibunnox3 = 7490638;
        break;
    case 77 :
        nibunno3x = 3962775;
        nibunnox3 = 7479429;
        break;
    case 78 :
        nibunno3x = 3959870;
        nibunnox3 = 7468237;
        break;
    case 79 :
        nibunno3x = 3956969;
        nibunnox3 = 7457061;
        break;
    case 80 :
        nibunno3x = 3954068;
        nibunnox3 = 7445893;
        break;
    case 81 :
        nibunno3x = 3951170;
        nibunnox3 = 7434745;
        break;
    case 82 :
        nibunno3x = 3948274;
        nibunnox3 = 7423605;
        break;
    case 83 :
        nibunno3x = 3945379;
        nibunnox3 = 7412476;
        break;
    case 84 :
        nibunno3x = 3942487;
        nibunnox3 = 7401363;
        break;
    case 85 :
        nibunno3x = 3939596;
        nibunnox3 = 7390262;
        break;
    case 86 :
        nibunno3x = 3936709;
        nibunnox3 = 7379178;
        break;
    case 87 :
        nibunno3x = 3933822;
        nibunnox3 = 7368102;
        break;
    case 88 :
        nibunno3x = 3930938;
        nibunnox3 = 7357039;
        break;
    case 89 :
        nibunno3x = 3928056;
        nibunnox3 = 7345990;
        break;
    case 90 :
        nibunno3x = 3925176;
        nibunnox3 = 7334958;
        break;
    case 91 :
        nibunno3x = 3922297;
        nibunnox3 = 7323931;
        break;
    case 92 :
        nibunno3x = 3919422;
        nibunnox3 = 7312923;
        break;
    case 93 :
        nibunno3x = 3916548;
        nibunnox3 = 7301926;
        break;
    case 94 :
        nibunno3x = 3913676;
        nibunnox3 = 7290947;
        break;
    case 95 :
        nibunno3x = 3910806;
        nibunnox3 = 7279971;
        break;
    case 96 :
        nibunno3x = 3907938;
        nibunnox3 = 7269016;
        break;
    case 97 :
        nibunno3x = 3905072;
        nibunnox3 = 7258068;
        break;
    case 98 :
        nibunno3x = 3902208;
        nibunnox3 = 7247135;
        break;
    case 99 :
        nibunno3x = 3899346;
        nibunnox3 = 7236215;
        break;
    case 100 :
        nibunno3x = 3896486;
        nibunnox3 = 7225311;
        break;
    case 101 :
        nibunno3x = 3893628;
        nibunnox3 = 7214413;
        break;
    case 102 :
        nibunno3x = 3890772;
        nibunnox3 = 7203531;
        break;
    case 103 :
        nibunno3x = 3887918;
        nibunnox3 = 7192665;
        break;
    case 104 :
        nibunno3x = 3885066;
        nibunnox3 = 7181804;
        break;
    case 105 :
        nibunno3x = 3882216;
        nibunnox3 = 7170961;
        break;
    case 106 :
        nibunno3x = 3879368;
        nibunnox3 = 7160130;
        break;
    case 107 :
        nibunno3x = 3876522;
        nibunnox3 = 7149310;
        break;
    case 108 :
        nibunno3x = 3873678;
        nibunnox3 = 7138504;
        break;
    case 109 :
        nibunno3x = 3870836;
        nibunnox3 = 7127711;
        break;
    case 110 :
        nibunno3x = 3867995;
        nibunnox3 = 7116927;
        break;
    case 111 :
        nibunno3x = 3865158;
        nibunnox3 = 7106161;
        break;
    case 112 :
        nibunno3x = 3862321;
        nibunnox3 = 7095404;
        break;
    case 113 :
        nibunno3x = 3859486;
        nibunnox3 = 7084657;
        break;
    case 114 :
        nibunno3x = 3856654;
        nibunnox3 = 7073926;
        break;
    case 115 :
        nibunno3x = 3853824;
        nibunnox3 = 7063206;
        break;
    case 116 :
        nibunno3x = 3850996;
        nibunnox3 = 7052502;
        break;
    case 117 :
        nibunno3x = 3848170;
        nibunnox3 = 7041809;
        break;
    case 118 :
        nibunno3x = 3845345;
        nibunnox3 = 7031123;
        break;
    case 119 :
        nibunno3x = 3842522;
        nibunnox3 = 7020454;
        break;
    case 120 :
        nibunno3x = 3839701;
        nibunnox3 = 7009793;
        break;
    case 121 :
        nibunno3x = 3836884;
        nibunnox3 = 6999154;
        break;
    case 122 :
        nibunno3x = 3834067;
        nibunnox3 = 6988519;
        break;
    case 123 :
        nibunno3x = 3831252;
        nibunnox3 = 6977898;
        break;
    case 124 :
        nibunno3x = 3828440;
        nibunnox3 = 6967291;
        break;
    case 125 :
        nibunno3x = 3825628;
        nibunnox3 = 6956691;
        break;
    case 126 :
        nibunno3x = 3822820;
        nibunnox3 = 6946109;
        break;
    case 127 :
        nibunno3x = 3820014;
        nibunnox3 = 6935539;
        break;
    case 128 :
        nibunno3x = 3817208;
        nibunnox3 = 6924979;
        break;
    case 129 :
        nibunno3x = 3814405;
        nibunnox3 = 6914429;
        break;
    case 130 :
        nibunno3x = 3811604;
        nibunnox3 = 6903896;
        break;
    case 131 :
        nibunno3x = 3808805;
        nibunnox3 = 6893370;
        break;
    case 132 :
        nibunno3x = 3806008;
        nibunnox3 = 6882861;
        break;
    case 133 :
        nibunno3x = 3803212;
        nibunnox3 = 6872358;
        break;
    case 134 :
        nibunno3x = 3800419;
        nibunnox3 = 6861873;
        break;
    case 135 :
        nibunno3x = 3797628;
        nibunnox3 = 6851398;
        break;
    case 136 :
        nibunno3x = 3794838;
        nibunnox3 = 6840935;
        break;
    case 137 :
        nibunno3x = 3792050;
        nibunnox3 = 6830483;
        break;
    case 138 :
        nibunno3x = 3789264;
        nibunnox3 = 6820039;
        break;
    case 139 :
        nibunno3x = 3786481;
        nibunnox3 = 6809617;
        break;
    case 140 :
        nibunno3x = 3783698;
        nibunnox3 = 6799198;
        break;
    case 141 :
        nibunno3x = 3780918;
        nibunnox3 = 6788794;
        break;
    case 142 :
        nibunno3x = 3778140;
        nibunnox3 = 6778402;
        break;
    case 143 :
        nibunno3x = 3775364;
        nibunnox3 = 6768024;
        break;
    case 144 :
        nibunno3x = 3772590;
        nibunnox3 = 6757653;
        break;
    case 145 :
        nibunno3x = 3769818;
        nibunnox3 = 6747298;
        break;
    case 146 :
        nibunno3x = 3767047;
        nibunnox3 = 6736954;
        break;
    case 147 :
        nibunno3x = 3764279;
        nibunnox3 = 6726622;
        break;
    case 148 :
        nibunno3x = 3761512;
        nibunnox3 = 6716301;
        break;
    case 149 :
        nibunno3x = 3758746;
        nibunnox3 = 6705987;
        break;
    case 150 :
        nibunno3x = 3755984;
        nibunnox3 = 6695695;
        break;
    case 151 :
        nibunno3x = 3753223;
        nibunnox3 = 6685408;
        break;
    case 152 :
        nibunno3x = 3750464;
        nibunnox3 = 6675133;
        break;
    case 153 :
        nibunno3x = 3747706;
        nibunnox3 = 6664869;
        break;
    case 154 :
        nibunno3x = 3744950;
        nibunnox3 = 6654618;
        break;
    case 155 :
        nibunno3x = 3742196;
        nibunnox3 = 6644377;
        break;
    case 156 :
        nibunno3x = 3739446;
        nibunnox3 = 6634153;
        break;
    case 157 :
        nibunno3x = 3736696;
        nibunnox3 = 6623937;
        break;
    case 158 :
        nibunno3x = 3733948;
        nibunnox3 = 6613733;
        break;
    case 159 :
        nibunno3x = 3731202;
        nibunnox3 = 6603537;
        break;
    case 160 :
        nibunno3x = 3728458;
        nibunnox3 = 6593359;
        break;
    case 161 :
        nibunno3x = 3725714;
        nibunnox3 = 6583185;
        break;
    case 162 :
        nibunno3x = 3722975;
        nibunnox3 = 6573029;
        break;
    case 163 :
        nibunno3x = 3720236;
        nibunnox3 = 6562881;
        break;
    case 164 :
        nibunno3x = 3717500;
        nibunnox3 = 6552751;
        break;
    case 165 :
        nibunno3x = 3714764;
        nibunnox3 = 6542623;
        break;
    case 166 :
        nibunno3x = 3712031;
        nibunnox3 = 6532508;
        break;
    case 167 :
        nibunno3x = 3709300;
        nibunnox3 = 6522409;
        break;
    case 168 :
        nibunno3x = 3706570;
        nibunnox3 = 6512316;
        break;
    case 169 :
        nibunno3x = 3703843;
        nibunnox3 = 6502240;
        break;
    case 170 :
        nibunno3x = 3701118;
        nibunnox3 = 6492174;
        break;
    case 171 :
        nibunno3x = 3698394;
        nibunnox3 = 6482117;
        break;
    case 172 :
        nibunno3x = 3695671;
        nibunnox3 = 6472071;
        break;
    case 173 :
        nibunno3x = 3692952;
        nibunnox3 = 6462040;
        break;
    case 174 :
        nibunno3x = 3690234;
        nibunnox3 = 6452019;
        break;
    case 175 :
        nibunno3x = 3687517;
        nibunnox3 = 6442009;
        break;
    case 176 :
        nibunno3x = 3684802;
        nibunnox3 = 6432008;
        break;
    case 177 :
        nibunno3x = 3682089;
        nibunnox3 = 6422020;
        break;
    case 178 :
        nibunno3x = 3679378;
        nibunnox3 = 6412043;
        break;
    case 179 :
        nibunno3x = 3676669;
        nibunnox3 = 6402078;
        break;
    case 180 :
        nibunno3x = 3673962;
        nibunnox3 = 6392122;
        break;
    case 181 :
        nibunno3x = 3671256;
        nibunnox3 = 6382180;
        break;
    case 182 :
        nibunno3x = 3668552;
        nibunnox3 = 6372248;
        break;
    case 183 :
        nibunno3x = 3665851;
        nibunnox3 = 6362329;
        break;
    case 184 :
        nibunno3x = 3663151;
        nibunnox3 = 6352419;
        break;
    case 185 :
        nibunno3x = 3660452;
        nibunnox3 = 6342519;
        break;
    case 186 :
        nibunno3x = 3657756;
        nibunnox3 = 6332629;
        break;
    case 187 :
        nibunno3x = 3655062;
        nibunnox3 = 6322754;
        break;
    case 188 :
        nibunno3x = 3652368;
        nibunnox3 = 6312888;
        break;
    case 189 :
        nibunno3x = 3649678;
        nibunnox3 = 6303036;
        break;
    case 190 :
        nibunno3x = 3646988;
        nibunnox3 = 6293191;
        break;
    case 191 :
        nibunno3x = 3644300;
        nibunnox3 = 6283356;
        break;
    case 192 :
        nibunno3x = 3641616;
        nibunnox3 = 6273537;
        break;
    case 193 :
        nibunno3x = 3638932;
        nibunnox3 = 6263727;
        break;
    case 194 :
        nibunno3x = 3636248;
        nibunnox3 = 6253922;
        break;
    case 195 :
        nibunno3x = 3633570;
        nibunnox3 = 6244137;
        break;
    case 196 :
        nibunno3x = 3630890;
        nibunnox3 = 6234358;
        break;
    case 197 :
        nibunno3x = 3628214;
        nibunnox3 = 6224593;
        break;
    case 198 :
        nibunno3x = 3625538;
        nibunnox3 = 6214833;
        break;
    case 199 :
        nibunno3x = 3622866;
        nibunnox3 = 6205088;
        break;
    case 200 :
        nibunno3x = 3620196;
        nibunnox3 = 6195358;
        break;
    case 201 :
        nibunno3x = 3617525;
        nibunnox3 = 6185629;
        break;
    case 202 :
        nibunno3x = 3614858;
        nibunnox3 = 6175919;
        break;
    case 203 :
        nibunno3x = 3612192;
        nibunnox3 = 6166216;
        break;
    case 204 :
        nibunno3x = 3609528;
        nibunnox3 = 6156525;
        break;
    case 205 :
        nibunno3x = 3606864;
        nibunnox3 = 6146842;
        break;
    case 206 :
        nibunno3x = 3604204;
        nibunnox3 = 6137174;
        break;
    case 207 :
        nibunno3x = 3601546;
        nibunnox3 = 6127517;
        break;
    case 208 :
        nibunno3x = 3598888;
        nibunnox3 = 6117865;
        break;
    case 209 :
        nibunno3x = 3596233;
        nibunnox3 = 6108229;
        break;
    case 210 :
        nibunno3x = 3593580;
        nibunnox3 = 6098602;
        break;
    case 211 :
        nibunno3x = 3590928;
        nibunnox3 = 6088985;
        break;
    case 212 :
        nibunno3x = 3588278;
        nibunnox3 = 6079383;
        break;
    case 213 :
        nibunno3x = 3585630;
        nibunnox3 = 6069785;
        break;
    case 214 :
        nibunno3x = 3582982;
        nibunnox3 = 6060197;
        break;
    case 215 :
        nibunno3x = 3580338;
        nibunnox3 = 6050624;
        break;
    case 216 :
        nibunno3x = 3577695;
        nibunnox3 = 6041063;
        break;
    case 217 :
        nibunno3x = 3575053;
        nibunnox3 = 6031507;
        break;
    case 218 :
        nibunno3x = 3572414;
        nibunnox3 = 6021968;
        break;
    case 219 :
        nibunno3x = 3569776;
        nibunnox3 = 6012434;
        break;
    case 220 :
        nibunno3x = 3567140;
        nibunnox3 = 6002914;
        break;
    case 221 :
        nibunno3x = 3564506;
        nibunnox3 = 5993404;
        break;
    case 222 :
        nibunno3x = 3561874;
        nibunnox3 = 5983904;
        break;
    case 223 :
        nibunno3x = 3559243;
        nibunnox3 = 5974413;
        break;
    case 224 :
        nibunno3x = 3556614;
        nibunnox3 = 5964935;
        break;
    case 225 :
        nibunno3x = 3553986;
        nibunnox3 = 5955461;
        break;
    case 226 :
        nibunno3x = 3551360;
        nibunnox3 = 5946005;
        break;
    case 227 :
        nibunno3x = 3548737;
        nibunnox3 = 5936558;
        break;
    case 228 :
        nibunno3x = 3546114;
        nibunnox3 = 5927118;
        break;
    case 229 :
        nibunno3x = 3543494;
        nibunnox3 = 5917692;
        break;
    case 230 :
        nibunno3x = 3540876;
        nibunnox3 = 5908274;
        break;
    case 231 :
        nibunno3x = 3538258;
        nibunnox3 = 5898865;
        break;
    case 232 :
        nibunno3x = 3535644;
        nibunnox3 = 5889472;
        break;
    case 233 :
        nibunno3x = 3533029;
        nibunnox3 = 5880082;
        break;
    case 234 :
        nibunno3x = 3530418;
        nibunnox3 = 5870707;
        break;
    case 235 :
        nibunno3x = 3527808;
        nibunnox3 = 5861344;
        break;
    case 236 :
        nibunno3x = 3525199;
        nibunnox3 = 5851986;
        break;
    case 237 :
        nibunno3x = 3522592;
        nibunnox3 = 5842640;
        break;
    case 238 :
        nibunno3x = 3519988;
        nibunnox3 = 5833308;
        break;
    case 239 :
        nibunno3x = 3517384;
        nibunnox3 = 5823981;
        break;
    case 240 :
        nibunno3x = 3514783;
        nibunnox3 = 5814668;
        break;
    case 241 :
        nibunno3x = 3512183;
        nibunnox3 = 5805362;
        break;
    case 242 :
        nibunno3x = 3509585;
        nibunnox3 = 5796068;
        break;
    case 243 :
        nibunno3x = 3506988;
        nibunnox3 = 5786784;
        break;
    case 244 :
        nibunno3x = 3504394;
        nibunnox3 = 5777511;
        break;
    case 245 :
        nibunno3x = 3501801;
        nibunnox3 = 5768248;
        break;
    case 246 :
        nibunno3x = 3499209;
        nibunnox3 = 5758992;
        break;
    case 247 :
        nibunno3x = 3496620;
        nibunnox3 = 5749748;
        break;
    case 248 :
        nibunno3x = 3494031;
        nibunnox3 = 5740514;
        break;
    case 249 :
        nibunno3x = 3491445;
        nibunnox3 = 5731290;
        break;
    case 250 :
        nibunno3x = 3488862;
        nibunnox3 = 5722080;
        break;
    case 251 :
        nibunno3x = 3486278;
        nibunnox3 = 5712874;
        break;
    case 252 :
        nibunno3x = 3483697;
        nibunnox3 = 5703682;
        break;
    case 253 :
        nibunno3x = 3481117;
        nibunnox3 = 5694496;
        break;
    case 254 :
        nibunno3x = 3478538;
        nibunnox3 = 5685320;
        break;
    case 255 :
        nibunno3x = 3475963;
        nibunnox3 = 5676159;
        break;
    case 256 :
        nibunno3x = 3473389;
        nibunnox3 = 5667007;
        break;
    case 257 :
        nibunno3x = 3470816;
        nibunnox3 = 5657864;
        break;
    case 258 :
        nibunno3x = 3468245;
        nibunnox3 = 5648728;
        break;
    case 259 :
        nibunno3x = 3465675;
        nibunnox3 = 5639604;
        break;
    case 260 :
        nibunno3x = 3463106;
        nibunnox3 = 5630487;
        break;
    case 261 :
        nibunno3x = 3460541;
        nibunnox3 = 5621384;
        break;
    case 262 :
        nibunno3x = 3457976;
        nibunnox3 = 5612290;
        break;
    case 263 :
        nibunno3x = 3455414;
        nibunnox3 = 5603206;
        break;
    case 264 :
        nibunno3x = 3452852;
        nibunnox3 = 5594131;
        break;
    case 265 :
        nibunno3x = 3450294;
        nibunnox3 = 5585067;
        break;
    case 266 :
        nibunno3x = 3447736;
        nibunnox3 = 5576013;
        break;
    case 267 :
        nibunno3x = 3445180;
        nibunnox3 = 5566968;
        break;
    case 268 :
        nibunno3x = 3442625;
        nibunnox3 = 5557930;
        break;
    case 269 :
        nibunno3x = 3440072;
        nibunnox3 = 5548906;
        break;
    case 270 :
        nibunno3x = 3437521;
        nibunnox3 = 5539889;
        break;
    case 271 :
        nibunno3x = 3434971;
        nibunnox3 = 5530881;
        break;
    case 272 :
        nibunno3x = 3432424;
        nibunnox3 = 5521888;
        break;
    case 273 :
        nibunno3x = 3429877;
        nibunnox3 = 5512898;
        break;
    case 274 :
        nibunno3x = 3427333;
        nibunnox3 = 5503923;
        break;
    case 275 :
        nibunno3x = 3424789;
        nibunnox3 = 5494951;
        break;
    case 276 :
        nibunno3x = 3422248;
        nibunnox3 = 5485994;
        break;
    case 277 :
        nibunno3x = 3419708;
        nibunnox3 = 5477046;
        break;
    case 278 :
        nibunno3x = 3417170;
        nibunnox3 = 5468106;
        break;
    case 279 :
        nibunno3x = 3414634;
        nibunnox3 = 5459178;
        break;
    case 280 :
        nibunno3x = 3412099;
        nibunnox3 = 5450258;
        break;
    case 281 :
        nibunno3x = 3409566;
        nibunnox3 = 5441347;
        break;
    case 282 :
        nibunno3x = 3407034;
        nibunnox3 = 5432447;
        break;
    case 283 :
        nibunno3x = 3404504;
        nibunnox3 = 5423556;
        break;
    case 284 :
        nibunno3x = 3401976;
        nibunnox3 = 5414675;
        break;
    case 285 :
        nibunno3x = 3399450;
        nibunnox3 = 5405803;
        break;
    case 286 :
        nibunno3x = 3396925;
        nibunnox3 = 5396942;
        break;
    case 287 :
        nibunno3x = 3394401;
        nibunnox3 = 5388088;
        break;
    case 288 :
        nibunno3x = 3391880;
        nibunnox3 = 5379245;
        break;
    case 289 :
        nibunno3x = 3389359;
        nibunnox3 = 5370409;
        break;
    case 290 :
        nibunno3x = 3386840;
        nibunnox3 = 5361585;
        break;
    case 291 :
        nibunno3x = 3384324;
        nibunnox3 = 5352770;
        break;
    case 292 :
        nibunno3x = 3381808;
        nibunnox3 = 5343963;
        break;
    case 293 :
        nibunno3x = 3379295;
        nibunnox3 = 5335168;
        break;
    case 294 :
        nibunno3x = 3376783;
        nibunnox3 = 5326383;
        break;
    case 295 :
        nibunno3x = 3374272;
        nibunnox3 = 5317603;
        break;
    case 296 :
        nibunno3x = 3371763;
        nibunnox3 = 5308835;
        break;
    case 297 :
        nibunno3x = 3369256;
        nibunnox3 = 5300077;
        break;
    case 298 :
        nibunno3x = 3366750;
        nibunnox3 = 5291324;
        break;
    case 299 :
        nibunno3x = 3364246;
        nibunnox3 = 5282586;
        break;
    case 300 :
        nibunno3x = 3361744;
        nibunnox3 = 5273856;
        break;
    case 301 :
        nibunno3x = 3359243;
        nibunnox3 = 5265133;
        break;
    case 302 :
        nibunno3x = 3356743;
        nibunnox3 = 5256420;
        break;
    case 303 :
        nibunno3x = 3354246;
        nibunnox3 = 5247719;
        break;
    case 304 :
        nibunno3x = 3351750;
        nibunnox3 = 5239024;
        break;
    case 305 :
        nibunno3x = 3349256;
        nibunnox3 = 5230341;
        break;
    case 306 :
        nibunno3x = 3346763;
        nibunnox3 = 5221666;
        break;
    case 307 :
        nibunno3x = 3344271;
        nibunnox3 = 5212999;
        break;
    case 308 :
        nibunno3x = 3341781;
        nibunnox3 = 5204341;
        break;
    case 309 :
        nibunno3x = 3339294;
        nibunnox3 = 5195695;
        break;
    case 310 :
        nibunno3x = 3336806;
        nibunnox3 = 5187054;
        break;
    case 311 :
        nibunno3x = 3334322;
        nibunnox3 = 5178426;
        break;
    case 312 :
        nibunno3x = 3331838;
        nibunnox3 = 5169806;
        break;
    case 313 :
        nibunno3x = 3329357;
        nibunnox3 = 5161195;
        break;
    case 314 :
        nibunno3x = 3326876;
        nibunnox3 = 5152592;
        break;
    case 315 :
        nibunno3x = 3324397;
        nibunnox3 = 5143997;
        break;
    case 316 :
        nibunno3x = 3321920;
        nibunnox3 = 5135415;
        break;
    case 317 :
        nibunno3x = 3319446;
        nibunnox3 = 5126842;
        break;
    case 318 :
        nibunno3x = 3316970;
        nibunnox3 = 5118273;
        break;
    case 319 :
        nibunno3x = 3314498;
        nibunnox3 = 5109717;
        break;
    case 320 :
        nibunno3x = 3312028;
        nibunnox3 = 5101170;
        break;
    case 321 :
        nibunno3x = 3309559;
        nibunnox3 = 5092633;
        break;
    case 322 :
        nibunno3x = 3307092;
        nibunnox3 = 5084104;
        break;
    case 323 :
        nibunno3x = 3304624;
        nibunnox3 = 5075579;
        break;
    case 324 :
        nibunno3x = 3302160;
        nibunnox3 = 5067070;
        break;
    case 325 :
        nibunno3x = 3299696;
        nibunnox3 = 5058564;
        break;
    case 326 :
        nibunno3x = 3297235;
        nibunnox3 = 5050071;
        break;
    case 327 :
        nibunno3x = 3294776;
        nibunnox3 = 5041588;
        break;
    case 328 :
        nibunno3x = 3292316;
        nibunnox3 = 5033109;
        break;
    case 329 :
        nibunno3x = 3289861;
        nibunnox3 = 5024646;
        break;
    case 330 :
        nibunno3x = 3287405;
        nibunnox3 = 5016185;
        break;
    case 331 :
        nibunno3x = 3284952;
        nibunnox3 = 5007737;
        break;
    case 332 :
        nibunno3x = 3282499;
        nibunnox3 = 4999295;
        break;
    case 333 :
        nibunno3x = 3280049;
        nibunnox3 = 4990865;
        break;
    case 334 :
        nibunno3x = 3277600;
        nibunnox3 = 4982443;
        break;
    case 335 :
        nibunno3x = 3275152;
        nibunnox3 = 4974029;
        break;
    case 336 :
        nibunno3x = 3272706;
        nibunnox3 = 4965622;
        break;
    case 337 :
        nibunno3x = 3270262;
        nibunnox3 = 4957229;
        break;
    case 338 :
        nibunno3x = 3267818;
        nibunnox3 = 4948839;
        break;
    case 339 :
        nibunno3x = 3265378;
        nibunnox3 = 4940464;
        break;
    case 340 :
        nibunno3x = 3262938;
        nibunnox3 = 4932092;
        break;
    case 341 :
        nibunno3x = 3260499;
        nibunnox3 = 4923731;
        break;
    case 342 :
        nibunno3x = 3258062;
        nibunnox3 = 4915378;
        break;
    case 343 :
        nibunno3x = 3255626;
        nibunnox3 = 4907032;
        break;
    case 344 :
        nibunno3x = 3253194;
        nibunnox3 = 4898700;
        break;
    case 345 :
        nibunno3x = 3250760;
        nibunnox3 = 4890371;
        break;
    case 346 :
        nibunno3x = 3248330;
        nibunnox3 = 4882055;
        break;
    case 347 :
        nibunno3x = 3245902;
        nibunnox3 = 4873749;
        break;
    case 348 :
        nibunno3x = 3243474;
        nibunnox3 = 4865446;
        break;
    case 349 :
        nibunno3x = 3241047;
        nibunnox3 = 4857154;
        break;
    case 350 :
        nibunno3x = 3238622;
        nibunnox3 = 4848871;
        break;
    case 351 :
        nibunno3x = 3236200;
        nibunnox3 = 4840598;
        break;
    case 352 :
        nibunno3x = 3233778;
        nibunnox3 = 4832329;
        break;
    case 353 :
        nibunno3x = 3231358;
        nibunnox3 = 4824075;
        break;
    case 354 :
        nibunno3x = 3228940;
        nibunnox3 = 4815828;
        break;
    case 355 :
        nibunno3x = 3226524;
        nibunnox3 = 4807590;
        break;
    case 356 :
        nibunno3x = 3224107;
        nibunnox3 = 4799355;
        break;
    case 357 :
        nibunno3x = 3221693;
        nibunnox3 = 4791132;
        break;
    case 358 :
        nibunno3x = 3219280;
        nibunnox3 = 4782917;
        break;
    case 359 :
        nibunno3x = 3216870;
        nibunnox3 = 4774713;
        break;
    case 360 :
        nibunno3x = 3214459;
        nibunnox3 = 4766512;
        break;
    case 361 :
        nibunno3x = 3212052;
        nibunnox3 = 4758325;
        break;
    case 362 :
        nibunno3x = 3209646;
        nibunnox3 = 4750146;
        break;
    case 363 :
        nibunno3x = 3207241;
        nibunnox3 = 4741977;
        break;
    case 364 :
        nibunno3x = 3204836;
        nibunnox3 = 4733810;
        break;
    case 365 :
        nibunno3x = 3202434;
        nibunnox3 = 4725652;
        break;
    case 366 :
        nibunno3x = 3200034;
        nibunnox3 = 4717508;
        break;
    case 367 :
        nibunno3x = 3197635;
        nibunnox3 = 4709372;
        break;
    case 368 :
        nibunno3x = 3195238;
        nibunnox3 = 4701244;
        break;
    case 369 :
        nibunno3x = 3192842;
        nibunnox3 = 4693123;
        break;
    case 370 :
        nibunno3x = 3190447;
        nibunnox3 = 4685010;
        break;
    case 371 :
        nibunno3x = 3188053;
        nibunnox3 = 4676902;
        break;
    case 372 :
        nibunno3x = 3185662;
        nibunnox3 = 4668808;
        break;
    case 373 :
        nibunno3x = 3183272;
        nibunnox3 = 4660723;
        break;
    case 374 :
        nibunno3x = 3180883;
        nibunnox3 = 4652640;
        break;
    case 375 :
        nibunno3x = 3178495;
        nibunnox3 = 4644567;
        break;
    case 376 :
        nibunno3x = 3176110;
        nibunnox3 = 4636507;
        break;
    case 377 :
        nibunno3x = 3173726;
        nibunnox3 = 4628453;
        break;
    case 378 :
        nibunno3x = 3171344;
        nibunnox3 = 4620411;
        break;
    case 379 :
        nibunno3x = 3168962;
        nibunnox3 = 4612369;
        break;
    case 380 :
        nibunno3x = 3166582;
        nibunnox3 = 4604339;
        break;
    case 381 :
        nibunno3x = 3164203;
        nibunnox3 = 4596316;
        break;
    case 382 :
        nibunno3x = 3161827;
        nibunnox3 = 4588306;
        break;
    case 383 :
        nibunno3x = 3159452;
        nibunnox3 = 4580303;
        break;
    case 384 :
        nibunno3x = 3157078;
        nibunnox3 = 4572307;
        break;
    case 385 :
        nibunno3x = 3154705;
        nibunnox3 = 4564316;
        break;
    case 386 :
        nibunno3x = 3152334;
        nibunnox3 = 4556335;
        break;
    case 387 :
        nibunno3x = 3149964;
        nibunnox3 = 4548364;
        break;
    case 388 :
        nibunno3x = 3147596;
        nibunnox3 = 4540402;
        break;
    case 389 :
        nibunno3x = 3145230;
        nibunnox3 = 4532445;
        break;
    case 390 :
        nibunno3x = 3142866;
        nibunnox3 = 4524501;
        break;
    case 391 :
        nibunno3x = 3140501;
        nibunnox3 = 4516559;
        break;
    case 392 :
        nibunno3x = 3138139;
        nibunnox3 = 4508630;
        break;
    case 393 :
        nibunno3x = 3135778;
        nibunnox3 = 4500706;
        break;
    case 394 :
        nibunno3x = 3133418;
        nibunnox3 = 4492791;
        break;
    case 395 :
        nibunno3x = 3131060;
        nibunnox3 = 4484884;
        break;
    case 396 :
        nibunno3x = 3128705;
        nibunnox3 = 4476988;
        break;
    case 397 :
        nibunno3x = 3126349;
        nibunnox3 = 4469095;
        break;
    case 398 :
        nibunno3x = 3123996;
        nibunnox3 = 4461213;
        break;
    case 399 :
        nibunno3x = 3121644;
        nibunnox3 = 4453339;
        break;
    case 400 :
        nibunno3x = 3119293;
        nibunnox3 = 4445473;
        break;
    case 401 :
        nibunno3x = 3116943;
        nibunnox3 = 4437613;
        break;
    case 402 :
        nibunno3x = 3114595;
        nibunnox3 = 4429761;
        break;
    case 403 :
        nibunno3x = 3112249;
        nibunnox3 = 4421920;
        break;
    case 404 :
        nibunno3x = 3109904;
        nibunnox3 = 4414087;
        break;
    case 405 :
        nibunno3x = 3107561;
        nibunnox3 = 4406260;
        break;
    case 406 :
        nibunno3x = 3105218;
        nibunnox3 = 4398441;
        break;
    case 407 :
        nibunno3x = 3102878;
        nibunnox3 = 4390630;
        break;
    case 408 :
        nibunno3x = 3100538;
        nibunnox3 = 4382828;
        break;
    case 409 :
        nibunno3x = 3098200;
        nibunnox3 = 4375031;
        break;
    case 410 :
        nibunno3x = 3095864;
        nibunnox3 = 4367247;
        break;
    case 411 :
        nibunno3x = 3093529;
        nibunnox3 = 4359466;
        break;
    case 412 :
        nibunno3x = 3091195;
        nibunnox3 = 4351694;
        break;
    case 413 :
        nibunno3x = 3088864;
        nibunnox3 = 4343935;
        break;
    case 414 :
        nibunno3x = 3086533;
        nibunnox3 = 4336178;
        break;
    case 415 :
        nibunno3x = 3084204;
        nibunnox3 = 4328431;
        break;
    case 416 :
        nibunno3x = 3081876;
        nibunnox3 = 4320691;
        break;
    case 417 :
        nibunno3x = 3079550;
        nibunnox3 = 4312962;
        break;
    case 418 :
        nibunno3x = 3077224;
        nibunnox3 = 4305236;
        break;
    case 419 :
        nibunno3x = 3074900;
        nibunnox3 = 4297520;
        break;
    case 420 :
        nibunno3x = 3072578;
        nibunnox3 = 4289813;
        break;
    case 421 :
        nibunno3x = 3070258;
        nibunnox3 = 4282113;
        break;
    case 422 :
        nibunno3x = 3067939;
        nibunnox3 = 4274422;
        break;
    case 423 :
        nibunno3x = 3065620;
        nibunnox3 = 4266734;
        break;
    case 424 :
        nibunno3x = 3063304;
        nibunnox3 = 4259059;
        break;
    case 425 :
        nibunno3x = 3060990;
        nibunnox3 = 4251392;
        break;
    case 426 :
        nibunno3x = 3058676;
        nibunnox3 = 4243730;
        break;
    case 427 :
        nibunno3x = 3056362;
        nibunnox3 = 4236073;
        break;
    case 428 :
        nibunno3x = 3054052;
        nibunnox3 = 4228429;
        break;
    case 429 :
        nibunno3x = 3051742;
        nibunnox3 = 4220790;
        break;
    case 430 :
        nibunno3x = 3049434;
        nibunnox3 = 4213159;
        break;
    case 431 :
        nibunno3x = 3047127;
        nibunnox3 = 4205537;
        break;
    case 432 :
        nibunno3x = 3044822;
        nibunnox3 = 4197924;
        break;
    case 433 :
        nibunno3x = 3042518;
        nibunnox3 = 4190314;
        break;
    case 434 :
        nibunno3x = 3040216;
        nibunnox3 = 4182717;
        break;
    case 435 :
        nibunno3x = 3037914;
        nibunnox3 = 4175123;
        break;
    case 436 :
        nibunno3x = 3035615;
        nibunnox3 = 4167540;
        break;
    case 437 :
        nibunno3x = 3033316;
        nibunnox3 = 4159961;
        break;
    case 438 :
        nibunno3x = 3031019;
        nibunnox3 = 4152392;
        break;
    case 439 :
        nibunno3x = 3028724;
        nibunnox3 = 4144832;
        break;
    case 440 :
        nibunno3x = 3026430;
        nibunnox3 = 4137277;
        break;
    case 441 :
        nibunno3x = 3024136;
        nibunnox3 = 4129730;
        break;
    case 442 :
        nibunno3x = 3021845;
        nibunnox3 = 4122191;
        break;
    case 443 :
        nibunno3x = 3019555;
        nibunnox3 = 4114661;
        break;
    case 444 :
        nibunno3x = 3017266;
        nibunnox3 = 4107136;
        break;
    case 445 :
        nibunno3x = 3014978;
        nibunnox3 = 4099619;
        break;
    case 446 :
        nibunno3x = 3012692;
        nibunnox3 = 4092110;
        break;
    case 447 :
        nibunno3x = 3010408;
        nibunnox3 = 4084610;
        break;
    case 448 :
        nibunno3x = 3008125;
        nibunnox3 = 4077117;
        break;
    case 449 :
        nibunno3x = 3005843;
        nibunnox3 = 4069630;
        break;
    case 450 :
        nibunno3x = 3003562;
        nibunnox3 = 4062150;
        break;
    case 451 :
        nibunno3x = 3001284;
        nibunnox3 = 4054681;
        break;
    case 452 :
        nibunno3x = 2999006;
        nibunnox3 = 4047217;
        break;
    case 453 :
        nibunno3x = 2996730;
        nibunnox3 = 4039761;
        break;
    case 454 :
        nibunno3x = 2994454;
        nibunnox3 = 4032311;
        break;
    case 455 :
        nibunno3x = 2992181;
        nibunnox3 = 4024870;
        break;
    case 456 :
        nibunno3x = 2989908;
        nibunnox3 = 4017434;
        break;
    case 457 :
        nibunno3x = 2987637;
        nibunnox3 = 4010009;
        break;
    case 458 :
        nibunno3x = 2985368;
        nibunnox3 = 4002593;
        break;
    case 459 :
        nibunno3x = 2983100;
        nibunnox3 = 3995180;
        break;
    case 460 :
        nibunno3x = 2980832;
        nibunnox3 = 3987774;
        break;
    case 461 :
        nibunno3x = 2978568;
        nibunnox3 = 3980378;
        break;
    case 462 :
        nibunno3x = 2976302;
        nibunnox3 = 3972986;
        break;
    case 463 :
        nibunno3x = 2974040;
        nibunnox3 = 3965604;
        break;
    case 464 :
        nibunno3x = 2971778;
        nibunnox3 = 3958230;
        break;
    case 465 :
        nibunno3x = 2969519;
        nibunnox3 = 3950863;
        break;
    case 466 :
        nibunno3x = 2967259;
        nibunnox3 = 3943499;
        break;
    case 467 :
        nibunno3x = 2965003;
        nibunnox3 = 3936151;
        break;
    case 468 :
        nibunno3x = 2962746;
        nibunnox3 = 3928804;
        break;
    case 469 :
        nibunno3x = 2960492;
        nibunnox3 = 3921468;
        break;
    case 470 :
        nibunno3x = 2958238;
        nibunnox3 = 3914134;
        break;
    case 471 :
        nibunno3x = 2955986;
        nibunnox3 = 3906811;
        break;
    case 472 :
        nibunno3x = 2953735;
        nibunnox3 = 3899493;
        break;
    case 473 :
        nibunno3x = 2951486;
        nibunnox3 = 3892186;
        break;
    case 474 :
        nibunno3x = 2949238;
        nibunnox3 = 3884882;
        break;
    case 475 :
        nibunno3x = 2946991;
        nibunnox3 = 3877586;
        break;
    case 476 :
        nibunno3x = 2944746;
        nibunnox3 = 3870300;
        break;
    case 477 :
        nibunno3x = 2942503;
        nibunnox3 = 3863023;
        break;
    case 478 :
        nibunno3x = 2940260;
        nibunnox3 = 3855748;
        break;
    case 479 :
        nibunno3x = 2938018;
        nibunnox3 = 3848481;
        break;
    case 480 :
        nibunno3x = 2935778;
        nibunnox3 = 3841224;
        break;
    case 481 :
        nibunno3x = 2933539;
        nibunnox3 = 3833969;
        break;
    case 482 :
        nibunno3x = 2931302;
        nibunnox3 = 3826728;
        break;
    case 483 :
        nibunno3x = 2929066;
        nibunnox3 = 3819489;
        break;
    case 484 :
        nibunno3x = 2926831;
        nibunnox3 = 3812258;
        break;
    case 485 :
        nibunno3x = 2924598;
        nibunnox3 = 3805034;
        break;
    case 486 :
        nibunno3x = 2922366;
        nibunnox3 = 3797821;
        break;
    case 487 :
        nibunno3x = 2920135;
        nibunnox3 = 3790611;
        break;
    case 488 :
        nibunno3x = 2917906;
        nibunnox3 = 3783410;
        break;
    case 489 :
        nibunno3x = 2915678;
        nibunnox3 = 3776215;
        break;
    case 490 :
        nibunno3x = 2913451;
        nibunnox3 = 3769028;
        break;
    case 491 :
        nibunno3x = 2911226;
        nibunnox3 = 3761848;
        break;
    case 492 :
        nibunno3x = 2909002;
        nibunnox3 = 3754676;
        break;
    case 493 :
        nibunno3x = 2906779;
        nibunnox3 = 3747509;
        break;
    case 494 :
        nibunno3x = 2904557;
        nibunnox3 = 3740347;
        break;
    case 495 :
        nibunno3x = 2902336;
        nibunnox3 = 3733194;
        break;
    case 496 :
        nibunno3x = 2900118;
        nibunnox3 = 3726050;
        break;
    case 497 :
        nibunno3x = 2897900;
        nibunnox3 = 3718913;
        break;
    case 498 :
        nibunno3x = 2895684;
        nibunnox3 = 3711780;
        break;
    case 499 :
        nibunno3x = 2893470;
        nibunnox3 = 3704660;
        break;
    case 500 :
        nibunno3x = 2891256;
        nibunnox3 = 3697541;
        break;
    case 501 :
        nibunno3x = 2889042;
        nibunnox3 = 3690428;
        break;
    case 502 :
        nibunno3x = 2886832;
        nibunnox3 = 3683327;
        break;
    case 503 :
        nibunno3x = 2884622;
        nibunnox3 = 3676230;
        break;
    case 504 :
        nibunno3x = 2882413;
        nibunnox3 = 3669140;
        break;
    case 505 :
        nibunno3x = 2880206;
        nibunnox3 = 3662060;
        break;
    case 506 :
        nibunno3x = 2878000;
        nibunnox3 = 3654982;
        break;
    case 507 :
        nibunno3x = 2875796;
        nibunnox3 = 3647917;
        break;
    case 508 :
        nibunno3x = 2873593;
        nibunnox3 = 3640855;
        break;
    case 509 :
        nibunno3x = 2871390;
        nibunnox3 = 3633798;
        break;
    case 510 :
        nibunno3x = 2869190;
        nibunnox3 = 3626751;
        break;
    case 511 :
        nibunno3x = 2866991;
        nibunnox3 = 3619711;
        break;
    case 512 :
        nibunno3x = 2864792;
        nibunnox3 = 3612677;
        break;
    case 513 :
        nibunno3x = 2862596;
        nibunnox3 = 3605650;
        break;
    case 514 :
        nibunno3x = 2860399;
        nibunnox3 = 3598626;
        break;
    case 515 :
        nibunno3x = 2858205;
        nibunnox3 = 3591614;
        break;
    case 516 :
        nibunno3x = 2856013;
        nibunnox3 = 3584610;
        break;
    case 517 :
        nibunno3x = 2853821;
        nibunnox3 = 3577608;
        break;
    case 518 :
        nibunno3x = 2851630;
        nibunnox3 = 3570615;
        break;
    case 519 :
        nibunno3x = 2849442;
        nibunnox3 = 3563630;
        break;
    case 520 :
        nibunno3x = 2847253;
        nibunnox3 = 3556649;
        break;
    case 521 :
        nibunno3x = 2845068;
        nibunnox3 = 3549680;
        break;
    case 522 :
        nibunno3x = 2842882;
        nibunnox3 = 3542713;
        break;
    case 523 :
        nibunno3x = 2840697;
        nibunnox3 = 3535752;
        break;
    case 524 :
        nibunno3x = 2838514;
        nibunnox3 = 3528798;
        break;
    case 525 :
        nibunno3x = 2836333;
        nibunnox3 = 3521854;
        break;
    case 526 :
        nibunno3x = 2834153;
        nibunnox3 = 3514915;
        break;
    case 527 :
        nibunno3x = 2831974;
        nibunnox3 = 3507984;
        break;
    case 528 :
        nibunno3x = 2829796;
        nibunnox3 = 3501058;
        break;
    case 529 :
        nibunno3x = 2827620;
        nibunnox3 = 3494139;
        break;
    case 530 :
        nibunno3x = 2825444;
        nibunnox3 = 3487227;
        break;
    case 531 :
        nibunno3x = 2823271;
        nibunnox3 = 3480324;
        break;
    case 532 :
        nibunno3x = 2821099;
        nibunnox3 = 3473427;
        break;
    case 533 :
        nibunno3x = 2818926;
        nibunnox3 = 3466531;
        break;
    case 534 :
        nibunno3x = 2816756;
        nibunnox3 = 3459647;
        break;
    case 535 :
        nibunno3x = 2814588;
        nibunnox3 = 3452768;
        break;
    case 536 :
        nibunno3x = 2812420;
        nibunnox3 = 3445896;
        break;
    case 537 :
        nibunno3x = 2810253;
        nibunnox3 = 3439030;
        break;
    case 538 :
        nibunno3x = 2808088;
        nibunnox3 = 3432171;
        break;
    case 539 :
        nibunno3x = 2805925;
        nibunnox3 = 3425321;
        break;
    case 540 :
        nibunno3x = 2803762;
        nibunnox3 = 3418474;
        break;
    case 541 :
        nibunno3x = 2801600;
        nibunnox3 = 3411635;
        break;
    case 542 :
        nibunno3x = 2799440;
        nibunnox3 = 3404803;
        break;
    case 543 :
        nibunno3x = 2797282;
        nibunnox3 = 3397978;
        break;
    case 544 :
        nibunno3x = 2795124;
        nibunnox3 = 3391159;
        break;
    case 545 :
        nibunno3x = 2792968;
        nibunnox3 = 3384347;
        break;
    case 546 :
        nibunno3x = 2790813;
        nibunnox3 = 3377542;
        break;
    case 547 :
        nibunno3x = 2788658;
        nibunnox3 = 3370739;
        break;
    case 548 :
        nibunno3x = 2786507;
        nibunnox3 = 3363950;
        break;
    case 549 :
        nibunno3x = 2784355;
        nibunnox3 = 3357162;
        break;
    case 550 :
        nibunno3x = 2782206;
        nibunnox3 = 3350384;
        break;
    case 551 :
        nibunno3x = 2780056;
        nibunnox3 = 3343609;
        break;
    case 552 :
        nibunno3x = 2777910;
        nibunnox3 = 3336846;
        break;
    case 553 :
        nibunno3x = 2775763;
        nibunnox3 = 3330086;
        break;
    case 554 :
        nibunno3x = 2773617;
        nibunnox3 = 3323330;
        break;
    case 555 :
        nibunno3x = 2771473;
        nibunnox3 = 3316582;
        break;
    case 556 :
        nibunno3x = 2769330;
        nibunnox3 = 3309841;
        break;
    case 557 :
        nibunno3x = 2767189;
        nibunnox3 = 3303107;
        break;
    case 558 :
        nibunno3x = 2765048;
        nibunnox3 = 3296379;
        break;
    case 559 :
        nibunno3x = 2762910;
        nibunnox3 = 3289657;
        break;
    case 560 :
        nibunno3x = 2760772;
        nibunnox3 = 3282943;
        break;
    case 561 :
        nibunno3x = 2758636;
        nibunnox3 = 3276237;
        break;
    case 562 :
        nibunno3x = 2756501;
        nibunnox3 = 3269535;
        break;
    case 563 :
        nibunno3x = 2754367;
        nibunnox3 = 3262839;
        break;
    case 564 :
        nibunno3x = 2752234;
        nibunnox3 = 3256150;
        break;
    case 565 :
        nibunno3x = 2750102;
        nibunnox3 = 3249468;
        break;
    case 566 :
        nibunno3x = 2747972;
        nibunnox3 = 3242792;
        break;
    case 567 :
        nibunno3x = 2745842;
        nibunnox3 = 3236119;
        break;
    case 568 :
        nibunno3x = 2743716;
        nibunnox3 = 3229459;
        break;
    case 569 :
        nibunno3x = 2741588;
        nibunnox3 = 3222798;
        break;
    case 570 :
        nibunno3x = 2739463;
        nibunnox3 = 3216150;
        break;
    case 571 :
        nibunno3x = 2737338;
        nibunnox3 = 3209502;
        break;
    case 572 :
        nibunno3x = 2735215;
        nibunnox3 = 3202865;
        break;
    case 573 :
        nibunno3x = 2733093;
        nibunnox3 = 3196233;
        break;
    case 574 :
        nibunno3x = 2730973;
        nibunnox3 = 3189609;
        break;
    case 575 :
        nibunno3x = 2728854;
        nibunnox3 = 3182989;
        break;
    case 576 :
        nibunno3x = 2726735;
        nibunnox3 = 3176375;
        break;
    case 577 :
        nibunno3x = 2724618;
        nibunnox3 = 3169768;
        break;
    case 578 :
        nibunno3x = 2722502;
        nibunnox3 = 3163170;
        break;
    case 579 :
        nibunno3x = 2720388;
        nibunnox3 = 3156574;
        break;
    case 580 :
        nibunno3x = 2718274;
        nibunnox3 = 3149986;
        break;
    case 581 :
        nibunno3x = 2716162;
        nibunnox3 = 3143405;
        break;
    case 582 :
        nibunno3x = 2714051;
        nibunnox3 = 3136829;
        break;
    case 583 :
        nibunno3x = 2711941;
        nibunnox3 = 3130260;
        break;
    case 584 :
        nibunno3x = 2709833;
        nibunnox3 = 3123698;
        break;
    case 585 :
        nibunno3x = 2707726;
        nibunnox3 = 3117143;
        break;
    case 586 :
        nibunno3x = 2705619;
        nibunnox3 = 3110591;
        break;
    case 587 :
        nibunno3x = 2703514;
        nibunnox3 = 3104046;
        break;
    case 588 :
        nibunno3x = 2701411;
        nibunnox3 = 3097511;
        break;
    case 589 :
        nibunno3x = 2699308;
        nibunnox3 = 3090978;
        break;
    case 590 :
        nibunno3x = 2697206;
        nibunnox3 = 3084452;
        break;
    case 591 :
        nibunno3x = 2695106;
        nibunnox3 = 3077930;
        break;
    case 592 :
        nibunno3x = 2693007;
        nibunnox3 = 3071418;
        break;
    case 593 :
        nibunno3x = 2690910;
        nibunnox3 = 3064912;
        break;
    case 594 :
        nibunno3x = 2688812;
        nibunnox3 = 3058409;
        break;
    case 595 :
        nibunno3x = 2686717;
        nibunnox3 = 3051915;
        break;
    case 596 :
        nibunno3x = 2684623;
        nibunnox3 = 3045426;
        break;
    case 597 :
        nibunno3x = 2682530;
        nibunnox3 = 3038946;
        break;
    case 598 :
        nibunno3x = 2680439;
        nibunnox3 = 3032470;
        break;
    case 599 :
        nibunno3x = 2678347;
        nibunnox3 = 3025996;
        break;
    case 600 :
        nibunno3x = 2676258;
        nibunnox3 = 3019532;
        break;
    case 601 :
        nibunno3x = 2674170;
        nibunnox3 = 3013075;
        break;
    case 602 :
        nibunno3x = 2672083;
        nibunnox3 = 3006625;
        break;
    case 603 :
        nibunno3x = 2669996;
        nibunnox3 = 3000177;
        break;
    case 604 :
        nibunno3x = 2667912;
        nibunnox3 = 2993736;
        break;
    case 605 :
        nibunno3x = 2665828;
        nibunnox3 = 2987303;
        break;
    case 606 :
        nibunno3x = 2663746;
        nibunnox3 = 2980877;
        break;
    case 607 :
        nibunno3x = 2661665;
        nibunnox3 = 2974455;
        break;
    case 608 :
        nibunno3x = 2659585;
        nibunnox3 = 2968040;
        break;
    case 609 :
        nibunno3x = 2657506;
        nibunnox3 = 2961630;
        break;
    case 610 :
        nibunno3x = 2655428;
        nibunnox3 = 2955228;
        break;
    case 611 :
        nibunno3x = 2653352;
        nibunnox3 = 2948830;
        break;
    case 612 :
        nibunno3x = 2651276;
        nibunnox3 = 2942438;
        break;
    case 613 :
        nibunno3x = 2649202;
        nibunnox3 = 2936052;
        break;
    case 614 :
        nibunno3x = 2647129;
        nibunnox3 = 2929672;
        break;
    case 615 :
        nibunno3x = 2645058;
        nibunnox3 = 2923300;
        break;
    case 616 :
        nibunno3x = 2642987;
        nibunnox3 = 2916932;
        break;
    case 617 :
        nibunno3x = 2640918;
        nibunnox3 = 2910572;
        break;
    case 618 :
        nibunno3x = 2638849;
        nibunnox3 = 2904215;
        break;
    case 619 :
        nibunno3x = 2636780;
        nibunnox3 = 2897861;
        break;
    case 620 :
        nibunno3x = 2634715;
        nibunnox3 = 2891520;
        break;
    case 621 :
        nibunno3x = 2632651;
        nibunnox3 = 2885184;
        break;
    case 622 :
        nibunno3x = 2630587;
        nibunnox3 = 2878852;
        break;
    case 623 :
        nibunno3x = 2628524;
        nibunnox3 = 2872526;
        break;
    case 624 :
        nibunno3x = 2626463;
        nibunnox3 = 2866205;
        break;
    case 625 :
        nibunno3x = 2624402;
        nibunnox3 = 2859891;
        break;
    case 626 :
        nibunno3x = 2622343;
        nibunnox3 = 2853582;
        break;
    case 627 :
        nibunno3x = 2620285;
        nibunnox3 = 2847279;
        break;
    case 628 :
        nibunno3x = 2618228;
        nibunnox3 = 2840984;
        break;
    case 629 :
        nibunno3x = 2616174;
        nibunnox3 = 2834695;
        break;
    case 630 :
        nibunno3x = 2614118;
        nibunnox3 = 2828409;
        break;
    case 631 :
        nibunno3x = 2612065;
        nibunnox3 = 2822129;
        break;
    case 632 :
        nibunno3x = 2610013;
        nibunnox3 = 2815857;
        break;
    case 633 :
        nibunno3x = 2607962;
        nibunnox3 = 2809589;
        break;
    case 634 :
        nibunno3x = 2605912;
        nibunnox3 = 2803329;
        break;
    case 635 :
        nibunno3x = 2603864;
        nibunnox3 = 2797077;
        break;
    case 636 :
        nibunno3x = 2601816;
        nibunnox3 = 2790823;
        break;
    case 637 :
        nibunno3x = 2599770;
        nibunnox3 = 2784580;
        break;
    case 638 :
        nibunno3x = 2597724;
        nibunnox3 = 2778340;
        break;
    case 639 :
        nibunno3x = 2595680;
        nibunnox3 = 2772112;
        break;
    case 640 :
        nibunno3x = 2593637;
        nibunnox3 = 2765884;
        break;
    case 641 :
        nibunno3x = 2591595;
        nibunnox3 = 2759664;
        break;
    case 642 :
        nibunno3x = 2589554;
        nibunnox3 = 2753449;
        break;
    case 643 :
        nibunno3x = 2587515;
        nibunnox3 = 2747241;
        break;
    case 644 :
        nibunno3x = 2585476;
        nibunnox3 = 2741036;
        break;
    case 645 :
        nibunno3x = 2583439;
        nibunnox3 = 2734839;
        break;
    case 646 :
        nibunno3x = 2581402;
        nibunnox3 = 2728645;
        break;
    case 647 :
        nibunno3x = 2579368;
        nibunnox3 = 2722463;
        break;
    case 648 :
        nibunno3x = 2577334;
        nibunnox3 = 2716282;
        break;
    case 649 :
        nibunno3x = 2575302;
        nibunnox3 = 2710108;
        break;
    case 650 :
        nibunno3x = 2573269;
        nibunnox3 = 2703937;
        break;
    case 651 :
        nibunno3x = 2571239;
        nibunnox3 = 2697775;
        break;
    case 652 :
        nibunno3x = 2569208;
        nibunnox3 = 2691615;
        break;
    case 653 :
        nibunno3x = 2567180;
        nibunnox3 = 2685463;
        break;
    case 654 :
        nibunno3x = 2565154;
        nibunnox3 = 2679320;
        break;
    case 655 :
        nibunno3x = 2563128;
        nibunnox3 = 2673181;
        break;
    case 656 :
        nibunno3x = 2561102;
        nibunnox3 = 2667043;
        break;
    case 657 :
        nibunno3x = 2559079;
        nibunnox3 = 2660915;
        break;
    case 658 :
        nibunno3x = 2557056;
        nibunnox3 = 2654791;
        break;
    case 659 :
        nibunno3x = 2555035;
        nibunnox3 = 2648675;
        break;
    case 660 :
        nibunno3x = 2553014;
        nibunnox3 = 2642563;
        break;
    case 661 :
        nibunno3x = 2550996;
        nibunnox3 = 2636457;
        break;
    case 662 :
        nibunno3x = 2548976;
        nibunnox3 = 2630354;
        break;
    case 663 :
        nibunno3x = 2546960;
        nibunnox3 = 2624262;
        break;
    case 664 :
        nibunno3x = 2544944;
        nibunnox3 = 2618170;
        break;
    case 665 :
        nibunno3x = 2542930;
        nibunnox3 = 2612089;
        break;
    case 666 :
        nibunno3x = 2540916;
        nibunnox3 = 2606009;
        break;
    case 667 :
        nibunno3x = 2538902;
        nibunnox3 = 2599935;
        break;
    case 668 :
        nibunno3x = 2536891;
        nibunnox3 = 2593868;
        break;
    case 669 :
        nibunno3x = 2534881;
        nibunnox3 = 2587808;
        break;
    case 670 :
        nibunno3x = 2532872;
        nibunnox3 = 2581754;
        break;
    case 671 :
        nibunno3x = 2530863;
        nibunnox3 = 2575701;
        break;
    case 672 :
        nibunno3x = 2528856;
        nibunnox3 = 2569656;
        break;
    case 673 :
        nibunno3x = 2526850;
        nibunnox3 = 2563616;
        break;
    case 674 :
        nibunno3x = 2524846;
        nibunnox3 = 2557585;
        break;
    case 675 :
        nibunno3x = 2522842;
        nibunnox3 = 2551556;
        break;
    case 676 :
        nibunno3x = 2520840;
        nibunnox3 = 2545534;
        break;
    case 677 :
        nibunno3x = 2518838;
        nibunnox3 = 2539518;
        break;
    case 678 :
        nibunno3x = 2516838;
        nibunnox3 = 2533505;
        break;
    case 679 :
        nibunno3x = 2514838;
        nibunnox3 = 2527498;
        break;
    case 680 :
        nibunno3x = 2512840;
        nibunnox3 = 2521498;
        break;
    case 681 :
        nibunno3x = 2510842;
        nibunnox3 = 2515500;
        break;
    case 682 :
        nibunno3x = 2508846;
        nibunnox3 = 2509512;
        break;
    case 683 :
        nibunno3x = 2506852;
        nibunnox3 = 2503530;
        break;
    case 684 :
        nibunno3x = 2504858;
        nibunnox3 = 2497550;
        break;
    case 685 :
        nibunno3x = 2502865;
        nibunnox3 = 2491577;
        break;
    case 686 :
        nibunno3x = 2500874;
        nibunnox3 = 2485610;
        break;
    case 687 :
        nibunno3x = 2498882;
        nibunnox3 = 2479646;
        break;
    case 688 :
        nibunno3x = 2496894;
        nibunnox3 = 2473691;
        break;
    case 689 :
        nibunno3x = 2494904;
        nibunnox3 = 2467737;
        break;
    case 690 :
        nibunno3x = 2492917;
        nibunnox3 = 2461791;
        break;
    case 691 :
        nibunno3x = 2490931;
        nibunnox3 = 2455851;
        break;
    case 692 :
        nibunno3x = 2488946;
        nibunnox3 = 2449916;
        break;
    case 693 :
        nibunno3x = 2486962;
        nibunnox3 = 2443987;
        break;
    case 694 :
        nibunno3x = 2484979;
        nibunnox3 = 2438062;
        break;
    case 695 :
        nibunno3x = 2482998;
        nibunnox3 = 2432144;
        break;
    case 696 :
        nibunno3x = 2481016;
        nibunnox3 = 2426229;
        break;
    case 697 :
        nibunno3x = 2479037;
        nibunnox3 = 2420323;
        break;
    case 698 :
        nibunno3x = 2477058;
        nibunnox3 = 2414417;
        break;
    case 699 :
        nibunno3x = 2475080;
        nibunnox3 = 2408522;
        break;
    case 700 :
        nibunno3x = 2473104;
        nibunnox3 = 2402628;
        break;
    case 701 :
        nibunno3x = 2471129;
        nibunnox3 = 2396744;
        break;
    case 702 :
        nibunno3x = 2469154;
        nibunnox3 = 2390861;
        break;
    case 703 :
        nibunno3x = 2467182;
        nibunnox3 = 2384987;
        break;
    case 704 :
        nibunno3x = 2465209;
        nibunnox3 = 2379115;
        break;
    case 705 :
        nibunno3x = 2463238;
        nibunnox3 = 2373251;
        break;
    case 706 :
        nibunno3x = 2461267;
        nibunnox3 = 2367388;
        break;
    case 707 :
        nibunno3x = 2459298;
        nibunnox3 = 2361534;
        break;
    case 708 :
        nibunno3x = 2457331;
        nibunnox3 = 2355686;
        break;
    case 709 :
        nibunno3x = 2455364;
        nibunnox3 = 2349843;
        break;
    case 710 :
        nibunno3x = 2453398;
        nibunnox3 = 2344002;
        break;
    case 711 :
        nibunno3x = 2451434;
        nibunnox3 = 2338169;
        break;
    case 712 :
        nibunno3x = 2449470;
        nibunnox3 = 2332342;
        break;
    case 713 :
        nibunno3x = 2447508;
        nibunnox3 = 2326518;
        break;
    case 714 :
        nibunno3x = 2445546;
        nibunnox3 = 2320702;
        break;
    case 715 :
        nibunno3x = 2443586;
        nibunnox3 = 2314891;
        break;
    case 716 :
        nibunno3x = 2441626;
        nibunnox3 = 2309080;
        break;
    case 717 :
        nibunno3x = 2439668;
        nibunnox3 = 2303281;
        break;
    case 718 :
        nibunno3x = 2437711;
        nibunnox3 = 2297483;
        break;
    case 719 :
        nibunno3x = 2435755;
        nibunnox3 = 2291693;
        break;
    case 720 :
        nibunno3x = 2433799;
        nibunnox3 = 2285903;
        break;
    case 721 :
        nibunno3x = 2431845;
        nibunnox3 = 2280123;
        break;
    case 722 :
        nibunno3x = 2429892;
        nibunnox3 = 2274347;
        break;
    case 723 :
        nibunno3x = 2427940;
        nibunnox3 = 2268576;
        break;
    case 724 :
        nibunno3x = 2425990;
        nibunnox3 = 2262813;
        break;
    case 725 :
        nibunno3x = 2424040;
        nibunnox3 = 2257052;
        break;
    case 726 :
        nibunno3x = 2422091;
        nibunnox3 = 2251296;
        break;
    case 727 :
        nibunno3x = 2420144;
        nibunnox3 = 2245548;
        break;
    case 728 :
        nibunno3x = 2418198;
        nibunnox3 = 2239805;
        break;
    case 729 :
        nibunno3x = 2416252;
        nibunnox3 = 2234066;
        break;
    case 730 :
        nibunno3x = 2414306;
        nibunnox3 = 2228329;
        break;
    case 731 :
        nibunno3x = 2412363;
        nibunnox3 = 2222600;
        break;
    case 732 :
        nibunno3x = 2410422;
        nibunnox3 = 2216879;
        break;
    case 733 :
        nibunno3x = 2408479;
        nibunnox3 = 2211157;
        break;
    case 734 :
        nibunno3x = 2406539;
        nibunnox3 = 2205443;
        break;
    case 735 :
        nibunno3x = 2404600;
        nibunnox3 = 2199736;
        break;
    case 736 :
        nibunno3x = 2402662;
        nibunnox3 = 2194034;
        break;
    case 737 :
        nibunno3x = 2400724;
        nibunnox3 = 2188333;
        break;
    case 738 :
        nibunno3x = 2398788;
        nibunnox3 = 2182639;
        break;
    case 739 :
        nibunno3x = 2396852;
        nibunnox3 = 2176951;
        break;
    case 740 :
        nibunno3x = 2394919;
        nibunnox3 = 2171270;
        break;
    case 741 :
        nibunno3x = 2392986;
        nibunnox3 = 2165591;
        break;
    case 742 :
        nibunno3x = 2391054;
        nibunnox3 = 2159918;
        break;
    case 743 :
        nibunno3x = 2389123;
        nibunnox3 = 2154252;
        break;
    case 744 :
        nibunno3x = 2387192;
        nibunnox3 = 2148588;
        break;
    case 745 :
        nibunno3x = 2385264;
        nibunnox3 = 2142932;
        break;
    case 746 :
        nibunno3x = 2383336;
        nibunnox3 = 2137278;
        break;
    case 747 :
        nibunno3x = 2381410;
        nibunnox3 = 2131633;
        break;
    case 748 :
        nibunno3x = 2379483;
        nibunnox3 = 2125988;
        break;
    case 749 :
        nibunno3x = 2377560;
        nibunnox3 = 2120354;
        break;
    case 750 :
        nibunno3x = 2375635;
        nibunnox3 = 2114719;
        break;
    case 751 :
        nibunno3x = 2373712;
        nibunnox3 = 2109091;
        break;
    case 752 :
        nibunno3x = 2371790;
        nibunnox3 = 2103470;
        break;
    case 753 :
        nibunno3x = 2369870;
        nibunnox3 = 2097854;
        break;
    case 754 :
        nibunno3x = 2367950;
        nibunnox3 = 2092240;
        break;
    case 755 :
        nibunno3x = 2366031;
        nibunnox3 = 2086632;
        break;
    case 756 :
        nibunno3x = 2364114;
        nibunnox3 = 2081031;
        break;
    case 757 :
        nibunno3x = 2362198;
        nibunnox3 = 2075434;
        break;
    case 758 :
        nibunno3x = 2360282;
        nibunnox3 = 2069843;
        break;
    case 759 :
        nibunno3x = 2358368;
        nibunnox3 = 2064254;
        break;
    case 760 :
        nibunno3x = 2356454;
        nibunnox3 = 2058673;
        break;
    case 761 :
        nibunno3x = 2354542;
        nibunnox3 = 2053095;
        break;
    case 762 :
        nibunno3x = 2352630;
        nibunnox3 = 2047522;
        break;
    case 763 :
        nibunno3x = 2350720;
        nibunnox3 = 2041955;
        break;
    case 764 :
        nibunno3x = 2348810;
        nibunnox3 = 2036392;
        break;
    case 765 :
        nibunno3x = 2346902;
        nibunnox3 = 2030835;
        break;
    case 766 :
        nibunno3x = 2344994;
        nibunnox3 = 2025281;
        break;
    case 767 :
        nibunno3x = 2343089;
        nibunnox3 = 2019735;
        break;
    case 768 :
        nibunno3x = 2341183;
        nibunnox3 = 2014191;
        break;
    case 769 :
        nibunno3x = 2339279;
        nibunnox3 = 2008653;
        break;
    case 770 :
        nibunno3x = 2337376;
        nibunnox3 = 2003122;
        break;
    case 771 :
        nibunno3x = 2335474;
        nibunnox3 = 1997594;
        break;
    case 772 :
        nibunno3x = 2333573;
        nibunnox3 = 1992072;
        break;
    case 773 :
        nibunno3x = 2331672;
        nibunnox3 = 1986551;
        break;
    case 774 :
        nibunno3x = 2329772;
        nibunnox3 = 1981038;
        break;
    case 775 :
        nibunno3x = 2327874;
        nibunnox3 = 1975529;
        break;
    case 776 :
        nibunno3x = 2325978;
        nibunnox3 = 1970027;
        break;
    case 777 :
        nibunno3x = 2324082;
        nibunnox3 = 1964529;
        break;
    case 778 :
        nibunno3x = 2322186;
        nibunnox3 = 1959033;
        break;
    case 779 :
        nibunno3x = 2320291;
        nibunnox3 = 1953543;
        break;
    case 780 :
        nibunno3x = 2318398;
        nibunnox3 = 1948060;
        break;
    case 781 :
        nibunno3x = 2316506;
        nibunnox3 = 1942580;
        break;
    case 782 :
        nibunno3x = 2314615;
        nibunnox3 = 1937107;
        break;
    case 783 :
        nibunno3x = 2312725;
        nibunnox3 = 1931638;
        break;
    case 784 :
        nibunno3x = 2310835;
        nibunnox3 = 1926171;
        break;
    case 785 :
        nibunno3x = 2308948;
        nibunnox3 = 1920714;
        break;
    case 786 :
        nibunno3x = 2307060;
        nibunnox3 = 1915258;
        break;
    case 787 :
        nibunno3x = 2305174;
        nibunnox3 = 1909807;
        break;
    case 788 :
        nibunno3x = 2303288;
        nibunnox3 = 1904361;
        break;
    case 789 :
        nibunno3x = 2301404;
        nibunnox3 = 1898921;
        break;
    case 790 :
        nibunno3x = 2299521;
        nibunnox3 = 1893484;
        break;
    case 791 :
        nibunno3x = 2297640;
        nibunnox3 = 1888054;
        break;
    case 792 :
        nibunno3x = 2295758;
        nibunnox3 = 1882627;
        break;
    case 793 :
        nibunno3x = 2293878;
        nibunnox3 = 1877205;
        break;
    case 794 :
        nibunno3x = 2291998;
        nibunnox3 = 1871787;
        break;
    case 795 :
        nibunno3x = 2290120;
        nibunnox3 = 1866376;
        break;
    case 796 :
        nibunno3x = 2288244;
        nibunnox3 = 1860971;
        break;
    case 797 :
        nibunno3x = 2286366;
        nibunnox3 = 1855565;
        break;
    case 798 :
        nibunno3x = 2284492;
        nibunnox3 = 1850170;
        break;
    case 799 :
        nibunno3x = 2282617;
        nibunnox3 = 1844776;
        break;
    case 800 :
        nibunno3x = 2280744;
        nibunnox3 = 1839389;
        break;
    case 801 :
        nibunno3x = 2278872;
        nibunnox3 = 1834004;
        break;
    case 802 :
        nibunno3x = 2277000;
        nibunnox3 = 1828623;
        break;
    case 803 :
        nibunno3x = 2275130;
        nibunnox3 = 1823250;
        break;
    case 804 :
        nibunno3x = 2273261;
        nibunnox3 = 1817882;
        break;
    case 805 :
        nibunno3x = 2271392;
        nibunnox3 = 1812518;
        break;
    case 806 :
        nibunno3x = 2269525;
        nibunnox3 = 1807157;
        break;
    case 807 :
        nibunno3x = 2267658;
        nibunnox3 = 1801799;
        break;
    case 808 :
        nibunno3x = 2265793;
        nibunnox3 = 1796451;
        break;
    case 809 :
        nibunno3x = 2263928;
        nibunnox3 = 1791104;
        break;
    case 810 :
        nibunno3x = 2262066;
        nibunnox3 = 1785765;
        break;
    case 811 :
        nibunno3x = 2260202;
        nibunnox3 = 1780426;
        break;
    case 812 :
        nibunno3x = 2258341;
        nibunnox3 = 1775094;
        break;
    case 813 :
        nibunno3x = 2256480;
        nibunnox3 = 1769764;
        break;
    case 814 :
        nibunno3x = 2254621;
        nibunnox3 = 1764445;
        break;
    case 815 :
        nibunno3x = 2252762;
        nibunnox3 = 1759126;
        break;
    case 816 :
        nibunno3x = 2250905;
        nibunnox3 = 1753813;
        break;
    case 817 :
        nibunno3x = 2249048;
        nibunnox3 = 1748505;
        break;
    case 818 :
        nibunno3x = 2247192;
        nibunnox3 = 1743200;
        break;
    case 819 :
        nibunno3x = 2245338;
        nibunnox3 = 1737900;
        break;
    case 820 :
        nibunno3x = 2243484;
        nibunnox3 = 1732605;
        break;
    case 821 :
        nibunno3x = 2241631;
        nibunnox3 = 1727315;
        break;
    case 822 :
        nibunno3x = 2239779;
        nibunnox3 = 1722029;
        break;
    case 823 :
        nibunno3x = 2237929;
        nibunnox3 = 1716750;
        break;
    case 824 :
        nibunno3x = 2236079;
        nibunnox3 = 1711472;
        break;
    case 825 :
        nibunno3x = 2234230;
        nibunnox3 = 1706200;
        break;
    case 826 :
        nibunno3x = 2232382;
        nibunnox3 = 1700933;
        break;
    case 827 :
        nibunno3x = 2230535;
        nibunnox3 = 1695670;
        break;
    case 828 :
        nibunno3x = 2228689;
        nibunnox3 = 1690412;
        break;
    case 829 :
        nibunno3x = 2226844;
        nibunnox3 = 1685159;
        break;
    case 830 :
        nibunno3x = 2224999;
        nibunnox3 = 1679907;
        break;
    case 831 :
        nibunno3x = 2223157;
        nibunnox3 = 1674666;
        break;
    case 832 :
        nibunno3x = 2221315;
        nibunnox3 = 1669427;
        break;
    case 833 :
        nibunno3x = 2219474;
        nibunnox3 = 1664191;
        break;
    case 834 :
        nibunno3x = 2217632;
        nibunnox3 = 1658957;
        break;
    case 835 :
        nibunno3x = 2215794;
        nibunnox3 = 1653732;
        break;
    case 836 :
        nibunno3x = 2213956;
        nibunnox3 = 1648513;
        break;
    case 837 :
        nibunno3x = 2212118;
        nibunnox3 = 1643292;
        break;
    case 838 :
        nibunno3x = 2210282;
        nibunnox3 = 1638081;
        break;
    case 839 :
        nibunno3x = 2208446;
        nibunnox3 = 1632873;
        break;
    case 840 :
        nibunno3x = 2206612;
        nibunnox3 = 1627669;
        break;
    case 841 :
        nibunno3x = 2204778;
        nibunnox3 = 1622470;
        break;
    case 842 :
        nibunno3x = 2202945;
        nibunnox3 = 1617274;
        break;
    case 843 :
        nibunno3x = 2201113;
        nibunnox3 = 1612082;
        break;
    case 844 :
        nibunno3x = 2199283;
        nibunnox3 = 1606899;
        break;
    case 845 :
        nibunno3x = 2197453;
        nibunnox3 = 1601717;
        break;
    case 846 :
        nibunno3x = 2195624;
        nibunnox3 = 1596541;
        break;
    case 847 :
        nibunno3x = 2193797;
        nibunnox3 = 1591368;
        break;
    case 848 :
        nibunno3x = 2191970;
        nibunnox3 = 1586200;
        break;
    case 849 :
        nibunno3x = 2190144;
        nibunnox3 = 1581037;
        break;
    case 850 :
        nibunno3x = 2188320;
        nibunnox3 = 1575881;
        break;
    case 851 :
        nibunno3x = 2186494;
        nibunnox3 = 1570722;
        break;
    case 852 :
        nibunno3x = 2184672;
        nibunnox3 = 1565574;
        break;
    case 853 :
        nibunno3x = 2182850;
        nibunnox3 = 1560429;
        break;
    case 854 :
        nibunno3x = 2181028;
        nibunnox3 = 1555287;
        break;
    case 855 :
        nibunno3x = 2179208;
        nibunnox3 = 1550150;
        break;
    case 856 :
        nibunno3x = 2177389;
        nibunnox3 = 1545019;
        break;
    case 857 :
        nibunno3x = 2175570;
        nibunnox3 = 1539888;
        break;
    case 858 :
        nibunno3x = 2173753;
        nibunnox3 = 1534768;
        break;
    case 859 :
        nibunno3x = 2171936;
        nibunnox3 = 1529647;
        break;
    case 860 :
        nibunno3x = 2170120;
        nibunnox3 = 1524532;
        break;
    case 861 :
        nibunno3x = 2168306;
        nibunnox3 = 1519422;
        break;
    case 862 :
        nibunno3x = 2166492;
        nibunnox3 = 1514317;
        break;
    case 863 :
        nibunno3x = 2164680;
        nibunnox3 = 1509216;
        break;
    case 864 :
        nibunno3x = 2162868;
        nibunnox3 = 1504118;
        break;
    case 865 :
        nibunno3x = 2161056;
        nibunnox3 = 1499025;
        break;
    case 866 :
        nibunno3x = 2159246;
        nibunnox3 = 1493937;
        break;
    case 867 :
        nibunno3x = 2157438;
        nibunnox3 = 1488853;
        break;
    case 868 :
        nibunno3x = 2155629;
        nibunnox3 = 1483773;
        break;
    case 869 :
        nibunno3x = 2153822;
        nibunnox3 = 1478699;
        break;
    case 870 :
        nibunno3x = 2152016;
        nibunnox3 = 1473630;
        break;
    case 871 :
        nibunno3x = 2150210;
        nibunnox3 = 1468559;
        break;
    case 872 :
        nibunno3x = 2148406;
        nibunnox3 = 1463499;
        break;
    case 873 :
        nibunno3x = 2146603;
        nibunnox3 = 1458442;
        break;
    case 874 :
        nibunno3x = 2144799;
        nibunnox3 = 1453385;
        break;
    case 875 :
        nibunno3x = 2142998;
        nibunnox3 = 1448338;
        break;
    case 876 :
        nibunno3x = 2141197;
        nibunnox3 = 1443291;
        break;
    case 877 :
        nibunno3x = 2139398;
        nibunnox3 = 1438252;
        break;
    case 878 :
        nibunno3x = 2137598;
        nibunnox3 = 1433214;
        break;
    case 879 :
        nibunno3x = 2135800;
        nibunnox3 = 1428181;
        break;
    case 880 :
        nibunno3x = 2134004;
        nibunnox3 = 1423155;
        break;
    case 881 :
        nibunno3x = 2132207;
        nibunnox3 = 1418129;
        break;
    case 882 :
        nibunno3x = 2130412;
        nibunnox3 = 1413111;
        break;
    case 883 :
        nibunno3x = 2128618;
        nibunnox3 = 1408097;
        break;
    case 884 :
        nibunno3x = 2126825;
        nibunnox3 = 1403087;
        break;
    case 885 :
        nibunno3x = 2125033;
        nibunnox3 = 1398083;
        break;
    case 886 :
        nibunno3x = 2123240;
        nibunnox3 = 1393078;
        break;
    case 887 :
        nibunno3x = 2121450;
        nibunnox3 = 1388078;
        break;
    case 888 :
        nibunno3x = 2119660;
        nibunnox3 = 1383086;
        break;
    case 889 :
        nibunno3x = 2117870;
        nibunnox3 = 1378094;
        break;
    case 890 :
        nibunno3x = 2116083;
        nibunnox3 = 1373111;
        break;
    case 891 :
        nibunno3x = 2114297;
        nibunnox3 = 1368131;
        break;
    case 892 :
        nibunno3x = 2112511;
        nibunnox3 = 1363156;
        break;
    case 893 :
        nibunno3x = 2110725;
        nibunnox3 = 1358182;
        break;
    case 894 :
        nibunno3x = 2108941;
        nibunnox3 = 1353213;
        break;
    case 895 :
        nibunno3x = 2107158;
        nibunnox3 = 1348248;
        break;
    case 896 :
        nibunno3x = 2105376;
        nibunnox3 = 1343290;
        break;
    case 897 :
        nibunno3x = 2103594;
        nibunnox3 = 1338333;
        break;
    case 898 :
        nibunno3x = 2101813;
        nibunnox3 = 1333382;
        break;
    case 899 :
        nibunno3x = 2100033;
        nibunnox3 = 1328435;
        break;
    case 900 :
        nibunno3x = 2098255;
        nibunnox3 = 1323494;
        break;
    case 901 :
        nibunno3x = 2096477;
        nibunnox3 = 1318554;
        break;
    case 902 :
        nibunno3x = 2094700;
        nibunnox3 = 1313619;
        break;
    case 903 :
        nibunno3x = 2092924;
        nibunnox3 = 1308690;
        break;
    case 904 :
        nibunno3x = 2091148;
        nibunnox3 = 1303761;
        break;
    case 905 :
        nibunno3x = 2089375;
        nibunnox3 = 1298843;
        break;
    case 906 :
        nibunno3x = 2087600;
        nibunnox3 = 1293922;
        break;
    case 907 :
        nibunno3x = 2085829;
        nibunnox3 = 1289010;
        break;
    case 908 :
        nibunno3x = 2084056;
        nibunnox3 = 1284096;
        break;
    case 909 :
        nibunno3x = 2082286;
        nibunnox3 = 1279193;
        break;
    case 910 :
        nibunno3x = 2080516;
        nibunnox3 = 1274291;
        break;
    case 911 :
        nibunno3x = 2078748;
        nibunnox3 = 1269395;
        break;
    case 912 :
        nibunno3x = 2076980;
        nibunnox3 = 1264502;
        break;
    case 913 :
        nibunno3x = 2075213;
        nibunnox3 = 1259614;
        break;
    case 914 :
        nibunno3x = 2073446;
        nibunnox3 = 1254729;
        break;
    case 915 :
        nibunno3x = 2071681;
        nibunnox3 = 1249848;
        break;
    case 916 :
        nibunno3x = 2069916;
        nibunnox3 = 1244970;
        break;
    case 917 :
        nibunno3x = 2068153;
        nibunnox3 = 1240099;
        break;
    case 918 :
        nibunno3x = 2066390;
        nibunnox3 = 1235231;
        break;
    case 919 :
        nibunno3x = 2064628;
        nibunnox3 = 1230365;
        break;
    case 920 :
        nibunno3x = 2062868;
        nibunnox3 = 1225506;
        break;
    case 921 :
        nibunno3x = 2061108;
        nibunnox3 = 1220649;
        break;
    case 922 :
        nibunno3x = 2059349;
        nibunnox3 = 1215798;
        break;
    case 923 :
        nibunno3x = 2057592;
        nibunnox3 = 1210953;
        break;
    case 924 :
        nibunno3x = 2055834;
        nibunnox3 = 1206107;
        break;
    case 925 :
        nibunno3x = 2054077;
        nibunnox3 = 1201267;
        break;
    case 926 :
        nibunno3x = 2052322;
        nibunnox3 = 1196433;
        break;
    case 927 :
        nibunno3x = 2050567;
        nibunnox3 = 1191600;
        break;
    case 928 :
        nibunno3x = 2048814;
        nibunnox3 = 1186773;
        break;
    case 929 :
        nibunno3x = 2047060;
        nibunnox3 = 1181948;
        break;
    case 930 :
        nibunno3x = 2045308;
        nibunnox3 = 1177128;
        break;
    case 931 :
        nibunno3x = 2043558;
        nibunnox3 = 1172315;
        break;
    case 932 :
        nibunno3x = 2041807;
        nibunnox3 = 1167503;
        break;
    case 933 :
        nibunno3x = 2040058;
        nibunnox3 = 1162696;
        break;
    case 934 :
        nibunno3x = 2038309;
        nibunnox3 = 1157892;
        break;
    case 935 :
        nibunno3x = 2036562;
        nibunnox3 = 1153093;
        break;
    case 936 :
        nibunno3x = 2034816;
        nibunnox3 = 1148300;
        break;
    case 937 :
        nibunno3x = 2033070;
        nibunnox3 = 1143508;
        break;
    case 938 :
        nibunno3x = 2031324;
        nibunnox3 = 1138720;
        break;
    case 939 :
        nibunno3x = 2029580;
        nibunnox3 = 1133936;
        break;
    case 940 :
        nibunno3x = 2027836;
        nibunnox3 = 1129155;
        break;
    case 941 :
        nibunno3x = 2026094;
        nibunnox3 = 1124382;
        break;
    case 942 :
        nibunno3x = 2024353;
        nibunnox3 = 1119610;
        break;
    case 943 :
        nibunno3x = 2022612;
        nibunnox3 = 1114842;
        break;
    case 944 :
        nibunno3x = 2020873;
        nibunnox3 = 1110080;
        break;
    case 945 :
        nibunno3x = 2019134;
        nibunnox3 = 1105321;
        break;
    case 946 :
        nibunno3x = 2017396;
        nibunnox3 = 1100565;
        break;
    case 947 :
        nibunno3x = 2015659;
        nibunnox3 = 1095815;
        break;
    case 948 :
        nibunno3x = 2013923;
        nibunnox3 = 1091067;
        break;
    case 949 :
        nibunno3x = 2012187;
        nibunnox3 = 1086323;
        break;
    case 950 :
        nibunno3x = 2010452;
        nibunnox3 = 1081583;
        break;
    case 951 :
        nibunno3x = 2008720;
        nibunnox3 = 1076851;
        break;
    case 952 :
        nibunno3x = 2006987;
        nibunnox3 = 1072118;
        break;
    case 953 :
        nibunno3x = 2005255;
        nibunnox3 = 1067390;
        break;
    case 954 :
        nibunno3x = 2003524;
        nibunnox3 = 1062666;
        break;
    case 955 :
        nibunno3x = 2001793;
        nibunnox3 = 1057944;
        break;
    case 956 :
        nibunno3x = 2000064;
        nibunnox3 = 1053230;
        break;
    case 957 :
        nibunno3x = 1998336;
        nibunnox3 = 1048517;
        break;
    case 958 :
        nibunno3x = 1996608;
        nibunnox3 = 1043810;
        break;
    case 959 :
        nibunno3x = 1994882;
        nibunnox3 = 1039107;
        break;
    case 960 :
        nibunno3x = 1993155;
        nibunnox3 = 1034405;
        break;
    case 961 :
        nibunno3x = 1991431;
        nibunnox3 = 1029710;
        break;
    case 962 :
        nibunno3x = 1989707;
        nibunnox3 = 1025018;
        break;
    case 963 :
        nibunno3x = 1987983;
        nibunnox3 = 1020329;
        break;
    case 964 :
        nibunno3x = 1986260;
        nibunnox3 = 1015643;
        break;
    case 965 :
        nibunno3x = 1984540;
        nibunnox3 = 1010965;
        break;
    case 966 :
        nibunno3x = 1982819;
        nibunnox3 = 1006287;
        break;
    case 967 :
        nibunno3x = 1981099;
        nibunnox3 = 1001614;
        break;
    case 968 :
        nibunno3x = 1979380;
        nibunnox3 = 996945;
        break;
    case 969 :
        nibunno3x = 1977662;
        nibunnox3 = 992279;
        break;
    case 970 :
        nibunno3x = 1975945;
        nibunnox3 = 987620;
        break;
    case 971 :
        nibunno3x = 1974229;
        nibunnox3 = 982963;
        break;
    case 972 :
        nibunno3x = 1972512;
        nibunnox3 = 978306;
        break;
    case 973 :
        nibunno3x = 1970798;
        nibunnox3 = 973660;
        break;
    case 974 :
        nibunno3x = 1969084;
        nibunnox3 = 969012;
        break;
    case 975 :
        nibunno3x = 1967371;
        nibunnox3 = 964370;
        break;
    case 976 :
        nibunno3x = 1965659;
        nibunnox3 = 959731;
        break;
    case 977 :
        nibunno3x = 1963948;
        nibunnox3 = 955099;
        break;
    case 978 :
        nibunno3x = 1962237;
        nibunnox3 = 950467;
        break;
    case 979 :
        nibunno3x = 1960527;
        nibunnox3 = 945839;
        break;
    case 980 :
        nibunno3x = 1958819;
        nibunnox3 = 941217;
        break;
    case 981 :
        nibunno3x = 1957111;
        nibunnox3 = 936598;
        break;
    case 982 :
        nibunno3x = 1955404;
        nibunnox3 = 931983;
        break;
    case 983 :
        nibunno3x = 1953697;
        nibunnox3 = 927370;
        break;
    case 984 :
        nibunno3x = 1951992;
        nibunnox3 = 922761;
        break;
    case 985 :
        nibunno3x = 1950288;
        nibunnox3 = 918159;
        break;
    case 986 :
        nibunno3x = 1948584;
        nibunnox3 = 913558;
        break;
    case 987 :
        nibunno3x = 1946881;
        nibunnox3 = 908963;
        break;
    case 988 :
        nibunno3x = 1945178;
        nibunnox3 = 904369;
        break;
    case 989 :
        nibunno3x = 1943478;
        nibunnox3 = 899783;
        break;
    case 990 :
        nibunno3x = 1941778;
        nibunnox3 = 895198;
        break;
    case 991 :
        nibunno3x = 1940078;
        nibunnox3 = 890615;
        break;
    case 992 :
        nibunno3x = 1938379;
        nibunnox3 = 886037;
        break;
    case 993 :
        nibunno3x = 1936682;
        nibunnox3 = 881466;
        break;
    case 994 :
        nibunno3x = 1934984;
        nibunnox3 = 876895;
        break;
    case 995 :
        nibunno3x = 1933288;
        nibunnox3 = 872328;
        break;
    case 996 :
        nibunno3x = 1931593;
        nibunnox3 = 867766;
        break;
    case 997 :
        nibunno3x = 1929899;
        nibunnox3 = 863208;
        break;
    case 998 :
        nibunno3x = 1928205;
        nibunnox3 = 858653;
        break;
    case 999 :
        nibunno3x = 1926512;
        nibunnox3 = 854102;
        break;
    case 1000 :
        nibunno3x = 1924820;
        nibunnox3 = 849554;
        break;
    case 1001 :
        nibunno3x = 1923129;
        nibunnox3 = 845010;
        break;
    case 1002 :
        nibunno3x = 1921440;
        nibunnox3 = 840472;
        break;
    case 1003 :
        nibunno3x = 1919749;
        nibunnox3 = 835933;
        break;
    case 1004 :
        nibunno3x = 1918061;
        nibunnox3 = 831402;
        break;
    case 1005 :
        nibunno3x = 1916372;
        nibunnox3 = 826871;
        break;
    case 1006 :
        nibunno3x = 1914685;
        nibunnox3 = 822345;
        break;
    case 1007 :
        nibunno3x = 1913000;
        nibunnox3 = 817826;
        break;
    case 1008 :
        nibunno3x = 1911314;
        nibunnox3 = 813306;
        break;
    case 1009 :
        nibunno3x = 1909630;
        nibunnox3 = 808794;
        break;
    case 1010 :
        nibunno3x = 1907946;
        nibunnox3 = 804284;
        break;
    case 1011 :
        nibunno3x = 1906264;
        nibunnox3 = 799779;
        break;
    case 1012 :
        nibunno3x = 1904581;
        nibunnox3 = 795273;
        break;
    case 1013 :
        nibunno3x = 1902900;
        nibunnox3 = 790773;
        break;
    case 1014 :
        nibunno3x = 1901220;
        nibunnox3 = 786279;
        break;
    case 1015 :
        nibunno3x = 1899540;
        nibunnox3 = 781787;
        break;
    case 1016 :
        nibunno3x = 1897862;
        nibunnox3 = 777300;
        break;
    case 1017 :
        nibunno3x = 1896183;
        nibunnox3 = 772813;
        break;
    case 1018 :
        nibunno3x = 1894506;
        nibunnox3 = 768330;
        break;
    case 1019 :
        nibunno3x = 1892830;
        nibunnox3 = 763855;
        break;
    case 1020 :
        nibunno3x = 1891154;
        nibunnox3 = 759382;
        break;
    case 1021 :
        nibunno3x = 1889480;
        nibunnox3 = 754913;
        break;
    case 1022 :
        nibunno3x = 1887806;
        nibunnox3 = 750445;
        break;
    case 1023 :
        nibunno3x = 1886133;
        nibunnox3 = 745983;
        break;
    case 1024 :
        nibunno3x = 1884461;
        nibunnox3 = 741523;
        break;
    case 1025 :
        nibunno3x = 1882789;
        nibunnox3 = 737066;
        break;
    case 1026 :
        nibunno3x = 1881120;
        nibunnox3 = 732618;
        break;
    case 1027 :
        nibunno3x = 1879449;
        nibunnox3 = 728168;
        break;
    case 1028 :
        nibunno3x = 1877780;
        nibunnox3 = 723724;
        break;
    case 1029 :
        nibunno3x = 1876112;
        nibunnox3 = 719283;
        break;
    case 1030 :
        nibunno3x = 1874444;
        nibunnox3 = 714844;
        break;
    case 1031 :
        nibunno3x = 1872778;
        nibunnox3 = 710409;
        break;
    case 1032 :
        nibunno3x = 1871113;
        nibunnox3 = 705981;
        break;
    case 1033 :
        nibunno3x = 1869448;
        nibunnox3 = 701554;
        break;
    case 1034 :
        nibunno3x = 1867784;
        nibunnox3 = 697130;
        break;
    case 1035 :
        nibunno3x = 1866121;
        nibunnox3 = 692712;
        break;
    case 1036 :
        nibunno3x = 1864458;
        nibunnox3 = 688295;
        break;
    case 1037 :
        nibunno3x = 1862797;
        nibunnox3 = 683884;
        break;
    case 1038 :
        nibunno3x = 1861136;
        nibunnox3 = 679474;
        break;
    case 1039 :
        nibunno3x = 1859476;
        nibunnox3 = 675069;
        break;
    case 1040 :
        nibunno3x = 1857817;
        nibunnox3 = 670669;
        break;
    case 1041 :
        nibunno3x = 1856158;
        nibunnox3 = 666269;
        break;
    case 1042 :
        nibunno3x = 1854500;
        nibunnox3 = 661875;
        break;
    case 1043 :
        nibunno3x = 1852844;
        nibunnox3 = 657484;
        break;
    case 1044 :
        nibunno3x = 1851188;
        nibunnox3 = 653098;
        break;
    case 1045 :
        nibunno3x = 1849532;
        nibunnox3 = 648712;
        break;
    case 1046 :
        nibunno3x = 1847878;
        nibunnox3 = 644332;
        break;
    case 1047 :
        nibunno3x = 1846225;
        nibunnox3 = 639957;
        break;
    case 1048 :
        nibunno3x = 1844572;
        nibunnox3 = 635583;
        break;
    case 1049 :
        nibunno3x = 1842920;
        nibunnox3 = 631212;
        break;
    case 1050 :
        nibunno3x = 1841269;
        nibunnox3 = 626847;
        break;
    case 1051 :
        nibunno3x = 1839619;
        nibunnox3 = 622486;
        break;
    case 1052 :
        nibunno3x = 1837968;
        nibunnox3 = 618123;
        break;
    case 1053 :
        nibunno3x = 1836320;
        nibunnox3 = 613768;
        break;
    case 1054 :
        nibunno3x = 1834672;
        nibunnox3 = 609417;
        break;
    case 1055 :
        nibunno3x = 1833025;
        nibunnox3 = 605069;
        break;
    case 1056 :
        nibunno3x = 1831378;
        nibunnox3 = 600722;
        break;
    case 1057 :
        nibunno3x = 1829732;
        nibunnox3 = 596381;
        break;
    case 1058 :
        nibunno3x = 1828088;
        nibunnox3 = 592045;
        break;
    case 1059 :
        nibunno3x = 1826444;
        nibunnox3 = 587708;
        break;
    case 1060 :
        nibunno3x = 1824800;
        nibunnox3 = 583377;
        break;
    case 1061 :
        nibunno3x = 1823158;
        nibunnox3 = 579049;
        break;
    case 1062 :
        nibunno3x = 1821517;
        nibunnox3 = 574726;
        break;
    case 1063 :
        nibunno3x = 1819876;
        nibunnox3 = 570405;
        break;
    case 1064 :
        nibunno3x = 1818235;
        nibunnox3 = 566085;
        break;
    case 1065 :
        nibunno3x = 1816596;
        nibunnox3 = 561773;
        break;
    case 1066 :
        nibunno3x = 1814958;
        nibunnox3 = 557462;
        break;
    case 1067 :
        nibunno3x = 1813320;
        nibunnox3 = 553156;
        break;
    case 1068 :
        nibunno3x = 1811684;
        nibunnox3 = 548854;
        break;
    case 1069 :
        nibunno3x = 1810048;
        nibunnox3 = 544555;
        break;
    case 1070 :
        nibunno3x = 1808412;
        nibunnox3 = 540257;
        break;
    case 1071 :
        nibunno3x = 1806778;
        nibunnox3 = 535964;
        break;
    case 1072 :
        nibunno3x = 1805144;
        nibunnox3 = 531676;
        break;
    case 1073 :
        nibunno3x = 1803511;
        nibunnox3 = 527389;
        break;
    case 1074 :
        nibunno3x = 1801879;
        nibunnox3 = 523106;
        break;
    case 1075 :
        nibunno3x = 1800247;
        nibunnox3 = 518825;
        break;
    case 1076 :
        nibunno3x = 1798616;
        nibunnox3 = 514549;
        break;
    case 1077 :
        nibunno3x = 1796988;
        nibunnox3 = 510279;
        break;
    case 1078 :
        nibunno3x = 1795358;
        nibunnox3 = 506010;
        break;
    case 1079 :
        nibunno3x = 1793730;
        nibunnox3 = 501743;
        break;
    case 1080 :
        nibunno3x = 1792102;
        nibunnox3 = 497480;
        break;
    case 1081 :
        nibunno3x = 1790476;
        nibunnox3 = 493223;
        break;
    case 1082 :
        nibunno3x = 1788849;
        nibunnox3 = 488965;
        break;
    case 1083 :
        nibunno3x = 1787225;
        nibunnox3 = 484716;
        break;
    case 1084 :
        nibunno3x = 1785600;
        nibunnox3 = 480464;
        break;
    case 1085 :
        nibunno3x = 1783976;
        nibunnox3 = 476221;
        break;
    case 1086 :
        nibunno3x = 1782354;
        nibunnox3 = 471978;
        break;
    case 1087 :
        nibunno3x = 1780732;
        nibunnox3 = 467741;
        break;
    case 1088 :
        nibunno3x = 1779110;
        nibunnox3 = 463505;
        break;
    case 1089 :
        nibunno3x = 1777489;
        nibunnox3 = 459271;
        break;
    case 1090 :
        nibunno3x = 1775870;
        nibunnox3 = 455045;
        break;
    case 1091 :
        nibunno3x = 1774251;
        nibunnox3 = 450820;
        break;
    case 1092 :
        nibunno3x = 1772633;
        nibunnox3 = 446597;
        break;
    case 1093 :
        nibunno3x = 1771015;
        nibunnox3 = 442378;
        break;
    case 1094 :
        nibunno3x = 1769400;
        nibunnox3 = 438166;
        break;
    case 1095 :
        nibunno3x = 1767782;
        nibunnox3 = 433952;
        break;
    case 1096 :
        nibunno3x = 1766168;
        nibunnox3 = 429744;
        break;
    case 1097 :
        nibunno3x = 1764554;
        nibunnox3 = 425540;
        break;
    case 1098 :
        nibunno3x = 1762940;
        nibunnox3 = 421339;
        break;
    case 1099 :
        nibunno3x = 1761327;
        nibunnox3 = 417140;
        break;
    case 1100 :
        nibunno3x = 1759716;
        nibunnox3 = 412945;
        break;
    case 1101 :
        nibunno3x = 1758104;
        nibunnox3 = 408754;
        break;
    case 1102 :
        nibunno3x = 1756494;
        nibunnox3 = 404567;
        break;
    case 1103 :
        nibunno3x = 1754884;
        nibunnox3 = 400380;
        break;
    case 1104 :
        nibunno3x = 1753274;
        nibunnox3 = 396198;
        break;
    case 1105 :
        nibunno3x = 1751667;
        nibunnox3 = 392021;
        break;
    case 1106 :
        nibunno3x = 1750059;
        nibunnox3 = 387845;
        break;
    case 1107 :
        nibunno3x = 1748454;
        nibunnox3 = 383675;
        break;
    case 1108 :
        nibunno3x = 1746847;
        nibunnox3 = 379506;
        break;
    case 1109 :
        nibunno3x = 1745242;
        nibunnox3 = 375341;
        break;
    case 1110 :
        nibunno3x = 1743637;
        nibunnox3 = 371177;
        break;
    case 1111 :
        nibunno3x = 1742034;
        nibunnox3 = 367019;
        break;
    case 1112 :
        nibunno3x = 1740432;
        nibunnox3 = 362866;
        break;
    case 1113 :
        nibunno3x = 1738830;
        nibunnox3 = 358714;
        break;
    case 1114 :
        nibunno3x = 1737228;
        nibunnox3 = 354564;
        break;
    case 1115 :
        nibunno3x = 1735627;
        nibunnox3 = 350419;
        break;
    case 1116 :
        nibunno3x = 1734027;
        nibunnox3 = 346277;
        break;
    case 1117 :
        nibunno3x = 1732429;
        nibunnox3 = 342140;
        break;
    case 1118 :
        nibunno3x = 1730830;
        nibunnox3 = 338003;
        break;
    case 1119 :
        nibunno3x = 1729232;
        nibunnox3 = 333870;
        break;
    case 1120 :
        nibunno3x = 1727636;
        nibunnox3 = 329743;
        break;
    case 1121 :
        nibunno3x = 1726040;
        nibunnox3 = 325618;
        break;
    case 1122 :
        nibunno3x = 1724444;
        nibunnox3 = 321493;
        break;
    case 1123 :
        nibunno3x = 1722850;
        nibunnox3 = 317374;
        break;
    case 1124 :
        nibunno3x = 1721256;
        nibunnox3 = 313258;
        break;
    case 1125 :
        nibunno3x = 1719664;
        nibunnox3 = 309147;
        break;
    case 1126 :
        nibunno3x = 1718071;
        nibunnox3 = 305035;
        break;
    case 1127 :
        nibunno3x = 1716480;
        nibunnox3 = 300929;
        break;
    case 1128 :
        nibunno3x = 1714890;
        nibunnox3 = 296828;
        break;
    case 1129 :
        nibunno3x = 1713299;
        nibunnox3 = 292726;
        break;
    case 1130 :
        nibunno3x = 1711710;
        nibunnox3 = 288630;
        break;
    case 1131 :
        nibunno3x = 1710121;
        nibunnox3 = 284536;
        break;
    case 1132 :
        nibunno3x = 1708533;
        nibunnox3 = 280446;
        break;
    case 1133 :
        nibunno3x = 1706947;
        nibunnox3 = 276361;
        break;
    case 1134 :
        nibunno3x = 1705360;
        nibunnox3 = 272275;
        break;
    case 1135 :
        nibunno3x = 1703775;
        nibunnox3 = 268196;
        break;
    case 1136 :
        nibunno3x = 1702190;
        nibunnox3 = 264117;
        break;
    case 1137 :
        nibunno3x = 1700606;
        nibunnox3 = 260045;
        break;
    case 1138 :
        nibunno3x = 1699022;
        nibunnox3 = 255972;
        break;
    case 1139 :
        nibunno3x = 1697441;
        nibunnox3 = 251907;
        break;
    case 1140 :
        nibunno3x = 1695858;
        nibunnox3 = 247840;
        break;
    case 1141 :
        nibunno3x = 1694278;
        nibunnox3 = 243781;
        break;
    case 1142 :
        nibunno3x = 1692697;
        nibunnox3 = 239721;
        break;
    case 1143 :
        nibunno3x = 1691118;
        nibunnox3 = 235666;
        break;
    case 1144 :
        nibunno3x = 1689539;
        nibunnox3 = 231614;
        break;
    case 1145 :
        nibunno3x = 1687961;
        nibunnox3 = 227566;
        break;
    case 1146 :
        nibunno3x = 1686384;
        nibunnox3 = 223520;
        break;
    case 1147 :
        nibunno3x = 1684807;
        nibunnox3 = 219478;
        break;
    case 1148 :
        nibunno3x = 1683232;
        nibunnox3 = 215441;
        break;
    case 1149 :
        nibunno3x = 1681656;
        nibunnox3 = 211403;
        break;
    case 1150 :
        nibunno3x = 1680082;
        nibunnox3 = 207370;
        break;
    case 1151 :
        nibunno3x = 1678508;
        nibunnox3 = 203341;
        break;
    case 1152 :
        nibunno3x = 1676935;
        nibunnox3 = 199313;
        break;
    case 1153 :
        nibunno3x = 1675363;
        nibunnox3 = 195290;
        break;
    case 1154 :
        nibunno3x = 1673792;
        nibunnox3 = 191272;
        break;
    case 1155 :
        nibunno3x = 1672222;
        nibunnox3 = 187255;
        break;
    case 1156 :
        nibunno3x = 1670652;
        nibunnox3 = 183240;
        break;
    case 1157 :
        nibunno3x = 1669082;
        nibunnox3 = 179229;
        break;
    case 1158 :
        nibunno3x = 1667514;
        nibunnox3 = 175220;
        break;
    case 1159 :
        nibunno3x = 1665946;
        nibunnox3 = 171217;
        break;
    case 1160 :
        nibunno3x = 1664378;
        nibunnox3 = 167214;
        break;
    case 1161 :
        nibunno3x = 1662812;
        nibunnox3 = 163216;
        break;
    case 1162 :
        nibunno3x = 1661247;
        nibunnox3 = 159221;
        break;
    case 1163 :
        nibunno3x = 1659682;
        nibunnox3 = 155228;
        break;
    case 1164 :
        nibunno3x = 1658119;
        nibunnox3 = 151242;
        break;
    case 1165 :
        nibunno3x = 1656554;
        nibunnox3 = 147252;
        break;
    case 1166 :
        nibunno3x = 1654992;
        nibunnox3 = 143271;
        break;
    case 1167 :
        nibunno3x = 1653432;
        nibunnox3 = 139294;
        break;
    case 1168 :
        nibunno3x = 1651869;
        nibunnox3 = 135314;
        break;
    case 1169 :
        nibunno3x = 1650310;
        nibunnox3 = 131344;
        break;
    case 1170 :
        nibunno3x = 1648749;
        nibunnox3 = 127371;
        break;
    case 1171 :
        nibunno3x = 1647190;
        nibunnox3 = 123403;
        break;
    case 1172 :
        nibunno3x = 1645632;
        nibunnox3 = 119437;
        break;
    case 1173 :
        nibunno3x = 1644074;
        nibunnox3 = 115478;
        break;
    case 1174 :
        nibunno3x = 1642518;
        nibunnox3 = 111521;
        break;
    case 1175 :
        nibunno3x = 1640962;
        nibunnox3 = 107565;
        break;
    case 1176 :
        nibunno3x = 1639406;
        nibunnox3 = 103613;
        break;
    case 1177 :
        nibunno3x = 1637852;
        nibunnox3 = 99666;
        break;
    case 1178 :
        nibunno3x = 1636298;
        nibunnox3 = 95719;
        break;
    case 1179 :
        nibunno3x = 1634744;
        nibunnox3 = 91774;
        break;
    case 1180 :
        nibunno3x = 1633192;
        nibunnox3 = 87834;
        break;
    case 1181 :
        nibunno3x = 1631641;
        nibunnox3 = 83899;
        break;
    case 1182 :
        nibunno3x = 1630090;
        nibunnox3 = 79966;
        break;
    case 1183 :
        nibunno3x = 1628540;
        nibunnox3 = 76035;
        break;
    case 1184 :
        nibunno3x = 1626990;
        nibunnox3 = 72108;
        break;
    case 1185 :
        nibunno3x = 1625441;
        nibunnox3 = 68181;
        break;
    case 1186 :
        nibunno3x = 1623893;
        nibunnox3 = 64260;
        break;
    case 1187 :
        nibunno3x = 1622346;
        nibunnox3 = 60342;
        break;
    case 1188 :
        nibunno3x = 1620799;
        nibunnox3 = 56427;
        break;
    case 1189 :
        nibunno3x = 1619252;
        nibunnox3 = 52513;
        break;
    case 1190 :
        nibunno3x = 1617708;
        nibunnox3 = 48605;
        break;
    case 1191 :
        nibunno3x = 1616163;
        nibunnox3 = 44699;
        break;
    case 1192 :
        nibunno3x = 1614620;
        nibunnox3 = 40796;
        break;
    case 1193 :
        nibunno3x = 1613076;
        nibunnox3 = 36895;
        break;
    case 1194 :
        nibunno3x = 1611534;
        nibunnox3 = 32997;
        break;
    case 1195 :
        nibunno3x = 1609992;
        nibunnox3 = 29103;
        break;
    case 1196 :
        nibunno3x = 1608452;
        nibunnox3 = 25213;
        break;
    case 1197 :
        nibunno3x = 1606910;
        nibunnox3 = 21322;
        break;
    case 1198 :
        nibunno3x = 1605372;
        nibunnox3 = 17438;
        break;
    case 1199 :
        nibunno3x = 1603832;
        nibunnox3 = 13556;
        break;
    case 1200 :
        nibunno3x = 1602295;
        nibunnox3 = 9678;
        break;
    case 1201 :
        nibunno3x = 1600757;
        nibunnox3 = 5799;
        break;
    case 1202 :
        nibunno3x = 1599220;
        nibunnox3 = 1925;
        break;
    case 1203 :
        nibunno3x = 1597684;
        nibunnox3 = 8384718;
        break;
    case 1204 :
        nibunno3x = 1596150;
        nibunnox3 = 8376988;
        break;
    case 1205 :
        nibunno3x = 1594614;
        nibunnox3 = 8369254;
        break;
    case 1206 :
        nibunno3x = 1593080;
        nibunnox3 = 8361533;
        break;
    case 1207 :
        nibunno3x = 1591548;
        nibunnox3 = 8353816;
        break;
    case 1208 :
        nibunno3x = 1590014;
        nibunnox3 = 8346102;
        break;
    case 1209 :
        nibunno3x = 1588483;
        nibunnox3 = 8338397;
        break;
    case 1210 :
        nibunno3x = 1586952;
        nibunnox3 = 8330696;
        break;
    case 1211 :
        nibunno3x = 1585422;
        nibunnox3 = 8323004;
        break;
    case 1212 :
        nibunno3x = 1583893;
        nibunnox3 = 8315322;
        break;
    case 1213 :
        nibunno3x = 1582363;
        nibunnox3 = 8307635;
        break;
    case 1214 :
        nibunno3x = 1580834;
        nibunnox3 = 8299958;
        break;
    case 1215 :
        nibunno3x = 1579308;
        nibunnox3 = 8292290;
        break;
    case 1216 :
        nibunno3x = 1577780;
        nibunnox3 = 8284626;
        break;
    case 1217 :
        nibunno3x = 1576254;
        nibunnox3 = 8276968;
        break;
    case 1218 :
        nibunno3x = 1574728;
        nibunnox3 = 8269312;
        break;
    case 1219 :
        nibunno3x = 1573204;
        nibunnox3 = 8261668;
        break;
    case 1220 :
        nibunno3x = 1571680;
        nibunnox3 = 8254028;
        break;
    case 1221 :
        nibunno3x = 1570157;
        nibunnox3 = 8246394;
        break;
    case 1222 :
        nibunno3x = 1568634;
        nibunnox3 = 8238761;
        break;
    case 1223 :
        nibunno3x = 1567112;
        nibunnox3 = 8231143;
        break;
    case 1224 :
        nibunno3x = 1565592;
        nibunnox3 = 8223527;
        break;
    case 1225 :
        nibunno3x = 1564070;
        nibunnox3 = 8215912;
        break;
    case 1226 :
        nibunno3x = 1562551;
        nibunnox3 = 8208308;
        break;
    case 1227 :
        nibunno3x = 1561032;
        nibunnox3 = 8200707;
        break;
    case 1228 :
        nibunno3x = 1559514;
        nibunnox3 = 8193116;
        break;
    case 1229 :
        nibunno3x = 1557996;
        nibunnox3 = 8185525;
        break;
    case 1230 :
        nibunno3x = 1556478;
        nibunnox3 = 8177941;
        break;
    case 1231 :
        nibunno3x = 1554962;
        nibunnox3 = 8170368;
        break;
    case 1232 :
        nibunno3x = 1553446;
        nibunnox3 = 8162794;
        break;
    case 1233 :
        nibunno3x = 1551931;
        nibunnox3 = 8155228;
        break;
    case 1234 :
        nibunno3x = 1550418;
        nibunnox3 = 8147672;
        break;
    case 1235 :
        nibunno3x = 1548902;
        nibunnox3 = 8140112;
        break;
    case 1236 :
        nibunno3x = 1547390;
        nibunnox3 = 8132568;
        break;
    case 1237 :
        nibunno3x = 1545878;
        nibunnox3 = 8125027;
        break;
    case 1238 :
        nibunno3x = 1544366;
        nibunnox3 = 8117489;
        break;
    case 1239 :
        nibunno3x = 1542856;
        nibunnox3 = 8109959;
        break;
    case 1240 :
        nibunno3x = 1541346;
        nibunnox3 = 8102432;
        break;
    case 1241 :
        nibunno3x = 1539836;
        nibunnox3 = 8094916;
        break;
    case 1242 :
        nibunno3x = 1538328;
        nibunnox3 = 8087401;
        break;
    case 1243 :
        nibunno3x = 1536820;
        nibunnox3 = 8079896;
        break;
    case 1244 :
        nibunno3x = 1535311;
        nibunnox3 = 8072386;
        break;
    case 1245 :
        nibunno3x = 1533805;
        nibunnox3 = 8064893;
        break;
    case 1246 :
        nibunno3x = 1532299;
        nibunnox3 = 8057402;
        break;
    case 1247 :
        nibunno3x = 1530794;
        nibunnox3 = 8049918;
        break;
    case 1248 :
        nibunno3x = 1529289;
        nibunnox3 = 8042438;
        break;
    case 1249 :
        nibunno3x = 1527786;
        nibunnox3 = 8034966;
        break;
    case 1250 :
        nibunno3x = 1526282;
        nibunnox3 = 8027499;
        break;
    case 1251 :
        nibunno3x = 1524780;
        nibunnox3 = 8020034;
        break;
    case 1252 :
        nibunno3x = 1523278;
        nibunnox3 = 8012580;
        break;
    case 1253 :
        nibunno3x = 1521776;
        nibunnox3 = 8005127;
        break;
    case 1254 :
        nibunno3x = 1520276;
        nibunnox3 = 7997682;
        break;
    case 1255 :
        nibunno3x = 1518776;
        nibunnox3 = 7990241;
        break;
    case 1256 :
        nibunno3x = 1517276;
        nibunnox3 = 7982806;
        break;
    case 1257 :
        nibunno3x = 1515778;
        nibunnox3 = 7975378;
        break;
    case 1258 :
        nibunno3x = 1514280;
        nibunnox3 = 7967955;
        break;
    case 1259 :
        nibunno3x = 1512784;
        nibunnox3 = 7960542;
        break;
    case 1260 :
        nibunno3x = 1511287;
        nibunnox3 = 7953128;
        break;
    case 1261 :
        nibunno3x = 1509791;
        nibunnox3 = 7945719;
        break;
    case 1262 :
        nibunno3x = 1508296;
        nibunnox3 = 7938321;
        break;
    case 1263 :
        nibunno3x = 1506802;
        nibunnox3 = 7930927;
        break;
    case 1264 :
        nibunno3x = 1505307;
        nibunnox3 = 7923533;
        break;
    case 1265 :
        nibunno3x = 1503814;
        nibunnox3 = 7916148;
        break;
    case 1266 :
        nibunno3x = 1502322;
        nibunnox3 = 7908769;
        break;
    case 1267 :
        nibunno3x = 1500830;
        nibunnox3 = 7901400;
        break;
    case 1268 :
        nibunno3x = 1499340;
        nibunnox3 = 7894035;
        break;
    case 1269 :
        nibunno3x = 1497848;
        nibunnox3 = 7886669;
        break;
    case 1270 :
        nibunno3x = 1496359;
        nibunnox3 = 7879315;
        break;
    case 1271 :
        nibunno3x = 1494870;
        nibunnox3 = 7871965;
        break;
    case 1272 :
        nibunno3x = 1493382;
        nibunnox3 = 7864618;
        break;
    case 1273 :
        nibunno3x = 1491894;
        nibunnox3 = 7857277;
        break;
    case 1274 :
        nibunno3x = 1490407;
        nibunnox3 = 7849946;
        break;
    case 1275 :
        nibunno3x = 1488920;
        nibunnox3 = 7842617;
        break;
    case 1276 :
        nibunno3x = 1487435;
        nibunnox3 = 7835294;
        break;
    case 1277 :
        nibunno3x = 1485950;
        nibunnox3 = 7827980;
        break;
    case 1278 :
        nibunno3x = 1484466;
        nibunnox3 = 7820664;
        break;
    case 1279 :
        nibunno3x = 1482982;
        nibunnox3 = 7813359;
        break;
    case 1280 :
        nibunno3x = 1481499;
        nibunnox3 = 7806060;
        break;
    case 1281 :
        nibunno3x = 1480017;
        nibunnox3 = 7798766;
        break;
    case 1282 :
        nibunno3x = 1478535;
        nibunnox3 = 7791473;
        break;
    case 1283 :
        nibunno3x = 1477054;
        nibunnox3 = 7784188;
        break;
    case 1284 :
        nibunno3x = 1475574;
        nibunnox3 = 7776913;
        break;
    case 1285 :
        nibunno3x = 1474094;
        nibunnox3 = 7769634;
        break;
    case 1286 :
        nibunno3x = 1472616;
        nibunnox3 = 7762369;
        break;
    case 1287 :
        nibunno3x = 1471136;
        nibunnox3 = 7755104;
        break;
    case 1288 :
        nibunno3x = 1469659;
        nibunnox3 = 7747848;
        break;
    case 1289 :
        nibunno3x = 1468183;
        nibunnox3 = 7740600;
        break;
    case 1290 :
        nibunno3x = 1466706;
        nibunnox3 = 7733349;
        break;
    case 1291 :
        nibunno3x = 1465230;
        nibunnox3 = 7726110;
        break;
    case 1292 :
        nibunno3x = 1463755;
        nibunnox3 = 7718872;
        break;
    case 1293 :
        nibunno3x = 1462280;
        nibunnox3 = 7711642;
        break;
    case 1294 :
        nibunno3x = 1460808;
        nibunnox3 = 7704421;
        break;
    case 1295 :
        nibunno3x = 1459334;
        nibunnox3 = 7697198;
        break;
    case 1296 :
        nibunno3x = 1457862;
        nibunnox3 = 7689984;
        break;
    case 1297 :
        nibunno3x = 1456390;
        nibunnox3 = 7682777;
        break;
    case 1298 :
        nibunno3x = 1454919;
        nibunnox3 = 7675576;
        break;
    case 1299 :
        nibunno3x = 1453448;
        nibunnox3 = 7668376;
        break;
    case 1300 :
        nibunno3x = 1451978;
        nibunnox3 = 7661183;
        break;
    case 1301 :
        nibunno3x = 1450510;
        nibunnox3 = 7653999;
        break;
    case 1302 :
        nibunno3x = 1449042;
        nibunnox3 = 7646817;
        break;
    case 1303 :
        nibunno3x = 1447573;
        nibunnox3 = 7639636;
        break;
    case 1304 :
        nibunno3x = 1446106;
        nibunnox3 = 7632465;
        break;
    case 1305 :
        nibunno3x = 1444640;
        nibunnox3 = 7625304;
        break;
    case 1306 :
        nibunno3x = 1443174;
        nibunnox3 = 7618142;
        break;
    case 1307 :
        nibunno3x = 1441710;
        nibunnox3 = 7610989;
        break;
    case 1308 :
        nibunno3x = 1440245;
        nibunnox3 = 7603839;
        break;
    case 1309 :
        nibunno3x = 1438781;
        nibunnox3 = 7596694;
        break;
    case 1310 :
        nibunno3x = 1437318;
        nibunnox3 = 7589553;
        break;
    case 1311 :
        nibunno3x = 1435855;
        nibunnox3 = 7582420;
        break;
    case 1312 :
        nibunno3x = 1434393;
        nibunnox3 = 7575293;
        break;
    case 1313 :
        nibunno3x = 1432932;
        nibunnox3 = 7568167;
        break;
    case 1314 :
        nibunno3x = 1431471;
        nibunnox3 = 7561051;
        break;
    case 1315 :
        nibunno3x = 1430012;
        nibunnox3 = 7553941;
        break;
    case 1316 :
        nibunno3x = 1428552;
        nibunnox3 = 7546832;
        break;
    case 1317 :
        nibunno3x = 1427094;
        nibunnox3 = 7539729;
        break;
    case 1318 :
        nibunno3x = 1425636;
        nibunnox3 = 7532632;
        break;
    case 1319 :
        nibunno3x = 1424179;
        nibunnox3 = 7525545;
        break;
    case 1320 :
        nibunno3x = 1422722;
        nibunnox3 = 7518456;
        break;
    case 1321 :
        nibunno3x = 1421266;
        nibunnox3 = 7511376;
        break;
    case 1322 :
        nibunno3x = 1419810;
        nibunnox3 = 7504299;
        break;
    case 1323 :
        nibunno3x = 1418356;
        nibunnox3 = 7497231;
        break;
    case 1324 :
        nibunno3x = 1416901;
        nibunnox3 = 7490161;
        break;
    case 1325 :
        nibunno3x = 1415448;
        nibunnox3 = 7483105;
        break;
    case 1326 :
        nibunno3x = 1413996;
        nibunnox3 = 7476050;
        break;
    case 1327 :
        nibunno3x = 1412543;
        nibunnox3 = 7468998;
        break;
    case 1328 :
        nibunno3x = 1411092;
        nibunnox3 = 7461954;
        break;
    case 1329 :
        nibunno3x = 1409641;
        nibunnox3 = 7454917;
        break;
    case 1330 :
        nibunno3x = 1408190;
        nibunnox3 = 7447883;
        break;
    case 1331 :
        nibunno3x = 1406742;
        nibunnox3 = 7440856;
        break;
    case 1332 :
        nibunno3x = 1405292;
        nibunnox3 = 7433833;
        break;
    case 1333 :
        nibunno3x = 1403844;
        nibunnox3 = 7426811;
        break;
    case 1334 :
        nibunno3x = 1402396;
        nibunnox3 = 7419798;
        break;
    case 1335 :
        nibunno3x = 1400949;
        nibunnox3 = 7412792;
        break;
    case 1336 :
        nibunno3x = 1399503;
        nibunnox3 = 7405791;
        break;
    case 1337 :
        nibunno3x = 1398058;
        nibunnox3 = 7398795;
        break;
    case 1338 :
        nibunno3x = 1396612;
        nibunnox3 = 7391798;
        break;
    case 1339 :
        nibunno3x = 1395168;
        nibunnox3 = 7384811;
        break;
    case 1340 :
        nibunno3x = 1393724;
        nibunnox3 = 7377833;
        break;
    case 1341 :
        nibunno3x = 1392281;
        nibunnox3 = 7370853;
        break;
    case 1342 :
        nibunno3x = 1390838;
        nibunnox3 = 7363883;
        break;
    case 1343 :
        nibunno3x = 1389397;
        nibunnox3 = 7356919;
        break;
    case 1344 :
        nibunno3x = 1387956;
        nibunnox3 = 7349956;
        break;
    case 1345 :
        nibunno3x = 1386514;
        nibunnox3 = 7342994;
        break;
    case 1346 :
        nibunno3x = 1385075;
        nibunnox3 = 7336047;
        break;
    case 1347 :
        nibunno3x = 1383636;
        nibunnox3 = 7329101;
        break;
    case 1348 :
        nibunno3x = 1382197;
        nibunnox3 = 7322161;
        break;
    case 1349 :
        nibunno3x = 1380758;
        nibunnox3 = 7315224;
        break;
    case 1350 :
        nibunno3x = 1379322;
        nibunnox3 = 7308295;
        break;
    case 1351 :
        nibunno3x = 1377884;
        nibunnox3 = 7301368;
        break;
    case 1352 :
        nibunno3x = 1376448;
        nibunnox3 = 7294447;
        break;
    case 1353 :
        nibunno3x = 1375014;
        nibunnox3 = 7287536;
        break;
    case 1354 :
        nibunno3x = 1373578;
        nibunnox3 = 7280622;
        break;
    case 1355 :
        nibunno3x = 1372144;
        nibunnox3 = 7273718;
        break;
    case 1356 :
        nibunno3x = 1370710;
        nibunnox3 = 7266816;
        break;
    case 1357 :
        nibunno3x = 1369278;
        nibunnox3 = 7259922;
        break;
    case 1358 :
        nibunno3x = 1367845;
        nibunnox3 = 7253032;
        break;
    case 1359 :
        nibunno3x = 1366412;
        nibunnox3 = 7246142;
        break;
    case 1360 :
        nibunno3x = 1364982;
        nibunnox3 = 7239267;
        break;
    case 1361 :
        nibunno3x = 1363552;
        nibunnox3 = 7232394;
        break;
    case 1362 :
        nibunno3x = 1362122;
        nibunnox3 = 7225522;
        break;
    case 1363 :
        nibunno3x = 1360693;
        nibunnox3 = 7218659;
        break;
    case 1364 :
        nibunno3x = 1359264;
        nibunnox3 = 7211798;
        break;
    case 1365 :
        nibunno3x = 1357836;
        nibunnox3 = 7204940;
        break;
    case 1366 :
        nibunno3x = 1356409;
        nibunnox3 = 7198093;
        break;
    case 1367 :
        nibunno3x = 1354982;
        nibunnox3 = 7191249;
        break;
    case 1368 :
        nibunno3x = 1353556;
        nibunnox3 = 7184408;
        break;
    case 1369 :
        nibunno3x = 1352130;
        nibunnox3 = 7177572;
        break;
    case 1370 :
        nibunno3x = 1350706;
        nibunnox3 = 7170745;
        break;
    case 1371 :
        nibunno3x = 1349281;
        nibunnox3 = 7163916;
        break;
    case 1372 :
        nibunno3x = 1347858;
        nibunnox3 = 7157096;
        break;
    case 1373 :
        nibunno3x = 1346435;
        nibunnox3 = 7150283;
        break;
    case 1374 :
        nibunno3x = 1345013;
        nibunnox3 = 7143474;
        break;
    case 1375 :
        nibunno3x = 1343592;
        nibunnox3 = 7136671;
        break;
    case 1376 :
        nibunno3x = 1342170;
        nibunnox3 = 7129867;
        break;
    case 1377 :
        nibunno3x = 1340750;
        nibunnox3 = 7123075;
        break;
    case 1378 :
        nibunno3x = 1339329;
        nibunnox3 = 7116282;
        break;
    case 1379 :
        nibunno3x = 1337910;
        nibunnox3 = 7109498;
        break;
    case 1380 :
        nibunno3x = 1336492;
        nibunnox3 = 7102720;
        break;
    case 1381 :
        nibunno3x = 1335074;
        nibunnox3 = 7095947;
        break;
    case 1382 :
        nibunno3x = 1333657;
        nibunnox3 = 7089176;
        break;
    case 1383 :
        nibunno3x = 1332240;
        nibunnox3 = 7082406;
        break;
    case 1384 :
        nibunno3x = 1330824;
        nibunnox3 = 7075646;
        break;
    case 1385 :
        nibunno3x = 1329408;
        nibunnox3 = 7068893;
        break;
    case 1386 :
        nibunno3x = 1327993;
        nibunnox3 = 7062140;
        break;
    case 1387 :
        nibunno3x = 1326580;
        nibunnox3 = 7055402;
        break;
    case 1388 :
        nibunno3x = 1325166;
        nibunnox3 = 7048655;
        break;
    case 1389 :
        nibunno3x = 1323752;
        nibunnox3 = 7041921;
        break;
    case 1390 :
        nibunno3x = 1322340;
        nibunnox3 = 7035190;
        break;
    case 1391 :
        nibunno3x = 1320928;
        nibunnox3 = 7028462;
        break;
    case 1392 :
        nibunno3x = 1319518;
        nibunnox3 = 7021747;
        break;
    case 1393 :
        nibunno3x = 1318106;
        nibunnox3 = 7015026;
        break;
    case 1394 :
        nibunno3x = 1316696;
        nibunnox3 = 7008315;
        break;
    case 1395 :
        nibunno3x = 1315287;
        nibunnox3 = 7001608;
        break;
    case 1396 :
        nibunno3x = 1313879;
        nibunnox3 = 6994908;
        break;
    case 1397 :
        nibunno3x = 1312470;
        nibunnox3 = 6988209;
        break;
    case 1398 :
        nibunno3x = 1311062;
        nibunnox3 = 6981516;
        break;
    case 1399 :
        nibunno3x = 1309657;
        nibunnox3 = 6974835;
        break;
    case 1400 :
        nibunno3x = 1308250;
        nibunnox3 = 6968149;
        break;
    case 1401 :
        nibunno3x = 1306844;
        nibunnox3 = 6961473;
        break;
    case 1402 :
        nibunno3x = 1305440;
        nibunnox3 = 6954802;
        break;
    case 1403 :
        nibunno3x = 1304035;
        nibunnox3 = 6948132;
        break;
    case 1404 :
        nibunno3x = 1302631;
        nibunnox3 = 6941468;
        break;
    case 1405 :
        nibunno3x = 1301227;
        nibunnox3 = 6934807;
        break;
    case 1406 :
        nibunno3x = 1299825;
        nibunnox3 = 6928158;
        break;
    case 1407 :
        nibunno3x = 1298424;
        nibunnox3 = 6921511;
        break;
    case 1408 :
        nibunno3x = 1297022;
        nibunnox3 = 6914869;
        break;
    case 1409 :
        nibunno3x = 1295622;
        nibunnox3 = 6908229;
        break;
    case 1410 :
        nibunno3x = 1294220;
        nibunnox3 = 6901591;
        break;
    case 1411 :
        nibunno3x = 1292821;
        nibunnox3 = 6894961;
        break;
    case 1412 :
        nibunno3x = 1291422;
        nibunnox3 = 6888339;
        break;
    case 1413 :
        nibunno3x = 1290024;
        nibunnox3 = 6881721;
        break;
    case 1414 :
        nibunno3x = 1288626;
        nibunnox3 = 6875105;
        break;
    case 1415 :
        nibunno3x = 1287229;
        nibunnox3 = 6868494;
        break;
    case 1416 :
        nibunno3x = 1285832;
        nibunnox3 = 6861889;
        break;
    case 1417 :
        nibunno3x = 1284437;
        nibunnox3 = 6855288;
        break;
    case 1418 :
        nibunno3x = 1283041;
        nibunnox3 = 6848691;
        break;
    case 1419 :
        nibunno3x = 1281647;
        nibunnox3 = 6842103;
        break;
    case 1420 :
        nibunno3x = 1280252;
        nibunnox3 = 6835516;
        break;
    case 1421 :
        nibunno3x = 1278859;
        nibunnox3 = 6828934;
        break;
    case 1422 :
        nibunno3x = 1277466;
        nibunnox3 = 6822354;
        break;
    case 1423 :
        nibunno3x = 1276074;
        nibunnox3 = 6815784;
        break;
    case 1424 :
        nibunno3x = 1274682;
        nibunnox3 = 6809215;
        break;
    case 1425 :
        nibunno3x = 1273291;
        nibunnox3 = 6802656;
        break;
    case 1426 :
        nibunno3x = 1271900;
        nibunnox3 = 6796098;
        break;
    case 1427 :
        nibunno3x = 1270510;
        nibunnox3 = 6789541;
        break;
    case 1428 :
        nibunno3x = 1269121;
        nibunnox3 = 6782994;
        break;
    case 1429 :
        nibunno3x = 1267733;
        nibunnox3 = 6776453;
        break;
    case 1430 :
        nibunno3x = 1266344;
        nibunnox3 = 6769913;
        break;
    case 1431 :
        nibunno3x = 1264957;
        nibunnox3 = 6763379;
        break;
    case 1432 :
        nibunno3x = 1263570;
        nibunnox3 = 6756847;
        break;
    case 1433 :
        nibunno3x = 1262184;
        nibunnox3 = 6750323;
        break;
    case 1434 :
        nibunno3x = 1260798;
        nibunnox3 = 6743805;
        break;
    case 1435 :
        nibunno3x = 1259413;
        nibunnox3 = 6737288;
        break;
    case 1436 :
        nibunno3x = 1258028;
        nibunnox3 = 6730779;
        break;
    case 1437 :
        nibunno3x = 1256645;
        nibunnox3 = 6724273;
        break;
    case 1438 :
        nibunno3x = 1255261;
        nibunnox3 = 6717770;
        break;
    case 1439 :
        nibunno3x = 1253880;
        nibunnox3 = 6711277;
        break;
    case 1440 :
        nibunno3x = 1252496;
        nibunnox3 = 6704781;
        break;
    case 1441 :
        nibunno3x = 1251115;
        nibunnox3 = 6698294;
        break;
    case 1442 :
        nibunno3x = 1249734;
        nibunnox3 = 6691812;
        break;
    case 1443 :
        nibunno3x = 1248354;
        nibunnox3 = 6685332;
        break;
    case 1444 :
        nibunno3x = 1246974;
        nibunnox3 = 6678857;
        break;
    case 1445 :
        nibunno3x = 1245595;
        nibunnox3 = 6672391;
        break;
    case 1446 :
        nibunno3x = 1244216;
        nibunnox3 = 6665924;
        break;
    case 1447 :
        nibunno3x = 1242838;
        nibunnox3 = 6659465;
        break;
    case 1448 :
        nibunno3x = 1241461;
        nibunnox3 = 6653011;
        break;
    case 1449 :
        nibunno3x = 1240084;
        nibunnox3 = 6646560;
        break;
    case 1450 :
        nibunno3x = 1238707;
        nibunnox3 = 6640111;
        break;
    case 1451 :
        nibunno3x = 1237332;
        nibunnox3 = 6633670;
        break;
    case 1452 :
        nibunno3x = 1235958;
        nibunnox3 = 6627239;
        break;
    case 1453 :
        nibunno3x = 1234582;
        nibunnox3 = 6620801;
        break;
    case 1454 :
        nibunno3x = 1233208;
        nibunnox3 = 6614373;
        break;
    case 1455 :
        nibunno3x = 1231836;
        nibunnox3 = 6607954;
        break;
    case 1456 :
        nibunno3x = 1230463;
        nibunnox3 = 6601537;
        break;
    case 1457 :
        nibunno3x = 1229090;
        nibunnox3 = 6595120;
        break;
    case 1458 :
        nibunno3x = 1227719;
        nibunnox3 = 6588710;
        break;
    case 1459 :
        nibunno3x = 1226348;
        nibunnox3 = 6582309;
        break;
    case 1460 :
        nibunno3x = 1224978;
        nibunnox3 = 6575909;
        break;
    case 1461 :
        nibunno3x = 1223608;
        nibunnox3 = 6569510;
        break;
    case 1462 :
        nibunno3x = 1222238;
        nibunnox3 = 6563119;
        break;
    case 1463 :
        nibunno3x = 1220870;
        nibunnox3 = 6556735;
        break;
    case 1464 :
        nibunno3x = 1219502;
        nibunnox3 = 6550353;
        break;
    case 1465 :
        nibunno3x = 1218134;
        nibunnox3 = 6543972;
        break;
    case 1466 :
        nibunno3x = 1216768;
        nibunnox3 = 6537602;
        break;
    case 1467 :
        nibunno3x = 1215402;
        nibunnox3 = 6531232;
        break;
    case 1468 :
        nibunno3x = 1214036;
        nibunnox3 = 6524871;
        break;
    case 1469 :
        nibunno3x = 1212672;
        nibunnox3 = 6518512;
        break;
    case 1470 :
        nibunno3x = 1211306;
        nibunnox3 = 6512155;
        break;
    case 1471 :
        nibunno3x = 1209943;
        nibunnox3 = 6505806;
        break;
    case 1472 :
        nibunno3x = 1208580;
        nibunnox3 = 6499461;
        break;
    case 1473 :
        nibunno3x = 1207217;
        nibunnox3 = 6493119;
        break;
    case 1474 :
        nibunno3x = 1205854;
        nibunnox3 = 6486780;
        break;
    case 1475 :
        nibunno3x = 1204494;
        nibunnox3 = 6480453;
        break;
    case 1476 :
        nibunno3x = 1203132;
        nibunnox3 = 6474120;
        break;
    case 1477 :
        nibunno3x = 1201771;
        nibunnox3 = 6467797;
        break;
    case 1478 :
        nibunno3x = 1200411;
        nibunnox3 = 6461478;
        break;
    case 1479 :
        nibunno3x = 1199052;
        nibunnox3 = 6455162;
        break;
    case 1480 :
        nibunno3x = 1197694;
        nibunnox3 = 6448858;
        break;
    case 1481 :
        nibunno3x = 1196335;
        nibunnox3 = 6442548;
        break;
    case 1482 :
        nibunno3x = 1194978;
        nibunnox3 = 6436248;
        break;
    case 1483 :
        nibunno3x = 1193622;
        nibunnox3 = 6429956;
        break;
    case 1484 :
        nibunno3x = 1192264;
        nibunnox3 = 6423659;
        break;
    case 1485 :
        nibunno3x = 1190909;
        nibunnox3 = 6417374;
        break;
    case 1486 :
        nibunno3x = 1189554;
        nibunnox3 = 6411091;
        break;
    case 1487 :
        nibunno3x = 1188199;
        nibunnox3 = 6404814;
        break;
    case 1488 :
        nibunno3x = 1186844;
        nibunnox3 = 6398537;
        break;
    case 1489 :
        nibunno3x = 1185492;
        nibunnox3 = 6392269;
        break;
    case 1490 :
        nibunno3x = 1184138;
        nibunnox3 = 6386004;
        break;
    case 1491 :
        nibunno3x = 1182786;
        nibunnox3 = 6379741;
        break;
    case 1492 :
        nibunno3x = 1181434;
        nibunnox3 = 6373485;
        break;
    case 1493 :
        nibunno3x = 1180082;
        nibunnox3 = 6367233;
        break;
    case 1494 :
        nibunno3x = 1178732;
        nibunnox3 = 6360985;
        break;
    case 1495 :
        nibunno3x = 1177382;
        nibunnox3 = 6354744;
        break;
    case 1496 :
        nibunno3x = 1176032;
        nibunnox3 = 6348503;
        break;
    case 1497 :
        nibunno3x = 1174683;
        nibunnox3 = 6342268;
        break;
    case 1498 :
        nibunno3x = 1173336;
        nibunnox3 = 6336040;
        break;
    case 1499 :
        nibunno3x = 1171988;
        nibunnox3 = 6329814;
        break;
    case 1500 :
        nibunno3x = 1170640;
        nibunnox3 = 6323592;
        break;
    case 1501 :
        nibunno3x = 1169294;
        nibunnox3 = 6317377;
        break;
    case 1502 :
        nibunno3x = 1167948;
        nibunnox3 = 6311163;
        break;
    case 1503 :
        nibunno3x = 1166602;
        nibunnox3 = 6304955;
        break;
    case 1504 :
        nibunno3x = 1165257;
        nibunnox3 = 6298752;
        break;
    case 1505 :
        nibunno3x = 1163912;
        nibunnox3 = 6292552;
        break;
    case 1506 :
        nibunno3x = 1162568;
        nibunnox3 = 6286355;
        break;
    case 1507 :
        nibunno3x = 1161226;
        nibunnox3 = 6280169;
        break;
    case 1508 :
        nibunno3x = 1159884;
        nibunnox3 = 6273983;
        break;
    case 1509 :
        nibunno3x = 1158541;
        nibunnox3 = 6267798;
        break;
    case 1510 :
        nibunno3x = 1157199;
        nibunnox3 = 6261621;
        break;
    case 1511 :
        nibunno3x = 1155859;
        nibunnox3 = 6255451;
        break;
    case 1512 :
        nibunno3x = 1154518;
        nibunnox3 = 6249278;
        break;
    case 1513 :
        nibunno3x = 1153178;
        nibunnox3 = 6243115;
        break;
    case 1514 :
        nibunno3x = 1151839;
        nibunnox3 = 6236955;
        break;
    case 1515 :
        nibunno3x = 1150500;
        nibunnox3 = 6230795;
        break;
    case 1516 :
        nibunno3x = 1149162;
        nibunnox3 = 6224644;
        break;
    case 1517 :
        nibunno3x = 1147824;
        nibunnox3 = 6218498;
        break;
    case 1518 :
        nibunno3x = 1146487;
        nibunnox3 = 6212355;
        break;
    case 1519 :
        nibunno3x = 1145150;
        nibunnox3 = 6206215;
        break;
    case 1520 :
        nibunno3x = 1143816;
        nibunnox3 = 6200085;
        break;
    case 1521 :
        nibunno3x = 1142479;
        nibunnox3 = 6193950;
        break;
    case 1522 :
        nibunno3x = 1141144;
        nibunnox3 = 6187823;
        break;
    case 1523 :
        nibunno3x = 1139810;
        nibunnox3 = 6181705;
        break;
    case 1524 :
        nibunno3x = 1138477;
        nibunnox3 = 6175588;
        break;
    case 1525 :
        nibunno3x = 1137144;
        nibunnox3 = 6169474;
        break;
    case 1526 :
        nibunno3x = 1135811;
        nibunnox3 = 6163364;
        break;
    case 1527 :
        nibunno3x = 1134480;
        nibunnox3 = 6157263;
        break;
    case 1528 :
        nibunno3x = 1133148;
        nibunnox3 = 6151161;
        break;
    case 1529 :
        nibunno3x = 1131817;
        nibunnox3 = 6145066;
        break;
    case 1530 :
        nibunno3x = 1130486;
        nibunnox3 = 6138974;
        break;
    case 1531 :
        nibunno3x = 1129156;
        nibunnox3 = 6132883;
        break;
    case 1532 :
        nibunno3x = 1127827;
        nibunnox3 = 6126801;
        break;
    case 1533 :
        nibunno3x = 1126499;
        nibunnox3 = 6120724;
        break;
    case 1534 :
        nibunno3x = 1125170;
        nibunnox3 = 6114649;
        break;
    case 1535 :
        nibunno3x = 1123843;
        nibunnox3 = 6108578;
        break;
    case 1536 :
        nibunno3x = 1122517;
        nibunnox3 = 6102516;
        break;
    case 1537 :
        nibunno3x = 1121190;
        nibunnox3 = 6096450;
        break;
    case 1538 :
        nibunno3x = 1119864;
        nibunnox3 = 6090390;
        break;
    case 1539 :
        nibunno3x = 1118538;
        nibunnox3 = 6084338;
        break;
    case 1540 :
        nibunno3x = 1117214;
        nibunnox3 = 6078290;
        break;
    case 1541 :
        nibunno3x = 1115890;
        nibunnox3 = 6072247;
        break;
    case 1542 :
        nibunno3x = 1114566;
        nibunnox3 = 6066201;
        break;
    case 1543 :
        nibunno3x = 1113242;
        nibunnox3 = 6060166;
        break;
    case 1544 :
        nibunno3x = 1111920;
        nibunnox3 = 6054135;
        break;
    case 1545 :
        nibunno3x = 1110598;
        nibunnox3 = 6048106;
        break;
    case 1546 :
        nibunno3x = 1109276;
        nibunnox3 = 6042080;
        break;
    case 1547 :
        nibunno3x = 1107956;
        nibunnox3 = 6036062;
        break;
    case 1548 :
        nibunno3x = 1106636;
        nibunnox3 = 6030048;
        break;
    case 1549 :
        nibunno3x = 1105316;
        nibunnox3 = 6024036;
        break;
    case 1550 :
        nibunno3x = 1103996;
        nibunnox3 = 6018027;
        break;
    case 1551 :
        nibunno3x = 1102678;
        nibunnox3 = 6012025;
        break;
    case 1552 :
        nibunno3x = 1101360;
        nibunnox3 = 6006024;
        break;
    case 1553 :
        nibunno3x = 1100042;
        nibunnox3 = 6000029;
        break;
    case 1554 :
        nibunno3x = 1098726;
        nibunnox3 = 5994041;
        break;
    case 1555 :
        nibunno3x = 1097408;
        nibunnox3 = 5988054;
        break;
    case 1556 :
        nibunno3x = 1096093;
        nibunnox3 = 5982073;
        break;
    case 1557 :
        nibunno3x = 1094778;
        nibunnox3 = 5976094;
        break;
    case 1558 :
        nibunno3x = 1093463;
        nibunnox3 = 5970121;
        break;
    case 1559 :
        nibunno3x = 1092148;
        nibunnox3 = 5964148;
        break;
    case 1560 :
        nibunno3x = 1090834;
        nibunnox3 = 5958181;
        break;
    case 1561 :
        nibunno3x = 1089521;
        nibunnox3 = 5952220;
        break;
    case 1562 :
        nibunno3x = 1088209;
        nibunnox3 = 5946266;
        break;
    case 1563 :
        nibunno3x = 1086896;
        nibunnox3 = 5940312;
        break;
    case 1564 :
        nibunno3x = 1085585;
        nibunnox3 = 5934362;
        break;
    case 1565 :
        nibunno3x = 1084274;
        nibunnox3 = 5928420;
        break;
    case 1566 :
        nibunno3x = 1082964;
        nibunnox3 = 5922476;
        break;
    case 1567 :
        nibunno3x = 1081654;
        nibunnox3 = 5916541;
        break;
    case 1568 :
        nibunno3x = 1080344;
        nibunnox3 = 5910604;
        break;
    case 1569 :
        nibunno3x = 1079035;
        nibunnox3 = 5904676;
        break;
    case 1570 :
        nibunno3x = 1077727;
        nibunnox3 = 5898753;
        break;
    case 1571 :
        nibunno3x = 1076419;
        nibunnox3 = 5892831;
        break;
    case 1572 :
        nibunno3x = 1075112;
        nibunnox3 = 5886915;
        break;
    case 1573 :
        nibunno3x = 1073805;
        nibunnox3 = 5881003;
        break;
    case 1574 :
        nibunno3x = 1072499;
        nibunnox3 = 5875093;
        break;
    case 1575 :
        nibunno3x = 1071193;
        nibunnox3 = 5869188;
        break;
    case 1576 :
        nibunno3x = 1069888;
        nibunnox3 = 5863289;
        break;
    case 1577 :
        nibunno3x = 1068583;
        nibunnox3 = 5857390;
        break;
    case 1578 :
        nibunno3x = 1067280;
        nibunnox3 = 5851501;
        break;
    case 1579 :
        nibunno3x = 1065976;
        nibunnox3 = 5845612;
        break;
    case 1580 :
        nibunno3x = 1064673;
        nibunnox3 = 5839729;
        break;
    case 1581 :
        nibunno3x = 1063371;
        nibunnox3 = 5833852;
        break;
    case 1582 :
        nibunno3x = 1062068;
        nibunnox3 = 5827971;
        break;
    case 1583 :
        nibunno3x = 1060768;
        nibunnox3 = 5822103;
        break;
    case 1584 :
        nibunno3x = 1059468;
        nibunnox3 = 5816237;
        break;
    case 1585 :
        nibunno3x = 1058167;
        nibunnox3 = 5810372;
        break;
    case 1586 :
        nibunno3x = 1056867;
        nibunnox3 = 5804512;
        break;
    case 1587 :
        nibunno3x = 1055568;
        nibunnox3 = 5798653;
        break;
    case 1588 :
        nibunno3x = 1054269;
        nibunnox3 = 5792803;
        break;
    case 1589 :
        nibunno3x = 1052971;
        nibunnox3 = 5786955;
        break;
    case 1590 :
        nibunno3x = 1051674;
        nibunnox3 = 5781111;
        break;
    case 1591 :
        nibunno3x = 1050377;
        nibunnox3 = 5775273;
        break;
    case 1592 :
        nibunno3x = 1049080;
        nibunnox3 = 5769436;
        break;
    case 1593 :
        nibunno3x = 1047784;
        nibunnox3 = 5763604;
        break;
    case 1594 :
        nibunno3x = 1046490;
        nibunnox3 = 5757780;
        break;
    case 1595 :
        nibunno3x = 1045194;
        nibunnox3 = 5751952;
        break;
    case 1596 :
        nibunno3x = 1043900;
        nibunnox3 = 5746138;
        break;
    case 1597 :
        nibunno3x = 1042606;
        nibunnox3 = 5740320;
        break;
    case 1598 :
        nibunno3x = 1041313;
        nibunnox3 = 5734510;
        break;
    case 1599 :
        nibunno3x = 1040020;
        nibunnox3 = 5728701;
        break;
    case 1600 :
        nibunno3x = 1038728;
        nibunnox3 = 5722897;
        break;
    case 1601 :
        nibunno3x = 1037436;
        nibunnox3 = 5717099;
        break;
    case 1602 :
        nibunno3x = 1036145;
        nibunnox3 = 5711301;
        break;
    case 1603 :
        nibunno3x = 1034854;
        nibunnox3 = 5705509;
        break;
    case 1604 :
        nibunno3x = 1033564;
        nibunnox3 = 5699722;
        break;
    case 1605 :
        nibunno3x = 1032274;
        nibunnox3 = 5693935;
        break;
    case 1606 :
        nibunno3x = 1030986;
        nibunnox3 = 5688159;
        break;
    case 1607 :
        nibunno3x = 1029697;
        nibunnox3 = 5682383;
        break;
    case 1608 :
        nibunno3x = 1028408;
        nibunnox3 = 5676608;
        break;
    case 1609 :
        nibunno3x = 1027122;
        nibunnox3 = 5670842;
        break;
    case 1610 :
        nibunno3x = 1025834;
        nibunnox3 = 5665078;
        break;
    case 1611 :
        nibunno3x = 1024548;
        nibunnox3 = 5659315;
        break;
    case 1612 :
        nibunno3x = 1023262;
        nibunnox3 = 5653560;
        break;
    case 1613 :
        nibunno3x = 1021976;
        nibunnox3 = 5647807;
        break;
    case 1614 :
        nibunno3x = 1020692;
        nibunnox3 = 5642062;
        break;
    case 1615 :
        nibunno3x = 1019408;
        nibunnox3 = 5636317;
        break;
    case 1616 :
        nibunno3x = 1018124;
        nibunnox3 = 5630578;
        break;
    case 1617 :
        nibunno3x = 1016840;
        nibunnox3 = 5624838;
        break;
    case 1618 :
        nibunno3x = 1015558;
        nibunnox3 = 5619107;
        break;
    case 1619 :
        nibunno3x = 1014276;
        nibunnox3 = 5613377;
        break;
    case 1620 :
        nibunno3x = 1012994;
        nibunnox3 = 5607654;
        break;
    case 1621 :
        nibunno3x = 1011714;
        nibunnox3 = 5601934;
        break;
    case 1622 :
        nibunno3x = 1010432;
        nibunnox3 = 5596215;
        break;
    case 1623 :
        nibunno3x = 1009153;
        nibunnox3 = 5590505;
        break;
    case 1624 :
        nibunno3x = 1007873;
        nibunnox3 = 5584792;
        break;
    case 1625 :
        nibunno3x = 1006594;
        nibunnox3 = 5579089;
        break;
    case 1626 :
        nibunno3x = 1005316;
        nibunnox3 = 5573389;
        break;
    case 1627 :
        nibunno3x = 1004037;
        nibunnox3 = 5567688;
        break;
    case 1628 :
        nibunno3x = 1002760;
        nibunnox3 = 5561995;
        break;
    case 1629 :
        nibunno3x = 1001484;
        nibunnox3 = 5556307;
        break;
    case 1630 :
        nibunno3x = 1000206;
        nibunnox3 = 5550617;
        break;
    case 1631 :
        nibunno3x = 998930;
        nibunnox3 = 5544936;
        break;
    case 1632 :
        nibunno3x = 997656;
        nibunnox3 = 5539260;
        break;
    case 1633 :
        nibunno3x = 996380;
        nibunnox3 = 5533585;
        break;
    case 1634 :
        nibunno3x = 995106;
        nibunnox3 = 5527914;
        break;
    case 1635 :
        nibunno3x = 993833;
        nibunnox3 = 5522250;
        break;
    case 1636 :
        nibunno3x = 992559;
        nibunnox3 = 5516586;
        break;
    case 1637 :
        nibunno3x = 991286;
        nibunnox3 = 5510927;
        break;
    case 1638 :
        nibunno3x = 990014;
        nibunnox3 = 5505270;
        break;
    case 1639 :
        nibunno3x = 988742;
        nibunnox3 = 5499620;
        break;
    case 1640 :
        nibunno3x = 987470;
        nibunnox3 = 5493970;
        break;
    case 1641 :
        nibunno3x = 986200;
        nibunnox3 = 5488327;
        break;
    case 1642 :
        nibunno3x = 984930;
        nibunnox3 = 5482686;
        break;
    case 1643 :
        nibunno3x = 983660;
        nibunnox3 = 5477054;
        break;
    case 1644 :
        nibunno3x = 982391;
        nibunnox3 = 5471419;
        break;
    case 1645 :
        nibunno3x = 981122;
        nibunnox3 = 5465791;
        break;
    case 1646 :
        nibunno3x = 979854;
        nibunnox3 = 5460167;
        break;
    case 1647 :
        nibunno3x = 978586;
        nibunnox3 = 5454543;
        break;
    case 1648 :
        nibunno3x = 977320;
        nibunnox3 = 5448931;
        break;
    case 1649 :
        nibunno3x = 976052;
        nibunnox3 = 5443313;
        break;
    case 1650 :
        nibunno3x = 974786;
        nibunnox3 = 5437705;
        break;
    case 1651 :
        nibunno3x = 973521;
        nibunnox3 = 5432100;
        break;
    case 1652 :
        nibunno3x = 972256;
        nibunnox3 = 5426498;
        break;
    case 1653 :
        nibunno3x = 970992;
        nibunnox3 = 5420900;
        break;
    case 1654 :
        nibunno3x = 969727;
        nibunnox3 = 5415304;
        break;
    case 1655 :
        nibunno3x = 968464;
        nibunnox3 = 5409716;
        break;
    case 1656 :
        nibunno3x = 967201;
        nibunnox3 = 5404129;
        break;
    case 1657 :
        nibunno3x = 965938;
        nibunnox3 = 5398544;
        break;
    case 1658 :
        nibunno3x = 964676;
        nibunnox3 = 5392967;
        break;
    case 1659 :
        nibunno3x = 963415;
        nibunnox3 = 5387391;
        break;
    case 1660 :
        nibunno3x = 962154;
        nibunnox3 = 5381821;
        break;
    case 1661 :
        nibunno3x = 960894;
        nibunnox3 = 5376252;
        break;
    case 1662 :
        nibunno3x = 959634;
        nibunnox3 = 5370687;
        break;
    case 1663 :
        nibunno3x = 958374;
        nibunnox3 = 5365124;
        break;
    case 1664 :
        nibunno3x = 957115;
        nibunnox3 = 5359569;
        break;
    case 1665 :
        nibunno3x = 955856;
        nibunnox3 = 5354016;
        break;
    case 1666 :
        nibunno3x = 954599;
        nibunnox3 = 5348467;
        break;
    case 1667 :
        nibunno3x = 953341;
        nibunnox3 = 5342921;
        break;
    case 1668 :
        nibunno3x = 952085;
        nibunnox3 = 5337382;
        break;
    case 1669 :
        nibunno3x = 950828;
        nibunnox3 = 5331844;
        break;
    case 1670 :
        nibunno3x = 949572;
        nibunnox3 = 5326308;
        break;
    case 1671 :
        nibunno3x = 948317;
        nibunnox3 = 5320778;
        break;
    case 1672 :
        nibunno3x = 947062;
        nibunnox3 = 5315251;
        break;
    case 1673 :
        nibunno3x = 945807;
        nibunnox3 = 5309727;
        break;
    case 1674 :
        nibunno3x = 944554;
        nibunnox3 = 5304210;
        break;
    case 1675 :
        nibunno3x = 943300;
        nibunnox3 = 5298691;
        break;
    case 1676 :
        nibunno3x = 942048;
        nibunnox3 = 5293181;
        break;
    case 1677 :
        nibunno3x = 940795;
        nibunnox3 = 5287672;
        break;
    case 1678 :
        nibunno3x = 939543;
        nibunnox3 = 5282167;
        break;
    case 1679 :
        nibunno3x = 938292;
        nibunnox3 = 5276668;
        break;
    case 1680 :
        nibunno3x = 937041;
        nibunnox3 = 5271171;
        break;
    case 1681 :
        nibunno3x = 935791;
        nibunnox3 = 5265677;
        break;
    case 1682 :
        nibunno3x = 934541;
        nibunnox3 = 5260185;
        break;
    case 1683 :
        nibunno3x = 933292;
        nibunnox3 = 5254701;
        break;
    case 1684 :
        nibunno3x = 932042;
        nibunnox3 = 5249216;
        break;
    case 1685 :
        nibunno3x = 930794;
        nibunnox3 = 5243739;
        break;
    case 1686 :
        nibunno3x = 929546;
        nibunnox3 = 5238262;
        break;
    case 1687 :
        nibunno3x = 928299;
        nibunnox3 = 5232791;
        break;
    case 1688 :
        nibunno3x = 927052;
        nibunnox3 = 5227322;
        break;
    case 1689 :
        nibunno3x = 925806;
        nibunnox3 = 5221861;
        break;
    case 1690 :
        nibunno3x = 924560;
        nibunnox3 = 5216400;
        break;
    case 1691 :
        nibunno3x = 923315;
        nibunnox3 = 5210941;
        break;
    case 1692 :
        nibunno3x = 922070;
        nibunnox3 = 5205491;
        break;
    case 1693 :
        nibunno3x = 920826;
        nibunnox3 = 5200037;
        break;
    case 1694 :
        nibunno3x = 919582;
        nibunnox3 = 5194593;
        break;
    case 1695 :
        nibunno3x = 918338;
        nibunnox3 = 5189150;
        break;
    case 1696 :
        nibunno3x = 917096;
        nibunnox3 = 5183711;
        break;
    case 1697 :
        nibunno3x = 915854;
        nibunnox3 = 5178278;
        break;
    case 1698 :
        nibunno3x = 914612;
        nibunnox3 = 5172846;
        break;
    case 1699 :
        nibunno3x = 913370;
        nibunnox3 = 5167418;
        break;
    case 1700 :
        nibunno3x = 912130;
        nibunnox3 = 5161996;
        break;
    case 1701 :
        nibunno3x = 910890;
        nibunnox3 = 5156575;
        break;
    case 1702 :
        nibunno3x = 909649;
        nibunnox3 = 5151154;
        break;
    case 1703 :
        nibunno3x = 908410;
        nibunnox3 = 5145743;
        break;
    case 1704 :
        nibunno3x = 907171;
        nibunnox3 = 5140333;
        break;
    case 1705 :
        nibunno3x = 905933;
        nibunnox3 = 5134927;
        break;
    case 1706 :
        nibunno3x = 904694;
        nibunnox3 = 5129523;
        break;
    case 1707 :
        nibunno3x = 903457;
        nibunnox3 = 5124124;
        break;
    case 1708 :
        nibunno3x = 902221;
        nibunnox3 = 5118732;
        break;
    case 1709 :
        nibunno3x = 900984;
        nibunnox3 = 5113334;
        break;
    case 1710 :
        nibunno3x = 899748;
        nibunnox3 = 5107950;
        break;
    case 1711 :
        nibunno3x = 898513;
        nibunnox3 = 5102565;
        break;
    case 1712 :
        nibunno3x = 897278;
        nibunnox3 = 5097183;
        break;
    case 1713 :
        nibunno3x = 896044;
        nibunnox3 = 5091808;
        break;
    case 1714 :
        nibunno3x = 894810;
        nibunnox3 = 5086431;
        break;
    case 1715 :
        nibunno3x = 893576;
        nibunnox3 = 5081064;
        break;
    case 1716 :
        nibunno3x = 892344;
        nibunnox3 = 5075696;
        break;
    case 1717 :
        nibunno3x = 891110;
        nibunnox3 = 5070330;
        break;
    case 1718 :
        nibunno3x = 889879;
        nibunnox3 = 5064974;
        break;
    case 1719 :
        nibunno3x = 888648;
        nibunnox3 = 5059617;
        break;
    case 1720 :
        nibunno3x = 887416;
        nibunnox3 = 5054262;
        break;
    case 1721 :
        nibunno3x = 886186;
        nibunnox3 = 5048915;
        break;
    case 1722 :
        nibunno3x = 884956;
        nibunnox3 = 5043570;
        break;
    case 1723 :
        nibunno3x = 883726;
        nibunnox3 = 5038226;
        break;
    case 1724 :
        nibunno3x = 882498;
        nibunnox3 = 5032890;
        break;
    case 1725 :
        nibunno3x = 881269;
        nibunnox3 = 5027555;
        break;
    case 1726 :
        nibunno3x = 880040;
        nibunnox3 = 5022222;
        break;
    case 1727 :
        nibunno3x = 878813;
        nibunnox3 = 5016893;
        break;
    case 1728 :
        nibunno3x = 877586;
        nibunnox3 = 5011572;
        break;
    case 1729 :
        nibunno3x = 876360;
        nibunnox3 = 5006250;
        break;
    case 1730 :
        nibunno3x = 875134;
        nibunnox3 = 5000936;
        break;
    case 1731 :
        nibunno3x = 873908;
        nibunnox3 = 4995622;
        break;
    case 1732 :
        nibunno3x = 872683;
        nibunnox3 = 4990310;
        break;
    case 1733 :
        nibunno3x = 871458;
        nibunnox3 = 4985000;
        break;
    case 1734 :
        nibunno3x = 870234;
        nibunnox3 = 4979697;
        break;
    case 1735 :
        nibunno3x = 869011;
        nibunnox3 = 4974403;
        break;
    case 1736 :
        nibunno3x = 867787;
        nibunnox3 = 4969103;
        break;
    case 1737 :
        nibunno3x = 866564;
        nibunnox3 = 4963811;
        break;
    case 1738 :
        nibunno3x = 865342;
        nibunnox3 = 4958521;
        break;
    case 1739 :
        nibunno3x = 864121;
        nibunnox3 = 4953238;
        break;
    case 1740 :
        nibunno3x = 862900;
        nibunnox3 = 4947957;
        break;
    case 1741 :
        nibunno3x = 861679;
        nibunnox3 = 4942677;
        break;
    case 1742 :
        nibunno3x = 860458;
        nibunnox3 = 4937400;
        break;
    case 1743 :
        nibunno3x = 859238;
        nibunnox3 = 4932129;
        break;
    case 1744 :
        nibunno3x = 858019;
        nibunnox3 = 4926860;
        break;
    case 1745 :
        nibunno3x = 856800;
        nibunnox3 = 4921596;
        break;
    case 1746 :
        nibunno3x = 855582;
        nibunnox3 = 4916335;
        break;
    case 1747 :
        nibunno3x = 854365;
        nibunnox3 = 4911081;
        break;
    case 1748 :
        nibunno3x = 853147;
        nibunnox3 = 4905823;
        break;
    case 1749 :
        nibunno3x = 851930;
        nibunnox3 = 4900575;
        break;
    case 1750 :
        nibunno3x = 850714;
        nibunnox3 = 4895326;
        break;
    case 1751 :
        nibunno3x = 849498;
        nibunnox3 = 4890081;
        break;
    case 1752 :
        nibunno3x = 848282;
        nibunnox3 = 4884841;
        break;
    case 1753 :
        nibunno3x = 847068;
        nibunnox3 = 4879605;
        break;
    case 1754 :
        nibunno3x = 845852;
        nibunnox3 = 4874369;
        break;
    case 1755 :
        nibunno3x = 844639;
        nibunnox3 = 4869140;
        break;
    case 1756 :
        nibunno3x = 843426;
        nibunnox3 = 4863914;
        break;
    case 1757 :
        nibunno3x = 842212;
        nibunnox3 = 4858689;
        break;
    case 1758 :
        nibunno3x = 841000;
        nibunnox3 = 4853472;
        break;
    case 1759 :
        nibunno3x = 839787;
        nibunnox3 = 4848252;
        break;
    case 1760 :
        nibunno3x = 838576;
        nibunnox3 = 4843041;
        break;
    case 1761 :
        nibunno3x = 837364;
        nibunnox3 = 4837827;
        break;
    case 1762 :
        nibunno3x = 836154;
        nibunnox3 = 4832621;
        break;
    case 1763 :
        nibunno3x = 834944;
        nibunnox3 = 4827422;
        break;
    case 1764 :
        nibunno3x = 833734;
        nibunnox3 = 4822221;
        break;
    case 1765 :
        nibunno3x = 832525;
        nibunnox3 = 4817026;
        break;
    case 1766 :
        nibunno3x = 831316;
        nibunnox3 = 4811833;
        break;
    case 1767 :
        nibunno3x = 830108;
        nibunnox3 = 4806644;
        break;
    case 1768 :
        nibunno3x = 828900;
        nibunnox3 = 4801457;
        break;
    case 1769 :
        nibunno3x = 827692;
        nibunnox3 = 4796273;
        break;
    case 1770 :
        nibunno3x = 826486;
        nibunnox3 = 4791099;
        break;
    case 1771 :
        nibunno3x = 825279;
        nibunnox3 = 4785921;
        break;
    case 1772 :
        nibunno3x = 824072;
        nibunnox3 = 4780745;
        break;
    case 1773 :
        nibunno3x = 822868;
        nibunnox3 = 4775580;
        break;
    case 1774 :
        nibunno3x = 821663;
        nibunnox3 = 4770414;
        break;
    case 1775 :
        nibunno3x = 820458;
        nibunnox3 = 4765252;
        break;
    case 1776 :
        nibunno3x = 819254;
        nibunnox3 = 4760094;
        break;
    case 1777 :
        nibunno3x = 818051;
        nibunnox3 = 4754939;
        break;
    case 1778 :
        nibunno3x = 816848;
        nibunnox3 = 4749787;
        break;
    case 1779 :
        nibunno3x = 815645;
        nibunnox3 = 4744637;
        break;
    case 1780 :
        nibunno3x = 814442;
        nibunnox3 = 4739491;
        break;
    case 1781 :
        nibunno3x = 813241;
        nibunnox3 = 4734350;
        break;
    case 1782 :
        nibunno3x = 812040;
        nibunnox3 = 4729213;
        break;
    case 1783 :
        nibunno3x = 810840;
        nibunnox3 = 4724078;
        break;
    case 1784 :
        nibunno3x = 809639;
        nibunnox3 = 4718944;
        break;
    case 1785 :
        nibunno3x = 808440;
        nibunnox3 = 4713817;
        break;
    case 1786 :
        nibunno3x = 807240;
        nibunnox3 = 4708690;
        break;
    case 1787 :
        nibunno3x = 806041;
        nibunnox3 = 4703569;
        break;
    case 1788 :
        nibunno3x = 804842;
        nibunnox3 = 4698451;
        break;
    case 1789 :
        nibunno3x = 803644;
        nibunnox3 = 4693333;
        break;
    case 1790 :
        nibunno3x = 802447;
        nibunnox3 = 4688223;
        break;
    case 1791 :
        nibunno3x = 801250;
        nibunnox3 = 4683115;
        break;
    case 1792 :
        nibunno3x = 800053;
        nibunnox3 = 4678008;
        break;
    case 1793 :
        nibunno3x = 798858;
        nibunnox3 = 4672908;
        break;
    case 1794 :
        nibunno3x = 797662;
        nibunnox3 = 4667809;
        break;
    case 1795 :
        nibunno3x = 796466;
        nibunnox3 = 4662713;
        break;
    case 1796 :
        nibunno3x = 795272;
        nibunnox3 = 4657624;
        break;
    case 1797 :
        nibunno3x = 794078;
        nibunnox3 = 4652536;
        break;
    case 1798 :
        nibunno3x = 792884;
        nibunnox3 = 4647450;
        break;
    case 1799 :
        nibunno3x = 791691;
        nibunnox3 = 4642367;
        break;
    case 1800 :
        nibunno3x = 790498;
        nibunnox3 = 4637287;
        break;
    case 1801 :
        nibunno3x = 789306;
        nibunnox3 = 4632211;
        break;
    case 1802 :
        nibunno3x = 788114;
        nibunnox3 = 4627143;
        break;
    case 1803 :
        nibunno3x = 786923;
        nibunnox3 = 4622072;
        break;
    case 1804 :
        nibunno3x = 785732;
        nibunnox3 = 4617010;
        break;
    case 1805 :
        nibunno3x = 784542;
        nibunnox3 = 4611945;
        break;
    case 1806 :
        nibunno3x = 783351;
        nibunnox3 = 4606884;
        break;
    case 1807 :
        nibunno3x = 782162;
        nibunnox3 = 4601829;
        break;
    case 1808 :
        nibunno3x = 780973;
        nibunnox3 = 4596778;
        break;
    case 1809 :
        nibunno3x = 779784;
        nibunnox3 = 4591726;
        break;
    case 1810 :
        nibunno3x = 778596;
        nibunnox3 = 4586681;
        break;
    case 1811 :
        nibunno3x = 777408;
        nibunnox3 = 4581640;
        break;
    case 1812 :
        nibunno3x = 776221;
        nibunnox3 = 4576601;
        break;
    case 1813 :
        nibunno3x = 775034;
        nibunnox3 = 4571566;
        break;
    case 1814 :
        nibunno3x = 773848;
        nibunnox3 = 4566533;
        break;
    case 1815 :
        nibunno3x = 772662;
        nibunnox3 = 4561500;
        break;
    case 1816 :
        nibunno3x = 771476;
        nibunnox3 = 4556475;
        break;
    case 1817 :
        nibunno3x = 770292;
        nibunnox3 = 4551452;
        break;
    case 1818 :
        nibunno3x = 769107;
        nibunnox3 = 4546434;
        break;
    case 1819 :
        nibunno3x = 767924;
        nibunnox3 = 4541419;
        break;
    case 1820 :
        nibunno3x = 766740;
        nibunnox3 = 4536403;
        break;
    case 1821 :
        nibunno3x = 765557;
        nibunnox3 = 4531394;
        break;
    case 1822 :
        nibunno3x = 764374;
        nibunnox3 = 4526387;
        break;
    case 1823 :
        nibunno3x = 763192;
        nibunnox3 = 4521384;
        break;
    case 1824 :
        nibunno3x = 762010;
        nibunnox3 = 4516383;
        break;
    case 1825 :
        nibunno3x = 760830;
        nibunnox3 = 4511389;
        break;
    case 1826 :
        nibunno3x = 759648;
        nibunnox3 = 4506390;
        break;
    case 1827 :
        nibunno3x = 758468;
        nibunnox3 = 4501402;
        break;
    case 1828 :
        nibunno3x = 757288;
        nibunnox3 = 4496415;
        break;
    case 1829 :
        nibunno3x = 756108;
        nibunnox3 = 4491429;
        break;
    case 1830 :
        nibunno3x = 754930;
        nibunnox3 = 4486451;
        break;
    case 1831 :
        nibunno3x = 753751;
        nibunnox3 = 4481472;
        break;
    case 1832 :
        nibunno3x = 752574;
        nibunnox3 = 4476499;
        break;
    case 1833 :
        nibunno3x = 751395;
        nibunnox3 = 4471525;
        break;
    case 1834 :
        nibunno3x = 750218;
        nibunnox3 = 4466559;
        break;
    case 1835 :
        nibunno3x = 749042;
        nibunnox3 = 4461594;
        break;
    case 1836 :
        nibunno3x = 747865;
        nibunnox3 = 4456630;
        break;
    case 1837 :
        nibunno3x = 746689;
        nibunnox3 = 4451670;
        break;
    case 1838 :
        nibunno3x = 745514;
        nibunnox3 = 4446715;
        break;
    case 1839 :
        nibunno3x = 744339;
        nibunnox3 = 4441765;
        break;
    case 1840 :
        nibunno3x = 743164;
        nibunnox3 = 4436812;
        break;
    case 1841 :
        nibunno3x = 741991;
        nibunnox3 = 4431870;
        break;
    case 1842 :
        nibunno3x = 740817;
        nibunnox3 = 4426927;
        break;
    case 1843 :
        nibunno3x = 739645;
        nibunnox3 = 4421990;
        break;
    case 1844 :
        nibunno3x = 738471;
        nibunnox3 = 4417050;
        break;
    case 1845 :
        nibunno3x = 737300;
        nibunnox3 = 4412120;
        break;
    case 1846 :
        nibunno3x = 736128;
        nibunnox3 = 4407187;
        break;
    case 1847 :
        nibunno3x = 734956;
        nibunnox3 = 4402258;
        break;
    case 1848 :
        nibunno3x = 733785;
        nibunnox3 = 4397336;
        break;
    case 1849 :
        nibunno3x = 732615;
        nibunnox3 = 4392417;
        break;
    case 1850 :
        nibunno3x = 731445;
        nibunnox3 = 4387498;
        break;
    case 1851 :
        nibunno3x = 730276;
        nibunnox3 = 4382586;
        break;
    case 1852 :
        nibunno3x = 729107;
        nibunnox3 = 4377673;
        break;
    case 1853 :
        nibunno3x = 727938;
        nibunnox3 = 4372763;
        break;
    case 1854 :
        nibunno3x = 726770;
        nibunnox3 = 4367862;
        break;
    case 1855 :
        nibunno3x = 725603;
        nibunnox3 = 4362961;
        break;
    case 1856 :
        nibunno3x = 724436;
        nibunnox3 = 4358063;
        break;
    case 1857 :
        nibunno3x = 723269;
        nibunnox3 = 4353167;
        break;
    case 1858 :
        nibunno3x = 722102;
        nibunnox3 = 4348275;
        break;
    case 1859 :
        nibunno3x = 720936;
        nibunnox3 = 4343383;
        break;
    case 1860 :
        nibunno3x = 719772;
        nibunnox3 = 4338501;
        break;
    case 1861 :
        nibunno3x = 718606;
        nibunnox3 = 4333615;
        break;
    case 1862 :
        nibunno3x = 717441;
        nibunnox3 = 4328735;
        break;
    case 1863 :
        nibunno3x = 716277;
        nibunnox3 = 4323859;
        break;
    case 1864 :
        nibunno3x = 715114;
        nibunnox3 = 4318988;
        break;
    case 1865 :
        nibunno3x = 713950;
        nibunnox3 = 4314113;
        break;
    case 1866 :
        nibunno3x = 712788;
        nibunnox3 = 4309248;
        break;
    case 1867 :
        nibunno3x = 711625;
        nibunnox3 = 4304383;
        break;
    case 1868 :
        nibunno3x = 710464;
        nibunnox3 = 4299525;
        break;
    case 1869 :
        nibunno3x = 709302;
        nibunnox3 = 4294666;
        break;
    case 1870 :
        nibunno3x = 708140;
        nibunnox3 = 4289808;
        break;
    case 1871 :
        nibunno3x = 706981;
        nibunnox3 = 4284960;
        break;
    case 1872 :
        nibunno3x = 705820;
        nibunnox3 = 4280108;
        break;
    case 1873 :
        nibunno3x = 704660;
        nibunnox3 = 4275262;
        break;
    case 1874 :
        nibunno3x = 703502;
        nibunnox3 = 4270421;
        break;
    case 1875 :
        nibunno3x = 702343;
        nibunnox3 = 4265583;
        break;
    case 1876 :
        nibunno3x = 701185;
        nibunnox3 = 4260748;
        break;
    case 1877 :
        nibunno3x = 700027;
        nibunnox3 = 4255913;
        break;
    case 1878 :
        nibunno3x = 698869;
        nibunnox3 = 4251081;
        break;
    case 1879 :
        nibunno3x = 697712;
        nibunnox3 = 4246256;
        break;
    case 1880 :
        nibunno3x = 696556;
        nibunnox3 = 4241432;
        break;
    case 1881 :
        nibunno3x = 695400;
        nibunnox3 = 4236609;
        break;
    case 1882 :
        nibunno3x = 694244;
        nibunnox3 = 4231795;
        break;
    case 1883 :
        nibunno3x = 693090;
        nibunnox3 = 4226981;
        break;
    case 1884 :
        nibunno3x = 691934;
        nibunnox3 = 4222168;
        break;
    case 1885 :
        nibunno3x = 690780;
        nibunnox3 = 4217356;
        break;
    case 1886 :
        nibunno3x = 689626;
        nibunnox3 = 4212552;
        break;
    case 1887 :
        nibunno3x = 688472;
        nibunnox3 = 4207750;
        break;
    case 1888 :
        nibunno3x = 687320;
        nibunnox3 = 4202950;
        break;
    case 1889 :
        nibunno3x = 686167;
        nibunnox3 = 4198153;
        break;
    case 1890 :
        nibunno3x = 685015;
        nibunnox3 = 4193361;
        break;
    case 1891 :
        nibunno3x = 683863;
        nibunnox3 = 4188569;
        break;
    case 1892 :
        nibunno3x = 682712;
        nibunnox3 = 4183781;
        break;
    case 1893 :
        nibunno3x = 681560;
        nibunnox3 = 4178995;
        break;
    case 1894 :
        nibunno3x = 680410;
        nibunnox3 = 4174214;
        break;
    case 1895 :
        nibunno3x = 679261;
        nibunnox3 = 4169440;
        break;
    case 1896 :
        nibunno3x = 678111;
        nibunnox3 = 4164663;
        break;
    case 1897 :
        nibunno3x = 676962;
        nibunnox3 = 4159892;
        break;
    case 1898 :
        nibunno3x = 675814;
        nibunnox3 = 4155124;
        break;
    case 1899 :
        nibunno3x = 674665;
        nibunnox3 = 4150355;
        break;
    case 1900 :
        nibunno3x = 673518;
        nibunnox3 = 4145593;
        break;
    case 1901 :
        nibunno3x = 672370;
        nibunnox3 = 4140831;
        break;
    case 1902 :
        nibunno3x = 671224;
        nibunnox3 = 4136078;
        break;
    case 1903 :
        nibunno3x = 670076;
        nibunnox3 = 4131320;
        break;
    case 1904 :
        nibunno3x = 668930;
        nibunnox3 = 4126569;
        break;
    case 1905 :
        nibunno3x = 667786;
        nibunnox3 = 4121825;
        break;
    case 1906 :
        nibunno3x = 666640;
        nibunnox3 = 4117076;
        break;
    case 1907 :
        nibunno3x = 665496;
        nibunnox3 = 4112335;
        break;
    case 1908 :
        nibunno3x = 664351;
        nibunnox3 = 4107595;
        break;
    case 1909 :
        nibunno3x = 663208;
        nibunnox3 = 4102863;
        break;
    case 1910 :
        nibunno3x = 662064;
        nibunnox3 = 4098125;
        break;
    case 1911 :
        nibunno3x = 660920;
        nibunnox3 = 4093395;
        break;
    case 1912 :
        nibunno3x = 659778;
        nibunnox3 = 4088668;
        break;
    case 1913 :
        nibunno3x = 658636;
        nibunnox3 = 4083944;
        break;
    case 1914 :
        nibunno3x = 657494;
        nibunnox3 = 4079224;
        break;
    case 1915 :
        nibunno3x = 656353;
        nibunnox3 = 4074504;
        break;
    case 1916 :
        nibunno3x = 655213;
        nibunnox3 = 4069792;
        break;
    case 1917 :
        nibunno3x = 654072;
        nibunnox3 = 4065079;
        break;
    case 1918 :
        nibunno3x = 652933;
        nibunnox3 = 4060373;
        break;
    case 1919 :
        nibunno3x = 651793;
        nibunnox3 = 4055665;
        break;
    case 1920 :
        nibunno3x = 650654;
        nibunnox3 = 4050960;
        break;
    case 1921 :
        nibunno3x = 649515;
        nibunnox3 = 4046260;
        break;
    case 1922 :
        nibunno3x = 648377;
        nibunnox3 = 4041562;
        break;
    case 1923 :
        nibunno3x = 647239;
        nibunnox3 = 4036868;
        break;
    case 1924 :
        nibunno3x = 646102;
        nibunnox3 = 4032178;
        break;
    case 1925 :
        nibunno3x = 644965;
        nibunnox3 = 4027489;
        break;
    case 1926 :
        nibunno3x = 643828;
        nibunnox3 = 4022801;
        break;
    case 1927 :
        nibunno3x = 642692;
        nibunnox3 = 4018121;
        break;
    case 1928 :
        nibunno3x = 641557;
        nibunnox3 = 4013441;
        break;
    case 1929 :
        nibunno3x = 640421;
        nibunnox3 = 4008761;
        break;
    case 1930 :
        nibunno3x = 639286;
        nibunnox3 = 4004087;
        break;
    case 1931 :
        nibunno3x = 638152;
        nibunnox3 = 3999417;
        break;
    case 1932 :
        nibunno3x = 637018;
        nibunnox3 = 3994749;
        break;
    case 1933 :
        nibunno3x = 635884;
        nibunnox3 = 3990082;
        break;
    case 1934 :
        nibunno3x = 634752;
        nibunnox3 = 3985422;
        break;
    case 1935 :
        nibunno3x = 633619;
        nibunnox3 = 3980765;
        break;
    case 1936 :
        nibunno3x = 632486;
        nibunnox3 = 3976108;
        break;
    case 1937 :
        nibunno3x = 631354;
        nibunnox3 = 3971451;
        break;
    case 1938 :
        nibunno3x = 630223;
        nibunnox3 = 3966802;
        break;
    case 1939 :
        nibunno3x = 629092;
        nibunnox3 = 3962154;
        break;
    case 1940 :
        nibunno3x = 627962;
        nibunnox3 = 3957511;
        break;
    case 1941 :
        nibunno3x = 626832;
        nibunnox3 = 3952869;
        break;
    case 1942 :
        nibunno3x = 625702;
        nibunnox3 = 3948231;
        break;
    case 1943 :
        nibunno3x = 624572;
        nibunnox3 = 3943594;
        break;
    case 1944 :
        nibunno3x = 623444;
        nibunnox3 = 3938961;
        break;
    case 1945 :
        nibunno3x = 622315;
        nibunnox3 = 3934329;
        break;
    case 1946 :
        nibunno3x = 621187;
        nibunnox3 = 3929702;
        break;
    case 1947 :
        nibunno3x = 620059;
        nibunnox3 = 3925077;
        break;
    case 1948 :
        nibunno3x = 618932;
        nibunnox3 = 3920457;
        break;
    case 1949 :
        nibunno3x = 617806;
        nibunnox3 = 3915840;
        break;
    case 1950 :
        nibunno3x = 616680;
        nibunnox3 = 3911224;
        break;
    case 1951 :
        nibunno3x = 615554;
        nibunnox3 = 3906612;
        break;
    case 1952 :
        nibunno3x = 614429;
        nibunnox3 = 3902003;
        break;
    case 1953 :
        nibunno3x = 613304;
        nibunnox3 = 3897397;
        break;
    case 1954 :
        nibunno3x = 612179;
        nibunnox3 = 3892791;
        break;
    case 1955 :
        nibunno3x = 611054;
        nibunnox3 = 3888189;
        break;
    case 1956 :
        nibunno3x = 609931;
        nibunnox3 = 3883591;
        break;
    case 1957 :
        nibunno3x = 608808;
        nibunnox3 = 3878996;
        break;
    case 1958 :
        nibunno3x = 607685;
        nibunnox3 = 3874404;
        break;
    case 1959 :
        nibunno3x = 606562;
        nibunnox3 = 3869813;
        break;
    case 1960 :
        nibunno3x = 605440;
        nibunnox3 = 3865227;
        break;
    case 1961 :
        nibunno3x = 604318;
        nibunnox3 = 3860642;
        break;
    case 1962 :
        nibunno3x = 603198;
        nibunnox3 = 3856063;
        break;
    case 1963 :
        nibunno3x = 602076;
        nibunnox3 = 3851484;
        break;
    case 1964 :
        nibunno3x = 600956;
        nibunnox3 = 3846908;
        break;
    case 1965 :
        nibunno3x = 599836;
        nibunnox3 = 3842336;
        break;
    case 1966 :
        nibunno3x = 598716;
        nibunnox3 = 3837765;
        break;
    case 1967 :
        nibunno3x = 597597;
        nibunnox3 = 3833199;
        break;
    case 1968 :
        nibunno3x = 596478;
        nibunnox3 = 3828630;
        break;
    case 1969 :
        nibunno3x = 595360;
        nibunnox3 = 3824072;
        break;
    case 1970 :
        nibunno3x = 594242;
        nibunnox3 = 3819516;
        break;
    case 1971 :
        nibunno3x = 593125;
        nibunnox3 = 3814961;
        break;
    case 1972 :
        nibunno3x = 592008;
        nibunnox3 = 3810406;
        break;
    case 1973 :
        nibunno3x = 590891;
        nibunnox3 = 3805855;
        break;
    case 1974 :
        nibunno3x = 589776;
        nibunnox3 = 3801312;
        break;
    case 1975 :
        nibunno3x = 588660;
        nibunnox3 = 3796768;
        break;
    case 1976 :
        nibunno3x = 587544;
        nibunnox3 = 3792224;
        break;
    case 1977 :
        nibunno3x = 586429;
        nibunnox3 = 3787687;
        break;
    case 1978 :
        nibunno3x = 585314;
        nibunnox3 = 3783149;
        break;
    case 1979 :
        nibunno3x = 584200;
        nibunnox3 = 3778618;
        break;
    case 1980 :
        nibunno3x = 583086;
        nibunnox3 = 3774087;
        break;
    case 1981 :
        nibunno3x = 581972;
        nibunnox3 = 3769557;
        break;
    case 1982 :
        nibunno3x = 580860;
        nibunnox3 = 3765033;
        break;
    case 1983 :
        nibunno3x = 579747;
        nibunnox3 = 3760511;
        break;
    case 1984 :
        nibunno3x = 578635;
        nibunnox3 = 3755993;
        break;
    case 1985 :
        nibunno3x = 577524;
        nibunnox3 = 3751476;
        break;
    case 1986 :
        nibunno3x = 576412;
        nibunnox3 = 3746962;
        break;
    case 1987 :
        nibunno3x = 575302;
        nibunnox3 = 3742456;
        break;
    case 1988 :
        nibunno3x = 574191;
        nibunnox3 = 3737947;
        break;
    case 1989 :
        nibunno3x = 573082;
        nibunnox3 = 3733445;
        break;
    case 1990 :
        nibunno3x = 571972;
        nibunnox3 = 3728941;
        break;
    case 1991 :
        nibunno3x = 570862;
        nibunnox3 = 3724438;
        break;
    case 1992 :
        nibunno3x = 569754;
        nibunnox3 = 3719942;
        break;
    case 1993 :
        nibunno3x = 568645;
        nibunnox3 = 3715448;
        break;
    case 1994 :
        nibunno3x = 567537;
        nibunnox3 = 3710958;
        break;
    case 1995 :
        nibunno3x = 566430;
        nibunnox3 = 3706469;
        break;
    case 1996 :
        nibunno3x = 565322;
        nibunnox3 = 3701984;
        break;
    case 1997 :
        nibunno3x = 564216;
        nibunnox3 = 3697501;
        break;
    case 1998 :
        nibunno3x = 563110;
        nibunnox3 = 3693023;
        break;
    case 1999 :
        nibunno3x = 562004;
        nibunnox3 = 3688546;
        break;
    case 2000 :
        nibunno3x = 560898;
        nibunnox3 = 3684071;
        break;
    case 2001 :
        nibunno3x = 559793;
        nibunnox3 = 3679597;
        break;
    case 2002 :
        nibunno3x = 558688;
        nibunnox3 = 3675128;
        break;
    case 2003 :
        nibunno3x = 557584;
        nibunnox3 = 3670663;
        break;
    case 2004 :
        nibunno3x = 556480;
        nibunnox3 = 3666199;
        break;
    case 2005 :
        nibunno3x = 555377;
        nibunnox3 = 3661740;
        break;
    case 2006 :
        nibunno3x = 554274;
        nibunnox3 = 3657281;
        break;
    case 2007 :
        nibunno3x = 553171;
        nibunnox3 = 3652826;
        break;
    case 2008 :
        nibunno3x = 552068;
        nibunnox3 = 3648372;
        break;
    case 2009 :
        nibunno3x = 550968;
        nibunnox3 = 3643926;
        break;
    case 2010 :
        nibunno3x = 549866;
        nibunnox3 = 3639478;
        break;
    case 2011 :
        nibunno3x = 548766;
        nibunnox3 = 3635037;
        break;
    case 2012 :
        nibunno3x = 547664;
        nibunnox3 = 3630594;
        break;
    case 2013 :
        nibunno3x = 546564;
        nibunnox3 = 3626152;
        break;
    case 2014 :
        nibunno3x = 545464;
        nibunnox3 = 3621717;
        break;
    case 2015 :
        nibunno3x = 544364;
        nibunnox3 = 3617283;
        break;
    case 2016 :
        nibunno3x = 543266;
        nibunnox3 = 3612857;
        break;
    case 2017 :
        nibunno3x = 542168;
        nibunnox3 = 3608428;
        break;
    case 2018 :
        nibunno3x = 541069;
        nibunnox3 = 3604000;
        break;
    case 2019 :
        nibunno3x = 539971;
        nibunnox3 = 3599578;
        break;
    case 2020 :
        nibunno3x = 538874;
        nibunnox3 = 3595161;
        break;
    case 2021 :
        nibunno3x = 537777;
        nibunnox3 = 3590743;
        break;
    case 2022 :
        nibunno3x = 536681;
        nibunnox3 = 3586329;
        break;
    case 2023 :
        nibunno3x = 535585;
        nibunnox3 = 3581919;
        break;
    case 2024 :
        nibunno3x = 534488;
        nibunnox3 = 3577508;
        break;
    case 2025 :
        nibunno3x = 533393;
        nibunnox3 = 3573100;
        break;
    case 2026 :
        nibunno3x = 532298;
        nibunnox3 = 3568699;
        break;
    case 2027 :
        nibunno3x = 531204;
        nibunnox3 = 3564297;
        break;
    case 2028 :
        nibunno3x = 530110;
        nibunnox3 = 3559901;
        break;
    case 2029 :
        nibunno3x = 529016;
        nibunnox3 = 3555506;
        break;
    case 2030 :
        nibunno3x = 527923;
        nibunnox3 = 3551114;
        break;
    case 2031 :
        nibunno3x = 526830;
        nibunnox3 = 3546720;
        break;
    case 2032 :
        nibunno3x = 525738;
        nibunnox3 = 3542335;
        break;
    case 2033 :
        nibunno3x = 524646;
        nibunnox3 = 3537951;
        break;
    case 2034 :
        nibunno3x = 523554;
        nibunnox3 = 3533569;
        break;
    case 2035 :
        nibunno3x = 522462;
        nibunnox3 = 3529190;
        break;
    case 2036 :
        nibunno3x = 521371;
        nibunnox3 = 3524812;
        break;
    case 2037 :
        nibunno3x = 520280;
        nibunnox3 = 3520438;
        break;
    case 2038 :
        nibunno3x = 519191;
        nibunnox3 = 3516069;
        break;
    case 2039 :
        nibunno3x = 518101;
        nibunnox3 = 3511701;
        break;
    case 2040 :
        nibunno3x = 517012;
        nibunnox3 = 3507335;
        break;
    case 2041 :
        nibunno3x = 515923;
        nibunnox3 = 3502972;
        break;
    case 2042 :
        nibunno3x = 514834;
        nibunnox3 = 3498610;
        break;
    case 2043 :
        nibunno3x = 513746;
        nibunnox3 = 3494254;
        break;
    case 2044 :
        nibunno3x = 512658;
        nibunnox3 = 3489897;
        break;
    case 2045 :
        nibunno3x = 511572;
        nibunnox3 = 3485547;
        break;
    case 2046 :
        nibunno3x = 510484;
        nibunnox3 = 3481195;
        break;
    case 2047 :
        nibunno3x = 509398;
        nibunnox3 = 3476850;
        break;
    case 2048 :
        nibunno3x = 507769;
        nibunnox3 = 3470334;
        break;
    case 2049 :
        nibunno3x = 505598;
        nibunnox3 = 3461656;
        break;
    case 2050 :
        nibunno3x = 503430;
        nibunnox3 = 3452989;
        break;
    case 2051 :
        nibunno3x = 501262;
        nibunnox3 = 3444331;
        break;
    case 2052 :
        nibunno3x = 499096;
        nibunnox3 = 3435684;
        break;
    case 2053 :
        nibunno3x = 496932;
        nibunnox3 = 3427047;
        break;
    case 2054 :
        nibunno3x = 494769;
        nibunnox3 = 3418424;
        break;
    case 2055 :
        nibunno3x = 492608;
        nibunnox3 = 3409807;
        break;
    case 2056 :
        nibunno3x = 490448;
        nibunnox3 = 3401201;
        break;
    case 2057 :
        nibunno3x = 488290;
        nibunnox3 = 3392607;
        break;
    case 2058 :
        nibunno3x = 486134;
        nibunnox3 = 3384027;
        break;
    case 2059 :
        nibunno3x = 483979;
        nibunnox3 = 3375451;
        break;
    case 2060 :
        nibunno3x = 481826;
        nibunnox3 = 3366888;
        break;
    case 2061 :
        nibunno3x = 479674;
        nibunnox3 = 3358336;
        break;
    case 2062 :
        nibunno3x = 477524;
        nibunnox3 = 3349794;
        break;
    case 2063 :
        nibunno3x = 475375;
        nibunnox3 = 3341261;
        break;
    case 2064 :
        nibunno3x = 473228;
        nibunnox3 = 3332741;
        break;
    case 2065 :
        nibunno3x = 471083;
        nibunnox3 = 3324229;
        break;
    case 2066 :
        nibunno3x = 468938;
        nibunnox3 = 3315727;
        break;
    case 2067 :
        nibunno3x = 466796;
        nibunnox3 = 3307238;
        break;
    case 2068 :
        nibunno3x = 464656;
        nibunnox3 = 3298758;
        break;
    case 2069 :
        nibunno3x = 462516;
        nibunnox3 = 3290286;
        break;
    case 2070 :
        nibunno3x = 460378;
        nibunnox3 = 3281824;
        break;
    case 2071 :
        nibunno3x = 458243;
        nibunnox3 = 3273378;
        break;
    case 2072 :
        nibunno3x = 456108;
        nibunnox3 = 3264936;
        break;
    case 2073 :
        nibunno3x = 453974;
        nibunnox3 = 3256507;
        break;
    case 2074 :
        nibunno3x = 451843;
        nibunnox3 = 3248087;
        break;
    case 2075 :
        nibunno3x = 449713;
        nibunnox3 = 3239679;
        break;
    case 2076 :
        nibunno3x = 447584;
        nibunnox3 = 3231279;
        break;
    case 2077 :
        nibunno3x = 445458;
        nibunnox3 = 3222893;
        break;
    case 2078 :
        nibunno3x = 443333;
        nibunnox3 = 3214514;
        break;
    case 2079 :
        nibunno3x = 441209;
        nibunnox3 = 3206145;
        break;
    case 2080 :
        nibunno3x = 439086;
        nibunnox3 = 3197785;
        break;
    case 2081 :
        nibunno3x = 436966;
        nibunnox3 = 3189439;
        break;
    case 2082 :
        nibunno3x = 434846;
        nibunnox3 = 3181099;
        break;
    case 2083 :
        nibunno3x = 432728;
        nibunnox3 = 3172769;
        break;
    case 2084 :
        nibunno3x = 430613;
        nibunnox3 = 3164453;
        break;
    case 2085 :
        nibunno3x = 428498;
        nibunnox3 = 3156145;
        break;
    case 2086 :
        nibunno3x = 426385;
        nibunnox3 = 3147846;
        break;
    case 2087 :
        nibunno3x = 424273;
        nibunnox3 = 3139556;
        break;
    case 2088 :
        nibunno3x = 422163;
        nibunnox3 = 3131278;
        break;
    case 2089 :
        nibunno3x = 420055;
        nibunnox3 = 3123011;
        break;
    case 2090 :
        nibunno3x = 417948;
        nibunnox3 = 3114750;
        break;
    case 2091 :
        nibunno3x = 415842;
        nibunnox3 = 3106500;
        break;
    case 2092 :
        nibunno3x = 413738;
        nibunnox3 = 3098261;
        break;
    case 2093 :
        nibunno3x = 411636;
        nibunnox3 = 3090033;
        break;
    case 2094 :
        nibunno3x = 409534;
        nibunnox3 = 3081812;
        break;
    case 2095 :
        nibunno3x = 407434;
        nibunnox3 = 3073601;
        break;
    case 2096 :
        nibunno3x = 405336;
        nibunnox3 = 3065402;
        break;
    case 2097 :
        nibunno3x = 403240;
        nibunnox3 = 3057212;
        break;
    case 2098 :
        nibunno3x = 401145;
        nibunnox3 = 3049033;
        break;
    case 2099 :
        nibunno3x = 399051;
        nibunnox3 = 3040861;
        break;
    case 2100 :
        nibunno3x = 396960;
        nibunnox3 = 3032701;
        break;
    case 2101 :
        nibunno3x = 394868;
        nibunnox3 = 3024548;
        break;
    case 2102 :
        nibunno3x = 392780;
        nibunnox3 = 3016407;
        break;
    case 2103 :
        nibunno3x = 390692;
        nibunnox3 = 3008277;
        break;
    case 2104 :
        nibunno3x = 388606;
        nibunnox3 = 3000153;
        break;
    case 2105 :
        nibunno3x = 386521;
        nibunnox3 = 2992038;
        break;
    case 2106 :
        nibunno3x = 384439;
        nibunnox3 = 2983940;
        break;
    case 2107 :
        nibunno3x = 382357;
        nibunnox3 = 2975846;
        break;
    case 2108 :
        nibunno3x = 380276;
        nibunnox3 = 2967760;
        break;
    case 2109 :
        nibunno3x = 378198;
        nibunnox3 = 2959685;
        break;
    case 2110 :
        nibunno3x = 376121;
        nibunnox3 = 2951622;
        break;
    case 2111 :
        nibunno3x = 374046;
        nibunnox3 = 2943568;
        break;
    case 2112 :
        nibunno3x = 371971;
        nibunnox3 = 2935522;
        break;
    case 2113 :
        nibunno3x = 369898;
        nibunnox3 = 2927485;
        break;
    case 2114 :
        nibunno3x = 367826;
        nibunnox3 = 2919457;
        break;
    case 2115 :
        nibunno3x = 365757;
        nibunnox3 = 2911443;
        break;
    case 2116 :
        nibunno3x = 363689;
        nibunnox3 = 2903435;
        break;
    case 2117 :
        nibunno3x = 361621;
        nibunnox3 = 2895433;
        break;
    case 2118 :
        nibunno3x = 359556;
        nibunnox3 = 2887447;
        break;
    case 2119 :
        nibunno3x = 357492;
        nibunnox3 = 2879468;
        break;
    case 2120 :
        nibunno3x = 355430;
        nibunnox3 = 2871498;
        break;
    case 2121 :
        nibunno3x = 353368;
        nibunnox3 = 2863535;
        break;
    case 2122 :
        nibunno3x = 351308;
        nibunnox3 = 2855584;
        break;
    case 2123 :
        nibunno3x = 349250;
        nibunnox3 = 2847643;
        break;
    case 2124 :
        nibunno3x = 347194;
        nibunnox3 = 2839711;
        break;
    case 2125 :
        nibunno3x = 345139;
        nibunnox3 = 2831789;
        break;
    case 2126 :
        nibunno3x = 343086;
        nibunnox3 = 2823877;
        break;
    case 2127 :
        nibunno3x = 341032;
        nibunnox3 = 2815968;
        break;
    case 2128 :
        nibunno3x = 338982;
        nibunnox3 = 2808074;
        break;
    case 2129 :
        nibunno3x = 336932;
        nibunnox3 = 2800190;
        break;
    case 2130 :
        nibunno3x = 334884;
        nibunnox3 = 2792313;
        break;
    case 2131 :
        nibunno3x = 332838;
        nibunnox3 = 2784444;
        break;
    case 2132 :
        nibunno3x = 330793;
        nibunnox3 = 2776589;
        break;
    case 2133 :
        nibunno3x = 328748;
        nibunnox3 = 2768737;
        break;
    case 2134 :
        nibunno3x = 326707;
        nibunnox3 = 2760899;
        break;
    case 2135 :
        nibunno3x = 324666;
        nibunnox3 = 2753069;
        break;
    case 2136 :
        nibunno3x = 322627;
        nibunnox3 = 2745248;
        break;
    case 2137 :
        nibunno3x = 320588;
        nibunnox3 = 2737433;
        break;
    case 2138 :
        nibunno3x = 318552;
        nibunnox3 = 2729629;
        break;
    case 2139 :
        nibunno3x = 316517;
        nibunnox3 = 2721836;
        break;
    case 2140 :
        nibunno3x = 314484;
        nibunnox3 = 2714053;
        break;
    case 2141 :
        nibunno3x = 312451;
        nibunnox3 = 2706275;
        break;
    case 2142 :
        nibunno3x = 310420;
        nibunnox3 = 2698508;
        break;
    case 2143 :
        nibunno3x = 308390;
        nibunnox3 = 2690750;
        break;
    case 2144 :
        nibunno3x = 306363;
        nibunnox3 = 2683004;
        break;
    case 2145 :
        nibunno3x = 304336;
        nibunnox3 = 2675262;
        break;
    case 2146 :
        nibunno3x = 302311;
        nibunnox3 = 2667532;
        break;
    case 2147 :
        nibunno3x = 300288;
        nibunnox3 = 2659811;
        break;
    case 2148 :
        nibunno3x = 298266;
        nibunnox3 = 2652100;
        break;
    case 2149 :
        nibunno3x = 296244;
        nibunnox3 = 2644395;
        break;
    case 2150 :
        nibunno3x = 294224;
        nibunnox3 = 2636699;
        break;
    case 2151 :
        nibunno3x = 292207;
        nibunnox3 = 2629016;
        break;
    case 2152 :
        nibunno3x = 290190;
        nibunnox3 = 2621338;
        break;
    case 2153 :
        nibunno3x = 288175;
        nibunnox3 = 2613670;
        break;
    case 2154 :
        nibunno3x = 286160;
        nibunnox3 = 2606009;
        break;
    case 2155 :
        nibunno3x = 284149;
        nibunnox3 = 2598363;
        break;
    case 2156 :
        nibunno3x = 282138;
        nibunnox3 = 2590719;
        break;
    case 2157 :
        nibunno3x = 280128;
        nibunnox3 = 2583086;
        break;
    case 2158 :
        nibunno3x = 278119;
        nibunnox3 = 2575461;
        break;
    case 2159 :
        nibunno3x = 276112;
        nibunnox3 = 2567846;
        break;
    case 2160 :
        nibunno3x = 274107;
        nibunnox3 = 2560243;
        break;
    case 2161 :
        nibunno3x = 272102;
        nibunnox3 = 2552643;
        break;
    case 2162 :
        nibunno3x = 270100;
        nibunnox3 = 2545055;
        break;
    case 2163 :
        nibunno3x = 268099;
        nibunnox3 = 2537477;
        break;
    case 2164 :
        nibunno3x = 266099;
        nibunnox3 = 2529905;
        break;
    case 2165 :
        nibunno3x = 264100;
        nibunnox3 = 2522342;
        break;
    case 2166 :
        nibunno3x = 262104;
        nibunnox3 = 2514791;
        break;
    case 2167 :
        nibunno3x = 260107;
        nibunnox3 = 2507243;
        break;
    case 2168 :
        nibunno3x = 258114;
        nibunnox3 = 2499711;
        break;
    case 2169 :
        nibunno3x = 256120;
        nibunnox3 = 2492182;
        break;
    case 2170 :
        nibunno3x = 254128;
        nibunnox3 = 2484662;
        break;
    case 2171 :
        nibunno3x = 252138;
        nibunnox3 = 2477153;
        break;
    case 2172 :
        nibunno3x = 250149;
        nibunnox3 = 2469652;
        break;
    case 2173 :
        nibunno3x = 248161;
        nibunnox3 = 2462156;
        break;
    case 2174 :
        nibunno3x = 246175;
        nibunnox3 = 2454673;
        break;
    case 2175 :
        nibunno3x = 244190;
        nibunnox3 = 2447198;
        break;
    case 2176 :
        nibunno3x = 242208;
        nibunnox3 = 2439733;
        break;
    case 2177 :
        nibunno3x = 240226;
        nibunnox3 = 2432276;
        break;
    case 2178 :
        nibunno3x = 238244;
        nibunnox3 = 2424824;
        break;
    case 2179 :
        nibunno3x = 236265;
        nibunnox3 = 2417382;
        break;
    case 2180 :
        nibunno3x = 234288;
        nibunnox3 = 2409950;
        break;
    case 2181 :
        nibunno3x = 232310;
        nibunnox3 = 2402525;
        break;
    case 2182 :
        nibunno3x = 230335;
        nibunnox3 = 2395108;
        break;
    case 2183 :
        nibunno3x = 228361;
        nibunnox3 = 2387700;
        break;
    case 2184 :
        nibunno3x = 226389;
        nibunnox3 = 2380304;
        break;
    case 2185 :
        nibunno3x = 224418;
        nibunnox3 = 2372912;
        break;
    case 2186 :
        nibunno3x = 222448;
        nibunnox3 = 2365531;
        break;
    case 2187 :
        nibunno3x = 220480;
        nibunnox3 = 2358160;
        break;
    case 2188 :
        nibunno3x = 218512;
        nibunnox3 = 2350791;
        break;
    case 2189 :
        nibunno3x = 216547;
        nibunnox3 = 2343438;
        break;
    case 2190 :
        nibunno3x = 214582;
        nibunnox3 = 2336087;
        break;
    case 2191 :
        nibunno3x = 212618;
        nibunnox3 = 2328746;
        break;
    case 2192 :
        nibunno3x = 210658;
        nibunnox3 = 2321419;
        break;
    case 2193 :
        nibunno3x = 208697;
        nibunnox3 = 2314093;
        break;
    case 2194 :
        nibunno3x = 206738;
        nibunnox3 = 2306781;
        break;
    case 2195 :
        nibunno3x = 204781;
        nibunnox3 = 2299475;
        break;
    case 2196 :
        nibunno3x = 202824;
        nibunnox3 = 2292173;
        break;
    case 2197 :
        nibunno3x = 200869;
        nibunnox3 = 2284885;
        break;
    case 2198 :
        nibunno3x = 198915;
        nibunnox3 = 2277604;
        break;
    case 2199 :
        nibunno3x = 196963;
        nibunnox3 = 2270330;
        break;
    case 2200 :
        nibunno3x = 195012;
        nibunnox3 = 2263064;
        break;
    case 2201 :
        nibunno3x = 193062;
        nibunnox3 = 2255806;
        break;
    case 2202 :
        nibunno3x = 191114;
        nibunnox3 = 2248560;
        break;
    case 2203 :
        nibunno3x = 189166;
        nibunnox3 = 2241318;
        break;
    case 2204 :
        nibunno3x = 187220;
        nibunnox3 = 2234086;
        break;
    case 2205 :
        nibunno3x = 185276;
        nibunnox3 = 2226864;
        break;
    case 2206 :
        nibunno3x = 183333;
        nibunnox3 = 2219648;
        break;
    case 2207 :
        nibunno3x = 181392;
        nibunnox3 = 2212440;
        break;
    case 2208 :
        nibunno3x = 179451;
        nibunnox3 = 2205241;
        break;
    case 2209 :
        nibunno3x = 177512;
        nibunnox3 = 2198049;
        break;
    case 2210 :
        nibunno3x = 175574;
        nibunnox3 = 2190868;
        break;
    case 2211 :
        nibunno3x = 173638;
        nibunnox3 = 2183693;
        break;
    case 2212 :
        nibunno3x = 171703;
        nibunnox3 = 2176526;
        break;
    case 2213 :
        nibunno3x = 169769;
        nibunnox3 = 2169367;
        break;
    case 2214 :
        nibunno3x = 167836;
        nibunnox3 = 2162215;
        break;
    case 2215 :
        nibunno3x = 165906;
        nibunnox3 = 2155075;
        break;
    case 2216 :
        nibunno3x = 163975;
        nibunnox3 = 2147939;
        break;
    case 2217 :
        nibunno3x = 162046;
        nibunnox3 = 2140811;
        break;
    case 2218 :
        nibunno3x = 160119;
        nibunnox3 = 2133695;
        break;
    case 2219 :
        nibunno3x = 158192;
        nibunnox3 = 2126582;
        break;
    case 2220 :
        nibunno3x = 156268;
        nibunnox3 = 2119480;
        break;
    case 2221 :
        nibunno3x = 154345;
        nibunnox3 = 2112387;
        break;
    case 2222 :
        nibunno3x = 152422;
        nibunnox3 = 2105297;
        break;
    case 2223 :
        nibunno3x = 150502;
        nibunnox3 = 2098222;
        break;
    case 2224 :
        nibunno3x = 148582;
        nibunnox3 = 2091150;
        break;
    case 2225 :
        nibunno3x = 146664;
        nibunnox3 = 2084087;
        break;
    case 2226 :
        nibunno3x = 144747;
        nibunnox3 = 2077034;
        break;
    case 2227 :
        nibunno3x = 142831;
        nibunnox3 = 2069986;
        break;
    case 2228 :
        nibunno3x = 140917;
        nibunnox3 = 2062948;
        break;
    case 2229 :
        nibunno3x = 139003;
        nibunnox3 = 2055914;
        break;
    case 2230 :
        nibunno3x = 137092;
        nibunnox3 = 2048893;
        break;
    case 2231 :
        nibunno3x = 135181;
        nibunnox3 = 2041876;
        break;
    case 2232 :
        nibunno3x = 133272;
        nibunnox3 = 2034871;
        break;
    case 2233 :
        nibunno3x = 131364;
        nibunnox3 = 2027871;
        break;
    case 2234 :
        nibunno3x = 129457;
        nibunnox3 = 2020877;
        break;
    case 2235 :
        nibunno3x = 127552;
        nibunnox3 = 2013895;
        break;
    case 2236 :
        nibunno3x = 125648;
        nibunnox3 = 2006921;
        break;
    case 2237 :
        nibunno3x = 123745;
        nibunnox3 = 1999950;
        break;
    case 2238 :
        nibunno3x = 121843;
        nibunnox3 = 1992989;
        break;
    case 2239 :
        nibunno3x = 119943;
        nibunnox3 = 1986038;
        break;
    case 2240 :
        nibunno3x = 118044;
        nibunnox3 = 1979093;
        break;
    case 2241 :
        nibunno3x = 116147;
        nibunnox3 = 1972156;
        break;
    case 2242 :
        nibunno3x = 114250;
        nibunnox3 = 1965226;
        break;
    case 2243 :
        nibunno3x = 112355;
        nibunnox3 = 1958304;
        break;
    case 2244 :
        nibunno3x = 110461;
        nibunnox3 = 1951391;
        break;
    case 2245 :
        nibunno3x = 108568;
        nibunnox3 = 1944483;
        break;
    case 2246 :
        nibunno3x = 106676;
        nibunnox3 = 1937584;
        break;
    case 2247 :
        nibunno3x = 104786;
        nibunnox3 = 1930693;
        break;
    case 2248 :
        nibunno3x = 102898;
        nibunnox3 = 1923811;
        break;
    case 2249 :
        nibunno3x = 101010;
        nibunnox3 = 1916936;
        break;
    case 2250 :
        nibunno3x = 99124;
        nibunnox3 = 1910068;
        break;
    case 2251 :
        nibunno3x = 97238;
        nibunnox3 = 1903206;
        break;
    case 2252 :
        nibunno3x = 95354;
        nibunnox3 = 1896353;
        break;
    case 2253 :
        nibunno3x = 93472;
        nibunnox3 = 1889508;
        break;
    case 2254 :
        nibunno3x = 91591;
        nibunnox3 = 1882672;
        break;
    case 2255 :
        nibunno3x = 89712;
        nibunnox3 = 1875844;
        break;
    case 2256 :
        nibunno3x = 87832;
        nibunnox3 = 1869018;
        break;
    case 2257 :
        nibunno3x = 85955;
        nibunnox3 = 1862205;
        break;
    case 2258 :
        nibunno3x = 84078;
        nibunnox3 = 1855397;
        break;
    case 2259 :
        nibunno3x = 82202;
        nibunnox3 = 1848595;
        break;
    case 2260 :
        nibunno3x = 80329;
        nibunnox3 = 1841804;
        break;
    case 2261 :
        nibunno3x = 78456;
        nibunnox3 = 1835019;
        break;
    case 2262 :
        nibunno3x = 76585;
        nibunnox3 = 1828242;
        break;
    case 2263 :
        nibunno3x = 74715;
        nibunnox3 = 1821473;
        break;
    case 2264 :
        nibunno3x = 72846;
        nibunnox3 = 1814708;
        break;
    case 2265 :
        nibunno3x = 70978;
        nibunnox3 = 1807954;
        break;
    case 2266 :
        nibunno3x = 69112;
        nibunnox3 = 1801208;
        break;
    case 2267 :
        nibunno3x = 67247;
        nibunnox3 = 1794467;
        break;
    case 2268 :
        nibunno3x = 65383;
        nibunnox3 = 1787736;
        break;
    case 2269 :
        nibunno3x = 63520;
        nibunnox3 = 1781009;
        break;
    case 2270 :
        nibunno3x = 61658;
        nibunnox3 = 1774292;
        break;
    case 2271 :
        nibunno3x = 59798;
        nibunnox3 = 1767582;
        break;
    case 2272 :
        nibunno3x = 57939;
        nibunnox3 = 1760879;
        break;
    case 2273 :
        nibunno3x = 56082;
        nibunnox3 = 1754183;
        break;
    case 2274 :
        nibunno3x = 54224;
        nibunnox3 = 1747493;
        break;
    case 2275 :
        nibunno3x = 52369;
        nibunnox3 = 1740812;
        break;
    case 2276 :
        nibunno3x = 50515;
        nibunnox3 = 1734139;
        break;
    case 2277 :
        nibunno3x = 48662;
        nibunnox3 = 1727474;
        break;
    case 2278 :
        nibunno3x = 46812;
        nibunnox3 = 1720817;
        break;
    case 2279 :
        nibunno3x = 44960;
        nibunnox3 = 1714164;
        break;
    case 2280 :
        nibunno3x = 43111;
        nibunnox3 = 1707518;
        break;
    case 2281 :
        nibunno3x = 41263;
        nibunnox3 = 1700882;
        break;
    case 2282 :
        nibunno3x = 39416;
        nibunnox3 = 1694253;
        break;
    case 2283 :
        nibunno3x = 37570;
        nibunnox3 = 1687627;
        break;
    case 2284 :
        nibunno3x = 35726;
        nibunnox3 = 1681015;
        break;
    case 2285 :
        nibunno3x = 33883;
        nibunnox3 = 1674406;
        break;
    case 2286 :
        nibunno3x = 32041;
        nibunnox3 = 1667805;
        break;
    case 2287 :
        nibunno3x = 30200;
        nibunnox3 = 1661210;
        break;
    case 2288 :
        nibunno3x = 28360;
        nibunnox3 = 1654623;
        break;
    case 2289 :
        nibunno3x = 26522;
        nibunnox3 = 1648044;
        break;
    case 2290 :
        nibunno3x = 24685;
        nibunnox3 = 1641473;
        break;
    case 2291 :
        nibunno3x = 22849;
        nibunnox3 = 1634908;
        break;
    case 2292 :
        nibunno3x = 21014;
        nibunnox3 = 1628352;
        break;
    case 2293 :
        nibunno3x = 19180;
        nibunnox3 = 1621797;
        break;
    case 2294 :
        nibunno3x = 17348;
        nibunnox3 = 1615257;
        break;
    case 2295 :
        nibunno3x = 15517;
        nibunnox3 = 1608719;
        break;
    case 2296 :
        nibunno3x = 13687;
        nibunnox3 = 1602190;
        break;
    case 2297 :
        nibunno3x = 11858;
        nibunnox3 = 1595669;
        break;
    case 2298 :
        nibunno3x = 10031;
        nibunnox3 = 1589153;
        break;
    case 2299 :
        nibunno3x = 8204;
        nibunnox3 = 1582646;
        break;
    case 2300 :
        nibunno3x = 6379;
        nibunnox3 = 1576144;
        break;
    case 2301 :
        nibunno3x = 4555;
        nibunnox3 = 1569650;
        break;
    case 2302 :
        nibunno3x = 2732;
        nibunnox3 = 1563165;
        break;
    case 2303 :
        nibunno3x = 912;
        nibunnox3 = 1556687;
        break;
    case 2304 :
        nibunno3x = 8386789;
        nibunnox3 = 1550212;
        break;
    case 2305 :
        nibunno3x = 8383150;
        nibunnox3 = 1543746;
        break;
    case 2306 :
        nibunno3x = 8379514;
        nibunnox3 = 1537287;
        break;
    case 2307 :
        nibunno3x = 8375881;
        nibunnox3 = 1530838;
        break;
    case 2308 :
        nibunno3x = 8372248;
        nibunnox3 = 1524390;
        break;
    case 2309 :
        nibunno3x = 8368620;
        nibunnox3 = 1517953;
        break;
    case 2310 :
        nibunno3x = 8364994;
        nibunnox3 = 1511525;
        break;
    case 2311 :
        nibunno3x = 8361368;
        nibunnox3 = 1505098;
        break;
    case 2312 :
        nibunno3x = 8357746;
        nibunnox3 = 1498681;
        break;
    case 2313 :
        nibunno3x = 8354128;
        nibunnox3 = 1492274;
        break;
    case 2314 :
        nibunno3x = 8350510;
        nibunnox3 = 1485870;
        break;
    case 2315 :
        nibunno3x = 8346895;
        nibunnox3 = 1479474;
        break;
    case 2316 :
        nibunno3x = 8343283;
        nibunnox3 = 1473086;
        break;
    case 2317 :
        nibunno3x = 8339671;
        nibunnox3 = 1466701;
        break;
    case 2318 :
        nibunno3x = 8336065;
        nibunnox3 = 1460328;
        break;
    case 2319 :
        nibunno3x = 8332459;
        nibunnox3 = 1453959;
        break;
    case 2320 :
        nibunno3x = 8328856;
        nibunnox3 = 1447598;
        break;
    case 2321 :
        nibunno3x = 8325253;
        nibunnox3 = 1441239;
        break;
    case 2322 :
        nibunno3x = 8321656;
        nibunnox3 = 1434895;
        break;
    case 2323 :
        nibunno3x = 8318059;
        nibunnox3 = 1428552;
        break;
    case 2324 :
        nibunno3x = 8314465;
        nibunnox3 = 1422218;
        break;
    case 2325 :
        nibunno3x = 8310874;
        nibunnox3 = 1415891;
        break;
    case 2326 :
        nibunno3x = 8307283;
        nibunnox3 = 1409568;
        break;
    case 2327 :
        nibunno3x = 8303698;
        nibunnox3 = 1403258;
        break;
    case 2328 :
        nibunno3x = 8300113;
        nibunnox3 = 1396950;
        break;
    case 2329 :
        nibunno3x = 8296531;
        nibunnox3 = 1390650;
        break;
    case 2330 :
        nibunno3x = 8292949;
        nibunnox3 = 1384353;
        break;
    case 2331 :
        nibunno3x = 8289372;
        nibunnox3 = 1378067;
        break;
    case 2332 :
        nibunno3x = 8285796;
        nibunnox3 = 1371786;
        break;
    case 2333 :
        nibunno3x = 8282222;
        nibunnox3 = 1365513;
        break;
    case 2334 :
        nibunno3x = 8278651;
        nibunnox3 = 1359245;
        break;
    case 2335 :
        nibunno3x = 8275084;
        nibunnox3 = 1352988;
        break;
    case 2336 :
        nibunno3x = 8271517;
        nibunnox3 = 1346734;
        break;
    case 2337 :
        nibunno3x = 8267953;
        nibunnox3 = 1340487;
        break;
    case 2338 :
        nibunno3x = 8264392;
        nibunnox3 = 1334248;
        break;
    case 2339 :
        nibunno3x = 8260831;
        nibunnox3 = 1328013;
        break;
    case 2340 :
        nibunno3x = 8257274;
        nibunnox3 = 1321787;
        break;
    case 2341 :
        nibunno3x = 8253721;
        nibunnox3 = 1315570;
        break;
    case 2342 :
        nibunno3x = 8250168;
        nibunnox3 = 1309355;
        break;
    case 2343 :
        nibunno3x = 8246617;
        nibunnox3 = 1303148;
        break;
    case 2344 :
        nibunno3x = 8243068;
        nibunnox3 = 1296946;
        break;
    case 2345 :
        nibunno3x = 8239524;
        nibunnox3 = 1290755;
        break;
    case 2346 :
        nibunno3x = 8235979;
        nibunnox3 = 1284567;
        break;
    case 2347 :
        nibunno3x = 8232439;
        nibunnox3 = 1278388;
        break;
    case 2348 :
        nibunno3x = 8228899;
        nibunnox3 = 1272213;
        break;
    case 2349 :
        nibunno3x = 8225362;
        nibunnox3 = 1266046;
        break;
    case 2350 :
        nibunno3x = 8221828;
        nibunnox3 = 1259886;
        break;
    case 2351 :
        nibunno3x = 8218296;
        nibunnox3 = 1253731;
        break;
    case 2352 :
        nibunno3x = 8214766;
        nibunnox3 = 1247585;
        break;
    case 2353 :
        nibunno3x = 8211238;
        nibunnox3 = 1241443;
        break;
    case 2354 :
        nibunno3x = 8207713;
        nibunnox3 = 1235310;
        break;
    case 2355 :
        nibunno3x = 8204188;
        nibunnox3 = 1229179;
        break;
    case 2356 :
        nibunno3x = 8200669;
        nibunnox3 = 1223061;
        break;
    case 2357 :
        nibunno3x = 8197150;
        nibunnox3 = 1216945;
        break;
    case 2358 :
        nibunno3x = 8193634;
        nibunnox3 = 1210838;
        break;
    case 2359 :
        nibunno3x = 8190118;
        nibunnox3 = 1204733;
        break;
    case 2360 :
        nibunno3x = 8186606;
        nibunnox3 = 1198639;
        break;
    case 2361 :
        nibunno3x = 8183095;
        nibunnox3 = 1192547;
        break;
    case 2362 :
        nibunno3x = 8179588;
        nibunnox3 = 1186465;
        break;
    case 2363 :
        nibunno3x = 8176084;
        nibunnox3 = 1180391;
        break;
    case 2364 :
        nibunno3x = 8172580;
        nibunnox3 = 1174320;
        break;
    case 2365 :
        nibunno3x = 8169079;
        nibunnox3 = 1168256;
        break;
    case 2366 :
        nibunno3x = 8165581;
        nibunnox3 = 1162201;
        break;
    case 2367 :
        nibunno3x = 8162083;
        nibunnox3 = 1156147;
        break;
    case 2368 :
        nibunno3x = 8158590;
        nibunnox3 = 1150105;
        break;
    case 2369 :
        nibunno3x = 8155098;
        nibunnox3 = 1144067;
        break;
    case 2370 :
        nibunno3x = 8151608;
        nibunnox3 = 1138037;
        break;
    case 2371 :
        nibunno3x = 8148121;
        nibunnox3 = 1132013;
        break;
    case 2372 :
        nibunno3x = 8144635;
        nibunnox3 = 1125993;
        break;
    case 2373 :
        nibunno3x = 8141152;
        nibunnox3 = 1119981;
        break;
    case 2374 :
        nibunno3x = 8137670;
        nibunnox3 = 1113974;
        break;
    case 2375 :
        nibunno3x = 8134192;
        nibunnox3 = 1107975;
        break;
    case 2376 :
        nibunno3x = 8130715;
        nibunnox3 = 1101981;
        break;
    case 2377 :
        nibunno3x = 8127241;
        nibunnox3 = 1095995;
        break;
    case 2378 :
        nibunno3x = 8123768;
        nibunnox3 = 1090013;
        break;
    case 2379 :
        nibunno3x = 8120299;
        nibunnox3 = 1084040;
        break;
    case 2380 :
        nibunno3x = 8116831;
        nibunnox3 = 1078071;
        break;
    case 2381 :
        nibunno3x = 8113364;
        nibunnox3 = 1072108;
        break;
    case 2382 :
        nibunno3x = 8109901;
        nibunnox3 = 1066152;
        break;
    case 2383 :
        nibunno3x = 8106439;
        nibunnox3 = 1060201;
        break;
    case 2384 :
        nibunno3x = 8102980;
        nibunnox3 = 1054258;
        break;
    case 2385 :
        nibunno3x = 8099524;
        nibunnox3 = 1048323;
        break;
    case 2386 :
        nibunno3x = 8096068;
        nibunnox3 = 1042390;
        break;
    case 2387 :
        nibunno3x = 8092615;
        nibunnox3 = 1036465;
        break;
    case 2388 :
        nibunno3x = 8089165;
        nibunnox3 = 1030547;
        break;
    case 2389 :
        nibunno3x = 8085718;
        nibunnox3 = 1024637;
        break;
    case 2390 :
        nibunno3x = 8082271;
        nibunnox3 = 1018730;
        break;
    case 2391 :
        nibunno3x = 8078827;
        nibunnox3 = 1012830;
        break;
    case 2392 :
        nibunno3x = 8075386;
        nibunnox3 = 1006938;
        break;
    case 2393 :
        nibunno3x = 8071945;
        nibunnox3 = 1001048;
        break;
    case 2394 :
        nibunno3x = 8068507;
        nibunnox3 = 995166;
        break;
    case 2395 :
        nibunno3x = 8065072;
        nibunnox3 = 989292;
        break;
    case 2396 :
        nibunno3x = 8061640;
        nibunnox3 = 983425;
        break;
    case 2397 :
        nibunno3x = 8058208;
        nibunnox3 = 977560;
        break;
    case 2398 :
        nibunno3x = 8054779;
        nibunnox3 = 971703;
        break;
    case 2399 :
        nibunno3x = 8051353;
        nibunnox3 = 965853;
        break;
    case 2400 :
        nibunno3x = 8047927;
        nibunnox3 = 960006;
        break;
    case 2401 :
        nibunno3x = 8044506;
        nibunnox3 = 954169;
        break;
    case 2402 :
        nibunno3x = 8041084;
        nibunnox3 = 948335;
        break;
    case 2403 :
        nibunno3x = 8037667;
        nibunnox3 = 942510;
        break;
    case 2404 :
        nibunno3x = 8034250;
        nibunnox3 = 936688;
        break;
    case 2405 :
        nibunno3x = 8030836;
        nibunnox3 = 930874;
        break;
    case 2406 :
        nibunno3x = 8027425;
        nibunnox3 = 925067;
        break;
    case 2407 :
        nibunno3x = 8024016;
        nibunnox3 = 919265;
        break;
    case 2408 :
        nibunno3x = 8020608;
        nibunnox3 = 913468;
        break;
    case 2409 :
        nibunno3x = 8017202;
        nibunnox3 = 907679;
        break;
    case 2410 :
        nibunno3x = 8013799;
        nibunnox3 = 901894;
        break;
    case 2411 :
        nibunno3x = 8010397;
        nibunnox3 = 896114;
        break;
    case 2412 :
        nibunno3x = 8006998;
        nibunnox3 = 890342;
        break;
    case 2413 :
        nibunno3x = 8003602;
        nibunnox3 = 884577;
        break;
    case 2414 :
        nibunno3x = 8000208;
        nibunnox3 = 878818;
        break;
    case 2415 :
        nibunno3x = 7996813;
        nibunnox3 = 873060;
        break;
    case 2416 :
        nibunno3x = 7993423;
        nibunnox3 = 867313;
        break;
    case 2417 :
        nibunno3x = 7990034;
        nibunnox3 = 861571;
        break;
    case 2418 :
        nibunno3x = 7986649;
        nibunnox3 = 855836;
        break;
    case 2419 :
        nibunno3x = 7983265;
        nibunnox3 = 850106;
        break;
    case 2420 :
        nibunno3x = 7979881;
        nibunnox3 = 844378;
        break;
    case 2421 :
        nibunno3x = 7976502;
        nibunnox3 = 838661;
        break;
    case 2422 :
        nibunno3x = 7973124;
        nibunnox3 = 832948;
        break;
    case 2423 :
        nibunno3x = 7969747;
        nibunnox3 = 827240;
        break;
    case 2424 :
        nibunno3x = 7966374;
        nibunnox3 = 821539;
        break;
    case 2425 :
        nibunno3x = 7963000;
        nibunnox3 = 815842;
        break;
    case 2426 :
        nibunno3x = 7959631;
        nibunnox3 = 810154;
        break;
    case 2427 :
        nibunno3x = 7956265;
        nibunnox3 = 804473;
        break;
    case 2428 :
        nibunno3x = 7952899;
        nibunnox3 = 798794;
        break;
    case 2429 :
        nibunno3x = 7949536;
        nibunnox3 = 793124;
        break;
    case 2430 :
        nibunno3x = 7946174;
        nibunnox3 = 787458;
        break;
    case 2431 :
        nibunno3x = 7942813;
        nibunnox3 = 781794;
        break;
    case 2432 :
        nibunno3x = 7939456;
        nibunnox3 = 776140;
        break;
    case 2433 :
        nibunno3x = 7936102;
        nibunnox3 = 770493;
        break;
    case 2434 :
        nibunno3x = 7932748;
        nibunnox3 = 764849;
        break;
    case 2435 :
        nibunno3x = 7929398;
        nibunnox3 = 759215;
        break;
    case 2436 :
        nibunno3x = 7926049;
        nibunnox3 = 753583;
        break;
    case 2437 :
        nibunno3x = 7922701;
        nibunnox3 = 747956;
        break;
    case 2438 :
        nibunno3x = 7919356;
        nibunnox3 = 742336;
        break;
    case 2439 :
        nibunno3x = 7916014;
        nibunnox3 = 736723;
        break;
    case 2440 :
        nibunno3x = 7912674;
        nibunnox3 = 731115;
        break;
    case 2441 :
        nibunno3x = 7909334;
        nibunnox3 = 725512;
        break;
    case 2442 :
        nibunno3x = 7905998;
        nibunnox3 = 719917;
        break;
    case 2443 :
        nibunno3x = 7902664;
        nibunnox3 = 714327;
        break;
    case 2444 :
        nibunno3x = 7899331;
        nibunnox3 = 708741;
        break;
    case 2445 :
        nibunno3x = 7896001;
        nibunnox3 = 703162;
        break;
    case 2446 :
        nibunno3x = 7892671;
        nibunnox3 = 697585;
        break;
    case 2447 :
        nibunno3x = 7889346;
        nibunnox3 = 692019;
        break;
    case 2448 :
        nibunno3x = 7886020;
        nibunnox3 = 686455;
        break;
    case 2449 :
        nibunno3x = 7882699;
        nibunnox3 = 680901;
        break;
    case 2450 :
        nibunno3x = 7879378;
        nibunnox3 = 675348;
        break;
    case 2451 :
        nibunno3x = 7876060;
        nibunnox3 = 669803;
        break;
    case 2452 :
        nibunno3x = 7872744;
        nibunnox3 = 664263;
        break;
    case 2453 :
        nibunno3x = 7869430;
        nibunnox3 = 658730;
        break;
    case 2454 :
        nibunno3x = 7866118;
        nibunnox3 = 653202;
        break;
    case 2455 :
        nibunno3x = 7862806;
        nibunnox3 = 647677;
        break;
    case 2456 :
        nibunno3x = 7859498;
        nibunnox3 = 642160;
        break;
    case 2457 :
        nibunno3x = 7856192;
        nibunnox3 = 636649;
        break;
    case 2458 :
        nibunno3x = 7852888;
        nibunnox3 = 631142;
        break;
    case 2459 :
        nibunno3x = 7849586;
        nibunnox3 = 625643;
        break;
    case 2460 :
        nibunno3x = 7846286;
        nibunnox3 = 620148;
        break;
    case 2461 :
        nibunno3x = 7842988;
        nibunnox3 = 614659;
        break;
    case 2462 :
        nibunno3x = 7839691;
        nibunnox3 = 609173;
        break;
    case 2463 :
        nibunno3x = 7836398;
        nibunnox3 = 603698;
        break;
    case 2464 :
        nibunno3x = 7833106;
        nibunnox3 = 598224;
        break;
    case 2465 :
        nibunno3x = 7829816;
        nibunnox3 = 592758;
        break;
    case 2466 :
        nibunno3x = 7826528;
        nibunnox3 = 587297;
        break;
    case 2467 :
        nibunno3x = 7823242;
        nibunnox3 = 581841;
        break;
    case 2468 :
        nibunno3x = 7819957;
        nibunnox3 = 576388;
        break;
    case 2469 :
        nibunno3x = 7816675;
        nibunnox3 = 570944;
        break;
    case 2470 :
        nibunno3x = 7813396;
        nibunnox3 = 565506;
        break;
    case 2471 :
        nibunno3x = 7810117;
        nibunnox3 = 560071;
        break;
    case 2472 :
        nibunno3x = 7806841;
        nibunnox3 = 554642;
        break;
    case 2473 :
        nibunno3x = 7803566;
        nibunnox3 = 549219;
        break;
    case 2474 :
        nibunno3x = 7800295;
        nibunnox3 = 543803;
        break;
    case 2475 :
        nibunno3x = 7797025;
        nibunnox3 = 538391;
        break;
    case 2476 :
        nibunno3x = 7793756;
        nibunnox3 = 532984;
        break;
    case 2477 :
        nibunno3x = 7790491;
        nibunnox3 = 527584;
        break;
    case 2478 :
        nibunno3x = 7787227;
        nibunnox3 = 522189;
        break;
    case 2479 :
        nibunno3x = 7783963;
        nibunnox3 = 516796;
        break;
    case 2480 :
        nibunno3x = 7780704;
        nibunnox3 = 511412;
        break;
    case 2481 :
        nibunno3x = 7777446;
        nibunnox3 = 506034;
        break;
    case 2482 :
        nibunno3x = 7774189;
        nibunnox3 = 500660;
        break;
    case 2483 :
        nibunno3x = 7770934;
        nibunnox3 = 495290;
        break;
    case 2484 :
        nibunno3x = 7767682;
        nibunnox3 = 489928;
        break;
    case 2485 :
        nibunno3x = 7764432;
        nibunnox3 = 484570;
        break;
    case 2486 :
        nibunno3x = 7761184;
        nibunnox3 = 479219;
        break;
    case 2487 :
        nibunno3x = 7757938;
        nibunnox3 = 473873;
        break;
    case 2488 :
        nibunno3x = 7754692;
        nibunnox3 = 468529;
        break;
    case 2489 :
        nibunno3x = 7751449;
        nibunnox3 = 463193;
        break;
    case 2490 :
        nibunno3x = 7748209;
        nibunnox3 = 457863;
        break;
    case 2491 :
        nibunno3x = 7744970;
        nibunnox3 = 452538;
        break;
    case 2492 :
        nibunno3x = 7741734;
        nibunnox3 = 447217;
        break;
    case 2493 :
        nibunno3x = 7738498;
        nibunnox3 = 441901;
        break;
    case 2494 :
        nibunno3x = 7735266;
        nibunnox3 = 436592;
        break;
    case 2495 :
        nibunno3x = 7732036;
        nibunnox3 = 431291;
        break;
    case 2496 :
        nibunno3x = 7728806;
        nibunnox3 = 425991;
        break;
    case 2497 :
        nibunno3x = 7725580;
        nibunnox3 = 420698;
        break;
    case 2498 :
        nibunno3x = 7722355;
        nibunnox3 = 415410;
        break;
    case 2499 :
        nibunno3x = 7719132;
        nibunnox3 = 410127;
        break;
    case 2500 :
        nibunno3x = 7715910;
        nibunnox3 = 404848;
        break;
    case 2501 :
        nibunno3x = 7712692;
        nibunnox3 = 399578;
        break;
    case 2502 :
        nibunno3x = 7709473;
        nibunnox3 = 394308;
        break;
    case 2503 :
        nibunno3x = 7706257;
        nibunnox3 = 389046;
        break;
    case 2504 :
        nibunno3x = 7703044;
        nibunnox3 = 383790;
        break;
    case 2505 :
        nibunno3x = 7699832;
        nibunnox3 = 378538;
        break;
    case 2506 :
        nibunno3x = 7696624;
        nibunnox3 = 373294;
        break;
    case 2507 :
        nibunno3x = 7693416;
        nibunnox3 = 368052;
        break;
    case 2508 :
        nibunno3x = 7690210;
        nibunnox3 = 362817;
        break;
    case 2509 :
        nibunno3x = 7687006;
        nibunnox3 = 357587;
        break;
    case 2510 :
        nibunno3x = 7683804;
        nibunnox3 = 352361;
        break;
    case 2511 :
        nibunno3x = 7680604;
        nibunnox3 = 347141;
        break;
    case 2512 :
        nibunno3x = 7677406;
        nibunnox3 = 341927;
        break;
    case 2513 :
        nibunno3x = 7674208;
        nibunnox3 = 336715;
        break;
    case 2514 :
        nibunno3x = 7671014;
        nibunnox3 = 331511;
        break;
    case 2515 :
        nibunno3x = 7667821;
        nibunnox3 = 326310;
        break;
    case 2516 :
        nibunno3x = 7664632;
        nibunnox3 = 321118;
        break;
    case 2517 :
        nibunno3x = 7661443;
        nibunnox3 = 315929;
        break;
    case 2518 :
        nibunno3x = 7658257;
        nibunnox3 = 310746;
        break;
    case 2519 :
        nibunno3x = 7655071;
        nibunnox3 = 305566;
        break;
    case 2520 :
        nibunno3x = 7651888;
        nibunnox3 = 300392;
        break;
    case 2521 :
        nibunno3x = 7648706;
        nibunnox3 = 295223;
        break;
    case 2522 :
        nibunno3x = 7645528;
        nibunnox3 = 290061;
        break;
    case 2523 :
        nibunno3x = 7642351;
        nibunnox3 = 284903;
        break;
    case 2524 :
        nibunno3x = 7639176;
        nibunnox3 = 279750;
        break;
    case 2525 :
        nibunno3x = 7636002;
        nibunnox3 = 274601;
        break;
    case 2526 :
        nibunno3x = 7632830;
        nibunnox3 = 269459;
        break;
    case 2527 :
        nibunno3x = 7629661;
        nibunnox3 = 264321;
        break;
    case 2528 :
        nibunno3x = 7626493;
        nibunnox3 = 259189;
        break;
    case 2529 :
        nibunno3x = 7623325;
        nibunnox3 = 254058;
        break;
    case 2530 :
        nibunno3x = 7620163;
        nibunnox3 = 248939;
        break;
    case 2531 :
        nibunno3x = 7617001;
        nibunnox3 = 243821;
        break;
    case 2532 :
        nibunno3x = 7613840;
        nibunnox3 = 238709;
        break;
    case 2533 :
        nibunno3x = 7610682;
        nibunnox3 = 233600;
        break;
    case 2534 :
        nibunno3x = 7607524;
        nibunnox3 = 228497;
        break;
    case 2535 :
        nibunno3x = 7604370;
        nibunnox3 = 223400;
        break;
    case 2536 :
        nibunno3x = 7601216;
        nibunnox3 = 218307;
        break;
    case 2537 :
        nibunno3x = 7598066;
        nibunnox3 = 213221;
        break;
    case 2538 :
        nibunno3x = 7594918;
        nibunnox3 = 208140;
        break;
    case 2539 :
        nibunno3x = 7591770;
        nibunnox3 = 203061;
        break;
    case 2540 :
        nibunno3x = 7588626;
        nibunnox3 = 197991;
        break;
    case 2541 :
        nibunno3x = 7585480;
        nibunnox3 = 192920;
        break;
    case 2542 :
        nibunno3x = 7582339;
        nibunnox3 = 187859;
        break;
    case 2543 :
        nibunno3x = 7579198;
        nibunnox3 = 182800;
        break;
    case 2544 :
        nibunno3x = 7576060;
        nibunnox3 = 177748;
        break;
    case 2545 :
        nibunno3x = 7572925;
        nibunnox3 = 172702;
        break;
    case 2546 :
        nibunno3x = 7569790;
        nibunnox3 = 167659;
        break;
    case 2547 :
        nibunno3x = 7566658;
        nibunnox3 = 162622;
        break;
    case 2548 :
        nibunno3x = 7563528;
        nibunnox3 = 157589;
        break;
    case 2549 :
        nibunno3x = 7560400;
        nibunnox3 = 152564;
        break;
    case 2550 :
        nibunno3x = 7557271;
        nibunnox3 = 147537;
        break;
    case 2551 :
        nibunno3x = 7554148;
        nibunnox3 = 142523;
        break;
    case 2552 :
        nibunno3x = 7551024;
        nibunnox3 = 137508;
        break;
    case 2553 :
        nibunno3x = 7547902;
        nibunnox3 = 132500;
        break;
    case 2554 :
        nibunno3x = 7544782;
        nibunnox3 = 127496;
        break;
    case 2555 :
        nibunno3x = 7541665;
        nibunnox3 = 122500;
        break;
    case 2556 :
        nibunno3x = 7538548;
        nibunnox3 = 117505;
        break;
    case 2557 :
        nibunno3x = 7535434;
        nibunnox3 = 112516;
        break;
    case 2558 :
        nibunno3x = 7532323;
        nibunnox3 = 107535;
        break;
    case 2559 :
        nibunno3x = 7529212;
        nibunnox3 = 102555;
        break;
    case 2560 :
        nibunno3x = 7526104;
        nibunnox3 = 97582;
        break;
    case 2561 :
        nibunno3x = 7522996;
        nibunnox3 = 92612;
        break;
    case 2562 :
        nibunno3x = 7519891;
        nibunnox3 = 87648;
        break;
    case 2563 :
        nibunno3x = 7516789;
        nibunnox3 = 82690;
        break;
    case 2564 :
        nibunno3x = 7513687;
        nibunnox3 = 77735;
        break;
    case 2565 :
        nibunno3x = 7510588;
        nibunnox3 = 72786;
        break;
    case 2566 :
        nibunno3x = 7507489;
        nibunnox3 = 67839;
        break;
    case 2567 :
        nibunno3x = 7504393;
        nibunnox3 = 62899;
        break;
    case 2568 :
        nibunno3x = 7501298;
        nibunnox3 = 57963;
        break;
    case 2569 :
        nibunno3x = 7498207;
        nibunnox3 = 53034;
        break;
    case 2570 :
        nibunno3x = 7495117;
        nibunnox3 = 48109;
        break;
    case 2571 :
        nibunno3x = 7492028;
        nibunnox3 = 43189;
        break;
    case 2572 :
        nibunno3x = 7488940;
        nibunnox3 = 38271;
        break;
    case 2573 :
        nibunno3x = 7485856;
        nibunnox3 = 33361;
        break;
    case 2574 :
        nibunno3x = 7482772;
        nibunnox3 = 28453;
        break;
    case 2575 :
        nibunno3x = 7479691;
        nibunnox3 = 23552;
        break;
    case 2576 :
        nibunno3x = 7476612;
        nibunnox3 = 18656;
        break;
    case 2577 :
        nibunno3x = 7473534;
        nibunnox3 = 13764;
        break;
    case 2578 :
        nibunno3x = 7470457;
        nibunnox3 = 8875;
        break;
    case 2579 :
        nibunno3x = 7467382;
        nibunnox3 = 3992;
        break;
    case 2580 :
        nibunno3x = 7464310;
        nibunnox3 = 8386837;
        break;
    case 2581 :
        nibunno3x = 7461240;
        nibunnox3 = 8377092;
        break;
    case 2582 :
        nibunno3x = 7458170;
        nibunnox3 = 8367355;
        break;
    case 2583 :
        nibunno3x = 7455103;
        nibunnox3 = 8357627;
        break;
    case 2584 :
        nibunno3x = 7452037;
        nibunnox3 = 8347906;
        break;
    case 2585 :
        nibunno3x = 7448974;
        nibunnox3 = 8338199;
        break;
    case 2586 :
        nibunno3x = 7445912;
        nibunnox3 = 8328500;
        break;
    case 2587 :
        nibunno3x = 7442851;
        nibunnox3 = 8318806;
        break;
    case 2588 :
        nibunno3x = 7439794;
        nibunnox3 = 8309129;
        break;
    case 2589 :
        nibunno3x = 7436737;
        nibunnox3 = 8299456;
        break;
    case 2590 :
        nibunno3x = 7433683;
        nibunnox3 = 8289798;
        break;
    case 2591 :
        nibunno3x = 7430629;
        nibunnox3 = 8280142;
        break;
    case 2592 :
        nibunno3x = 7427578;
        nibunnox3 = 8270499;
        break;
    case 2593 :
        nibunno3x = 7424528;
        nibunnox3 = 8260865;
        break;
    case 2594 :
        nibunno3x = 7421480;
        nibunnox3 = 8251239;
        break;
    case 2595 :
        nibunno3x = 7418434;
        nibunnox3 = 8241622;
        break;
    case 2596 :
        nibunno3x = 7415390;
        nibunnox3 = 8232017;
        break;
    case 2597 :
        nibunno3x = 7412347;
        nibunnox3 = 8222417;
        break;
    case 2598 :
        nibunno3x = 7409308;
        nibunnox3 = 8212834;
        break;
    case 2599 :
        nibunno3x = 7406269;
        nibunnox3 = 8203255;
        break;
    case 2600 :
        nibunno3x = 7403232;
        nibunnox3 = 8193685;
        break;
    case 2601 :
        nibunno3x = 7400197;
        nibunnox3 = 8184128;
        break;
    case 2602 :
        nibunno3x = 7397162;
        nibunnox3 = 8174574;
        break;
    case 2603 :
        nibunno3x = 7394131;
        nibunnox3 = 8165033;
        break;
    case 2604 :
        nibunno3x = 7391101;
        nibunnox3 = 8155501;
        break;
    case 2605 :
        nibunno3x = 7388072;
        nibunnox3 = 8145978;
        break;
    case 2606 :
        nibunno3x = 7385044;
        nibunnox3 = 8136457;
        break;
    case 2607 :
        nibunno3x = 7382020;
        nibunnox3 = 8126955;
        break;
    case 2608 :
        nibunno3x = 7378996;
        nibunnox3 = 8117456;
        break;
    case 2609 :
        nibunno3x = 7375975;
        nibunnox3 = 8107970;
        break;
    case 2610 :
        nibunno3x = 7372956;
        nibunnox3 = 8098494;
        break;
    case 2611 :
        nibunno3x = 7369938;
        nibunnox3 = 8089025;
        break;
    case 2612 :
        nibunno3x = 7366921;
        nibunnox3 = 8079563;
        break;
    case 2613 :
        nibunno3x = 7363906;
        nibunnox3 = 8070111;
        break;
    case 2614 :
        nibunno3x = 7360894;
        nibunnox3 = 8060672;
        break;
    case 2615 :
        nibunno3x = 7357882;
        nibunnox3 = 8051237;
        break;
    case 2616 :
        nibunno3x = 7354873;
        nibunnox3 = 8041814;
        break;
    case 2617 :
        nibunno3x = 7351866;
        nibunnox3 = 8032400;
        break;
    case 2618 :
        nibunno3x = 7348860;
        nibunnox3 = 8022994;
        break;
    case 2619 :
        nibunno3x = 7345855;
        nibunnox3 = 8013596;
        break;
    case 2620 :
        nibunno3x = 7342852;
        nibunnox3 = 8004206;
        break;
    case 2621 :
        nibunno3x = 7339852;
        nibunnox3 = 7994830;
        break;
    case 2622 :
        nibunno3x = 7336854;
        nibunnox3 = 7985461;
        break;
    case 2623 :
        nibunno3x = 7333855;
        nibunnox3 = 7976096;
        break;
    case 2624 :
        nibunno3x = 7330861;
        nibunnox3 = 7966749;
        break;
    case 2625 :
        nibunno3x = 7327867;
        nibunnox3 = 7957406;
        break;
    case 2626 :
        nibunno3x = 7324874;
        nibunnox3 = 7948071;
        break;
    case 2627 :
        nibunno3x = 7321885;
        nibunnox3 = 7938747;
        break;
    case 2628 :
        nibunno3x = 7318896;
        nibunnox3 = 7929429;
        break;
    case 2629 :
        nibunno3x = 7315909;
        nibunnox3 = 7920123;
        break;
    case 2630 :
        nibunno3x = 7312921;
        nibunnox3 = 7910816;
        break;
    case 2631 :
        nibunno3x = 7309939;
        nibunnox3 = 7901531;
        break;
    case 2632 :
        nibunno3x = 7306957;
        nibunnox3 = 7892250;
        break;
    case 2633 :
        nibunno3x = 7303978;
        nibunnox3 = 7882981;
        break;
    case 2634 :
        nibunno3x = 7300999;
        nibunnox3 = 7873717;
        break;
    case 2635 :
        nibunno3x = 7298020;
        nibunnox3 = 7864455;
        break;
    case 2636 :
        nibunno3x = 7295047;
        nibunnox3 = 7855215;
        break;
    case 2637 :
        nibunno3x = 7292074;
        nibunnox3 = 7845980;
        break;
    case 2638 :
        nibunno3x = 7289101;
        nibunnox3 = 7836747;
        break;
    case 2639 :
        nibunno3x = 7286131;
        nibunnox3 = 7827528;
        break;
    case 2640 :
        nibunno3x = 7283162;
        nibunnox3 = 7818317;
        break;
    case 2641 :
        nibunno3x = 7280197;
        nibunnox3 = 7809118;
        break;
    case 2642 :
        nibunno3x = 7277230;
        nibunnox3 = 7799918;
        break;
    case 2643 :
        nibunno3x = 7274268;
        nibunnox3 = 7790736;
        break;
    case 2644 :
        nibunno3x = 7271305;
        nibunnox3 = 7781557;
        break;
    case 2645 :
        nibunno3x = 7268347;
        nibunnox3 = 7772395;
        break;
    case 2646 :
        nibunno3x = 7265386;
        nibunnox3 = 7763228;
        break;
    case 2647 :
        nibunno3x = 7262431;
        nibunnox3 = 7754083;
        break;
    case 2648 :
        nibunno3x = 7259476;
        nibunnox3 = 7744942;
        break;
    case 2649 :
        nibunno3x = 7256522;
        nibunnox3 = 7735808;
        break;
    case 2650 :
        nibunno3x = 7253572;
        nibunnox3 = 7726687;
        break;
    case 2651 :
        nibunno3x = 7250622;
        nibunnox3 = 7717569;
        break;
    case 2652 :
        nibunno3x = 7247672;
        nibunnox3 = 7708459;
        break;
    case 2653 :
        nibunno3x = 7244725;
        nibunnox3 = 7699358;
        break;
    case 2654 :
        nibunno3x = 7241782;
        nibunnox3 = 7690274;
        break;
    case 2655 :
        nibunno3x = 7238839;
        nibunnox3 = 7681195;
        break;
    case 2656 :
        nibunno3x = 7235896;
        nibunnox3 = 7672117;
        break;
    case 2657 :
        nibunno3x = 7232956;
        nibunnox3 = 7663052;
        break;
    case 2658 :
        nibunno3x = 7230018;
        nibunnox3 = 7653996;
        break;
    case 2659 :
        nibunno3x = 7227082;
        nibunnox3 = 7644952;
        break;
    case 2660 :
        nibunno3x = 7224146;
        nibunnox3 = 7635911;
        break;
    case 2661 :
        nibunno3x = 7221214;
        nibunnox3 = 7626884;
        break;
    case 2662 :
        nibunno3x = 7218280;
        nibunnox3 = 7617854;
        break;
    case 2663 :
        nibunno3x = 7215350;
        nibunnox3 = 7608843;
        break;
    case 2664 :
        nibunno3x = 7212422;
        nibunnox3 = 7599839;
        break;
    case 2665 :
        nibunno3x = 7209496;
        nibunnox3 = 7590843;
        break;
    case 2666 :
        nibunno3x = 7206571;
        nibunnox3 = 7581856;
        break;
    case 2667 :
        nibunno3x = 7203648;
        nibunnox3 = 7572875;
        break;
    case 2668 :
        nibunno3x = 7200726;
        nibunnox3 = 7563904;
        break;
    case 2669 :
        nibunno3x = 7197805;
        nibunnox3 = 7554940;
        break;
    case 2670 :
        nibunno3x = 7194886;
        nibunnox3 = 7545983;
        break;
    case 2671 :
        nibunno3x = 7191970;
        nibunnox3 = 7537040;
        break;
    case 2672 :
        nibunno3x = 7189054;
        nibunnox3 = 7528100;
        break;
    case 2673 :
        nibunno3x = 7186141;
        nibunnox3 = 7519173;
        break;
    case 2674 :
        nibunno3x = 7183230;
        nibunnox3 = 7510253;
        break;
    case 2675 :
        nibunno3x = 7180318;
        nibunnox3 = 7501336;
        break;
    case 2676 :
        nibunno3x = 7177410;
        nibunnox3 = 7492433;
        break;
    case 2677 :
        nibunno3x = 7174502;
        nibunnox3 = 7483537;
        break;
    case 2678 :
        nibunno3x = 7171597;
        nibunnox3 = 7474649;
        break;
    case 2679 :
        nibunno3x = 7168693;
        nibunnox3 = 7465769;
        break;
    case 2680 :
        nibunno3x = 7165792;
        nibunnox3 = 7456902;
        break;
    case 2681 :
        nibunno3x = 7162891;
        nibunnox3 = 7448038;
        break;
    case 2682 :
        nibunno3x = 7159993;
        nibunnox3 = 7439185;
        break;
    case 2683 :
        nibunno3x = 7157095;
        nibunnox3 = 7430337;
        break;
    case 2684 :
        nibunno3x = 7154200;
        nibunnox3 = 7421502;
        break;
    case 2685 :
        nibunno3x = 7151305;
        nibunnox3 = 7412668;
        break;
    case 2686 :
        nibunno3x = 7148413;
        nibunnox3 = 7403848;
        break;
    case 2687 :
        nibunno3x = 7145521;
        nibunnox3 = 7395032;
        break;
    case 2688 :
        nibunno3x = 7142632;
        nibunnox3 = 7386227;
        break;
    case 2689 :
        nibunno3x = 7139744;
        nibunnox3 = 7377430;
        break;
    case 2690 :
        nibunno3x = 7136857;
        nibunnox3 = 7368636;
        break;
    case 2691 :
        nibunno3x = 7133974;
        nibunnox3 = 7359860;
        break;
    case 2692 :
        nibunno3x = 7131091;
        nibunnox3 = 7351087;
        break;
    case 2693 :
        nibunno3x = 7128210;
        nibunnox3 = 7342322;
        break;
    case 2694 :
        nibunno3x = 7125331;
        nibunnox3 = 7333568;
        break;
    case 2695 :
        nibunno3x = 7122451;
        nibunnox3 = 7324814;
        break;
    case 2696 :
        nibunno3x = 7119577;
        nibunnox3 = 7316081;
        break;
    case 2697 :
        nibunno3x = 7116702;
        nibunnox3 = 7307347;
        break;
    case 2698 :
        nibunno3x = 7113828;
        nibunnox3 = 7298621;
        break;
    case 2699 :
        nibunno3x = 7110955;
        nibunnox3 = 7289901;
        break;
    case 2700 :
        nibunno3x = 7108087;
        nibunnox3 = 7281200;
        break;
    case 2701 :
        nibunno3x = 7105219;
        nibunnox3 = 7272501;
        break;
    case 2702 :
        nibunno3x = 7102351;
        nibunnox3 = 7263806;
        break;
    case 2703 :
        nibunno3x = 7099486;
        nibunnox3 = 7255124;
        break;
    case 2704 :
        nibunno3x = 7096621;
        nibunnox3 = 7246443;
        break;
    case 2705 :
        nibunno3x = 7093759;
        nibunnox3 = 7237776;
        break;
    case 2706 :
        nibunno3x = 7090900;
        nibunnox3 = 7229120;
        break;
    case 2707 :
        nibunno3x = 7088041;
        nibunnox3 = 7220469;
        break;
    case 2708 :
        nibunno3x = 7085182;
        nibunnox3 = 7211820;
        break;
    case 2709 :
        nibunno3x = 7082328;
        nibunnox3 = 7203188;
        break;
    case 2710 :
        nibunno3x = 7079473;
        nibunnox3 = 7194559;
        break;
    case 2711 :
        nibunno3x = 7076620;
        nibunnox3 = 7185938;
        break;
    case 2712 :
        nibunno3x = 7073770;
        nibunnox3 = 7177329;
        break;
    case 2713 :
        nibunno3x = 7070920;
        nibunnox3 = 7168724;
        break;
    case 2714 :
        nibunno3x = 7068073;
        nibunnox3 = 7160131;
        break;
    case 2715 :
        nibunno3x = 7065226;
        nibunnox3 = 7151540;
        break;
    case 2716 :
        nibunno3x = 7062382;
        nibunnox3 = 7142962;
        break;
    case 2717 :
        nibunno3x = 7059538;
        nibunnox3 = 7134386;
        break;
    case 2718 :
        nibunno3x = 7056697;
        nibunnox3 = 7125824;
        break;
    case 2719 :
        nibunno3x = 7053856;
        nibunnox3 = 7117265;
        break;
    case 2720 :
        nibunno3x = 7051018;
        nibunnox3 = 7108718;
        break;
    case 2721 :
        nibunno3x = 7048182;
        nibunnox3 = 7100177;
        break;
    case 2722 :
        nibunno3x = 7045345;
        nibunnox3 = 7091641;
        break;
    case 2723 :
        nibunno3x = 7042513;
        nibunnox3 = 7083121;
        break;
    case 2724 :
        nibunno3x = 7039681;
        nibunnox3 = 7074605;
        break;
    case 2725 :
        nibunno3x = 7036849;
        nibunnox3 = 7066091;
        break;
    case 2726 :
        nibunno3x = 7034020;
        nibunnox3 = 7057589;
        break;
    case 2727 :
        nibunno3x = 7031194;
        nibunnox3 = 7049099;
        break;
    case 2728 :
        nibunno3x = 7028368;
        nibunnox3 = 7040613;
        break;
    case 2729 :
        nibunno3x = 7025542;
        nibunnox3 = 7032130;
        break;
    case 2730 :
        nibunno3x = 7022719;
        nibunnox3 = 7023660;
        break;
    case 2731 :
        nibunno3x = 7019899;
        nibunnox3 = 7015200;
        break;
    case 2732 :
        nibunno3x = 7017079;
        nibunnox3 = 7006744;
        break;
    case 2733 :
        nibunno3x = 7014260;
        nibunnox3 = 6998296;
        break;
    case 2734 :
        nibunno3x = 7011445;
        nibunnox3 = 6989859;
        break;
    case 2735 :
        nibunno3x = 7008630;
        nibunnox3 = 6981426;
        break;
    case 2736 :
        nibunno3x = 7005817;
        nibunnox3 = 6973006;
        break;
    case 2737 :
        nibunno3x = 7003003;
        nibunnox3 = 6964583;
        break;
    case 2738 :
        nibunno3x = 7000192;
        nibunnox3 = 6956173;
        break;
    case 2739 :
        nibunno3x = 6997384;
        nibunnox3 = 6947774;
        break;
    case 2740 :
        nibunno3x = 6994576;
        nibunnox3 = 6939379;
        break;
    case 2741 :
        nibunno3x = 6991771;
        nibunnox3 = 6930996;
        break;
    case 2742 :
        nibunno3x = 6988966;
        nibunnox3 = 6922616;
        break;
    case 2743 :
        nibunno3x = 6986164;
        nibunnox3 = 6914248;
        break;
    case 2744 :
        nibunno3x = 6983362;
        nibunnox3 = 6905882;
        break;
    case 2745 :
        nibunno3x = 6980563;
        nibunnox3 = 6897529;
        break;
    case 2746 :
        nibunno3x = 6977764;
        nibunnox3 = 6889179;
        break;
    case 2747 :
        nibunno3x = 6974968;
        nibunnox3 = 6880841;
        break;
    case 2748 :
        nibunno3x = 6972172;
        nibunnox3 = 6872506;
        break;
    case 2749 :
        nibunno3x = 6969379;
        nibunnox3 = 6864183;
        break;
    case 2750 :
        nibunno3x = 6966586;
        nibunnox3 = 6855863;
        break;
    case 2751 :
        nibunno3x = 6963796;
        nibunnox3 = 6847555;
        break;
    case 2752 :
        nibunno3x = 6961006;
        nibunnox3 = 6839249;
        break;
    case 2753 :
        nibunno3x = 6958219;
        nibunnox3 = 6830956;
        break;
    case 2754 :
        nibunno3x = 6955434;
        nibunnox3 = 6822670;
        break;
    case 2755 :
        nibunno3x = 6952648;
        nibunnox3 = 6814388;
        break;
    case 2756 :
        nibunno3x = 6949866;
        nibunnox3 = 6806117;
        break;
    case 2757 :
        nibunno3x = 6947084;
        nibunnox3 = 6797853;
        break;
    case 2758 :
        nibunno3x = 6944305;
        nibunnox3 = 6789598;
        break;
    case 2759 :
        nibunno3x = 6941527;
        nibunnox3 = 6781350;
        break;
    case 2760 :
        nibunno3x = 6938749;
        nibunnox3 = 6773104;
        break;
    case 2761 :
        nibunno3x = 6935974;
        nibunnox3 = 6764870;
        break;
    case 2762 :
        nibunno3x = 6933199;
        nibunnox3 = 6756640;
        break;
    case 2763 :
        nibunno3x = 6930427;
        nibunnox3 = 6748421;
        break;
    case 2764 :
        nibunno3x = 6927656;
        nibunnox3 = 6740210;
        break;
    case 2765 :
        nibunno3x = 6924886;
        nibunnox3 = 6732001;
        break;
    case 2766 :
        nibunno3x = 6922120;
        nibunnox3 = 6723809;
        break;
    case 2767 :
        nibunno3x = 6919354;
        nibunnox3 = 6715620;
        break;
    case 2768 :
        nibunno3x = 6916588;
        nibunnox3 = 6707435;
        break;
    case 2769 :
        nibunno3x = 6913825;
        nibunnox3 = 6699260;
        break;
    case 2770 :
        nibunno3x = 6911062;
        nibunnox3 = 6691088;
        break;
    case 2771 :
        nibunno3x = 6908302;
        nibunnox3 = 6682929;
        break;
    case 2772 :
        nibunno3x = 6905542;
        nibunnox3 = 6674773;
        break;
    case 2773 :
        nibunno3x = 6902785;
        nibunnox3 = 6666628;
        break;
    case 2774 :
        nibunno3x = 6900030;
        nibunnox3 = 6658491;
        break;
    case 2775 :
        nibunno3x = 6897274;
        nibunnox3 = 6650356;
        break;
    case 2776 :
        nibunno3x = 6894523;
        nibunnox3 = 6642238;
        break;
    case 2777 :
        nibunno3x = 6891769;
        nibunnox3 = 6634114;
        break;
    case 2778 :
        nibunno3x = 6889021;
        nibunnox3 = 6626010;
        break;
    case 2779 :
        nibunno3x = 6886272;
        nibunnox3 = 6617905;
        break;
    case 2780 :
        nibunno3x = 6883525;
        nibunnox3 = 6609812;
        break;
    case 2781 :
        nibunno3x = 6880780;
        nibunnox3 = 6601727;
        break;
    case 2782 :
        nibunno3x = 6878035;
        nibunnox3 = 6593644;
        break;
    case 2783 :
        nibunno3x = 6875293;
        nibunnox3 = 6585572;
        break;
    case 2784 :
        nibunno3x = 6872551;
        nibunnox3 = 6577504;
        break;
    case 2785 :
        nibunno3x = 6869812;
        nibunnox3 = 6569447;
        break;
    case 2786 :
        nibunno3x = 6867073;
        nibunnox3 = 6561393;
        break;
    case 2787 :
        nibunno3x = 6864337;
        nibunnox3 = 6553351;
        break;
    case 2788 :
        nibunno3x = 6861601;
        nibunnox3 = 6545311;
        break;
    case 2789 :
        nibunno3x = 6858868;
        nibunnox3 = 6537284;
        break;
    case 2790 :
        nibunno3x = 6856135;
        nibunnox3 = 6529259;
        break;
    case 2791 :
        nibunno3x = 6853405;
        nibunnox3 = 6521246;
        break;
    case 2792 :
        nibunno3x = 6850675;
        nibunnox3 = 6513237;
        break;
    case 2793 :
        nibunno3x = 6847948;
        nibunnox3 = 6505238;
        break;
    case 2794 :
        nibunno3x = 6845221;
        nibunnox3 = 6497242;
        break;
    case 2795 :
        nibunno3x = 6842497;
        nibunnox3 = 6489258;
        break;
    case 2796 :
        nibunno3x = 6839773;
        nibunnox3 = 6481277;
        break;
    case 2797 :
        nibunno3x = 6837052;
        nibunnox3 = 6473308;
        break;
    case 2798 :
        nibunno3x = 6834331;
        nibunnox3 = 6465341;
        break;
    case 2799 :
        nibunno3x = 6831612;
        nibunnox3 = 6457382;
        break;
    case 2800 :
        nibunno3x = 6828894;
        nibunnox3 = 6449430;
        break;
    case 2801 :
        nibunno3x = 6826178;
        nibunnox3 = 6441489;
        break;
    case 2802 :
        nibunno3x = 6823462;
        nibunnox3 = 6433548;
        break;
    case 2803 :
        nibunno3x = 6820748;
        nibunnox3 = 6425617;
        break;
    case 2804 :
        nibunno3x = 6818038;
        nibunnox3 = 6417698;
        break;
    case 2805 :
        nibunno3x = 6815328;
        nibunnox3 = 6409782;
        break;
    case 2806 :
        nibunno3x = 6812618;
        nibunnox3 = 6401873;
        break;
    case 2807 :
        nibunno3x = 6809911;
        nibunnox3 = 6393971;
        break;
    case 2808 :
        nibunno3x = 6807205;
        nibunnox3 = 6386078;
        break;
    case 2809 :
        nibunno3x = 6804500;
        nibunnox3 = 6378190;
        break;
    case 2810 :
        nibunno3x = 6801796;
        nibunnox3 = 6370306;
        break;
    case 2811 :
        nibunno3x = 6799096;
        nibunnox3 = 6362437;
        break;
    case 2812 :
        nibunno3x = 6796396;
        nibunnox3 = 6354571;
        break;
    case 2813 :
        nibunno3x = 6793696;
        nibunnox3 = 6346708;
        break;
    case 2814 :
        nibunno3x = 6790999;
        nibunnox3 = 6338858;
        break;
    case 2815 :
        nibunno3x = 6788302;
        nibunnox3 = 6331009;
        break;
    case 2816 :
        nibunno3x = 6785608;
        nibunnox3 = 6323172;
        break;
    case 2817 :
        nibunno3x = 6782916;
        nibunnox3 = 6315342;
        break;
    case 2818 :
        nibunno3x = 6780223;
        nibunnox3 = 6307515;
        break;
    case 2819 :
        nibunno3x = 6777534;
        nibunnox3 = 6299698;
        break;
    case 2820 :
        nibunno3x = 6774844;
        nibunnox3 = 6291885;
        break;
    case 2821 :
        nibunno3x = 6772156;
        nibunnox3 = 6284080;
        break;
    case 2822 :
        nibunno3x = 6769471;
        nibunnox3 = 6276286;
        break;
    case 2823 :
        nibunno3x = 6766786;
        nibunnox3 = 6268494;
        break;
    case 2824 :
        nibunno3x = 6764104;
        nibunnox3 = 6260714;
        break;
    case 2825 :
        nibunno3x = 6761422;
        nibunnox3 = 6252937;
        break;
    case 2826 :
        nibunno3x = 6758742;
        nibunnox3 = 6245167;
        break;
    case 2827 :
        nibunno3x = 6756064;
        nibunnox3 = 6237408;
        break;
    case 2828 :
        nibunno3x = 6753385;
        nibunnox3 = 6229647;
        break;
    case 2829 :
        nibunno3x = 6750709;
        nibunnox3 = 6221899;
        break;
    case 2830 :
        nibunno3x = 6748036;
        nibunnox3 = 6214161;
        break;
    case 2831 :
        nibunno3x = 6745362;
        nibunnox3 = 6206422;
        break;
    case 2832 :
        nibunno3x = 6742690;
        nibunnox3 = 6198694;
        break;
    case 2833 :
        nibunno3x = 6740020;
        nibunnox3 = 6190973;
        break;
    case 2834 :
        nibunno3x = 6737350;
        nibunnox3 = 6183255;
        break;
    case 2835 :
        nibunno3x = 6734683;
        nibunnox3 = 6175549;
        break;
    case 2836 :
        nibunno3x = 6732018;
        nibunnox3 = 6167849;
        break;
    case 2837 :
        nibunno3x = 6729352;
        nibunnox3 = 6160153;
        break;
    case 2838 :
        nibunno3x = 6726691;
        nibunnox3 = 6152471;
        break;
    case 2839 :
        nibunno3x = 6724027;
        nibunnox3 = 6144785;
        break;
    case 2840 :
        nibunno3x = 6721366;
        nibunnox3 = 6137109;
        break;
    case 2841 :
        nibunno3x = 6718708;
        nibunnox3 = 6129444;
        break;
    case 2842 :
        nibunno3x = 6716050;
        nibunnox3 = 6121783;
        break;
    case 2843 :
        nibunno3x = 6713395;
        nibunnox3 = 6114133;
        break;
    case 2844 :
        nibunno3x = 6710740;
        nibunnox3 = 6106485;
        break;
    case 2845 :
        nibunno3x = 6708085;
        nibunnox3 = 6098840;
        break;
    case 2846 :
        nibunno3x = 6705433;
        nibunnox3 = 6091207;
        break;
    case 2847 :
        nibunno3x = 6702784;
        nibunnox3 = 6083584;
        break;
    case 2848 :
        nibunno3x = 6700135;
        nibunnox3 = 6075964;
        break;
    case 2849 :
        nibunno3x = 6697486;
        nibunnox3 = 6068347;
        break;
    case 2850 :
        nibunno3x = 6694840;
        nibunnox3 = 6060742;
        break;
    case 2851 :
        nibunno3x = 6692194;
        nibunnox3 = 6053139;
        break;
    case 2852 :
        nibunno3x = 6689551;
        nibunnox3 = 6045547;
        break;
    case 2853 :
        nibunno3x = 6686908;
        nibunnox3 = 6037959;
        break;
    case 2854 :
        nibunno3x = 6684268;
        nibunnox3 = 6030381;
        break;
    case 2855 :
        nibunno3x = 6681628;
        nibunnox3 = 6022805;
        break;
    case 2856 :
        nibunno3x = 6678991;
        nibunnox3 = 6015242;
        break;
    case 2857 :
        nibunno3x = 6676354;
        nibunnox3 = 6007681;
        break;
    case 2858 :
        nibunno3x = 6673718;
        nibunnox3 = 6000126;
        break;
    case 2859 :
        nibunno3x = 6671084;
        nibunnox3 = 5992579;
        break;
    case 2860 :
        nibunno3x = 6668452;
        nibunnox3 = 5985039;
        break;
    case 2861 :
        nibunno3x = 6665821;
        nibunnox3 = 5977505;
        break;
    case 2862 :
        nibunno3x = 6663190;
        nibunnox3 = 5969975;
        break;
    case 2863 :
        nibunno3x = 6660562;
        nibunnox3 = 5962455;
        break;
    case 2864 :
        nibunno3x = 6657936;
        nibunnox3 = 5954943;
        break;
    case 2865 :
        nibunno3x = 6655309;
        nibunnox3 = 5947432;
        break;
    case 2866 :
        nibunno3x = 6652684;
        nibunnox3 = 5939929;
        break;
    case 2867 :
        nibunno3x = 6650062;
        nibunnox3 = 5932436;
        break;
    case 2868 :
        nibunno3x = 6647440;
        nibunnox3 = 5924948;
        break;
    case 2869 :
        nibunno3x = 6644821;
        nibunnox3 = 5917469;
        break;
    case 2870 :
        nibunno3x = 6642202;
        nibunnox3 = 5909993;
        break;
    case 2871 :
        nibunno3x = 6639584;
        nibunnox3 = 5902526;
        break;
    case 2872 :
        nibunno3x = 6636967;
        nibunnox3 = 5895059;
        break;
    case 2873 :
        nibunno3x = 6634354;
        nibunnox3 = 5887609;
        break;
    case 2874 :
        nibunno3x = 6631741;
        nibunnox3 = 5880161;
        break;
    case 2875 :
        nibunno3x = 6629128;
        nibunnox3 = 5872715;
        break;
    case 2876 :
        nibunno3x = 6626518;
        nibunnox3 = 5865281;
        break;
    case 2877 :
        nibunno3x = 6623908;
        nibunnox3 = 5857849;
        break;
    case 2878 :
        nibunno3x = 6621301;
        nibunnox3 = 5850428;
        break;
    case 2879 :
        nibunno3x = 6618694;
        nibunnox3 = 5843010;
        break;
    case 2880 :
        nibunno3x = 6616088;
        nibunnox3 = 5835599;
        break;
    case 2881 :
        nibunno3x = 6613484;
        nibunnox3 = 5828195;
        break;
    case 2882 :
        nibunno3x = 6610882;
        nibunnox3 = 5820797;
        break;
    case 2883 :
        nibunno3x = 6608281;
        nibunnox3 = 5813407;
        break;
    case 2884 :
        nibunno3x = 6605682;
        nibunnox3 = 5806023;
        break;
    case 2885 :
        nibunno3x = 6603082;
        nibunnox3 = 5798641;
        break;
    case 2886 :
        nibunno3x = 6600486;
        nibunnox3 = 5791271;
        break;
    case 2887 :
        nibunno3x = 6597889;
        nibunnox3 = 5783903;
        break;
    case 2888 :
        nibunno3x = 6595294;
        nibunnox3 = 5776543;
        break;
    case 2889 :
        nibunno3x = 6592702;
        nibunnox3 = 5769193;
        break;
    case 2890 :
        nibunno3x = 6590110;
        nibunnox3 = 5761845;
        break;
    case 2891 :
        nibunno3x = 6587520;
        nibunnox3 = 5754505;
        break;
    case 2892 :
        nibunno3x = 6584932;
        nibunnox3 = 5747175;
        break;
    case 2893 :
        nibunno3x = 6582343;
        nibunnox3 = 5739845;
        break;
    case 2894 :
        nibunno3x = 6579757;
        nibunnox3 = 5732524;
        break;
    case 2895 :
        nibunno3x = 6577171;
        nibunnox3 = 5725206;
        break;
    case 2896 :
        nibunno3x = 6574588;
        nibunnox3 = 5717899;
        break;
    case 2897 :
        nibunno3x = 6572006;
        nibunnox3 = 5710601;
        break;
    case 2898 :
        nibunno3x = 6569425;
        nibunnox3 = 5703303;
        break;
    case 2899 :
        nibunno3x = 6566845;
        nibunnox3 = 5696013;
        break;
    case 2900 :
        nibunno3x = 6564268;
        nibunnox3 = 5688733;
        break;
    case 2901 :
        nibunno3x = 6561691;
        nibunnox3 = 5681455;
        break;
    case 2902 :
        nibunno3x = 6559114;
        nibunnox3 = 5674181;
        break;
    case 2903 :
        nibunno3x = 6556540;
        nibunnox3 = 5666917;
        break;
    case 2904 :
        nibunno3x = 6553968;
        nibunnox3 = 5659661;
        break;
    case 2905 :
        nibunno3x = 6551395;
        nibunnox3 = 5652406;
        break;
    case 2906 :
        nibunno3x = 6548826;
        nibunnox3 = 5645163;
        break;
    case 2907 :
        nibunno3x = 6546256;
        nibunnox3 = 5637922;
        break;
    case 2908 :
        nibunno3x = 6543688;
        nibunnox3 = 5630687;
        break;
    case 2909 :
        nibunno3x = 6541123;
        nibunnox3 = 5623464;
        break;
    case 2910 :
        nibunno3x = 6538558;
        nibunnox3 = 5616243;
        break;
    case 2911 :
        nibunno3x = 6535993;
        nibunnox3 = 5609026;
        break;
    case 2912 :
        nibunno3x = 6533431;
        nibunnox3 = 5601818;
        break;
    case 2913 :
        nibunno3x = 6530870;
        nibunnox3 = 5594617;
        break;
    case 2914 :
        nibunno3x = 6528310;
        nibunnox3 = 5587418;
        break;
    case 2915 :
        nibunno3x = 6525752;
        nibunnox3 = 5580231;
        break;
    case 2916 :
        nibunno3x = 6523195;
        nibunnox3 = 5573047;
        break;
    case 2917 :
        nibunno3x = 6520639;
        nibunnox3 = 5565868;
        break;
    case 2918 :
        nibunno3x = 6518084;
        nibunnox3 = 5558697;
        break;
    case 2919 :
        nibunno3x = 6515532;
        nibunnox3 = 5551533;
        break;
    case 2920 :
        nibunno3x = 6512980;
        nibunnox3 = 5544374;
        break;
    case 2921 :
        nibunno3x = 6510430;
        nibunnox3 = 5537222;
        break;
    case 2922 :
        nibunno3x = 6507880;
        nibunnox3 = 5530074;
        break;
    case 2923 :
        nibunno3x = 6505333;
        nibunnox3 = 5522936;
        break;
    case 2924 :
        nibunno3x = 6502786;
        nibunnox3 = 5515799;
        break;
    case 2925 :
        nibunno3x = 6500239;
        nibunnox3 = 5508667;
        break;
    case 2926 :
        nibunno3x = 6497695;
        nibunnox3 = 5501543;
        break;
    case 2927 :
        nibunno3x = 6495152;
        nibunnox3 = 5494428;
        break;
    case 2928 :
        nibunno3x = 6492612;
        nibunnox3 = 5487319;
        break;
    case 2929 :
        nibunno3x = 6490072;
        nibunnox3 = 5480216;
        break;
    case 2930 :
        nibunno3x = 6487532;
        nibunnox3 = 5473115;
        break;
    case 2931 :
        nibunno3x = 6484996;
        nibunnox3 = 5466026;
        break;
    case 2932 :
        nibunno3x = 6482460;
        nibunnox3 = 5458939;
        break;
    case 2933 :
        nibunno3x = 6479924;
        nibunnox3 = 5451859;
        break;
    case 2934 :
        nibunno3x = 6477391;
        nibunnox3 = 5444785;
        break;
    case 2935 :
        nibunno3x = 6474859;
        nibunnox3 = 5437718;
        break;
    case 2936 :
        nibunno3x = 6472327;
        nibunnox3 = 5430654;
        break;
    case 2937 :
        nibunno3x = 6469798;
        nibunnox3 = 5423600;
        break;
    case 2938 :
        nibunno3x = 6467269;
        nibunnox3 = 5416547;
        break;
    case 2939 :
        nibunno3x = 6464743;
        nibunnox3 = 5409507;
        break;
    case 2940 :
        nibunno3x = 6462217;
        nibunnox3 = 5402469;
        break;
    case 2941 :
        nibunno3x = 6459691;
        nibunnox3 = 5395432;
        break;
    case 2942 :
        nibunno3x = 6457168;
        nibunnox3 = 5388407;
        break;
    case 2943 :
        nibunno3x = 6454646;
        nibunnox3 = 5381388;
        break;
    case 2944 :
        nibunno3x = 6452125;
        nibunnox3 = 5374372;
        break;
    case 2945 :
        nibunno3x = 6449605;
        nibunnox3 = 5367362;
        break;
    case 2946 :
        nibunno3x = 6447088;
        nibunnox3 = 5360363;
        break;
    case 2947 :
        nibunno3x = 6444571;
        nibunnox3 = 5353367;
        break;
    case 2948 :
        nibunno3x = 6442056;
        nibunnox3 = 5346376;
        break;
    case 2949 :
        nibunno3x = 6439542;
        nibunnox3 = 5339393;
        break;
    case 2950 :
        nibunno3x = 6437028;
        nibunnox3 = 5332412;
        break;
    case 2951 :
        nibunno3x = 6434516;
        nibunnox3 = 5325440;
        break;
    case 2952 :
        nibunno3x = 6432007;
        nibunnox3 = 5318477;
        break;
    case 2953 :
        nibunno3x = 6429496;
        nibunnox3 = 5311511;
        break;
    case 2954 :
        nibunno3x = 6426988;
        nibunnox3 = 5304555;
        break;
    case 2955 :
        nibunno3x = 6424482;
        nibunnox3 = 5297608;
        break;
    case 2956 :
        nibunno3x = 6421976;
        nibunnox3 = 5290665;
        break;
    case 2957 :
        nibunno3x = 6419473;
        nibunnox3 = 5283729;
        break;
    case 2958 :
        nibunno3x = 6416970;
        nibunnox3 = 5276796;
        break;
    case 2959 :
        nibunno3x = 6414469;
        nibunnox3 = 5269874;
        break;
    case 2960 :
        nibunno3x = 6411967;
        nibunnox3 = 5262949;
        break;
    case 2961 :
        nibunno3x = 6409470;
        nibunnox3 = 5256039;
        break;
    case 2962 :
        nibunno3x = 6406972;
        nibunnox3 = 5249132;
        break;
    case 2963 :
        nibunno3x = 6404474;
        nibunnox3 = 5242227;
        break;
    case 2964 :
        nibunno3x = 6401980;
        nibunnox3 = 5235333;
        break;
    case 2965 :
        nibunno3x = 6399486;
        nibunnox3 = 5228441;
        break;
    case 2966 :
        nibunno3x = 6396994;
        nibunnox3 = 5221559;
        break;
    case 2967 :
        nibunno3x = 6394501;
        nibunnox3 = 5214675;
        break;
    case 2968 :
        nibunno3x = 6392011;
        nibunnox3 = 5207803;
        break;
    case 2969 :
        nibunno3x = 6389522;
        nibunnox3 = 5200937;
        break;
    case 2970 :
        nibunno3x = 6387034;
        nibunnox3 = 5194073;
        break;
    case 2971 :
        nibunno3x = 6384547;
        nibunnox3 = 5187216;
        break;
    case 2972 :
        nibunno3x = 6382063;
        nibunnox3 = 5180369;
        break;
    case 2973 :
        nibunno3x = 6379579;
        nibunnox3 = 5173524;
        break;
    case 2974 :
        nibunno3x = 6377095;
        nibunnox3 = 5166681;
        break;
    case 2975 :
        nibunno3x = 6374614;
        nibunnox3 = 5159850;
        break;
    case 2976 :
        nibunno3x = 6372134;
        nibunnox3 = 5153024;
        break;
    case 2977 :
        nibunno3x = 6369655;
        nibunnox3 = 5146201;
        break;
    case 2978 :
        nibunno3x = 6367177;
        nibunnox3 = 5139386;
        break;
    case 2979 :
        nibunno3x = 6364702;
        nibunnox3 = 5132579;
        break;
    case 2980 :
        nibunno3x = 6362227;
        nibunnox3 = 5125775;
        break;
    case 2981 :
        nibunno3x = 6359752;
        nibunnox3 = 5118974;
        break;
    case 2982 :
        nibunno3x = 6357280;
        nibunnox3 = 5112183;
        break;
    case 2983 :
        nibunno3x = 6354808;
        nibunnox3 = 5105395;
        break;
    case 2984 :
        nibunno3x = 6352339;
        nibunnox3 = 5098616;
        break;
    case 2985 :
        nibunno3x = 6349870;
        nibunnox3 = 5091841;
        break;
    case 2986 :
        nibunno3x = 6347401;
        nibunnox3 = 5085067;
        break;
    case 2987 :
        nibunno3x = 6344935;
        nibunnox3 = 5078304;
        break;
    case 2988 :
        nibunno3x = 6342469;
        nibunnox3 = 5071543;
        break;
    case 2989 :
        nibunno3x = 6340006;
        nibunnox3 = 5064793;
        break;
    case 2990 :
        nibunno3x = 6337543;
        nibunnox3 = 5058044;
        break;
    case 2991 :
        nibunno3x = 6335080;
        nibunnox3 = 5051298;
        break;
    case 2992 :
        nibunno3x = 6332620;
        nibunnox3 = 5044563;
        break;
    case 2993 :
        nibunno3x = 6330163;
        nibunnox3 = 5037838;
        break;
    case 2994 :
        nibunno3x = 6327703;
        nibunnox3 = 5031107;
        break;
    case 2995 :
        nibunno3x = 6325248;
        nibunnox3 = 5024391;
        break;
    case 2996 :
        nibunno3x = 6322792;
        nibunnox3 = 5017676;
        break;
    case 2997 :
        nibunno3x = 6320338;
        nibunnox3 = 5010968;
        break;
    case 2998 :
        nibunno3x = 6317886;
        nibunnox3 = 5004267;
        break;
    case 2999 :
        nibunno3x = 6315433;
        nibunnox3 = 4997569;
        break;
    case 3000 :
        nibunno3x = 6312982;
        nibunnox3 = 4990875;
        break;
    case 3001 :
        nibunno3x = 6310534;
        nibunnox3 = 4984193;
        break;
    case 3002 :
        nibunno3x = 6308086;
        nibunnox3 = 4977513;
        break;
    case 3003 :
        nibunno3x = 6305638;
        nibunnox3 = 4970835;
        break;
    case 3004 :
        nibunno3x = 6303193;
        nibunnox3 = 4964167;
        break;
    case 3005 :
        nibunno3x = 6300748;
        nibunnox3 = 4957501;
        break;
    case 3006 :
        nibunno3x = 6298306;
        nibunnox3 = 4950847;
        break;
    case 3007 :
        nibunno3x = 6295864;
        nibunnox3 = 4944193;
        break;
    case 3008 :
        nibunno3x = 6293422;
        nibunnox3 = 4937543;
        break;
    case 3009 :
        nibunno3x = 6290983;
        nibunnox3 = 4930903;
        break;
    case 3010 :
        nibunno3x = 6288544;
        nibunnox3 = 4924265;
        break;
    case 3011 :
        nibunno3x = 6286108;
        nibunnox3 = 4917638;
        break;
    case 3012 :
        nibunno3x = 6283672;
        nibunnox3 = 4911012;
        break;
    case 3013 :
        nibunno3x = 6281238;
        nibunnox3 = 4904394;
        break;
    case 3014 :
        nibunno3x = 6278803;
        nibunnox3 = 4897777;
        break;
    case 3015 :
        nibunno3x = 6276373;
        nibunnox3 = 4891174;
        break;
    case 3016 :
        nibunno3x = 6273942;
        nibunnox3 = 4884569;
        break;
    case 3017 :
        nibunno3x = 6271512;
        nibunnox3 = 4877971;
        break;
    case 3018 :
        nibunno3x = 6269083;
        nibunnox3 = 4871380;
        break;
    case 3019 :
        nibunno3x = 6266656;
        nibunnox3 = 4864793;
        break;
    case 3020 :
        nibunno3x = 6264229;
        nibunnox3 = 4858211;
        break;
    case 3021 :
        nibunno3x = 6261805;
        nibunnox3 = 4851637;
        break;
    case 3022 :
        nibunno3x = 6259381;
        nibunnox3 = 4845067;
        break;
    case 3023 :
        nibunno3x = 6256958;
        nibunnox3 = 4838502;
        break;
    case 3024 :
        nibunno3x = 6254536;
        nibunnox3 = 4831939;
        break;
    case 3025 :
        nibunno3x = 6252116;
        nibunnox3 = 4825387;
        break;
    case 3026 :
        nibunno3x = 6249697;
        nibunnox3 = 4818837;
        break;
    case 3027 :
        nibunno3x = 6247279;
        nibunnox3 = 4812293;
        break;
    case 3028 :
        nibunno3x = 6244862;
        nibunnox3 = 4805755;
        break;
    case 3029 :
        nibunno3x = 6242449;
        nibunnox3 = 4799228;
        break;
    case 3030 :
        nibunno3x = 6240034;
        nibunnox3 = 4792699;
        break;
    case 3031 :
        nibunno3x = 6237620;
        nibunnox3 = 4786175;
        break;
    case 3032 :
        nibunno3x = 6235210;
        nibunnox3 = 4779662;
        break;
    case 3033 :
        nibunno3x = 6232798;
        nibunnox3 = 4773148;
        break;
    case 3034 :
        nibunno3x = 6230389;
        nibunnox3 = 4766643;
        break;
    case 3035 :
        nibunno3x = 6227980;
        nibunnox3 = 4760142;
        break;
    case 3036 :
        nibunno3x = 6225574;
        nibunnox3 = 4753649;
        break;
    case 3037 :
        nibunno3x = 6223168;
        nibunnox3 = 4747159;
        break;
    case 3038 :
        nibunno3x = 6220762;
        nibunnox3 = 4740671;
        break;
    case 3039 :
        nibunno3x = 6218359;
        nibunnox3 = 4734193;
        break;
    case 3040 :
        nibunno3x = 6215958;
        nibunnox3 = 4727722;
        break;
    case 3041 :
        nibunno3x = 6213556;
        nibunnox3 = 4721253;
        break;
    case 3042 :
        nibunno3x = 6211156;
        nibunnox3 = 4714790;
        break;
    case 3043 :
        nibunno3x = 6208758;
        nibunnox3 = 4708333;
        break;
    case 3044 :
        nibunno3x = 6206359;
        nibunnox3 = 4701878;
        break;
    case 3045 :
        nibunno3x = 6203965;
        nibunnox3 = 4695438;
        break;
    case 3046 :
        nibunno3x = 6201570;
        nibunnox3 = 4688995;
        break;
    case 3047 :
        nibunno3x = 6199174;
        nibunnox3 = 4682555;
        break;
    case 3048 :
        nibunno3x = 6196783;
        nibunnox3 = 4676128;
        break;
    case 3049 :
        nibunno3x = 6194390;
        nibunnox3 = 4669700;
        break;
    case 3050 :
        nibunno3x = 6192000;
        nibunnox3 = 4663279;
        break;
    case 3051 :
        nibunno3x = 6189610;
        nibunnox3 = 4656862;
        break;
    case 3052 :
        nibunno3x = 6187222;
        nibunnox3 = 4650452;
        break;
    case 3053 :
        nibunno3x = 6184836;
        nibunnox3 = 4644049;
        break;
    case 3054 :
        nibunno3x = 6182449;
        nibunnox3 = 4637648;
        break;
    case 3055 :
        nibunno3x = 6180066;
        nibunnox3 = 4631256;
        break;
    case 3056 :
        nibunno3x = 6177682;
        nibunnox3 = 4624867;
        break;
    case 3057 :
        nibunno3x = 6175300;
        nibunnox3 = 4618483;
        break;
    case 3058 :
        nibunno3x = 6172918;
        nibunnox3 = 4612103;
        break;
    case 3059 :
        nibunno3x = 6170539;
        nibunnox3 = 4605732;
        break;
    case 3060 :
        nibunno3x = 6168160;
        nibunnox3 = 4599363;
        break;
    case 3061 :
        nibunno3x = 6165782;
        nibunnox3 = 4593000;
        break;
    case 3062 :
        nibunno3x = 6163405;
        nibunnox3 = 4586639;
        break;
    case 3063 :
        nibunno3x = 6161030;
        nibunnox3 = 4580289;
        break;
    case 3064 :
        nibunno3x = 6158656;
        nibunnox3 = 4573940;
        break;
    case 3065 :
        nibunno3x = 6156283;
        nibunnox3 = 4567598;
        break;
    case 3066 :
        nibunno3x = 6153913;
        nibunnox3 = 4561265;
        break;
    case 3067 :
        nibunno3x = 6151542;
        nibunnox3 = 4554931;
        break;
    case 3068 :
        nibunno3x = 6149172;
        nibunnox3 = 4548603;
        break;
    case 3069 :
        nibunno3x = 6146803;
        nibunnox3 = 4542280;
        break;
    case 3070 :
        nibunno3x = 6144436;
        nibunnox3 = 4535965;
        break;
    case 3071 :
        nibunno3x = 6142069;
        nibunnox3 = 4529650;
        break;
    case 3072 :
        nibunno3x = 6139705;
        nibunnox3 = 4523347;
        break;
    case 3073 :
        nibunno3x = 6137341;
        nibunnox3 = 4517045;
        break;
    case 3074 :
        nibunno3x = 6134980;
        nibunnox3 = 4510753;
        break;
    case 3075 :
        nibunno3x = 6132618;
        nibunnox3 = 4504459;
        break;
    case 3076 :
        nibunno3x = 6130258;
        nibunnox3 = 4498175;
        break;
    case 3077 :
        nibunno3x = 6127898;
        nibunnox3 = 4491893;
        break;
    case 3078 :
        nibunno3x = 6125542;
        nibunnox3 = 4485622;
        break;
    case 3079 :
        nibunno3x = 6123184;
        nibunnox3 = 4479347;
        break;
    case 3080 :
        nibunno3x = 6120828;
        nibunnox3 = 4473080;
        break;
    case 3081 :
        nibunno3x = 6118474;
        nibunnox3 = 4466823;
        break;
    case 3082 :
        nibunno3x = 6116120;
        nibunnox3 = 4460566;
        break;
    case 3083 :
        nibunno3x = 6113768;
        nibunnox3 = 4454318;
        break;
    case 3084 :
        nibunno3x = 6111416;
        nibunnox3 = 4448069;
        break;
    case 3085 :
        nibunno3x = 6109068;
        nibunnox3 = 4441832;
        break;
    case 3086 :
        nibunno3x = 6106718;
        nibunnox3 = 4435596;
        break;
    case 3087 :
        nibunno3x = 6104371;
        nibunnox3 = 4429367;
        break;
    case 3088 :
        nibunno3x = 6102025;
        nibunnox3 = 4423143;
        break;
    case 3089 :
        nibunno3x = 6099679;
        nibunnox3 = 4416921;
        break;
    case 3090 :
        nibunno3x = 6097334;
        nibunnox3 = 4410706;
        break;
    case 3091 :
        nibunno3x = 6094993;
        nibunnox3 = 4404500;
        break;
    case 3092 :
        nibunno3x = 6092650;
        nibunnox3 = 4398293;
        break;
    case 3093 :
        nibunno3x = 6090310;
        nibunnox3 = 4392095;
        break;
    case 3094 :
        nibunno3x = 6087970;
        nibunnox3 = 4385900;
        break;
    case 3095 :
        nibunno3x = 6085632;
        nibunnox3 = 4379710;
        break;
    case 3096 :
        nibunno3x = 6083293;
        nibunnox3 = 4373522;
        break;
    case 3097 :
        nibunno3x = 6080956;
        nibunnox3 = 4367341;
        break;
    case 3098 :
        nibunno3x = 6078622;
        nibunnox3 = 4361168;
        break;
    case 3099 :
        nibunno3x = 6076288;
        nibunnox3 = 4354999;
        break;
    case 3100 :
        nibunno3x = 6073954;
        nibunnox3 = 4348832;
        break;
    case 3101 :
        nibunno3x = 6071623;
        nibunnox3 = 4342674;
        break;
    case 3102 :
        nibunno3x = 6069292;
        nibunnox3 = 4336517;
        break;
    case 3103 :
        nibunno3x = 6066964;
        nibunnox3 = 4330372;
        break;
    case 3104 :
        nibunno3x = 6064634;
        nibunnox3 = 4324224;
        break;
    case 3105 :
        nibunno3x = 6062308;
        nibunnox3 = 4318085;
        break;
    case 3106 :
        nibunno3x = 6059982;
        nibunnox3 = 4311949;
        break;
    case 3107 :
        nibunno3x = 6057658;
        nibunnox3 = 4305823;
        break;
    case 3108 :
        nibunno3x = 6055333;
        nibunnox3 = 4299695;
        break;
    case 3109 :
        nibunno3x = 6053011;
        nibunnox3 = 4293576;
        break;
    case 3110 :
        nibunno3x = 6050689;
        nibunnox3 = 4287460;
        break;
    case 3111 :
        nibunno3x = 6048368;
        nibunnox3 = 4281350;
        break;
    case 3112 :
        nibunno3x = 6046050;
        nibunnox3 = 4275245;
        break;
    case 3113 :
        nibunno3x = 6043730;
        nibunnox3 = 4269143;
        break;
    case 3114 :
        nibunno3x = 6041413;
        nibunnox3 = 4263046;
        break;
    case 3115 :
        nibunno3x = 6039097;
        nibunnox3 = 4256955;
        break;
    case 3116 :
        nibunno3x = 6036782;
        nibunnox3 = 4250870;
        break;
    case 3117 :
        nibunno3x = 6034468;
        nibunnox3 = 4244788;
        break;
    case 3118 :
        nibunno3x = 6032155;
        nibunnox3 = 4238711;
        break;
    case 3119 :
        nibunno3x = 6029845;
        nibunnox3 = 4232643;
        break;
    case 3120 :
        nibunno3x = 6027534;
        nibunnox3 = 4226574;
        break;
    case 3121 :
        nibunno3x = 6025225;
        nibunnox3 = 4220515;
        break;
    case 3122 :
        nibunno3x = 6022916;
        nibunnox3 = 4214457;
        break;
    case 3123 :
        nibunno3x = 6020610;
        nibunnox3 = 4208406;
        break;
    case 3124 :
        nibunno3x = 6018304;
        nibunnox3 = 4202361;
        break;
    case 3125 :
        nibunno3x = 6015998;
        nibunnox3 = 4196316;
        break;
    case 3126 :
        nibunno3x = 6013694;
        nibunnox3 = 4190279;
        break;
    case 3127 :
        nibunno3x = 6011392;
        nibunnox3 = 4184247;
        break;
    case 3128 :
        nibunno3x = 6009091;
        nibunnox3 = 4178220;
        break;
    case 3129 :
        nibunno3x = 6006790;
        nibunnox3 = 4172196;
        break;
    case 3130 :
        nibunno3x = 6004490;
        nibunnox3 = 4166178;
        break;
    case 3131 :
        nibunno3x = 6002192;
        nibunnox3 = 4160165;
        break;
    case 3132 :
        nibunno3x = 5999894;
        nibunnox3 = 4154154;
        break;
    case 3133 :
        nibunno3x = 5997598;
        nibunnox3 = 4148150;
        break;
    case 3134 :
        nibunno3x = 5995303;
        nibunnox3 = 4142151;
        break;
    case 3135 :
        nibunno3x = 5993011;
        nibunnox3 = 4136162;
        break;
    case 3136 :
        nibunno3x = 5990718;
        nibunnox3 = 4130171;
        break;
    case 3137 :
        nibunno3x = 5988426;
        nibunnox3 = 4124186;
        break;
    case 3138 :
        nibunno3x = 5986135;
        nibunnox3 = 4118206;
        break;
    case 3139 :
        nibunno3x = 5983844;
        nibunnox3 = 4112228;
        break;
    case 3140 :
        nibunno3x = 5981557;
        nibunnox3 = 4106261;
        break;
    case 3141 :
        nibunno3x = 5979270;
        nibunnox3 = 4100295;
        break;
    case 3142 :
        nibunno3x = 5976982;
        nibunnox3 = 4094330;
        break;
    case 3143 :
        nibunno3x = 5974699;
        nibunnox3 = 4088379;
        break;
    case 3144 :
        nibunno3x = 5972413;
        nibunnox3 = 4082423;
        break;
    case 3145 :
        nibunno3x = 5970130;
        nibunnox3 = 4076477;
        break;
    case 3146 :
        nibunno3x = 5967850;
        nibunnox3 = 4070540;
        break;
    case 3147 :
        nibunno3x = 5965568;
        nibunnox3 = 4064600;
        break;
    case 3148 :
        nibunno3x = 5963290;
        nibunnox3 = 4058672;
        break;
    case 3149 :
        nibunno3x = 5961010;
        nibunnox3 = 4052740;
        break;
    case 3150 :
        nibunno3x = 5958733;
        nibunnox3 = 4046819;
        break;
    case 3151 :
        nibunno3x = 5956456;
        nibunnox3 = 4040898;
        break;
    case 3152 :
        nibunno3x = 5954180;
        nibunnox3 = 4034985;
        break;
    case 3153 :
        nibunno3x = 5951906;
        nibunnox3 = 4029077;
        break;
    case 3154 :
        nibunno3x = 5949634;
        nibunnox3 = 4023174;
        break;
    case 3155 :
        nibunno3x = 5947360;
        nibunnox3 = 4017269;
        break;
    case 3156 :
        nibunno3x = 5945089;
        nibunnox3 = 4011375;
        break;
    case 3157 :
        nibunno3x = 5942821;
        nibunnox3 = 4005489;
        break;
    case 3158 :
        nibunno3x = 5940550;
        nibunnox3 = 3999599;
        break;
    case 3159 :
        nibunno3x = 5938282;
        nibunnox3 = 3993717;
        break;
    case 3160 :
        nibunno3x = 5936017;
        nibunnox3 = 3987845;
        break;
    case 3161 :
        nibunno3x = 5933750;
        nibunnox3 = 3981971;
        break;
    case 3162 :
        nibunno3x = 5931487;
        nibunnox3 = 3976107;
        break;
    case 3163 :
        nibunno3x = 5929222;
        nibunnox3 = 3970241;
        break;
    case 3164 :
        nibunno3x = 5926960;
        nibunnox3 = 3964385;
        break;
    case 3165 :
        nibunno3x = 5924700;
        nibunnox3 = 3958533;
        break;
    case 3166 :
        nibunno3x = 5922439;
        nibunnox3 = 3952685;
        break;
    case 3167 :
        nibunno3x = 5920180;
        nibunnox3 = 3946841;
        break;
    case 3168 :
        nibunno3x = 5917921;
        nibunnox3 = 3941000;
        break;
    case 3169 :
        nibunno3x = 5915665;
        nibunnox3 = 3935168;
        break;
    case 3170 :
        nibunno3x = 5913409;
        nibunnox3 = 3929338;
        break;
    case 3171 :
        nibunno3x = 5911153;
        nibunnox3 = 3923510;
        break;
    case 3172 :
        nibunno3x = 5908900;
        nibunnox3 = 3917691;
        break;
    case 3173 :
        nibunno3x = 5906647;
        nibunnox3 = 3911874;
        break;
    case 3174 :
        nibunno3x = 5904394;
        nibunnox3 = 3906060;
        break;
    case 3175 :
        nibunno3x = 5902144;
        nibunnox3 = 3900254;
        break;
    case 3176 :
        nibunno3x = 5899894;
        nibunnox3 = 3894450;
        break;
    case 3177 :
        nibunno3x = 5897646;
        nibunnox3 = 3888652;
        break;
    case 3178 :
        nibunno3x = 5895398;
        nibunnox3 = 3882861;
        break;
    case 3179 :
        nibunno3x = 5893153;
        nibunnox3 = 3877074;
        break;
    case 3180 :
        nibunno3x = 5890906;
        nibunnox3 = 3871285;
        break;
    case 3181 :
        nibunno3x = 5888662;
        nibunnox3 = 3865507;
        break;
    case 3182 :
        nibunno3x = 5886418;
        nibunnox3 = 3859729;
        break;
    case 3183 :
        nibunno3x = 5884177;
        nibunnox3 = 3853962;
        break;
    case 3184 :
        nibunno3x = 5881936;
        nibunnox3 = 3848196;
        break;
    case 3185 :
        nibunno3x = 5879695;
        nibunnox3 = 3842432;
        break;
    case 3186 :
        nibunno3x = 5877457;
        nibunnox3 = 3836678;
        break;
    case 3187 :
        nibunno3x = 5875219;
        nibunnox3 = 3830925;
        break;
    case 3188 :
        nibunno3x = 5872981;
        nibunnox3 = 3825175;
        break;
    case 3189 :
        nibunno3x = 5870746;
        nibunnox3 = 3819432;
        break;
    case 3190 :
        nibunno3x = 5868511;
        nibunnox3 = 3813694;
        break;
    case 3191 :
        nibunno3x = 5866278;
        nibunnox3 = 3807960;
        break;
    case 3192 :
        nibunno3x = 5864044;
        nibunnox3 = 3802227;
        break;
    case 3193 :
        nibunno3x = 5861812;
        nibunnox3 = 3796501;
        break;
    case 3194 :
        nibunno3x = 5859583;
        nibunnox3 = 3790784;
        break;
    case 3195 :
        nibunno3x = 5857352;
        nibunnox3 = 3785065;
        break;
    case 3196 :
        nibunno3x = 5855125;
        nibunnox3 = 3779355;
        break;
    case 3197 :
        nibunno3x = 5852896;
        nibunnox3 = 3773643;
        break;
    case 3198 :
        nibunno3x = 5850670;
        nibunnox3 = 3767941;
        break;
    case 3199 :
        nibunno3x = 5848444;
        nibunnox3 = 3762241;
        break;
    case 3200 :
        nibunno3x = 5846221;
        nibunnox3 = 3756551;
        break;
    case 3201 :
        nibunno3x = 5843996;
        nibunnox3 = 3750857;
        break;
    case 3202 :
        nibunno3x = 5841775;
        nibunnox3 = 3745174;
        break;
    case 3203 :
        nibunno3x = 5839554;
        nibunnox3 = 3739492;
        break;
    case 3204 :
        nibunno3x = 5837334;
        nibunnox3 = 3733816;
        break;
    case 3205 :
        nibunno3x = 5835115;
        nibunnox3 = 3728146;
        break;
    case 3206 :
        nibunno3x = 5832895;
        nibunnox3 = 3722473;
        break;
    case 3207 :
        nibunno3x = 5830678;
        nibunnox3 = 3716810;
        break;
    case 3208 :
        nibunno3x = 5828462;
        nibunnox3 = 3711153;
        break;
    case 3209 :
        nibunno3x = 5826247;
        nibunnox3 = 3705496;
        break;
    case 3210 :
        nibunno3x = 5824033;
        nibunnox3 = 3699847;
        break;
    case 3211 :
        nibunno3x = 5821820;
        nibunnox3 = 3694202;
        break;
    case 3212 :
        nibunno3x = 5819608;
        nibunnox3 = 3688559;
        break;
    case 3213 :
        nibunno3x = 5817397;
        nibunnox3 = 3682922;
        break;
    case 3214 :
        nibunno3x = 5815189;
        nibunnox3 = 3677294;
        break;
    case 3215 :
        nibunno3x = 5812980;
        nibunnox3 = 3671664;
        break;
    case 3216 :
        nibunno3x = 5810770;
        nibunnox3 = 3666036;
        break;
    case 3217 :
        nibunno3x = 5808565;
        nibunnox3 = 3660421;
        break;
    case 3218 :
        nibunno3x = 5806360;
        nibunnox3 = 3654807;
        break;
    case 3219 :
        nibunno3x = 5804155;
        nibunnox3 = 3649196;
        break;
    case 3220 :
        nibunno3x = 5801950;
        nibunnox3 = 3643586;
        break;
    case 3221 :
        nibunno3x = 5799748;
        nibunnox3 = 3637986;
        break;
    case 3222 :
        nibunno3x = 5797546;
        nibunnox3 = 3632388;
        break;
    case 3223 :
        nibunno3x = 5795346;
        nibunnox3 = 3626794;
        break;
    case 3224 :
        nibunno3x = 5793146;
        nibunnox3 = 3621207;
        break;
    case 3225 :
        nibunno3x = 5790948;
        nibunnox3 = 3615621;
        break;
    case 3226 :
        nibunno3x = 5788750;
        nibunnox3 = 3610041;
        break;
    case 3227 :
        nibunno3x = 5786554;
        nibunnox3 = 3604466;
        break;
    case 3228 :
        nibunno3x = 5784358;
        nibunnox3 = 3598893;
        break;
    case 3229 :
        nibunno3x = 5782164;
        nibunnox3 = 3593325;
        break;
    case 3230 :
        nibunno3x = 5779970;
        nibunnox3 = 3587763;
        break;
    case 3231 :
        nibunno3x = 5777778;
        nibunnox3 = 3582203;
        break;
    case 3232 :
        nibunno3x = 5775586;
        nibunnox3 = 3576649;
        break;
    case 3233 :
        nibunno3x = 5773396;
        nibunnox3 = 3571099;
        break;
    case 3234 :
        nibunno3x = 5771206;
        nibunnox3 = 3565552;
        break;
    case 3235 :
        nibunno3x = 5769019;
        nibunnox3 = 3560014;
        break;
    case 3236 :
        nibunno3x = 5766830;
        nibunnox3 = 3554474;
        break;
    case 3237 :
        nibunno3x = 5764645;
        nibunnox3 = 3548943;
        break;
    case 3238 :
        nibunno3x = 5762458;
        nibunnox3 = 3543410;
        break;
    case 3239 :
        nibunno3x = 5760274;
        nibunnox3 = 3537887;
        break;
    case 3240 :
        nibunno3x = 5758093;
        nibunnox3 = 3532372;
        break;
    case 3241 :
        nibunno3x = 5755909;
        nibunnox3 = 3526851;
        break;
    case 3242 :
        nibunno3x = 5753728;
        nibunnox3 = 3521341;
        break;
    case 3243 :
        nibunno3x = 5751547;
        nibunnox3 = 3515830;
        break;
    case 3244 :
        nibunno3x = 5749369;
        nibunnox3 = 3510331;
        break;
    case 3245 :
        nibunno3x = 5747191;
        nibunnox3 = 3504833;
        break;
    case 3246 :
        nibunno3x = 5745013;
        nibunnox3 = 3499336;
        break;
    case 3247 :
        nibunno3x = 5742836;
        nibunnox3 = 3493845;
        break;
    case 3248 :
        nibunno3x = 5740663;
        nibunnox3 = 3488363;
        break;
    case 3249 :
        nibunno3x = 5738488;
        nibunnox3 = 3482879;
        break;
    case 3250 :
        nibunno3x = 5736313;
        nibunnox3 = 3477396;
        break;
    case 3251 :
        nibunno3x = 5734141;
        nibunnox3 = 3471923;
        break;
    case 3252 :
        nibunno3x = 5731969;
        nibunnox3 = 3466452;
        break;
    case 3253 :
        nibunno3x = 5729800;
        nibunnox3 = 3460990;
        break;
    case 3254 :
        nibunno3x = 5727631;
        nibunnox3 = 3455529;
        break;
    case 3255 :
        nibunno3x = 5725462;
        nibunnox3 = 3450070;
        break;
    case 3256 :
        nibunno3x = 5723296;
        nibunnox3 = 3444621;
        break;
    case 3257 :
        nibunno3x = 5721130;
        nibunnox3 = 3439173;
        break;
    case 3258 :
        nibunno3x = 5718964;
        nibunnox3 = 3433727;
        break;
    case 3259 :
        nibunno3x = 5716801;
        nibunnox3 = 3428290;
        break;
    case 3260 :
        nibunno3x = 5714638;
        nibunnox3 = 3422854;
        break;
    case 3261 :
        nibunno3x = 5712475;
        nibunnox3 = 3417421;
        break;
    case 3262 :
        nibunno3x = 5710314;
        nibunnox3 = 3411993;
        break;
    case 3263 :
        nibunno3x = 5708152;
        nibunnox3 = 3406566;
        break;
    case 3264 :
        nibunno3x = 5705995;
        nibunnox3 = 3401152;
        break;
    case 3265 :
        nibunno3x = 5703835;
        nibunnox3 = 3395732;
        break;
    case 3266 :
        nibunno3x = 5701678;
        nibunnox3 = 3390322;
        break;
    case 3267 :
        nibunno3x = 5699521;
        nibunnox3 = 3384914;
        break;
    case 3268 :
        nibunno3x = 5697367;
        nibunnox3 = 3379514;
        break;
    case 3269 :
        nibunno3x = 5695213;
        nibunnox3 = 3374116;
        break;
    case 3270 :
        nibunno3x = 5693059;
        nibunnox3 = 3368719;
        break;
    case 3271 :
        nibunno3x = 5690906;
        nibunnox3 = 3363329;
        break;
    case 3272 :
        nibunno3x = 5688756;
        nibunnox3 = 3357944;
        break;
    case 3273 :
        nibunno3x = 5686606;
        nibunnox3 = 3352564;
        break;
    case 3274 :
        nibunno3x = 5684455;
        nibunnox3 = 3347182;
        break;
    case 3275 :
        nibunno3x = 5682307;
        nibunnox3 = 3341809;
        break;
    case 3276 :
        nibunno3x = 5680159;
        nibunnox3 = 3336438;
        break;
    case 3277 :
        nibunno3x = 5678014;
        nibunnox3 = 3331075;
        break;
    case 3278 :
        nibunno3x = 5675869;
        nibunnox3 = 3325715;
        break;
    case 3279 :
        nibunno3x = 5673724;
        nibunnox3 = 3320356;
        break;
    case 3280 :
        nibunno3x = 5671579;
        nibunnox3 = 3314999;
        break;
    case 3281 :
        nibunno3x = 5669437;
        nibunnox3 = 3309651;
        break;
    case 3282 :
        nibunno3x = 5667295;
        nibunnox3 = 3304304;
        break;
    case 3283 :
        nibunno3x = 5665156;
        nibunnox3 = 3298966;
        break;
    case 3284 :
        nibunno3x = 5663016;
        nibunnox3 = 3293627;
        break;
    case 3285 :
        nibunno3x = 5660878;
        nibunnox3 = 3288297;
        break;
    case 3286 :
        nibunno3x = 5658739;
        nibunnox3 = 3282964;
        break;
    case 3287 :
        nibunno3x = 5656603;
        nibunnox3 = 3277640;
        break;
    case 3288 :
        nibunno3x = 5654467;
        nibunnox3 = 3272318;
        break;
    case 3289 :
        nibunno3x = 5652332;
        nibunnox3 = 3267002;
        break;
    case 3290 :
        nibunno3x = 5650198;
        nibunnox3 = 3261688;
        break;
    case 3291 :
        nibunno3x = 5648065;
        nibunnox3 = 3256378;
        break;
    case 3292 :
        nibunno3x = 5645935;
        nibunnox3 = 3251077;
        break;
    case 3293 :
        nibunno3x = 5643804;
        nibunnox3 = 3245776;
        break;
    case 3294 :
        nibunno3x = 5641675;
        nibunnox3 = 3240482;
        break;
    case 3295 :
        nibunno3x = 5639545;
        nibunnox3 = 3235185;
        break;
    case 3296 :
        nibunno3x = 5637416;
        nibunnox3 = 3229896;
        break;
    case 3297 :
        nibunno3x = 5635290;
        nibunnox3 = 3224611;
        break;
    case 3298 :
        nibunno3x = 5633164;
        nibunnox3 = 3219331;
        break;
    case 3299 :
        nibunno3x = 5631040;
        nibunnox3 = 3214057;
        break;
    case 3300 :
        nibunno3x = 5628916;
        nibunnox3 = 3208785;
        break;
    case 3301 :
        nibunno3x = 5626792;
        nibunnox3 = 3203513;
        break;
    case 3302 :
        nibunno3x = 5624670;
        nibunnox3 = 3198247;
        break;
    case 3303 :
        nibunno3x = 5622548;
        nibunnox3 = 3192987;
        break;
    case 3304 :
        nibunno3x = 5620429;
        nibunnox3 = 3187732;
        break;
    case 3305 :
        nibunno3x = 5618310;
        nibunnox3 = 3182478;
        break;
    case 3306 :
        nibunno3x = 5616192;
        nibunnox3 = 3177230;
        break;
    case 3307 :
        nibunno3x = 5614074;
        nibunnox3 = 3171983;
        break;
    case 3308 :
        nibunno3x = 5611957;
        nibunnox3 = 3166742;
        break;
    case 3309 :
        nibunno3x = 5609842;
        nibunnox3 = 3161506;
        break;
    case 3310 :
        nibunno3x = 5607727;
        nibunnox3 = 3156271;
        break;
    case 3311 :
        nibunno3x = 5605614;
        nibunnox3 = 3151042;
        break;
    case 3312 :
        nibunno3x = 5603502;
        nibunnox3 = 3145818;
        break;
    case 3313 :
        nibunno3x = 5601391;
        nibunnox3 = 3140600;
        break;
    case 3314 :
        nibunno3x = 5599279;
        nibunnox3 = 3135379;
        break;
    case 3315 :
        nibunno3x = 5597170;
        nibunnox3 = 3130167;
        break;
    case 3316 :
        nibunno3x = 5595061;
        nibunnox3 = 3124957;
        break;
    case 3317 :
        nibunno3x = 5592954;
        nibunnox3 = 3119752;
        break;
    case 3318 :
        nibunno3x = 5590846;
        nibunnox3 = 3114549;
        break;
    case 3319 :
        nibunno3x = 5588740;
        nibunnox3 = 3109351;
        break;
    case 3320 :
        nibunno3x = 5586637;
        nibunnox3 = 3104162;
        break;
    case 3321 :
        nibunno3x = 5584532;
        nibunnox3 = 3098971;
        break;
    case 3322 :
        nibunno3x = 5582428;
        nibunnox3 = 3093781;
        break;
    case 3323 :
        nibunno3x = 5580328;
        nibunnox3 = 3088605;
        break;
    case 3324 :
        nibunno3x = 5578226;
        nibunnox3 = 3083425;
        break;
    case 3325 :
        nibunno3x = 5576126;
        nibunnox3 = 3078251;
        break;
    case 3326 :
        nibunno3x = 5574028;
        nibunnox3 = 3073082;
        break;
    case 3327 :
        nibunno3x = 5571928;
        nibunnox3 = 3067912;
        break;
    case 3328 :
        nibunno3x = 5569831;
        nibunnox3 = 3062750;
        break;
    case 3329 :
        nibunno3x = 5567734;
        nibunnox3 = 3057589;
        break;
    case 3330 :
        nibunno3x = 5565640;
        nibunnox3 = 3052438;
        break;
    case 3331 :
        nibunno3x = 5563546;
        nibunnox3 = 3047289;
        break;
    case 3332 :
        nibunno3x = 5561452;
        nibunnox3 = 3042140;
        break;
    case 3333 :
        nibunno3x = 5559358;
        nibunnox3 = 3036993;
        break;
    case 3334 :
        nibunno3x = 5557267;
        nibunnox3 = 3031856;
        break;
    case 3335 :
        nibunno3x = 5555176;
        nibunnox3 = 3026719;
        break;
    case 3336 :
        nibunno3x = 5553086;
        nibunnox3 = 3021588;
        break;
    case 3337 :
        nibunno3x = 5550998;
        nibunnox3 = 3016463;
        break;
    case 3338 :
        nibunno3x = 5548909;
        nibunnox3 = 3011334;
        break;
    case 3339 :
        nibunno3x = 5546824;
        nibunnox3 = 3006218;
        break;
    case 3340 :
        nibunno3x = 5544738;
        nibunnox3 = 3001102;
        break;
    case 3341 :
        nibunno3x = 5542652;
        nibunnox3 = 2995989;
        break;
    case 3342 :
        nibunno3x = 5540569;
        nibunnox3 = 2990882;
        break;
    case 3343 :
        nibunno3x = 5538486;
        nibunnox3 = 2985776;
        break;
    case 3344 :
        nibunno3x = 5536404;
        nibunnox3 = 2980676;
        break;
    case 3345 :
        nibunno3x = 5534322;
        nibunnox3 = 2975577;
        break;
    case 3346 :
        nibunno3x = 5532241;
        nibunnox3 = 2970484;
        break;
    case 3347 :
        nibunno3x = 5530162;
        nibunnox3 = 2965395;
        break;
    case 3348 :
        nibunno3x = 5528083;
        nibunnox3 = 2960308;
        break;
    case 3349 :
        nibunno3x = 5526006;
        nibunnox3 = 2955226;
        break;
    case 3350 :
        nibunno3x = 5523930;
        nibunnox3 = 2950150;
        break;
    case 3351 :
        nibunno3x = 5521854;
        nibunnox3 = 2945075;
        break;
    case 3352 :
        nibunno3x = 5519779;
        nibunnox3 = 2940004;
        break;
    case 3353 :
        nibunno3x = 5517704;
        nibunnox3 = 2934936;
        break;
    case 3354 :
        nibunno3x = 5515633;
        nibunnox3 = 2929877;
        break;
    case 3355 :
        nibunno3x = 5513560;
        nibunnox3 = 2924815;
        break;
    case 3356 :
        nibunno3x = 5511488;
        nibunnox3 = 2919758;
        break;
    case 3357 :
        nibunno3x = 5509418;
        nibunnox3 = 2914707;
        break;
    case 3358 :
        nibunno3x = 5507350;
        nibunnox3 = 2909661;
        break;
    case 3359 :
        nibunno3x = 5505282;
        nibunnox3 = 2904616;
        break;
    case 3360 :
        nibunno3x = 5503213;
        nibunnox3 = 2899573;
        break;
    case 3361 :
        nibunno3x = 5501146;
        nibunnox3 = 2894534;
        break;
    case 3362 :
        nibunno3x = 5499082;
        nibunnox3 = 2889506;
        break;
    case 3363 :
        nibunno3x = 5497018;
        nibunnox3 = 2884478;
        break;
    case 3364 :
        nibunno3x = 5494954;
        nibunnox3 = 2879452;
        break;
    case 3365 :
        nibunno3x = 5492890;
        nibunnox3 = 2874427;
        break;
    case 3366 :
        nibunno3x = 5490829;
        nibunnox3 = 2869410;
        break;
    case 3367 :
        nibunno3x = 5488768;
        nibunnox3 = 2864396;
        break;
    case 3368 :
        nibunno3x = 5486707;
        nibunnox3 = 2859383;
        break;
    case 3369 :
        nibunno3x = 5484649;
        nibunnox3 = 2854379;
        break;
    case 3370 :
        nibunno3x = 5482591;
        nibunnox3 = 2849376;
        break;
    case 3371 :
        nibunno3x = 5480533;
        nibunnox3 = 2844375;
        break;
    case 3372 :
        nibunno3x = 5478476;
        nibunnox3 = 2839379;
        break;
    case 3373 :
        nibunno3x = 5476422;
        nibunnox3 = 2834388;
        break;
    case 3374 :
        nibunno3x = 5474366;
        nibunnox3 = 2829399;
        break;
    case 3375 :
        nibunno3x = 5472313;
        nibunnox3 = 2824415;
        break;
    case 3376 :
        nibunno3x = 5470261;
        nibunnox3 = 2819435;
        break;
    case 3377 :
        nibunno3x = 5468209;
        nibunnox3 = 2814457;
        break;
    case 3378 :
        nibunno3x = 5466157;
        nibunnox3 = 2809481;
        break;
    case 3379 :
        nibunno3x = 5464108;
        nibunnox3 = 2804513;
        break;
    case 3380 :
        nibunno3x = 5462059;
        nibunnox3 = 2799548;
        break;
    case 3381 :
        nibunno3x = 5460010;
        nibunnox3 = 2794583;
        break;
    case 3382 :
        nibunno3x = 5457964;
        nibunnox3 = 2789626;
        break;
    case 3383 :
        nibunno3x = 5455916;
        nibunnox3 = 2784669;
        break;
    case 3384 :
        nibunno3x = 5453872;
        nibunnox3 = 2779719;
        break;
    case 3385 :
        nibunno3x = 5451828;
        nibunnox3 = 2774772;
        break;
    case 3386 :
        nibunno3x = 5449783;
        nibunnox3 = 2769826;
        break;
    case 3387 :
        nibunno3x = 5447740;
        nibunnox3 = 2764884;
        break;
    case 3388 :
        nibunno3x = 5445700;
        nibunnox3 = 2759951;
        break;
    case 3389 :
        nibunno3x = 5443657;
        nibunnox3 = 2755013;
        break;
    case 3390 :
        nibunno3x = 5441617;
        nibunnox3 = 2750083;
        break;
    case 3391 :
        nibunno3x = 5439580;
        nibunnox3 = 2745162;
        break;
    case 3392 :
        nibunno3x = 5437540;
        nibunnox3 = 2740236;
        break;
    case 3393 :
        nibunno3x = 5435503;
        nibunnox3 = 2735318;
        break;
    case 3394 :
        nibunno3x = 5433466;
        nibunnox3 = 2730401;
        break;
    case 3395 :
        nibunno3x = 5431432;
        nibunnox3 = 2725493;
        break;
    case 3396 :
        nibunno3x = 5429396;
        nibunnox3 = 2720582;
        break;
    case 3397 :
        nibunno3x = 5427362;
        nibunnox3 = 2715677;
        break;
    case 3398 :
        nibunno3x = 5425330;
        nibunnox3 = 2710777;
        break;
    case 3399 :
        nibunno3x = 5423299;
        nibunnox3 = 2705882;
        break;
    case 3400 :
        nibunno3x = 5421268;
        nibunnox3 = 2700989;
        break;
    case 3401 :
        nibunno3x = 5419237;
        nibunnox3 = 2696097;
        break;
    case 3402 :
        nibunno3x = 5417208;
        nibunnox3 = 2691210;
        break;
    case 3403 :
        nibunno3x = 5415180;
        nibunnox3 = 2686327;
        break;
    case 3404 :
        nibunno3x = 5413153;
        nibunnox3 = 2681451;
        break;
    case 3405 :
        nibunno3x = 5411125;
        nibunnox3 = 2676572;
        break;
    case 3406 :
        nibunno3x = 5409100;
        nibunnox3 = 2671701;
        break;
    case 3407 :
        nibunno3x = 5407075;
        nibunnox3 = 2666833;
        break;
    case 3408 :
        nibunno3x = 5405052;
        nibunnox3 = 2661969;
        break;
    case 3409 :
        nibunno3x = 5403028;
        nibunnox3 = 2657105;
        break;
    case 3410 :
        nibunno3x = 5401006;
        nibunnox3 = 2652249;
        break;
    case 3411 :
        nibunno3x = 5398984;
        nibunnox3 = 2647392;
        break;
    case 3412 :
        nibunno3x = 5396965;
        nibunnox3 = 2642545;
        break;
    case 3413 :
        nibunno3x = 5394946;
        nibunnox3 = 2637699;
        break;
    case 3414 :
        nibunno3x = 5392927;
        nibunnox3 = 2632854;
        break;
    case 3415 :
        nibunno3x = 5390908;
        nibunnox3 = 2628011;
        break;
    case 3416 :
        nibunno3x = 5388892;
        nibunnox3 = 2623176;
        break;
    case 3417 :
        nibunno3x = 5386876;
        nibunnox3 = 2618342;
        break;
    case 3418 :
        nibunno3x = 5384862;
        nibunnox3 = 2613515;
        break;
    case 3419 :
        nibunno3x = 5382847;
        nibunnox3 = 2608688;
        break;
    case 3420 :
        nibunno3x = 5380834;
        nibunnox3 = 2603866;
        break;
    case 3421 :
        nibunno3x = 5378821;
        nibunnox3 = 2599046;
        break;
    case 3422 :
        nibunno3x = 5376811;
        nibunnox3 = 2594234;
        break;
    case 3423 :
        nibunno3x = 5374800;
        nibunnox3 = 2589420;
        break;
    case 3424 :
        nibunno3x = 5372791;
        nibunnox3 = 2584615;
        break;
    case 3425 :
        nibunno3x = 5370781;
        nibunnox3 = 2579807;
        break;
    case 3426 :
        nibunno3x = 5368774;
        nibunnox3 = 2575008;
        break;
    case 3427 :
        nibunno3x = 5366767;
        nibunnox3 = 2570211;
        break;
    case 3428 :
        nibunno3x = 5364760;
        nibunnox3 = 2565414;
        break;
    case 3429 :
        nibunno3x = 5362754;
        nibunnox3 = 2560623;
        break;
    case 3430 :
        nibunno3x = 5360750;
        nibunnox3 = 2555837;
        break;
    case 3431 :
        nibunno3x = 5358748;
        nibunnox3 = 2551056;
        break;
    case 3432 :
        nibunno3x = 5356744;
        nibunnox3 = 2546272;
        break;
    case 3433 :
        nibunno3x = 5354743;
        nibunnox3 = 2541497;
        break;
    case 3434 :
        nibunno3x = 5352742;
        nibunnox3 = 2536724;
        break;
    case 3435 :
        nibunno3x = 5350741;
        nibunnox3 = 2531951;
        break;
    case 3436 :
        nibunno3x = 5348742;
        nibunnox3 = 2527184;
        break;
    case 3437 :
        nibunno3x = 5346744;
        nibunnox3 = 2522422;
        break;
    case 3438 :
        nibunno3x = 5344747;
        nibunnox3 = 2517665;
        break;
    case 3439 :
        nibunno3x = 5342749;
        nibunnox3 = 2512905;
        break;
    case 3440 :
        nibunno3x = 5340754;
        nibunnox3 = 2508155;
        break;
    case 3441 :
        nibunno3x = 5338759;
        nibunnox3 = 2503405;
        break;
    case 3442 :
        nibunno3x = 5336764;
        nibunnox3 = 2498657;
        break;
    case 3443 :
        nibunno3x = 5334772;
        nibunnox3 = 2493918;
        break;
    case 3444 :
        nibunno3x = 5332780;
        nibunnox3 = 2489179;
        break;
    case 3445 :
        nibunno3x = 5330788;
        nibunnox3 = 2484442;
        break;
    case 3446 :
        nibunno3x = 5328799;
        nibunnox3 = 2479714;
        break;
    case 3447 :
        nibunno3x = 5326808;
        nibunnox3 = 2474984;
        break;
    case 3448 :
        nibunno3x = 5324821;
        nibunnox3 = 2470262;
        break;
    case 3449 :
        nibunno3x = 5322832;
        nibunnox3 = 2465537;
        break;
    case 3450 :
        nibunno3x = 5320846;
        nibunnox3 = 2460821;
        break;
    case 3451 :
        nibunno3x = 5318860;
        nibunnox3 = 2456107;
        break;
    case 3452 :
        nibunno3x = 5316874;
        nibunnox3 = 2451394;
        break;
    case 3453 :
        nibunno3x = 5314888;
        nibunnox3 = 2446683;
        break;
    case 3454 :
        nibunno3x = 5312905;
        nibunnox3 = 2441980;
        break;
    case 3455 :
        nibunno3x = 5310922;
        nibunnox3 = 2437278;
        break;
    case 3456 :
        nibunno3x = 5308942;
        nibunnox3 = 2432584;
        break;
    case 3457 :
        nibunno3x = 5306960;
        nibunnox3 = 2427889;
        break;
    case 3458 :
        nibunno3x = 5304980;
        nibunnox3 = 2423198;
        break;
    case 3459 :
        nibunno3x = 5303002;
        nibunnox3 = 2418512;
        break;
    case 3460 :
        nibunno3x = 5301022;
        nibunnox3 = 2413825;
        break;
    case 3461 :
        nibunno3x = 5299045;
        nibunnox3 = 2409145;
        break;
    case 3462 :
        nibunno3x = 5297068;
        nibunnox3 = 2404467;
        break;
    case 3463 :
        nibunno3x = 5295091;
        nibunnox3 = 2399790;
        break;
    case 3464 :
        nibunno3x = 5293117;
        nibunnox3 = 2395122;
        break;
    case 3465 :
        nibunno3x = 5291143;
        nibunnox3 = 2390455;
        break;
    case 3466 :
        nibunno3x = 5289169;
        nibunnox3 = 2385789;
        break;
    case 3467 :
        nibunno3x = 5287198;
        nibunnox3 = 2381132;
        break;
    case 3468 :
        nibunno3x = 5285226;
        nibunnox3 = 2376472;
        break;
    case 3469 :
        nibunno3x = 5283256;
        nibunnox3 = 2371822;
        break;
    case 3470 :
        nibunno3x = 5281285;
        nibunnox3 = 2367169;
        break;
    case 3471 :
        nibunno3x = 5279317;
        nibunnox3 = 2362524;
        break;
    case 3472 :
        nibunno3x = 5277348;
        nibunnox3 = 2357877;
        break;
    case 3473 :
        nibunno3x = 5275381;
        nibunnox3 = 2353238;
        break;
    case 3474 :
        nibunno3x = 5273414;
        nibunnox3 = 2348601;
        break;
    case 3475 :
        nibunno3x = 5271448;
        nibunnox3 = 2343966;
        break;
    case 3476 :
        nibunno3x = 5269483;
        nibunnox3 = 2339335;
        break;
    case 3477 :
        nibunno3x = 5267521;
        nibunnox3 = 2334711;
        break;
    case 3478 :
        nibunno3x = 5265556;
        nibunnox3 = 2330083;
        break;
    case 3479 :
        nibunno3x = 5263594;
        nibunnox3 = 2325464;
        break;
    case 3480 :
        nibunno3x = 5261634;
        nibunnox3 = 2320849;
        break;
    case 3481 :
        nibunno3x = 5259673;
        nibunnox3 = 2316235;
        break;
    case 3482 :
        nibunno3x = 5257712;
        nibunnox3 = 2311622;
        break;
    case 3483 :
        nibunno3x = 5255755;
        nibunnox3 = 2307019;
        break;
    case 3484 :
        nibunno3x = 5253796;
        nibunnox3 = 2302412;
        break;
    case 3485 :
        nibunno3x = 5251838;
        nibunnox3 = 2297811;
        break;
    case 3486 :
        nibunno3x = 5249882;
        nibunnox3 = 2293214;
        break;
    case 3487 :
        nibunno3x = 5247928;
        nibunnox3 = 2288622;
        break;
    case 3488 :
        nibunno3x = 5245972;
        nibunnox3 = 2284028;
        break;
    case 3489 :
        nibunno3x = 5244019;
        nibunnox3 = 2279443;
        break;
    case 3490 :
        nibunno3x = 5242066;
        nibunnox3 = 2274859;
        break;
    case 3491 :
        nibunno3x = 5240113;
        nibunnox3 = 2270276;
        break;
    case 3492 :
        nibunno3x = 5238163;
        nibunnox3 = 2265701;
        break;
    case 3493 :
        nibunno3x = 5236212;
        nibunnox3 = 2261125;
        break;
    case 3494 :
        nibunno3x = 5234263;
        nibunnox3 = 2256556;
        break;
    case 3495 :
        nibunno3x = 5232313;
        nibunnox3 = 2251985;
        break;
    case 3496 :
        nibunno3x = 5230366;
        nibunnox3 = 2247423;
        break;
    case 3497 :
        nibunno3x = 5228419;
        nibunnox3 = 2242863;
        break;
    case 3498 :
        nibunno3x = 5226472;
        nibunnox3 = 2238302;
        break;
    case 3499 :
        nibunno3x = 5224526;
        nibunnox3 = 2233748;
        break;
    case 3500 :
        nibunno3x = 5222582;
        nibunnox3 = 2229198;
        break;
    case 3501 :
        nibunno3x = 5220637;
        nibunnox3 = 2224645;
        break;
    case 3502 :
        nibunno3x = 5218696;
        nibunnox3 = 2220105;
        break;
    case 3503 :
        nibunno3x = 5216754;
        nibunnox3 = 2215561;
        break;
    case 3504 :
        nibunno3x = 5214811;
        nibunnox3 = 2211020;
        break;
    case 3505 :
        nibunno3x = 5212873;
        nibunnox3 = 2206491;
        break;
    case 3506 :
        nibunno3x = 5210932;
        nibunnox3 = 2201956;
        break;
    case 3507 :
        nibunno3x = 5208994;
        nibunnox3 = 2197429;
        break;
    case 3508 :
        nibunno3x = 5207056;
        nibunnox3 = 2192903;
        break;
    case 3509 :
        nibunno3x = 5205118;
        nibunnox3 = 2188379;
        break;
    case 3510 :
        nibunno3x = 5203182;
        nibunnox3 = 2183859;
        break;
    case 3511 :
        nibunno3x = 5201246;
        nibunnox3 = 2179344;
        break;
    case 3512 :
        nibunno3x = 5199313;
        nibunnox3 = 2174834;
        break;
    case 3513 :
        nibunno3x = 5197378;
        nibunnox3 = 2170322;
        break;
    case 3514 :
        nibunno3x = 5195444;
        nibunnox3 = 2165815;
        break;
    case 3515 :
        nibunno3x = 5193512;
        nibunnox3 = 2161312;
        break;
    case 3516 :
        nibunno3x = 5191582;
        nibunnox3 = 2156814;
        break;
    case 3517 :
        nibunno3x = 5189650;
        nibunnox3 = 2152314;
        break;
    case 3518 :
        nibunno3x = 5187721;
        nibunnox3 = 2147822;
        break;
    case 3519 :
        nibunno3x = 5185792;
        nibunnox3 = 2143332;
        break;
    case 3520 :
        nibunno3x = 5183866;
        nibunnox3 = 2138849;
        break;
    case 3521 :
        nibunno3x = 5181937;
        nibunnox3 = 2134361;
        break;
    case 3522 :
        nibunno3x = 5180011;
        nibunnox3 = 2129881;
        break;
    case 3523 :
        nibunno3x = 5178085;
        nibunnox3 = 2125402;
        break;
    case 3524 :
        nibunno3x = 5176160;
        nibunnox3 = 2120929;
        break;
    case 3525 :
        nibunno3x = 5174236;
        nibunnox3 = 2116457;
        break;
    case 3526 :
        nibunno3x = 5172313;
        nibunnox3 = 2111988;
        break;
    case 3527 :
        nibunno3x = 5170390;
        nibunnox3 = 2107522;
        break;
    case 3528 :
        nibunno3x = 5168470;
        nibunnox3 = 2103064;
        break;
    case 3529 :
        nibunno3x = 5166550;
        nibunnox3 = 2098607;
        break;
    case 3530 :
        nibunno3x = 5164630;
        nibunnox3 = 2094151;
        break;
    case 3531 :
        nibunno3x = 5162710;
        nibunnox3 = 2089697;
        break;
    case 3532 :
        nibunno3x = 5160792;
        nibunnox3 = 2085247;
        break;
    case 3533 :
        nibunno3x = 5158874;
        nibunnox3 = 2080802;
        break;
    case 3534 :
        nibunno3x = 5156959;
        nibunnox3 = 2076362;
        break;
    case 3535 :
        nibunno3x = 5155042;
        nibunnox3 = 2071919;
        break;
    case 3536 :
        nibunno3x = 5153128;
        nibunnox3 = 2067486;
        break;
    case 3537 :
        nibunno3x = 5151214;
        nibunnox3 = 2063052;
        break;
    case 3538 :
        nibunno3x = 5149300;
        nibunnox3 = 2058620;
        break;
    case 3539 :
        nibunno3x = 5147386;
        nibunnox3 = 2054190;
        break;
    case 3540 :
        nibunno3x = 5145475;
        nibunnox3 = 2049768;
        break;
    case 3541 :
        nibunno3x = 5143564;
        nibunnox3 = 2045347;
        break;
    case 3542 :
        nibunno3x = 5141654;
        nibunnox3 = 2040931;
        break;
    case 3543 :
        nibunno3x = 5139745;
        nibunnox3 = 2036515;
        break;
    case 3544 :
        nibunno3x = 5137837;
        nibunnox3 = 2032105;
        break;
    case 3545 :
        nibunno3x = 5135929;
        nibunnox3 = 2027696;
        break;
    case 3546 :
        nibunno3x = 5134021;
        nibunnox3 = 2023288;
        break;
    case 3547 :
        nibunno3x = 5132116;
        nibunnox3 = 2018888;
        break;
    case 3548 :
        nibunno3x = 5130211;
        nibunnox3 = 2014490;
        break;
    case 3549 :
        nibunno3x = 5128306;
        nibunnox3 = 2010092;
        break;
    case 3550 :
        nibunno3x = 5126401;
        nibunnox3 = 2005697;
        break;
    case 3551 :
        nibunno3x = 5124499;
        nibunnox3 = 2001308;
        break;
    case 3552 :
        nibunno3x = 5122597;
        nibunnox3 = 1996922;
        break;
    case 3553 :
        nibunno3x = 5120695;
        nibunnox3 = 1992537;
        break;
    case 3554 :
        nibunno3x = 5118796;
        nibunnox3 = 1988160;
        break;
    case 3555 :
        nibunno3x = 5116897;
        nibunnox3 = 1983784;
        break;
    case 3556 :
        nibunno3x = 5114998;
        nibunnox3 = 1979409;
        break;
    case 3557 :
        nibunno3x = 5113099;
        nibunnox3 = 1975035;
        break;
    case 3558 :
        nibunno3x = 5111203;
        nibunnox3 = 1970670;
        break;
    case 3559 :
        nibunno3x = 5109306;
        nibunnox3 = 1966302;
        break;
    case 3560 :
        nibunno3x = 5107411;
        nibunnox3 = 1961942;
        break;
    case 3561 :
        nibunno3x = 5105515;
        nibunnox3 = 1957581;
        break;
    case 3562 :
        nibunno3x = 5103620;
        nibunnox3 = 1953224;
        break;
    case 3563 :
        nibunno3x = 5101728;
        nibunnox3 = 1948872;
        break;
    case 3564 :
        nibunno3x = 5099836;
        nibunnox3 = 1944525;
        break;
    case 3565 :
        nibunno3x = 5097943;
        nibunnox3 = 1940174;
        break;
    case 3566 :
        nibunno3x = 5096053;
        nibunnox3 = 1935832;
        break;
    case 3567 :
        nibunno3x = 5094163;
        nibunnox3 = 1931492;
        break;
    case 3568 :
        nibunno3x = 5092273;
        nibunnox3 = 1927153;
        break;
    case 3569 :
        nibunno3x = 5090386;
        nibunnox3 = 1922822;
        break;
    case 3570 :
        nibunno3x = 5088498;
        nibunnox3 = 1918487;
        break;
    case 3571 :
        nibunno3x = 5086610;
        nibunnox3 = 1914159;
        break;
    case 3572 :
        nibunno3x = 5084725;
        nibunnox3 = 1909835;
        break;
    case 3573 :
        nibunno3x = 5082838;
        nibunnox3 = 1905508;
        break;
    case 3574 :
        nibunno3x = 5080954;
        nibunnox3 = 1901190;
        break;
    case 3575 :
        nibunno3x = 5079070;
        nibunnox3 = 1896873;
        break;
    case 3576 :
        nibunno3x = 5077188;
        nibunnox3 = 1892561;
        break;
    case 3577 :
        nibunno3x = 5075305;
        nibunnox3 = 1888249;
        break;
    case 3578 :
        nibunno3x = 5073424;
        nibunnox3 = 1883942;
        break;
    case 3579 :
        nibunno3x = 5071543;
        nibunnox3 = 1879637;
        break;
    case 3580 :
        nibunno3x = 5069665;
        nibunnox3 = 1875340;
        break;
    case 3581 :
        nibunno3x = 5067784;
        nibunnox3 = 1871036;
        break;
    case 3582 :
        nibunno3x = 5065906;
        nibunnox3 = 1866742;
        break;
    case 3583 :
        nibunno3x = 5064030;
        nibunnox3 = 1862452;
        break;
    case 3584 :
        nibunno3x = 5062153;
        nibunnox3 = 1858162;
        break;
    case 3585 :
        nibunno3x = 5060276;
        nibunnox3 = 1853875;
        break;
    case 3586 :
        nibunno3x = 5058402;
        nibunnox3 = 1849591;
        break;
    case 3587 :
        nibunno3x = 5056528;
        nibunnox3 = 1845312;
        break;
    case 3588 :
        nibunno3x = 5054654;
        nibunnox3 = 1841034;
        break;
    case 3589 :
        nibunno3x = 5052781;
        nibunnox3 = 1836759;
        break;
    case 3590 :
        nibunno3x = 5050909;
        nibunnox3 = 1832487;
        break;
    case 3591 :
        nibunno3x = 5049038;
        nibunnox3 = 1828219;
        break;
    case 3592 :
        nibunno3x = 5047168;
        nibunnox3 = 1823954;
        break;
    case 3593 :
        nibunno3x = 5045299;
        nibunnox3 = 1819692;
        break;
    case 3594 :
        nibunno3x = 5043430;
        nibunnox3 = 1815432;
        break;
    case 3595 :
        nibunno3x = 5041561;
        nibunnox3 = 1811173;
        break;
    case 3596 :
        nibunno3x = 5039695;
        nibunnox3 = 1806922;
        break;
    case 3597 :
        nibunno3x = 5037828;
        nibunnox3 = 1802669;
        break;
    case 3598 :
        nibunno3x = 5035963;
        nibunnox3 = 1798424;
        break;
    case 3599 :
        nibunno3x = 5034097;
        nibunnox3 = 1794177;
        break;
    case 3600 :
        nibunno3x = 5032234;
        nibunnox3 = 1789937;
        break;
    case 3601 :
        nibunno3x = 5030370;
        nibunnox3 = 1785696;
        break;
    case 3602 :
        nibunno3x = 5028508;
        nibunnox3 = 1781462;
        break;
    case 3603 :
        nibunno3x = 5026645;
        nibunnox3 = 1777226;
        break;
    case 3604 :
        nibunno3x = 5024785;
        nibunnox3 = 1772999;
        break;
    case 3605 :
        nibunno3x = 5022925;
        nibunnox3 = 1768772;
        break;
    case 3606 :
        nibunno3x = 5021065;
        nibunnox3 = 1764546;
        break;
    case 3607 :
        nibunno3x = 5019205;
        nibunnox3 = 1760322;
        break;
    case 3608 :
        nibunno3x = 5017348;
        nibunnox3 = 1756106;
        break;
    case 3609 :
        nibunno3x = 5015491;
        nibunnox3 = 1751890;
        break;
    case 3610 :
        nibunno3x = 5013634;
        nibunnox3 = 1747677;
        break;
    case 3611 :
        nibunno3x = 5011778;
        nibunnox3 = 1743467;
        break;
    case 3612 :
        nibunno3x = 5009923;
        nibunnox3 = 1739258;
        break;
    case 3613 :
        nibunno3x = 5008070;
        nibunnox3 = 1735058;
        break;
    case 3614 :
        nibunno3x = 5006216;
        nibunnox3 = 1730856;
        break;
    case 3615 :
        nibunno3x = 5004364;
        nibunnox3 = 1726658;
        break;
    case 3616 :
        nibunno3x = 5002512;
        nibunnox3 = 1722461;
        break;
    case 3617 :
        nibunno3x = 5000662;
        nibunnox3 = 1718273;
        break;
    case 3618 :
        nibunno3x = 4998811;
        nibunnox3 = 1714082;
        break;
    case 3619 :
        nibunno3x = 4996962;
        nibunnox3 = 1709895;
        break;
    case 3620 :
        nibunno3x = 4995114;
        nibunnox3 = 1705713;
        break;
    case 3621 :
        nibunno3x = 4993266;
        nibunnox3 = 1701531;
        break;
    case 3622 :
        nibunno3x = 4991418;
        nibunnox3 = 1697352;
        break;
    case 3623 :
        nibunno3x = 4989571;
        nibunnox3 = 1693177;
        break;
    case 3624 :
        nibunno3x = 4987726;
        nibunnox3 = 1689006;
        break;
    case 3625 :
        nibunno3x = 4985881;
        nibunnox3 = 1684837;
        break;
    case 3626 :
        nibunno3x = 4984036;
        nibunnox3 = 1680669;
        break;
    case 3627 :
        nibunno3x = 4982194;
        nibunnox3 = 1676509;
        break;
    case 3628 :
        nibunno3x = 4980350;
        nibunnox3 = 1672346;
        break;
    case 3629 :
        nibunno3x = 4978508;
        nibunnox3 = 1668188;
        break;
    case 3630 :
        nibunno3x = 4976668;
        nibunnox3 = 1664034;
        break;
    case 3631 :
        nibunno3x = 4974828;
        nibunnox3 = 1659881;
        break;
    case 3632 :
        nibunno3x = 4972987;
        nibunnox3 = 1655730;
        break;
    case 3633 :
        nibunno3x = 4971148;
        nibunnox3 = 1651584;
        break;
    case 3634 :
        nibunno3x = 4969312;
        nibunnox3 = 1647445;
        break;
    case 3635 :
        nibunno3x = 4967473;
        nibunnox3 = 1643300;
        break;
    case 3636 :
        nibunno3x = 4965637;
        nibunnox3 = 1639164;
        break;
    case 3637 :
        nibunno3x = 4963801;
        nibunnox3 = 1635028;
        break;
    case 3638 :
        nibunno3x = 4961966;
        nibunnox3 = 1630898;
        break;
    case 3639 :
        nibunno3x = 4960132;
        nibunnox3 = 1626767;
        break;
    case 3640 :
        nibunno3x = 4958299;
        nibunnox3 = 1622643;
        break;
    case 3641 :
        nibunno3x = 4956466;
        nibunnox3 = 1618519;
        break;
    case 3642 :
        nibunno3x = 4954633;
        nibunnox3 = 1614395;
        break;
    case 3643 :
        nibunno3x = 4952803;
        nibunnox3 = 1610281;
        break;
    case 3644 :
        nibunno3x = 4950973;
        nibunnox3 = 1606167;
        break;
    case 3645 :
        nibunno3x = 4949143;
        nibunnox3 = 1602053;
        break;
    case 3646 :
        nibunno3x = 4947313;
        nibunnox3 = 1597942;
        break;
    case 3647 :
        nibunno3x = 4945486;
        nibunnox3 = 1593837;
        break;
    case 3648 :
        nibunno3x = 4943658;
        nibunnox3 = 1589732;
        break;
    case 3649 :
        nibunno3x = 4941832;
        nibunnox3 = 1585633;
        break;
    case 3650 :
        nibunno3x = 4940005;
        nibunnox3 = 1581533;
        break;
    case 3651 :
        nibunno3x = 4938181;
        nibunnox3 = 1577441;
        break;
    case 3652 :
        nibunno3x = 4936356;
        nibunnox3 = 1573345;
        break;
    case 3653 :
        nibunno3x = 4934533;
        nibunnox3 = 1569259;
        break;
    case 3654 :
        nibunno3x = 4932709;
        nibunnox3 = 1565169;
        break;
    case 3655 :
        nibunno3x = 4930886;
        nibunnox3 = 1561085;
        break;
    case 3656 :
        nibunno3x = 4929066;
        nibunnox3 = 1557004;
        break;
    case 3657 :
        nibunno3x = 4927244;
        nibunnox3 = 1552925;
        break;
    case 3658 :
        nibunno3x = 4925425;
        nibunnox3 = 1548850;
        break;
    case 3659 :
        nibunno3x = 4923606;
        nibunnox3 = 1544776;
        break;
    case 3660 :
        nibunno3x = 4921788;
        nibunnox3 = 1540708;
        break;
    case 3661 :
        nibunno3x = 4919968;
        nibunnox3 = 1536636;
        break;
    case 3662 :
        nibunno3x = 4918153;
        nibunnox3 = 1532575;
        break;
    case 3663 :
        nibunno3x = 4916335;
        nibunnox3 = 1528510;
        break;
    case 3664 :
        nibunno3x = 4914520;
        nibunnox3 = 1524452;
        break;
    case 3665 :
        nibunno3x = 4912705;
        nibunnox3 = 1520395;
        break;
    case 3666 :
        nibunno3x = 4910893;
        nibunnox3 = 1516346;
        break;
    case 3667 :
        nibunno3x = 4909078;
        nibunnox3 = 1512292;
        break;
    case 3668 :
        nibunno3x = 4907266;
        nibunnox3 = 1508244;
        break;
    case 3669 :
        nibunno3x = 4905454;
        nibunnox3 = 1504199;
        break;
    case 3670 :
        nibunno3x = 4903642;
        nibunnox3 = 1500154;
        break;
    case 3671 :
        nibunno3x = 4901833;
        nibunnox3 = 1496118;
        break;
    case 3672 :
        nibunno3x = 4900022;
        nibunnox3 = 1492078;
        break;
    case 3673 :
        nibunno3x = 4898215;
        nibunnox3 = 1488047;
        break;
    case 3674 :
        nibunno3x = 4896406;
        nibunnox3 = 1484013;
        break;
    case 3675 :
        nibunno3x = 4894600;
        nibunnox3 = 1479987;
        break;
    case 3676 :
        nibunno3x = 4892792;
        nibunnox3 = 1475959;
        break;
    case 3677 :
        nibunno3x = 4890986;
        nibunnox3 = 1471936;
        break;
    case 3678 :
        nibunno3x = 4889182;
        nibunnox3 = 1467917;
        break;
    case 3679 :
        nibunno3x = 4887376;
        nibunnox3 = 1463895;
        break;
    case 3680 :
        nibunno3x = 4885573;
        nibunnox3 = 1459882;
        break;
    case 3681 :
        nibunno3x = 4883770;
        nibunnox3 = 1455869;
        break;
    case 3682 :
        nibunno3x = 4881967;
        nibunnox3 = 1451857;
        break;
    case 3683 :
        nibunno3x = 4880167;
        nibunnox3 = 1447854;
        break;
    case 3684 :
        nibunno3x = 4878366;
        nibunnox3 = 1443848;
        break;
    case 3685 :
        nibunno3x = 4876566;
        nibunnox3 = 1439846;
        break;
    case 3686 :
        nibunno3x = 4874767;
        nibunnox3 = 1435849;
        break;
    case 3687 :
        nibunno3x = 4872967;
        nibunnox3 = 1431850;
        break;
    case 3688 :
        nibunno3x = 4871170;
        nibunnox3 = 1427859;
        break;
    case 3689 :
        nibunno3x = 4869373;
        nibunnox3 = 1423868;
        break;
    case 3690 :
        nibunno3x = 4867576;
        nibunnox3 = 1419878;
        break;
    case 3691 :
        nibunno3x = 4865780;
        nibunnox3 = 1415894;
        break;
    case 3692 :
        nibunno3x = 4863985;
        nibunnox3 = 1411909;
        break;
    case 3693 :
        nibunno3x = 4862191;
        nibunnox3 = 1407930;
        break;
    case 3694 :
        nibunno3x = 4860397;
        nibunnox3 = 1403951;
        break;
    case 3695 :
        nibunno3x = 4858606;
        nibunnox3 = 1399981;
        break;
    case 3696 :
        nibunno3x = 4856814;
        nibunnox3 = 1396008;
        break;
    case 3697 :
        nibunno3x = 4855022;
        nibunnox3 = 1392039;
        break;
    case 3698 :
        nibunno3x = 4853232;
        nibunnox3 = 1388072;
        break;
    case 3699 :
        nibunno3x = 4851442;
        nibunnox3 = 1384108;
        break;
    case 3700 :
        nibunno3x = 4849652;
        nibunnox3 = 1380147;
        break;
    case 3701 :
        nibunno3x = 4847864;
        nibunnox3 = 1376189;
        break;
    case 3702 :
        nibunno3x = 4846078;
        nibunnox3 = 1372235;
        break;
    case 3703 :
        nibunno3x = 4844290;
        nibunnox3 = 1368280;
        break;
    case 3704 :
        nibunno3x = 4842504;
        nibunnox3 = 1364329;
        break;
    case 3705 :
        nibunno3x = 4840718;
        nibunnox3 = 1360382;
        break;
    case 3706 :
        nibunno3x = 4838935;
        nibunnox3 = 1356440;
        break;
    case 3707 :
        nibunno3x = 4837150;
        nibunnox3 = 1352495;
        break;
    case 3708 :
        nibunno3x = 4835366;
        nibunnox3 = 1348555;
        break;
    case 3709 :
        nibunno3x = 4833583;
        nibunnox3 = 1344616;
        break;
    case 3710 :
        nibunno3x = 4831801;
        nibunnox3 = 1340681;
        break;
    case 3711 :
        nibunno3x = 4830022;
        nibunnox3 = 1336754;
        break;
    case 3712 :
        nibunno3x = 4828240;
        nibunnox3 = 1332821;
        break;
    case 3713 :
        nibunno3x = 4826461;
        nibunnox3 = 1328896;
        break;
    case 3714 :
        nibunno3x = 4824682;
        nibunnox3 = 1324972;
        break;
    case 3715 :
        nibunno3x = 4822903;
        nibunnox3 = 1321049;
        break;
    case 3716 :
        nibunno3x = 4821127;
        nibunnox3 = 1317134;
        break;
    case 3717 :
        nibunno3x = 4819350;
        nibunnox3 = 1313216;
        break;
    case 3718 :
        nibunno3x = 4817572;
        nibunnox3 = 1309301;
        break;
    case 3719 :
        nibunno3x = 4815798;
        nibunnox3 = 1305391;
        break;
    case 3720 :
        nibunno3x = 4814023;
        nibunnox3 = 1301484;
        break;
    case 3721 :
        nibunno3x = 4812248;
        nibunnox3 = 1297576;
        break;
    case 3722 :
        nibunno3x = 4810477;
        nibunnox3 = 1293678;
        break;
    case 3723 :
        nibunno3x = 4808704;
        nibunnox3 = 1289777;
        break;
    case 3724 :
        nibunno3x = 4806931;
        nibunnox3 = 1285877;
        break;
    case 3725 :
        nibunno3x = 4805161;
        nibunnox3 = 1281984;
        break;
    case 3726 :
        nibunno3x = 4803390;
        nibunnox3 = 1278089;
        break;
    case 3727 :
        nibunno3x = 4801621;
        nibunnox3 = 1274202;
        break;
    case 3728 :
        nibunno3x = 4799851;
        nibunnox3 = 1270313;
        break;
    case 3729 :
        nibunno3x = 4798084;
        nibunnox3 = 1266431;
        break;
    case 3730 :
        nibunno3x = 4796316;
        nibunnox3 = 1262547;
        break;
    case 3731 :
        nibunno3x = 4794550;
        nibunnox3 = 1258670;
        break;
    case 3732 :
        nibunno3x = 4792783;
        nibunnox3 = 1254791;
        break;
    case 3733 :
        nibunno3x = 4791018;
        nibunnox3 = 1250917;
        break;
    case 3734 :
        nibunno3x = 4789252;
        nibunnox3 = 1247044;
        break;
    case 3735 :
        nibunno3x = 4787488;
        nibunnox3 = 1243175;
        break;
    case 3736 :
        nibunno3x = 4785727;
        nibunnox3 = 1239313;
        break;
    case 3737 :
        nibunno3x = 4783963;
        nibunnox3 = 1235447;
        break;
    case 3738 :
        nibunno3x = 4782200;
        nibunnox3 = 1231584;
        break;
    case 3739 :
        nibunno3x = 4780441;
        nibunnox3 = 1227728;
        break;
    case 3740 :
        nibunno3x = 4778680;
        nibunnox3 = 1223871;
        break;
    case 3741 :
        nibunno3x = 4776919;
        nibunnox3 = 1220015;
        break;
    case 3742 :
        nibunno3x = 4775161;
        nibunnox3 = 1216167;
        break;
    case 3743 :
        nibunno3x = 4773403;
        nibunnox3 = 1212319;
        break;
    case 3744 :
        nibunno3x = 4771645;
        nibunnox3 = 1208472;
        break;
    case 3745 :
        nibunno3x = 4769887;
        nibunnox3 = 1204627;
        break;
    case 3746 :
        nibunno3x = 4768132;
        nibunnox3 = 1200789;
        break;
    case 3747 :
        nibunno3x = 4766376;
        nibunnox3 = 1196949;
        break;
    case 3748 :
        nibunno3x = 4764622;
        nibunnox3 = 1193116;
        break;
    case 3749 :
        nibunno3x = 4762867;
        nibunnox3 = 1189281;
        break;
    case 3750 :
        nibunno3x = 4761114;
        nibunnox3 = 1185451;
        break;
    case 3751 :
        nibunno3x = 4759360;
        nibunnox3 = 1181622;
        break;
    case 3752 :
        nibunno3x = 4757608;
        nibunnox3 = 1177796;
        break;
    case 3753 :
        nibunno3x = 4755856;
        nibunnox3 = 1173971;
        break;
    case 3754 :
        nibunno3x = 4754107;
        nibunnox3 = 1170155;
        break;
    case 3755 :
        nibunno3x = 4752358;
        nibunnox3 = 1166339;
        break;
    case 3756 :
        nibunno3x = 4750609;
        nibunnox3 = 1162525;
        break;
    case 3757 :
        nibunno3x = 4748860;
        nibunnox3 = 1158711;
        break;
    case 3758 :
        nibunno3x = 4747111;
        nibunnox3 = 1154898;
        break;
    case 3759 :
        nibunno3x = 4745365;
        nibunnox3 = 1151093;
        break;
    case 3760 :
        nibunno3x = 4743618;
        nibunnox3 = 1147286;
        break;
    case 3761 :
        nibunno3x = 4741873;
        nibunnox3 = 1143487;
        break;
    case 3762 :
        nibunno3x = 4740127;
        nibunnox3 = 1139685;
        break;
    case 3763 :
        nibunno3x = 4738382;
        nibunnox3 = 1135887;
        break;
    case 3764 :
        nibunno3x = 4736640;
        nibunnox3 = 1132093;
        break;
    case 3765 :
        nibunno3x = 4734896;
        nibunnox3 = 1128300;
        break;
    case 3766 :
        nibunno3x = 4733155;
        nibunnox3 = 1124513;
        break;
    case 3767 :
        nibunno3x = 4731412;
        nibunnox3 = 1120721;
        break;
    case 3768 :
        nibunno3x = 4729672;
        nibunnox3 = 1116939;
        break;
    case 3769 :
        nibunno3x = 4727932;
        nibunnox3 = 1113157;
        break;
    case 3770 :
        nibunno3x = 4726192;
        nibunnox3 = 1109376;
        break;
    case 3771 :
        nibunno3x = 4724454;
        nibunnox3 = 1105599;
        break;
    case 3772 :
        nibunno3x = 4722715;
        nibunnox3 = 1101824;
        break;
    case 3773 :
        nibunno3x = 4720978;
        nibunnox3 = 1098052;
        break;
    case 3774 :
        nibunno3x = 4719241;
        nibunnox3 = 1094282;
        break;
    case 3775 :
        nibunno3x = 4717504;
        nibunnox3 = 1090512;
        break;
    case 3776 :
        nibunno3x = 4715770;
        nibunnox3 = 1086750;
        break;
    case 3777 :
        nibunno3x = 4714034;
        nibunnox3 = 1082986;
        break;
    case 3778 :
        nibunno3x = 4712302;
        nibunnox3 = 1079230;
        break;
    case 3779 :
        nibunno3x = 4710568;
        nibunnox3 = 1075471;
        break;
    case 3780 :
        nibunno3x = 4708836;
        nibunnox3 = 1071717;
        break;
    case 3781 :
        nibunno3x = 4707103;
        nibunnox3 = 1067962;
        break;
    case 3782 :
        nibunno3x = 4705372;
        nibunnox3 = 1064213;
        break;
    case 3783 :
        nibunno3x = 4703641;
        nibunnox3 = 1060465;
        break;
    case 3784 :
        nibunno3x = 4701913;
        nibunnox3 = 1056724;
        break;
    case 3785 :
        nibunno3x = 4700182;
        nibunnox3 = 1052978;
        break;
    case 3786 :
        nibunno3x = 4698454;
        nibunnox3 = 1049238;
        break;
    case 3787 :
        nibunno3x = 4696726;
        nibunnox3 = 1045500;
        break;
    case 3788 :
        nibunno3x = 4694998;
        nibunnox3 = 1041763;
        break;
    case 3789 :
        nibunno3x = 4693273;
        nibunnox3 = 1038034;
        break;
    case 3790 :
        nibunno3x = 4691546;
        nibunnox3 = 1034302;
        break;
    case 3791 :
        nibunno3x = 4689822;
        nibunnox3 = 1030574;
        break;
    case 3792 :
        nibunno3x = 4688098;
        nibunnox3 = 1026851;
        break;
    case 3793 :
        nibunno3x = 4686373;
        nibunnox3 = 1023126;
        break;
    case 3794 :
        nibunno3x = 4684651;
        nibunnox3 = 1019407;
        break;
    case 3795 :
        nibunno3x = 4682929;
        nibunnox3 = 1015691;
        break;
    case 3796 :
        nibunno3x = 4681207;
        nibunnox3 = 1011974;
        break;
    case 3797 :
        nibunno3x = 4679485;
        nibunnox3 = 1008259;
        break;
    case 3798 :
        nibunno3x = 4677766;
        nibunnox3 = 1004551;
        break;
    case 3799 :
        nibunno3x = 4676046;
        nibunnox3 = 1000841;
        break;
    case 3800 :
        nibunno3x = 4674328;
        nibunnox3 = 997138;
        break;
    case 3801 :
        nibunno3x = 4672609;
        nibunnox3 = 993434;
        break;
    case 3802 :
        nibunno3x = 4670892;
        nibunnox3 = 989733;
        break;
    case 3803 :
        nibunno3x = 4669174;
        nibunnox3 = 986034;
        break;
    case 3804 :
        nibunno3x = 4667458;
        nibunnox3 = 982338;
        break;
    case 3805 :
        nibunno3x = 4665742;
        nibunnox3 = 978643;
        break;
    case 3806 :
        nibunno3x = 4664029;
        nibunnox3 = 974956;
        break;
    case 3807 :
        nibunno3x = 4662313;
        nibunnox3 = 971264;
        break;
    case 3808 :
        nibunno3x = 4660600;
        nibunnox3 = 967579;
        break;
    case 3809 :
        nibunno3x = 4658887;
        nibunnox3 = 963894;
        break;
    case 3810 :
        nibunno3x = 4657174;
        nibunnox3 = 960211;
        break;
    case 3811 :
        nibunno3x = 4655464;
        nibunnox3 = 956536;
        break;
    case 3812 :
        nibunno3x = 4653752;
        nibunnox3 = 952858;
        break;
    case 3813 :
        nibunno3x = 4652044;
        nibunnox3 = 949187;
        break;
    case 3814 :
        nibunno3x = 4650334;
        nibunnox3 = 945514;
        break;
    case 3815 :
        nibunno3x = 4648624;
        nibunnox3 = 941842;
        break;
    case 3816 :
        nibunno3x = 4646917;
        nibunnox3 = 938178;
        break;
    case 3817 :
        nibunno3x = 4645208;
        nibunnox3 = 934511;
        break;
    case 3818 :
        nibunno3x = 4643503;
        nibunnox3 = 930852;
        break;
    case 3819 :
        nibunno3x = 4641796;
        nibunnox3 = 927190;
        break;
    case 3820 :
        nibunno3x = 4640090;
        nibunnox3 = 923533;
        break;
    case 3821 :
        nibunno3x = 4638385;
        nibunnox3 = 919876;
        break;
    case 3822 :
        nibunno3x = 4636681;
        nibunnox3 = 916223;
        break;
    case 3823 :
        nibunno3x = 4634977;
        nibunnox3 = 912573;
        break;
    case 3824 :
        nibunno3x = 4633276;
        nibunnox3 = 908928;
        break;
    case 3825 :
        nibunno3x = 4631574;
        nibunnox3 = 905282;
        break;
    case 3826 :
        nibunno3x = 4629872;
        nibunnox3 = 901640;
        break;
    case 3827 :
        nibunno3x = 4628172;
        nibunnox3 = 897999;
        break;
    case 3828 :
        nibunno3x = 4626472;
        nibunnox3 = 894362;
        break;
    case 3829 :
        nibunno3x = 4624771;
        nibunnox3 = 890723;
        break;
    case 3830 :
        nibunno3x = 4623073;
        nibunnox3 = 887091;
        break;
    case 3831 :
        nibunno3x = 4621375;
        nibunnox3 = 883460;
        break;
    case 3832 :
        nibunno3x = 4619677;
        nibunnox3 = 879830;
        break;
    case 3833 :
        nibunno3x = 4617980;
        nibunnox3 = 876204;
        break;
    case 3834 :
        nibunno3x = 4616284;
        nibunnox3 = 872579;
        break;
    case 3835 :
        nibunno3x = 4614589;
        nibunnox3 = 868959;
        break;
    case 3836 :
        nibunno3x = 4612894;
        nibunnox3 = 865339;
        break;
    case 3837 :
        nibunno3x = 4611199;
        nibunnox3 = 861720;
        break;
    case 3838 :
        nibunno3x = 4609507;
        nibunnox3 = 858109;
        break;
    case 3839 :
        nibunno3x = 4607815;
        nibunnox3 = 854498;
        break;
    case 3840 :
        nibunno3x = 4606123;
        nibunnox3 = 850888;
        break;
    case 3841 :
        nibunno3x = 4604431;
        nibunnox3 = 847280;
        break;
    case 3842 :
        nibunno3x = 4602739;
        nibunnox3 = 843672;
        break;
    case 3843 :
        nibunno3x = 4601050;
        nibunnox3 = 840071;
        break;
    case 3844 :
        nibunno3x = 4599361;
        nibunnox3 = 836472;
        break;
    case 3845 :
        nibunno3x = 4597672;
        nibunnox3 = 832873;
        break;
    case 3846 :
        nibunno3x = 4595983;
        nibunnox3 = 829276;
        break;
    case 3847 :
        nibunno3x = 4594294;
        nibunnox3 = 825680;
        break;
    case 3848 :
        nibunno3x = 4592608;
        nibunnox3 = 822090;
        break;
    case 3849 :
        nibunno3x = 4590922;
        nibunnox3 = 818501;
        break;
    case 3850 :
        nibunno3x = 4589236;
        nibunnox3 = 814915;
        break;
    case 3851 :
        nibunno3x = 4587553;
        nibunnox3 = 811334;
        break;
    case 3852 :
        nibunno3x = 4585867;
        nibunnox3 = 807748;
        break;
    case 3853 :
        nibunno3x = 4584184;
        nibunnox3 = 804171;
        break;
    case 3854 :
        nibunno3x = 4582501;
        nibunnox3 = 800593;
        break;
    case 3855 :
        nibunno3x = 4580818;
        nibunnox3 = 797016;
        break;
    case 3856 :
        nibunno3x = 4579138;
        nibunnox3 = 793447;
        break;
    case 3857 :
        nibunno3x = 4577456;
        nibunnox3 = 789876;
        break;
    case 3858 :
        nibunno3x = 4575776;
        nibunnox3 = 786309;
        break;
    case 3859 :
        nibunno3x = 4574098;
        nibunnox3 = 782745;
        break;
    case 3860 :
        nibunno3x = 4572418;
        nibunnox3 = 779180;
        break;
    case 3861 :
        nibunno3x = 4570740;
        nibunnox3 = 775619;
        break;
    case 3862 :
        nibunno3x = 4569061;
        nibunnox3 = 772058;
        break;
    case 3863 :
        nibunno3x = 4567384;
        nibunnox3 = 768502;
        break;
    case 3864 :
        nibunno3x = 4565707;
        nibunnox3 = 764946;
        break;
    case 3865 :
        nibunno3x = 4564033;
        nibunnox3 = 761399;
        break;
    case 3866 :
        nibunno3x = 4562358;
        nibunnox3 = 757848;
        break;
    case 3867 :
        nibunno3x = 4560682;
        nibunnox3 = 754299;
        break;
    case 3868 :
        nibunno3x = 4559008;
        nibunnox3 = 750753;
        break;
    case 3869 :
        nibunno3x = 4557336;
        nibunnox3 = 747213;
        break;
    case 3870 :
        nibunno3x = 4555663;
        nibunnox3 = 743671;
        break;
    case 3871 :
        nibunno3x = 4553992;
        nibunnox3 = 740136;
        break;
    case 3872 :
        nibunno3x = 4552321;
        nibunnox3 = 736600;
        break;
    case 3873 :
        nibunno3x = 4550650;
        nibunnox3 = 733066;
        break;
    case 3874 :
        nibunno3x = 4548979;
        nibunnox3 = 729533;
        break;
    case 3875 :
        nibunno3x = 4547311;
        nibunnox3 = 726006;
        break;
    case 3876 :
        nibunno3x = 4545642;
        nibunnox3 = 722478;
        break;
    case 3877 :
        nibunno3x = 4543975;
        nibunnox3 = 718956;
        break;
    case 3878 :
        nibunno3x = 4542307;
        nibunnox3 = 715432;
        break;
    case 3879 :
        nibunno3x = 4540640;
        nibunnox3 = 711913;
        break;
    case 3880 :
        nibunno3x = 4538974;
        nibunnox3 = 708395;
        break;
    case 3881 :
        nibunno3x = 4537309;
        nibunnox3 = 704880;
        break;
    case 3882 :
        nibunno3x = 4535644;
        nibunnox3 = 701367;
        break;
    case 3883 :
        nibunno3x = 4533979;
        nibunnox3 = 697854;
        break;
    case 3884 :
        nibunno3x = 4532317;
        nibunnox3 = 694348;
        break;
    case 3885 :
        nibunno3x = 4530654;
        nibunnox3 = 690841;
        break;
    case 3886 :
        nibunno3x = 4528992;
        nibunnox3 = 687337;
        break;
    case 3887 :
        nibunno3x = 4527330;
        nibunnox3 = 683835;
        break;
    case 3888 :
        nibunno3x = 4525669;
        nibunnox3 = 680336;
        break;
    case 3889 :
        nibunno3x = 4524008;
        nibunnox3 = 676838;
        break;
    case 3890 :
        nibunno3x = 4522348;
        nibunnox3 = 673342;
        break;
    case 3891 :
        nibunno3x = 4520689;
        nibunnox3 = 669848;
        break;
    case 3892 :
        nibunno3x = 4519032;
        nibunnox3 = 666360;
        break;
    case 3893 :
        nibunno3x = 4517374;
        nibunnox3 = 662872;
        break;
    case 3894 :
        nibunno3x = 4515716;
        nibunnox3 = 659384;
        break;
    case 3895 :
        nibunno3x = 4514060;
        nibunnox3 = 655902;
        break;
    case 3896 :
        nibunno3x = 4512406;
        nibunnox3 = 652423;
        break;
    case 3897 :
        nibunno3x = 4510750;
        nibunnox3 = 648942;
        break;
    case 3898 :
        nibunno3x = 4509094;
        nibunnox3 = 645462;
        break;
    case 3899 :
        nibunno3x = 4507441;
        nibunnox3 = 641989;
        break;
    case 3900 :
        nibunno3x = 4505788;
        nibunnox3 = 638516;
        break;
    case 3901 :
        nibunno3x = 4504135;
        nibunnox3 = 635045;
        break;
    case 3902 :
        nibunno3x = 4502484;
        nibunnox3 = 631578;
        break;
    case 3903 :
        nibunno3x = 4500832;
        nibunnox3 = 628112;
        break;
    case 3904 :
        nibunno3x = 4499180;
        nibunnox3 = 624647;
        break;
    case 3905 :
        nibunno3x = 4497532;
        nibunnox3 = 621188;
        break;
    case 3906 :
        nibunno3x = 4495882;
        nibunnox3 = 617728;
        break;
    case 3907 :
        nibunno3x = 4494232;
        nibunnox3 = 614268;
        break;
    case 3908 :
        nibunno3x = 4492585;
        nibunnox3 = 610816;
        break;
    case 3909 :
        nibunno3x = 4490938;
        nibunnox3 = 607364;
        break;
    case 3910 :
        nibunno3x = 4489291;
        nibunnox3 = 603913;
        break;
    case 3911 :
        nibunno3x = 4487644;
        nibunnox3 = 600463;
        break;
    case 3912 :
        nibunno3x = 4485998;
        nibunnox3 = 597018;
        break;
    case 3913 :
        nibunno3x = 4484353;
        nibunnox3 = 593572;
        break;
    case 3914 :
        nibunno3x = 4482709;
        nibunnox3 = 590132;
        break;
    case 3915 :
        nibunno3x = 4481065;
        nibunnox3 = 586692;
        break;
    case 3916 :
        nibunno3x = 4479424;
        nibunnox3 = 583259;
        break;
    case 3917 :
        nibunno3x = 4477780;
        nibunnox3 = 579821;
        break;
    case 3918 :
        nibunno3x = 4476139;
        nibunnox3 = 576389;
        break;
    case 3919 :
        nibunno3x = 4474496;
        nibunnox3 = 572956;
        break;
    case 3920 :
        nibunno3x = 4472857;
        nibunnox3 = 569530;
        break;
    case 3921 :
        nibunno3x = 4471216;
        nibunnox3 = 566102;
        break;
    case 3922 :
        nibunno3x = 4469576;
        nibunnox3 = 562677;
        break;
    case 3923 :
        nibunno3x = 4467938;
        nibunnox3 = 559256;
        break;
    case 3924 :
        nibunno3x = 4466300;
        nibunnox3 = 555837;
        break;
    case 3925 :
        nibunno3x = 4464662;
        nibunnox3 = 552419;
        break;
    case 3926 :
        nibunno3x = 4463026;
        nibunnox3 = 549004;
        break;
    case 3927 :
        nibunno3x = 4461390;
        nibunnox3 = 545589;
        break;
    case 3928 :
        nibunno3x = 4459753;
        nibunnox3 = 542176;
        break;
    case 3929 :
        nibunno3x = 4458118;
        nibunnox3 = 538767;
        break;
    case 3930 :
        nibunno3x = 4456486;
        nibunnox3 = 535366;
        break;
    case 3931 :
        nibunno3x = 4454851;
        nibunnox3 = 531959;
        break;
    case 3932 :
        nibunno3x = 4453218;
        nibunnox3 = 528555;
        break;
    case 3933 :
        nibunno3x = 4451586;
        nibunnox3 = 525156;
        break;
    case 3934 :
        nibunno3x = 4449954;
        nibunnox3 = 521758;
        break;
    case 3935 :
        nibunno3x = 4448323;
        nibunnox3 = 518363;
        break;
    case 3936 :
        nibunno3x = 4446691;
        nibunnox3 = 514967;
        break;
    case 3937 :
        nibunno3x = 4445062;
        nibunnox3 = 511576;
        break;
    case 3938 :
        nibunno3x = 4443433;
        nibunnox3 = 508188;
        break;
    case 3939 :
        nibunno3x = 4441804;
        nibunnox3 = 504800;
        break;
    case 3940 :
        nibunno3x = 4440175;
        nibunnox3 = 501413;
        break;
    case 3941 :
        nibunno3x = 4438548;
        nibunnox3 = 498031;
        break;
    case 3942 :
        nibunno3x = 4436922;
        nibunnox3 = 494652;
        break;
    case 3943 :
        nibunno3x = 4435294;
        nibunnox3 = 491270;
        break;
    case 3944 :
        nibunno3x = 4433670;
        nibunnox3 = 487896;
        break;
    case 3945 :
        nibunno3x = 4432045;
        nibunnox3 = 484523;
        break;
    case 3946 :
        nibunno3x = 4430419;
        nibunnox3 = 481147;
        break;
    case 3947 :
        nibunno3x = 4428796;
        nibunnox3 = 477778;
        break;
    case 3948 :
        nibunno3x = 4427173;
        nibunnox3 = 474410;
        break;
    case 3949 :
        nibunno3x = 4425550;
        nibunnox3 = 471043;
        break;
    case 3950 :
        nibunno3x = 4423927;
        nibunnox3 = 467678;
        break;
    case 3951 :
        nibunno3x = 4422307;
        nibunnox3 = 464318;
        break;
    case 3952 :
        nibunno3x = 4420687;
        nibunnox3 = 460960;
        break;
    case 3953 :
        nibunno3x = 4419067;
        nibunnox3 = 457604;
        break;
    case 3954 :
        nibunno3x = 4417447;
        nibunnox3 = 454247;
        break;
    case 3955 :
        nibunno3x = 4415827;
        nibunnox3 = 450892;
        break;
    case 3956 :
        nibunno3x = 4414210;
        nibunnox3 = 447543;
        break;
    case 3957 :
        nibunno3x = 4412592;
        nibunnox3 = 444193;
        break;
    case 3958 :
        nibunno3x = 4410976;
        nibunnox3 = 440848;
        break;
    case 3959 :
        nibunno3x = 4409359;
        nibunnox3 = 437503;
        break;
    case 3960 :
        nibunno3x = 4407742;
        nibunnox3 = 434158;
        break;
    case 3961 :
        nibunno3x = 4406128;
        nibunnox3 = 430820;
        break;
    case 3962 :
        nibunno3x = 4404512;
        nibunnox3 = 427479;
        break;
    case 3963 :
        nibunno3x = 4402898;
        nibunnox3 = 424143;
        break;
    case 3964 :
        nibunno3x = 4401286;
        nibunnox3 = 420811;
        break;
    case 3965 :
        nibunno3x = 4399672;
        nibunnox3 = 417477;
        break;
    case 3966 :
        nibunno3x = 4398061;
        nibunnox3 = 414149;
        break;
    case 3967 :
        nibunno3x = 4396450;
        nibunnox3 = 410822;
        break;
    case 3968 :
        nibunno3x = 4394839;
        nibunnox3 = 407496;
        break;
    case 3969 :
        nibunno3x = 4393228;
        nibunnox3 = 404171;
        break;
    case 3970 :
        nibunno3x = 4391617;
        nibunnox3 = 400847;
        break;
    case 3971 :
        nibunno3x = 4390009;
        nibunnox3 = 397529;
        break;
    case 3972 :
        nibunno3x = 4388401;
        nibunnox3 = 394213;
        break;
    case 3973 :
        nibunno3x = 4386793;
        nibunnox3 = 390897;
        break;
    case 3974 :
        nibunno3x = 4385185;
        nibunnox3 = 387582;
        break;
    case 3975 :
        nibunno3x = 4383580;
        nibunnox3 = 384275;
        break;
    case 3976 :
        nibunno3x = 4381974;
        nibunnox3 = 380965;
        break;
    case 3977 :
        nibunno3x = 4380367;
        nibunnox3 = 377656;
        break;
    case 3978 :
        nibunno3x = 4378762;
        nibunnox3 = 374351;
        break;
    case 3979 :
        nibunno3x = 4377157;
        nibunnox3 = 371046;
        break;
    case 3980 :
        nibunno3x = 4375555;
        nibunnox3 = 367749;
        break;
    case 3981 :
        nibunno3x = 4373952;
        nibunnox3 = 364449;
        break;
    case 3982 :
        nibunno3x = 4372350;
        nibunnox3 = 361154;
        break;
    case 3983 :
        nibunno3x = 4370748;
        nibunnox3 = 357858;
        break;
    case 3984 :
        nibunno3x = 4369147;
        nibunnox3 = 354568;
        break;
    case 3985 :
        nibunno3x = 4367545;
        nibunnox3 = 351274;
        break;
    case 3986 :
        nibunno3x = 4365946;
        nibunnox3 = 347988;
        break;
    case 3987 :
        nibunno3x = 4364346;
        nibunnox3 = 344700;
        break;
    case 3988 :
        nibunno3x = 4362748;
        nibunnox3 = 341418;
        break;
    case 3989 :
        nibunno3x = 4361149;
        nibunnox3 = 338134;
        break;
    case 3990 :
        nibunno3x = 4359552;
        nibunnox3 = 334855;
        break;
    case 3991 :
        nibunno3x = 4357954;
        nibunnox3 = 331575;
        break;
    case 3992 :
        nibunno3x = 4356358;
        nibunnox3 = 328300;
        break;
    case 3993 :
        nibunno3x = 4354762;
        nibunnox3 = 325026;
        break;
    case 3994 :
        nibunno3x = 4353166;
        nibunnox3 = 321752;
        break;
    case 3995 :
        nibunno3x = 4351573;
        nibunnox3 = 318485;
        break;
    case 3996 :
        nibunno3x = 4349978;
        nibunnox3 = 315217;
        break;
    case 3997 :
        nibunno3x = 4348384;
        nibunnox3 = 311949;
        break;
    case 3998 :
        nibunno3x = 4346791;
        nibunnox3 = 308685;
        break;
    case 3999 :
        nibunno3x = 4345201;
        nibunnox3 = 305427;
        break;
    case 4000 :
        nibunno3x = 4343608;
        nibunnox3 = 302165;
        break;
    case 4001 :
        nibunno3x = 4342016;
        nibunnox3 = 298907;
        break;
    case 4002 :
        nibunno3x = 4340426;
        nibunnox3 = 295652;
        break;
    case 4003 :
        nibunno3x = 4338838;
        nibunnox3 = 292401;
        break;
    case 4004 :
        nibunno3x = 4337248;
        nibunnox3 = 289148;
        break;
    case 4005 :
        nibunno3x = 4335658;
        nibunnox3 = 285896;
        break;
    case 4006 :
        nibunno3x = 4334071;
        nibunnox3 = 282650;
        break;
    case 4007 :
        nibunno3x = 4332484;
        nibunnox3 = 279406;
        break;
    case 4008 :
        nibunno3x = 4330897;
        nibunnox3 = 276163;
        break;
    case 4009 :
        nibunno3x = 4329310;
        nibunnox3 = 272920;
        break;
    case 4010 :
        nibunno3x = 4327724;
        nibunnox3 = 269680;
        break;
    case 4011 :
        nibunno3x = 4326139;
        nibunnox3 = 266442;
        break;
    case 4012 :
        nibunno3x = 4324555;
        nibunnox3 = 263208;
        break;
    case 4013 :
        nibunno3x = 4322971;
        nibunnox3 = 259974;
        break;
    case 4014 :
        nibunno3x = 4321387;
        nibunnox3 = 256742;
        break;
    case 4015 :
        nibunno3x = 4319806;
        nibunnox3 = 253516;
        break;
    case 4016 :
        nibunno3x = 4318222;
        nibunnox3 = 250285;
        break;
    case 4017 :
        nibunno3x = 4316641;
        nibunnox3 = 247061;
        break;
    case 4018 :
        nibunno3x = 4315060;
        nibunnox3 = 243838;
        break;
    case 4019 :
        nibunno3x = 4313479;
        nibunnox3 = 240614;
        break;
    case 4020 :
        nibunno3x = 4311901;
        nibunnox3 = 237399;
        break;
    case 4021 :
        nibunno3x = 4310322;
        nibunnox3 = 234181;
        break;
    case 4022 :
        nibunno3x = 4308744;
        nibunnox3 = 230967;
        break;
    case 4023 :
        nibunno3x = 4307164;
        nibunnox3 = 227751;
        break;
    case 4024 :
        nibunno3x = 4305588;
        nibunnox3 = 224541;
        break;
    case 4025 :
        nibunno3x = 4304011;
        nibunnox3 = 221333;
        break;
    case 4026 :
        nibunno3x = 4302434;
        nibunnox3 = 218124;
        break;
    case 4027 :
        nibunno3x = 4300858;
        nibunnox3 = 214918;
        break;
    case 4028 :
        nibunno3x = 4299283;
        nibunnox3 = 211715;
        break;
    case 4029 :
        nibunno3x = 4297708;
        nibunnox3 = 208513;
        break;
    case 4030 :
        nibunno3x = 4296136;
        nibunnox3 = 205316;
        break;
    case 4031 :
        nibunno3x = 4294562;
        nibunnox3 = 202119;
        break;
    case 4032 :
        nibunno3x = 4292989;
        nibunnox3 = 198922;
        break;
    case 4033 :
        nibunno3x = 4291417;
        nibunnox3 = 195729;
        break;
    case 4034 :
        nibunno3x = 4289845;
        nibunnox3 = 192537;
        break;
    case 4035 :
        nibunno3x = 4288276;
        nibunnox3 = 189351;
        break;
    case 4036 :
        nibunno3x = 4286704;
        nibunnox3 = 186160;
        break;
    case 4037 :
        nibunno3x = 4285135;
        nibunnox3 = 182976;
        break;
    case 4038 :
        nibunno3x = 4283566;
        nibunnox3 = 179793;
        break;
    case 4039 :
        nibunno3x = 4281997;
        nibunnox3 = 176611;
        break;
    case 4040 :
        nibunno3x = 4280428;
        nibunnox3 = 173430;
        break;
    case 4041 :
        nibunno3x = 4278862;
        nibunnox3 = 170255;
        break;
    case 4042 :
        nibunno3x = 4277294;
        nibunnox3 = 167078;
        break;
    case 4043 :
        nibunno3x = 4275727;
        nibunnox3 = 163902;
        break;
    case 4044 :
        nibunno3x = 4274161;
        nibunnox3 = 160730;
        break;
    case 4045 :
        nibunno3x = 4272598;
        nibunnox3 = 157565;
        break;
    case 4046 :
        nibunno3x = 4271032;
        nibunnox3 = 154394;
        break;
    case 4047 :
        nibunno3x = 4269469;
        nibunnox3 = 151229;
        break;
    case 4048 :
        nibunno3x = 4267904;
        nibunnox3 = 148064;
        break;
    case 4049 :
        nibunno3x = 4266343;
        nibunnox3 = 144904;
        break;
    case 4050 :
        nibunno3x = 4264780;
        nibunnox3 = 141743;
        break;
    case 4051 :
        nibunno3x = 4263218;
        nibunnox3 = 138586;
        break;
    case 4052 :
        nibunno3x = 4261657;
        nibunnox3 = 135428;
        break;
    case 4053 :
        nibunno3x = 4260097;
        nibunnox3 = 132275;
        break;
    case 4054 :
        nibunno3x = 4258537;
        nibunnox3 = 129123;
        break;
    case 4055 :
        nibunno3x = 4256977;
        nibunnox3 = 125971;
        break;
    case 4056 :
        nibunno3x = 4255418;
        nibunnox3 = 122824;
        break;
    case 4057 :
        nibunno3x = 4253860;
        nibunnox3 = 119677;
        break;
    case 4058 :
        nibunno3x = 4252303;
        nibunnox3 = 116533;
        break;
    case 4059 :
        nibunno3x = 4250746;
        nibunnox3 = 113391;
        break;
    case 4060 :
        nibunno3x = 4249189;
        nibunnox3 = 110250;
        break;
    case 4061 :
        nibunno3x = 4247634;
        nibunnox3 = 107112;
        break;
    case 4062 :
        nibunno3x = 4246078;
        nibunnox3 = 103975;
        break;
    case 4063 :
        nibunno3x = 4244524;
        nibunnox3 = 100842;
        break;
    case 4064 :
        nibunno3x = 4242970;
        nibunnox3 = 97708;
        break;
    case 4065 :
        nibunno3x = 4241416;
        nibunnox3 = 94578;
        break;
    case 4066 :
        nibunno3x = 4239862;
        nibunnox3 = 91446;
        break;
    case 4067 :
        nibunno3x = 4238310;
        nibunnox3 = 88320;
        break;
    case 4068 :
        nibunno3x = 4236758;
        nibunnox3 = 85195;
        break;
    case 4069 :
        nibunno3x = 4235206;
        nibunnox3 = 82071;
        break;
    case 4070 :
        nibunno3x = 4233656;
        nibunnox3 = 78951;
        break;
    case 4071 :
        nibunno3x = 4232107;
        nibunnox3 = 75834;
        break;
    case 4072 :
        nibunno3x = 4230556;
        nibunnox3 = 72713;
        break;
    case 4073 :
        nibunno3x = 4229006;
        nibunnox3 = 69597;
        break;
    case 4074 :
        nibunno3x = 4227458;
        nibunnox3 = 66484;
        break;
    case 4075 :
        nibunno3x = 4225912;
        nibunnox3 = 63375;
        break;
    case 4076 :
        nibunno3x = 4224364;
        nibunnox3 = 60264;
        break;
    case 4077 :
        nibunno3x = 4222816;
        nibunnox3 = 57153;
        break;
    case 4078 :
        nibunno3x = 4221271;
        nibunnox3 = 54050;
        break;
    case 4079 :
        nibunno3x = 4219726;
        nibunnox3 = 50947;
        break;
    case 4080 :
        nibunno3x = 4218181;
        nibunnox3 = 47844;
        break;
    case 4081 :
        nibunno3x = 4216636;
        nibunnox3 = 44743;
        break;
    case 4082 :
        nibunno3x = 4215091;
        nibunnox3 = 41643;
        break;
    case 4083 :
        nibunno3x = 4213549;
        nibunnox3 = 38549;
        break;
    case 4084 :
        nibunno3x = 4212006;
        nibunnox3 = 35453;
        break;
    case 4085 :
        nibunno3x = 4210464;
        nibunnox3 = 32361;
        break;
    case 4086 :
        nibunno3x = 4208923;
        nibunnox3 = 29272;
        break;
    case 4087 :
        nibunno3x = 4207381;
        nibunnox3 = 26181;
        break;
    case 4088 :
        nibunno3x = 4205840;
        nibunnox3 = 23094;
        break;
    case 4089 :
        nibunno3x = 4204300;
        nibunnox3 = 20008;
        break;
    case 4090 :
        nibunno3x = 4202761;
        nibunnox3 = 16926;
        break;
    case 4091 :
        nibunno3x = 4201222;
        nibunnox3 = 13844;
        break;
    case 4092 :
        nibunno3x = 4199683;
        nibunnox3 = 10763;
        break;
    case 4093 :
        nibunno3x = 4198147;
        nibunnox3 = 7689;
        break;
    case 4094 :
        nibunno3x = 4196610;
        nibunnox3 = 4612;
        break;
    case 4095 :
        nibunno3x = 4195072;
        nibunnox3 = 1536;
        break;
    }
    
  nibunno3x |= (1 << 23);
  nibunnox3 |= (1 << 23);
  
  /*
  
  printf("n3x   is   \n");
  lprint(nibunno3x);
  printf("nx3   is   \n");
  lprint(nibunnox3);
  
  */

  unsigned long int nibunnox3a,nibunnox3ak1,nibunnox3ak0;
  
  nibunnox3a = ((unsigned long int)ma) * ((unsigned long int)nibunnox3);
  nibunnox3ak1 = (get(e,0,0) == 1) ? (( ((1-(get(e,0,0)))<<11|get(m,22,12)) > 1202) ? (nibunnox3a >> 2) : nibunnox3a >> 1) : ( ( ((1-(get(e,0,0)))<<11|get(m,22,12)) > 2579) ? (nibunnox3a >> 2) : (nibunnox3a >> 1));
  nibunnox3ak0 = ( ((1-(get(e,0,0)))<<11|get(m,22,12)) > 2303) ? (nibunnox3ak1 << 1) : nibunnox3ak1;
  
  /*
  
  printf("n3a   is   \n");
  lprint(nibunnox3a);
  printf("n3ak1 is   \n");
  lprint(nibunnox3ak1);
  
  */
  
  unsigned int nibunnox3ak,rootabunno1;
  unsigned long int roota;
  
  nibunnox3ak = lget(nibunnox3ak0,47,24);
  rootabunno1 = nibunno3x - nibunnox3ak;
  roota = ((unsigned long int)ma) * ((unsigned long int)rootabunno1);
  
  unsigned long int keta;
  unsigned int ans;
  
  keta = (lget(roota,47,47) == 0) ?
               ((lget(roota,46,46) == 0) ?
               ((lget(roota,45,45) == 0) ? 
               ((lget(roota,44,44) == 0) ? (roota << 5) : (roota << 4)) : (roota << 3)) : (roota << 2)): (roota << 1);
  ans = lget(keta,47,25);
  y = (e == 0 || e == 255) ? (s<<31) : ((s<<31)|(ea<<23)|ans) ;
  
  /*
  
  printf("rabn1 is   ");
  print(rootabunno1);
  printf("noota is   \n");
  lprint(roota);
  printf("keta  is   \n");
  lprint(keta);
  printf("ans   is   ");
  print(ans);
  printf("y     is   ");
  print(y);
  
  */
  
  return y;
    
}


//個人的なテスト用

int main (){

  union Fbit{
    unsigned int iv;
    float fv;
  };

  union Fbit x,y,z;
  x.fv = 21;
  y.fv = 6;
  
  z.iv = fmul(x.iv,y.iv);
  printf("fmul(21,6) = %f\n",z.fv);
  
  z.iv = finv(y.iv);
  printf("finv(6) = %f\n",z.fv);
  
  z.iv = fdiv(x.iv,y.iv);
  printf("fdiv(21,6) = %f\n",z.fv);
  
  z.iv = fsqrt(y.iv);
  printf("fsqrt(6) = %f\n",z.fv);
  
  x.fv = -21;
  z.iv = fdiv(x.iv,y.iv);
  printf("fdiv(-21,6) = %f\n",z.fv);

}

