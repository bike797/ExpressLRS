#pragma once

#if defined(PLATFORM_ESP32) && defined(BLE_HID_JOYSTICK)

#include <Arduino.h>
#include "targets.h"
#include <BleGamepad.h>

#if defined(Regulatory_Domain_AU_915) || defined(Regulatory_Domain_EU_868) || defined(Regulatory_Domain_FCC_915) || defined(Regulatory_Domain_AU_433) || defined(Regulatory_Domain_EU_433)
#include "SX127xDriver.h"
extern SX127xDriver Radio;
#endif

#if defined(Regulatory_Domain_ISM_2400)
#include "SX1280Driver.h"
extern SX1280Driver Radio;
#endif

#include "CRSF.h"
extern CRSF crsf;

#define numOfButtons 0
#define numOfHatSwitches 0
#define enableX true
#define enableY true
#define enableZ true
#define enableRZ true
#define enableRX true
#define enableRY true
#define enableSlider1 true
#define enableSlider2 true
#define enableRudder false
#define enableThrottle false
#define enableAccelerator false
#define enableBrake false
#define enableSteering false

BleGamepad bleGamepad("ExpressLRS Joystick", "ELRS", 100);

void BluetoothJoystickBegin()
{
    bleGamepad.setAutoReport(false);
    Serial.println("Starting BLE Joystick!");
    bleGamepad.setControllerType(CONTROLLER_TYPE_GAMEPAD);
    bleGamepad.begin(numOfButtons, numOfHatSwitches, enableX, enableY, enableZ, enableRZ, enableRX, enableRY, enableSlider1, enableSlider2, enableRudder, enableThrottle, enableAccelerator, enableBrake, enableSteering);
}

void BluetoothJoystickSendReport()
{
    if (bleGamepad.isConnected())
    {
        bleGamepad.sendReport();
    }
}

void BluetoothJoystickUpdateValues()
{
    if (bleGamepad.isConnected())
    {
        int16_t data[sizeof(crsf.ChannelDataIn)] = {0};

        for (uint8_t i = 0; i < 9; i++)
        {
            //data[i] = map(crsf.ChannelDataIn[i], CRSF_CHANNEL_VALUE_MIN - 1, CRSF_CHANNEL_VALUE_MAX + 1, -32768, 32768);
            data[i] = map(crsf.ChannelDataIn[i], CRSF_CHANNEL_VALUE_MIN - 1, CRSF_CHANNEL_VALUE_MAX + 1, -1, 32768);
        }

        bleGamepad.setX(data[0]);
        bleGamepad.setY(data[1]);
        bleGamepad.setZ(data[2]);
        bleGamepad.setRX(data[3]);
        bleGamepad.setRY(data[4]);
        bleGamepad.setRZ(data[5]);
        bleGamepad.setSlider1(data[6]);
        bleGamepad.setSlider2(data[7]);
    }
}

#endif