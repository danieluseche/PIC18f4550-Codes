#include "SmartWatch.h"
#include <ds1338.c>

void SmartWatch_setup()
{
    SmartWatch.mode = CLOCK;
    SmartWatch.previous_mode = CONFIG;
    SmartWatch.mode_set = FALSE;
    SmartWatch.button_pressed = NO_BUTTON_PRESSED;
    SmartWatch.read_clock = FALSE;
    SmartWatch.active_crono = FALSE;

    //seting timer:
    SmartWatch.timer_done = FALSE;
    SmartWatch.active_timer = FALSE;
    SmartWatch.idle_timer = FALSE;
    SmartWatch.setting_timer = FALSE;
    SmartWatch.end_time_timer.hour = 0;
    SmartWatch.end_time_timer.min = 0;
    SmartWatch.end_time_timer.sec = 0;

    SmartWatch.setting_year = FALSE;
    SmartWatch.setting_month = FALSE;
    SmartWatch.setting_day = FALSE;
    SmartWatch.setting_hour = FALSE;
    SmartWatch.setting_min = FALSE;
    SmartWatch.setting_sec = FALSE;
}

void SmartWatch_read_rtc()
{
    rtc_get_date(SmartWatch.date_time.day, SmartWatch.date_time.month, SmartWatch.date_time.year, SmartWatch.date_time.dow);
    rtc_get_time(SmartWatch.date_time.hour, SmartWatch.date_time.min, SmartWatch.date_time.sec);
    return;
}

void SmartWatch_alarm()
{
    SmartWatch_get_buttons();
    if(SmartWatch.active_alarm == TRUE && SmartWatch.alarm_triggered==True)
    {
        //ALARM TASKS
        continue;
    }
    if (SmartWatch.mode_set) //Si el modo fué seleccionado:
    {
        if (SmartWatch.active_alarm == FALSE) // configure el tiempo de parada:
        {
            if (SmartWatch.setting_alarm == FALSE)
            {
                SmartWatch.setting_hour = TRUE;
                SmartWatch.setting_alarm = TRUE;
            }
            switch (SmartWatch.button_pressed)
            {
            case UP_BUTTON:
                if (SmartWatch.setting_hour)
                    SmartWatch.alarm.hour = (SmartWatch.alarm.hour < 23) ? SmartWatch.alarm.hour + 1 : 0;
                if (SmartWatch.setting_min)
                    SmartWatch.alarm.min = (SmartWatch.alarm.min < 59) ? SmartWatch.alarm.min + 1 : 0;
                if (SmartWatch.setting_sec)
                    SmartWatch.alarm.sec = (SmartWatch.alarm.sec < 59) ? SmartWatch.alarm.sec + 1 : 0;

                break;
            case DOWN_BUTTON:
                if (SmartWatch.setting_hour)
                    SmartWatch.alarm.hour = (SmartWatch.alarm.hour > 0) ? SmartWatch.alarm.hour - 1 : 23;
                if (SmartWatch.setting_min)
                    SmartWatch.alarm.min = (SmartWatch.alarm.min > 0) ? SmartWatch.alarm.min - 1 : 59;
                if (SmartWatch.setting_sec)
                    SmartWatch.alarm.sec = (SmartWatch.alarm.sec > 0) ? SmartWatch.alarm.sec - 1 : 59;
            case CONFIG_BUTTON:
                if (SmartWatch.setting_hour)
                {
                    SmartWatch.setting_hour = FALSE;
                    SmartWatch.setting_min = TRUE;
                }
                if (SmartWatch.setting_min)
                {
                    SmartWatch.setting_min = FALSE;
                    SmartWatch.setting_sec = TRUE;
                }
                if (SmartWatch.setting_sec)
                {
                    SmartWatch.setting_sec = FALSE;
                    SmartWatch.setting_alarm = FALSE;
                    SmartWatch.active_alarm = TRUE; //Active la alarma
                }
                break;
            }
        }
        else if (SmartWatch.active_alarm)
        {
            SmartWatch_check_alarm();
            if (SmartWatch.button_pressed == CONFIG_BUTTON)
            {
                SmartWatch.active_alarm = FALSE;
            }
        }
    }
    else
    {
        switch (SmartWatch.button_pressed)
        {
        case UP_BUTTON:
            SmartWatch.mode = CONFIG;
            SmartWatch.button_pressed = NO_BUTTON_PRESSED;
            break;
        case DOWN_BUTTON:
            SmartWatch.mode = CRONO;
            SmartWatch.button_pressed = NO_BUTTON_PRESSED;
            break;
        case CONFIG_BUTTON:
            //set alarm mode
            SmartWatch.mode_set = TRUE;
            SmartWatch.button_pressed = NO_BUTTON_PRESSED;
            break;
        default:
            break;
        }
    }
    //Se indica que ya se recorrió esta funcion al menos una vez
    SmartWatch.previous_mode = ALARM;
    return;
}

