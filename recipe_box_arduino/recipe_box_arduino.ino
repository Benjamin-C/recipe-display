#include "Arduino.h"
//The setup function is called once at startup of the sketch

#include "main.h"

void setup() {
	Serial.begin(115200);
	Serial.println("Hi");

	printf("Does this work too?\n");

	pinMode(33, OUTPUT);
	digitalWrite(33, HIGH);

	main();
// Add your initialization code here
}

// The loop function is called in an endless loop



void loop() {
//Add your repeated code here
}
