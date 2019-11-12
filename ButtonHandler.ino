// Test program to show how to use the button handler class
// Shows how to use a regular button where we poll the state and one where we use an interrupt function so we don't have to poll.
// If we poll all the time, we can also get a hold down event.
// If we rely on interrupts, we only get things to happen when the button is pressed down or released. We then also need to reset the state 

#include "ButtonHandler.h"

const uint8_t buttonPin0 = 8;
const uint8_t buttonPin1 = 2;

ButtonHandler pollButton(buttonPin0); // can add the pin in the constructor
ButtonHandler interruptButton;    // or leave it to be added later

buttonEnum pollButtonState = buttonEnum::NO_CHANGE;

volatile buttonEnum interruptButtonState = buttonEnum::NO_CHANGE;
volatile bool interruptEventOccurred = false;

void setup(){
    Serial.begin(250000);
    Serial.println("Connected to ButtonHandler.ino");

    pollButton.init(); // does things like set internal pullup high and make it an input pin.

    attachInterrupt(digitalPinToInterrupt(buttonPin1), buttonISR, CHANGE); // makes the pin call an interrupt function. Do this so we don't have to poll the button state.
    interruptButton.init(buttonPin1); // have to assign pin here because we didn't above.
}

void loop(){
    if(interruptEventOccurred){
        interruptEventOccurred = false;
        
        switch(interruptButtonState){
            case buttonEnum::BUTTON_UP:             Serial.println("INT BUTTON_UP");            break;
            case buttonEnum::BUTTON_DOWN:           Serial.println("INT BUTTON_DOWN");          break;
            case buttonEnum::BUTTON_DOUBLECLICK:    Serial.println("INT BUTTON_DOUBLECLICK");   break;
            case buttonEnum::BUTTON_HOLD:           Serial.println("INT BUTTON_HOLD");          break; // will never occur!
        }
    }

    switch(pollButton.poll()){
        case buttonEnum::NO_CHANGE:             break;
        case buttonEnum::BUTTON_UP:             Serial.println("BUTTON_UP");            break;
        case buttonEnum::BUTTON_DOWN:           Serial.println("BUTTON_DOWN");          break;
        case buttonEnum::BUTTON_DOUBLECLICK:    Serial.println("BUTTON_DOUBLECLICK");   break;
        case buttonEnum::BUTTON_HOLD:           Serial.println("BUTTON_HOLD");          break;
    }

}

void buttonISR(){
    buttonEnum tmp = interruptButton.poll();
    
    if(tmp != buttonEnum::NO_CHANGE){
        interruptEventOccurred = true;
        interruptButtonState = tmp;
    }
}