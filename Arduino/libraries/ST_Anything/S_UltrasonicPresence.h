//******************************************************************************************
//  File: S_UltrasonicPresence.h
//  Authors: Benjamin Kraus
//
//  Summary:  S_UltrasonicPresence is a class which implements a "Presence Sensor" device capability using an ultrasonic distance sensor.
//              It inherits from the st::Sensor class.
//
//              Create an instance of this class in your sketch's global variable section
//              For Example:
//                  bool initalState = false; // Start off not-present
//                  bool longIsPresent = false; // Long distance means not-present
//                  unsigned long longThreshold = 10656; // 6 feet = 6*12*148 = 10656
//                  unsigned long shortThreshold = 7104; // 4 feet = 4*12*148 = 7104
//                  static st::S_UltrasonicPresence sensor1(F("presence1"),
//                      PIN_ULTRASONIC_TRIG, PIN_ULTRASONIC_ECHO,
//                      initalState, longIsPresent, longThreshold, shortThreshold);
//
//              st::S_UltrasonicPresence() constructor requires the following arguments
//                - String &name - REQUIRED - the name of the object - must start with "presence".
//                - byte digitalTriggerPin - REQUIRED - the Arduino Pin to be used as a digital output to trigger ultrasonic
//                - byte digitalEchoPin - REQUIRED - the Arduino Pin to be used as a digital input to read the echo
//                - bool initalState - REQUIRED - the initial state of the presence detector (true = present)
//                - bool longIsPresent - REQUIRED - the method to convert from distance to presence
//                                                  true mean a long distance is considered present
//                                                  false means a short distance is considered present
//                - unsigned long longThreshold - REQUIRED - the threshold in microseconds considered a "long distance"
//                - unsigned long shortThreshold - REQUIRED - the threshold in microseconds considered a "short distance"
//
//              Calculating the threshold
//                The threshold is based on the amount of time it takes for the ultrasonic signal to leave the transmitter,
//                bounce off the target, and return to the receiver, which is based on the speed of sound.
//                From the HC-SR04 documentation, to convert from centimeters or inches to microseconds, use these equations:
//                    time (uS) = distance from sensor (cm) * 58 uS/cm
//                    time (uS) = distance from sensor (inches) * 148 uS/inch
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
