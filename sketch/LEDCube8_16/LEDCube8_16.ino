#define IR_SMALLD_NEC
#include <IRsmallDecoder.h> 

#define ISDEBUG false

#define DATA PD5
#define CLK  PD4
#define LE   PD3

#define SIZEx 8
#define SIZEy 8
#define SIZEz 16
#define SIZExLIM SIZEx - 1
#define SIZEyLIM SIZEy - 1
#define SIZEzLIM SIZEz - 1

#define TEXTTIME  1              // in seconds
#define timeToAutoChangeMode 10 // in seconds

IRsmallDecoder irDecoder(10);
irSmallD_t irData;

byte matrix[8][8][16];

uint8_t zCount = 0;
uint8_t mode = 1; // 0 - leds off; 1 - all leds on; 2 - rain; 3 - stars; 4 - starsInv(); 5 - uppRain(); 6 - movingDot(); 7 - salute();
uint8_t modesCount = 8;
uint8_t saluteCadr = 0;
uint8_t saluteX ,saluteY = 4;

unsigned long curTimeToAutoChangeMode = 0;
unsigned long showTextCounter = 0;
 
uint16_t counter = 0;

int xPos,yPos,zPos = 0;
int xSign = 1;
int ySign = 1;
int zSign = 1;



bool cyclingMode = false;
bool oneTime, textShowOnetime = true;

//=========================================================
void fillMatrix(byte val = 0)
{
   for(uint8_t z = 0; z < SIZEz; z ++)
   {
      for (uint8_t y = 0; y < SIZEy; y ++)
      {
          for (uint8_t x = 0; x < SIZEx; x ++)
          {
            matrix[x][y][z]=val;
          }
      }
   }
}

//=========================================================
void turnLayer(uint8_t layer)
{
  // LOW value at tranzistors base = opened = layer on
  if (layer < 8)
  {
    PORTA &= ~(1 << layer);
  }
  else
  {
    PORTC &= ~(1 << (15 - layer));
  }
}

//=========================================================
void turnOffLayers()
{
    // Hi value at tranzistors base = closed = layer off
    PORTA = 255; 
    PORTC = 255;
}
//==============================================================================
void sentData()
{
    turnOffLayers();          // turn off all tranzistors
    for (uint8_t y = 0; y < SIZEy; y ++)
    {
        for (uint8_t x = 0; x < SIZEx; x ++)
        {
           PORTD &= ~(1 << DATA);
           PORTD |= (matrix[x][y][zCount] << DATA);
           PORTD |= 1 << CLK;     //set clock pin to 1
           PORTD &= ~(1 << CLK);  //set clock pin to 0
        }
    }
    PORTD |= (1 << LE);       //turn on Latch
    turnLayer(zCount);           //turn on tranzistor = vertical layer
    PORTD &= ~(1 << LE); //turn off Latch
    zCount ++;
    if (zCount >= SIZEz)zCount = 0;
}


//==============================================================================
ISR(TIMER2_COMPA_vect)
{
    sentData();
}

//==============================================================================
void setPixel(uint8_t x, uint8_t y, uint8_t z, uint8_t val = 1)
{
  if (x >= 0 && x < SIZEx && y >= 0 && y < SIZEy && z >= 0 && z < SIZEz)   matrix[x][y][z] = val;
}

