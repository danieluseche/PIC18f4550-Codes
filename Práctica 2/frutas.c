#include "frutas.h"

void dibujar_tanques(){

    char fresa [] = "Frs.";
    char mango [] = "Man.";
    char pera [] = "Per.";
    char manzana [] = "Mnz.";

    glcd_rect(5,13,30,55,NO,ON);
    glcd_rect(37,13,62,55,NO,ON);
    glcd_rect(69,13,94,55,NO,ON);
    glcd_rect(101,13,126,55,NO,ON);

    glcd_text57(5,56,fresa,1,ON);
    glcd_text57(37,56,mango,1,ON);
    glcd_text57(69,56,pera,1,ON);
    glcd_text57(101,56,manzana,1,ON);
}

float map(float valor, float entradaMin, float entradaMax, float salidaMin, float salidaMax){                                
    return ((((valor-entradaMin)*(salidaMax-salidaMin))/(entradaMax-entradaMin))+salidaMin); 
}

void llenar_tanques(int8 cont_fresa, int8 cont_mango, int8 cont_pera, int8 cont_manzana){

    glcd_rect(5, map(cont_fresa,0,MAX_FRESAS, 55, 13),30,55,YES,ON);
    glcd_rect(37,map(cont_mango,0,MAX_MANGOS,55,13),62,55,YES,ON);
    glcd_rect(69,map(cont_pera,0,MAX_PERAS,55,13),94,55,YES,ON);
    glcd_rect(101,map(cont_manzana,0,MAX_MANZANAS,55,13),126,55,YES,ON);
    
    if(cont_fresa == MAX_FRESAS)     glcd_text57(5,5,full_str,1,ON);
    if(cont_mango == MAX_MANGOS)     glcd_text57(37,5,full_str,1,ON);
    if(cont_pera == MAX_PERAS)       glcd_text57(69,5,full_str,1,ON);
    if(cont_manzana == MAX_MANZANAS) glcd_text57(101,5,full_str,1,ON);
}

//////////////////////////////FUNCIONES DE LA CLASE///////////////////////////////

void fruta_init() {
    for (N_FRUTA tipo_fruta = FRESAS; tipo_fruta <= MANZANAS; tipo_fruta++) {
        frutas[tipo_fruta].cont = 0;
        frutas[tipo_fruta].enable_flag = 0;
        frutas[tipo_fruta].full = 0;
        frutas[tipo_fruta].reset = 0;
    }
}

void fruta_count() {

    if (frutas[FRESAS].enable_flag) {
        if (frutas[FRESAS].cont++ >= MAX_FRESAS) {
            frutas[FRESAS].full = 1;
            frutas[FRESAS].cont = MAX_FRESAS;
            disable_interrupts(INT_EXT);
        }
        frutas[FRESAS].led_turnoff_timer = get_ticks() + LED_TIME;
        output_high(PIN_A0);
        frutas[FRESAS].enable_flag = 0;
    }

    if (frutas[MANGOS].enable_flag) {
        if (frutas[MANGOS].cont++ >= MAX_MANGOS) {
            frutas[MANGOS].full = 1;
            frutas[MANGOS].cont = MAX_MANGOS;
        }
        frutas[MANGOS].led_turnoff_timer = get_ticks() + LED_TIME;
        output_high(PIN_A1);
        frutas[MANGOS].enable_flag = 0;
    }

    if (frutas[PERAS].enable_flag) {
        if (frutas[PERAS].cont++ >= MAX_PERAS) {
            frutas[PERAS].full = 1;
            frutas[PERAS].cont = MAX_PERAS;
        }
        frutas[PERAS].led_turnoff_timer = get_ticks() + LED_TIME;
        output_high(PIN_A2);
        frutas[PERAS].enable_flag = 0;
    }

    if (frutas[MANZANAS].enable_flag) {
        if (frutas[MANZANAS].cont++ >= MAX_MANZANAS) {
            frutas[MANZANAS].full = 1;
            frutas[MANZANAS].cont = MAX_MANZANAS;
            disable_interrupts(INT_EXT2);
        }
        frutas[MANZANAS].led_turnoff_timer = get_ticks() + LED_TIME;
        output_high(PIN_A3);
        frutas[MANZANAS].enable_flag = 0;
    }
    if (frutas[PERAS].full && frutas[MANGOS].full) {
        disable_interrupts(INT_EXT1);
    }
}

void fruta_reset() {
    if (frutas[FRESAS].reset) {
        frutas[FRESAS].reset = 0;
        frutas[FRESAS].full = 0;
        frutas[FRESAS].cont = 0;
        glcd_rect(6, 14, 29, 54, ON, OFF);
        glcd_text57(5, 5, full_str, 1, OFF);
        enable_interrupts(INT_EXT);
    }
    if (frutas[MANGOS].reset) {
        frutas[MANGOS].reset = 0;
        frutas[MANGOS].full = 0;
        frutas[MANGOS].cont = 0;
        glcd_rect(38, 14, 61, 54, ON, OFF);
        glcd_text57(37, 5, full_str, 1, OFF);
        enable_interrupts(INT_EXT1);
    }
    if (frutas[PERAS].reset) {
        frutas[PERAS].reset = 0;
        frutas[PERAS].full = 0;
        frutas[PERAS].cont = 0;
        glcd_rect(70, 14, 93, 54, ON, OFF);
        glcd_text57(69, 5, full_str, 1, OFF);
        enable_interrupts(INT_EXT1);
    }

    if (frutas[MANZANAS].reset) {
        frutas[MANZANAS].reset = 0;
        frutas[MANZANAS].full = 0;
        frutas[MANZANAS].cont = 0;
        glcd_rect(102, 14, 125, 54, ON, OFF);
        glcd_text57(101, 5, full_str, 1, OFF);
        enable_interrupts(INT_EXT2);
    }
}