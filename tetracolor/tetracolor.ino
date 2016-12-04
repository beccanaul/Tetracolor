#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, 9, NEO_RGB + NEO_KHZ800);

#define SensorPin1      A0
#define SensorPin2      A3
#define SensorPin3      A5
#define filterSamples   31              // filterSamples should  be an odd number, no smaller than 3
int sensSmoothArray1 [filterSamples];   // array for holding raw sensor values for sensor1 
int sensSmoothArray2 [filterSamples];
int sensSmoothArray3 [filterSamples];// array for holding raw sensor values for sensor2 

int rawData1, smoothData1;  // variables for sensor1 data
int rawData2, smoothData2;  // variables for sensor2 data
int rawData3, smoothData3;  // variables for sensor3 data

void setup(){
  Serial.begin(9600);
//  turn on neopixels
  strip.begin();
  strip.setBrightness(80); 
  strip.show(); 
}
void loop(){       // test the digitalSmooth function
  rawData1 = analogRead(SensorPin1);                        // read sensor 1
  smoothData1 = digitalSmooth(rawData1, sensSmoothArray1);  // every sensor you use with digitalSmooth needs its own array

  rawData2 = analogRead(SensorPin2);                        // read sensor 2
  smoothData2 = digitalSmooth(rawData2, sensSmoothArray2);  // every sensor you use with digitalSmooth needs its own array

 rawData3 = analogRead(SensorPin3);                        // read sensor 3
  smoothData3 = digitalSmooth(rawData3, sensSmoothArray3);  // every sensor you use with digitalSmooth needs its own array

int r;
int g;
int b;


//    Serial.println(smoothData3);

//sets red

if (smoothData1 < 160)
{
  r = 255;
}
else if (smoothData1 >= 161 && smoothData1 < 600)
{
    r = map(smoothData1, 161, 600, 255, 1);
}
else if (smoothData1 >=600)
{
  r = 0;
}



// sets g

if (smoothData2 < 120)
{
  g = 255;
}
else if (smoothData2 >= 121 && smoothData2 < 630)
{
    g = map(smoothData2, 121, 630, 255, 0);
}
else if (smoothData2 >= 630)
{
  g = 0;
}

// sets b


if (smoothData3 < 360)
{
  b = 255;
}
else if (smoothData3 > 361 && smoothData3 < 500)
{
    b = map(smoothData3, 361, 500, 255, 0);
}

else if (smoothData3 >= 500) {
  b = 0;
}




//for processing

    Serial.print(r);
    Serial.print("  ");
    Serial.print(g);
    Serial.print("  ");
    Serial.println(b);


    

    
    
  strip.setPixelColor(0, r, g, b); 
  strip.setPixelColor(1, r, g, b); 
  strip.setPixelColor(2, r, g, b); 
  strip.setPixelColor(3, r, g, b); 
  strip.setPixelColor(4, r, g, b); 
  strip.setPixelColor(5, r, g, b); 
  strip.setPixelColor(6, r, g, b); 
  strip.setPixelColor(7, r, g, b);
  strip.setPixelColor(8, r, g, b); 
  strip.setPixelColor(9, r, g, b); 
  strip.setPixelColor(10, r, g, b); 
  strip.setPixelColor(11, r, g, b); 
  strip.setPixelColor(12, r, g, b); 
  strip.setPixelColor(13, r, g, b); 
  strip.setPixelColor(14, r, g, b); 
  strip.setPixelColor(15, r, g, b);
  strip.setPixelColor(16, r, g, b);
  strip.setPixelColor(17, r, g, b); 
  strip.setPixelColor(18, r, g, b); 
  strip.setPixelColor(19, r, g, b); 
  strip.setPixelColor(20, r, g, b); 
  strip.setPixelColor(21, r, g, b); 
  strip.setPixelColor(22, r, g, b); 
  strip.setPixelColor(23, r, g, b); 

  strip.show();
}

int digitalSmooth(int rawIn, int *sensSmoothArray){     // "int *sensSmoothArray" passes an array to the function - the asterisk indicates the array name is a pointer
  int j, k, temp, top, bottom;
  long total;
  static int i;
 // static int raw[filterSamples];
  static int sorted[filterSamples];
  boolean done;

  i = (i + 1) % filterSamples;    // increment counter and roll over if necc. -  % (modulo operator) rolls over variable
  sensSmoothArray[i] = rawIn;                 // input new data into the oldest slot

  // Serial.print("raw = ");

  for (j=0; j<filterSamples; j++){     // transfer data array into anther array for sorting and averaging
    sorted[j] = sensSmoothArray[j];
  }

  done = 0;                // flag to know when we're done sorting              
  while(done != 1){        // simple swap sort, sorts numbers from lowest to highest
    done = 1;
    for (j = 0; j < (filterSamples - 1); j++){
      if (sorted[j] > sorted[j + 1]){     // numbers are out of order - swap
        temp = sorted[j + 1];
        sorted [j+1] =  sorted[j] ;
        sorted [j] = temp;
        done = 0;
      }
    }
  }

/*
  for (j = 0; j < (filterSamples); j++){    // print the array to debug
    Serial.print(sorted[j]); 
    Serial.print("   "); 
  }
  Serial.println();
*/

  // throw out top and bottom 15% of samples - limit to throw out at least one from top and bottom
  bottom = max(((filterSamples * 15)  / 100), 1); 
  top = min((((filterSamples * 85) / 100) + 1  ), (filterSamples - 1));   // the + 1 is to make up for asymmetry caused by integer rounding
  k = 0;
  total = 0;
  for ( j = bottom; j< top; j++){
    total += sorted[j];  // total remaining indices
    k++; 
    // Serial.print(sorted[j]); 
    // Serial.print("   "); 
  }

  return total / k;    // divide by number of samples
}

