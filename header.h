/* 
 * File:   header.h
 * Author: KAMAL
 *
 * Created on October 22, 2016, 6:02 PM
 */

#ifndef HEADER_H
#define	HEADER_H

#ifdef	__cplusplus
extern "C" {
#endif
#ifdef	__cplusplus
}
#endif
#endif	/* HEADER_H */

#include<xc.h>
#include<libpic30.h>
#include<stdio.h>
#include <math.h>

void delay(unsigned int);

void InitPWM(void);
void duty(unsigned int);
