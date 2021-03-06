#include <REGX52.H>
#define TH0_Value 0x00;
#define TL0_Value 0x00;

char code kolumny[4] = {0xF7, 0xFB, 0xFD};
char code klawisze[12] = {0xE7, 0xD7, 0xB7, 0x7D, 0xEB, 0xDB, 0xBB, 0x7B, 0xED, 0xDD, 0xBD, 0x7D};
char code dekoder[12] = {1,4,7,10,2,5,8,11,3,6,9,12};

unsigned char Prawy;
unsigned char Lewy;
unsigned char dana;
unsigned char ID;

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
	char w =0;
	char val = 0;
	int flaga = 0;
	char nacisniety = 0x00;
	char nacisniety1 = 0x00;
	char nacisniety2 = 0x00;
	SerialInit();
	ES = 1;
	EA = 1;
	P3_4 = 0;
	ID=1;
	while(1){
		if(i==3)i=0;
		if(j==12)j=0;
		P2 = kolumny[i++];
		val = P2;
		if(val != kolumny[i-1]){
			for(w=0;w<4;w++){
				if(val == klawisze[j++]){
					while(P2==klawisze[j-1]);
					nacisniety=dekoder[j-1];
					flaga = 1;
				}
			}
		}
		else j+=4;
		if(flaga==1){
			flaga  = 0;
			if(nacisniety == 12){
				Prawy=nacisniety1; 
				Lewy=nacisniety2; 
				dana = (Lewy*10)+Prawy;
				Nadaj(dana, 3);
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