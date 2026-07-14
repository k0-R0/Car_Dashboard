#include "ecu2_sensor.h"
#include "adc.h"
#include "can.h"
#include "msg_id.h"
#include "digital_keypad.h"
#include "uart.h"

void config() {
    init_digital_keypad();
    init_adc();
    init_can();
}

void buff_cpy(char *dest, char *src) {
    int i;
    for (i = 0; src[i]; i++)
        dest[i] = src[i];

    dest[i] = '\0';
}

void write_indicator_buffer(IndicatorStatus indicator, char *indicator_buffer) {
    switch (indicator) {
        case e_ind_off: buff_cpy(indicator_buffer, "OFF  ");
            break;
        case e_ind_left: buff_cpy(indicator_buffer, "<-   ");
            break;
        case e_ind_right: buff_cpy(indicator_buffer, "->   ");
            break;
        case e_ind_hazard: buff_cpy(indicator_buffer, "<- ->");
            break;
    }
}

void write_rpm_buffer(uint16_t rpm, char *rpm_buffer) {
    int i = 3;
    while (i >= 0) {
        rpm_buffer[i--] = (rpm % 10) + '0';
        rpm /= 10;
    }
    rpm_buffer[4] = '\0';
}


int8_t indicator_buffer[6];
uint8_t rpm_buffer[5];

int main() {
    //call the functions
    config();
    while (1) {
        uint16_t rpm = get_rpm();
        write_rpm_buffer(rpm, rpm_buffer);
        can_transmit(RPM_MSG_ID, rpm_buffer, 4);
        __delay_ms(50);
        uint8_t indicator = process_indicator();
        //        write_indicator_buffer(indicator, indicator_buffer);
        can_transmit(INDICATOR_MSG_ID, &indicator, 1);
        __delay_ms(50);
        //        puts("\n\r indicator : ");
        //        puts(indicator_buffer);
        //        puts("\n\r rpm : ");
        //        puts(rpm_buffer);
    }
}
