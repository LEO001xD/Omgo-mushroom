#define BLYNK_TEMPLATE_ID "TMPL6N_-7GvRl"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "..................."

#include <TridentTD_LineNotify.h>
#include <WiFi.h>
#include <HCSR04.h> 

#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define SOI_PIN 4

#define SSID "Wokwi-GUEST"               
#define PASSWORD ""                      
#define LINE_TOKEN "......................"      
LiquidCrystal lcd(12, 13, 16, 17, 18, 19);
HCSR04 hc(23, 22); //tric,echo
void setup() {
  lcd.begin(16, 2);
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, SSID, PASSWORD);
  WiFi.begin(SSID, PASSWORD);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  LINE.setToken(LINE_TOKEN);
  //LINE.notify("Hello from ESP32 on Wokwi!");
  //LINE.notifyPicture("สวัสดีวันอังคาร","https://blogger.googleusercontent.com/img/b/R29vZ2xl/AVvXsEjT21_Z_eLPyjFuRQDzJiN9hGFsRHeatpgeDj47T93t93MV3BvROSx6c4Tku-dWEKFaVadWRSWHl9MPEo3ErxqoTv9fT4jHt8v8CZ4aXVfCL86oBroodOgN3xciMB8ffO_y7T-Ob2TUj-AxcWsDPcnmxnRFNUiKZqgOO5H3Gcp3A5W3taHgoBFoL63cUg/s2093/1684132556354.jpg");
  //LINE.notifyPicture("uto kawaii","https://cdn.discordapp.com/attachments/1138409352777175150/1188863197151776828/GBOOGKHawAALsGz.jfif?ex=659c1222&is=65899d22&hm=4fff9989b9c2c766ddf79ba549247abfa939ae32417bf16259d9f0b76512031c&");
}

void soi_moisture() {//lcd show soi mois,temp

  lcd.setCursor(0, 0);
  lcd.print("Moisture : "); 
  lcd.print(moisture);
  lcd.print("        "); 
  delay(100);
  lcd.setCursor(0, 1);
  lcd.print("Temperature : "); // dht ยังไม่เขียน
  lcd.print(temp);// dht ยังไม่เขียน
  lcd.print("        "); 
  delay(100);

}
void DHT() { //อุณหภูมิในอากาศ เขียนไม่เป็น

  
}
void soi_moisture() {//ความขื้นในดิน

  int moisture = analogRead(SOI_PIN); // read the analog value from sensor
  Serial.print("Moisture in soi value: ");
  Serial.println(moisture);
  delay(500);
}

void water() {//ดูว่าน้ำจะหมดไหม
  int distance = hc.dist();
  if (distance <= 20){
    LINE.notify("เติมน้ำ");
    Blynk.virtualWrite(V3,distance);
  }
  else{
    //
  }
}
void loop() {
  DHT();
  soi_moisture();
  water();
  delay(100);
  Blynk.run();
}