//==============================================================================
void sendTextToMatrix (byte indx[][2], uint8_t sizeOfArr)
{
    for (uint8_t x = 0; x < sizeOfArr; x ++) { setPixel(indx[x][0], 0, indx[x][1]); }
}
//////=========================================================
void drawText(uint8_t n)
{
   fillMatrix(0);
   switch (n)
    {
        case 0:
        {
          byte indx[][2] ={{2, 3},{3, 3},{4, 3},{5, 3},{1, 4},{6, 4},{1, 5},{6, 5},{1, 6},{6, 6},{1, 7},{6, 7},{1, 8},{6, 8},{1, 9},{6, 9},{1, 10},{6, 10},{1, 11},{6, 11},{2, 12},{3, 12},{4, 12},{5, 12}};
          sendTextToMatrix(indx, 24);
        }
        break; 
        case 1:
        {
          byte indx[][2] ={{3, 3},{3, 4},{3, 5},{3, 6},{3, 7},{3, 8},{3, 9},{3, 10},{5, 10},{3, 11},{4, 11},{3, 12}};
          sendTextToMatrix(indx, 12);
        }
        break;
        case 2:
        {
          byte indx[][2] ={{1, 3},{2, 3},{3, 3},{4, 3},{5, 3},{6, 3},{5, 4},{4, 5},{3, 6},{2, 7},{1, 8},{1, 9},{1, 10},{6, 10},{1, 11},{6, 11},{2, 12},{3, 12},{4, 12},{5, 12}};
          sendTextToMatrix(indx, 20);
        }
        break;
        case 3:
        {
          byte indx[][2] ={{2, 3},{3, 3},{4, 3},{5, 3},{1, 4},{6, 4},{1, 5},{6, 5},{1, 6},{1, 7},{2, 8},{3, 8},{4, 8},{1, 9},{1, 10},{6, 10},{1, 11},{6, 11},{2, 12},{3, 12},{4, 12},{5, 12}};
          sendTextToMatrix(indx, 22);
        }
        break;
        case 4:
        {
          byte indx[][2] ={{2, 3},{2, 4},{1, 5},{2, 5},{3, 5},{4, 5},{5, 5},{6, 5},{7, 5},{2, 6},{7, 6},{2, 7},{6, 7},{2, 8},{5, 8},{2, 9},{5, 9},{2, 10},{4, 10},{2, 11},{3, 11},{2, 12}};
          sendTextToMatrix(indx, 22);
        }
        break;
        case 5:
        {
          byte indx[][2] ={{2, 3},{3, 3},{4, 3},{5, 3},{2, 4},{6, 4},{1, 5},{6, 5},{1, 6},{1, 7},{1, 8},{6, 8},{2, 9},{3, 9},{4, 9},{5, 9},{6, 9},{6, 10},{5, 11},{1, 12},{2, 12},{3, 12},{4, 12},{5, 12}};
          sendTextToMatrix(indx, 24);
        }
        break;
        case 6:
        {
          byte indx[][2] ={{2, 3},{3, 3},{4, 3},{5, 3},{1, 4},{6, 4},{1, 5},{6, 5},{1, 6},{6, 6},{1, 7},{5, 7},{6, 7},{2, 8},{3, 8},{4, 8},{6, 8},{6, 9},{6, 10},{1, 11},{5, 11},{2, 12},{3, 12},{4, 12}};
          sendTextToMatrix(indx, 24);
        }
        break;
        case 7:
        {
          byte indx[][2] ={{4, 3},{4, 4},{4, 5},{3, 6},{3, 7},{3, 8},{2, 9},{2, 10},{1, 11},{1, 12},{2, 12},{3, 12},{4, 12},{5, 12},{6, 12}};
          sendTextToMatrix(indx, 15);
        }
        break;
        case 8:
        {
          byte indx[][2] ={{2, 3},{3, 3},{4, 3},{5, 3},{1, 4},{6, 4},{1, 5},{6, 5},{1, 6},{6, 6},{1, 7},{6, 7},{2, 8},{3, 8},{4, 8},{5, 8},{1, 9},{6, 9},{1, 10},{6, 10},{1, 11},{6, 11},{2, 12},{3, 12},{4, 12},{5, 12}};
          sendTextToMatrix(indx, 26);
        }
        break;
        case 9:
        {
          byte indx[][2] ={{3, 3},{4, 3},{5, 3},{2, 4},{6, 4},{1, 5},{1, 6},{1, 7},{3, 7},{4, 7},{5, 7},{1, 8},{2, 8},{6, 8},{1, 9},{6, 9},{1, 10},{6, 10},{1, 11},{6, 11},{2, 12},{3, 12},{4, 12},{5, 12}};
          sendTextToMatrix(indx, 24);
        }
        break;
    }
}

