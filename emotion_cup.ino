#include <HX711_ADC.h>
#include <EEPROM.h>
#include <CuteBuzzerSounds.h>

//int buzzer = 2; // 用Pin0 輸出方波至蜂鳴器
#define BUZZER_PIN 2

//HX711 constructor (dout pin, sck pin):
HX711_ADC LoadCell(4, 5);

const int eepromAdress = 0;

long t;
int weigh;
int currentWeigh;
int secondWeigh;

//=============================================
int R[] = {2,7,A5,A0,13,A4,12,A2}; //行
int C[] = {6,11,10,3,A3,A1,8,9};   //列
 
int sad[8][8] =       //sad icon
{
  {0,0,0,0,0,0,0,0},
  {1,1,1,0,0,1,1,1},
  {1,0,1,0,0,1,0,1},
  {1,1,1,0,0,1,1,1},
  {0,0,0,1,1,0,0,0},
  {0,0,1,0,0,1,0,0},
  {0,1,0,0,0,0,1,0},
  {0,0,0,0,0,0,0,0}
};
 
int happy[8][8] =      //happy icon
{
  {0,0,0,0,0,0,0,0},
  {1,1,1,0,0,1,1,1},
  {1,0,1,0,0,1,0,1},
  {1,1,1,0,0,1,1,1},
  {0,0,1,1,1,1,0,0},
  {0,1,0,0,0,0,1,0},
  {0,0,1,1,1,1,0,0},
  {0,0,0,0,0,0,0,0}
};

int brink[8][8] =    //眨眼icon
{
  {0,0,0,0,0,0,0,0},
  {1,1,1,0,0,0,0,0},
  {1,0,1,0,0,1,1,1},
  {1,1,1,0,0,0,0,0},
  {0,0,0,0,0,0,0,0},
  {0,1,0,0,0,0,1,0},
  {0,0,1,1,1,1,0,0},
  {0,0,0,0,0,0,0,0}
};


byte emoji = 1;
unsigned long changeEmoji = 0;

void setup() {
  // put your setup code here, to run once:

  int calValue; // calibration value
  calValue = 50; // uncomment this if you want to set this value in the sketch 
  Serial.begin(9600); delay(10);
  Serial.println();
  Serial.println("Starting...");
  LoadCell.begin();
  long stabilisingtime = 2000; // tare preciscion can be improved by adding a few seconds of stabilising time
  LoadCell.start(stabilisingtime);
  if(LoadCell.getTareTimeoutFlag()) {
    Serial.println("Tare timeout, check MCU>HX711 wiring and pin designations");
  }
  else {
    LoadCell.setCalFactor(calValue); // set calibration value (float)
    Serial.println("Startup + tare is complete");
  }

  weigh = LoadCell.getData();

  for(int i=0; i<8; i++)
  {
    pinMode(R[i],OUTPUT);
    pinMode(C[i],OUTPUT);
    digitalWrite(R[i], LOW);
    digitalWrite(C[i], LOW);
  }
  
  //蜂鳴器 setup
//  pinMode(buzzer,OUTPUT);
    cute.init(BUZZER_PIN); 
  
}

void loop() {
  // put your main code here, to run repeatedly:
  LoadCell.update();

  //get smoothed value from data set
  if (millis() > t + 20000) {
    currentWeigh = LoadCell.getData();
    Serial.print("Current weight is: ");
    Serial.println(currentWeigh);
    t = millis();
  }
  
  if(emoji==1)
    DisplayHappy();
  else if(emoji == 2)
    DisplaySad();
    
  if(millis() - changeEmoji >= 20000){
    secondWeigh = LoadCell.getData();
    Serial.print("Second weight is: ");
    Serial.println(secondWeigh);
    if(secondWeigh - currentWeigh < 0) emoji = 1;   //測到的重量沒有改變，就哭哭；一直沒裝水，也哭哭
    else emoji = 2, cute.play(S_HAPPY);;
    changeEmoji = millis();
  }

  //設定蜂鳴器響的條件
//  if (millis() > t + 20000) {
//    if(emoji == 2)
//    Serial.println("BUZZER!");
//    cute.play(S_HAPPY);
}

void DisplaySad()  //顯示哭臉的設定
{
  for (int i=0; i<8; i++){  //每一個 R 跑一次
    for (int j=0; j<8; j++){  //每個 C 跑一次
      if (sad[i][j]==1) {  //若位置正好是 1，要亮燈
        digitalWrite(R[i], HIGH); //開Ｒ排燈
        digitalWrite(C[j], LOW);  //開Ｃ排燈
        digitalWrite(R[i], LOW); //關Ｒ排燈
        digitalWrite(C[j], HIGH);  //關Ｃ排燈
      }
    }
  }
}

void DisplayHappy()  //顯示笑臉的設定
{
  for (int i=0; i<8; i++){  //每一個 R 跑一次
    for (int j=0; j<8; j++){  //每個 C 跑一次
      if (happy[i][j]==1) {  //若位置正好是 1，要亮燈
        digitalWrite(R[i], HIGH); //開Ｒ排燈
        digitalWrite(C[j], LOW);  //開Ｃ排燈
        digitalWrite(R[i], LOW); //關Ｒ排燈
        digitalWrite(C[j], HIGH);  //關Ｃ排燈
      }
    }
  }
}
