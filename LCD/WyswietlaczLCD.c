#include <REGX52.H>

extern void LcdInit();
extern void LcdWelcome();
extern void Lcd_Cursor (char row, char column);
extern void Lcd_DisplayCharacter (char a_char);
extern void Lcd_DisplayString (char row, char column, char *string);
extern void Lcd_WriteControl (unsigned char LcdCommand);

unsigned char data Var1, Var2, Var3;
unsigned char data Bufor[] = {"Lukasz Tymoszuk"};
unsigned char data ClearLine[] = {"                "};
unsigned char data LoadingChars[] = {0x00,0x01,0x02,0x03,0x04};
	
unsigned char CurrentLoadingChar;
unsigned char Wiersz;
unsigned char Kolumna;
unsigned char DlugoscBufora;
unsigned char Loading;

void LoadingOnOff(){
	TR1=!TR1;
}

void PrzesunDoGory(){
	Lcd_DisplayString (Wiersz,1,ClearLine);
	Wiersz--;
	if(Wiersz==0)Wiersz=4;
	Lcd_DisplayString (Wiersz,Kolumna,Bufor);
}

void PrzesunDoDolu(){
	Lcd_DisplayString (Wiersz,1,ClearLine);
	Wiersz++;
	if(Wiersz==5)Wiersz=1;
	Lcd_DisplayString (Wiersz,Kolumna,Bufor);
}

void PrzesunWLewo(){
	Lcd_DisplayString (Wiersz,1,ClearLine);
	if(Kolumna>1)Kolumna--;
	Lcd_DisplayString (Wiersz,Kolumna,Bufor);
}

void PrzesunWPrawo(){
	Lcd_DisplayString (Wiersz,1,ClearLine);
	if(DlugoscBufora+Kolumna<17)Kolumna++;
	Lcd_DisplayString (Wiersz,Kolumna,Bufor);
}


void ISR_INT0() interrupt 0{
	//PrzesunDoGory();
	//PrzesunWLewo();
	LoadingOnOff();
}

void ISR_INT1() interrupt 2{
	//PrzesunDoDolu();
	//PrzesunWPrawo();
}

void ISR_Timer1() interrupt 3{
	TR1=0;
	Loading++;
	if(Loading==80){
		Loading=0;
		Lcd_DisplayString (2,1,ClearLine);
	}
	
	CurrentLoadingChar = LoadingChars[Loading%5];
	Lcd_Cursor(2, (Loading/5)+1);
	Lcd_DisplayCharacter(CurrentLoadingChar);
	
	TH1=0xC0;
	TL1=0x00;
	TR1=1;
}

void init(){
	Wiersz = 1;
	Kolumna = 1;
	DlugoscBufora=15;
	EX0=1;
	EX1=1;
	IT0=1;
	IT1=1;
	
	ET1=1;
	TMOD=0x10;
	
	EA=1;
}

void main(void){
	LcdInit();
	init();
	LcdWelcome();
	while(1);
}