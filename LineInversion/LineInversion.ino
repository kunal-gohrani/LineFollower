/*
   These are the possible conditions of the sensors
   0 1 1 1 1 1 1 1                          // error = -7
   0 0 1 1 1 1 1 1                          // error = -6
   0 0 0 1 1 1 1 1 || 1 0 1 1 1 1 1 1       // error = -5
   1 0 0 1 1 1 1 1                          // error = -4
   1 0 0 0 1 1 1 1 || 1 1 0 1 1 1 1 1       // error = -3
   1 1 0 0 1 1 1 1                          // error = -2
   1 1 0 0 0 1 1 1 || 1 1 1 0 1 1 1 1       // error = -1
   1 1 1 0 0 1 1 1                          // error = 0
   1 1 1 0 0 0 1 1 || 1 1 1 1 0 1 1 1       // error = 1
   1 1 1 1 0 0 1 1                          // error = 2
   1 1 1 1 0 0 0 1 || 1 1 1 1 1 0 1 1       // error = 3
   1 1 1 1 1 0 0 1                          // error = 4
   1 1 1 1 1 0 0 0 || 1 1 1 1 1 1 0 1       // error = 5
   1 1 1 1 1 1 0 0                          // error = 6
   1 1 1 1 1 1 1 0                          // error = 7

   Note : Adjust your connections and wiring in such a way that for "front(leftMotorSpeed , rightMotorSpeed)" the motor moves in forward direction.
   Also , make sure that pins 0 ans 1 of arduino are not connected to anything before uploading the code.

   If this program needs to be used on a white line with black background track then read the info given in the readSensors() function and change code accordingly


*/

#define s1  A0
#define s2  A1
#define s3  A2
#define s4  3
#define s5  4
#define s6  5
#define s7  6
#define s8  7
#define LED A3
#define INVERTED_MODE_LED 2

#define RIN1  12
#define RIN2  13
#define LIN1  8
#define LIN2  9

#define R_EN  10
#define L_EN  11

#define NO_OF_SENSORS   8
#define diff_const      9 //changed from 4-7-9
#define kd 35 //changed this from 23-35
#define ki 0.07
#define baseMotorSpeed  100 //changed this from 70-95-100
#define turnspeed       70

uint8_t val[NO_OF_SENSORS] = {0, 0, 0, 0, 0, 0, 0, 0},prev_val[NO_OF_SENSORS]={0, 0, 0, 0, 0, 0, 0, 0}, sensors[NO_OF_SENSORS] = {s8, s7, s6, s5, s4, s3, s2, s1} , i = 0; int error_dir = 0; int difference = 0,last_difference = 0,stop = 0,PID_value=0,P=0,D=0,I=0;
int leftMotorSpeed = 0, rightMotorSpeed = 0;
int invertedMode=0;

void front(int lms , int rms)
{
  digitalWrite(LIN1 , LOW);      // left motor front
  digitalWrite(LIN2 , HIGH);
  digitalWrite(RIN1 , HIGH);      // right motor front
  digitalWrite(RIN2 , LOW);

  analogWrite(L_EN , lms);
  analogWrite(R_EN , rms);
}

void reverse(int lms , int rms)
{
  digitalWrite(LIN1 , HIGH);       // left motor back
  digitalWrite(LIN2 , LOW);
  digitalWrite(RIN1 , LOW);       // right motor back
  digitalWrite(RIN2 , HIGH);

  analogWrite(L_EN , lms);
  analogWrite(R_EN , rms);
}

void rotate_CCW(int lms , int rms)
{
  digitalWrite(LIN1 , HIGH);      // left motor back
  digitalWrite(LIN2 , LOW);
  digitalWrite(RIN1 , HIGH);     // right motor front
  digitalWrite(RIN2 , LOW);

  analogWrite(L_EN , lms);
  analogWrite(R_EN , rms);
}

