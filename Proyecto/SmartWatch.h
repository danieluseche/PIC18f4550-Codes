typedef enum
{
    CRONO,
    ALARM,
    TIMER,
    CLOCK,
    CONFIG
} T_mode;

typedef enum
{
    UP_BUTTON,
    DOWN_BUTTON,
    CONFIG_BUTTON,
    NO_BUTTON_PRESSED
} T_buttons;

typedef struct
{
    int8 year;
    int8 month;
    int8 day;
    int8 dow;
    int8 hour;
    int8 min;
    int8 sec;
} T_rtc_time;

struct T_SmartWatch
{
    /*************VARIABLES DE ESTADO******************/
    T_mode mode;              //En que modo se encuentra actualmente
    T_mode previous_mode;     //Cual fue el modo anterior (se usa para evitar actualizaciones de pantalla)
    T_buttons button_pressed; //Cual boton ha sido presionado
    int1 mode_set;            //Indica si se ha oprimido el boton de seleccion en algún modo
    int1 read_clock;          //Indica si debe leerse el reloj (Evita el polling)

    /*************Variables de configuracion**************/
    int1 setting_year;
    int1 setting_month;
    int1 setting_day;
    int1 setting_hour;
    int1 setting_min;
    int1 setting_sec;


    /*************Variables de Reloj *********************/
    T_rtc_time date_time; //La estructura que guarda los valores actuales del RTC

    /*************Variables de Cronómetro ****************/
    int1 active_crono;                    //Indica si el cronometro está contando
    T_rtc_time init_time_crono;     //tiempo en el que empezó la cuenta
    T_rtc_time end_time_crono;      //tiempo en el que finalizó la cuenta
    unsigned int32 time_diff_crono; //diferencia de tiempo a mostrar

    /*************Variables de Temporizador ****************/
    int1 active_timer;
    int1 setting_timer;
    int1 idle_timer;
    int1 timer_done;
    T_rtc_time init_time_timer;     //tiempo en el que empezó la cuenta
    T_rtc_time end_time_timer;      //tiempo en el que se desea finalizar la cuenta
    unsigned int32 time_diff_timer; //diferencia de tiempo a mostrar
    /** NOTA:
    * La unica diferencia entre el temporizador y el cronómetro
    * es que el temporizador tiene un tiempo de finalizacion fijo,
    * es decir, el tiempo al que quiere llegarse, por el contrario,
    * el tiempo final de un cronómetro está definido por la pulsacion
    * del boton de seleccion, en este caso particular CONFIG_BUTTON.
    * para ambos casos la solucion del tiempo a mostrar es la misma:
    * tiempo = final - inicio
    * para trabajar todo en la misma unidad
    * final = 3600*final.hora + 60*final.min + final.sec
    */

    /*************Variables de ALARMA ********************/
    int1 active_alarm;
    int1 setting_alarm;
    int1 alarm_triggered;
    T_rtc_time alarm;
    /** NOTA:
     * La alarma es identica al funcionamiento del timer con la unica excepcion que no hay que mostrarlo de forma decremental.
     * 
    */

} SmartWatch;

/*****************Funcionalidades de Módulo **************/
void SmartWatch_setup();    //Inicializa el modulo SmartWatch
void SmartWatch_read_rtc(); //Lee los valores del RTC

/****************Funcionalidades de Seleccion de Modo ****/
void SmartWatch_alarm();
void SmartWatch_crono();
void SmartWatch_timer();
void SmartWatch_clock();
void SmartWatch_config();

/****************Funcionalidades de chequeo constante ****/
void SmartWatch_check_alarm();
void SmartWatch_check_timer();

unsigned int32 SmartWatch_time_to_sec(T_rtc_time);

void SmartWatch_get_buttons();