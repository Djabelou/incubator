// Script Arduino pour programmer un incubateur
// mesure de la température avec DHT22
// relay 5V pour controler une lampe (100W) sur 220V
// ecran lcd LCM1602C

// include the library code:
#include <LiquidCrystal.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>

// Temp/Humidity Sensor
#define DHTPIN 8     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN, DHTTYPE);

// Relay
#define RELAYPIN 9

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//Variables
float hum;   //Stores humidity value
float tempp; //Stores temperature value Celsius
float tset;  //Stores the set temperature

//1 int potpin = 2; // // select the input pin for the potentiometer
//1 int potval; // valeur potentiometre

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
}

void loop()
{
  // temperature setting
  // tempeh: 34°C puis 30°C après 12h
  tset = 30;
  
  //1 potval = analogRead(potpin); // valeur de potval entre 0 et 1024
  //1 int tset = map(potval, 0, 1023, 0, 50);// reechelonne entre 0 et 50
  
  //Read data and store it to variables hum and temp
  hum = dht.readHumidity();
  tempp= dht.readTemperature();
  // Check if any reads failed and exit early (to try again).
  if (isnan(hum) || isnan(tempp)) // check if dht22 is well connected
  {
    Serial.println("Failed to read from DHT sensor!");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Failed reading");
    lcd.setCursor(0,1);
    lcd.print("lamp stopped");
    digitalWrite(RELAYPIN, HIGH); // arret de la lampe si prb de connection
    return;
  }
  // lcd print Temperature
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Tset:");
  lcd.print(tset);
  lcd.print(" C");
  lcd.setCursor(0,1);
  lcd.print("T:");
  lcd.print(tempp);
  lcd.print(" C");
  //Print temp and humidity values to serial monitor
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(tempp);
    Serial.println(" Celsius");
  // Condition d'allumage de la lampe
  if (tempp < tset)
  {
  digitalWrite(RELAYPIN, LOW);
  delay(1000);
  } 
  else if (tempp > tset + 0.1)
  {
  digitalWrite(RELAYPIN, HIGH);
  delay(60000);
  }
  delay(1000);
}


