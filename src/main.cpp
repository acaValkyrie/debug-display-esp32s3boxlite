#include <Arduino.h>
#include "lgfx.h"

const int led_pin = 14;
const int switch_pin = 1;

class Console{
  private:
    LGFX display;
    int max_column;
    int max_row;
    int row;
    int textSize;
    int textHeight;
    int textWidth;
    void print_row(const String str){
      // strの右方向を空白で埋めて出力
      int len = str.length();
      int space_len = this->max_column - len%this->max_column;
      String print_str = str;
      for(int i=0; i<space_len; i++){ print_str += ' '; }
      this->display.print(print_str);
      row+=this->textHeight * (len/this->max_column + 1);
      if(row >= this->display.height()){
        row = 0;
      }
    }

  public:
    Console(int textSize){
      this->display.init();
      this->display.setTextSize(textSize);
      this->textSize = textSize;
      this->textHeight = 8*textSize;
      this->textWidth = 6*textSize;
      this->max_row = this->display.height()/textHeight;
      this->max_column = this->display.width()/textWidth;
      this->row = 0;
    }

    void print(const String str){
      this->display.startWrite();
      int strlen = str.length();
      String str_out = "";
      for(int i=0; i<strlen; i++){
        if(str[i] == '\n'){
          this->display.setCursor(0, row);
          this->print_row(str_out);
          str_out = "";
        }else{
          str_out = str_out + str[i];
          if(i==strlen-1){
            this->display.setCursor(0, row);
            this->print_row(str_out);
          }
        }
      }
      this->display.endWrite();
      this->display.display();
    }
};

Console console(1);

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(1);
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW);
}

int count = 0;
#define NONE   0
#define LEFT   1
#define CENTER 2
#define RIGHT  3

int getSwitchState(){
  const int threshold_width = 250;
  const int left = 3067;
  const int center = 2505;
  const int right = 997;
  int val = analogRead(switch_pin);
  if (left + threshold_width > val && val > left - threshold_width){
    return LEFT;
  }else if (center + threshold_width > val && val > center - threshold_width){
    return CENTER;
  }else if (right + threshold_width > val && val > right - threshold_width){
    return RIGHT;
  }else{
    return NONE;
  }
}

void loop() {
  if(Serial.available() <= 0) return;
  String str = Serial.readStringUntil('\n');
  console.print(str);
}
