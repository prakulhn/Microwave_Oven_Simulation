/*
 * File:   main.c
 */


#include "main.h"

#pragma config WDTE = OFF

unsigned char sec = 0, min = 0, flag = 0;
int operation_flag = POWER_ON_SCREEN;
static void init_config(void) {

    init_clcd();


    init_matrix_keypad();

    //RC2 pin as output
    FAN_DDR = 0;
    FAN = OFF;

   BUZZER_DDR = 0; //RC1 AS OUTPUT
   BUZZER = OFF;

     //initialization
   init_timer2();

    PEIE = 1;
    GIE = 1;
}

void main(void) {
    init_config();

  //variable declaration

    unsigned char key;
    int reset_flag;
    while (1) {
        key = read_matrix_keypad(STATE);
        if(operation_flag == MENU_DISPLAY_SCREEN)
        {

        if(key == 1)
        {
            operation_flag = MICRO_MODE;
            reset_flag = MODE_RESET ;
            clear_screen();
            clcd_print(" power = 900w ", LINE2(0));
            __delay_ms(3000);
            clear_screen();
        }
        else if(key == 2)
        {
            operation_flag = GRILL_MODE;
            reset_flag = MODE_RESET;
            clear_screen();

        }
        else if(key == 3)
        {
            operation_flag = CONVECTION_MODE;
            reset_flag = MODE_RESET;
            clear_screen();

        }
        else if(key == 4)
        {
            sec = 30;
            min = 0;
            FAN = ON;
             TMR2ON = ON;
             clear_screen();
             operation_flag = TIME_DISPLAY;
        }
       }
       else if(operation_flag == TIME_DISPLAY)
       {
            if(key == 4)
            {
               sec = sec + 30;//sec > 59 , sec == 60 -> min
               if(sec > 59)
               {
                   min++;
                   sec = sec - 60;
               }
            }
            else if(key == 5)
            {
                operation_flag = PAUSE;
            }
            else if(key == 6)
            {
                operation_flag = STOP;
                clear_screen();

            }
        }
        else if(operation_flag == PAUSE)
        {
           if(key == 4)
           {
               FAN = ON;
               TMR2ON = ON;
               operation_flag = TIME_DISPLAY;
           }
        }


       switch(operation_flag)
       {
           case POWER_ON_SCREEN:
               power_on_screen();
               operation_flag = MENU_DISPLAY_SCREEN ;
               clear_screen();
                 break;
           case MENU_DISPLAY_SCREEN:
               menu_display_screen();
               break;
           case GRILL_MODE:
               set_time(key, reset_flag);
               break;
           case MICRO_MODE:
               set_time(key, reset_flag);
               break;
           case CONVECTION_MODE:
               if(flag == 0)
               {
                   set_temp(key, reset_flag);
                   if(flag == 1)//# pressed
                   {
                       clear_screen();
                      reset_flag = MODE_RESET;
                      continue;
                   }
               }
               else if(flag == 1)
               {
                   set_time(key, reset_flag);
               }

               break;
           case TIME_DISPLAY:
               time_display_screen();
               break;
           case PAUSE:
               FAN = OFF;
               TMR2ON = OFF;
               break;
           case STOP:
               FAN = OFF;
               TMR2ON = OFF;
               operation_flag = MENU_DISPLAY_SCREEN ;
               break;
        }
       reset_flag = RESET_NOTHING;

    }
}

void time_display_screen(void)
{
    //line 1 display
    clcd_print(" TIME = ", LINE1(0));
    //print min & sec
     clcd_putch( min/10 + '0', LINE1(9));
    clcd_putch( min%10 + '0', LINE1(10));
    clcd_putch(':', LINE1(11));

    //sec
    clcd_putch( sec/10 + '0', LINE1(12));
    clcd_putch( sec%10 + '0', LINE1(13));

 //print
    clcd_print(" 4.Start/Resume", LINE2(0));
    clcd_print(" 5.Pause", LINE3(0));
    clcd_print(" 6.Stop", LINE4(0));

    if(sec == 0 && min == 0)
    {
        clear_screen();
        clcd_print(" Time Up !!", LINE2(0));
        BUZZER = ON;
        __delay_ms(3000); //sec
        clear_screen();
        BUZZER = OFF;
        FAN = OFF;

        TMR2ON = OFF;
        operation_flag = MENU_DISPLAY_SCREEN ;

    }
}
void clear_screen(void)
{
    clcd_write(CLEAR_DISP_SCREEN, INST_MODE) ;
    __delay_us(500) ;
}
void power_on_screen(void)
{
    unsigned char i;

   for (i = 0; i < 16; i++)
   {

       clcd_putch(BAR, LINE1(i));
       __delay_ms(100);

   }


  clcd_print(" Powering ON ", LINE2(0)) ;
  clcd_print(" Microwave Oven ", LINE3(0));


  for (i = 0; i < 16; i++)
   {
      clcd_putch(BAR, LINE4(i));
        __delay_ms(100);
  }


  __delay_ms(3000);

}



