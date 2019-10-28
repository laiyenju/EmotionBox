//引入資料庫
#include <HX711_ADC.h>  //HX711 套件庫
#include <EEPROM.h>
#include <CuteBuzzerSounds.h> //提供給蜂鳴器採用的聲音庫，可以實現豐富的音效，卻能避開 delay 寫法

// 配合 CuteBuzzerSounds 資料庫的方式，將 Pin2 設定為蜂鳴器輸出口
#define BUZZER_PIN 2

//HX711 constructor (dout pin, sck pin):
HX711_ADC LoadCell(4, 5);

const int eepromAdress = 0;

long t;
int currentWeigh;
int secondWeigh;

//=============================================
int R[] = {2,7,A5,A0,13,A4,12,A2}; //行
int C[] = {6,11,10,3,A3,A1,8,9};   //列
 
int sad[8][8] =       //哭臉表情
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
 
int happy[8][8] =      //笑臉表情
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


byte emoji = 1;
unsigned long changeEmoji = 0;

void setup() {

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
  
   //蜂鳴器初始化
    cute.init(BUZZER_PIN); 
  
}

void loop() {

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
    else emoji = 2, cute.play(S_HAPPY);;  //顯示哭臉的同時，蜂鳴器發出 S_HAPPY 的聲音
    changeEmoji = millis();
  }
 
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
