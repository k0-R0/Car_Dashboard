#include "ecu2_sensor.h"
#include "adc.h"
#include "can.h"
#include "msg_id.h"
#include "digital_keypad.h"
#include "uart.h"

uint16_t get_rpm() {
    //Implement the rpm function
    uint32_t rpm = read_adc(CHANNEL4);
    rpm = (rpm * 6000) / 1023;
    return (uint16_t) rpm;
}

IndicatorStatus process_indicator() {
    //Implement the indicator function
    static IndicatorStatus indicator;
    unsigned char key = read_digital_keypad(STATE_CHANGE);
    if (key == SWITCH1)
        indicator = e_ind_left;
    else if (key == SWITCH2)
        indicator = e_ind_right;
    else if (key == SWITCH3)
        indicator = e_ind_hazard;
    else if(key == SWITCH4)
        indicator = e_ind_off;
    return indicator;
}
