#include <18F4550.h>
#fuses HS,NOWDT,NOPROTECT,NOLVP
#use delay (clock=16000000)
#use timer(tick=1us, bits=32, NOISR)
#include "frutas.c"
#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////
// como usar el timer para contar los 50ms de cada led
//#include <18F4550.h>
//#use delay (clock=16000000)
//#use timer(tick=1ms, bits=32, NOISR) cada tick es un ms
//unsigned int16 end_tick;
//int16 diff_tick;
//
//void main(void) {
//
//    end_tick = 0;
//    diff_tick = 0;
//    int1 flag = 0;
//    while (TRUE) {
//
//        if (!input(PIN_E0) && !flag) {
//            end_tick = get_ticks() + 51;
//            output_high(PIN_B0);
//            flag = 1;


//            diff_tick = end_tick - get_ticks();
//
//            if (diff_tick <=1 ) {
//                output_low(PIN_B0);
//                flag = 0;
//            }
//        }
//    }
//}
////////////////////////////////////////////////////////////////////////////////

#DEVICE HIGH_INTS=TRUE

// Prototipo de funciones
void setup();
void loop();
void main();

signed int32 tick;

#INT_EXT HIGH

void ext_isr() {
    frutas[FRESAS].enable_flag = 1;
}

#INT_EXT1 HIGH

void ext1_isr() {
    if (input(PIN_E0) == 1 && !frutas[MANGOS].full) { // Cayo un mango
        frutas[MANGOS].enable_flag = 1;
    }
    if (input(PIN_E1) == 1 && !frutas[PERAS].full) { // Cayo una pera
        frutas[PERAS].enable_flag = 1;
    }
}
#INT_EXT2

void ext2_isr() { // Cayo una manzana
    frutas[MANZANAS].enable_flag = 1;
}

#INT_RB

void rb_isr() {
    if (input(PIN_B4) == 0 && frutas[FRESAS].full) { // Reinicialize la banda de fresas
        frutas[FRESAS].reset = 1;
    }
    if (input(PIN_B5) == 0 && frutas[MANGOS].full) { // Reinicialize la banda de mangos
        frutas[MANGOS].reset = 1;
    }
    if (input(PIN_B6) == 0 && frutas[PERAS].full) { // Reinicialize la banda de  peras
        frutas[PERAS].reset = 1;
    }
    if (input(PIN_B7) == 0 && frutas[MANZANAS].full) { // Reinicialize la banda de  manzanas
        frutas[MANZANAS].reset = 1;
    }
}

void main() {
    setup();
    for (;;) {
        loop();
    }
}

void setup() {
    glcd_init(ON);
    dibujar_tanques();

    setup_adc_ports(NO_ANALOGS);
    port_b_pullups(FAlSE);
    set_tris_a(0x00);
    set_tris_e(0x07); // Puerto E como entrada
    set_tris_b(0xFF); //Puerto B como salida

    enable_interrupts(GLOBAL);
    enable_interrupts(INT_EXT);
    enable_interrupts(INT_EXT1);
    enable_interrupts(INT_EXT2);
    enable_interrupts(INT_RB);

    ext_int_edge(0, L_TO_H);
    ext_int_edge(1, L_TO_H);
    ext_int_edge(2, L_TO_H);

    fruta_init();
}

void loop() {

    fruta_count();
    fruta_reset();
    
    tick = get_ticks();
    if(frutas[FRESAS].led_turnoff_timer - tick <=1)   
        output_low(PIN_A0);
    if(frutas[MANGOS].led_turnoff_timer - tick <=1)   
        output_low(PIN_A1);
    if(frutas[PERAS].led_turnoff_timer - tick <=1)    
        output_low(PIN_A2);
    if(frutas[MANZANAS].led_turnoff_timer - tick <=1) 
        output_low(PIN_A3);
    
    llenar_tanques(frutas[FRESAS].cont, frutas[MANGOS].cont, frutas[PERAS].cont, frutas[MANZANAS].cont);
    delay_ms(20);
    return;
}
