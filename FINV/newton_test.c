#include <stdio.h>
#include <math.h>

union Fbit{
    int iv;
    float fv;
};

int print_b(int x, int n){
    
    for(int i=n;i>=0;i--) printf("%d",(x >> i) & 1);
    printf(";\n");
    
}

int main (){

    union Fbit a, b, x, x2, xa, xi;
    
    while(1){
    
      printf("\ninput : ");
      scanf("%f",&x.fv);
      
      printf("x = ");
      print_b(x.iv,31);
      
      a = x;
      x.iv = (x.iv & 0b11111111111000000000000) | (1 << 30);
      x2.iv = x.iv | 0b111111111111;
      print_b(x.iv,31);
      print_b(x2.iv,31);
      xa.fv = ((1/x.fv)+(1/x2.fv))/2;
      
      printf("int(%d)!! %d\n",x.iv,xa.iv);     
      
      printf("table x = ");
      print_b(xa.iv,31);
      
      b = xa;
      xa.fv *= xa.fv;
      xa.iv &= 0b11111111111111111111111;
      
      printf("table x^2 = ");
      print_b(xa.iv,22);
      
      xa.iv *= (a.iv & 0b11111111111111111111111);
      xa.fv = 2 * b.fv - xa.fv;
      
      printf("\nans = ");
      print_b(xa.iv,22);

    }

}
