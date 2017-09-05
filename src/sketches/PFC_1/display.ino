void initTouch(){
  dispx = 240;
  dispy = 320;
  text_y_center = (dispy / 2) - 6;
  
////  button.initButton(&myGLCD, 50, 50, 70, 40, ILI9341_DARKCYAN, ILI9341_BLUE, ILI9341_GREENYELLOW, "BUTTON", 2);
//  button.initButtonAsKey(button.ALPHA_NUM_KEY, "2", "two", 60, 100);
//  button.drawButton();
//
//  button2.initButtonAsKey(button.ALPHA_NUM_KEY, "3", "three", 160, 100);
//  button2.drawButton();
}



void waitForTouch()
{
  while (myTouch.dataAvailable() == true) { yield(); }
  while (myTouch.dataAvailable() == false) { yield(); }
  while (myTouch.dataAvailable() == true) { yield(); }
}

void drawCrossHair(int x, int y, int color)
{
  myGLCD.drawRect(x - 10, y - 10, 20, 20, color);
  myGLCD.drawLine(x - 5, y, x + 5, y, color);
  myGLCD.drawLine(x, y - 5, x, y + 5, color);
}

void readCoordinates()
{
  int iter = 5000;
  int failcount = 0;
  int cnt = 0;
  uint32_t tx = 0;
  uint32_t ty = 0;
  boolean OK = false;

  while (OK == false)
  {
    //myGLCD.setColor(255, 255, 255);
    myGLCD.setTextColor(ILI9341_BLACK);
    myGLCD.setCursor(100, 50);
    myGLCD.print("*  HOLD!  *");
    myGLCD.setTextColor(ILI9341_WHITE);
    myGLCD.setCursor(100, 50);
    myGLCD.print("*  PRESS  *");
    while (myTouch.dataAvailable() == false) { yield(); }
    myGLCD.setTextColor(ILI9341_BLACK);
    myGLCD.setCursor(100, 50);
    myGLCD.print("*  PRESS  *");
    myGLCD.setTextColor(ILI9341_WHITE);
    myGLCD.setCursor(100, 50);
    myGLCD.print("*  HOLD!  *");
    while ((myTouch.dataAvailable() == true) && (cnt<iter) && (failcount<10000))
    {
      yield();
      myTouch.calibrateRead();
      if (!((myTouch.TP_X == 65535) || (myTouch.TP_Y == 65535)))
      {
        tx += myTouch.TP_X;
        ty += myTouch.TP_Y;
        cnt++;
      }
      else
        failcount++;
    }
    if (cnt >= iter)
    {
      OK = true;
    }
    else
    {
      tx = 0;
      ty = 0;
      cnt = 0;
    }
    if (failcount >= 10000)
      fail();
  }
  cx = tx / iter;
  cy = ty / iter;
}

void calibrate(int x, int y, int i)
{
  drawCrossHair(x, y, myGLCD.color565(255, 255, 255));
  readCoordinates();
  myGLCD.setTextColor(ILI9341_BLACK);
  myGLCD.setCursor(100, 50);
  myGLCD.print("*  HOLD!  *");
  myGLCD.setTextColor(ILI9341_WHITE);
  myGLCD.setCursor(100, 50);
  myGLCD.println("* RELEASE *");
  drawCrossHair(x, y, myGLCD.color565(80, 80, 80));
  rx[i] = cx;
  ry[i] = cy;
  while (myTouch.dataAvailable() == true) { yield(); }
  myGLCD.setTextColor(ILI9341_BLACK);
  myGLCD.setCursor(100, 50);
  myGLCD.println("* RELEASE *");
}

void toHex(uint32_t num)
{
  buf[0] = '0';
  buf[1] = 'x';
  buf[10] = 'U';
  buf[11] = 'L';
  buf[12] = 0;
  for (int zz = 9; zz>1; zz--)
  {
    if ((num & 0xF) > 9)
      buf[zz] = (num & 0xF) + 55;
    else
      buf[zz] = (num & 0xF) + 48;
    num = num >> 4;
  }
}

