/**
  ******************************************************************************
  * @file    main.c
  * @author  Alice, Banky
  * @version V1.0
  * @date    01-Februrary-2018
  * @brief   Default main function.
  *
  *
  * This file serves as a a PWM driver for a motor controller. It receives data
  * from the CAN bus and generates an appropriate PWM signal
  ******************************************************************************
*/
#include "mbed.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "canlib.h"
#include "pins.h"

Serial pc(USBTX, USBRX);
uint32_t pwm_ID = 400;
float freq = 50;
PwmOut pwm_pin(PB_15);
DigitalOut dir_pin(PC_6);

/*
 *  Generates a PWM signal with a duty cycle of pwm_duty/256
 */
void pwm_gen(uint32_t pwm_duty)
{
    float period = 1/freq;  
    pwm_pin.period(period);
    if (pwm_duty <= 255)
    {
        pwm_pin.write((float)pwm_duty/255.0);
    }
}

/*
 *  Controls the motor speed. Direction = 1 is clockwise
 */
void motor_control(int16_t speed)
{
    pwm_gen(abs(speed));
    dir_pin = (speed <= 0);
}

/*
 *  Receives pwm data from CAN bus
 */
void CANLIB_Rx_OnMessageReceived(void)
{
    uint32_t pwm_duty= CANLIB_Rx_GetAsUint(CANLIB_INDEX_0);
    pwm_gen(pwm_duty);
}

int main()
{
    if (CANLIB_Init(pwm_ID, CANLIB_LOOPBACK_ON) != 0)
    {
        pc.printf("CAN Initialization Faipwm_pin\r\n");
    }
    if (CANLIB_AddFilter(500) != 0)
    {
        pc.printf("CAN Add Filter Faipwm_pin\r\n");
    }

    // Test motor by moving it forward and backwards
    int16_t speed = 0;
    int16_t increment = 10;
    int16_t max_speed = 255;

    while (true) {
        pwm_gen(speed);
        speed += increment;
        if ((speed < -max_speed && increment < 0) || (speed > max_speed && increment > 0))
        {
            increment = -increment;
        }

        motor_control(speed);
        wait(0.2);
    }

}
#ifdef __cplusplus
}
#endif
