#include "adc.h"
#include "can.h"
#include "ecu_sensor1.h"
#include "msg_id.h"

void config(void) {
    init_can();
    init_adc();
    init_digital_keypad();
}

void convert_speed(uint16_t speed, unsigned char *buffer) {
    for (int i = 1; i >= 0; i--) {
        buffer[i] = (speed % 10 + '0');
        speed /= 10;
    }
    buffer[2] = '\0';
}

int main() {
    //Call the functions
    config();
    uint16_t speed;
    unsigned char gear;
    unsigned char speed_str[3];
    while (1) {
        speed = get_speed();
        convert_speed(speed, speed_str);
        can_transmit(SPEED_MSG_ID, speed_str, 3);
        __delay_ms(50);
        gear = get_gear_pos();
        can_transmit(GEAR_MSG_ID, &gear, 1);
        __delay_ms(50);
    }
}