void menu_display_screen(void)
{
 clcd_print ("1.Micro", LINE1(0));
 clcd_print ("2.Grill", LINE2(0));
 clcd_print("3.Convection", LINE3(0));
 clcd_print ("4.Start", LINE4(0));
}

void set_temp(unsigned char key,int reset_flag)
{
 static unsigned char key_count, blink, temp;
    static int wait;
    if( reset_flag == MODE_RESET )
    {
        key_count = 0;
        wait = 0;
        blink = 0;
        flag = 0;
        temp =0;
        key = ALL_RELEASED;
        clcd_print("SET TEMP (oC)", LINE1(0));
        clcd_print("TEMP = ", LINE2(0));
    //sec 0 to 59
        clcd_print("*:CLEAR #:ENTER", LINE4(0));
         }

    //Reading the temp
    if ((key != '*') && (key != '#') && (key != ALL_RELEASED))
    {
    //key = 1 5 4
        key_count++;

        if(key_count <= 3)
        {
            temp = temp*10 + key;

        }

    }
    else if(key == '*')// sec or min
    {
        temp =0;
        key_count = 0;
    }
    else if(key == '#')//enter key
    {
        clear_screen();
        //pre-Heating
        clcd_print(" Pre-Heating ", LINE1(0));
        clcd_print("Time Rem.= ",LINE3(0));
        TMR2ON = 1;
        sec = 18;
        while(sec != 0)// sec == 0
        {

            //temp
   // clcd_putch((sec/100) + '0', LINE3(11));//123/100 -> 1
    clcd_putch((sec/10)%10 + '0', LINE3(12));//123/10
    clcd_putch((sec%10) + '0', LINE3(13));// 10 - 3
    }

        //sec = 0
        if(sec == 0)
        {
            flag = 1;
            TMR2ON = 0;
            //set time screen
            //operation_flag = GRILL_MODE;


        }


     }
     if(wait++ == 15)//0 1 ...15
    {
      wait = 0;
      blink = !blink;
     //printing temp
      //temp
    clcd_putch((temp/100) + '0', LINE2(8));//123/100 -> 1
    clcd_putch((temp/10)%10 + '0', LINE2(9));//123/10
    clcd_putch((temp%10) + '0', LINE2(10));// 10 - 3
    }

     if(blink)
     {
     clcd_print(" ", LINE2(8));



    }

 }
void set_time(unsigned char key, int reset_flag )
{
    static unsigned char key_count, blink_pos, blink;
    static int wait;
    if( reset_flag == MODE_RESET )
    {
       key_count = 0;
       sec = 0;
       min = 0;
       blink_pos = 0;
       wait = 0;
       blink = 0;
       key = ALL_RELEASED;
    clcd_print("SET TIME (MM:SS)", LINE1(0));
    clcd_print("TIME- ", LINE2(0));
    //sec 0 to 59
    clcd_print("*:CLEAR #:ENTER", LINE4(0));

    }

    if ((key != '*') && (key != '#') && (key != ALL_RELEASED))
    {
    //key = 1 5 4
        key_count++;

        if(key_count <= 2)
        {
            sec = sec*10 + key;
            blink_pos = 0;

        }
        else if(key_count > 2 && key_count <= 4 )
        {
            min = min*10 + key;
            blink_pos = 1;
        }

    }
    else if(key == '*')// sec or min
    {
      if( blink_pos == 0)
      {
          sec = 0;
          key_count = 0;
      }
      else if( blink_pos == 1)
      {
        min = 0;
        key_count = 2;
      }
    }
    else if(key == '#')
    {
        clear_screen();
        operation_flag = TIME_DISPLAY;
        FAN = ON; // on
        /*SWITCHING OM TIMER 2*/
        TMR2ON = ON;
    }

    if(wait++ == 15)//0 1 ...15
    {
      wait = 0;
      blink = !blink;
     //printing min and sec
    //min
    clcd_putch( min/10 + '0', LINE2(6));
    clcd_putch( min%10 + '0', LINE2(7));
    clcd_putch(':', LINE2(8));

    //sec
    clcd_putch( sec/10 + '0', LINE2(9));
    clcd_putch( sec%10 + '0', LINE2(10));
    }

     if(blink)
     {
         switch(blink_pos)
         {
             case 0: //sec
                 clcd_print(" ", LINE2(9));
                break;
             case 1: //min
                 clcd_print(" ", LINE2(6));
                 break;
         }


     }

}
