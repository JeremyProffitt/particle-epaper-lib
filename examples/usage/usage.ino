// Example usage for ePaper library by Jeremy Proffitt <proffitt.jeremy@gmail.com>.

#include "ePaper.h"

// Initialize objects from the lib
EPaper ePaper;

void setup() {
    // Call functions on initialized library objects that require hardware
    ePaper.begin();
}

void loop() {
    // Use the library's initialized objects and functions
    ePaper.process();
}