void SmartWatch_crono()
{
    SmartWatch_get_buttons();
    if (SmartWatch.mode_set) //Si el modo fué seleccionado:
    {
        if (SmartWatch.active_crono == FALSE)
        {
            SmartWatch.init_time_crono = SmartWatch.date_time; // Guardar el último tiempo actualizado
            SmartWatch.active_crono = TRUE;
            SmartWatch.mode_set = FALSE;
        }
        else //La siguiente vez que se pulse el boton CONFIG_BUTTON:
        {
            SmartWatch.end_time_crono.hour = SmartWatch.date_time.hour - SmartWatch.init_time_crono.hour;
            SmartWatch.end_time_crono.min = SmartWatch.date_time.min - SmartWatch.init_time_crono.min;
            SmartWatch.end_time_crono.sec = SmartWatch.date_time.sec - SmartWatch.init_time_crono.sec;
            //Almacenar el valor total en segundos:
            //SmartWatch.time_diff_crono puede tener un valor máximo de (3600*23+60*59+59) = 0x1517F = 86399seg
            //Puede que sea una variable innecesaria
            SmartWatch.time_diff_crono = 3600 * SmartWatch.end_time_crono.hour + 60 * SmartWatch.end_time_crono.min + SmartWatch.end_time_crono.sec;
            SmartWatch.active_crono = FALSE;
            SmartWatch.mode_set = FALSE;
        }
        return;
    }
    else
    {
        switch (SmartWatch.button_pressed)
        {
        case UP_BUTTON:
            SmartWatch.mode = ALARM;
            SmartWatch.button_pressed = NO_BUTTON_PRESSED;
            break;
        case DOWN_BUTTON:
            SmartWatch.mode = TIMER;
            SmartWatch.button_pressed = NO_BUTTON_PRESSED;
            break;
        case CONFIG_BUTTON:
            SmartWatch.mode_set = TRUE;
            SmartWatch.button_pressed = NO_BUTTON_PRESSED;
            break;
        default:
            if (SmartWatch.active_crono)
            {
                //Si está activo compare con el tiempo actualsi no se almacenará el ultimo valor cuando se pulsó el boton
                SmartWatch.end_time_crono.hour = SmartWatch.date_time.hour - SmartWatch.init_time_crono.hour;
                SmartWatch.end_time_crono.min = SmartWatch.date_time.min - SmartWatch.init_time_crono.min;
                SmartWatch.end_time_crono.sec = SmartWatch.date_time.sec - SmartWatch.init_time_crono.sec;
            }
            //Mostrar SmartWatch.end_time_crono
            /**
             * if (SmartWatch.mode != SmartWatch.previous_mode)
            */
            break;
        }
    }
    //Se indica que ya se recorrió esta funcion al menos una vez
    SmartWatch.previous_mode = CRONO;
    return;
}

