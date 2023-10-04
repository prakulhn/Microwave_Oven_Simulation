/*
 * File:   main.h
 * Author: DELL
 *
 * Created on September 20, 2023, 10:58 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#include <xc.h>
#include "clcd.h"
#include "matrix_keypad.h"
#include "timers.h"

#define POWER_ON_SCREEN        0x01
#define MENU_DISPLAY_SCREEN    0x02
#define MICRO_MODE             0x03
#define GRILL_MODE             0x04
#define CONVECTION_MODE        0x05
#define START_MODE             0x06
#define TIME_DISPLAY           0x07
#define START                  0x08
#define PAUSE                  0x09
#define STOP                   0x0A

#define RESET_NOTHING  0x0F
#define MODE_RESET     0x10
#define RESET_TEMP     0x11
//#define GRILL_MODE_RESET;

#define FAN         RC2
#define FAN_DDR     TRISC2
#define BUZZER      RC1
#define BUZZER_DDR  TRISC1
#define DOOR        RB3
#define DOOR_DDR    TRISB3

#define OFF         0
#define ON          1

void clear_screen(void);
void power_on_screen(void);
void menu_display_screen(void);
void set_time(unsigned char key,int reset_flag);
void time_display_screen(void);
void init_timer2(void);
void set_temp(unsigned char key,int reset_flag);

#endif	/* MAIN_H */

