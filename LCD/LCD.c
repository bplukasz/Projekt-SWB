#define Test 0  // 1 - do testowania
                // 0 - normalna praca
// Definicje zasobow sprzetowych
sfr  P2    = 0xA0;
sbit P2_6  = P2^6;
sbit P2_5  = P2^5;
sbit P2_4  = P2^4;
sbit P2_3  = P2^3;
sbit P2_2  = P2^2;
sbit P2_1  = P2^1;
sbit P2_0  = P2^0;

unsigned char LoadingChar0[] = {	0x10,0x10,0x10,0x10,0x10,0x10,0x10};
unsigned char LoadingChar1[] = {	0x18,0x18,0x18,0x18,0x18,0x18,0x18};
unsigned char LoadingChar2[] = {	0x1c,0x1c,0x1c,0x1c,0x1c,0x1c,0x1c};
unsigned char LoadingChar3[] = {	0x1e,0x1e,0x1e,0x1e,0x1e,0x1e,0x1e};
unsigned char LoadingChar4[] = {	0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f};

// Definicje ogolne
#define False    			0  
#define True     			1

// Definicje podlaczenia wyswietlacza LCD
#define LCD_RS    P2_4         /* p1.4 LCD Register Select line           */
#define LCD_RW    P2_5         /* p1.5 LCD Read / Write line              */
#define LCD_E     P2_6         /* p1.6 LCD Enable line                    */
#define LCD_DB4   P2_0         /* high nibble of port 1 is used for data  */
#define LCD_DB5   P2_1         /* high nibble of port 1 is used for data  */
#define LCD_DB6   P2_2         /* high nibble of port 1 is used for data  */
#define LCD_DB7   P2_3         /* high nibble of port 1 is used for data  */

// Definicje komend wyswietlacza LCD
#define LCD_CONFIG		      0x28
#define LCD_CLEAR		        0x01
#define LCD_HOME		        0x02
#define LCD_ENTRY_MODE		  0x06
#define LCD_DISPLAY_OFF		  0x08
#define LCD_CURSOR_ON 		  0x0A
#define LCD_DISPLAY_ON		  0x0C
#define LCD_CURSOR_BLINK	  0x0D
#define LCD_CURSOR_LINE		  0x0E
#define LCD_CURSOR_COM		  0x0F
#define LCD_CURSOR_LEFT		  0x10
#define LCD_CURSOR_RIGHT	  0x14
#define LCD_SHIFT_LEFT		  0x18
#define LCD_SHIFT_RIGHT		  0x1C
#define LCD_SET_CGRAM_ADDR	0x40
#define LCD_SET_DDRAM_ADDR	0x80

static void Lcd_WriteData (unsigned char LcdData);
/*
#ifndef __LCD4B_H__
#define __LCD4B_H__

sbit LcdEnable = 0xA0^6;
sbit LcdRead	=	0xA0^5;
sbit LcdReg	=	0xA0^4;

void WriteToLcdCtrlRegister(char X)
{
	LcdReg = 0;								// ustawienie sygnalów sterujacych
	LcdRead = 0;
	Lcd_WriteData(X);
}

void LcdWrite(char X)
{
	LcdReg = 1;
	LcdRead = 0;
	Lcd_WriteData(X);
}

void DefineSpecialCharacters(char *ptr)
{
	WriteToLcdCtrlRegister(0x40);		// ustawienie trybu definicji
	while (*ptr != 0)						// petla wykonywana do napotkania znaku konca tablicy
	{
		LcdWrite(*ptr);					// zapis znaku do lcd cgram
		ptr++;								// nastepna pozycja tablicy (wskaznika)
	}
	WriteToLcdCtrlRegister(0x80);		// przelaczenie do trybu wyswietlania
}
*/
// Definicje ekranow
static unsigned char code Screan[] =  "                "
                                      "                "
                                      "                "
                                      "                ";

// Importowane procedury i funkcje
extern w1ms();
extern w5mS();
extern w50mS();


void Lcd_WriteControl (unsigned char LcdCommand)
 {
   unsigned char Lcd_Comm = 0;
   static bit LCDReady;
    
   LCD_RS = False;
   LCD_RW = False;

   Lcd_Comm = LcdCommand >> 4;
   P2 &= 0xF0;
   P2 |= Lcd_Comm;

   LCD_E = True;
   LCD_E = False;

   Lcd_Comm = LcdCommand & 0x0F;  
   P2 &=0xF0;      // P2 = P2 & 0xF0
   P2 |= Lcd_Comm;

   LCD_E = True;
   LCD_E = False;

   P2 |= 0x0F;     // P2 = P2 | 0x0F

   LCD_RW = True;
   LCD_RS = False;
   
   if (Test == 0)
    {
      LCDReady = 1;
      while (LCDReady == 1)
       {
         LCD_E = True;
         LCDReady = LCD_DB7;
         LCD_E	= False;
         LCD_E = True;
         LCD_E	= False;
       }
    }          
 }

