void setupBuzzer(){
  // just make a tone on startup 
  tone(PIEZO_PIN, 440, 500);
}

void setupDXL(){
  const char *log;
  bool result = false;
  uint16_t model_number = 0;

  Serial.println(DEVICE_NAME);

  result = dxl_wb.init(DEVICE_NAME, BAUDRATE, &log);
  if (result == false){
    Serial.println(log);
    Serial.println("Failed to init");}
  else{
    Serial.print("Succeeded to init : ");
    Serial.println(BAUDRATE);}

    delay(10);
  result = dxl_wb.ping(dxl_id, &model_number, &log);
  if (result == false){
    Serial.println(log);
    Serial.println("Failed to ping");}
  else{
    Serial.println("Succeeded to ping");
    Serial.print("id : ");
    Serial.print(dxl_id);
    Serial.print(" model_number : ");
    Serial.println(model_number);}
    delay(10);
  dxl_wb.torqueOff(dxl_id);
  //dxl_wb.torqueOff(2);
  delay(10);
  /*
  result = dxl_wb.itemWrite(dxl_id, "Drive_Mode", 0b00000100, &log); //SET SERVO#1 TO TIME BASED joint MODE
  if (result == false){
    Serial.println(log);
    Serial.println("Failed to change time mode");}
  else{Serial.println("Succeed to change timebased mode");}

  delay(10);
  result = dxl_wb.itemWrite(2, "Drive_Mode", 0b00000101, &log); //SET SERVO#1 TO TIME BASED joint MODE
  if (result == false){
    Serial.println(log);
    Serial.println("Failed to change time mode");}
  else{Serial.println("Succeed to change timebased mode");}
*/

  delay(10);
  result = dxl_wb.jointMode(dxl_id, 0,0, &log);
  if (result == false){
    Serial.println(log);
    Serial.println("Failed to change joint mode");}
  else{
    Serial.println("Succeed to change joint mode");
    Serial.println("Dynamixel is moving...");}
    dxl_wb.torqueOn(dxl_id);
   // dxl_wb.torqueOn(2);
}

void setupLCD() {
  LCD.begin(9600);
  delay(1500);

  LCD.write(0xFE);
  LCD.write(0xD1);
  LCD.write(16);  // 16 columns
  LCD.write(2);   // 2 rows
  LCD.flush();

  // set the contrast to max
  LCD.write(0xFE);
  LCD.write(0x50);
  LCD.write(200); //255
  LCD.flush();

  // set the brightness to max
  LCD.write(0xFE);
  LCD.write(0x99);
  LCD.write(150);
  LCD.flush();

  // clear screen
  LCD.write(0xFE);
  LCD.write(0x58);
  LCD.flush();

  // put cursor at home and say hello
  LCD.write(0xFE);
  LCD.write(0x48);
  LCD.flush();
  LCD.print("   E-VENT!");
  LCD.flush();
}
