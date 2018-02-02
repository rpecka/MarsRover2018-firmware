/**
  ******************************************************************************
  * @file    main.c
  * @author  Banky
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

int main()
{
    if (CANLIB_Init(pwm_ID, CANLIB_LOOPBACK_ON) != 0)
    {
        pc.printf("CAN Initialization Failed\r\n");
    }
    if (CANLIB_AddFilter(500) != 0)
    {
        pc.printf("CAN Add Filter Failed\r\n");
    }
}

/*
 *  Generates a PWM signal with a duty cycle of pwm_duty/256
 */
void pwm_gen(uint32_t pwm_duty)
{

}

/*
 *  Receives pwm data from CAN bus
 */
void CANLIB_Rx_OnMessageReceived(void)
{
    uint32_t pwm_duty= CANLIB_Rx_GetAsUint(CANLIB_INDEX_0);
    pwm_gen(pwm_duty);
}
#ifdef __cplusplus
}
#endif