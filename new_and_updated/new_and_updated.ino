#include <BluetoothSerial.h>//nw edit
#define DEBUG_SW 1
BluetoothSerial SerialBT;//nw
#include <WiFi.h>
#include <WiFiClient.h>
#include <FirebaseESP32.h>
#include "DHT.h"
#define DHT11PIN 23
DHT dht(DHT11PIN, DHT11);
// Firebase Credentials
#define FIREBASE_HOST "smart-home-ts-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "yxbdgoYkrTPyH5CEUrYkEf2kRT4DJGdQxtjILHdJ"
String ssid_pass;//nw edit
String olta;
// WiFi Credentials
#define WIFI_SSID "🎈"
#define WIFI_PASSWORD "1223334444"
float volta;
String ssid; //nw edit
String pass; //nw edit
// Function Declaration
void  with_internet();
void  without_internet();
void Temprature();
void Radar();
void speed0();
void speed1();
void speed2();
void speed3();
void speed4();

// Indicator light
#define L1  13
#define L2  14
#define LED_BT 10//nw edit
#define SW  5//nw edit
// Pins of Fan Regulator Knob
#define s1  12
#define s2  14
#define s3  27
#define s4  26

// Pins of Switches
#define S5 34 //s1
#define S6 35 //s2
#define S7 33 //s3
#define S8 32 //s4
#define S9 25 //s5

// Pins of Relay (Appliances Control)
#define R5 22
#define R6 21
#define R7 19
#define R8 18

// Pins of Relay (Fan Speed Control)
#define Speed1 15
#define Speed2 2
#define Speed4 4


// Pins of Relay (Fan Speed Control)


//Define FirebaseESP32 data object
FirebaseData firebaseData;
FirebaseJson json;

// Necessary Variables

bool flag = false;//nw edit
// Necessary Variables
bool speed1_flag = 1;
bool speed2_flag = 1;
bool speed3_flag = 1;
bool speed4_flag = 1;
bool speed0_flag = 1;

bool speed0_firebase_flag = 1;
bool speed1_firebase_flag = 1;
bool speed2_firebase_flag = 1;
bool speed3_firebase_flag = 1;
bool speed4_firebase_flag = 1;

int switch_ON_Flag1_previous_I = 0;
int switch_ON_Flag2_previous_I = 0;
int switch_ON_Flag3_previous_I = 0;
int switch_ON_Flag4_previous_I = 0;

int Speed_Value ;
void setup()
{
  // put your setup code here, to run once:
// put your setup code here, to run once:
  pinMode(s1, INPUT_PULLUP);
  pinMode(s2, INPUT_PULLUP);
  pinMode(s3, INPUT_PULLUP);
  pinMode(s4, INPUT_PULLUP);
  pinMode(S5, INPUT_PULLUP);
  pinMode(S6, INPUT_PULLUP);
  pinMode(S7, INPUT_PULLUP);
  pinMode(S8, INPUT_PULLUP);
  pinMode(S9, INPUT_PULLUP);
  pinMode(SW,INPUT_PULLUP);//nw

  pinMode(R5, OUTPUT);
  pinMode(R6, OUTPUT);
  pinMode(R7, OUTPUT);
  pinMode(R8, OUTPUT);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(LED_BT,OUTPUT);//nw 
  pinMode(Speed1, OUTPUT);
  pinMode(Speed2, OUTPUT);
  pinMode(Speed4, OUTPUT);

  Serial.begin(115200);
  

  while(!Serial);   //When the serial port is opened, the program starts to execute.

  delay(500);

  Serial.println("Ready");  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
dht.begin();
}