void rotate_CW(int lms , int rms)
{
  digitalWrite(LIN1 , LOW);      // left motor front
  digitalWrite(LIN2 , HIGH);
  digitalWrite(RIN1 , LOW);      // right motor back
  digitalWrite(RIN2 , HIGH);

  analogWrite(L_EN , lms);
  analogWrite(R_EN , rms);
}
void ruk() //stop motors
{
  digitalWrite(LIN1, LOW);
  digitalWrite(LIN2, LOW);
  digitalWrite(RIN1, LOW);
  digitalWrite(RIN2, LOW);
}
void prev()
{

  for(int i=0;i<NO_OF_SENSORS;i++)
  {
    prev_val[i]=val[i];
  }
}
void readSensors()
{
  //this function is to read the sensor values and store it in the "val" array
  //Serial.println("INREAD");
  int j;
  for (i = 0, j = 0; i < 8 ; i++, j++)
  {
    val[j] = digitalRead(sensors[i]);     //reading the values of the sensors
    //Serial.print(val[j]);
    /* if used on white line with black background, uncomment the following statement(s) */
    /*if(val[j]==0)
    {
        val[j]=1;
    }else if(val[j]==1)
    {
        val[j]==0;
    }*/

  }
  //Serial.println();

}

void detectLinePosition()
{
  //Serial.println("In line");
    if(val[0] ^ val[7])
        error_dir = val[0] - val[7];

    if((val[0] == 1 && val[1] == 1 && val[2] == 1 && val[3] == 1 && val[4] == 1 && val[5] == 1 && val[6] == 1 && val[7] == 1) || (val[0] == 1 && val[1] == 1 && val[2] == 1 && val[3] == 1 && val[4] == 1 && val[5] == 1 && val[6] == 1 && val[7] == 0)|| (val[0] == 0 && val[1] == 1 && val[2] == 1 && val[3] == 1 && val[4] == 1 && val[5] == 1 && val[6] == 1 && val[7] == 1)||(val[0] == 0 && val[1] == 1 && val[2] == 1 && val[3] == 1 && val[4] == 1 && val[5] == 1 && val[6] == 1 && val[7] == 0)|| (val[0] == 1 && val[1] == 1 && val[2] == 1 && val[3] == 1 && val[4] == 1 && val[5] == 1 && val[6] == 0 && val[7] == 0)||(val[0] == 0 && val[1] == 0 && val[2] == 1 && val[3] == 1 && val[4] == 1 && val[5] == 1 && val[6] == 1 && val[7] == 1)||(val[0] == 0 && val[1] == 0 && val[2] == 0 && val[3] == 1 && val[4] == 1 && val[5] == 1 && val[6] == 1 && val[7] == 1) || (val[0] == 1 && val[1] == 1 && val[2] == 1 && val[3] == 1 && val[4] == 1 && val[5] == 0 && val[6] == 0 && val[7] == 0))
    {
        // when the bot goes out of the line for white background
        // bot will indicate a stop if on a black background
        if(invertedMode==0 && (val[0] == 1 && val[1] == 1 && val[2] == 1 && val[3] == 1 && val[4] == 1 && val[5] == 1 && val[6] == 1 && val[7] == 1))//indicates white background and bot goes out of Line
        {
            delay(30);
            readSensors();
            if(val[0] == 1 && val[1] == 1 && val[2] == 1 && val[3] == 1 && val[4] == 1 && val[5] == 1 && val[6] == 1 && val[7] == 1)
            {
            if (error_dir < 0)
            {
            difference = -9;
            }
            else if (error_dir > 0)
            {
            difference = 9;
            }
            else
            {
            difference = 0;
            }
            }else
            {
            difference=0;
            }
        }
        else if(invertedMode==1)//indicates black background and need to stop bot or detect checkpoint
        {
            prev(); //to store previous sensor values in a copy array
            delay(30); //time to gauge whether it was a stop or checkpoint
            readSensors();
            /** Below if statement is for detection of stop on a black line
            *  if wanna detect white stop line
            * use the commented if condition
            **/
            if((val[0]==prev_val[0] && val[1]==prev_val[1] && val[2]==prev_val[2] && val[3]==prev_val[3] && val[4]==prev_val[4] && val[5]==prev_val[5] && val[6]==prev_val[6] && val[7]==prev_val[7]) && (val[0]==1 && val[1]==1 && val[2]==1 && val[3]==1 && val[4]==1 && val[5]==1 && val[6]==1 && val[7]==1))
            {

                ruk();
                difference=999;
                digitalWrite(LED,HIGH);
                delay(500);
                digitalWrite(LED,LOW);
            }
            else
            {
                digitalWrite(LED,HIGH);
                delay(20);
                digitalWrite(LED,LOW);
                difference = 0;
            }
        }

    }
    if ((val[0] == 0 && val[1] == 0 && val[2] == 0 && val[3] == 0 && val[4] == 0 && val[5] == 0 && val[6] == 0 && val[7] == 0)||(val[0] == 0 && val[1] == 0 && val[2] == 0 && val[3] == 0 && val[4] == 0 && val[5] == 0 && val[6] == 0 && val[7] == 1)|| (val[0] == 1 && val[1] == 0 && val[2] == 0 && val[3] == 0 && val[4] == 0 && val[5] == 0 && val[6] == 0 && val[7] == 0)||(val[0] == 1 && val[1] == 0 && val[2] == 0 && val[3] == 0 && val[4] == 0 && val[5] == 0 && val[6] == 0 && val[7] == 1)|| (val[0] == 0 && val[1] == 0 && val[2] == 0 && val[3] == 0 && val[4] == 0 && val[5] == 0 && val[6] == 1 && val[7] == 1)||(val[0] == 1 && val[1] == 1 && val[2] == 0 && val[3] == 0 && val[4] == 0 && val[5] == 0 && val[6] == 0 && val[7] == 0)||(val[0] == 1 && val[1] == 1 && val[2] == 1 && val[3] == 0 && val[4] == 0 && val[5] == 0 && val[6] == 0 && val[7] == 0) || (val[0] == 0 && val[1] == 0 && val[2] == 0 && val[3] == 0 && val[4] == 0 && val[5] == 1 && val[6] == 1 && val[7] == 1) )
    {
        if(invertedMode==0)//white background checkpoint or stop
        {
            prev(); //to store previous sensor values in a copy array
            delay(30); //time to gauge whether it was a stop or checkpoint
            readSensors();
            /** Below if statement is for detection of stop on a black line
            *  if wanna detect white stop line
            * use the commented if condition
            **/
            if((val[0]==prev_val[0] && val[1]==prev_val[1] && val[2]==prev_val[2] && val[3]==prev_val[3] && val[4]==prev_val[4] && val[5]==prev_val[5] && val[6]==prev_val[6] && val[7]==prev_val[7]) && (val[0]==0 && val[1]==0 && val[2]==0 && val[3]==0 && val[4]==0 && val[5]==0 && val[6]==0 && val[7]==0))
            {
                ruk();
                difference=999;
                digitalWrite(LED,HIGH);
                delay(500);
                digitalWrite(LED,LOW);
            }
            else
            {
                digitalWrite(LED,HIGH);
                delay(30);
                digitalWrite(LED,LOW);
                difference = 0;
            }
        }
        else if(invertedMode==1 && (val[0] == 0 && val[1] == 0 && val[2] == 0 && val[3] == 0 && val[4] == 0 && val[5] == 0 && val[6] == 0 && val[7] == 0))//indicates bot went out in black background case
        {
            delay(20);
            readSensors();
            if(val[0] == 0 && val[1] == 0 && val[2] == 0 && val[3] == 0 && val[4] == 0 && val[5] == 0 && val[6] == 0 && val[7] == 0)
            {
            if (error_dir < 0)
            {
            difference = -9;
            }
            else if (error_dir > 0)
            {
            difference = 9;
            }
            else
            {
            difference = 0;
            }
            }else
            {
            difference=0;
            }
        }
    }
    if(difference!=999 && difference!=-10 && difference!=10 && difference!=9 && difference!=last_difference)
    {
        last_difference = difference;
    }
    if (val[0] == 0 && val[1] == 1 && val[2] == 1 && val[3] == 1 && val[4] == 1 && val[5] == 1 && val[6] == 1 && val[7] == 1)
    {
        difference = -7;
        //invertedMode=0;
    }
    //for a situation where there is left and straight line
    else if((val[0] == 0 && val[1] == 0 && val[2] == 0 && val[3] == 0 && val[4] == 1 && val[5] == 1 && val[6] == 1 && val[7] == 1) || (val[0] == 0 && val[1] == 0 && val[2] == 0 && val[3] == 0 && val[4] == 0 && val[5] == 1 && val[6] == 1 && val[7] == 1))
    {
        difference = -10;
        //invertedMode=0;
    }
    //for a situation where there is right and straight line
    else if((val[0] == 1 && val[1] == 1 && val[2] == 1 && val[3] == 1 && val[4] == 0 && val[5] == 0 && val[6] == 0 && val[7] == 0) || (val[0] == 1 && val[1] == 1 && val[2] == 1 && val[3] == 0 && val[4] == 0 && val[5] == 0 && val[6] == 0 && val[7] == 0))
    {
        difference = 10;
        //invertedMode=0;
    }
    else if (val[0] == 0 && val[1] == 0 && val[2] == 1 && val[3] == 1 && val[4] == 1 && val[5] == 1 && val[6] == 1 && val[7] == 1)
    {
        difference = -6;
        //invertedMode=0;
    }
    else if (  (val[0] == 0 && val[1] == 0 && val[2] == 0 && val[3] == 1 && val[4] == 1 && val[5] == 1 && val[6] == 1 && val[7] == 1) || (val[0] == 1 && val[1] == 0 && val[2] == 1 && val[3] == 1 && val[4] == 1 && val[5] == 1 && val[6] == 1 && val[7] == 1) )
    {
        difference = -5;
        //invertedMode=0;
    }
    else if (val[0] == 0 && val[1] == 0 && val[2] == 0 && val[3] == 0 && val[4] == 1 && val[5] == 1 && val[6] == 1 && val[7] == 1)
    {
        difference=-5;
        //invertedMode=0;
    }
    else if (val[0] == 1 && val[1] == 0 && val[2] == 0 && val[3] == 1 && val[4] == 1 && val[5] == 1 && val[6] == 1 && val[7] == 1)
    {
        difference = -4;
        //invertedMode=0;
    }
    else if (  (val[0] == 1 && val[1] == 0 && val[2] == 0 && val[3] == 0 && val[4] == 1 && val[5] == 1 && val[6] == 1 && val[7] == 1) || (val[0] == 1 && val[1] == 1 && val[2] == 0 && val[3] == 1 && val[4] == 1 && val[5] == 1 && val[6] == 1 && val[7] == 1) )
    {
        difference = -3;
        invertedMode=0;
    }
    else if (val[0] == 1 && val[1] == 1 && val[2] == 0 && val[3] == 0 && val[4] == 1 && val[5] == 1 && val[6] == 1 && val[7] == 1)
    {
        difference = -2;
        invertedMode=0;
    }
    else if (  (val[0] == 1 && val[1] == 1 && val[2] == 0 && val[3] == 0 && val[4] == 0 && val[5] == 1 && val[6] == 1 && val[7] == 1) || (val[0] == 1 && val[1] == 1 && val[2] == 1 && val[3] == 0 && val[4] == 1 && val[5] == 1 && val[6] == 1 && val[7] == 1) )
    {
        difference = -1;
        invertedMode=0;
    }
    else if (val[0] == 1 && val[1] == 1 && val[2] == 1 && val[3] == 0 && val[4] == 0 && val[5] == 1 && val[6] == 1 && val[7] == 1)
    {
        difference = 0;
        invertedMode=0;
    }
    else if (  (val[0] == 1 && val[1] == 1 && val[2] == 1 && val[3] == 0 && val[4] == 0 && val[5] == 0 && val[6] == 1 && val[7] == 1) || (val[0] == 1 && val[1] == 1 && val[2] == 1 && val[3] == 1 && val[4] == 0 && val[5] == 1 && val[6] == 1 && val[7] == 1) )
    {
        difference = 1;
        invertedMode=0;
    }
    else if (val[0] == 1 && val[1] == 1 && val[2] == 1 && val[3] == 1 && val[4] == 0 && val[5] == 0 && val[6] == 1 && val[7] == 1)
    {
        difference = 2;
        invertedMode=0;
    }
    else if (  (val[0] == 1 && val[1] == 1 && val[2] == 1 && val[3] == 1 && val[4] == 0 && val[5] == 0 && val[6] == 0 && val[7] == 1) || (val[0] == 1 && val[1] == 1 && val[2] == 1 && val[3] == 1 && val[4] == 1 && val[5] == 0 && val[6] == 1 && val[7] == 1) )
    {
        difference = 3;
        invertedMode=0;
    }
    else if (val[0] == 1 && val[1] == 1 && val[2] == 1 && val[3] == 1 && val[4] == 1 && val[5] == 0 && val[6] == 0 && val[7] == 1)
    {
        difference = 4;
        //invertedMode=0;
    }
    else if (  (val[0] == 1 && val[1] == 1 && val[2] == 1 && val[3] == 1 && val[4] == 1 && val[5] == 0 && val[6] == 0 && val[7] == 0) || (val[0] == 1 && val[1] == 1 && val[2] == 1 && val[3] == 1 && val[4] == 1 && val[5] == 1 && val[6] == 0 && val[7] == 1) )
    {
        difference = 5;
        //invertedMode=0;
    }
    else if (val[0] == 1 && val[1] == 1 && val[2] == 1 && val[3] == 1 && val[4] == 1 && val[5] == 1 && val[6] == 0 && val[7] == 0)
    {
        difference = 6;
        //invertedMode=0;
    }
    else if (val[0] == 1 && val[1] == 1 && val[2] == 1 && val[3] == 1 && val[4] == 1 && val[5] == 1 && val[6] == 1 && val[7] == 0)
    {
        difference = 7;
        //invertedMode=0;
    }
    // when the bot reaches a checkpoint / a stop in white background
    /*else if((val[0] == 0 && val[1] == 0 && val[2] == 0 && val[3] == 0 && val[4] == 0 && val[5] == 0 && val[6] == 0 && val[7] == 0)||(val[0] == 0 && val[1] == 0 && val[2] == 0 && val[3] == 0 && val[4] == 0 && val[5] == 0 && val[6] == 0 && val[7] == 1)|| (val[0] == 1 && val[1] == 0 && val[2] == 0 && val[3] == 0 && val[4] == 0 && val[5] == 0 && val[6] == 0 && val[7] == 0)||(val[0] == 1 && val[1] == 0 && val[2] == 0 && val[3] == 0 && val[4] == 0 && val[5] == 0 && val[6] == 0 && val[7] == 1)|| (val[0] == 0 && val[1] == 0 && val[2] == 0 && val[3] == 0 && val[4] == 0 && val[5] == 0 && val[6] == 1 && val[7] == 1)||(val[0] == 1 && val[1] == 1 && val[2] == 0 && val[3] == 0 && val[4] == 0 && val[5] == 0 && val[6] == 0 && val[7] == 0)||(val[0] == 1 && val[1] == 1 && val[2] == 1 && val[3] == 0 && val[4] == 0 && val[5] == 0 && val[6] == 0 && val[7] == 0) || (val[0] == 0 && val[1] == 0 && val[2] == 0 && val[3] == 0 && val[4] == 0 && val[5] == 1 && val[6] == 1 && val[7] == 1))*/
    if (val[0] == 1 && val[1] == 0 && val[2] == 0 && val[3] == 0 && val[4] == 0 && val[5] == 0 && val[6] == 0 && val[7] == 0)
    {
        difference = -7;
        //invertedMode=1;
    }
    //for a situation where there is left and straight line
    else if((val[0] == 1 && val[1] == 1 && val[2] == 1 && val[3] == 1 && val[4] == 0 && val[5] == 0 && val[6] == 0 && val[7] == 0) || (val[0] == 1 && val[1] == 1 && val[2] == 1 && val[3] == 1 && val[4] == 1 && val[5] == 0 && val[6] == 0 && val[7] == 0))
    {
        difference = -10;
        //invertedMode=1;
    }
    //for a situation where there is right and straight line
    else if((val[0] == 0 && val[1] == 0 && val[2] == 0 && val[3] == 0 && val[4] == 1 && val[5] == 1 && val[6] == 1 && val[7] == 1) || (val[0] == 0 && val[1] == 0 && val[2] == 0 && val[3] == 1 && val[4] == 1 && val[5] == 1 && val[6] == 1 && val[7] == 1))
    {
        difference = 10;
        //invertedMode=1;
    }
    else if (val[0] == 1 && val[1] == 1 && val[2] == 0 && val[3] == 0 && val[4] == 0 && val[5] == 0 && val[6] == 0 && val[7] == 0)
    {
        difference = -6;
        //invertedMode=1;
    }
    else if (  (val[0] == 1 && val[1] == 1 && val[2] == 1 && val[3] == 0 && val[4] == 0 && val[5] == 0 && val[6] == 0 && val[7] == 0) || (val[0] == 0 && val[1] == 1 && val[2] == 0 && val[3] == 0 && val[4] == 0 && val[5] == 0 && val[6] == 0 && val[7] == 0) )
    {
        difference = -5;
        //invertedMode=1;
    }
    else if (val[0] == 1 && val[1] == 1 && val[2] == 1 && val[3] == 1 && val[4] == 0 && val[5] == 0 && val[6] == 0 && val[7] == 0)
    {
        difference = -5;
        //invertedMode=1;
    }
    else if (val[0] == 0 && val[1] == 1 && val[2] == 1 && val[3] == 0 && val[4] == 0 && val[5] == 0 && val[6] == 0 && val[7] == 0)
    {
        difference = -4;
        //invertedMode=1;
    }
    else if (  (val[0] == 0 && val[1] == 1 && val[2] == 1 && val[3] == 1 && val[4] == 0 && val[5] == 0 && val[6] == 0 && val[7] == 0) || (val[0] == 0 && val[1] == 0 && val[2] == 1 && val[3] == 0 && val[4] == 0 && val[5] == 0 && val[6] == 0 && val[7] == 0) )
    {
        difference = -3;
        invertedMode=1;
    }
    else if (val[0] == 0 && val[1] == 0 && val[2] == 1 && val[3] == 1 && val[4] == 0 && val[5] == 0 && val[6] == 0 && val[7] == 0)
    {
        difference = -2;
        invertedMode=1;
    }
    else if (  (val[0] == 0 && val[1] == 0 && val[2] == 1 && val[3] == 1 && val[4] == 1 && val[5] == 0 && val[6] == 0 && val[7] == 0) || (val[0] == 0 && val[1] == 0 && val[2] == 0 && val[3] == 1 && val[4] == 0 && val[5] == 0 && val[6] == 0 && val[7] == 0) )
    {
        difference = -1;
        invertedMode=1;
    }
    else if (val[0] == 0 && val[1] == 0 && val[2] == 0 && val[3] == 1 && val[4] == 1 && val[5] == 0 && val[6] == 0 && val[7] == 0)
    {
        difference = 0;
        invertedMode=1;
    }
    else if (  (val[0] == 0 && val[1] == 0 && val[2] == 0 && val[3] == 1 && val[4] == 1 && val[5] == 1 && val[6] == 0 && val[7] == 0) || (val[0] == 0 && val[1] == 0 && val[2] == 0 && val[3] == 0 && val[4] == 1 && val[5] == 0 && val[6] == 0 && val[7] == 0) )
    {
        difference = 1;
        invertedMode=1;
    }
    else if (val[0] == 0 && val[1] == 0 && val[2] == 0 && val[3] == 0 && val[4] == 1 && val[5] == 1 && val[6] == 0 && val[7] == 0)
    {
        difference = 2;
        invertedMode=1;
    }
    else if (  (val[0] == 0 && val[1] == 0 && val[2] == 0 && val[3] == 0 && val[4] == 1 && val[5] == 1 && val[6] == 1 && val[7] == 0) || (val[0] == 0 && val[1] == 0 && val[2] == 0 && val[3] == 0 && val[4] == 0 && val[5] == 1 && val[6] == 0 && val[7] == 0) )
    {
        difference = 3;
        invertedMode=1;
    }
    else if (val[0] == 0 && val[1] == 0 && val[2] == 0 && val[3] == 0 && val[4] == 0&& val[5] == 1 && val[6] == 1 && val[7] == 0)
    {
        difference = 4;
        //invertedMode=1;
    }
    else if (  (val[0] == 0 && val[1] == 0 && val[2] == 0 && val[3] == 0 && val[4] == 0 && val[5] == 1 && val[6] == 1 && val[7] == 1) || (val[0] == 0 && val[1] == 0 && val[2] == 0 && val[3] == 0 && val[4] == 0 && val[5] == 0 && val[6] == 1 && val[7] == 0) )
    {
        difference = 5;
        //invertedMode=1;
    }
    else if (val[0] == 0 && val[1] == 0 && val[2] == 0 && val[3] == 0 && val[4] == 0 && val[5] == 0 && val[6] == 1 && val[7] == 1)
    {
        difference = 6;
        //invertedMode=1;
    }
    else if (val[0] == 0 && val[1] == 0 && val[2] == 0 && val[3] == 0 && val[4] == 0 && val[5] == 0 && val[6] == 0 && val[7] == 1)
    {
        difference = 7;
        //invertedMode=1;
    }
}

