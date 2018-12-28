#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
SoftwareSerial BTserial(D10, D9);//rx,tx D10===Tx,D9==rx
int btresponse = 0;

//-------- Customise these values -----------
const char* ssid = "Phoenix";
const char* password = "08082017";
 
#define ORG "tkv6xk"
#define DEVICE_TYPE "bluetooth"
#define DEVICE_ID "2802" 
#define TOKEN "wNYyHDVjPrTBrMk43"
//-------- Customise the above values --------
 
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/Data/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
 
WiFiClient wifiClient;
PubSubClient client(server, 1883,wifiClient);

int TotalResources=100;
int FoodKit=50;
int MedicineKit=30;
int ClotheKit=20;

void setup() {
  Serial.begin(9600);
  BTserial.begin(9600);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   Serial.print(".");
  } 
 Serial.println("");
 
 Serial.print("WiFi connected, IP address: ");
 Serial.println(WiFi.localIP());
}
 
void loop() {
if (BTserial.available() > 0) { 
    btresponse = BTserial.read(); // Reads the data from the serial port
    Serial.println(btresponse);
    PublishData(btresponse); 
    btresponse = 0;
    return;
  }
}



void PublishData(int t){
 if (!!!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(500);
 }
 Serial.println();
 }
 String data;
  if(t==49){
    data="\"Food Kit\"";
    FoodKit--;
    TotalResources--;
  }
  else if(t==50){
     data="\" Medicine Kit\"";
     MedicineKit--;
     TotalResources--;
  }
  else if(t==51){
     data="\"Clothe Kit\"";
     ClotheKit--;
     TotalResources--;
  }
  String payload = "{\"d\":{\"Food\":";
  payload += FoodKit;
  payload+="," "\"Medicine\":";
  payload += MedicineKit;
  payload+="," "\"Clothes\":";
  payload += ClotheKit;
  payload+="," "\"Requested\":";
  payload+=data;
  payload += "}}";
 Serial.print("Sending payload: ");
 Serial.println(payload);
  
 if (client.publish(topic, (char*) payload.c_str())) {
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }
}
