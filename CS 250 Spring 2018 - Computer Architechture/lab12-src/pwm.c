#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

/*You can set Pin numbers here*/
#define RED_BUTTON		0
#define GREEN_BUTTON	2
#define BLUE_BUTTON		3
#define INC_BUTTON		4
#define DEC_BUTTON		5
#define RED_LED			27
#define GREEN_LED		28
#define BLUE_LED		29

int main(void) {
	
	//initialize GPIO pins
	if(wiringPiSetup() == -1){
		return 1;
	}
	
	//set pins to input for buttons
	pinMode(RED_BUTTON, INPUT);
	pinMode(GREEN_BUTTON, INPUT);
	pinMode(BLUE_BUTTON, INPUT);
	pinMode(INC_BUTTON, INPUT);
	pinMode(DEC_BUTTON, INPUT);
	//set pins to output for LEDs
	pinMode(RED_LED, OUTPUT);
	pinMode(GREEN_LED, OUTPUT);
	pinMode(BLUE_LED, OUTPUT);
	
	//set values of the buttons to low
	int rVal = LOW;
	int gVal = LOW;
	int bVal = LOW;
	int iVal = LOW;
	int dVal = LOW;

	//set led values to have a max of 100 and to start at 0
	int rLEDVal = softPwmCreate(RED_LED, 0, 100);
	int gLEDVal = softPwmCreate(GREEN_LED, 0, 100);
	int bLEDVal = softPwmCreate(BLUE_LED, 0, 100);

	while(1){
	
		//find if button is pressed
		rVal = digitalRead(RED_BUTTON);
		gVal = digitalRead(GREEN_BUTTON);
		bVal = digitalRead(BLUE_BUTTON);
		iVal = digitalRead(INC_BUTTON);
		dVal = digitalRead(DEC_BUTTON);

		//if redbutton is pressed and increased buttton is pressed
		//increase red value by 5
		if(rVal && iVal){
			if(rLEDVal != 100){ //make sure value is not at max
				rLEDVal += 5;
				softPwmWrite(RED_LED,rLEDVal);
			}
		}
		//if green button pressed and increase button pressed
		//increase green value by 5
		if(gVal && iVal){
			if(gLEDVal != 100){	//make sure value is not at max
				gLEDVal += 5;
				softPwmWrite(GREEN_LED,gLEDVal);
			}
		}
		//if blue button pressed and increase button pressed
		//increase blue by 5
		if(bVal && iVal){
			if(bLEDVal != 100){ //make sure value is not at max
				bLEDVal += 5;
				softPwmWrite(BLUE_LED,bLEDVal);
			}
		}
		//if red button pressed and decrease button pressed
		//decrease red by 5
		if(rVal && dVal){
			if(rLEDVal != 0){ //make sure value is not at min
				rLEDVal -= 5;
				softPwmWrite(RED_LED,rLEDVal);
			}
		}
		//if green button pressed and decrease button pressed
		//decrease green by 5
		if(gVal && dVal){
			if(gLEDVal != 0){ //make sure value is not at min
				gLEDVal -= 5;
				softPwmWrite(GREEN_LED,gLEDVal);
			}
		}
		//if blue button pressed and decrease button pressed
		//decrease blue by 5
		if(bVal && dVal){
			if(bLEDVal != 0){ //make sure value is not at min
				bLEDVal -= 5;
				softPwmWrite(BLUE_LED,bLEDVal);
			}
		}
		//delay for button lag
		delay(100);
	}
	return 0;
}
