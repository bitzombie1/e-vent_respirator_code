void outputLCD(String outStr, uint8_t red, uint8_t green, uint8_t blue ) {
  // clear screen
  LCD.write(0xFE);
  LCD.write(0x58);
  LCD.flush();

  // cursor at home
  LCD.write(0xFE);
  LCD.write(0x48);
  LCD.flush();

  // say things
  LCD.print(outStr);
  LCD.flush();

  //BG color 
  LCD.write(0xFE);
  LCD.write(0xD0);
  LCD.write(red);
  LCD.write(green);
  LCD.write(blue);
  LCD.flush();
}
