#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/timer.h"
typedef enum{ MAIN, JWCLAY, McCULLOUGH, CITYBLVD, MAX} state;

//The reason it goes from First Station: Pin 3 to Fourth Station:Pin 0
//It was just easier to organize the wires while also having my starting led away start at the very end.
void FirstStation(){
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x0);
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6, GPIO_PIN_6);
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_PIN_3);
}

void SecondStation(){
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x0);
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_PIN_2);
}

void ThirdStation(){
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x0);
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, GPIO_PIN_1);
}

void FourthStation(){
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x0);
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, GPIO_PIN_0);
}

void GreenLed(){
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6, 0x0);
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, GPIO_PIN_4);
}

void RedLed(){
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0x0);
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6, GPIO_PIN_6);

}

int Reverse = false;
state MainStation(uint32_t seconds){
    FirstStation();
    state Next = MAIN;
    if(seconds == 1)
        Next = JWCLAY;
        Reverse  = false;
    return Next;
}
state JWClayStation(uint32_t seconds){
    SecondStation();
    state Next = JWCLAY;
    if(seconds == 1 && !Reverse )
        Next = McCULLOUGH;
    else if(seconds == 1 && Reverse )
        Next = MAIN;
    return Next;
}
state McCulloughStation(uint32_t seconds){
    ThirdStation();
    state Next = McCULLOUGH;
    if(seconds == 1 && !Reverse )
        Next = CITYBLVD;
    else if(seconds == 1 && Reverse )
        Next = JWCLAY;
    return Next;
}
state CityStation(uint32_t seconds){
    FourthStation();
    state Next = CITYBLVD;
    if(seconds == 1){
            Next = McCULLOUGH;
            Reverse  = true;
    }
    return Next;
}

volatile uint32_t seconds = 0;
void Timer(){
    seconds++;
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
}

void Crosswalk(state next){
    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x0);
    while(seconds != 2){
        if(next == McCULLOUGH && seconds == 1){
            GreenLed();
        }
        else if (next != McCULLOUGH && seconds == 1){
           RedLed();
        }
    }
    seconds = 0;
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
}

// State table
state(*state_table[MAX])(uint32_t) = {MainStation, JWClayStation, McCulloughStation, CityStation};

// From ADC to Digital Voltage Value
 ADC(void){
    uint32_t adcValues[1];
    volatile float voltage;
    ADCProcessorTrigger(ADC0_BASE, 3);
    while(!ADCIntStatus(ADC0_BASE, 3, false));
    {
    }
    ADCIntClear(ADC0_BASE, 3);
    ADCSequenceDataGet(ADC0_BASE, 3, adcValues);
    voltage = (float)adcValues[0] * (3.3/4096.0);
    return voltage;
}

int main(void)
{
    state Current = MAIN;
    state Next = Current;

    //Sets clock to 16 MHz
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // Enable the GPIO port that is used
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    // Outputs
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_4);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_6);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_1);

    //Enable the ADC functions and get data
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);
    ADCIntClear(ADC0_BASE, 3);

    // Enable the Timer functions and get data
    TimerClockSourceSet(TIMER0_BASE, TIMER_CLOCK_SYSTEM);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, 64000);
    TimerPrescaleSet(TIMER0_BASE, TIMER_A, 200);
    TimerIntRegister(TIMER0_BASE, TIMER_A, Timer);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER0_BASE, TIMER_A);

    int Switch = ADC();

    while(1)
    {
        Switch = ADC();
        if(Current < MAX){
            Next = state_table[Current](seconds);
        }
        if(Current != Next){
            seconds = 0;
            Crosswalk(Next);
        }
        Current = Next;
        while(Switch == 0){
            //Stops the train
            TimerDisable(TIMER0_BASE, TIMER_A);
            Switch= ADC();
        }
        //Train is no longer stopping
        TimerEnable(TIMER0_BASE, TIMER_A);
    }
}


