/*
 * int tidal = 0;            // tidal volume or how far the bag is squezed 
int respRate = 10;        // respiratory Rate (10 to 40 breaths/min)
int i_e_ratio = 1;        // I/E ratio (1:1 to 1:4)
int assistContThresh = 0;  // Assist Control Threshold 
 */


void chkPots(){
  int val =0;
  val = analogRead(POT_1_PIN);
  tidal_pos = map(val,1024,0,OPENPOS,CLOSEPOS);
  tidal = map(tidal_pos,OPENPOS, CLOSEPOS,200, 800);

  
  val = analogRead(POT_2_PIN);
  respRate = map(val,1024,0,8,40);
  calcBPM(respRate);

  val = analogRead(PRESS_SENSE_PIN);
  pressure = map(val,102,904,-5,5); 
  Serial.println(val);
  
}
// find out the time in MS to take in each state 
void calcBPM(int rr){
  if(rr >= 8 && rr <12){
    squeez_ms = 3000;
    relax_ms = 500; 
    rest_ms = map(rr,8,11,4000,1500);
  }
  else if(rr >=12 && rr <=40){
    squeez_ms = map(rr,12,40,3000,1000);
    relax_ms = 500; 
    rest_ms = map(rr,12,40,1500,0);
  }
}

