#include <xc.h>
#include <string.h>
#include "message_handler.h"
#include "msg_id.h"
#include "can.h"
#include "clcd.h"


volatile unsigned char led_state = LED_OFF, status = e_ind_off;

void handle_speed_data(uint8_t *data, uint8_t len) {
    //Implement the speed function
    data[len] = '\0';
    clcd_print(data, LINE2(0));
}

void handle_gear_data(uint8_t *data, uint8_t len) {
    //Implement the gear function
    clcd_putch(data[0], LINE2(5));
}

void handle_rpm_data(uint8_t *data, uint8_t len) {
    //Implement the rpm function
    data[len] = '\0';
    clcd_print(data, LINE2(7));
}

void handle_engine_temp_data(uint8_t *data, uint8_t len) {
    //Implement the temperature function
}

void blink(void) {
    static int delay = 0;
    if (++delay < 10) {
        RIGHT_IND_ON();
        LEFT_IND_ON();
    } else if (delay < 20) {
        RIGHT_IND_OFF();
        LEFT_IND_OFF();
    } else
        delay = 0;
}

void handle_indicator_data(uint8_t *data, uint8_t len) {
    //Implement the indicator function
    static unsigned char status = 0xFF;
    static int delay = 0;
    switch (data[0]) {
        case e_ind_off:
        {
            if (status != data[0])
                clcd_print("OFF ", LINE2(12));
            RIGHT_IND_OFF();
            LEFT_IND_OFF();
            break;
        }
        case e_ind_left:
        {
            if (status != data[0])
                clcd_print("<-  ", LINE2(12));
            RIGHT_IND_OFF();
            if (++delay < 10)
                LEFT_IND_ON();
            else if (delay < 20)
                LEFT_IND_OFF();
            else
                delay = 0;
            break;
        }
        case e_ind_right:
        {
            if (status != data[0])
                clcd_print("  ->", LINE2(12));
            LEFT_IND_OFF();
            if (++delay < 10)
                RIGHT_IND_ON();
            else if (delay < 20)
                RIGHT_IND_OFF();
            else
                delay = 0;
            break;
        }
        case e_ind_hazard:
        {
            if (status != data[0])
                clcd_print("<-->", LINE2(12));
            blink();
            break;
        }
    }
    status = data[0];
}

void process_canbus_data() {
    //process the CAN bus data
    uint16_t msg_id;
    uint8_t data[8];
    uint8_t len;
    can_receive(&msg_id, data, &len);
    if (len) {
        switch (msg_id) {
            case SPEED_MSG_ID: handle_speed_data(data, len);
                break;
            case GEAR_MSG_ID: handle_gear_data(data, len);
                break;
            case RPM_MSG_ID: handle_rpm_data(data, len);
                break;
            case INDICATOR_MSG_ID: handle_indicator_data(data, len);
                break;
        }
    }
}
