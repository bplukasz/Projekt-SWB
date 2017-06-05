#include <REGX52.H>

volatile unsigned char sekundnik;
volatile unsigned char minutnik;
 
 
void ISRsekundnik() interrupt 2{
	sekundnik++;
	if(sekundnik==60){
		sekundnik=0;
		minutnik++;
		P2_4=1;
	}
}


void ISRminutnik() interrupt 0{
	P2_4=0;
}
 void SerialInit(void)
{
  SCON  = 0x50; // REN  = 1
  T2CON = 0x38; // TCLK = 1
                  // RCLK = 1
                  // TR2  = 0
                  // EXEN2= 1
  RCAP2L = TL2 = 0xDC;
  RCAP2H = TH2 = 0xFF;
  TR2 = 1;
}
void init(void){
	sekundnik=0;
	minutnik=0;
	EX0=1;
	IT0=1;
	EX1=1;
	IT1=1;
	EA=1;
	P2=0xDD;
}

void main(void){
	
	SerialInit();
	init();
		ES = 1;

	P3_4 = 0;
	TR2=1;
	while(1);
}