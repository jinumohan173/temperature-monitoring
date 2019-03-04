#include<pic.h>
#define _XTAL_FREQ 20e6
#define relay RA5
#define rs RA2
#define en RA3

void serial_init();
void serial_display(const char *w);
void serial_out(unsigned char z);
void gsm_init();
void interrupt funct(void);
void gprs();
void adc_init();
void adc_conversion();
void hexa_val(unsigned int c);
void lcd_init();
void command(unsigned char);
void lcd_data(unsigned char);
void lcd_display(const char*);

unsigned int a,b,c,d,j,k,l,m,n;
							
void main()
{
TRISA=0x01;
PORTA=0x00;
TRISD=0x00;
PORTD=0x00;
TRISC=0x80;
PORTC=0x00;
adc_init();
lcd_init();
command(0x80);
lcd_display("TEMP:");
command(0x87);
lcd_display("Deg");
serial_init();
gsm_init();
while(1)
{
adc_conversion();
command(0x85);
hexa_val(c);
gprs();
}
}
void gsm_init()
{
serial_display("AT");
serial_out(0x0d);
__delay_ms (500);
serial_display("AT");
serial_out(0x0d);
__delay_ms (500);
serial_display("AT+CREG?");
serial_out(0x0d);
__delay_ms (500);
serial_display("AT+SAPBR=1,1");
serial_out(0x0d);
__delay_ms (1000);
serial_display("AT+HTTPINIT");
serial_out(0x0d);
__delay_ms (1000);
}
void serial_init()
{
SPBRG=129;
BRGH=1;
SYNC=0;
SPEN=1;
RCIE=1;
CREN=1;
TXEN=1;
GIE=1;
PEIE=1;
}
void gprs()
{
serial_display("AT+HTTPPARA=");
serial_out('"');
serial_display("URL");
serial_out('"');
serial_out(',');
serial_out('"');
serial_display("krishtec.co.in/m2m?S=");
serial_out(m+0x30);
serial_out(n+0x30);
serial_out('"');
serial_out(0x0d);
__delay_ms (1000);
serial_display("AT+HTTPPARA=");
serial_out('"');
serial_display("CID");
serial_out('"');
serial_out(',');
serial_out('1');
serial_out(0x0d);
__delay_ms (2000);
serial_display("AT+HTTPACTION=1");
serial_out(0x0d);
__delay_ms (1000);
}

void serial_out(unsigned char z)
{
TXREG=z;
while(!TXIF);
TXIF=0;
}
void serial_display(const char *w)
{
while(*w)
{
serial_out(*w++);
}
}

void adc_init()
{
ADCON1=0x8E;
}
void adc_conversion()
{
	ADCON0=0XC5;
	while(ADGO);
	a=ADRESL;
	b=ADRESH;
	b=b*256;
	c=b+a/2;
}
	void hexa_val(unsigned int val)
	{
	d=val/1000;
	j=val%1000;
	k=j/100;
	l=j%100;
	m=l/10;
	n=l%10;
	lcd_data(m+0x30);
	lcd_data(n+0x30);
	}
void lcd_init()
{
command(0x01);
command(0x06);
command(0x0c);
command(0x38);
}
void command(unsigned char com)
{
PORTD=com;
rs=0;
en=1;
__delay_ms(10);
en=0;
__delay_ms(10);
}
void lcd_data(unsigned char D)
{
PORTD=D;
rs=1;
en=1;
__delay_ms(10);
en=0;
__delay_ms(10);
}
void lcd_display(const char *word)
{
while(*word)
{
lcd_data(*word++);
__delay_ms(1);
}
}
