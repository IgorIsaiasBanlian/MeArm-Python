/*
 * VERSIPON: 0.1
 * LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
 * AUTHOR: Tiago Queiroz (github.com/belimawr)
 */
#include <Servo.h>


/*
 * You need to find the values that work for you.
 * The Servo Knob example is quite useful to do that.
 */

#define LED 13

#define MAX_X 167
#define MAX_Y 180
#define MAX_Z 180
#define MAX_G 120

#define MIN_X 43
#define MIN_Y 42
#define MIN_Z 0
#define MIN_G 90

#define BUFFER_SIZE 5

int basePin = 11;
int shoulderPin = 10;
int elbowPin = 9;
int gripperPin = 8;
Servo esc, elbow, shoulder, base, gripper;

int xdirPin = 14;
int ydirPin = 15;
int zdirPin = 13;
int gdirPin = 12;

byte in_buffer[BUFFER_SIZE];

void read_serial_data();

/*
 * The default position.
 */
int x = 128;
int y = 86;
int z = 63;
int g = 95;

void setup() {
  Serial.begin(9600);

  /* Attach ESC */
  esc.attach(7);
  esc.write(0);

  /*
   * Attach and put seros in
   * their initial position
   */
  elbow.attach(elbowPin);
  shoulder.attach(shoulderPin);
  base.attach(basePin);
  gripper.attach(gripperPin);

  elbow.write(90);
  shoulder.write(90);
  base.write(90);
  gripper.write(90);

  /* Setup LED/Debug */
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

void loop() {
  float dx = map(analogRead(xdirPin), 0, 1023, -5, 5);
  float dy = map(analogRead(ydirPin), 0, 1023, -5, 7);
  float dz = map(analogRead(zdirPin), 0, 1023, -5, 5);
  float dg = map(analogRead(gdirPin), 0, 1023, -5, 5);

  x += dx;
  y += dy;
  z += dz;
  g += dg;

  if(x > MAX_X)
    x = MAX_X;
  if(x < MIN_X)
    x = MIN_X;

  if(y > MAX_Y)
    y = MAX_Y;
  if(y < MIN_Y)
    y = MIN_Y;

  if(z > MAX_Z)
    z = MAX_Z;
  if(z < MIN_Z)
    z = MIN_Z;

  if(g > MAX_G)
    g = MAX_G;
  if(g < MIN_G)
    g = MIN_G;


  Serial.print("(");
  Serial.print(x);

  Serial.print(", ");
  Serial.print(y);

  Serial.print(", ");
  Serial.print(z);

  Serial.print(") - ");
  Serial.println(g);

  elbow.write(x);
  shoulder.write(y);
  base.write(z);
  gripper.write(g);
  delay(100);

  if(Serial.available())
    read_serial_data();
}

void read_serial_data()
{
    int count = 0, i;
    char inchar;
    bool incoming_data = true;
    int sx, sy, sz, sg;

    digitalWrite(LED, HIGH);
    while (count < 4)/* 0, 1, 2, 3 */
    {
        if(Serial.available())
        {
            Serial.print("in: ");
            inchar = Serial.read();
            in_buffer[count++%BUFFER_SIZE] = inchar;
            Serial.println(inchar, HEX);
        }
        in_buffer[4] = '\0'; /* END */
    }
    for(int i = 0;i < BUFFER_SIZE; i++)
    {
        Serial.print("Buffer[");
        Serial.print(i, DEC);
        Serial.print("] = ");
        Serial.println(in_buffer[i]);
    }


   sx = (int)in_buffer[0];
   sy = (int)in_buffer[1];
   sz = (int)in_buffer[2];
   sg = (int)in_buffer[3];

   Serial.print("*Buffer[");
   Serial.print(0, DEC);
   Serial.print("] = ");
   Serial.println(sx);

   Serial.print("*Buffer[");
   Serial.print(1, DEC);
   Serial.print("] = ");
   Serial.println(sy);

   Serial.print("*Buffer[");
   Serial.print(2, DEC);
   Serial.print("] = ");
   Serial.println(sz);

   Serial.print("*Buffer[");
   Serial.print(3, DEC);
   Serial.print("] = ");
   Serial.println(sg);

    if(sx > MAX_X)
      sx = MAX_X;
    if(sx < MIN_X)
      sx = MIN_X;

    if(sy > MAX_Y)
      sy = MAX_Y;
    if(sy < MIN_Y)
      sy = MIN_Y;

    if(sz > MAX_Z)
      sz = MAX_Z;
    if(sz < MIN_Z)
      sz = MIN_Z;

    if(sg > MAX_G)
      sg = MAX_G;
    if(sg < MIN_G)
      sg = MIN_G;

    Serial.print("**Buffer[");
    Serial.print(0, DEC);
    Serial.print("] = ");
    Serial.println(sx);

    Serial.print("**Buffer[");
    Serial.print(1, DEC);
    Serial.print("] = ");
    Serial.println(sy);

    Serial.print("**Buffer[");
    Serial.print(2, DEC);
    Serial.print("] = ");
    Serial.println(sz);

    Serial.print("**Buffer[");
    Serial.print(3, DEC);
    Serial.print("] = ");
    Serial.println(sg);

    while((x!=sx) || (y!=sy) || (z!=sz) || (g!=sg))
    {
        if(x > sx)
            x -= 1;
        else if (x < sx)
            x += 1;

        if(y > sy)
            y -= 1;
        else if (y < sy)
            y += 1;

        if(z > sz)
            z -= 1;
        else if (z < sz)
            z += 1;

        if(g > sg)
            g -= 1;
        else if (g < sg)
            g += 1;

        elbow.write(x);
        shoulder.write(y);
        base.write(z);
        gripper.write(g);

        delay(50);
    }

    x = sx;
    y = sy;
    z = sz;
    g = sg;

    elbow.write(sx);
    shoulder.write(sy);
    base.write(sz);
    gripper.write(sg);

    delay(100);

    digitalWrite(LED, LOW);
}

/*
 * 109 - 96 - 38 - 110
 * m^&n
 *
 * 71 - 81 - 61 - 120
 * GQ=x
*/