void loop()
{
  /*************************************/
 
  
digitalWrite(LED_BT, LOW);
  int input = digitalRead(SW);
  Serial.print("input: ");
  Serial.println(input);
  if(input == 0)
  {
    delay(50);
    flag = true;
    digitalWrite(LED_BT, HIGH);
  }
  
  while(flag == true)
  { 
    
    while(SerialBT.available())
    {
      ssid_pass = SerialBT.readString();
      Serial.write(SerialBT.read());
      Serial.println(ssid_pass);
  
      for (int i = 0; i < ssid_pass.length(); i++) 
      {
        if (ssid_pass.substring(i, i+1) == ",") 
        {
          ssid = ssid_pass.substring(0, i);
          pass = ssid_pass.substring(i+1);
          Serial.print("SSID = ");Serial.println(ssid);
          Serial.print("Password = ");Serial.println(pass);
          delay(2000);
          flag = false;

          int n1 = ssid.length();
    char char_array1[n1 + 1];
    strcpy(char_array1, ssid.c_str());
  
    int n2 = pass.length();
    char char_array2[n2 + 1];
    strcpy(char_array2, pass.c_str());
  

    WiFi.begin(char_array1, char_array2);                 // write wifi name & password           
  
    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
    Serial.println();
    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
    


          break;
        }
      }  
    }    
  }
 /**********************************************/
  if (WiFi.status() != WL_CONNECTED)
  {
    if (DEBUG_SW) Serial.println("Not Connected");
    digitalWrite(L1, HIGH);
    digitalWrite(L2, LOW);
    without_internet();
    
  }

  else
  {
    if (DEBUG_SW) Serial.println(" Connected");
    Data_from_firebase();
    digitalWrite(L2, HIGH);
    digitalWrite(L1, LOW);
    with_internet();
    Temprature();
    
    
  }
}

