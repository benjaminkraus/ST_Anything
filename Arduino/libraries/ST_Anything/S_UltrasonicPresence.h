//******************************************************************************************
//  File: S_UltrasonicPresence.h
//  Authors: Benjamin Kraus
//
//  Summary:  S_UltrasonicPresence is a class which implements a "Presence Sensor" device capability using an ultrasonic distance sensor.
//              It inherits from the st::Sensor class.
//
//              Create an instance of this class in your sketch's global variable section
//              For Example:  st::S_UltrasonicPresence sensor1(F("presence1"), PIN_ULTRASONIC_T, PIN_ULTRASONIC_E);
//
//              st::S_UltrasonicPresence() constructor requires the following arguments
//                - String &name - REQUIRED - the name of the object - must start with "presence".
//                - byte digitalTriggerPin - REQUIRED - the Arduino Pin to be used as a digital output to trigger ultrasonic
//                - byte digitalEchoPin - REQUIRED - the Arduino Pin to be used as a digital input to read the echo
//
//
//  Change History:
//
//    Date        Who             What
//    ----        ---             ----
//    2022-02-14  Benjamin Kraus  Original Creation
//
//******************************************************************************************

#ifndef ST_S_UltrasonicPresence_H
#define ST_S_UltrasonicPresence_H

#include "Sensor.h"

namespace st
{
    class S_UltrasonicPresence : public Sensor
    {
        private:
            // high-level configurable settings
            byte m_nDigitalTriggerPin;        // Ultrasonic sensor trigger pin
            byte m_nDigitalEchoPin;           // Ultrasonic sensor echo pin
            bool m_bLongIsPresent;            // true = long is present, false = long is not-present
            unsigned long m_nLongThreshold;   // Reading above which is considered "long"
            unsigned long m_nShortThreshold;  // Reading below which is considered "short"

            // low-level configurable settings
            uint8_t m_nRequiredCounts;      // samples required before detecting
            unsigned long m_nInterval;      // milliseconds between polling
            unsigned long m_nMaxReading;    // Maximum valid reading

            // state variables
            uint8_t m_nCurrentCount;        // number of samples in the current state
            unsigned long m_nPreviousTime;  // milliseconds - time of last poll
            bool m_bPresent;                // true = present, false = not-present

            void updateStatus();
            void sendUpdate();

        public:
            //constructor
            S_UltrasonicPresence(const __FlashStringHelper *name,
                byte digitalTriggerPin, byte digitalEchoPin,
                bool initalState, bool longIsPresent,
                unsigned long longThreshold, unsigned long shortThreshold);

            //destructor
            virtual ~S_UltrasonicPresence();

            //initialization function
            virtual void init();

            //update function
            virtual void update();

            //called periodically by Everything class to ensure ST Cloud is kept consistent with the state of the presence sensor
            virtual void refresh();

            //sets
            void setPin(byte &trigPin, byte &echoPin);
    };
}

#endif
