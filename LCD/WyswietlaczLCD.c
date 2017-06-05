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
unsigned char godzina;
unsigned char godzinaL;
unsigned char godzinaP;
volatile unsigned char Loading;
unsigned char Lewy;
unsigned char Prawy;

unsigned char flagaTestowa=0x00;

sbit DE = P3^4;
void Click(){
	godzinaL=godzina/10;
	godzinaP=godzina%10;
	if(godzinaP==10)
		godzinaP=0;
	if(godzinaL==6){
		godzinaL=0;
		godzinaP=0;
	}

	Loading++;
	if(Loading==80){
		Loading=0;
		Lcd_DisplayString (3,1,ClearLine);
	}
	Lcd_Cursor(2,10);
	
	Lcd_DisplayCharacter(godzinaL+48);
	Lcd_DisplayCharacter(godzinaP+48);
	Lcd_DisplayCharacter(':');
	Lcd_DisplayCharacter(Lewy+48);
	Lcd_DisplayCharacter(Prawy+48);
	
	
	
	CurrentLoadingChar = LoadingChars[Loading%5];
	Lcd_Cursor(3, (Loading/5)+1);
	Lcd_DisplayCharacter(CurrentLoadingChar);
}

void init(){
	Wiersz = 1;
	Kolumna = 1;
	Lewy=0;
	Prawy=0;
	Loading=0;
	EA=1;
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
	
void ISR_serial(void) interrupt 4
{
	if (RI == 1) {
		if(SBUF=='m'){
			Prawy++;
			if(Prawy==10){
				Prawy=0;
				Lewy++;
			}
			if(Lewy==6){
				Lewy=0;
				godzinaP++;
			}
		}
		else{
			if(SBUF<24)godzina = SBUF;
		}
		Click();
		RI=0;
	}
}

void main(void){
	LcdInit();
	SerialInit();
	init();
	ES = 1;
	EA = 1;
	P3_4 = 0;
	TR2=1;
	LcdWelcome();
	Lcd_DisplayString(1,1,Bufor);
	Lcd_DisplayString(2,1,"Godzina: ");
	Lcd_DisplayString(4,1,"0%          100%");
	while(1);
}