void Data_from_firebase()
{
  if (Firebase.getInt(firebaseData, "/Appliances/fan"))
  {
    if (firebaseData.intData() == 0)
    {
      if (speed0_firebase_flag == 1)
      {
        speed0();
        if (DEBUG_SW) Serial.println("Speed0 Firebase");
        speed0_firebase_flag = 0;
        speed1_firebase_flag = 1;
        speed2_firebase_flag = 1;
        speed3_firebase_flag = 1;
        speed4_firebase_flag = 1;
      }
    }
    else if (firebaseData.intData() == 1)
    {
      if (speed1_firebase_flag == 1)
      {
        speed1();
        if (DEBUG_SW) Serial.println("Speed1 Firebase");
        speed0_firebase_flag = 1;
        speed1_firebase_flag = 0;
        speed2_firebase_flag = 1;
        speed3_firebase_flag = 1;
        speed4_firebase_flag = 1;
      }
    }
    else if (firebaseData.intData() == 2)
    {
      if (speed2_firebase_flag == 1)
      {
        speed2();
        if (DEBUG_SW) Serial.println("Speed2 Firebase");
        speed0_firebase_flag = 1;
        speed1_firebase_flag = 1;
        speed2_firebase_flag = 0;
        speed3_firebase_flag = 1;
        speed4_firebase_flag = 1;
      }
    }
    else if (firebaseData.intData() == 3)
    {
      if (speed3_firebase_flag == 1)
      {
        speed3();
        if (DEBUG_SW) Serial.println("Speed3 Firebase");
        speed0_firebase_flag = 1;
        speed1_firebase_flag = 1;
        speed2_firebase_flag = 1;
        speed3_firebase_flag = 0;
        speed4_firebase_flag = 1;
      }
    }
    else if (firebaseData.intData() == 4)
    {
      if (speed4_firebase_flag == 1)
      {
        speed4();
        if (DEBUG_SW) Serial.println("Speed4 Firebase");
        speed0_firebase_flag = 1;
        speed1_firebase_flag = 1;
        speed2_firebase_flag = 1;
        speed3_firebase_flag = 1;
        speed4_firebase_flag = 0;
      }
    }
    else
    {
      // Do nothing
    }

    if (Firebase.getString(firebaseData, "/Appliances/appliance1"))
    {
      if (DEBUG_SW) Serial.print("Relay1 - ");
      if (DEBUG_SW) Serial.println(firebaseData.stringData());

      if (firebaseData.stringData() == "1")
      {
        digitalWrite(R5, HIGH);
      }
      else
      {
        digitalWrite(R5, LOW);
      }
    }



    if (Firebase.getString(firebaseData, "/Appliances/appliance2")) {

      if (DEBUG_SW)  Serial.print("Relay2 - ");
      if (DEBUG_SW)  Serial.println(firebaseData.stringData());
      if (firebaseData.stringData() == "1")
      {
        digitalWrite(R6, HIGH);
      }
      else
      {
        digitalWrite(R6, LOW);
      }

    }

    if (Firebase.getString(firebaseData, "/Appliances/appliance3"))
    {
      if (DEBUG_SW) Serial.print("Relay3 - ");
      if (DEBUG_SW) Serial.println(firebaseData.stringData());

      if (firebaseData.stringData() == "1")
      {
        digitalWrite(R7, HIGH);
      }
      else
      {
        digitalWrite(R7, LOW);
      }

    }

    if (Firebase.getString(firebaseData, "/Appliances/appliance4"))
    {
      if (DEBUG_SW)  Serial.print("Relay4 - ");
      if (DEBUG_SW)  Serial.println(firebaseData.stringData());

      if (firebaseData.stringData() == "1")
      {
        digitalWrite(R8, HIGH);
      }
      else
      {
        digitalWrite(R8, LOW);
      }

    }
   
    }
  }


  
  
  void with_internet()
{

  // FOR FAN
  if (digitalRead(s1) == LOW && speed1_flag == 1)
  {
    speed1();
    Speed_Value = 1;
    json.set("/fan", Speed_Value);
    Firebase.updateNode(firebaseData, "/Appliances", json);
    speed1_flag = 0;
    speed2_flag = 1;
    speed3_flag = 1;
    speed4_flag = 1;
    speed0_flag = 1;


  }
  if (digitalRead(s2) == LOW && digitalRead(s3) == HIGH && speed2_flag == 1)
  {
    speed2();
    Speed_Value = 2;
    json.set("/fan", Speed_Value);
    Firebase.updateNode(firebaseData, "/Appliances", json);
    speed1_flag = 1;
    speed2_flag = 0;
    speed3_flag = 1;
    speed4_flag = 1;
    speed0_flag = 1;

  }
  if (digitalRead(s2) == LOW && digitalRead(s3) == LOW && speed3_flag == 1)
  {
    speed3();
    Speed_Value = 3;
    json.set("/fan", Speed_Value);
    Firebase.updateNode(firebaseData, "/Appliances", json);
    speed1_flag = 1;
    speed2_flag = 1;
    speed3_flag = 0;
    speed4_flag = 1;
    speed0_flag = 1;
  }
  if (digitalRead(s4) == LOW  && speed4_flag == 1)
  {
    speed4();
    Speed_Value = 4;
    json.set("/fan", Speed_Value);
    Firebase.updateNode(firebaseData, "/Appliances", json);
    speed1_flag = 1;
    speed2_flag = 1;
    speed3_flag = 1;
    speed4_flag = 0;
    speed0_flag = 1;
  }
  if (digitalRead(s1) == HIGH && digitalRead(s2) == HIGH && digitalRead(s3) == HIGH && digitalRead(s4) == HIGH  && speed0_flag == 1)
  {
    speed0();
    Speed_Value = 0;
    json.set("/fan", Speed_Value);
    Firebase.updateNode(firebaseData, "/Appliances", json);
    speed1_flag = 1;
    speed2_flag = 1;
    speed3_flag = 1;
    speed4_flag = 1;
    speed0_flag = 0;
  }


  // FOR SWITCH
  //Switch 1
  if (digitalRead(S5) == LOW)
  {
    if (switch_ON_Flag1_previous_I == 0 )
    {
      digitalWrite(R5, HIGH);
      if (DEBUG_SW) Serial.println("Relay1- ON");
      String Value1 = "1";
      json.set("/appliance1", Value1);
      Firebase.updateNode(firebaseData, "/Appliances", json);
      switch_ON_Flag1_previous_I = 1;
    }
    if (DEBUG_SW) Serial.println("Switch1 -ON");

  }
  if (digitalRead(S5) == HIGH )
  {
    if (switch_ON_Flag1_previous_I == 1)
    {
      digitalWrite(R5, LOW);
      if (DEBUG_SW) Serial.println("Relay1 OFF");
      String Value1 = "0";
      json.set("/appliance1", Value1);
      Firebase.updateNode(firebaseData, "/Appliances", json);
      switch_ON_Flag1_previous_I = 0;
    }
    if (DEBUG_SW)Serial.println("Switch1 OFF");
  }

//Switch 2
  if (digitalRead(S6) == LOW)
  {
    if (switch_ON_Flag2_previous_I == 0 )
    {
      digitalWrite(R6, HIGH);
      if (DEBUG_SW)  Serial.println("Relay2- ON");
      String Value2 = "1";
      json.set("/appliance2", Value2);
      Firebase.updateNode(firebaseData, "/Appliances", json);
      switch_ON_Flag2_previous_I = 1;
    }
    if (DEBUG_SW) Serial.println("Switch2 -ON");

  }
  if (digitalRead(S6) == HIGH )
  {
    if (switch_ON_Flag2_previous_I == 1)
    {
      digitalWrite(R6, LOW);
      if (DEBUG_SW) Serial.println("Relay2 OFF");
      String Value2 = "0";
      json.set("/appliance2", Value2);
      Firebase.updateNode(firebaseData, "/Appliances", json);
      switch_ON_Flag2_previous_I = 0;
    }
    if (DEBUG_SW)Serial.println("Switch2 OFF");
  }
//switch 3
  if (digitalRead(S7) == LOW)
  {
    if (switch_ON_Flag3_previous_I == 0 )
    {
      digitalWrite(R7, HIGH);
      if (DEBUG_SW) Serial.println("Relay3- ON");
      String Value3 = "1";
      json.set("/appliance3", Value3);
      Firebase.updateNode(firebaseData, "/Appliances", json);
      switch_ON_Flag3_previous_I = 1;
    }
    if (DEBUG_SW) Serial.println("Switch3 -ON");

  }
  if (digitalRead(S7) == HIGH )
  {
    if (switch_ON_Flag3_previous_I == 1)
    {
      digitalWrite(R7, LOW);
      if (DEBUG_SW) Serial.println("Relay3 OFF");
      String Value3 = "0";
      json.set("/appliance3", Value3);
      Firebase.updateNode(firebaseData, "/Appliances", json);
      switch_ON_Flag3_previous_I = 0;
    }
    if (DEBUG_SW)Serial.println("Switch3 OFF");
  }
//switch 4
  if (digitalRead(S8) == LOW)
  {
    if (switch_ON_Flag4_previous_I == 0 )
    {
      digitalWrite(R8, HIGH);
      if (DEBUG_SW) Serial.println("Relay4- ON");
      String Value4 = "1";
      json.set("/appliance4", Value4);
      Firebase.updateNode(firebaseData, "/Appliances", json);
      switch_ON_Flag4_previous_I = 1;
    }
    if (DEBUG_SW) Serial.println("Switch4 -ON");

  }
  if (digitalRead(S8) == HIGH )
  {
    if (switch_ON_Flag4_previous_I == 1)
    {
      digitalWrite(R8, LOW);
      if (DEBUG_SW) Serial.println("Relay4 OFF");
      String Value4 = "0";
      json.set("/appliance4", Value4);
      Firebase.updateNode(firebaseData, "/Appliances", json);
      switch_ON_Flag4_previous_I = 0;
    }
    if (DEBUG_SW) Serial.println("Switch4 OFF");
  }

if (digitalRead(S9) == LOW)
  {
    if (switch_ON_Flag4_previous_I == 0 )
    {
      //digitalWrite(R9, HIGH);
      //if (DEBUG_SW) Serial.println("Relay- ON");
      String Value4 = "1";
      json.set("/appliance5", Value4);
      Firebase.updateNode(firebaseData, "/Appliances", json);
      switch_ON_Flag4_previous_I = 1;
    }
    if (DEBUG_SW) Serial.println("Switch4 -ON");

  }
//switch 5
  if (digitalRead(S9) == HIGH )
  {
    if (switch_ON_Flag4_previous_I == 1)
    {
      //digitalWrite(R9, LOW);
     // if (DEBUG_SW) Serial.println("Relay4 OFF");
      String Value5 = "0";
      json.set("/appliance5", Value5);
      Firebase.updateNode(firebaseData, "/Appliances", json);
      switch_ON_Flag4_previous_I = 0;
    }
    if (DEBUG_SW) Serial.println("Switch5 OFF");
  }

}


  
  void without_internet()
{

  // FOR FAN
  if (digitalRead(s1) == LOW && speed1_flag == 1)
  {
    speed1();

    speed1_flag = 0;
    speed2_flag = 1;
    speed3_flag = 1;
    speed4_flag = 1;
    speed0_flag = 1;


  }
  if (digitalRead(s2) == LOW && digitalRead(s3) == HIGH && speed2_flag == 1)
  {
    speed2();

    speed1_flag = 1;
    speed2_flag = 0;
    speed3_flag = 1;
    speed4_flag = 1;
    speed0_flag = 1;

  }
  if (digitalRead(s2) == LOW && digitalRead(s3) == LOW && speed3_flag == 1)
  {
    speed3();

    speed1_flag = 1;
    speed2_flag = 1;
    speed3_flag = 0;
    speed4_flag = 1;
    speed0_flag = 1;
  }
  if (digitalRead(s4) == LOW  && speed4_flag == 1)
  {
    speed4();

    speed1_flag = 1;
    speed2_flag = 1;
    speed3_flag = 1;
    speed4_flag = 0;
    speed0_flag = 1;
  }
  if (digitalRead(s1) == HIGH && digitalRead(s2) == HIGH && digitalRead(s3) == HIGH && digitalRead(s4) == HIGH  && speed0_flag == 1)
  {
    speed0();

    speed1_flag = 1;
    speed2_flag = 1;
    speed3_flag = 1;
    speed4_flag = 1;
    speed0_flag = 0;
  }


  // FOR SWITCH
  digitalWrite(R5, !digitalRead(S5));
  digitalWrite(R6, !digitalRead(S6));
  digitalWrite(R7, !digitalRead(S7));
  digitalWrite(R8, !digitalRead(S8));
}



