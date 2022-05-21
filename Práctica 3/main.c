#include <18F4550.h>
#device ADC=10
#fuses HS,NOWDT,NOPROTECT
#use delay(clock=20M)
#use rs232(baud=9600, xmit =PIN_C6, rcv=PIN_C7)

#define GLCD_CS1     PIN_C1   // Chip Selection 1
#define GLCD_CS2     PIN_C0   // Chip Selection 2
#define GLCD_DI      PIN_C2   // Data or Instruction input
#define GLCD_RW      PIN_C4   // Read/Write
#define GLCD_E       PIN_C5   // Enable
#define GLCD_RST     PIN_E0   // Reset  //SE CAMBI? POR QUE EN C6 EST? EL TX

#include <HDM64GS12.c>
#include <graphics.c>

#define RESOLUTION 4.883E-3 // 5/1024

float temp_conversion_factor(unsigned int16 D) {
    return ((D * RESOLUTION) / .01);
}

float pressure_conversion_factor(unsigned int16 D) {
    return (((D * RESOLUTION / 5) + 0.095) / 9E-3);
}

float rh_conversion_factor(unsigned int16 D) {
    return (((D * RESOLUTION / 5) - .1515) / .00636);
}


#INT_AD

void ad_isr() {
    continue;
}

typedef enum {
    TEMP, PRESSURE, RH
} T_sensors;
T_sensors sensor_to_read;

float sensor_values[3];
unsigned int16 sensor_v[3];

void main() {
    T_sensors sensor_to_read = TEMP;
    for (T_sensors s = TEMP; s <= RH; s++)
        sensor_values[s] = 0;

    setup_adc_ports(AN0_TO_AN2_ANALOG);
    setup_adc(ADC_CLOCK_DIV_32);

    //enable_interrupts(GLOBAL);
    //enable_interrupts(INT_AD);


    for (;;) {

        set_adc_channel(TEMP);
        delay_us(20);
        sensor_v[TEMP] = temp_conversion_factor(read_adc());
        if (adc_done()) {
            set_adc_channel(PRESSURE);
            delay_us(20);
            sensor_v[PRESSURE] = pressure_conversion_factor(read_adc());
            if (adc_done()) {
                set_adc_channel(RH);
                delay_us(20);
                sensor_v[RH] = rh_conversion_factor(read_adc());
            }
        }
    }
}