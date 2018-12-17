//pump motor valve
#define pumpEnable01 6 //analogWriteなのでPWM
#define pumpEnable02 9 //analogWriteなのでPWM
//#define pumpEnable03 10 //analogWriteなのでPWM これミスってたかも
#define pumpEnable03 12 //analogWriteなのでPWM
#define pumpEnable04 11 //analogWriteなのでPWM
#define pumpMotor01 14
#define pumpMotor02 5 
//#define pumpMotor03 12 これミスってたかも
#define pumpMotor04 13 
#define valve01 2
#define valve02 7 //3だったがchange
#define valve03 4
#define valve04 8 //5だったがchange
#define led_B 3
//#define led_G 
#define led_R 10

int snail,anago,iso,jelly; 
boolean mad = false;
boolean mad_switch = false;
int R,G,B,H=0;
int snailIn,anagoIn,isoIn,jellyIn;
int snailInFlg,anagoInFlg,isoInFlg,jellyInFlg;
int release_=0;

void setup() {
  
  Serial.begin(9600);

  //-----↓pump setting---------
  pinMode(pumpEnable01,OUTPUT);
  pinMode(pumpMotor01,OUTPUT);
  pinMode(pumpEnable02,OUTPUT);
  pinMode(pumpMotor02,OUTPUT);
  pinMode(pumpEnable03,OUTPUT);
//  pinMode(pumpMotor03,OUTPUT);
  pinMode(pumpEnable04,OUTPUT);
  pinMode(pumpMotor04,OUTPUT);
  pinMode(valve01,OUTPUT);
  pinMode(valve02,OUTPUT);
  pinMode(valve03,OUTPUT);
  pinMode(valve04,OUTPUT);
  pinMode(led_R,OUTPUT);
//  pinMode(led_G,OUTPUT);
  pinMode(led_B,OUTPUT);

  snail = 0;
  anago = 30;
  iso = 70;
  jelly = 40;

  snailIn = 0;
  anagoIn = 0;
  isoIn = 0;
  jellyIn = 0;

  
  digitalWrite(valve01,HIGH);
  digitalWrite(valve02,HIGH);
  digitalWrite(valve03,HIGH);
  digitalWrite(valve04,HIGH);
  delay(4000);
  
}

void loop() {
//Serial.println((String)"H: " + H);
//Serial.println((String)"mad_switch: " + mad_switch);
//  test();
  pumpSafety();
  controll(); 
//turn_off();          
}

void controll(){
  
  sensor_check();
  
  if(mad == true){
    if(mad_switch == false){
      turn_up();
    }else{
      mad_mode();
    }
  }else{
    if(mad_switch == true){
      turn_off();
    }else{
      normal_mode();
    }
  }

  analogWrite(led_B,B) ;                
  analogWrite(led_R,R) ;               
//  analogWrite(led_G,G) ;  
}



//ポンプの安全を守る
void pumpSafety () {
  int pressure01 = analogRead(A0);
  int pressure02 = analogRead(A1);
  int pressure03 = analogRead(A2);
  int pressure04 = analogRead(A3);
  Serial.println((String)"pressure: " + pressure01 + ", " + pressure02 + ", " + pressure03 + ", " + pressure04);

  if(pressure01>400) digitalWrite(valve01,HIGH);
  if(pressure02>400) digitalWrite(valve02,HIGH);
  if(pressure03>400) digitalWrite(valve03,HIGH);
  if(pressure04>400) digitalWrite(valve04,HIGH);
}

void sensor_check(){
  
    int distance = analogRead(A4);
    Serial.println((String)"distance: " + distance);
    Serial.println((String)"mad: " + mad);
    if(distance > 400){
      mad = true;
    }else{
      mad = false;
    }
    
}

void turn_up(){
   /* HSVのH値を各ＬＥＤのアナログ出力値(0-255)に変換する処理 */

   /* H値(240-360) 青-紫-赤   */
   B = map(H,0,20,255,0) ;   // 青LED B←→R
   R = map(H,0,20,50,255) ;   // 青LED R←→B
   G= 0;
   
   if(H<20) H++;

   if(analogRead(A0) < 250 && snailInFlg == false){
    snailIn++;
    digitalWrite(valve01,LOW);
    digitalWrite(pumpMotor01, LOW);
    analogWrite(pumpEnable01, 180); //Max 255
   }else{
    snailInFlg = true;
    digitalWrite(valve01,LOW);
    digitalWrite(pumpMotor01, LOW);
    analogWrite(pumpEnable01, 0); //Max 255
   }
   
   if(analogRead(A1) < 300 && anagoInFlg == false){
    anagoIn++;
    digitalWrite(valve02,LOW);
    digitalWrite(pumpMotor02, LOW);
    analogWrite(pumpEnable02, 180); //Max 255
   }else{
    anagoInFlg = true;
    if(anagoInFlg){
    digitalWrite(valve02,LOW);
    digitalWrite(pumpMotor02, LOW);
    analogWrite(pumpEnable02, 0); //Max 255
    }
   }
   
   if(analogRead(A2) < 300 && isoInFlg == false){
    isoIn++;
    digitalWrite(valve03,LOW);
//    digitalWrite(pumpMotor03, HIGH);
    analogWrite(pumpEnable03, 130); //Max 255
   }else{
    isoInFlg = true;
    if(isoInFlg){
    digitalWrite(valve03,LOW);
//    digitalWrite(pumpMotor03, LOW);
    analogWrite(pumpEnable03, 0); //Max 255
    }
   }
   
   if(analogRead(A3) < 140 && jellyInFlg == false){
    jellyIn++;
    digitalWrite(valve04,LOW);
    digitalWrite(pumpMotor04, HIGH);
    analogWrite(pumpEnable04, 0); //Max 255
   }else{
    jellyInFlg = true;
    if(jellyInFlg){
    digitalWrite(valve04,LOW);
    digitalWrite(pumpMotor04, LOW);
    analogWrite(pumpEnable04, 0); //Max 255
    }
   }
   
   if(snailInFlg == true && anagoInFlg == true && isoInFlg == true && jellyInFlg == true ){
      mad_switch = true;
      H=0;
   }
}

