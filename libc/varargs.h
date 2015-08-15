/*
 *     File Name: varargs.h
 *     Description:  handle variable argument list  
 *     Author: iczelion
 *     Email: qomolangmaice@163.com 
 *     Created: 2015年08月15日 星期六 21时44分27秒
 */

#ifndef INCLUDE_VARARGS_H_ 
#define INCLUDE_VARARGS_H_ 

/* The type va_list is defined for variables used to traverse the list. */
typedef __builtin_va_list va_list; 

/* The va_start() macro is invoked to initialise ap 
 * to the beginning of the list before any calls to va_arg(). 
 */
#define va_start(ap, last) 	 	(__builtin_va_start(ap, last)) 

/* The va_arg() macro will return the next argument 
 * in the list pointed to by ap. */
#define va_arg(ap, type) 	 	(__builtin_va_arg(ap, type)) 

/* The va_end() macro is used to clean up. */
#define va_end(ap) 	 	

#endif   

