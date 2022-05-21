#define GLCD_CS1     PIN_C1   // Chip Selection 1
#define GLCD_CS2     PIN_C0   // Chip Selection 2
#define GLCD_DI      PIN_C2   // Data or Instruction input
#define GLCD_RW      PIN_C4   // Read/Write
#define GLCD_E       PIN_C5   // Enable
#define GLCD_RST     PIN_C6   // Reset

#include <HDM64GS12.c>
#include <graphics.c>

#define MAX_FRESAS 10
#define MAX_MANGOS 20
#define MAX_PERAS 15
#define MAX_MANZANAS 5

#define LED_TIME 50

typedef struct {
    int8 cont;
    int1 enable_flag;
    int1 full;
    int1 reset;
    signed int32 led_turnoff_timer;

} T_FRUTA;

typedef enum {
    FRESAS, MANGOS, PERAS, MANZANAS
} N_FRUTA;

void fruta_init();
void fruta_count();
void fruta_reset();

void dibujar_tanques(void);
void llenar_tanques(int8 cont_fresa, int8 cont_mango, int8 cont_pera, int8 cont_manzana);
double map(float valor, float entradaMin, float entradaMax, float salidaMin, float salidaMax);

T_FRUTA frutas[4];
char full_str[] = "FULL"; 