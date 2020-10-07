#include <Servo.h>
#define touchpin 5 // sets the capactitive touch sensor @pin 4


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
int fingerprintID = 0;
String IDname;

int irSensor = 7; // declaring the pin where IR sensor is attached
Servo door; //naming the servo
int pos = 0; // initial position of the servo
int ledPin = 11; // sets the LED @pin 2

void setup() {
  door.attach(9); // pin to which servo is attached
  pinMode(13,OUTPUT);
  Serial.begin(9600);
  pinMode(irSensor, INPUT); // setting the pin to take input from the IR sensor
  pinMode(touchpin, INPUT); //sets the touch sensor as input
  pinMode(ledPin, OUTPUT);  //sets the led as output

  //Fingerprint sensor module setup
  Serial.begin(9600);
  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } 
  else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  //OLED display setup
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  //displays main screen
  displayMainScreen();
}

void loop() {
  detectPeople();
  detectFingerprint();
  detectPassenger();
}

void detectPeople(){
  int buttonState = digitalRead(irSensor); //reading the input
  Serial.println(buttonState); 
  
  if(buttonState == 0){ //if an object is detected the value turns 0
    pos = 180; //open the door
    door.write(pos);
    //digitalWrite(13,HIGH);//turn on indicator
    }
  else{
    pos = 0; // closing the door when no object
    door.write(pos);
   // digitalWrite(13,LOW);//turn off indicator
    }
  delay(1000);
}


void  detectFingerprint()
{
  displayMainScreen();
  fingerprintID = getFingerprintIDez();
  delay(50);
  if(fingerprintID == 1 || fingerprintID == 3 || fingerprintID == 4 || fingerprintID == 5){
    IDname = "Sara";
    displayUserGreeting(IDname);            
  }  
  else if(fingerprintID == 2){
    IDname = "Rui";  
    displayUserGreeting(IDname);            
  }
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print("Found ID #"); 
  Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); 
  Serial.println(finger.confidence);
  return finger.fingerID; 
}

void displayMainScreen(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(7,5);
  display.println("Waiting fingerprint");
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(52,20);
  display.println("...");  
  display.display();
  delay(2000);
}

void displayUserGreeting(String Name){
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0,0);
  display.print("Hello Mira");
  display.setCursor(0,15);
  display.print(Name); 
  display.display();
  delay(5000);
  fingerprintID = 0; 
}










  

void detectPassenger()
{
  int touchValue = digitalRead(touchpin); //reads the touch sensor signal

  if (touchValue == HIGH){      //if sensor is HIGH
    digitalWrite(ledPin, HIGH);   //LED will turn ON
  }
  else {       //otherwise
    digitalWrite(ledPin,LOW); //LED is turned OFF
  } 
 // delay(300);   //delay of 300milliseconds
}

 
