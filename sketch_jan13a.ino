// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
  Name:       JsonTestWithServer.ino
  Created:  2020/1/3 20:23:16
  Author:     MAOM



*/

// Define User Types below here or use a .h file
//
#include <ArduinoJson.h>
#include <Ethernet.h>

// Define Function Prototypes that use User Types below here or use a .h file
//
#define FirstTitle "MsgType"
#define SecondTitle "MsgContent"

//Define   net   // here change the server sata 
IPAddress SERVER_IP(192, 168, 1, 111); 
#define SERVER_PORT 55666
#define HEARTBEAT_TIME 3000
#define MY_MCU_PORT 4000
#define MY_MCU_ID "TEST_MAO"

char * clientConnect();
char * readPage();

unsigned long time_now = 0;
unsigned long heart_time_mark = 0;

int read_Page_Mode = 0;
bool isMsgDeal = true;

const char* json = "{\"MsgType\":\"LightControl\",\"MsgContent\":[{\"LightID\":\"L_1\",\"LightStatus\":0},{\"LightID \":\"L_1 \",\"LightStatus\":0}]}";



#if defined(WIZ550io_WITH_MACADDRESS) // Use assigned MAC address of WIZ550io
;
#else
byte myMac[] = { 0xAA, 0x06, 0xAA, 0xAA, 0xAA, 0x10 };
#endif 

EthernetClient client;


int falseConnectNum = 1;
bool isConnected = false;

IPAddress MY_IP(192, 168, 1, 20);

void SetUpMsg();
void ParseTheMsg();

void initMcu();
void heartbeats();

StaticJsonDocument<400> msg;
char *testM;

// Define Functions below here or use other .ino or cpp files
//


// The setup() function runs once each time the micro-controller starts
void setup()
{
  Serial.begin(9600);
  delay(400);
#if defined(WIZ550io_WITH_MACADDRESS) 
  Serial.println("no mac");
  Ethernet.begin(MY_IP);
#else
  Ethernet.begin(myMac, MY_IP);
  Serial.println("mac");
#endif
  delay(100);
  //initMcu();
}
/*
  {
  "MsgType": "Heart",
  "MsgContent" : {
    "Mcuid":"A1",
    "McuStatus" : 0
  }
  }
*/
void ParseTheMsg(char argv[])
{
  Serial.println("get to the parse function");
  for (int i = 0 ; i<200;i++){Serial.print(argv[i]);}Serial.println("");
  
  if (isMsgDeal == false)
  {
    const size_t capacity = JSON_ARRAY_SIZE(2) + 3 * JSON_OBJECT_SIZE(2) + 100;
    DynamicJsonDocument doc1(capacity);


    deserializeJson(doc1, argv);

    const char* MsgType = doc1["MsgType"]; // "LightControl"

    Serial.println("---------------");
    Serial.print(MsgType);
    Serial.println("");
    // According to the MsgType desides whitch pattern of parsing will be chosen
    if (MsgType == "LightControl")
    {


      const char* MsgContent_0_LightID = doc1["MsgContent"][0]["LightID"]; // "L_1"
      int MsgContent_0_LightStatus = doc1["MsgContent"][0]["LightStatus"]; // 0

      const char* MsgContent_1_LightID_ = doc1["MsgContent"][1]["LightID "]; // "L_1 "
      int MsgContent_1_LightStatus = doc1["MsgContent"][1]["LightStatus"]; // 0
    }
    if (MsgType[0] == 'D')
    {
      int dorStatus = doc1["DoorStatus"];
     // Serial.print("doorstatus  is   :    ");
     // Serial.println(dorStatus);
      delay(100);
      client.print("i get the message ---");
      //client.print(dorStatus);
      client.print("bye");
    }
    isMsgDeal = true;
  }
  
  /*
  {
    "MsgType": "DoorControl",
      "MsgContent" : {
      "DoorStatus": 1
    }
  }
  */
  
}
void SetUpHeartMsg() //standerd heartbeat
{
  msg[FirstTitle] = "Heart";
  JsonObject NextLevel = msg.createNestedObject(SecondTitle);

  NextLevel["McuId"] = "A_1";
  NextLevel["McuStatus"] = 1;


  //serializeJsonPretty(msg,Serial);


}
// Add the main program code into the continuous loop() function
void loop()
{
  delay(100);
  time_now = millis();
  char *a = clientConnect();
  heartbeats();

}

void heartbeats() 
{
  if ((time_now - heart_time_mark)>= HEARTBEAT_TIME)
  {
    Serial.println("heart beating one time");
    heart_time_mark = time_now;
    client.print("{\"MsgType\":\"Heart\",\"MsgContent\":{\"Mcuid\":\"A1\",\"McuStatus\" : 0}}");
    //client.print("hello world");
  }
}
char * readPage() {
  char inString[1024];
  int stringPos = 0;
  int loopcounte = 0;
  bool startRead = false;
  //read the page, and capture & return everything between '<' and '>'
  memset(&inString, 0, sizeof(inString)); //clear inString memory
 //Serial.println("stuck in readPage");
  if (client.available()) {
       while (true) {
       loopcounte ++;
       if (loopcounte >=1024){memset(&inString, 0, sizeof(inString));loopcounte = 0 ; Serial.println("bad msg");return inString;}
       char c = client.read();
          //Serial.println("stuck in readPage--char c");     
          //delay(10);
//        Serial.print("常规字符  c : ");
//        Serial.print(c);
//        Serial.println("");
      if (c == '{') {                   //'{' is our begining character//Ready to start reading the part 
        startRead = true;
        //delay(10);
        //Serial.println("读到了左括号");
        inString[stringPos] = c;
        delay(10);
        stringPos++;
        read_Page_Mode = read_Page_Mode + 1;

       }
      else if (startRead) {
        if (read_Page_Mode == 0) // end  and goto msg analyse
        {
          //delay(10);
          Serial.println("读结束了");
          client.flush();
          //  cmdRoot = parseServerCmd(inString, &cmd);    // there is where parse the Msg
            //Serial.println(inString);
            Serial.println("end of read msg");
          for (size_t i = 0; i <= stringPos; i++)
          {
            Serial.print(inString[i]);
          }
          Serial.println("");
          startRead = false;
          isMsgDeal = false;
          ParseTheMsg(inString);//   传参 char 数组 进来
          
          return inString;
        }else {
          //delay(10);

          if (c == '}')
          {
           // Serial.println("读到了右括号");
            read_Page_Mode = read_Page_Mode - 1;
          }
          
          inString[stringPos] = c;
          stringPos++;
        }
      }
    }
  }
}
char* clientConnect() {
  if (!client.connected()) {
    client.stop();
    Serial.println("begin");
    if (client.connect(SERVER_IP, SERVER_PORT)) {
      Serial.println("end");


      Serial.println("success!");
      isConnected = true;
    }
    else {
      isConnected = false;
      falseConnectNum++;
      Serial.println("failed!");
      if (falseConnectNum > 3) {
        Serial.println("false time over 3");
      }
    }
  }
  else {
    char *c = readPage();
    return c;
  }
}
