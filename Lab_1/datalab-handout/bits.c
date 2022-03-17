/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* Copyright (C) 1991-2018 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses Unicode 10.0.0.  Version 10.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2017, fifth edition, plus
   the following additions from Amendment 1 to the fifth edition:
   - 56 emoji characters
   - 285 hentaigana
   - 3 additional Zanabazar Square characters */
/* We do not support C11 <threads.h>.  */
         
 
         
/* 
 * bitOr - x|y using only ~ and & 
 *   Example: bitOr(6, 5) = 7
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */
int bitOr(int x, int y) {
  return ~(~x & ~y);
}



/* 
 * evenBits - return word with all even-numbered bits set to 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 1
 */
int evenBits(void) {
    int answer = 0x55;
    
    answer = answer + (answer << 8);
    answer = answer + (answer << 16);
    return answer;
}



/* 
 * minusOne - return a value of -1 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 1
 */
int minusOne(void) {
  return ~ 0x0;
}



/* 
 * allEvenBits - return 1 if all even-numbered bits in word set to 1
 *   Examples allEvenBits(0xFFFFFFFE) = 0, allEvenBits(0x55555555) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allEvenBits(int x) {
    
    int det_val = 0xAA;
    int result = x;
    det_val = det_val + (det_val << 8);
    det_val = det_val + (det_val << 16);
    
    result = ~ (result | det_val);
    return !result;
}



/* 
 * anyOddBit - return 1 if any odd-numbered bit in word set to 1
 *   Examples anyOddBit(0x5) = 0, anyOddBit(0x7) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int anyOddBit(int x) {
    int new_num = 0xAA;
    new_num = new_num + (new_num << 8);
    new_num = new_num + (new_num << 16);
    return !!(x & new_num);
}



/* 
 * byteSwap - swaps the nth byte and the mth byte
 *  Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *            byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *  You may assume that 0 <= n <= 3, 0 <= m <= 3
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 2
 */
int byteSwap(int x, int n, int m) {
    /* initialize varaibles */
    int bit_n;
    int bit_m;
        
    int num_n;
    int num_m;
    
    int other_bytes_n;
    int other_bytes_m;
    int other_bytes;
    
    /* convert m and n to bits instead of bytes */
    bit_n = n << 3;
    bit_m = m << 3;
    
    /* find the right place to make all 1's for the bytes needing to be swapped */
    num_n = (0xFF) << bit_n;
    other_bytes_n = ~num_n;
    num_n = (num_n & x);
    
    num_n = num_n >> bit_n;
    num_n = num_n & (0xFF);
    num_n = num_n << bit_m;
    
    num_m = (0xFF) << bit_m;
    other_bytes_m = ~num_m;
    num_m = (num_m & x);

    num_m = num_m >> bit_m;
    num_m = num_m & (0xFF);
    num_m = num_m << bit_n;
    
    /* find the right place to make all 1's for all other bytes not needing to be swapped */
    other_bytes = (other_bytes_n & other_bytes_m);
    other_bytes = other_bytes & x;
    
    /* put together num_n + num_m + other_bytes */
    // store the swaps together  
    return num_n | num_m | other_bytes;
}



/* 
 * addOK - Determine if can compute x+y without overflow
 *   Example: addOK(0x80000000,0x80000000) = 0,
 *            addOK(0x80000000,0x70000000) = 1, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int addOK(int x, int y) {
    /* Using this example to model function off of:
    Example: addOK(0x80000000,0x70000000) = 1 */
    
    /* initialize varaibles */
    int num;
    int xy_sig_fig;
    int x_xor_num_sig_fig;
    
    int xy_xor;
    int x_xor_num;    
    
    /* get the sum of x and y
    Two overflow cases signed numbers:
    1. + overflow -> if and only if x > 0 and y > 0, s<= 0
    2. - overflow -> if and only if x < 0 and y < 0, s >= 0 */
    num = x + y;
    
    /* check the signs of x and y, and num with either x or y
    two's-complement means first significant digit will be 
    - if 1 in the first place
    + if 0 in the first place */ 
    xy_xor = x ^ y;
    x_xor_num = x ^ num;
    
    /* shift bits down by 31-bits to preserve the first bit value of a 1 or a 0 */
    xy_sig_fig = xy_xor >> 31;
    x_xor_num_sig_fig = x_xor_num >> 31;
   
    /* ~xy_sig_fig, only want same sign values i.e. x < 0 & y < 0 OR x > 0 & y > 0 
    ~xy_sig_fig = 1111 1111 1111 1111 1111 1111 1111 1111  */
    xy_sig_fig = ~xy_sig_fig;
    
    /* now compare the significant digits of ~(x ^ y) >> 31 against x + y >> 31
    Ex: x = 0x80000000 y = 0x70000000, 
    ~xy_sig_fig = 1111 1111 1111 1111 1111 1111 1111 1111 
    num_sig_fig=  1111 1111 1111 1111 1111 1111 1111 1111 */
    num = xy_sig_fig & x_xor_num_sig_fig;
    
    /* return num banged to get the correct result back */
    return !num;
}



/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
    /* x = 2, y = 4, z = 5
    initialize variables */
    int x_value;
    int xy_value;
    int xz_value;
    int final_val;
    
    /* first take x and double bang to get 1 or 0
    this tells us if x was number other 0 to begin with */
    x_value = !!x;              // 0x1 0001
    x_value = ~x_value;         // 0xE 1110
    x_value = x_value + 0x1;    // 0xF 1111
    
    /* mask out one of the values depending on if x was true or false */
    xy_value = x_value & y;     // 1111 & 0100 = 0100
    x_value = ~x_value;         // 0000
    xz_value = x_value & z;     // 0000 & 0101 = 0000
    
    final_val = xy_value | xz_value; // 0100
    return final_val;
}



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
      
    big_omega = big_omega + (~x + 1);
    big_oh = x + (~big_oh + 1);
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