void SmartWatch_timer()
{

    SmartWatch_get_buttons();
    if (SmartWatch.mode_set)
    {
        if (SmartWatch.active_timer == FALSE || SmartWatch.idle_timer == FALSE) // configure el tiempo de parada:
        {
            if (SmartWatch.setting_timer == FALSE)
            {
                SmartWatch.setting_hour = TRUE;
                SmartWatch.setting_timer = TRUE;
            }
            switch (SmartWatch.button_pressed)
            {
            case UP_BUTTON:
                if (SmartWatch.setting_hour)
                    SmartWatch.end_time_timer.hour = (SmartWatch.end_time_timer.hour < 23) ? SmartWatch.end_time_timer.hour + 1 : 0;
                if (SmartWatch.setting_min)
                    SmartWatch.end_time_timer.min = (SmartWatch.end_time_timer.min < 59) ? SmartWatch.end_time_timer.min + 1 : 0;
                if (SmartWatch.setting_sec)
                    SmartWatch.end_time_timer.sec = (SmartWatch.end_time_timer.sec < 59) ? SmartWatch.end_time_timer.sec + 1 : 0;

                break;
            case DOWN_BUTTON:
                if (SmartWatch.setting_hour)
                    SmartWatch.end_time_timer.hour = (SmartWatch.end_time_timer.hour > 0) ? SmartWatch.end_time_timer.hour - 1 : 23;
                if (SmartWatch.setting_min)
                    SmartWatch.end_time_timer.min = (SmartWatch.end_time_timer.min > 0) ? SmartWatch.end_time_timer.min - 1 : 59;
                if (SmartWatch.setting_sec)
                    SmartWatch.end_time_timer.sec = (SmartWatch.end_time_timer.sec > 0) ? SmartWatch.end_time_timer.sec - 1 : 59;
            case CONFIG_BUTTON:
                if (SmartWatch.setting_hour)
                {
                    SmartWatch.setting_hour = FALSE;
                    SmartWatch.setting_min = TRUE;
                }
                if (SmartWatch.setting_min)
                {
                    SmartWatch.setting_min = FALSE;
                    SmartWatch.setting_sec = TRUE;
                }
                if (SmartWatch.setting_sec)
                {
                    SmartWatch.setting_sec = FALSE;
                    SmartWatch.setting_timer = FALSE;
                    SmartWatch.idle_timer = TRUE; //Espere a la activacion de LA CUENTA REGRESIVA
                }
                break;
            }
        }
        else if (SmartWatch.idle_timer)
        {
            if (SmartWatch.button_pressed == CONFIG_BUTTON)
            {
                SmartWatch.idle_timer = FALSE;
                SmartWatch.active_timer = TRUE;
                //Si los segundos sobrepasan 59:
                if (SmartWatch.date_time.sec + SmartWatch.end_time_timer.sec > 59)
                {
                    SmartWatch.end_time_timer.min += 1;
                    SmartWatch.end_time_timer.sec = (SmartWatch.end_time_timer.sec + SmartWatch.date_time.sec) - 59;
                }
                else
                {
                    SmartWatch.end_time_timer.sec += SmartWatch.date_time.sec;
                }
                //Si los minutos sobrepasan 59:
                if (SmartWatch.date_time.min + SmartWatch.end_time_timer.min > 59)
                {
                    SmartWatch.end_time_timer.hour += SmartWatch.date_time.hour + 1;
                    SmartWatch.end_time_timer.min = (SmartWatch.end_time_timer.min + SmartWatch.date_time.min) - 59;
                }
                else
                {
                    SmartWatch.end_time_timer.hour += SmartWatch.date_time.hour;
                    SmartWatch.end_time_timer.min += SmartWatch.date_time.min;
                }
            }
        }
        else if (SmartWatch.active_timer)
        {
            SmartWatch_check_timer();
            //ACA DEBE MOSTRARSE EL TIMER DISMINUYENDO
            //Si el tiempo final y el actual son iguales o si se pulsa el reinicie la cuenta
            if (SmartWatch.button_pressed == CONFIG_BUTTON)
            {
                SmartWatch.timer_done = TRUE;
            }
        }
    }
    if (SmartWatch.timer_done)
    {
        //imprimir finalizado
        SmartWatch.timer_done = FALSE;
        SmartWatch.active_timer = FALSE;
        SmartWatch.idle_timer = FALSE;
        SmartWatch.setting_timer = FALSE;
        SmartWatch.end_time_timer.hour = 0;
        SmartWatch.end_time_timer.min = 0;
        SmartWatch.end_time_timer.sec = 0;
    }

    switch (SmartWatch.button_pressed)
    {
    case UP_BUTTON:
        SmartWatch.mode = CRONO;
        SmartWatch.button_pressed = NO_BUTTON_PRESSED;
        break;
    case DOWN_BUTTON:
        SmartWatch.mode = CLOCK;
        SmartWatch.button_pressed = NO_BUTTON_PRESSED;
        break;
    case CONFIG_BUTTON:
        //set alarm
        SmartWatch.mode_set = TRUE;
        SmartWatch.button_pressed = NO_BUTTON_PRESSED;
        break;
    default:
        break;
    }
    //Se indica que ya se recorrió esta funcion al menos una vez
    SmartWatch.previous_mode = TIMER;
    return;
}

