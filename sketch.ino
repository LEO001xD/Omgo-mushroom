#define BLYNK_TEMPLATE_ID "TMPL6sbgFAXW0"
#define BLYNK_TEMPLATE_NAME "Project"
#define BLYNK_AUTH_TOKEN ""
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include <TridentTD_LineNotify.h>
#include <WiFi.h>

#define SSID ".........."               
#define PASSWORD ".........."                      
#define LINE_TOKEN ""


#include <DHT.h>
#define DHTPIN 15   // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT sensor type
DHT dht(DHTPIN, DHTTYPE);
int moisture;
float temperature;
float humidity;

#include <HCSR04.h>
HCSR04 hc(23, 22);

#include <Adafruit_NeoPixel.h>
#define PIN            13  // พินที่ Neopixel ต่ออยู่
#define NUMPIXELS      16 // จำนวน Neopixel ในสตริป
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define SOI_PIN 34
#define Relay1 14

//#include <LiquidCrystal.h>
//LiquidCrystal lcd(12, 4, 5, 18, 19, 21);

#define LDR_PIN 35
int val_ldr;

int get3;
int get0;

void setup() {
  pinMode(SOI_PIN,INPUT);
  pinMode(Relay1, OUTPUT);

  pinMode(LDR_PIN,INPUT);

  dht.begin();
  
  strip.begin();
  strip.show(); // ตั้งค่าสีเริ่มต้นให้ทุกหลอดเป็น 'ปิด'

  //lcd.begin(16, 2);
  
  //Blynk.begin(BLYNK_AUTH_TOKEN, SSID, PASSWORD);

  //WiFi.begin(SSID, PASSWORD);
  //Serial.printf("WiFi connecting to %s\n",  SSID);
  //while (WiFi.status() != WL_CONNECTED) {
   // Serial.print(".");
  //  delay(400);
  //}
  //Serial.printf("\nWiFi connected\nIP : ");
  //Serial.println(WiFi.localIP());
  //Serial.println("");
  //Serial.println("WiFi connected");

  LINE.setToken(LINE_TOKEN);
  LINE.notify("line work");
  LINE.notifyPicture("https://cdn.discordapp.com/attachments/1196691631307501568/1204732285904224318/image.png?ex=65d5cd5f&is=65c3585f&hm=529883797b6392e8da0a67dc15fdf838c85559d196ff11f8a0f48c56a41d0f49&");
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, SSID, PASSWORD);

}
void DHT() {
  //dht
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  Serial.print("temperature : ");
  Serial.println(temperature);
  
  Serial.print("humidity :");
  Serial.println(humidity);
  Blynk.virtualWrite(V4,humidity); 
  
}
void ultra() { 
  float distance = hc.dist();
  //float distance1 = (float)distance/11.0 * 100.0;
  float distance1 = map(distance,2,13,100,0);
  Blynk.virtualWrite(V1,distance1);
  if (distance1 <= 30){
    digitalWrite(Relay1,LOW);
    LINE.notify("เติมน้ำ");
    
  }
  Serial.print("distance :"); //return current distance (cm) in serial
  Serial.println(distance); //return current distance (cm) in serial

}
void fillSolidColor(uint32_t color) { //neopixel
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}
void soi_moisture_and_rod_nam() {//ความขื้นในดิน
  moisture = analogRead(SOI_PIN); // read the analog value from sensor
  moisture =map(moisture, 2890,1000,0,100);
  Serial.print("Moisture in soi value: ");
  Serial.println(moisture);
  //Blynk.virtualWrite(V4,moisture); 
  delay(500);
  if(humidity<=75){
    digitalWrite(Relay1,HIGH);
    } 
  else{
    digitalWrite(Relay1,LOW);
    } 
  }
void ldr(){
  val_ldr = (analogRead(LDR_PIN));
  Serial.print("LDR Sun :");
  Serial.println(val_ldr);
}

BLYNK_WRITE(V3) {
  get3 = param.asInt();
}
BLYNK_WRITE(V0) {
  get0 = param.asInt();
}

void loop() {
  Blynk.run();
  Serial.print(get0);
  if(get0==1){
    soi_moisture_and_rod_nam();
    ldr();
    DHT();
    ultra();
  }
  else{
    //
    digitalWrite(Relay1,LOW);
  }

  if(get3==0){
    fillSolidColor(strip.Color(0, 0, 0)); // turn off
  }
  else if(get3==1){
    fillSolidColor(strip.Color(55, 10, 255)); // black light
  }
  else if(get3==2){
    fillSolidColor(strip.Color(255, 255, 0)); // yellow
  }                               
  else{
    //digitalWrite(Relay1,LOW);
  }
  char mix_temp_humi[150];
  sprintf(mix_temp_humi, "%.2f 'C | Humi %.2f | L %d | Soil %.2f %%", temperature ,humidity ,val_ldr,moisture);
  Serial.print(mix_temp_humi);
  delay(400);
  Blynk.virtualWrite(V2,mix_temp_humi);
  /*
  lcd.setCursor(0, 0);
  lcd.print("Moisture: "); 
  lcd.print(moisture);
  lcd.print("        "); 
  delay(100);
  lcd.setCursor(0, 1);
  lcd.print("Temp: "); 
  lcd.print(temperature);
  lcd.print("        "); 
  delay(100);
  */


  delay(100);
}
