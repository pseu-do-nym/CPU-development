#include <stdio.h>

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
    printf("case(m[10:0])\n");
    
    for(int i=0;i<2048;i++){
        
        printf("11'd%d : tab2 = ",i);
        v1.iv = (1 << 30) + (i << 12);
        v2.iv = v1.iv + 0b111111111111;
        ans.fv = ((1/v1.fv)+(1/v2.fv))/2;
        ans.fv *= ans.fv;
        printf("23'b");
        print_b(ans.iv & 0b11111111111111111111111);
        
    }
     
     printf("endcase\n");
     
}
