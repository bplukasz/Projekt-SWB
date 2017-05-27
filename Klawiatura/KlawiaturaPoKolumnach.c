#include <REGX52.H>
#define TH0_Value 0x00;
#define TL0_Value 0x00;

char code kolumny[4] = {0xFD, 0xFB, 0xF7};
char code klawisze[12] = {0xE7, 0xD7, 0xB7, 0x7D, 0xEB, 0xDB, 0xBB, 0x7B, 0xED, 0xDD, 0xBD, 0x7D};
char code dekoder[12] = {1,4,7,10,2,5,8,11,3,6,9,12};

void main(void){
	char i = 0;
	char j = 0;
	char w =0;
	char val = 0;
	int flaga = 0;
	char nacisniety = 0x00;
	char nacisniety1 = 0x00;
	char nacisniety2 = 0x00;
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