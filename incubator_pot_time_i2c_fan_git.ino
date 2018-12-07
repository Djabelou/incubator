#include <Adafruit_Sensor.h>

// include the library code:
#include <LiquidCrystal.h>
#include <DHT.h>

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
float hum;  //Stores humidity value
float tempp; //Stores temperature value Celsius

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
  // Wait a few seconds between measurements.
  delay(2000);
  //Read data and store it to variables hum and temp
  hum = dht.readHumidity();
  tempp= dht.readTemperature();
  // Check if any reads failed and exit early (to try again).
  if (isnan(hum) || isnan(tempp))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // lcd print Temperature
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temperature:");
  lcd.setCursor(0,1);
  lcd.print(tempp);
  lcd.print("C");
  //Print temp and humidity values to serial monitor
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(tempp);
    Serial.println(" Celsius");
  // Condition d'allumage de la lampe
  if (tempp < 34)
  {
  digitalWrite(RELAYPIN, LOW);
  } 
  else if (tempp > 35)
  {
  digitalWrite(RELAYPIN, HIGH);
  }
}


