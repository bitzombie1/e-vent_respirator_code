#include <DynamixelWorkbench.h>
// PIN assignments
#define PIEZO_PIN A0
 
#define POT_1_PIN A1      // tidal volume 
#define POT_2_PIN A2      // respiratory Rate
#define POT_3_PIN A3      // I/E ratio 
#define POT_4_PIN A4      // assist control threshhold 
#define POT_5_PIN A5      // ? 



// Dynamixel values 
#define DEVICE_NAME "3"     //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP
#define BAUDRATE  2000000
#define DXL_ID    1
#define OPENPOS 275         // our open (min) pos . value 
#define CLOSEPOS 752        // our close (MAX) pos value

// LCD values 
#define LCD  Serial1

DynamixelWorkbench dxl_wb;
uint8_t dxl_id = DXL_ID;

unsigned long now_t, prev_t, delta_t ;      // our loop timing vars 

int rest_t, squeez_t, relax_t;    // our current timmer holding vars
int squeez_ms = 3000, relax_ms = 500, rest_ms= 4000; // actual time in each state set by knobs
int tidal_pos = OPENPOS;           // actuall position of servo 1 
int state =0;                     // 0 = resting open(back porch), 1 = squeezing, 2 = relaxing 

// these values are actually displayed on the LCD
int tidal = 200;      // tidal volume or how far the bag is squezed 200 - 800ml
int respRate = 8;        // respiratory Rate (8 to 40 breaths/min)


int i_e_ratio = 1;        // I/E ratio (1:1 to 1:4)
int assistContThresh = 0;  // Assist Control Threshold 

void setup() 
{
  Serial.begin(57600);   // our serial debug output    
  // while(!Serial); // Wait for Opening Serial Monitor
  setupDXL();         // setup dynamixel 
  setupLCD();         // set up the 2X16 LCD display
  chkPots();
  int currVal = printPos(); // chk POS and move to open POS if needed 
  if( currVal != -1){
    if(currVal > OPENPOS){
      homeToOpen(currVal);
    }
  }
  setupBuzzer(); // make a tone that setup is done
}

void loop(){
  now_t = millis();
  if(now_t < prev_t){delta_t = now_t + (4294967295 - prev_t);} //millis rollover check
  else{delta_t = now_t - prev_t;}
  delay(1);
  chkPots();
  updateState();
  
  //dxl_wb.itemWrite(dxl_id, "Profile_Velocity", 3000);
  //dxl_wb.goalPosition(dxl_id, (int32_t)OPENPOS); // go to open 
  //delay(3000);  //delay(map(respRate,10,40,4000,500));
  //dxl_wb.itemWrite(dxl_id, "Profile_Velocity", 1000);
  //dxl_wb.goalPosition(dxl_id, (int32_t)CLOSEPOS);  // go to close min needs 1 sec. 
  //delay(1000);
  
  prev_t = now_t;
}

void updateState(){
  if(state == 0){
    if(rest_t > rest_ms){           // go to state 1
      state = 1;                                                  // set state to 1 
     Serial.println( dxl_wb.itemWrite(dxl_id, "Profile_Velocity", squeez_ms));    // set speed in ms
      dxl_wb.goalPosition(dxl_id, (int32_t)tidal_pos);            // set pos
      squeez_t =0;                                                //set timmer to zero
    }
    else{ rest_t += delta_t;}
  }
  else if(state == 1){
    if(squeez_t > squeez_ms){        // go to state 2
      state = 2;                                                  // set state to 2
      dxl_wb.itemWrite(dxl_id, "Profile_Velocity", relax_ms);    // set speed in ms
      dxl_wb.goalPosition(dxl_id, (int32_t)OPENPOS);            // set pos to open
      relax_t =0;
    }
    else{ squeez_t += delta_t;}
  }
  else if(state == 2){
    if(relax_t > relax_ms){         // go back to state 0
      state = 0; 
      rest_t = 0;
    }
    else{relax_t += delta_t;}
  }
}

void homeToOpen(int currPos){
   for(int i=currPos;i >275;i--){
    dxl_wb.goalPosition(dxl_id, (int32_t)i);
    delay(1);
  }
}
int printPos(){
  int32_t get_data = 0;
   bool result = dxl_wb.itemRead(1, "Present_Position", &get_data);
  if (result == false)
  {
    //Serial.println(log);
    Serial.println("Failed to get present position");
    return -1;
  }
  else
  {
    Serial.print("Succeed to get present position(value1 : ");
    Serial.print(get_data);
    Serial.println(")");
    return get_data;
  }
}

