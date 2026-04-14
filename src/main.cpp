#include "mbed.h"

//initialise pins
DigitalOut buzzer(PE_10);
AnalogIn gas(A2);
AnalogIn pot(A0);
AnalogIn temp(A1);

//constant strings used in printing to terminal
const string POT_NAME = "POTENTIOMETER";
const string TEMP_NAME = "TEMPERATURE";
const string GAS_NAME = "GAS";
const string TEMP_WARNING = "TEMPERATURE WARNING";
const string GAS_WARNING = "GAS WARNING";
const string SYSTEM_NORMAL = "ALL SYSTEMS NORMAL";

//alarm state
bool alarmActive = 0;

//Function to continuously read and print data from sensor
float readSensor(PinName pin, const string &sensorName) {
    AnalogIn sensor(pin);
    float sensorVal = sensor.read();
    //printf("%s raw sensor value: %f \n", sensorName.c_str(), sensorVal); //only used to test functionality
    return sensorVal;
}

//Function to set the threshold of a sensor
float threshold(const string &sensorName) {
    int SensorThreshold = readSensor(A0, POT_NAME) * 100;
    //printf("%s sensor threshold: %d \n", sensorName.c_str(), SensorThreshold); //testing
    return SensorThreshold;
}

//function to control the buzzer
void buzzerState() {
    buzzer = alarmActive;
}

//function to print 'all clear' if no alarms are active
void systemNormal() {
    if (alarmActive == 0) {
        printf("%s \n", SYSTEM_NORMAL.c_str());
    }
}

//Function to evaluate sensor data
void evaluateData(const string &sensorName, PinName pin) {
    int thresholdVal = threshold(sensorName);
    float sensorVal = readSensor(pin, sensorName);
    if (sensorName == TEMP_NAME) {
        int celcius = sensorVal * 330; //converts sensor value to degrees Celsius
        //printf("Temp value: %d degrees Celsius \nThreshold value: %d degrees celcius\n", celcius,
        //    thresholdVal); //testing
        if (celcius > thresholdVal) {
            printf("%s ALARM SOUNDING \n", TEMP_WARNING.c_str()); //testing
            alarmActive = 1;
        }
    }
    if (sensorName == GAS_NAME) {
        int ppmThreshold = thresholdVal * 8;
        int ppm = sensorVal * 1700; //roughly converts value to ppm
        //printf("gas level: %d ppm \nThreshold value: %d ppm \n", ppm, ppmThreshold); //testing
        if (ppm > ppmThreshold) {
            printf("%s ALARM SOUNDING \n", GAS_WARNING.c_str());
            alarmActive = 1;
        }
    }
    buzzerState();
}

int main() {
    while (true) {
        alarmActive = 0;
        evaluateData(TEMP_NAME, A1);
        evaluateData(GAS_NAME,A2);
        systemNormal();
        ThisThread::sleep_for(3000ms);
    }
}
