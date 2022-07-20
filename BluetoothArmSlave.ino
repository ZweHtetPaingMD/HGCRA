#include<Servo.h>

Servo armServo;
Servo elbowServo;
Servo gripperServo;
Servo baseServo;

String raw;
int firstCommaIndex;
int secondCommaIndex;

String gripPos;
String elboArmPos;
String basePos;

int gripPosInt;
int elboArmPosInt;
int elboPosInt;
int armPosInt;
int basePosInt;

void control();

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    Serial.setTimeout(35);
    armServo.attach(9);
    elbowServo.attach(6);
    gripperServo.attach(11);
    baseServo.attach(3);
}

void loop() {
    control();
}


void control()
{
    if (Serial.available())
    {
        raw = Serial.readString();
    }

    firstCommaIndex = raw.indexOf(',');
    secondCommaIndex = raw.indexOf(',', firstCommaIndex + 1);

    gripPos = raw.substring(0, firstCommaIndex);
    elboArmPos = raw.substring(firstCommaIndex + 1, secondCommaIndex);
    basePos = raw.substring(secondCommaIndex + 1);

    gripPosInt = gripPos.toInt();
    elboArmPosInt = elboArmPos.toInt();
    basePosInt = basePos.toInt();

    gripPosInt = map(gripPosInt, 830, 860, 90, 0);
    gripPosInt = constrain(gripPosInt, 0, 90);

    armPosInt = map(elboArmPosInt, -25, 25, 90, 180);
    armPosInt = constrain(armPosInt, 90, 180);

    elboPosInt = map(elboArmPosInt, -25, 25, 30, 60);
    elboPosInt = constrain(elboPosInt, 30, 60);

    basePosInt = map(basePosInt, -20, 20, 20, 140);
    basePosInt = constrain(basePosInt, 20, 140);

    if (gripPosInt == 90)
    {
        gripperServo.write(90);
    }
    else if (gripPosInt == 0)
    {
        gripperServo.write(0);
    }

    if (elboPosInt != 0 && armPosInt != 0 && basePosInt != 0)
    {
        elbowServo.write(elboPosInt);
        armServo.write(armPosInt);
        baseServo.write(basePosInt);
    }


    Serial.print(gripPosInt);
    Serial.print('\t');
    Serial.print(elboPosInt);
    Serial.print('\t');
    Serial.print(armPosInt);
    Serial.print('\t');
    Serial.println(basePosInt);
}