// Wyslanie danych do wyswietlacza LCD. Magistrala 4-o bitowa
static void Lcd_WriteData (unsigned char LcdData)
 {
   unsigned char Lcd_Data = 0;
   static bit LCDReady;
 
   LCD_RS = True;
   LCD_RW = False;

   Lcd_Data = LcdData >> 4;
   P2 &= 0xF0;
   P2 |= Lcd_Data;

   LCD_E = True;
   LCD_E = False;

   Lcd_Data = LcdData & 0x0F;  
   P2 &=0xF0;   
   P2 |= Lcd_Data;

   LCD_E = True;
   LCD_E = False;

   P2 |= 0x0F;

   LCD_RW = True;
   LCD_RS = False;
   
   if (Test == 0)
    {
      LCDReady = 1;
      while (LCDReady == 1)
       {
         LCD_E = True;
         LCDReady = LCD_DB7;
         LCD_E	= False;
   	     LCD_E	= True;
   	     LCD_E	= False;
       }
    }             
 }

// Wyswietlenie znaku w miejscu polozenie kursora
void Lcd_DisplayCharacter (char a_char)
 {
   Lcd_WriteData(a_char);
 }

/* Ustawienie kursora na wskazany wiersz i kolumne.
*     1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16
*    ----------------------------------------------------------------
* 1 |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
* 2 |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
* 3 |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
* 4 |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
*/
void Lcd_Cursor (char row, char column)
 {
   if (row == 1) Lcd_WriteControl (0x80 + column - 1);
   if (row == 2) Lcd_WriteControl (0xc0 + column - 1);
   if (row == 3) Lcd_WriteControl (0x90 + column - 1);
   if (row == 4) Lcd_WriteControl (0xd0 + column - 1);
 }

// Wyswietlenie ciagu znakow w konkretym wierszu (bez zawijania wiersza)
void Lcd_DisplayRow (char row, char *string)
 {
   char i;
   Lcd_Cursor (row, 1);
   for (i=0; i<16; i++) Lcd_DisplayCharacter (*string++);
 }


// Wyswietlenie calego ekranu - 64 znaki. Znaki zawarte w tablicy
/* przyklad:
** char screen[] =  "PBW-850   2003r."
**			            "----------------"
**			            "  K. Murawski   "
**			            "J. Chudzikiewicz";
**      LCD_DisplayScreen(screen);
*/
void Lcd_DisplayScreen (char *ptr)
 {
   Lcd_DisplayRow(1,ptr +  0);
   Lcd_DisplayRow(2,ptr + 16);
   Lcd_DisplayRow(3,ptr + 32);
   Lcd_DisplayRow(4,ptr + 48);
 }

// Wyswietlenie ekranu powitalnego
void LcdWelcome(void)
 {
   Lcd_DisplayScreen(Screan);        
 }

// Wyswietlenie ciagu znakow od danej kolumny i wiersza
void Lcd_DisplayString (char row, char column, char *string)
 {
   Lcd_Cursor (row, column);
   while (*string) Lcd_DisplayCharacter (*string++);
 }

 void LCDBuildChar(unsigned char loc, unsigned char *p)
{
     unsigned char i;
	 
     if(loc<8)                                 //If valid address
	 {
         Lcd_WriteControl(0x40+(loc*8));               //Write to CGRAM
         for(i=0;i<8;i++)Lcd_WriteData(p[i]);                   //Write the character pattern to CGRAM
   }
	 Lcd_WriteControl(0x80);                           //shift back to DDRAM location 0
}
 
// Inicjalizacja wyswietlacza; 4 wiersze, 4 bitowa magistrala danych
void LcdInit(void)
 {
   w50mS();
   P2 = 0x83;
   LCD_E   = True;
     w1ms();
   LCD_E   = False;
   w5mS();
   LCD_E   = True;
     w1ms();
   LCD_E   = False;
   w1ms();
   LCD_E   = True;
     w1ms();
   LCD_E   = False;
   w1ms();
   LCD_DB4 = False;
   LCD_E   = True;
     w1ms();
   LCD_E   = False;
   w1ms();
   Lcd_WriteControl(LCD_CONFIG);     
   Lcd_WriteControl(LCD_CLEAR);      
   Lcd_WriteControl(LCD_DISPLAY_OFF);
   Lcd_WriteControl(LCD_DISPLAY_ON); 
   Lcd_WriteControl(LCD_ENTRY_MODE); 
	
	 LCDBuildChar(0, LoadingChar0);                    
   LCDBuildChar(1, LoadingChar1);                   
   LCDBuildChar(2, LoadingChar2);
   LCDBuildChar(3, LoadingChar3);                    
   LCDBuildChar(4, LoadingChar4);   
	 
   //Lcd_WriteControl(LCD_CURSOR_COM); 
   Lcd_WriteControl(LCD_CLEAR);      
 }