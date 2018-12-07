// Script Arduino pour programmer un incubateur
// mesure de la température avec DHT22
// relay 5V pour controler une lampe (100W) sur 220V
// ecran lcd LCM1602C

// include the library code:
#include <LiquidCrystal.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Temp/Humidity Sensor
#define DHTPIN 8     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN, DHTTYPE);

// Relay
#define RELAYPIN 9

//I2C pins declaration
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

//Variables
float hum;   //Stores humidity value
int humr; // rounded humidity
float tempp; //Stores temperature value Celsius
int temppr; // rounded tempp
int const potpin = A0; // // select the input pin for the potentiometer
int potval; // valeur potentiometre
int tset;  //Stores the set temperature

// Time
long day = 86400000; // 86400000 milliseconds in a day
long hour = 3600000; // 3600000 milliseconds in an hour
long minute = 60000; // 60000 milliseconds in a minute
long second =  1000; // 1000 milliseconds in a second

void setup()
{
  // setup the Tempt/Humidity Sensor 
  dht.begin();
  // print on laptop screen
  Serial.begin(9600);
  // Setup relay
  pinMode(RELAYPIN, OUTPUT);
  // set up the number of columns and rows on the LCD 
  lcd.begin(16, 2);
  lcd.backlight();//To Power ON the back light
  //lcd.backlight();// To Power OFF the back light
}

void loop()
{ 
  potval = analogRead(potpin); // valeur de potval entre 0 et 1024
  tset = map(potval, 0, 1023, 10, 40);// reechelonne entre 0 et 40
    
  if (potval > 1023 || isnan(potval)) // check if potpin is well connected
  {
    Serial.println("Failed to read from pot!");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("pot failed");
    lcd.setCursor(0,1);
    lcd.print("lamp stopped");
    digitalWrite(RELAYPIN, HIGH); // arret de la lampe si prb de connection
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
    lcd.print("lamp stopped");
    digitalWrite(RELAYPIN, HIGH); // arret de la lampe si prb de connection
    return;
  }
  
  time();
  
  // lcd print Temperature
  //lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Ts:");
  lcd.print(tset);
  lcd.print(" C");
  lcd.setCursor(10,0);
  lcd.print("H:");
  lcd.print(humr);
  lcd.print("%");
  lcd.setCursor(0,1);
  lcd.print("T :");
  lcd.print(temppr);
  lcd.print(" C");
  
  //Print temp and humidity values to serial monitor
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(tempp);
    Serial.println(" Celsius");
  // Condition d'allumage de la lampe
  if (temppr < tset)
  {
  digitalWrite(RELAYPIN, LOW);
  delay(500);
  } 
  else if (temppr > tset)
  {
  digitalWrite(RELAYPIN, HIGH);
  //delay(60000);
  }
  //delay(1000);
}


void time(){
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
