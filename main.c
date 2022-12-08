#include <msp430.h> 

volatile unsigned int c = 0;
volatile unsigned int game = 1;
/**
 * main.c
 */
volatile unsigned int b1counter = 0;
volatile unsigned int b2counter = 0;


void delay(int max){

    volatile unsigned int i;

    for(i=0;i<max;i++){

        volatile unsigned int j = 65000;

        while(j > 0){
            j--;
        }
    }
}

void clear(){
    P1OUT &= ~BIT5;
    P4OUT &= ~BIT0;
}
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	//led
	P4DIR |= BIT0;
	P1DIR |= BIT5;

    //external button pullup
    P1DIR &= ~BIT3;
    P1IES |= BIT3;

    P1IFG &= ~BIT3;
    P1IE |= BIT3;

    //second external button pullup
    P3DIR &= ~BIT4;
    P3IES |= BIT4;

    P3IFG &= ~BIT4;
    P3IE |= BIT4;

    //third button this one starts time
    P2DIR &= ~BIT5;
    P2IES |= BIT5;

    P2IFG &= ~BIT5;
    P2IE |= BIT5;

    P1DIR |= BIT0;
    P1OUT &= ~BIT0;

    //Timer
    TB0CTL |= TBCLR;             // Reset timer
    TB0CTL |= TBSSEL__ACLK;      // Clock = ACKL
     TB0CTL |= MC__CONTINOUS;     // Mode=Continuous



      //Setup TB0 overflow IRQ
      TB0CTL |= TBIE;

    __enable_interrupt();
    PM5CTL0 &= ~LOCKLPM5;
    TB0CTL &= ~TBIFG;


    clear();

    for(;;){

   }
	return 0;
}

//player1 button
#pragma vector = PORT1_VECTOR
__interrupt void ISR_PORT1_BIT3(void){

    b1counter++;

    P1IFG &= ~BIT3;
}
//player 2 button
#pragma vector = PORT3_VECTOR
__interrupt void ISR_PORT3_BIT4(void){


    b2counter++;

    P3IFG &= ~BIT4;
}
//start timer button
/*#pragma vector = PORT2_VECTOR
__interrupt void ISR_PORT2_BIT5(void){


    game = 1;

    P2IFG &= ~BIT5;
}
*/
#pragma vector = TIMER0_B1_VECTOR
__interrupt void ISR_TB0_Overflow(void){

    if(game == 1){
        c += 2;
        if(c == 6){
           P1OUT ^= BIT0;
            if(b1counter > b2counter){
                P1OUT |= BIT5;

                  //  P4OUT |= BIT0;
                }
            else if(b2counter > b1counter){
                P4OUT |= BIT0;

              //  P1OUT |= BIT5;
            }else{
                P4OUT |= BIT0;
                P1OUT |= BIT5;
            }

            delay(10);

            b1counter = 0;
            b2counter = 0;

            P1OUT &= ~BIT5;
            P4OUT &= ~BIT0;
            c = 0;
        //game = 0;
    }
    else if (game == 0){
        clear();
    }
   }
   TB0CTL &= ~TBIFG;             // Clear IRQ Flag
}