void turn_off(){
   /* HSVのH値を各ＬＥＤのアナログ出力値(0-255)に変換する処理 */

   for(H=20;H>0;H--){
    /* H値(240-360) 青-紫-赤   */
   B = map(H,0,20,255,0) ;   // 青LED B←→R
   R = map(H,0,20,50,255) ;   // 青LED R←→B
   G= 0;
  analogWrite(led_B,B) ;                
  analogWrite(led_R,R) ;    
  
    digitalWrite(valve01,HIGH);
    digitalWrite(valve02,HIGH);
    digitalWrite(valve03,HIGH);
    digitalWrite(valve04,HIGH);

    delay(40);
   }
   
    snailIn = 0;
    anagoIn = 0;
    isoIn = 0;
    jellyIn = 0;
    snailInFlg = false;
    anagoInFlg = false;
    isoInFlg = false;
    jellyInFlg = false;
    mad_switch = false;
    H = 0;

    
    digitalWrite(valve01,HIGH);
    digitalWrite(valve02,HIGH);
    digitalWrite(valve03,HIGH);
    digitalWrite(valve04,HIGH);
    delay(2000);
}

void mad_mode(){
   R = 255;
   G = 0;
   B = 0;
   H = 0;
}

void normal_mode(){
  

    snail_motion(snail);
    anago_motion(anago);
    iso_motion(iso);
    jelly_motion(jelly);

    snail++;
    anago++;
    iso++;
    jelly++;

    R = 50;
    G = 0;
    B = 255;

    H = 0;
}



void test() {
    digitalWrite(pumpMotor01, LOW);
    analogWrite(pumpEnable01, 200); //Max 255
//    digitalWrite(pumpMotor02, LOW);
    analogWrite(pumpEnable02, 200); //Max 255
//    digitalWrite(pumpMotor03, HIGH);
    analogWrite(pumpEnable03, 200); //Max 255
    digitalWrite(pumpMotor04, LOW);
    analogWrite(pumpEnable04, 0); //Max 255
}

void snail_motion(int count) {
  if(count < 60) {
    digitalWrite(valve01,LOW);
    digitalWrite(pumpMotor01, LOW);
    analogWrite(pumpEnable01, 180); //Max 255
  }else{
    digitalWrite(valve01,HIGH);
    digitalWrite(pumpMotor01, LOW);
    analogWrite(pumpEnable01, 0); //Max 255
    if(count>120) snail = 0;
  }
}
void anago_motion(int count) {
  if(count < 45) {
    digitalWrite(valve02,LOW);
    digitalWrite(pumpMotor02, LOW);
    analogWrite(pumpEnable02, 180); //Max 255
  }else{
    digitalWrite(valve02,HIGH);
    digitalWrite(pumpMotor02, LOW);
    analogWrite(pumpEnable02, 0); //Max 255
    if(count>100) anago = 0;
  }
}
void iso_motion(int count) {
  if(count < 20) {
    digitalWrite(valve03,LOW);
//    digitalWrite(pumpMotor03, HIGH);
    analogWrite(pumpEnable03, 200); //Max 255
  }else{
    digitalWrite(valve03,HIGH);
//    digitalWrite(pumpMotor03, LOW);
    analogWrite(pumpEnable03, 0); //Max 255
    if(count>70) iso = 0;
  }
}
void jelly_motion(int count) {
  if(count < 20) {
    digitalWrite(valve04,LOW);
    digitalWrite(pumpMotor04, HIGH);
    analogWrite(pumpEnable04, 0); //Max 255
  }else{
    digitalWrite(valve04,HIGH);
    digitalWrite(pumpMotor04, LOW);
    analogWrite(pumpEnable04, 0); //Max 255
    if(count>100) jelly = 0;
  }
}

