// BranchingFunctionsDelays.c Lab 6
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to 
// toggle an LED while a button is pressed and 
// turn the LED on when the button is released.  
// This lab will use the hardware already built into the LaunchPad.
// Daniel Valvano, Jonathan Valvano
// January 15, 2016

// built-in connection: PF0 connected to negative logic momentary switch, SW2
// built-in connection: PF1 connected to red LED
// built-in connection: PF2 connected to blue LED
// built-in connection: PF3 connected to green LED
// built-in connection: PF4 connected to negative logic momentary switch, SW1

#include "TExaS.h"

unsigned long SW1;
unsigned long PF2;

#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC)) // perform input/output
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400)) // direction register -- is it input(bit 0) or output(bit 1)?
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOF      0x00000020  // port F Clock Gating Control

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(unsigned long time);

int main(void){ unsigned long volatile delay;
  TExaS_Init(SW_PIN_PF4, LED_PIN_PF2);  // activate grader and set system clock to 80 MHz
  // initialization goes here
	SYSCTL_RCGC2_R |= 0x00000020; //activate port f clock
	delay = SYSCTL_RCGC2_R; // allow time for clock to start
	GPIO_PORTF_AMSEL_R &= ~0x14;
	GPIO_PORTF_PCTL_R &= ~0x000F0F00;
	GPIO_PORTF_DIR_R = 0x04;
	GPIO_PORTF_AFSEL_R &= ~0x14;
	GPIO_PORTF_DEN_R = 0x14;
	GPIO_PORTF_PUR_R = 0x10;
	GPIO_PORTF_DATA_R = 0x04;
	
  EnableInterrupts();           // enable interrupts for the grader
	
  while(1){
    // body goes here
		SW1 = GPIO_PORTF_DATA_R&0x10;     // read PF4 into SW1
		Delay100ms(1);
		if(!SW1) {
			GPIO_PORTF_DATA_R ^= 0x04;
		} else if (SW1) {
			GPIO_PORTF_DATA_R = 0x04;
		} 
	}
}

void Delay100ms(unsigned long time){
  unsigned long i;
  while(time > 0){
    i = 1333333;  // this number means 100ms
    while(i > 0){
      i = i - 1;
    }
    time = time - 1; // decrements every 100 ms
  }
}
