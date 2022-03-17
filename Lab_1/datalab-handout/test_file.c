#include <cstdio>
/*
 * satAdd - adds two numbers but when positive overflow occurs, returns
 *          maximum possible value (Tmax), and when negative overflow occurs,
 *          it returns minimum negative value (Tmin)
 *   Examples: satAdd(0x40000000,0x40000000) = 0x7fffffff
 *             satAdd(0x80000000,0xffffffff) = 0x80000000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 30
 *   Rating: 4
 */
int satAdd(int x, int y) {
    // Using this example to model function off of:
    // Example: addOK(0x80000000,0x70000000) = 1
    
    // initialize varaibles
    int num;
    int num_sig_fig;
    int xy_sig_fig;
    int x_xor_num_sig_fig;
    
    int xy_xor;
    int x_xor_num; 
    int ovr_flw;
    
    int t_min;
    int t_max;
    
    int y_sig_fig;
    int num_not_ovr_flw;
    int num_sig_fig_t_min;
    
    int result;
    
    // get the sum of x and y
    // Two overflow cases signed numbers:
    // 1. + overflow -> if and only if x > 0 and y > 0, s<= 0
    // 2. - overflow -> if and only if x < 0 and y < 0, s >= 0
    num = x + y;
    printf("num : %x \n", num);
    
    // check the signs of x and y, and num with either x or y
    // two's-complement means first significant digit will be 
    // - if 1 in the first place
    // + if 0 in the first place
    xy_xor = x ^ y;
    printf("xy_xor : %x \n", xy_xor);

    x_xor_num = x ^ num;
    printf("x_xor_num : %x \n", x_xor_num);

    // shift bits down by 31-bits to preserve the first bit value of a 1 or a 0
    xy_sig_fig = xy_xor >> 31;
    printf("xy_sig_fig : %x \n", xy_sig_fig);
    
    x_xor_num_sig_fig = x_xor_num >> 31;
    printf("x_xor_num_sig_fig : %x \n", x_xor_num_sig_fig);
    
    num_sig_fig = num >> 31;
    printf("num : %x \n", num);
    
    // ~xy_sig_fig, only want same sign values i.e. x < 0 & y < 0 OR x > 0 & y > 0 
    // ~xy_sig_fig = 1111 1111 1111 1111 1111 1111 1111 1111 
    xy_sig_fig = ~xy_sig_fig;
    printf("xy_sig_fig : %x \n", xy_sig_fig);
    
    // now compare the significant digits of ~(x ^ y) >> 31 against x + y >> 31
    // Ex: x = 0x80000000 y = 0x70000000, 
    // ~xy_sig_fig = 1111 1111 1111 1111 1111 1111 1111 1111 
    // num_sig_fig = 1111 1111 1111 1111 1111 1111 1111 1111 
    // gives all 1's back if overflow occurred
    ovr_flw = xy_sig_fig & x_xor_num_sig_fig;
    printf("ovr_flw : %x \n", ovr_flw);
    
    t_min = ovr_flw << 31;
    printf("t_min : %x \n", t_min);

    t_max = t_min ^ ovr_flw;
    printf("t_max : %x \n", t_max);
    
    y_sig_fig = y >> 31;
    printf("y_sig_fig : %x \n", y_sig_fig);
    
    // put it all together now
    // check for positive or negative overflow
    // if neither return the sum of x + y
    // use t_min and t_max to get right value
    num_not_ovr_flw = ~ovr_flw & num;
    printf("num_not_ovr_flw : %x \n", num_not_ovr_flw);

    num_sig_fig_t_min = t_min ^ num_sig_fig;
    printf("num_sig_fig_t_min : %x \n", num_sig_fig_t_min);

    result = num_not_ovr_flw | (ovr_flw & num_sig_fig_t_min);
    printf("result : %x \n", result);

    return result;
}



int main(){
    int num;
    num = satAdd(0x30000000, 0x7FFFFFFF);
}

