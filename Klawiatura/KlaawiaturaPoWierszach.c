#include <REGX52.H>
#define TH0_Value 0x00;
#define TL0_Value 0x00;

char code wiersze[4] = {0xEF, 0xDF, 0xBF, 0x7F};
char code klawisze[12] = {0xE7, 0xEB, 0xED, 0xD7, 0xDB, 0xDD, 0xB7, 0xBB, 0xBD, 0x77, 0x7B, 0x7D};
unsigned char Prawy;
unsigned char Lewy;
unsigned char dana;
void Nadaj(unsigned char Dana)
 {
   P3_4 = 1;
   TI = 0;
   SBUF = Dana;
   while(TI == 0) {;}
   TI = 0;
   P3_4 = 0;
 } 
 

void InitSerial(void){
		Prawy=0x00;
		Lewy=0x00;
		P3_4 = 0;
		T2CON = 0x30;
		SCON = 0X50;
		TH2 = RCAP2H = 0xFF;
		TL2 = RCAP2L = 0XDC;
		ES = 1;
		EA  = 1;
		TR2 = 1;
		RI = 1;
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

void main(void){
	char i = 0;
	char j = 0;
	char k =0;
	char val = 0;
	int flaga = 0;
	char nacisniety = 0x00;
	char nacisniety1 = 0x00;
	char nacisniety2 = 0x00;
	SerialInit();
	ES = 1;
	EA = 1;
	P3_4 = 0;
	while(1){
		if(i==4)i=0;
		if(j==12)j=0;
		P2 = wiersze[i++];
		val = P2;
		if(val != wiersze[i-1]){
			for(k=0;k<3;k++){
				if(val == klawisze[j++]){
					while(P2==klawisze[j-1]);
					nacisniety=j;
					flaga = 1;
				}
			}
		}
		else j+=3;
		if(flaga==1){
			flaga  = 0;
			if(nacisniety == 12){
				Prawy=nacisniety1; 
				Lewy=nacisniety2; 
				dana = (Lewy*10)+Prawy;
				Nadaj(dana);
				nacisniety1 = 0x00;
				nacisniety2 = 0x00;
			}
			if(nacisniety==10){
				if(P0_0==1)
					P0_0=0;
				else P0_0=1;
			}
			else{
				nacisniety2 = nacisniety1;
				if(nacisniety==11)nacisniety=0;
			  nacisniety1 = nacisniety;
			}
		}
	}
}