void controlMotors()
{
  //Serial.print("Difference = ");
  //Serial.println(difference);
    if (difference == -9)
    {
      while ((val[0] == 1 && val[1] == 1 && val[2] == 1 && val[3] == 1 && val[4] == 1 && val[5] == 1 && val[6] == 1 && val[7] == 1))
      {
        rotate_CCW(turnspeed , turnspeed);
        readSensors();
        detectLinePosition();
        if (!(val[0] == 1 && val[1] == 1 && val[2] == 1 && val[3] == 1 && val[4] == 1 && val[5] == 1 && val[6] == 1 && val[7] == 1) && difference>-9)
        {
          //ruk();
          break;
        }
      }
      error_dir = 0;
    }
    else if (difference == 9)
    {
      while ((val[0] == 1 && val[1] == 1 && val[2] == 1 && val[3] == 1 && val[4] == 1 && val[5] == 1 && val[6] == 1 && val[7] == 1))
      {
        rotate_CW(turnspeed,turnspeed);
        readSensors();
        detectLinePosition();
        if (!(val[0] == 1 && val[1] == 1 && val[2] == 1 && val[3] == 1 && val[4] == 1 && val[5] == 1 && val[6] == 1 && val[7] == 1) && difference<9)
        {
          //ruk();
          break;
        }
      }
      error_dir = 0;
    }
    /*else if (difference == 999)
    {
      ruk();
    }*/
    else if(difference == -10)
    {
      while((val[0] == 0 && val[1] == 0 && val[2] == 0 && val[3] == 0 && val[4] == 1 && val[5] == 1 && val[6] == 1 && val[7] == 1) || (val[0] == 0 && val[1] == 0 && val[2] == 0 && val[3] == 0 && val[4] == 0 && val[5] == 1 && val[6] == 1 && val[7] == 1))
      {
        rotate_CCW(turnspeed,turnspeed);
        readSensors();
        if(!((val[0] == 0 && val[1] == 0 && val[2] == 0 && val[3] == 0 && val[4] == 1 && val[5] == 1 && val[6] == 1 && val[7] == 1) || (val[0] == 0 && val[1] == 0 && val[2] == 0 && val[3] == 0 && val[4] == 0 && val[5] == 1 && val[6] == 1 && val[7] == 1)))
        {
          //ruk();
          break;
        }
      }
    }
    else if(difference == 10)
    {
      while((val[0] == 1 && val[1] == 1 && val[2] == 1 && val[3] == 1 && val[4] == 0 && val[5] == 0 && val[6] == 0 && val[7] == 0) || (val[0] == 1 && val[1] == 1 && val[2] == 1 && val[3] == 0 && val[4] == 0 && val[5] == 0 && val[6] == 0 && val[7] == 0))
      {
        rotate_CW(turnspeed,turnspeed);
        readSensors();
        if(!((val[0] == 1 && val[1] == 1 && val[2] == 1 && val[3] == 1 && val[4] == 0 && val[5] == 0 && val[6] == 0 && val[7] == 0) || (val[0] == 1 && val[1] == 1 && val[2] == 1 && val[3] == 0 && val[4] == 0 && val[5] == 0 && val[6] == 0 && val[7] == 0)))
        {
          //ruk();
          delay(10);
          break;
        }
      }
    }
    else if(difference==999)
    {

    }
    else
    {
      P=difference*diff_const;
      D=(difference-last_difference)*kd;
      I=(I+difference)*ki;
      I=constrain(I,-40,+40);
      PID_value=P+D+I;
      leftMotorSpeed = baseMotorSpeed + PID_value;
      rightMotorSpeed = baseMotorSpeed - PID_value;
      //leftMotorSpeed = (leftMotorSpeed < 0) ? 0 : leftMotorSpeed;
      //rightMotorSpeed = (rightMotorSpeed < 0) ? 0 : rightMotorSpeed;
      leftMotorSpeed = constrain(leftMotorSpeed , 0 , 255);
      rightMotorSpeed = constrain(rightMotorSpeed , 0 , 255);

      //Serial.print("Left motor = ");
      //Serial.println(leftMotorSpeed);
      //Serial.print("Right motor = ");
      //Serial.println(rightMotorSpeed);
      front(leftMotorSpeed , rightMotorSpeed);
      if(D!=0)
        delay(15);
      //delay(5000);
    }

}

void setup()
{

  //Serial.begin(9600);
  for (i = 0 ; i < NO_OF_SENSORS ; i++)
    pinMode(sensors[i], INPUT);

  pinMode(RIN1 , OUTPUT);
  pinMode(RIN2 , OUTPUT);
  pinMode(LIN1 , OUTPUT);
  pinMode(LIN2 , OUTPUT);
  pinMode(R_EN , OUTPUT);
  pinMode(L_EN , OUTPUT);
  pinMode(LED,OUTPUT);
  pinMode(A5,OUTPUT);
  pinMode(INVERTED_MODE_LED,OUTPUT);
  digitalWrite(A5,LOW);
}

void loop()
{
  //Serial.println("HI");
  readSensors();
  detectLinePosition();
  controlMotors();
  if(invertedMode==1)
  {
      digitalWrite(INVERTED_MODE_LED,HIGH);
  }
  else if(invertedMode==0)
  {
      digitalWrite(INVERTED_MODE_LED,LOW);
  }
}
