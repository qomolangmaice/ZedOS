/*
 *     File Name: printf.h
 *     Description: header file for printf.c  
 *     Author: iczelion
 *     Email: qomolangmaice@163.com 
 *     Created: 2015年08月16日 星期日 00时34分31秒
 */

#ifndef INCLUDE_PRINTF_H_ 
#define INCLUDE_PRINTF_H_ 
#include "varargs.h" 
#include "types.h" 

#define assert(x, info)                                         \
    do {                                                    \
        if (!(x)) {                                     \
            panic(info);                    \
        }                                               \
    } while (0)

// 编译期间静态检测
#define static_assert(x)                                    \
    switch (x) { case 0: case (x): ; }

/* kernel print function */
void printf(const char *format, ...); 

/* kernel print function  with color*/
void printf_with_color(uint8 font_color,const char *format, ...);  

#endif 