/* 
 * replaceByte(x,n,c) - Replace byte n in x with c
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: replaceByte(0x12345678,1,0xab) = 0x1234ab78
 *   You can assume 0 <= n <= 3 and 0 <= c <= 255
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 3
 */
int replaceByte(int x, int n, int c) {
    /* initialize variables */
    int bit_n;
    int num_n;
    int bit_c;
    
    bit_n = n << 3;
    num_n = (0xFF) << bit_n;
    num_n = ~num_n;
    num_n = num_n & x;

    bit_c = c << bit_n;
    num_n = bit_c + num_n;
    return num_n;
}



/* reverseBits - reverse the bits in a 32-bit integer,
              i.e. b0 swaps with b31, b1 with b30, etc
 *  Examples: reverseBits(0x11111111) = 0x88888888
 *            reverseBits(0xdeadbeef) = 0xf77db57b
 *            reverseBits(0x88888888) = 0x11111111
 *            reverseBits(0)  = 0
 *            reverseBits(-1) = -1
 *            reverseBits(0x9) = 0x90000000
 *  Legal ops: ! ~ & ^ | + << >> and unsigned int type
 *  Max ops: 90
 *  Rating: 4
 */
int reverseBits(int x) {
    unsigned int val_1;
    unsigned int val_2;
    
    int const_1 = 0x55555555;
    int const_2 = 0xAAAAAAAA;
    
    int const_3 = 0x33333333;
    int const_4 = 0xCCCCCCCC;
    
    int const_5 = 0x0F0F0F0F;
    int const_6 = 0xF0F0F0F0;
        
    int const_7 = 0x00FF00FF;
    int const_8 = 0xFF00FF00;
    
    int const_9 = 0x0000FFFF;
    int const_10 = 0xFFFF0000;
        
    val_1 = x & const_1;
    val_2 = x & const_2;
    val_1 = val_1 << 1;
    val_2 = val_2  >> 1;
    x = val_1 | val_2;
    
    
    val_1 = x & const_3;
    val_2 = x & const_4;
    val_1 = val_1 << 2;
    val_2 = val_2  >> 2;
    x = val_1 | val_2;
    
    
    val_1 = x & const_5;
    val_2 = x & const_6;
    val_1 = val_1 << 4;
    val_2 = val_2  >> 4;
    x = val_1 | val_2;
    
        
    val_1 = x & const_7;
    val_2 = x & const_8;
    val_1 = val_1 << 8;
    val_2 = val_2  >> 8;
    x = val_1 | val_2;
    
            
    val_1 = x & const_9;
    val_2 = x & const_10;
    val_1 = val_1 << 16;
    val_2 = val_2  >> 16;
    x = val_1 | val_2;
    
    return x;
}



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
    
    int t_min = 0x1;
    int t_max;
    
    int y_sig_fig;
    int result;
    
    // get the sum of x and y
    // Two overflow cases signed numbers:
    // 1. + overflow -> if and only if x > 0 and y > 0, s<= 0
    // 2. - overflow -> if and only if x < 0 and y < 0, s >= 0
    num = x + y;
    
    // check the signs of x and y, and num with either x or y
    // two's-complement means first significant digit will be 
    // - if 1 in the first place
    // + if 0 in the first place
    xy_xor = x ^ y;
    x_xor_num = x ^ num;

    // shift bits down by 31-bits to preserve the first bit value of a 1 or a 0
    xy_sig_fig = xy_xor >> 31;
    x_xor_num_sig_fig = x_xor_num >> 31;
        
    // ~xy_sig_fig, only want same sign values i.e. x < 0 & y < 0 OR x > 0 & y > 0 
    // ~xy_sig_fig = 1111 1111 1111 1111 1111 1111 1111 1111 
    xy_sig_fig = ~xy_sig_fig;
    
    // now compare the significant digits of ~(x ^ y) >> 31 against x + y >> 31
    // Ex: x = 0x80000000 y = 0x70000000, 
    // ~xy_sig_fig = 1111 1111 1111 1111 1111 1111 1111 1111 
    // num_sig_fig = 1111 1111 1111 1111 1111 1111 1111 1111 
    // gives all 1's back if overflow occurred
    ovr_flw = xy_sig_fig & x_xor_num_sig_fig;
    
    
    
    // find t_min and t_max 
    t_min = ovr_flw << 31;
    t_max = t_min ^ ovr_flw;
    
    // find positive overflow and negative overflow
    // + overflow if sign of x OR y are negative AND overflow happened
    // - overflow if sign of x OR y are positive AND overflow happened
    y_sig_fig = y >> 31;
    num_sig_fig = num >> 31;
    result = (t_min ^ num_sig_fig & ovr_flw) | ~ovr_flw & num;
    return result;
}





/*
 * Extra credit
 */

/* 
 * float_abs - Return bit-level equivalent of absolute value of f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument..
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_abs(unsigned uf) {
    int t_min = 0x1;
    int t_max;
    
    t_min = t_min << 31;
    t_max = ~t_min;
    return uf & 0x7fffffff;
}



/* 
 * float_f2i - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int float_f2i(unsigned uf) {
  return 2;
}



/* 
 * float_half - Return bit-level equivalent of expression 0.5*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_half(unsigned uf) {
  return 2;
}
