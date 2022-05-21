#include <18F4550.h>
#fuses HS,NOWDT,NOPROTECT,NOLVP
#use delay (clock=16000000)
#use standard_io(a)

#define KBD_COL1 PIN_B0
#define KBD_COL2 PIN_B1
#define KBD_COL3 PIN_B2
#define KBD_COL4 PIN_B3
#define KBD_ROW1 PIN_B4
#define KBD_ROW2 PIN_B5
#define KBD_ROW3 PIN_B6
#define KBD_ROW4 PIN_B7

#include <kbd4.c>
#include "pantalla.c"
#include <stdlib.h>
#include <string.h> 

typedef enum {IDLE, CONFIG, ACCEPTED, REJECTED}T_state;

T_state state = IDLE, follower_state = CONFIG;

unsigned char empleados[4][7] = {
    "000000",
    "000000",
    "000000",
    "000000"    
};

char master_key[7] = "AB01CD";

char password_text[] = "Password";
char config_text[] = "Config new password";
char changed_pass[]= "Password     changed";
char asterisk [2]= "*";
char buffer[7] = "";

void setup();
void loop();
void main();
void idle_state();
void config_state();
void accepted_state();
void rejected_state();

void main(){
    setup();
    for(;;){
        loop();
    }

}

void setup(){
    kbd_init();
    glcd_init(ON);
    setup_adc_ports(NO_ANALOGS);
    port_b_pullups(FAlSE);
    set_tris_a(0);
    set_tris_e(0b100);
    glcd_bitmap(LOGO);
    delay_ms(1000);  
    glcd_fillScreen(OFF);
    empleados[0] = "965027";
    empleados[1] = "105131";
    empleados[2] = "686443";
    empleados[3] = "151712";    
    
}

void loop(){
    switch(state){
        case IDLE:
            idle_state();
            break;
        case CONFIG:
            config_state();
            break;
        case ACCEPTED:
            accepted_state();
            break;
        case REJECTED:
            rejected_state();
            break;
    }
    return;
}

void accepted_state(){
    if(follower_state != state){
        glcd_bitmap(ACEPTADO);
        follower_state = ACCEPTED;        
    }
    output_high(PIN_E0);
    for(int i=0;i<10;i++){
        delay_ms(20);
        output_toggle(PIN_A0);
    }
    output_low(PIN_A0);  
    output_low(PIN_E0);          
    state = IDLE;
    delay_ms(800);
    return;
}
void rejected_state(){
    if(follower_state != state){
        glcd_bitmap(ALARMA);
        follower_state = REJECTED;        
    }
    output_high(PIN_E1);
    while(input(PIN_E2)){
        delay_ms(50);
        output_toggle(PIN_A0);      
    }
    output_low(PIN_A0);
    output_low(PIN_E1);
    state = IDLE;
    return;
}

void idle_state(){
    static unsigned char i;
    unsigned char c;
    c = kbd_getc();
    if(follower_state != state){
        i = 0;
        for(int k=0; k<7; k++)
            buffer[k] = "";
        glcd_fillScreen(OFF);        
        glcd_text57(20, 10, password_text, 2, ON);
        glcd_rect(14, 30, 114, 50, NO, ON);         
        follower_state = IDLE;
    }
    if(c){          
        if(i< 96){                                                      
            glcd_text57(20 + i, 32, asterisk, 2, ON);
            buffer[i/16] = c;
            i+=16;
        }else{
            if(!strcmp(buffer, master_key)){
                state = CONFIG;
            }else{
                for(int k=0;k<4;k++){
                    if(!strcmp(buffer, empleados[k]))
                        state = ACCEPTED;
                }
            }
            if(state==follower_state)
                state = REJECTED;
        }
    }
   return;
}


void config_state(){
    static unsigned char i;
    unsigned char c;
    c = kbd_getc();
    if(follower_state != state){
        i = 0;
        for(int k=0; k<7; k++)
            buffer[k] = "";
        glcd_fillScreen(OFF);
        glcd_bitmap(CONF);
        delay_ms(800);   
        glcd_fillScreen(OFF);   
        glcd_text57(0, 0, config_text, 1, ON);
        glcd_rect(14, 30, 114, 50, NO, ON);         
        follower_state = CONFIG;
    }
    if(c){         
            if(i< 96){                                                      
                glcd_text57(20 + i, 32, asterisk, 2, ON);
                buffer[i/16] = c;
                i+=16;
            }else{
                strcopy(master_key, buffer);
                glcd_fillScreen(OFF);
                glcd_text57(10, 10, changed_pass, 2, ON);
                delay_ms(800);
                state = IDLE;
        }
    }
    return;
}
