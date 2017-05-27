#include <REGX52.H>
#define TH0_Value 0x00;
#define TL0_Value 0x00;

char code wiersze[4] = {0xEF, 0xDF, 0xBF, 0x7F};
char code klawisze[12] = {0xE7, 0xEB, 0xED, 0xD7, 0xDB, 0xDD, 0xB7, 0xBB, 0xBD, 0x77, 0x7B, 0x7D};
char code dekoder[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
char code slimakTab[19]={0x01,0x02,0x04, 0x08, 0x08, 0x10, 0x20, 0x01, 0x01, 0x02, 0x40, 0x40, 0x20, 0x01, 0x01, 0x02, 0x40, 0x20, 0x01 };

char Lewy;
char Prawy;
char FlagaEkranu;
char counterSlimaka;

void ISR_INT0(void) interrupt 0{
	Prawy++;
	if(Prawy==10){
		Prawy=0;
		Lewy++;
	}
	if(Prawy==10)Prawy=0;
}

void ISR_INT1(void) interrupt 2{
	Prawy--;
	if(Prawy==-1){
		Prawy=9;
		Lewy--;
	}
	if(Lewy==-1)Lewy=9;
}

void zmienEkran(void){
	if(P3_0==0){
		P3_0=1;
		P3_1=0;
	}
	else{
		P3_0=0;
		P3_1=1;
	}
}

void slimak(void) interrupt 1{
	P2=0x00;
	if(counterSlimaka==4 || counterSlimaka==8 || counterSlimaka==12 || counterSlimaka==15)zmienEkran();
	P2=slimakTab[counterSlimaka];
	counterSlimaka++;
	if(counterSlimaka==19)counterSlimaka=0;
}



void ISR_Timer0(void){
	P2=0x00;
	TR0=0;
	if(FlagaEkranu==0){
		P2=dekoder[Prawy];
		P3_0=1;
		P3_1=0;
		FlagaEkranu=1;
	}
	else{
		P2=dekoder[Lewy];
		P3_0=0;
		P3_1=1;
		FlagaEkranu=0;
	}
	TH0 = TH0_Value;
	TL0 = TL0_Value;
	TR0=1;
}

void init(void){
	Lewy=0;
	Prawy=0;
	P2=0x00;
	P3_0=1;
	P3_1=0;
	FlagaEkranu=0;
	counterSlimaka=0;
	EX0=1;
	EX1=1;
	IT0=1;
	IT1=1;
	TMOD=0x01;
	TH0=TH0_Value;
	TL0=TL0_Value;
	ET0=1;
	EA=1;
	TR0=1;
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
	init();
	while(1){
		if(i==4)i=0;
		if(j==12)j=0;
		P1 = wiersze[i++];
		val = P1;
		if(val != wiersze[i-1]){
			for(k=0;k<3;k++){
				if(val == klawisze[j++]){
					while(P1==klawisze[j-1]);
					nacisniety=j;
					flaga = 1;
				}
			}
		}
		else j+=3;
		if(flaga==1){
			flaga  = 0;
			if(nacisniety == 10){
				Prawy=nacisniety1;
				Lewy=nacisniety2;
				nacisniety1 = 0x00;
				nacisniety2 = 0x00;
			}
			else{
				nacisniety2 = nacisniety1;
				if(nacisniety==11)nacisniety=0;
			  nacisniety1 = nacisniety;
			}
		}
	}
}