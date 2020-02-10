#include<p18f4550.h>
#include <math.h>
#include "vector_relocate.h"

//Declarations
#define pi 3.141592 
float lat[5]={18.457419,18.457577,18.458006,18.458265,18.458570};
float longi[5]={73.851382,73.851238,73.851283,73.851242,73.851209};
float ghdg[5]={42.479,353.912,8.365,5.710};
unsigned int a,out_num;
unsigned char string1[]={"\n\rEnter the latitude"},
			  string2[]={"\n\rEnter the longitude"},
			  string3[]={"\n\rForward"},
			  string4[]={"\n\rBack"},
			  string5[]={"\n\rRight"},
			  string6[]={"\n\rLeft"},
			  string7[]={"\n\rSlight Right"},
			  string8[]={"\n\rSlight Left"},
			  string9[]={"\n\rHard Right"},
			  string10[]={"\n\rHard Left"},
		      string11[]={"\n\rYou have reached"};
//Function Prototypes
float convert(char a[]);
int nearway(float diflat[],float diflong[]);
void TXstring(unsigned char *string);
void TXbyte(unsigned char data);
unsigned char rx_data(void);
void msdelay (unsigned int time);		//Function to generate delay
void delay(void);						//for 10usec delay using timer
void objectdetect(void);
//Start of Main Program
void main()
{
	char latitude[10],longitude[10];
    float difflat,difflongi;
	unsigned int i, flag=0,check,prev=0;
    float currentlat,currentlong,diflat[5],diflong[5];
	float chdg,devlat,devlong,dhdg;
    TRISDbits.TRISD0=1;		//For switch
	TRISDbits.TRISD1=0;
    TRISC=0b10000010;		//echo(RC1) and trigger(RC0)
	TRISB=0X00;
	TRISA=0X00;
    ADCON1=0X0F;
	INTCON2bits.RBPU=0;
	PORTB=0XFF;
	PORTDbits.RD1=1; 
	SPBRGH=0X04;			//TO SET THE BAUD RATE AS 9600
	SPBRG=0XE1;				//TO SET THE BAUD RATE AS 9600
	TXSTA=0X24;				//CONFIGURATION OF TXSTA
	RCSTA=0X90;				//CONFIGURATION OF RCSTA
	BAUDCON=0X08;			//CONFIGURATION OF BAUD RATE	
 while(1)
	{
		msdelay(500);
		while(PORTDbits.RD0==1)
		{
			objectdetect();
		}
	{
		TXstring(string1);
	for(i=0;i<9;i++)
	{
		latitude[i]=rx_data();
	}
	latitude[9]=0;
	TXstring(string2);
	for(i=0;i<9;i++)
	{
		longitude[i]=rx_data();
	}
	longitude[9]=0;
	currentlat=convert(latitude);
	currentlong=convert(longitude);
	for(i=0;i<5;i++)
	{
		diflat[i]=currentlat-lat[i];
		diflong[i]=currentlong-longi[i];
	}
	check=nearway(diflat,diflong);
	if(check==6)  //Not near a waypoint
	{	
			devlat=currentlat-lat[prev];
			devlong=currentlong-longi[prev];
          	chdg=atan(devlong/devlat)*(180/pi);
			dhdg=chdg-ghdg[prev];
			if(dhdg<0)
			{
				dhdg=dhdg+360;
			}
			if(dhdg>20&&dhdg<70) //slight right
			{
				TXstring(string7);
				PORTBbits.RB2=0;
				msdelay(2000);
				PORTBbits.RB2=1;
			}
			else if(dhdg>70&&dhdg<110) //right 
			{
				TXstring(string5);
				PORTBbits.RB3=0;
				msdelay(2000);
				PORTBbits.RB3=1; 
			}
			else if(dhdg>110&&dhdg<150) //hard right 
			{
				TXstring(string9); 
				PORTBbits.RB4=0;
				msdelay(2000);
				PORTBbits.RB4=1;
			}
			else if(dhdg>150&&dhdg<220)  //Turn back
			{
				TXstring(string4);
				PORTBbits.RB1=0;
				msdelay(2000);
				PORTBbits.RB1=1;
			}
			else if(dhdg>220&&dhdg<250)  //Hard left
			{
				TXstring(string10);
				PORTBbits.RB7=0;
				msdelay(2000);
				PORTBbits.RB7=1;
			}
			else if(dhdg>250&&dhdg<290)  //Left
			{
				TXstring(string6);
				PORTBbits.RB6=0;
				msdelay(2000);
				PORTBbits.RB6=1;
			}
			else if(dhdg>290&&dhdg<330)//Slight left
			{
				TXstring(string8);
				PORTBbits.RB5=0;
				msdelay(2000);
				PORTBbits.RB5=1;
			}
			
			else //Forward
			{
				TXstring(string3);
				PORTBbits.RB0=0;
				msdelay(2000);
				PORTBbits.RB0=1;
			}
	}
	else
	{
		prev=check;
		if(check==0)
		{
			TXstring(string3);
			PORTBbits.RB0=0;
			msdelay(2000);
			PORTBbits.RB0=1;
		}
		else if(check==1)
		{
			TXstring(string7);
			PORTBbits.RB2=0;
			msdelay(2000);
			PORTBbits.RB2=1;
		}
		else if(check==2)
		{
			TXstring(string3);
			PORTBbits.RB0=0;
			msdelay(2000);
			PORTBbits.RB0=1;
		}
		else if(check==3)
		{
			TXstring(string3);
			PORTBbits.RB0=0;
			msdelay(2000);
			PORTBbits.RB0=1;
		}
		else
		{
			TXstring(string11);
		}
	}
}
	}
}
//Function Definitions
void msdelay (unsigned int time) 		//Function to generate delay
{
	unsigned int i, j;
	for (i = 0; i < time; i++)
		for (j = 0; j < 710; j++);		//Calibrated for a 1 ms delay in MPLAB
}
void delay(void)		//for 10usec delay using timer
{
	T0CONbits.TMR0ON=1;
	while(INTCONbits.TMR0IF==0);
	T0CONbits.TMR0ON=0;
}
void objectdetect()
{
	T0CON=0b0001000;
	TMR0H=0XFF;
	TMR0L=0X88;
	T1CON=0x30;
	TMR1H=0X00;
	TMR1L=0X00;
	PR2=0xBA;          // set PWM Frequency 4KHz  
    CCP1CON=0X0C;        // Configure CCP1CON as PWM mode.
   	T2CON=0X07;      //Start timer 2 with prescaler 1:16
	PORTCbits.RC0=1;
	delay();
	INTCONbits.TMR0IF=0;
	PORTCbits.RC0=0;
	while(PORTCbits.RC1==0);
	T1CONbits.TMR1ON=1;
	while(PORTCbits.RC1==1);
	T1CONbits.TMR1ON=0;
	a=(unsigned int)(TMR1L + (256*TMR1H));
	a=a/88.03;
	out_num=a+1;
	if(out_num<=50)
	{
			PORTAbits.RA5=1;    // Vcc of driver
     		PORTAbits.RA4=0;    // ground for driver
			CCPR1L=0X96;	// ----------Duty Cycle 80%-----------
			CCP1CONbits.DC1B1=0; 
			CCP1CONbits.DC1B0=0; 
			msdelay(3000);
	     	PORTAbits.RA5=0;    // Vcc of driver
	}
	else if(out_num>50&out_num<100)
	{
			PORTAbits.RA5=1;    // Vcc of driver
     		PORTAbits.RA4=0;    // ground for driver
		   CCPR1L=0X4B;	// ----------Duty Cycle 40%-----------
			CCP1CONbits.DC1B1=0; 
			CCP1CONbits.DC1B0=0;
	     	msdelay(3000);
	     	PORTAbits.RA5=0;    // Vcc of driver	
	}
	else
	{
		PORTAbits.RA5=0;    // Vcc of driver
     	PORTAbits.RA4=0;    // ground for driver
	}
}
float convert(char a[])
{
     float deg,min,sec;
     deg = (a[0]-'0')*10 + (a[1]-'0');
     min = (a[3]-'0')/10.000000 + (a[4]-'0')/100.000000 + (a[5]-'0')/1000.000000 ;
     sec = (a[6]-'0')/10000.000000 + (a[7]-'0')/100000.000000 + (a[8]-'0')/1000000.000000 ;

    return deg+min+sec;

}
int nearway(float diflat[],float diflong[])
{
	int i,check;	
	for(i=0;i<5;i++)
	{
		if(diflat[i]>-0.000010&&diflat[i]<0.000010&&diflong[i]>-0.000010&&diflong[i]<0.000010)
		{
			check=i;
			break;
		}
		else
		{
			check=6;
		}			
	}
	return(check); 
}
void TXstring(unsigned char *string)
{
	unsigned char i=0;
	for(i=0;string[i]!='\0';i++)
	{
		TXbyte(string[i]);
	}
}	
void TXbyte(unsigned char data)
{
	while(TXSTAbits.TRMT==0);
	TXREG=data;
}	  
unsigned char rx_data(void)
{
    while(PIR1bits.RCIF==0);
	return(RCREG);
}