#include <cstdio>

/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
    int big_omega = 0x2F;
    int big_oh = 0x3A;
    int bounded;
      
    big_omega = big_omega + (~x);
    big_oh = x + (~big_oh);
    
    /* big_omega = ~(x + ~big_omega);
    big_oh = ~(~x + big_oh); */
    bounded = big_omega & big_oh;
    bounded = bounded >> 31;
    
    return !!(bounded);
    /* x - y = ~(~x + y) */
    
    /* subtract by doing ~(x + 0x2F) // one less than 0x30)
    0x58
    
    /* lower bound =>  ~(~x + 0x2F) // 0x2F is one less than 0x30
    upper bound => ~(~x + 0x40)    // take 0x58 and subtract x
    
    !! */
}


int main(){
    int num;
    num = isAsciiDigit(0x35);
}
