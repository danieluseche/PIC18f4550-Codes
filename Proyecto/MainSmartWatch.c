#include <18F4550.h>
#fuses HS, NOWDT, NOPROTECT, NOLVP
#use delay(xtal = 20000000)
#use rs232(baud = 9600, xmit = PIN_C6, rcv = PIN_C7)

#define GLCD_CS1 PIN_C1 // Chip Selection 1
#define GLCD_CS2 PIN_C0 // Chip Selection 2
#define GLCD_DI PIN_C2  // Data or Instruction input
#define GLCD_RW PIN_C4  // Read/Write
#define GLCD_E PIN_C5   // Enable
#define GLCD_RST PIN_E0 // Reset
//#define FAST_GLCD

#include <HDM64GS12.c>
#include <graphics.c>
#include "SmartWatch.c"
#use i2c(master, sda = PIN_B0, scl = PIN_B1, force_hw)

void setup();
void loop();

#INT_EXT2
void ext2_ISR()
{
    SmartWatch.button_pressed = CONFIG_BUTTON;
}

#INT_CCP1
void cpp1_ISR()
{
    SmartWatch.read_clock = TRUE;
}

void main()
{
    setup();
    for (;;)
    {
        loop();
    }
}

void setup()
{
    glcd_init(ON);
    rtc_init();

    setup_timer_1(T1_INTERNAL | T1_DIV_BY_8);
    setup_ccp1(CCP_COMPARE_INT);
    ccp_1 = 62500; //disparo cada 100ms
    set_timer1(0);

    enable_interrupts(INT_CCP1);
    enable_interrupts(INT_EXT2);
    enable_interrupts(GLOBAL);
    SmartWatch_setup();
    //Imprimir los elementos fijos en pantalla
    return;
}

void loop()
{
    if (SmartWatch.read_clock)
    {
        //este bloque se ejecutará cada 100ms mas lo que tarde un ciclo de programa
        SmartWatch_read_rtc();
        SmartWatch_check_alarm();
        if (SmartWatch.active_timer)
            SmartWatch_check_timer();
        //ACÁ ACTUALIZAR EL VALOR DEL RELOJ EN PANTALLA
        SmartWatch.read_clock = FALSE;
    }

    switch (SmartWatch.mode)
    {
    case ALARM:
        SmartWatch_alarm();
        break;
    case CRONO:
        SmartWatch_crono();
        break;
    case TIMER:
        SmartWatch_timer();
        break;
    case CLOCK:
        SmartWatch_clock();
        break;
    case CONFIG:
        SmartWatch_config();
        break;
    }
    return;
}