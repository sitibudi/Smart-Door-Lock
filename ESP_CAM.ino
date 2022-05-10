//============================LIBRARY=========================
#include "esp_camera.h"
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "time.h"
#include <FirebaseESP32.h>
#include <addons/RTDBHelper.h>
//============================DONE============================

//==========================INITIALIZATION=====================

// Select camera model
//#define CAMERA_MODEL_WROVER_KIT // Has PSRAM
#define CAMERA_MODEL_AI_THINKER // Has PSRAM
#include "camera_pins.h"

const char* ssid = "STUDIO GAMING";
const char* password = "sukaayamgoreng";
//authenticator FIREBASE realtime database
#define DATABASE_URL "https://smart-door-322ac-default-rtdb.asia-southeast1.firebasedatabase.app/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define DATABASE_SECRET "p4uaOxKEg3uVi4vHRQcHHcRv9ECOuWcntiw988JL"

/* Define the Firebase Data object */
FirebaseData fbdo;
//FirebaseData fbdo1;
//FirebaseData fbdo2;
//FirebaseData fbdo3;
//FirebaseData fbdo4;
//FirebaseData fbdo5;
//FirebaseData fbdo6;

/* Define the FirebaseAuth data for authentication data */
FirebaseAuth auth;

/* Define the FirebaseConfig data for config data */
FirebaseConfig config;

//=====================NTP server time==========================================
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,"pool.ntp.org", 25200);  // GMT(+7) * 3600 = gmtOffset_sec 
const char* ntpServer = "pool.ntp.org";
//==========================DEFINE PIN========================================
#define relay 14
const int limit = 13;
#define buzzer 12
int dly = 3000; //delay sistem alarm
int button = 0;




//============================JADWAL 1===========================
int jam1 =0;
int menit1=0;
//============================DONE===============================
//============================JADWAL 2===========================
int jam2 =0;
int menit2=0;
//============================DONE===============================
//============================JADWAL 3===========================
int jam3 =0;
int menit3=0;
//============================DONE===============================



//===========================FUNCTION============================

void startCameraServer();
void firebaseSetup(){
  //========================Setup FIREBASE realtime database======================
    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    /* Assign the certificate file (optional) */
    //config.cert.file = "/cert.cer";
    //config.cert.file_storage = StorageType::FLASH;

    /* Assign the database URL and database secret(required) */
    config.database_url = DATABASE_URL;
    config.signer.tokens.legacy_token = DATABASE_SECRET;

    Firebase.reconnectWiFi(true);

    /* Initialize the library with the Firebase authen and config */
    Firebase.begin(&config, &auth);
  Serial.setDebugOutput(true);
  Serial.println();
  
}


void JADWAL1 (){
 
  //=========================================JADWAL 1 ===========================================================
  Firebase.getString(fbdo,"/waktu1/jam1",jam1);
  jam1 = fbdo.intData();
  
  Firebase.getString(fbdo,"/waktu1/menit1",menit1);
  menit1=fbdo.intData();
//  Serial.println(jam1);
//  Serial.println(menit1);

   if(timeClient.getHours() ==jam1 and timeClient.getMinutes()==menit1 and timeClient.getSeconds()==0){
    digitalWrite(relay,HIGH);
    delay(dly);
    digitalWrite(relay,LOW);
   
  

  }
  else{
    if(digitalRead(limit)==HIGH){
    digitalWrite(buzzer,HIGH);
  }
    else {
    digitalWrite(buzzer,LOW);
  }
  
  }
  
 
   delay(300);
   
}
void JADWAL2 (){
  //=========================================JADWAL 2 ===========================================================
   Firebase.getString(fbdo,"/waktu2/jam2",jam2);
  jam2 = fbdo.intData();
  
  Firebase.getString(fbdo,"/waktu2/menit2",menit2);
  menit2=fbdo.intData();
//  Serial.println(jam2);
//  Serial.println(menit2);

   if(timeClient.getHours() ==jam2 and timeClient.getMinutes()==menit2 and timeClient.getSeconds()==0){
    digitalWrite(relay,HIGH);
    delay(dly);
    digitalWrite(relay,LOW);
//    digitalWrite(buzzer,LOW);
  }
  else{
    if(digitalRead(limit)==HIGH){
    digitalWrite(buzzer,HIGH);
  }
    else {
    digitalWrite(buzzer,LOW);
  }

  
  }
  
}
void JADWAL3 (){
   //=========================================JADWAL 3 ===========================================================
   Firebase.getString(fbdo,"/waktu3/jam3",jam3);
  jam3 = fbdo.intData();
  
  Firebase.getString(fbdo,"/waktu3/menit3",menit3);
  menit3=fbdo.intData();
//  Serial.println(jam3);
//  Serial.println(menit3);

   if(timeClient.getHours() ==jam3 and timeClient.getMinutes()==menit3 and timeClient.getSeconds()==0){
    digitalWrite(relay,HIGH);
    delay(dly);
    digitalWrite(relay,LOW);

  }
  else{
    if(digitalRead(limit)==HIGH){
    digitalWrite(buzzer,HIGH);
  }
    else {
    digitalWrite(buzzer,LOW);
  }

  }
  
}
void espsetup(){
  
   WiFi.begin(ssid, password);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();



  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  
  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1); // flip it back
    s->set_brightness(s, 1); // up the brightness just a bit
    s->set_saturation(s, -2); // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  s->set_framesize(s, FRAMESIZE_QVGA);

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif

 

  startCameraServer();

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");

 
 //======================Preparing NTP Time configuration=====================
     
}
//get local GMT time function
void printLocalTime()
{
 timeClient.update();
 timeClient.getFormattedTime();

 delay(1000);
 
 
}

//security system function
void security(){
  
   if(digitalRead(limit)==HIGH){
    digitalWrite(buzzer,HIGH);
  }
  else {
    digitalWrite(buzzer,LOW);
  }
}


//================================DONE==========================

void runfirebase(){
//  Read realtime database pintu
//  int n =0;
  int button_old;
  
  if(Firebase.getString(fbdo,"/relay/pintu",button)){
  button = fbdo.intData();
  if(button == 1){
    digitalWrite(relay,HIGH);
    delay(dly);
    digitalWrite(relay,LOW);
    button =0;
    Firebase.setInt(fbdo,"/relay/pintu",button);
    
  }
  else {
    if(digitalRead(limit)==HIGH){
    digitalWrite(buzzer,HIGH);
  }
    else {
    digitalWrite(buzzer,LOW);
  }
  }
  
  }
  delay(300);
  

  
  
}

void setup() {
  Serial.begin(115200);
  
  
  pinMode(relay,OUTPUT);
  pinMode(limit,INPUT_PULLUP);
  pinMode(buzzer,OUTPUT);
  espsetup();
  firebaseSetup();
  timeClient.begin();
  
}

void loop() {
  // put your main code here, to run repeatedly:

  printLocalTime();
  runfirebase();
  JADWAL1();
  JADWAL2();
  JADWAL3();
  
}