//==============================================================================
void rain()
{
   if (counter > 100) // counter for delay simulation
      {
          for(int8_t z = 0; z < SIZEzLIM; z ++)
          {
              for (int8_t x = 0; x < SIZEx; x ++)
              {
                  for (int8_t y = 0; y < SIZEy; y ++)
                  {
                      matrix[x][y][z] = matrix[x][y][z + 1];
                      matrix[x][y][z + 1] = 0;
                  }
              }
          }
          // spawn 3 new dot at top
          setPixel(random(0, SIZEx), random(0, SIZEy), SIZEzLIM);
          setPixel(random(0, SIZEx), random(0, SIZEy), SIZEzLIM);
          setPixel(random(0, SIZEx), random(0, SIZEy), SIZEzLIM);
          counter=0;
      }else
      {
        counter++;
      }
}

//==============================================================================
void uppRain()
{
   if (counter > 100) // counter for delay simulation
      {
          for(int8_t z = SIZEzLIM; z > 0; z --)
          {
              for (int8_t x = 0; x < SIZEx; x ++)
              {
                  for (int8_t y = 0; y < SIZEy; y ++)
                  {
                      matrix[x][y][z] = matrix[x][y][z - 1];
                      matrix[x][y][z - 1] = 0;
                  }
              }
          }
          // spawn 3 new dot at top
          setPixel(random(0, SIZEx), random(0, SIZEy), 0);
          setPixel(random(0, SIZEx), random(0, SIZEy), 0);
          setPixel(random(0, SIZEx), random(0, SIZEy), 0);
          counter=0;
      }else
      {
        counter++;
      }
}
//==============================================================================
void stars()
{
  if (counter > 100)// counter for delay simulation
  {
      fillMatrix(0);
      for (uint8_t i = 0; i < 20; i++) //20 stars 
      {
         setPixel(random(0, SIZEx), random(0, SIZEy), random(0, SIZEz));
      }
      counter=0;
  }else  counter++;
}

//==============================================================================
void starsInv()
{
  if (counter > 100)// counter for delay simulation
  {
      fillMatrix(1);
      for (uint8_t i = 0; i < 20; i++) //20 stars 
      {
         setPixel(random(0, SIZEx), random(0, SIZEy), random(0, SIZEz), 0);
      }
      counter=0;
  }else  counter++;
}

//==============================================================================
void movingDot()
{
   if (oneTime)
   {
        xPos=random(2, SIZEx);
        yPos=random(2, SIZEy);
        zPos=random(2, SIZEz);
        setPixel(xPos, yPos, zPos);
        oneTime = false;
    }
    if (counter > 80)
    {
      counter = 0;
      xPos += xSign;
      yPos += ySign;
      zPos += zSign;
      uint8_t val1 = random(1, 3);
      uint8_t val2 = random(1, 3);
      uint8_t val3 = random(1, 3);
      if (xPos > SIZExLIM){xSign = -val1 * (xSign / abs(xSign)); xPos = SIZExLIM;}
      if (xPos < 1){xSign = -val1 * (xSign/abs(xSign)); xPos = 0;}
      if (yPos > SIZEyLIM){ySign = -val2 * (ySign/abs(ySign)); yPos = SIZEyLIM;}
      if (yPos < 1){ySign = -val2 * (ySign/abs(ySign)); yPos = 0;}
      if (zPos > SIZEzLIM){zSign = -val3 * (zSign/abs(zSign)); zPos = SIZEzLIM;}
      if (zPos < 1){zSign = -val3 * (zSign/abs(zSign)); zPos = 0;}

      fillMatrix(0);
      setPixel(xPos, yPos, zPos);
    }
    else  counter++;
}



