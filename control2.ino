#include <Servo.h>
#include <SoftwareSerial.h>    //包含软串口头文件，硬串口通信文件库系统自带String uart1_receive_buf = "";
/*******************************一些宏定义****************************************/
SoftwareSerial mySerial(A4,A5); //创建一个软串口的类，模拟引脚4,5分别代表 RX, TX引脚 AR多功能板
Servo myServo;
int angle;
int myId, myPwm, myTime, destination;
char cmd_return[100];//这里的cmd_return的字符长度要足够的大，根据舵机个数定，大小 = 15*个数+10
String inputString = "";  
boolean stringComplete = false;  

void setup() {
    Serial.begin(9600);           //硬件串口
    mySerial.begin(9600);         //设置软串口波特率 <--原装波特率默认为115200，这里是使用initialize修改后的波特率
    Serial.println("请输入舵机ID和角度（格式：ID,角度）：");      
}

void loop() {
                
        while (Serial.available()>0) {
        char inChar = (char)Serial.read(); 
        if (inChar == '\n') { 
            stringComplete = true;
        } else {
            inputString += inChar; 
        }
        }

        if (stringComplete){
          int comma_index = inputString.indexOf(',');
          if (comma_index > 0){
            String id_string = inputString.substring(0, comma_index);
            String angle_string = inputString.substring(comma_index + 1); 

            myId = id_string.toInt(); 
            angle = angle_string.toInt(); 

            if (angle >= 40 && angle <= 140) {  

              destination = angle/9.0*100 + 500;
              Serial.print("舵机");
              Serial.print(myId);
              Serial.print("旋转到角度：");
              Serial.println(angle);
              sprintf(cmd_return, "#%03dP%04dT%04d!", myId, destination, 1000);
              Serial.print(cmd_return);
              mySerial.print(cmd_return);

            } else if(angle<40){
                angle = 40;
                destination = angle/9.0*100 + 500;
                sprintf(cmd_return, "#%03dP%04dT%04d!", myId, destination, 1000);
                Serial.print(cmd_return);
                mySerial.print(cmd_return);
            } else {
              angle = 140;
              destination = angle/9.0*100 + 500;
              sprintf(cmd_return, "#%03dP%04dT%04d!", myId, destination, 1000);
              Serial.print(cmd_return);
              mySerial.print(cmd_return);
            }
          }else{
            Serial.println("输入格式无效，请使用格式：ID,角度");
          }
          inputString = "";
          stringComplete = false;
          Serial.println("请输入舵机ID和角度（格式：ID,角度）：");

          while (Serial.available() > 0) {
            Serial.read();
          }
        }
}






