//int vcc_pin=A0; 
int x_pin=A3;
int y_pin=A4;
int z_pin=A5;
//int gnd_pin_acc=A4;
int buzzer=9; // Need PWM digial pin for control on the volume
int magnet_pin=A6;
//int magnet_vss=A7; no need - conected directly to grnd
int magnet_threshold=30;

int red=2;
int green=4;
int blue=5;
int common_catode_led=6;
int x;
int y;
int z;
int m;
int threshold =10;
int init_x;
int init_y;
int init_z;
int reference_pin;
int reference_value;
int gravity_upper_value = 400;
int gravity_lower_value = 310;
int refernce_diff=50;
 void(* resetFunc) (void) = 0; //declare reset function @ address 0
void setup() {
  delay(1000);
  int init_stat;
  silent_all();
  Serial.begin(9600); //sets the baud rate at 9600 so we can check the values the microphone is obtaining on the Serial Monitor
  pinMode(buzzer, OUTPUT);   
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT); 
  pinMode(red, OUTPUT);
  pinMode(common_catode_led,OUTPUT);
 //  pinMode(vcc_pin,OUTPUT);
 //  pinMode(gnd_pin_acc,OUTPUT);
 //  pinMode(magnet_vss,OUTPUT);
   pinMode(magnet_pin,INPUT);
   digitalWrite(common_catode_led,1);      
   init_x = init_pin(x_pin);
   init_y = init_pin(y_pin);
   init_z = init_pin(z_pin);

 //  digitalWrite(vcc_pin,1);      
 //  digitalWrite(magnet_vss,0);      
 //  digitalWrite(gnd_pin_acc,0);

}
void loop(){
  silent_all();
   m= analogRead(magnet_pin);
    delay(500);
 //   Serial.print("m ");
 //   Serial.println(m);
    if (m<magnet_threshold) {
   //   Serial.print("m ");
   //   Serial.println(m);
      digitalWrite(green,0);      
      delay (500);
      digitalWrite(green,1);      
   }
  while (m <magnet_threshold) {
      m= analogRead(magnet_pin);
      delay (500);
     // Serial.print("m ");
     //  Serial.println(m);
       if (m > magnet_threshold) {
         digitalWrite(red,0);
         delay(500);
         digitalWrite(red,1);
         resetFunc(); //call reset 
       }

    }

 // Serial.print("m ");
  // Serial.println(m);

    value_check (init_x, x_pin) ;  //pin 14
     value_check (init_y, y_pin) ; // pin 15
     value_check (init_z, z_pin) ; // pin 16

    delay(500);

}
void alarm () {
     analogWrite(buzzer, 220);
     digitalWrite(red,0);
}

void silent_all () {

   analogWrite(buzzer, 255);
   digitalWrite(green,1);
   digitalWrite(blue, 1);
   digitalWrite(red, 1);  
}
void value_check (int init_value,int pin) {
    int sample;
    sample= analogRead(pin); 
  //  Serial.print("pin ");
 //   Serial.println(sample);
  if (pin != reference_pin ) {
     if (sample>init_value+threshold  || sample<init_value-threshold) {
        Serial.print("pin ");
        Serial.print(pin);
        Serial.print(" ");
       Serial.println(sample );
        alarm(); 
      }
  } else if (pin == reference_pin ) {
        int diff = sample - reference_value;
             if (abs(diff) > refernce_diff) {
                     silent_all();
          //            Serial.print("pin ");
          //           Serial.print(pin);
          //            Serial.print(" ");
          //            Serial.println(sample );
          //            Serial.println("RESET");
                    delay(2000);
                   resetFunc(); //call reset         
              }
   }
 }
 int init_pin (int pin) {
   int init_value;
   init_value= analogRead(pin);
   delay(10);
   if (init_value > gravity_upper_value || init_value < gravity_lower_value) {
      reference_pin = pin;   
      reference_value = init_value;
    //  Serial.print("refernce_pin: ");
   }
 //  Serial.print(pin);
 //  Serial.print(" ");
 //  Serial.println(init_value);
   return init_value;

 }

