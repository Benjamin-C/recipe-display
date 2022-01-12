/* arduinostandin.cpp
 *
 * Creates standin versions of several Arduino functions that you may want to use.
 *
 * Author: Benjamin Crall
 */

#include "arduinostandin.h"

#include <stdio.h>
#include <chrono>
#include <thread>
#include <stdlib.h>
#include <stdarg.h>

SerialClass Serial;

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void delay(int ms) {
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

unsigned long millis(void) {
	std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
			std::chrono::system_clock::now().time_since_epoch()
	);
	return ms.count();
}

unsigned long micros(void) {
	std::chrono::microseconds ms = std::chrono::duration_cast< std::chrono::microseconds >(
			std::chrono::system_clock::now().time_since_epoch()
	);
	return ms.count();
}

void pinMode(int a, int b) {
	// Does nothing here
}

void digitalWrite(int a, int b) {
	// Does nothing here
}

int digitalRead(int a) {
	return 0;
}

int random(int max) {
	return rand() % max;
}

void SerialClass::print(const char* msg) {
	printf("%s", msg);
}

void SerialClass::println(const char* msg) {
	printf("%s\n", msg);
}

int SerialClass::available(void) {
	return 0;
}

char SerialClass::read(void) {
	return 0;
}

int SerialClass::printf(const char* format, ...) {
	va_list arg;
	int done;

	va_start (arg, format);
	done = vfprintf (stdout, format, arg);
	va_end (arg);

	return done;
}
