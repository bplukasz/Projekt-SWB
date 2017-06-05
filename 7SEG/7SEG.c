#include <REGX52.H>
#define TH1_Value 0x1F;
#define TL1_Value 0x00;

unsigned char xdata LewyW  _at_ 0xFE00;
unsigned char xdata PrawyW _at_ 0xFD00;

unsigned char code dekoder[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
volatile unsigned char FlagaEkranu;
volatile unsigned char Lewy;
volatile unsigned char Prawy;


void ISR_Serial(void) interrupt 4
 {
   if (RI == 1)
    {
      RI = 0;
      //Odbierz();
    }
 }

void Nadaj(unsigned char Dana, unsigned char Adresat)
 {
	 P3_4 = 1;
   TI = 0;
	 Dana=(Adresat<<6)|Dana;
   SBUF = Dana;
   while(TI == 0) {;}
   TI = 0;
   P3_4 = 0;
 } 
 
 
 void Wyswietlanie(){
	 PrawyW=dekoder[Prawy]; 
	 LewyW=dekoder[Lewy];
 }
 
 void inkrementuj(){
	Prawy++;
	if(Prawy==10){
		Prawy=0;
		Lewy++;
	}
	if(Lewy==6){
		Lewy=0;
		Nadaj('<',3);
	}
 }
 
 void ISR_Timer1(void)interrupt 3{
	TR1=0;
	if(FlagaEkranu==0){
		inkrementuj();
	}
	FlagaEkranu=!FlagaEkranu;
	TH1 = TH1_Value;
	TL1 = TL1_Value;
	TR1=1;
}
 
 void ISR_INT0(void) interrupt 0{
	inkrementuj();
	Nadaj('<',3);
}

 void SerialInit(void)
 {
	 P3_4=0;
   TH2 = RCAP2H = 0xFF;
   TL2 = RCAP2L = 0xDC;
   T2CON = 0x30;
   SCON = 0x50;
   ES = 1;
 }
 
void init(void){
	Lewy=0;
	Prawy=0;
	FlagaEkranu=0;
	EX0=1;
	IT0=1;
	TH1=TH1_Value;
	TL1=TL1_Value;
	ET1=1;
	TMOD=0x10;
	EA=1;
	TR1=1;
	TR2=1;
}

void main(void){
	SerialInit();
	init();
	while(1){
		Wyswietlanie();
	}
}