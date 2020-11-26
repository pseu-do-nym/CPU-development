#include <stdio.h>
#include <math.h>

union Fbit{
    int iv;
    float fv;
};

int print_b(int x){
    
    for(int i=22;i>=0;i--) printf("%d",(x >> i) & 1);
    printf(";\n");
    
}

int main(void){
    
    union Fbit v1,v2,ans;
    printf("case({e[0],m[10:0]})\n");
    
    for(int i=0;i<4096;i++){
        
        printf("12'd%d : tab = ",i);
        v1.iv = (127 << 23) + (i << 12);
        v2.iv = v1.iv + 0b111111111111;
        ans.fv = 3.0*((1/(sqrtf(v1.fv)))+(1/(sqrtf(v2.fv))))/4.0;
        printf("23'b");
        print_b(ans.iv);
        
    }
     
     printf("endcase\n");
     
}
