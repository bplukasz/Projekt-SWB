#include <REGX52.H>
#define TH0_Value 0x00;
#define TL0_Value 0x00;

char code wiersze[4] = {0xEF, 0xDF, 0xBF, 0x7F};
char code klawisze[12] = {0xE7, 0xEB, 0xED, 0xD7, 0xDB, 0xDD, 0xB7, 0xBB, 0xBD, 0x77, 0x7B, 0x7D};

void main(void){
	char i = 0;
	char j = 0;
	char k =0;
	char val = 0;
	int flaga = 0;
	char nacisniety = 0x00;
	char nacisniety1 = 0x00;
	char nacisniety2 = 0x00;
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
				//Prawy=nacisniety1; Wyslij prawy
				//Lewy=nacisniety2; Wyslij lewy
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