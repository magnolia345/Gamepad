#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>

MCUFRIEND_kbv tft;
int px;
int py;
// Shield pin configurations
const int XP = 8, XM = A2, YP = A3, YM = 9; 
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 200);

// Use your custom calibration bounds here
int TS_LEFT = 114, TS_RT = 915, TS_TOP = 911, TS_BOT = 69; 
// Pressure thresholds
#define MINPRESSURE 3
#define MAXPRESSURE 300000083

// Brush Settings
#define BRUSH_SIZE 3
uint16_t current_color = 0xFFFF; // White brush

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
  drawUI();
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
    draw();
  }else{
    last_x = -1;
    last_y = -1;
  }
}

void cleardisplay() {
  tft.fillScreen(0x0000); // Clear whole screen to black
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
  tft.fillRect(295, 220, 30, 10, WHITE);
}
void collision(int x, int y, int w, int h){
  if(px > x -1 && px < x + w + 1 && py > y -1 && py < y + h + 1){
    collide = true;

  }
  else{
    collide = false;
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
        tft.drawLine(last_x, last_y, px, py, color);
        tft.drawLine(last_x + 0.1, last_y, px + 0.1, py, color);
        tft.drawLine(last_x, last_y + 0.1, px, py + 0.1, color);
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