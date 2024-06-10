#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

const int Laser_Pin = 5;  // Laser Pin
int buttonState = 0; 
const int buttonPin = 3;  // Changed button pin to pin 3

// Debounce variables
int lastButtonState = 0; 
unsigned long lastDebounceTime = 0;  
unsigned long debounceDelay = 50;    

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  Serial.begin(9600);
  Serial.println("Adafruit MLX90614 test"); 
  
  pinMode(Laser_Pin, OUTPUT);
  pinMode(buttonPin, INPUT);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  display.clearDisplay();
  display.setRotation(2);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 35);
  display.println("Initializing Temp");
  display.display();
  delay(250);
  display.clearDisplay();

  mlx.begin(); 
}

void loop() {
  // Read the button state and implement debounce
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
    }
  }

  lastButtonState = reading;
  
  // Print ambient and object temperature in Celsius and Fahrenheit
  float ambientC = mlx.readAmbientTempC();
  float objectC = mlx.readObjectTempC();
  float ambientF = mlx.readAmbientTempF();
  float objectF = mlx.readObjectTempF();
  
  // Commented out for variety
  // Serial.print("Ambient = "); Serial.print(ambientC); 
  // Serial.print("*C\tObject = "); Serial.print(objectC); Serial.println("*C");
  // Serial.print("Ambient = "); Serial.print(ambientF); 
  // Serial.print("*F\tObject = "); Serial.print(objectF); Serial.println("*F");
  
  // Check if the pushbutton is pressed
  if (buttonState == HIGH) {
    digitalWrite(Laser_Pin, HIGH);
    
    display.clearDisplay();
    display.setTextSize(2);  // Size 2 means each pixel is 12 width and 16 high
    display.setCursor(25, 10);
    display.print(objectC);
    display.setCursor(95, 10);
    display.print("C");
    display.setCursor(25, 36);
    display.print(objectF);
    display.setCursor(95, 36);
    display.print("F");
    display.display();
    
  } else {
    digitalWrite(Laser_Pin, LOW);
  
    display.clearDisplay();
    display.setTextSize(2);  // Size 2 means each pixel is 12 width and 16 high
    display.setCursor(35, 10);
    display.print("-----");
    display.setCursor(105, 10);
    display.print("");
    display.setCursor(35, 36);
    display.print("-----");
    display.setCursor(105, 36);
    display.print("");
    display.display();
  }

  // Commented out for variety
  // Serial.println();
  delay(500);
}
