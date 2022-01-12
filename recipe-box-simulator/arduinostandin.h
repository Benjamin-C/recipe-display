#ifndef ARDUINO_STANDIN_H
#define ARDUINO_STANDIN_H

#define INPUT 0
#define OUTPUT 0
#define HIGH 0
#define LOW 0
#define LED_BUILTIN 0

long map(long x, long in_min, long in_max, long out_min, long out_max);
void delay(int ms);
unsigned long millis(void);
unsigned long micros(void);
int random(int max);
void pinMode(int a, int b);
void digitalWrite(int a, int b);
int digitalRead(int a);

class SerialClass {
public:
	void print(const char* msg);
	void println(const char* msg);
	int printf(const char* format, ...);
	int available(void);
	char read(void);
};

extern SerialClass Serial;

#endif
