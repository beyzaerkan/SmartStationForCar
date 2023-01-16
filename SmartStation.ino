#include <Blynk.h>                 
#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h>
#include "ThingSpeak.h"

#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701
#define SS_PIN 4  //D2
#define RST_PIN 5 //D1
#define BLYNK_PRINT Serial 

#define BLYNK_TEMPLATE_ID "yourBlynkTemplateId"
#define BLYNK_DEVICE_NAME "SmartStation"
#define BLYNK_AUTH_TOKEN "yourBlynkAuthToken"

unsigned long channelID = 1995128;             // Thingspeak channel ID 
unsigned int field_no=1; 
const char* writeAPIKey = "ThingSpeakWriteAPIKey";   // Thingspeak write API Key 
const char* readAPIKey = "ThingSpeakReadAPIKey";

// hcsr04 pins
const int trigPin = 16; // D0
const int echoPin = 5; //D1

//rgb led pins
const int red = 2; // D4
const int green = 15; //D8

//blynk info
char auth[] = "yourBlynkAuthToken";
char ssid[] = "yourWifiName";
char pass[] = "yourWifiPassword";

WidgetLED led1(V1);
int buton;
BLYNK_WRITE(V2){
  buton = param.asInt();
  Serial.println(buton);
}


MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

long duration;
float distanceCm;
int statuss = 0;
int out = 0;
bool isEmpty = true;
int rentingNumber = 0;

WiFiClient client; 


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  delay(1000);
  
}

void loop() {
  Blynk.run();
  ThingSpeak.begin(client);
  // arac kirada ise
  if(statuss == 1){
    Blynk.virtualWrite(V3, "Araç kirada");
    // returne basılırsa aracı iade et   
    if(buton == 0){
      statuss = 0;
      led1.on();
      Blynk.virtualWrite(V3, "Araç iade edildi");
      Serial.println("Araç iade edildi");
      delay(1000);
    }
  } // arac kirada degil ise
  else{
    // arac kirada degil ise
  Blynk.virtualWrite(V3, "Hoşgeldin");
  // put your main code here, to run repeatedly:
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_VELOCITY/2;
  Blynk.virtualWrite(V0, distanceCm);
  // araba varsa
    if (distanceCm <= 10) {
    led1.on();
    digitalWrite(red, LOW);
    digitalWrite(green, HIGH);

    // rent butonuna basılırsa 
    if(buton == 1){
      switch(statuss){
        // arac kiralanmadiysa 
        case 0:
          Blynk.virtualWrite(V3, "Kiralama yapabilirsin");
          Serial.println("Kiralama yapabilirsin");
          delay(1000);
          Blynk.virtualWrite(V3, "NFC veya RFID kartını okut");
          rfid();
          break;
          // arac kiralandiysa
        case 1:
          Blynk.virtualWrite(V3, "Araç kirada");
          return;
          break; 
        default:
          break;
      }
    }
  } else {
    if(buton == 1){
      Blynk.virtualWrite(V3, "Araç yok, kiralama yapamazsın");
      Serial.println("Araç yok, kiralama yapamazsın");
    }
    led1.off();
    digitalWrite(red, HIGH);
    digitalWrite(green, LOW);
  }
    
  // Prints the distance on the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  
  delay(1000);
  }
}

void rfid(){
    // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    Serial.println("Yeni kart gelmedi");
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    Serial.println("kart okunamadı");
    return;
  }
  //Show UID on serial monitor
  Serial.println();
  Serial.print(" UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println();
  if (content.substring(1) == "53 A3 2B 34") //change UID of the card that you want to give access
  {
    digitalWrite(red, HIGH);
    digitalWrite(green, LOW);
    delay(250);
    digitalWrite(red, HIGH);
    digitalWrite(green, HIGH);
    delay(250);
    digitalWrite(red, HIGH);
    digitalWrite(green, LOW);
    delay(250);
    digitalWrite(red, HIGH);
    digitalWrite(green, HIGH);
    
    Serial.println(" Access Granted ");
    Serial.println(" Welcome Beyza ");
    Serial.println("Araç kiralandı");
    Blynk.virtualWrite(V3, "Araç kiralandı");
    Blynk.virtualWrite(V2, 1);
    led1.off();
    statuss = 1;
    rentingNumber += 1;
    Serial.print("Sayı: ");
    Serial.println(rentingNumber);
    ThingSpeak.writeField (channelID, field_no, rentingNumber, writeAPIKey);       // sıcaklık değerini gönder 
  }
  
  else   {
    digitalWrite(red, LOW);
    digitalWrite(green, HIGH);
    delay(250);
    digitalWrite(red, HIGH);
    digitalWrite(green, HIGH);
    delay(250);
    digitalWrite(red, LOW);
    digitalWrite(green, HIGH);
    delay(250);
    digitalWrite(red, HIGH);
    digitalWrite(green, HIGH);

    Blynk.virtualWrite(V3, "Erişim engellendi");
    Blynk.virtualWrite(V2, 1);
    delay(3000);
  }
}
