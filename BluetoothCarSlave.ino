#include<SoftwareSerial.h>

char command;

void setup()
{
    Serial.begin(9600);
    Serial.println("This is the slave");
    pinMode(13, OUTPUT); // left motor forward
    pinMode(12, OUTPUT); // left motor reverse
    pinMode(11, OUTPUT); // right motor forward
    pinMode(10, OUTPUT); // right motor reverse
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    analogWrite(5, 150);
    analogWrite(6, 150);
}

void loop()
{
    if (Serial.available() > 0)
    {
        command = Serial.read();
    }

    //Serial.println((char)command);
    switch (command)
    {
        case 'F':
            //            Serial.println("GoingForward");
            digitalWrite(13, HIGH);
            digitalWrite(11, HIGH);
            break;

        case 'B':
            //            Serial.println("GoingBack");
            digitalWrite(12, HIGH);
            digitalWrite(10, HIGH);
            break;

        case 'R':
            //            Serial.println("GoingRight");
            digitalWrite(12, HIGH);
            digitalWrite(11, HIGH);
            break;

        case 'L':
            //            Serial.println("GoingLeft");
            digitalWrite(10, HIGH);
            digitalWrite(13, HIGH);
            break;

        case 'S':
            //            Serial.println("Stop");
            digitalWrite(13, LOW);
            digitalWrite(12, LOW);
            digitalWrite(11, LOW);
            digitalWrite(10, LOW);
            break;



        default:
            break;
    }
}
