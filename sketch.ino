
#define BLYNK_TEMPLATE_ID "TMPL6sbgFAXW0"
#define BLYNK_TEMPLATE_NAME "Project"
#define BLYNK_AUTH_TOKEN "MbXFGvPOOno5ZqH50ziatY0nMDaQ4mFa"
int get3;

#include <TridentTD_LineNotify.h>
#include <WiFi.h>
#include <HCSR04.h> 

#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal.h>

#include <DHT.h>
#define DHTPIN 15   // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT sensor type
DHT dht(DHTPIN, DHTTYPE);
int moisture;
float temperature;

#define LDR_PIN 2
int val_ldr;

#include <Adafruit_NeoPixel.h>
#define Neo_PIN 34	 // input pin Neopixel is attached to
#define NUMPIXELS 12 // number of neopixels in Ring
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, Neo_PIN, NEO_GRB + NEO_KHZ800);
int delayval = 100; // timing delay
int redColor;
int greenColor;
int blueColor;

#define SOI_PIN 4
#define Relay1 14

#define SSID "DX100"               
#define PASSWORD "11111111"                      
#define LINE_TOKEN "HUZldWYLqry1RakCi7gxmgTzjdxccHUwrrvApBWWs05"      
LiquidCrystal lcd(12, 13, 16, 17, 18, 19);
HCSR04 hc(23, 22); //tric,echo

void setup() {
  pinMode(LDR_PIN,INPUT);
  pinMode(SOI_PIN,INPUT);
  pinMode(Relay1, OUTPUT);
  
  dht.begin();
  pixels.begin();

  lcd.begin(16, 2);
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, SSID, PASSWORD);
  /*WiFi.begin(SSID, PASSWORD);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  LINE.setToken(LINE_TOKEN);*/
}

BLYNK_WRITE(V3)
{   
  get3 = param.asInt();
}
void ldr(){
  val_ldr = map(analogRead(LDR_PIN),32,4063,1,255);
  Serial.println(val_ldr);
}
void DHT() { //อุณหภูมิในอากาศ เขียนไม่เป็น
  //dht
  temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  char mix_temp_humi[50];
  sprintf(mix_temp_humi, "Temperature %d \n Humidity %d", temperature ,humidity);
  Blynk.virtualWrite(V2, mix_temp_humi);

}
void soi_moisture_and_rod_nam() {//ความขื้นในดิน
  moisture = analogRead(SOI_PIN); // read the analog value from sensor
  Serial.print("Moisture in soi value: ");
  Serial.println(moisture);
  delay(500);
  if(moisture<=80){
    digitalWrite(Relay1,HIGH);
    } 
  else{
    digitalWrite(Relay1,LOW);
    } 
  }
void water() {//ดูว่าน้ำจะหมดไหม
  int distance = hc.dist();
  if (distance <= 20){
    LINE.notify("เติมน้ำ");
    Blynk.virtualWrite(V1,distance);
  }
  else{
    //
  }
}


void loop() {
  Blynk.run();
  ldr();
  DHT();
  soi_moisture_and_rod_nam();
  
  lcd.setCursor(0, 0);
  lcd.print("Moisture : "); 
  lcd.print(moisture);
  lcd.print("        "); 
  delay(100);
  lcd.setCursor(0, 1);
  lcd.print("Temperature : "); // dht ยังไม่เขียน
  lcd.print(temperature);// dht ยังไม่เขียน
  lcd.print("        "); 
  delay(100);
  
  water();//check water
  if(get3==0){  //none
    redColor = 0;
    greenColor = 0;
    blueColor = 0;
    for(int i=0;i<NUMPIXELS;i++){
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(redColor, greenColor, blueColor)); // Moderately bright green color.
      pixels.show(); // This sends the updated pixel color to the hardware.
      //delay(delayval); // Delay for a period of time (in milliseconds).
      // Serial.println(i);
    }
  }
  if(get3==1){  //purple
    redColor = 169;
    greenColor = 49;
    blueColor = 253;
    for(int i=0;i<NUMPIXELS;i++){
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(redColor, greenColor, blueColor)); // Moderately bright green color.
      pixels.show(); // This sends the updated pixel color to the hardware.
      //delay(delayval); // Delay for a period of time (in milliseconds).
      // Serial.println(i);
    }
  }
  if(get3==2){  //yellow
    redColor = 249;
    greenColor = 255;
    blueColor = 169;
    for(int i=0;i<NUMPIXELS;i++){
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(redColor, greenColor, blueColor)); // Moderately bright green color.
      pixels.show(); // This sends the updated pixel color to the hardware.
      //delay(delayval); // Delay for a period of time (in milliseconds).
      // Serial.println(i);
    }
  }
  delay(100);
  
}
