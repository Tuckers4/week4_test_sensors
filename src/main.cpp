//continuously read data from temp, pot and gas sensor
//adjust sensors with pot
//trigger alarms
//activate buzzer
#include "mbed.h"

DigitalOut buzzer(PE_10);
AnalogIn gas(A2);
AnalogIn pot(A0);
AnalogIn temp(A1);

//Function to read input from keyboard & extra bits
UnbufferedSerial uartUsb(USBTX, USBRX, 115200);
char receivedChar = '\0';
bool alarmActive = 0;

void readChar() {
    if (uartUsb.readable()) {
        uartUsb.read(&receivedChar, 1);
    }
}

const string POT_NAME = "POTENTIOMETER";
const string TEMP_NAME = "TEMPERATURE";
const string GAS_NAME = "GAS";
const string TEMP_WARNING = "TEMPERATURE WARNING";
const string GAS_WARNING = "GAS WARNING";
const string SYSTEM_NORMAL = "ALL SYSTEMS NORMAL";

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

void systemNormal() {
    if (alarmActive == 0) {
        printf("%s \n",SYSTEM_NORMAL.c_str());
    }
}

//Function to evaluate sensor data
void evaluateData(const string &sensorName, PinName pin) {
    int thresholdVal = threshold(sensorName);
    float sensorVal = readSensor(pin, sensorName);
    if (sensorName == TEMP_NAME) {
        int celcius = sensorVal * 330; //converts sensor value to degrees Celsius
        printf("Temp value: %d degrees Celsius \nThreshold value: %d degrees celcius\n", celcius,
               thresholdVal); //testing
        if (celcius > thresholdVal) {
            printf("%s ALARM SOUNDING \n", TEMP_WARNING.c_str()); //testing
            alarmActive = 1;
        }
    }

    if (sensorName == GAS_NAME) {
        int ppmThreshold = thresholdVal * 8;
        int ppm = sensorVal * 1700; //roughly converts value to ppm
        printf("gas level: %d ppm \nThreshold value: %d ppm \n", ppm, ppmThreshold); //testing
        if (ppm > ppmThreshold) {
            printf("%s ALARM SOUNDING \n", GAS_WARNING.c_str());
            alarmActive = 1;
        }
    }
    buzzerState();
    systemNormal();
}



int main() {
    while (true) {
        alarmActive = 0;
        evaluateData(TEMP_NAME, A1);
        evaluateData(GAS_NAME,A2);
        ThisThread::sleep_for(3000ms);
    }
}


//         //reads from 0V to 3.3V (0-1 on analogue pin) * analogue reading by 100 to get value from 0-100
//         int potVal = pot.read() * 100;
//         //gas sensor is digital so always either 1 or 0
//         bool gasSensorVal = gasSensor.read();
//         //convert pin value to temp in celcius
//         float tempVal = tempSensor.read() * 330;
//
//         //print values of all sensors
//         printf(
//             "\n potentiometer position (Threshold) : %d \n Gas sensor value : %d \n Temperature sensor value : %f degrees \n ",
//             potVal, gasSensorVal, tempVal);
//         buzzer = 0;
//
//         if (tempVal > potVal || gasSensorVal == 0) {
//             printf("\n SYSTEM WARNING\n ");
//             buzzer = 1;
//             if (tempVal > potVal) {
//                 printf("\n WARNING OVER TEMPERATURE DETECTED! \n");
//             }
//             if (gasSensorVal == 0) {
//                 printf("\n WARNING GAS DETECTED! \n");
//                 buzzer = 1;
//             } else if (tempVal < potVal && gasSensorVal == 1) {
//                 printf("\n SYSTEM NORMAL \n ");
//             }
//         }
//         ThisThread::sleep_for(2000);
//     }
// }
