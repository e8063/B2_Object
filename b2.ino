#include <Illumination.h>//IlluminationLibraryをインクルードします
Illumination lib;

bool PIN[7];
int count = 0;

void setup() {
  //void setup内で必ず始めに呼び出す
  lib.setup(true);//引数をtrueにすると2番(SSR_1)から9番(SSR_8)のピンを自動的に初期化
  pinMode(A0,INPUT);
  randomSeed(analogRead(A1));//random関数のシードをセット
  lib.setPattern(pattern,'P');//パターン点灯888
  lib.setPattern(all_off,'0');//全消灯
  lib.setPattern(all_on,'1');//全点灯
}

void loop() {
  button();
  lib.loopAction();//loop関数内で必ず実行
}

bool readbutton(){//チャタリング防止機能を付与したdigitalRead(A0)
  int digital_on=0;
  for(int i=0;i<=50;i++){
    if(digitalRead(A0))
      digital_on++;
    delay(1);
    }
  if(digital_on > 25)
    return true;
  return false;
}

void button(){//ボタンの処理を行う関数
  static bool button_stack = false;//以前のボタンの状態を記録
  if(readbutton()){
    if(!button_stack){
      count++;
      button_stack = true;
    }
  }
  else{//ボタンが押されなくなったら保持状態を解除
    button_stack = false;
  }
}

void randomAction(){
 for(int i=0;i<=7;i++)
    PIN[i] = random(2);//ピンの状態をランダムに選択
}

//関数を宣言し、点灯パターンを記述
void pattern(){
  while(true){
    randomAction();//ピンの状態をランダムに選択
    switch(count){
      case 0://ボタンが押されていない状態
        for(int i=0;i<=7;i++){
          digitalWrite(i+2,PIN[i]);//フルでランダム
        }
        break;
      case 1://ボタンが1回押されている状態
        for(int i=3;i<=7;i++){
          digitalWrite(i+2,PIN[i]);//2,3,4,を固定、他はランダム
        }
        break;
      case 2://ボタンが2回押されている状態
        for(int i=5;i<=7;i++){
          digitalWrite(i+2,PIN[i]);//2,3,4,5,6を固定、他はランダム
        }
        break;

      case 3://ボタンが3回押されている状態
        //待機(なにもしない) 
        break;
        
      default :
        count = 0;
        break;
    }
    lib.delay(500);
  }
}

void all_off(){//全消灯
  for(int i=2;i<=9;i++){
      digitalWrite(i,LOW);
  }
  lib.wait();//次の制御コマンドを受信するまで待機
}

void all_on(){//全点灯
  for(int i=2;i<=9;i++){
      digitalWrite(i,HIGH);
  }
  lib.wait();//次の制御コマンドを受信するまで待機
}