void SmartWatch_clock()
{
    //El reloj no tiene interaccion es solo informativo
    SmartWatch_get_buttons();
    switch (SmartWatch.button_pressed)
    {
    case UP_BUTTON:
        SmartWatch.mode = TIMER;
        SmartWatch.button_pressed = NO_BUTTON_PRESSED;
        break;
    case DOWN_BUTTON:
        SmartWatch.mode = CONFIG;
        SmartWatch.button_pressed = NO_BUTTON_PRESSED;
        break;
    case CONFIG_BUTTON:
        SmartWatch.mode_set = TRUE;
        SmartWatch.button_pressed = NO_BUTTON_PRESSED;
        break;
    default:
        //MOSTRAR EL RELOJ
        break;
    }
    //Se indica que ya se recorrió esta funcion al menos una vez
    SmartWatch.previous_mode = CLOCK;
    return;
}

void SmartWatch_config()
{
    SmartWatch_get_buttons();
    switch (SmartWatch.button_pressed)
    {
    case UP_BUTTON:
        SmartWatch.mode = CLOCK;
        SmartWatch.button_pressed = NO_BUTTON_PRESSED;
        break;
    case DOWN_BUTTON:
        SmartWatch.mode = ALARM;
        SmartWatch.button_pressed = NO_BUTTON_PRESSED;
        break;
    case CONFIG_BUTTON:
        SmartWatch.mode_set = TRUE;
        SmartWatch.button_pressed = NO_BUTTON_PRESSED;
        break;
    default:
        break;
    }
    //Se indica que ya se recorrió esta funcion al menos una vez
    SmartWatch.previous_mode = CONFIG;
    return;
}

void SmartWatch_check_alarm()
{
    if ((SmartWatch.alarm.hour == SmartWatch.date_time.hour) && (SmartWatch.alarm.min == SmartWatch.date_time.min) && (SmartWatch.alarm.sec - SmartWatch.date_time.sec) <= 1)
    {
        SmartWatch.alarm_triggered = TRUE;
        SmartWatch.previous_mode = SmartWatch.mode;
        SmartWatch.mode = ALARM;
    }
    return;
}
void SmartWatch_check_timer()
{
    if ((SmartWatch.end_time_timer.hour == SmartWatch.date_time.hour) && (SmartWatch.end_time_timer.min == SmartWatch.date_time.min) && (SmartWatch.end_time_timer.sec - SmartWatch.date_time.sec) <= 1)
    {
        SmartWatch.timer_done = TRUE;
        SmartWatch.previous_mode = SmartWatch.mode;
        SmartWatch.mode = TIMER;
    }
    return;
}

void SmartWatch_get_buttons()
{
    if (SmartWatch.button_pressed != CONFIG)
    {
        if (!input(PIN_E1))
            SmartWatch.button_pressed = UP_BUTTON;
        else if (!input(PIN_E2))
            SmartWatch.button_pressed = DOWN_BUTTON;
        else
            SmartWatch.button_pressed = NO_BUTTON_PRESSED;
    }
}