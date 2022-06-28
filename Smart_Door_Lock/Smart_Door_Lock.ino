//============================LIBRARY=========================
#include "esp_camera.h"
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "time.h"
#include <FirebaseESP32.h>

//============================DONE============================

//==========================INITIALIZATION=====================

// Select camera model
//#define CAMERA_MODEL_WROVER_KIT // Has PSRAM
#define CAMERA_MODEL_AI_THINKER // Has PSRAM
#include "camera_pins.h"

const char* ssid = " ";
const char* password = " ";
//authenticator FIREBASE realtime database
#define DATABASE_URL "https://smart-door-322ac-default-rtdb.asia-southeast1.firebasedatabase.app/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define DATABASE_SECRET "p4uaOxKEg3uVi4vHRQcHHcRv9ECOuWcntiw988JL"

/* Define the Firebase Data object */
FirebaseData fbdo;
//FirebaseData fbdo1;
//FirebaseData fbdo2;

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
int alarm;



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

void startCameraServer();  // function library untuk memulai camera server

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

//=========================================================SECURITY SYSTEM FUNCTION====================================================================
// function untuk mengaktifkan buzzer jika limit switch ditrigger
void security(){
   
   if(digitalRead(limit)==HIGH){ // jika limit switch release(lepas), maka akan arduino membaca nilai 1 (HIGH)
    digitalWrite(buzzer,HIGH); // maka mengaktifkan buzzer untuk berbunyi
    alarm = 1;
    Firebase.setInt(fbdo,"/notification/alarm",alarm);
  }
  else {  // jika keadaan lain atau limit switch tertekan 
    digitalWrite(buzzer,LOW); // maka tidak mengaktifkan buzzer
    alarm = 0;
    Firebase.setInt(fbdo,"/notification/alarm",alarm);
  }
}


  //=========================================JADWAL 1 ===========================================================
void JADWAL1 (){
 
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
   security();
  }
   delay(300);
   
}

//=========================================JADWAL 2 ===========================================================
void JADWAL2 (){

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
    security();
  }
  
}

//=========================================JADWAL 3 ===========================================================
void JADWAL3 (){
  
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
   security();

  }
  
}
void espsetup(){
  Serial.setDebugOutput(true);
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
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG; // for streaming
  //config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;
  
  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if(config.pixel_format == PIXFORMAT_JPEG){
    if(psramFound()){
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
      // Limit the frame size when PSRAM is not available
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } else {
    // Best option for face detection/recognition
    config.frame_size = FRAMESIZE_240X240;
#if CONFIG_IDF_TARGET_ESP32S3
    config.fb_count = 2;
#endif
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
  if(config.pixel_format == PIXFORMAT_JPEG){
    s->set_framesize(s, FRAMESIZE_QVGA);
  }

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif

#if defined(CAMERA_MODEL_ESP32S3_EYE)
  s->set_vflip(s, 1);
#endif
//===========================WIFI ACCESS POINT============================================
  WiFi.begin(ssid, password);
  WiFi.setSleep(false);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  startCameraServer();

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");
}

 //======================Preparing NTP Time configuration=====================
//get local GMT time function
void printLocalTime()
{
 timeClient.update(); // function library untuk mendapatkan update real time waktu dari timeClient
 timeClient.getFormattedTime(); // function library untuk mendapatkan format waktu (HH;mm;dd) 

 delay(1000); // delay 1 detik untuk update waktu dari timeClient
 
 
}




//=============================================================================DONE=====================================================

void runfirebase(){
//  Read realtime database pintu  
  // esp membaca data dari firebase(path:/relay/pintu) dan disimpan ke variable button
  if(Firebase.getString(fbdo,"/relay/pintu",button)){  
    button = fbdo.intData(); // data yang dibaca disimpan dalam data object (fbdo) lalu dikonversi ke tipe data int dan dimasukkan ke variabel button
    if(button == 1){ //jika baca yang dibaca sama dengan 1 maka 
      digitalWrite(relay,HIGH); // maka mengaktifkan relay 
      delay(dly);               // delay dengan nilai variabel yang dapat diatur untuk menahan agar relay tetap aktif
      digitalWrite(relay,LOW);  // setelah delay maka relay akan mati dan solenoid kembali menutup

      button =0;        // setelah itu mengembalikan nilai button ke 0 
      // lalu mengirimkan nilai button ke path firebase pintu agar setelah relay mati, nilai pintu di firebase juga terupdate menjadi 0
      Firebase.setInt(fbdo,"/relay/pintu",button); 
    
      }

  else { // jika firebase tidak membaca nilai dari path pintu 
    // maka melakukan logika untuk mengaktifkan alarm 
    security();
  }
  
  }
  delay(300);
}
// ========================================================MAIN PROGRAM================================================================
void setup() {  // void setup merupakan fungsi program yang hanya dijalankan sekali saja 

  Serial.begin(115200);  // memulai serial komunikasi dengan baud rate 115200 
  // mengatur mode pinout yang dipakai dalam esp32 cam
  pinMode(relay,OUTPUT);
  pinMode(limit,INPUT_PULLUP);
  pinMode(buzzer,OUTPUT);
  // memanggil function untuk setup 
  espsetup();
  firebaseSetup();
  timeClient.begin();
  
}

void loop() {
  // put your main code here, to run repeatedly:
 // memanggil function yang sudah dibuat agar bekerja terus menerus (looping)
  printLocalTime(); 
  runfirebase();
  JADWAL1();
  JADWAL2();
  JADWAL3();
  
}
