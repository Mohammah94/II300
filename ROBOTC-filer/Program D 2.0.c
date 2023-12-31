#pragma config(Sensor, S1,     leftTouch,      sensorEV3_Touch)
#pragma config(Sensor, S2,     gyroSensor,     sensorEV3_Gyro, modeEV3Gyro_RateAndAngle)
#pragma config(Sensor, S3,     rightTouch,     sensorEV3_Touch)
#pragma config(Sensor, S4,     sonarSensor,    sensorEV3_Ultrasonic)
#pragma config(Motor,  motorA,          armMotor,      tmotorEV3_Large, openLoop, encoder)
#pragma config(Motor,  motorB,          leftMotor,     tmotorEV3_Large, openLoop, driveLeft, encoder)
#pragma config(Motor,  motorC,          rightMotor,    tmotorEV3_Large, openLoop, driveRight, encoder)
#pragma config(Motor,  motorD,           ,             tmotorEV3_Large, openLoop, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

void forward(int a);
void reverse(int a);
void right_turn(int b);
void left_turn(int b);
void leave_package();
void touch_wall();
float distance[180];
float distance_control[180];
void lokalisering();

task main()
{
	lokalisering();
	sleep(500);
	touch_wall();
	sleep(500);
	reverse(20);
	sleep(500);
	left_turn(180);
	sleep(500);
	touch_wall();
	sleep(500);
	reverse(40);
	sleep(500);
	left_turn(90);
	sleep(500);
	forward(250);
	sleep(500);
	right_turn(90);
	sleep(500);
	touch_wall();
	sleep(500);
	reverse(5);
	sleep(500);
//Leave package and reverse function
	leave_package();
	sleep(500);
	right_turn(90);
	sleep(500);
	forward(250);
	sleep(500);
	right_turn(90);
	sleep(500);
	touch_wall();
	reverse(40);

}

void forward(int a)
{
	float target_dist = (a / (3*PI)) * 360;
		resetMotorEncoder(leftMotor);
	resetMotorEncoder(rightMotor);

	setMotorSyncEncoder(motorB, motorC, 0, target_dist, 80);

	waitUntilMotorStop(motorB);
	waitUntilMotorStop(motorC);
}

void reverse(int a)
{
	float target_dist = (a / (3*PI)) * 360;
		resetMotorEncoder(leftMotor);
	resetMotorEncoder(rightMotor);

	setMotorSyncEncoder(motorB, motorC, 0, target_dist, -20);

	waitUntilMotorStop(motorB);
	waitUntilMotorStop(motorC);
}

void right_turn(int b)
{
	float circ_Axel = 11.7;
	float circ_Wheel = 3.0;
	float v = b;
	float target_deg = (circ_Axel/circ_Wheel) * v;

	resetMotorEncoder(leftMotor);
	resetMotorEncoder(rightMotor);
	sleep(1000);
	setMotorSyncEncoder(motorB, motorC, 100, target_deg, 10);

	waitUntilMotorStop(motorB);
	waitUntilMotorStop(motorC);
}

void left_turn(int b)
{
	float circ_Axel = 11.7;
	float circ_Wheel = 3.0;
	float v = b;

	float target_deg = (circ_Axel/circ_Wheel) * v;

	resetMotorEncoder(leftMotor);
	resetMotorEncoder(rightMotor);
	sleep(1000);
	setMotorSyncEncoder(motorB, motorC, -100, target_deg, 10);

	waitUntilMotorStop(motorB);
	waitUntilMotorStop(motorC);
}

void leave_package()
{
		resetMotorEncoder(armMotor);
		resetMotorEncoder(motorB);
		float target_dist = (-1*(35 / (3*PI)) * 360);
	do {
		setMotorSync(motorB,motorC,0,-20);
		setMotorSpeed(armMotor,40);
	}
	while (getMotorEncoder(motorB) > target_dist);

	setMotorSpeed(motorB, 0);
	setMotorSpeed(motorC, 0);
	setMotorSpeed(armMotor,0);
	sleep(500);
}

void touch_wall()
{
	do{
		setMotorSync(leftMotor, rightMotor, 0, 50);
	}
	while ((getTouchValue(rightTouch)==0)||(getTouchValue(leftTouch)==0));
}

void lokalisering(void)
{
	//int control;
	int copy_count;
	int index_count;
	int n = 0;
	int i;
	int j;
	float min_distance;
	int s = 5;
	float circ_Axel = 11.7;
	float circ_Wheel = 3;
	float target_rotation;
	//float dist = 10;
	//float v = 90;
	//float deg_tot = (circ_Axel/circ_Wheel) * 360;
	//float target_dist = (dist / (circ_Wheel*PI)) * 360;
	//float target_deg = (circ_Axel/circ_Wheel) * v;
	float full_rotation = (circ_Axel/circ_Wheel) * 360;

	resetMotorEncoder(motorB);
	resetMotorEncoder(motorC);
	clearDebugStream();
	//M�ter avst�nd
	do
	{
		setMotorSync(motorB, motorC, 100, 10);
		distance[n] = getUSDistance(S4);
		n = ((getMotorEncoder(motorB)) / (circ_Axel/circ_Wheel))/2;
	}
	while (getMotorEncoder(motorB) < full_rotation);
	setMotorSpeed(motorB,0);
	setMotorSpeed(motorC,0);
	sleep(1000);
	for(j = 0; j < 180; j++)
	{
		writeDebugStreamLine("distance[%d]=%.2f", j, distance[j]);
	}
	//Kopierar arrays
	for( copy_count = 0; copy_count < 180; ++copy_count)
	{
		distance_control[copy_count] = distance[copy_count];
  }  //L�gger det minsta v�rdet f�rst i array
	for( i = 0; i < 180; ++i)
	{
		if (distance[0] > distance[i])
		{
			distance[0] = distance[i];
		}
	}

	//Tilldelar minsta v�rdet till min_distance
	min_distance = distance[0];

	//Letar upp minsta v�rdets plats i array
	for(index_count = 0; index_count < 180; ++index_count)
		if(distance_control[index_count] == min_distance)
		{
			break;
		}
	writeDebugStreamLine("Minsta avst�ndet �r %.2f och det har index %d", min_distance, index_count);
	target_rotation = (circ_Axel/circ_Wheel) * (index_count + 1);
	writeDebugStreamLine("Target_rotation = %.2f", target_rotation);
	//V�nder sig mot minsta avst�ndet
	resetMotorEncoder(motorB);
	resetMotorEncoder(motorC);
	do
	{
		setMotorSync(motorB, motorC, 100, 10);
	}
	while (getMotorEncoder(motorB) < target_rotation*2);
	setMotorSpeed(motorB,0);
	setMotorSpeed(motorC,0);
	sleep(1000);
}
