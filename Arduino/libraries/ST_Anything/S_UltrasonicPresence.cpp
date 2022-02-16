//******************************************************************************************
//  File: S_UltrasonicPresence.cpp
//  Authors: Benjamin Kraus
//
//  Summary:  S_UltrasonicPresence is a class which implements a "Presence Sensor" device capability using an ultrasonic distance sensor.
//              It inherits from the st::Sensor class.
//
//  Change History:
//
//    Date        Who             What
//    ----        ---             ----
//    2022-02-14  Benjamin Kraus  Original Creation
//
//******************************************************************************************

#include "S_UltrasonicPresence.h"
#include "Everything.h"

namespace st
{
//private
    void S_UltrasonicPresence::updateStatus() {
        unsigned long now = millis();
        if (now < m_nPreviousTime || (now - m_nPreviousTime) >= m_nInterval) {
            // Clear the trigger pin
            digitalWrite(m_nDigitalTriggerPin, LOW);
            delayMicroseconds(2);

            // Set the trigger pin HIGH for 10 microseconds
            digitalWrite(m_nDigitalTriggerPin, HIGH);
            delayMicroseconds(10);
            digitalWrite(m_nDigitalTriggerPin, LOW);

            // Read the incoming pulse on the echo pin
            unsigned long duration = pulseIn(m_nDigitalEchoPin, HIGH);

            // Record the time of the last sample.
            m_nPreviousTime = millis();

            // Check if the reading is within a valid range.
            if (duration > m_nMaxReading) {
                // Invalid reading.
                return;
            }

            /*
             * If m_bPresent == true and m_bLongIsPresent == true, then the
             * last reading was long, so check for a short reading.
             *
             * If m_bPresent == false and m_bLongIsPresent == false, then the
             * last reading was long, so check for a short reading.
             *
             * If m_bPresent == true and m_bLongIsPresent == false, then the
             * last reading was short, so check for a long reading.
             *
             * If m_bPresent == false and m_bLongIsPresent == true, then the
             * last reading was short, so check for a long reading.
             */
            bool checkForLong = (m_bPresent != m_bLongIsPresent);

            bool lastPresentState = m_bPresent;
            if (checkForLong && duration >= m_nLongThreshold ||
                !checkForLong && duration <= m_nShortThreshold) {
                m_nCurrentCount++;
                if (m_nCurrentCount >= m_nRequiredCounts) {
                    m_bPresent = !m_bPresent;
                    m_nCurrentCount = 0;
                }
            } else {
                m_nCurrentCount = 0;
            }

            if (lastPresentState != m_bPresent) {
                sendUpdate();
            }
        }
    }

    void S_UltrasonicPresence::sendUpdate() {
        Everything::sendSmartString(getName() + (m_bPresent ? F(" present") : F(" notpresent")));
    }

//public
    //constructor - called in your sketch's global variable declaration section
    S_UltrasonicPresence::S_UltrasonicPresence(const __FlashStringHelper *name,
                byte digitalTriggerPin, byte digitalEchoPin,
                bool initalState, bool longIsPresent,
                unsigned long longThreshold, unsigned long shortThreshold):
        Sensor(name),
        m_bLongIsPresent(longIsPresent),
        m_nLongThreshold(longThreshold),
        m_nShortThreshold(shortThreshold),
        m_nRequiredCounts(4), // require 4 matching samples
        m_nInterval(1000),    // sample every 1000 microseconds
        m_nMaxReading(23200), // 23200 microsecond pulse (400 cm)
        m_nCurrentCount(0),
        m_nPreviousTime(0),
        m_bPresent(initalState)
    {
        setPin(digitalTriggerPin, digitalEchoPin);
    }

    //destructor
    S_UltrasonicPresence::~S_UltrasonicPresence()
    {

    }

    void S_UltrasonicPresence::init() {
        // Poll for a new sample and send update if needed.
        updateStatus();
    }

    void S_UltrasonicPresence::update() {
        // Poll for a new sample and send update if needed.
        updateStatus();
    }

    void S_UltrasonicPresence::refresh() {
        // Send the current state without polling.
        sendUpdate();
    }

    void S_UltrasonicPresence::setPin(byte &trigPin,byte &echoPin)
    {
        m_nDigitalTriggerPin = trigPin;
        m_nDigitalEchoPin = echoPin;

        pinMode(m_nDigitalTriggerPin, OUTPUT);
        digitalWrite(m_nDigitalTriggerPin, LOW);
        pinMode(m_nDigitalEchoPin, INPUT);
    }
}
