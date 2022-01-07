/* 
 *  
 * Eindwerk Patrik Horemans
 * ARD-2
 * Besturen van een autootje met 4 DC motoren via BT
 * 
 * 
 */
int s=100;
int RX_BT = 15;
int TX_BT = 13;
int frequency = 100;
int duty_cycle= 0;
int pCLK = 14;
int pData = 27;
int pLatch = 26;
int pPWM1 = 25;
int pPWM2 = 33;
int pPWM3 = 32;
int pPWM4 = 12;
byte ch;
String sdata = "";

//bit order to send
//01 02 03 04 05 06 07 08
//3B 4F 3F 2B 1F 1B 2F 4B (motor-direction example 4F=motor4-Forward)
int M_Forward  = 0b01101010;
int M_Backward = 0b10010101;
int M_Stop = 0b00000000;

void clk(){
    delay(s);
    digitalWrite(pCLK, 1);
    delay(s);
    digitalWrite(pCLK, 0);
    delay(s);
}
    
void latch(){
    digitalWrite(pLatch,1);
    delay(s);
    digitalWrite(pLatch,0);
    delay(s);
}
    
void motor_send(int tempByte){
    motor_stop();
    int tempBit=0;
    for(int bit=0;bit<8;bit ++){
        tempBit = 0b10000000 & tempByte;
        if(tempBit){
            digitalWrite(pData,1);
            }
        else{
            digitalWrite(pData,0);
            }
        delay(s);
        clk();
        tempByte = (tempByte << 1);
    }
    latch();
}
    
void motor_stop(){
  ledcWrite(1,0);
  ledcWrite(2,0);
  ledcWrite(3,0);
  ledcWrite(4,0);
}

void motorL(int theDuty){
  ledcWrite(1,theDuty);
  ledcWrite(4,theDuty);
}

void motorR(int theDuty){
  ledcWrite(2,theDuty);
  ledcWrite(3,theDuty);

}


void setup(){
    Serial.begin(115200);
    Serial1.begin(9600, SERIAL_8N1,RX_BT,TX_BT);
    Serial1.println("Car is alive!\n");
    pinMode(pCLK, OUTPUT);
    pinMode(pData, OUTPUT);
    pinMode(pLatch, OUTPUT);
    ledcAttachPin(pPWM1,1);
    ledcAttachPin(pPWM2,2);
    ledcAttachPin(pPWM3,3);
    ledcAttachPin(pPWM4,4);
    ledcSetup(1,frequency,8);
    ledcSetup(2,frequency,8);
    ledcSetup(3,frequency,8);
    ledcSetup(4,frequency,8);
    motor_send(M_Stop);
}

void loop(){
   if (Serial1.available()) 
   {
       ch = Serial1.read();
       switch(ch){
        case 'S':
          Serial.println("STOP");
          motor_stop();
          sdata = "";
          break;
        case 'F':
          motor_send(M_Forward);
          Serial.println("Forward");
          sdata = "";
          break;
        case 'B':
          motor_send(M_Backward);
          Serial.println("Backward");
          sdata = "";
          break;
        case 'L':
          duty_cycle = sdata.toInt();
          motorL(duty_cycle);
          Serial.print("Left ");Serial.println(String(duty_cycle));
          sdata = "";
          break;
        case 'R':
          duty_cycle = sdata.toInt();
          motorR(duty_cycle);
          Serial.print("Right ");Serial.println(String(duty_cycle));
          sdata = "";
          break;
        default:
          sdata = sdata + (char)ch;
          break;
        }
   }
}
