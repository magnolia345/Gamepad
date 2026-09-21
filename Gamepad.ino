#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
MCUFRIEND_kbv tft;
int px;
int py;
// Shield pin configurations
const int XP = 8, XM = A2, YP = A3, YM = 9; 
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 200);
int logged =true;
int done = true;
// Use your custom calibration bounds here
int TS_LEFT = 114, TS_RT = 915, TS_TOP = 911, TS_BOT = 69; 
// Pressure thresholds
#define MINPRESSURE 1
#define MAXPRESSURE 300000083
String passcode = "842842";
String pass = ""; 


// Brush Settings
#define BRUSH_SIZE 5
uint16_t current_color = 0xFFFF; // White brush
bool run = true;
// Variables to track continuous line drawing
int last_x = -1;
int last_y = -1;
bool collide = false;
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GREY    0x7BEF
int color = WHITE;

void setup() {
  Serial.begin(9600);
  tft.reset();
  uint16_t identifier = tft.readID();
  tft.begin(identifier);
  tft.setRotation(1); // Landscape view (320x240)
  cleardisplay();
  password();
}

void loop() {
  TSPoint p = ts.getPoint();
  pinMode(YP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(XP, OUTPUT);
  pinMode(YM, OUTPUT);
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    px = map(p.y, TS_BOT, TS_TOP, 0, tft.width());
    py = map(p.x, TS_RT, TS_LEFT, 0, tft.height());
    if(logged){
      check();
    }
  if(!logged){
    pad();
  }
  }else{
    last_x = -1;
    last_y = -1;
  }
}

void cleardisplay() {
  tft.fillScreen(0x0000);
  if(!done){
    drawUI();
  } // Clear whole screen to black

}
void drawUI() {
  // Draw canvas divider line
  tft.drawFastVLine(238, 0, 240, 0x5AAB); // Gray line separating screen
  // Draw Clear Button
  tft.fillRect(245, 10, 70, 40, 0xF800); // Red box
  tft.setCursor(255, 22);
  tft.setTextColor(0xFFFF);
  tft.setTextSize(2);
  tft.print("CLEAR");
  tft.fillRect(245, 60, 70, 20, WHITE);
  tft.fillRect(245, 85, 70, 20, GREEN);
  tft.fillRect(245, 110, 70, 20, BLUE);
  tft.fillRect(245, 135, 70, 20, RED);
}
bool collision(int x, int y, int w, int h){
  if(px > x -1 && px < x + w + 1 && py > y -1 && py < y + h + 1){
    collide = true;
    return true;

  }
  else{
    collide = false;
    return false;
  }
}
void draw(){
  if (px > 245 && px < 315 && py > 10 && py < 50) {
      cleardisplay();
      drawUI();
      delay(200);
      return;
    }
    if (px < 235) {
      if (last_x == -1 && last_y == -1) {
        tft.fillCircle(px, py, BRUSH_SIZE, color);
      } else {
        
      }
      
      last_x = px;
      last_y = py;
    }
    collision(245, 135, 70, 20);
    if(collide){
      color = RED;
    }
    collision(245, 60, 70, 20);
    if(collide){
      color = WHITE;
    }
    collision(245, 85, 70, 20);
    if(collide){
      color = GREEN;
    }
    collision(245, 110, 70, 20);
    if(collide){
      color = BLUE;
    }
}
void password(){
  tft.fillCircle(110, 45, 15, WHITE);
  tft.fillCircle(160, 45, 15, WHITE);
  tft.fillCircle(210, 45, 15, WHITE);
  tft.fillCircle(110, 95, 15, WHITE);
  tft.fillCircle(160, 95, 15, WHITE);
  tft.fillCircle(210, 95, 15, WHITE);
  tft.fillCircle(110, 145, 15, WHITE);
  tft.fillCircle(160, 145, 15, WHITE);
  tft.fillCircle(210, 145, 15, WHITE);
  tft.fillCircle(160, 195, 15, WHITE);
  tft.setTextSize(2);
  print(1, 110, 45);
  print(2, 160, 45);
  print(3, 210, 45);
  print(4, 110, 95);
  print(5, 160, 95);
  print(6, 210, 95);
  print(7, 110, 145);
  print(8, 160, 145);
  print(9, 210, 145);
  print(0, 160, 195);
}
void print(int n, int xd, int yd){
  tft.setTextColor(BLACK);
  tft.setCursor(xd-4, yd-4);
  tft.print(n);
}
bool circle(int cx, int cy, int r){
  int dis = (px - cx) * (px -cx) + (py - cy) * (py - cy);
  if(dis <= r * r){
    return true;
  }else{
    return false;
  }
}
void check(){
      int val = 6;
    for(int x = 110; x <= 210; x += 50){
      val -= 8;
      int y = 45;
      for(y = 45; y <= 145; y += 50){
        val += 3;
        if(circle(x, y, 15) && px < 230 && px > 90 && py > 25 && py < 165){
          pass += String(val);
          tft.setTextColor(WHITE, BLACK);
          tft.setTextSize(2);
          tft.setCursor(130, 10);
          tft.print(pass);
          delay(150);
        }
      }
    }
    if(circle(160, 195, 15)){
      pass += String(0);
      tft.setTextColor(WHITE, BLACK);
      tft.setTextSize(2);
      tft.setCursor(130, 10);
      tft.print(pass);
      delay(150);
    }
    if(pass == passcode){
      tft.setCursor(82, 220);
      tft.setTextSize(2);
      tft.setTextColor(WHITE);
      tft.print("Access Accepted");
      delay(1000);
      cleardisplay();
      logged = false;

    }else{
      if(pass.length() == 6){
        tft.setCursor(85, 220);
        tft.setTextSize(2);
        tft.setTextColor(WHITE);
        tft.print("Access Denied");
        delay(1000);
        pass = String();
        cleardisplay();
        password();
      }
    }
}
void pad(){
  if(done){
    drawUI();
    done = false;
  }
  draw();
}

