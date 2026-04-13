//continuously read data from temp, pot and gas sensor
//adjust sensors with pot
//trigger alarms
//activate buzzer
#include "mbed.h"

// DigitalOut buzzer(PE_10);
// DigitalIn gasSensor(PE_12);
// AnalogIn pot(A0);
// AnalogIn tempSensor(A1);

//Function to read input from keyboard & extra bits
UnbufferedSerial uartUsb(USBTX, USBRX, 115200);
char receivedChar = '\0';
int warningActive = 0;

void readChar() {
    if (uartUsb.readable()) {
        uartUsb.read(&receivedChar, 1);
    }
}

const string POT_NAME = "POTENTIOMETER";

//Function to continuously read and print data from sensor
void readSensor(PinName pin, const string &sensorName) {
    AnalogIn sensor(pin);
    float sensorVal = sensor.read();
    printf("%s raw sensor value: %f \n", sensorName.c_str(), sensorVal);
    ThisThread::sleep_for(2000ms);
}

int main() {
    while (true) {
        readSensor(A0, POT_NAME);
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
