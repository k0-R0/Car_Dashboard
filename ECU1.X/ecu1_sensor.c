#include "ecu_sensor1.h"
#include "adc.h"
#include "can.h"
#include "msg_id.h"
#include "uart.h"


unsigned char gear_pos[8] = {'N', '1', '2', '3', '4', '5', 'R', 'C'};

uint16_t get_speed() {
    // Implement the speed function
    uint32_t speed = read_adc(CHANNEL4);
    speed = (speed * 99) / 1023;
    return speed;
}

unsigned char get_gear_pos() {
    // Implement the gear function
    static unsigned char gear = 0;
    unsigned char key = read_digital_keypad(STATE_CHANGE);
    if (key == ALL_RELEASED)
        return gear_pos[gear];
    if (key != SWITCH3 && gear == 7) {
        gear = 0;
        return gear_pos[gear];
    }
    if (key == SWITCH1 && gear < 6)
        gear++;
    else if (key == SWITCH2 && gear > 0)
        gear--;
    else if (key == SWITCH3)
        gear = 7;

    return gear_pos[gear];
}