// Fan Speed Control

void speed0()
{
  //All Relays Off - Fan at speed 0
  if (DEBUG_SW)Serial.println("SPEED 0");
  digitalWrite(Speed1, LOW);
  digitalWrite(Speed2, LOW);
  digitalWrite(Speed4, LOW);

}

void speed1()
{
  //Speed1 Relay On - Fan at speed 1
  if (DEBUG_SW)Serial.println("SPEED 1");
  digitalWrite(Speed1, LOW);
  digitalWrite(Speed2, LOW);
  digitalWrite(Speed4, LOW);
  delay(1000);
  digitalWrite(Speed1, HIGH);
}

void speed2()
{
  //Speed2 Relay On - Fan at speed 2
  if (DEBUG_SW)Serial.println("SPEED 2");
  digitalWrite(Speed1, LOW);
  digitalWrite(Speed2, LOW);
  digitalWrite(Speed4, LOW);
  delay(1000);
  digitalWrite(Speed2, HIGH);
}

void speed3()
{
  //Speed1 & Speed2 Relays On - Fan at speed 3
  if (DEBUG_SW)Serial.println("SPEED 3");
  digitalWrite(Speed1, LOW);
  digitalWrite(Speed2, LOW);
  digitalWrite(Speed4, LOW);
  delay(1000);
  digitalWrite(Speed1, HIGH);
  digitalWrite(Speed2, HIGH);

}

void speed4()
{
  //Speed4 Relay On - Fan at speed 4
  if (DEBUG_SW)Serial.println("SPEED 4");
  digitalWrite(Speed1, LOW);
  digitalWrite(Speed2, LOW);
  digitalWrite(Speed4, LOW);
  delay(1000);
  digitalWrite(Speed4, HIGH);
}

  void Temprature()
{
  
 volta= dht.readTemperature();
 
  Serial.print("Temprature ");
  Serial.print(volta, 2);
  Serial.print("C");
   
  delay(100);
   
  json.set("/Temperature", volta);
  Firebase.updateNode(firebaseData,"/Sensor",json);
   if (Firebase.getInt(firebaseData, "/Sensor/Temperature"))
  {
  Serial.println(firebaseData.intData());
  Serial.print("V");
  }
}

  