void startup()
{
  myGLCD.fillRect(0, 0, dispx - 1, 13, myGLCD.color565(255, 0, 0));
  myGLCD.drawLine(0, 14, dispx - 1, 14, myGLCD.color565(255, 255, 255));
  myGLCD.setCursor(5, 5);
  myGLCD.println("UTouch Calibration");

  if (dispx == 220)
  {
    myGLCD.println("");
    myGLCD.println("");
    myGLCD.print("Use a stylus or something");
    myGLCD.print("similar to touch as close");
    myGLCD.print("to the center of the");
    myGLCD.print("highlighted crosshair as");
    myGLCD.print("possible. Keep as still as");
    myGLCD.print("possible and keep holding");
    myGLCD.print("until the highlight is");
    myGLCD.print("removed. Repeat for all");
    myGLCD.print("crosshairs in sequence.");
    myGLCD.print("Touch screen to continue");
  }
  else
  {
    myGLCD.println("");
    myGLCD.println("");
    myGLCD.println("INSTRUCTIONS");
    myGLCD.println("Use a stylus or something similar to");
    myGLCD.println("touch as close to the center of the");
    myGLCD.println("highlighted crosshair as possible. Keep");
    myGLCD.println("as still as possible and keep holding");
    myGLCD.println("until the highlight is removed. Repeat");
    myGLCD.println("for all crosshairs in sequence.");
    myGLCD.println("");
    myGLCD.println("Further instructions will be displayed");
    myGLCD.println("when the calibration is complete.");
    myGLCD.println("");
    myGLCD.println("Do NOT use your finger as a calibration");
    myGLCD.println("stylus or the result WILL BE imprecise.");
    myGLCD.println("");
    myGLCD.println("Touch screen to continue");
  }

  waitForTouch();
  myGLCD.fillScreen(myGLCD.color565(0, 0, 0));
}

void done()
{
  myGLCD.fillScreen(myGLCD.color565(0, 0, 0));
  myGLCD.fillRect(0, 0, dispx - 1, 13, myGLCD.color565(255, 0, 0));
  myGLCD.drawLine(0, 14, dispx - 1, 14, myGLCD.color565(255, 255, 255));
  myGLCD.setCursor(5, 5);
  myGLCD.setTextColor(myGLCD.color565(255, 255, 255));
  myGLCD.print("UTouch Calibration");

  if (dispx == 220)
  {
    myGLCD.println("");
    myGLCD.println("");
    myGLCD.println("To use the new calibration");
    myGLCD.println("settings you must edit the");
    myGLCD.setTextColor(myGLCD.color565(160, 160, 255));
    myGLCD.println("UTouchCD.h");
    myGLCD.setTextColor(myGLCD.color565(255, 255, 255));
    myGLCD.println("file and change");
    myGLCD.println("the following values. The");
    myGLCD.println("values are located right");
    myGLCD.println("below the opening comment.");
    myGLCD.println("CAL_X");
    myGLCD.println("CAL_Y");
    myGLCD.println("CAL_S");
    toHex(calx);
    myGLCD.println(buf);
    toHex(caly);
    myGLCD.println(buf);
    toHex(cals);
    myGLCD.println(buf);
  }
  else
  {
    myGLCD.println("");
    myGLCD.println("");
    myGLCD.println("CALIBRATION COMPLETE");
    myGLCD.println("To use the new calibration");
    myGLCD.println("settings you must edit the");
    myGLCD.setTextColor(myGLCD.color565(160, 160, 255));
    myGLCD.println("UTouchCD.h");
    myGLCD.setTextColor(myGLCD.color565(255, 255, 255));
    myGLCD.println("file and change");
    myGLCD.println("the following values.");
    myGLCD.println("The values are located right");
    myGLCD.println("below the opening comment in");
    myGLCD.println("the file.");
    myGLCD.println("CAL_X");
    myGLCD.println("CAL_Y");
    myGLCD.println("CAL_S");
    Serial.println("CAL_X");
    Serial.println("CAL_Y");
    Serial.println("CAL_S");

    toHex(calx);
    myGLCD.println(buf);
    Serial.println(buf);
    toHex(caly);
    myGLCD.println(buf);
    Serial.println(buf);
    toHex(cals);
    myGLCD.println(buf);
    Serial.println(buf);
  }

}

void fail()
{
  myGLCD.fillScreen(myGLCD.color565(0, 0, 0));
  myGLCD.fillRect(0, 0, dispx - 1, 13, myGLCD.color565(255, 0, 0));
  myGLCD.drawLine(0, 14, dispx - 1, 14, myGLCD.color565(255, 0, 0));
  myGLCD.setCursor(5, 5);
  myGLCD.println("UTouch Calibration FAILED");
  myGLCD.println("");
  myGLCD.println("Unable to read the position");
  myGLCD.println("of the press. This is a");
  myGLCD.println("hardware issue and can");
  myGLCD.println("not be corrected in");
  myGLCD.println("software.");

  while (true) { yield(); };
}

