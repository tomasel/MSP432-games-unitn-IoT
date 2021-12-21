#include "input.h"

void _inputInit (){
    /* Configures Pin 6.0 and 4.4 as ADC input */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN0, GPIO_TERTIARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN4, GPIO_TERTIARY_MODULE_FUNCTION);

    /* Initializing ADC (ADCOSC/64/8) */
    ADC14_enableModule();
    ADC14_initModule(ADC_CLOCKSOURCE_ACLK, ADC_PREDIVIDER_32, ADC_DIVIDER_1, 0); // ACLK/32 = 2 kHz

    /* Configuring ADC Memory (ADC_MEM0 - ADC_MEM1 (A15, A9)  with repeat)
         * with internal 2.5v reference */
    ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);
    ADC14_configureConversionMemory(ADC_MEM0,
            ADC_VREFPOS_AVCC_VREFNEG_VSS,
            ADC_INPUT_A15, ADC_NONDIFFERENTIAL_INPUTS);

    ADC14_configureConversionMemory(ADC_MEM1,
            ADC_VREFPOS_AVCC_VREFNEG_VSS,
            ADC_INPUT_A9, ADC_NONDIFFERENTIAL_INPUTS);

    /* Enabling the interrupt when a conversion on channel 1 (end of sequence)
     *  is complete and enabling conversions */
    ADC14_enableInterrupt(ADC_INT1);

    /* Enabling Interrupts */
    Interrupt_enableInterrupt(INT_ADC14);
    /* Setting up the sample timer to automatically step through the sequence
     * convert.
     */
    ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

    /* Triggering the start of the sample */
    ADC14_enableConversion();
    ADC14_toggleConversionTrigger();

    //BUTTON B
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5,GPIO_PIN1);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P5,GPIO_PIN1);
    //Interrupt_enableInterrupt(INT_PORT5);

    //BUTTON A
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3,GPIO_PIN5);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P3,GPIO_PIN5);
    //Interrupt_enableInterrupt(INT_PORT3);

    //BUTTON J
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4,GPIO_PIN1);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P4,GPIO_PIN1);
    //Interrupt_enableInterrupt(INT_PORT4);

    //BUTTON 1 (on board left)
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1,GPIO_PIN1);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P1,GPIO_PIN1);
    //BUTTON 2 (on board right)
   MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1,GPIO_PIN4);
   MAP_GPIO_enableInterrupt(GPIO_PORT_P1,GPIO_PIN4);
   //Interrupt_enableInterrupt(INT_PORT1);
}

void ADC14_IRQHandler(void) {   //handles joystick input and stores to variables<
    uint64_t status;
    status = ADC14_getEnabledInterruptStatus();
    ADC14_clearInterruptFlag(status);

    /* ADC_MEM1 conversion completed */
    if(status & ADC_INT1) {
        /* Store ADC14 conversion results */
        joystick_x = ADC14_getResult(ADC_MEM0);
        joystick_y = ADC14_getResult(ADC_MEM1);
    }
}

uint16_t getButtons(){
    uint16_t buttons = 0;
    buttons |= GPIO_getInputPinValue(GPIO_PORT_P1,GPIO_PIN1);    //button 1;
    buttons |= GPIO_getInputPinValue(GPIO_PORT_P1,GPIO_PIN4)<<1; //button 2;
    buttons |= GPIO_getInputPinValue(GPIO_PORT_P5,GPIO_PIN1)<<2; //button A;
    buttons |= GPIO_getInputPinValue(GPIO_PORT_P3,GPIO_PIN5)<<3; //button B;
    buttons |= GPIO_getInputPinValue(GPIO_PORT_P4,GPIO_PIN1)<<4; //button J;

    buttons |= (joystick_y < JOYSTICK_CENTER + JOYSTICK_DEVIATION)<<5;
    buttons |= (joystick_y > JOYSTICK_CENTER - JOYSTICK_DEVIATION)<<6;
    buttons |= (joystick_x < JOYSTICK_CENTER + JOYSTICK_DEVIATION)<<7;
    buttons |= (joystick_x > JOYSTICK_CENTER - JOYSTICK_DEVIATION)<<8;
    return ~buttons;
}
