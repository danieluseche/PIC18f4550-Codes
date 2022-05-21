#define GLCD_CS1     PIN_C1   // Chip Selection 1
#define GLCD_CS2     PIN_C0   // Chip Selection 2
#define GLCD_DI      PIN_C2   // Data or Instruction input
#define GLCD_RW      PIN_C4   // Read/Write
#define GLCD_E       PIN_C5   // Enable
#define GLCD_RST     PIN_C6   // Reset
//#define FAST_GLCD

#include <HDM64GS12.c>
#include <graphics.c>

#include "bitmap.h"
enum bitmap{LOGO, ALARMA, ACEPTADO, CONF};

void glcd_bitmap(char bitmap){
    unsigned int8 i, j,m=64;
    unsigned long page = 0;

    // Loop a traves de paginas verticales 
    // ref:https://exploreembedded.com/wiki/Interfacing_GLCD(128x64)_with_PIC16F877A
    for(i = 0; i < 8; ++i){
        output_low(GLCD_DI);                      // Modo comando
        glcd_writeByte(GLCD_LEFT, 0b01000000);    // 0x40 hasta 0x7F (posicion del cursor por pagina 0-63)
        glcd_writeByte(GLCD_RIGHT, 0b01000000);
        glcd_writeByte(GLCD_LEFT, i | 0b10111000);// Escoje la direccion de la pagina
        glcd_writeByte(GLCD_RIGHT, i | 0b10111000);
        output_high(GLCD_DI);                     // Modo datos
        switch(bitmap){
            case LOGO:
                for(j=0; j<64; j++){
                        glcd_writeByte(GLCD_LEFT, seguridad_bmp[j+page]);
                        glcd_writeByte(GLCD_RIGHT, seguridad_bmp[j+page+m]);
                }
            break;         
            case ALARMA:
                for(j=0; j<64; j++){
                        glcd_writeByte(GLCD_LEFT, alarma_bmp[j+page]);                    
                        glcd_writeByte(GLCD_RIGHT, alarma_bmp[j+page+m]);
                }
            break;               
            case ACEPTADO:
                for(j=0; j<64; j++){
                        glcd_writeByte(GLCD_LEFT, aceptado_bmp[j+page]);                    
                        glcd_writeByte(GLCD_RIGHT, aceptado_bmp[j+page+m]);
                }
            break;                                                        
            case CONF:
                for(j=0; j<64; j++){
                        glcd_writeByte(GLCD_LEFT, conf_bmp[j+page]);                    
                        glcd_writeByte(GLCD_RIGHT, conf_bmp[j+page+m]);
                }
            break;         
        }
        page += 128;         
    }
}

