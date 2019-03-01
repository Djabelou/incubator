// Script Arduino pour programmer un incubateur
// mesure de la température et humidité avec DHT22
// relay 5V pour controler une lampe (100W) sur 220V et un ventilateur 9V
// ecran lcd LCM1602C

// deshydratation
// 42°C 18h (pain d'essene)

// Tempeh
// 32°C pendant 12h puis 30°C

// riz
// 30°C

// blé
// 20°C

// graine en generale
// 26°C

// saccharomyces cerevisiae
// 30°C - 35°C pour initial grow
// 37.5°C - 39.8°C ensuite

// include the library code:
//
// pour lcd
#include <LiquidCrystal.h>
// pour dht
#include <DHT.h> 
#include <Adafruit_Sensor.h>
// pour i2c
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// Temp/Humidity Sensor
#define DHTPIN 8     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN, DHTTYPE);
// Relay
#define RELAYPIN1 9 // in1: lamp
int RELAYPIN2 = 6; // in2: fan  // ou #define RELAYPIN2 6
//I2C pins declaration
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
// potentiometre
int const potpin = A0; // // select the input pin for the potentiometer


//Variables
float hum;   //Stores humidity value
int humr; // rounded humidity
float tempp; //Stores temperature value Celsius
int temppr; // rounded tempp
int potval; // valeur potentiometre
int tset;  //Stores the set temperature

// For the lcd.clear();
int humr1;
int tset1;
int temppr1;

void setup()
{
  // setup the Tempt/Humidity Sensor 
  dht.begin();
  // print on laptop screen
  Serial.begin(9600);
  // Setup relay
  pinMode(RELAYPIN1, OUTPUT);
  pinMode(RELAYPIN2, OUTPUT);
  // set up the number of columns and rows on the LCD 
  lcd.begin(16, 2);
  lcd.backlight();//To Power ON the back light
  //lcd.backlight();// To Power OFF the back light
  
  humr=0;
  temppr=0;
}

void loop()
{ 
  
  intro();
  
  potval = analogRead(potpin); // valeur de potval entre 0 et 1024
  tset = map(potval, 0, 1023, 20, 36);// reechelonne entre 20°C et 35°C
  
  humr1 = humr;
  temppr1=temppr;
  tset1=tset;
    
  if (potval > 1023 || isnan(potval)) // check if potpin is well connected
  {
    Serial.println("Failed to read from pot!");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("pot failed");
    lcd.setCursor(0,1);
    lcd.print("lamp stopped");
    digitalWrite(RELAYPIN1, HIGH); // arret de la lampe si prb de connection
    return;
  }
  
  //Read data and store it to variables hum and temp
  hum = dht.readHumidity();
  humr=round(hum);
  tempp= dht.readTemperature();
  temppr= round(tempp);
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(hum) || isnan(tempp)) // check if dht22 is well connected
  {
    Serial.println("Failed to read from DHT sensor!");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("DHT Failed");
    lcd.setCursor(0,1);
    lcd.print("devices stopped");
    digitalWrite(RELAYPIN1, HIGH); // arret de la lampe si prb de connection
    digitalWrite(RELAYPIN2, HIGH); // arret du fan si prb de connection
    return;
  }
  
  if ( humr != humr1 || tset != tset1 || temppr != temppr1 )
  {
    lcd.clear();
  }
  
  time();
  
  // lcd print Temperature
  //lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Ts:");
  lcd.print(tset);
  lcd.print("'C");
  lcd.setCursor(10,0);
  lcd.print("H:");
  lcd.print(humr);
  lcd.print("%");
  lcd.setCursor(0,1);
  lcd.print("T :");
  lcd.print(temppr);
  lcd.print("'C");
  
  //Print temp and humidity values to serial monitor
  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.print(" %, Temp: ");
  Serial.print(tempp);
  Serial.println(" Celsius");
  Serial.print(" %, Temp set: ");
  Serial.print(tset);
  Serial.println(" Celsius");
      
  // Condition d'allumage de la lampe
  if (temppr < tset)
  {
  digitalWrite(RELAYPIN1, LOW);
  Serial.print("Lamp ON ");
  } 
  else if (temppr >= tset + 1)
  {
  digitalWrite(RELAYPIN1, HIGH);
  Serial.print("Lamp OFF ");
  }
  
  // Condition d'allumage du fan
  if (hum > 95)
  {
  digitalWrite(RELAYPIN2, LOW);
  Serial.print("Fan ON ");
  } 
  else if (hum < 94)
  {
  digitalWrite(RELAYPIN2, HIGH);
  Serial.print("Fan OFF ");
  }
  
  delay(500);
}


void time()
{
  // Time
  long day = 86400000; // 86400000 milliseconds in a day
  long hour = 3600000; // 3600000 milliseconds in an hour
  long minute = 60000; // 60000 milliseconds in a minute
  long second =  1000; // 1000 milliseconds in a second

  long timeNow = millis();
  int days = timeNow / day ;                                //number of days
  int hours = (timeNow % day) / hour;                       //the remainder from days division (in milliseconds) divided by hours, this gives the full hours
  int minutes = ((timeNow % day) % hour) / minute ;         //and so on...
  int seconds = (((timeNow % day) % hour) % minute) / second;
  
  // digital clock display of current time
  lcd.setCursor(9,1);
  lcd.print(days,DEC);
  printDigits(hours);  
  printDigits(minutes);
}

void printDigits(byte digits){
   // utility function for digital clock display: prints colon and leading 0
   lcd.print(":");
   if(digits < 10)
     lcd.print('0');
     lcd.print(digits,DEC);  
}

void intro()
{
while(millis()<1000*5)
  {
  lcd.setCursor(0,0);
  lcd.print("Incubator");
  lcd.setCursor(0,1);
  lcd.print("010319 Raph81212");
  }
}