//==============================================================================
void drawSphere(uint8_t a, uint8_t b, uint8_t c, uint8_t r)
{
  for(uint8_t z = 0; z < SIZEz; z ++)
   {
      for (uint8_t y = 0; y < SIZEy; y ++)
      {
          for (uint8_t x = 0; x < SIZEx; x ++)
          {
            int xa = x - a;
            int yb = y - b;
            int zc = z - c;
            if (xa * xa + yb * yb + zc * zc == r * r)  setPixel(x, y, z);
          }
      }
   }
}
//==============================================================================
void salute()
{
      fillMatrix(0);
      if ( saluteCadr < 10) drawSphere(saluteX, saluteY, saluteCadr, 0); else if (saluteCadr < 20) drawSphere(saluteX, saluteY, 10, saluteCadr - 10);
      saluteCadr ++;
      if (saluteCadr > 30) 
      {
        saluteCadr = 0;
        saluteX = random(1 , SIZExLIM);
        saluteY = random(1 , SIZEyLIM);
      }
      delay(50);
}

//==============================================================================
void processMode() 
{
    switch (mode)
      {
        case 0:
          fillMatrix(0);
        break;
        case 1:
          fillMatrix(1);
        break;
        case 2:
          rain();
        break;
        case 3:
          stars();
        break;
        case 4:
          starsInv();
        break;
        case 5:
          uppRain();
        break;
        case 6:
          movingDot();
        break;
        case 7:
          salute();
        break;
      }
}

//==============================================================================
void setup() 
{
    fillMatrix(0);
    DDRD |= 1 << CLK | 1 << DATA | 1 << LE; //set pins drives led Drivers to OUTPUT
    DDRC = 255; // all pins C port is OUTPUT (8 layers tranzistors)
    DDRA = 255; // all pins A port is OUTPUT (8 layers tranzistors)
    turnOffLayers(); 
    
    if (ISDEBUG)
    { 
      Serial.begin(9600); // start serial for debugging purpose
    }

    // Timer1 for background sending info to drivers
    cli(); 
    TCCR2A = 0; 
    TCCR2B = 0;    
    TCCR2B |= (1 << CS20) | (1 << CS21); // set clock devider to 32 . If this value will be lower - no time left for main code.
    TIMSK2 |= (1 << OCIE2A); //Set overflow channel A interrupt
    sei();
}

//==============================================================================
void loop() 
{
    if(irDecoder.dataAvailable(irData)) 
    { 
      bool rightButtonPressed = false;
      oneTime = true;
      textShowOnetime = true;
      saluteCadr = 0;
      cyclingMode = false;
      
      switch (irData.cmd)
      {
        case 0:
          mode = mode == 0 ? 1: 0;
          rightButtonPressed = true;
        break;
        case 0x13:
          mode = 1;
          rightButtonPressed = true;
        break;
        case 0xB:
          mode = 2;
          rightButtonPressed = true;
        break;
        case 0x1B:
          mode = 3;
          rightButtonPressed = true;
        break;
        case 0x3:
          mode = 4;
          rightButtonPressed = true;
        break;
        case 0x14:
          mode = 5;
          rightButtonPressed = true;
        break;
        case 0xC:
          mode = 6;
          rightButtonPressed = true;
        break;
        case 0x1C:
          mode = 7;
          rightButtonPressed = true;
        break;
        case 0x19:
          cyclingMode = true;
          curTimeToAutoChangeMode = millis();
          mode = 1;
        break;
      }
      if (rightButtonPressed) showTextCounter = millis(); //do not show text if not defined buttons pressed
    }
  
    if (millis() - showTextCounter < TEXTTIME * 1000)
    {
      if (textShowOnetime) 
      {
        drawText(mode);
        textShowOnetime = false;
      }
    }
    else
    {
        if (cyclingMode)
        {
            if(millis() - curTimeToAutoChangeMode > timeToAutoChangeMode * 1000)
            {
              curTimeToAutoChangeMode = millis();
              mode ++;
              if (mode >= modesCount) mode = 1;
            }
        }
        processMode();
    }
    delayMicroseconds(100);
}
