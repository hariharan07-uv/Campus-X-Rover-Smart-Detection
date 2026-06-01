#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>
#include <SoftwareSerial.h>

/* ===== TFT & IO ===== */
#define TFT_CS   10
#define TFT_DC   9
#define TFT_RST  8
#define BUZZER_PIN 6

Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_RST);

/* ===== COLORS ===== */
#define DARK_BG      0x0000
#define PANEL_BG     0x1082
#define CYAN_COLOR   0x07FF
#define GREEN_COLOR  0x07E0
#define YELLOW_COLOR 0xFFE0
#define RED_COLOR    0xF800
#define TEXT_WHITE   0xFFFF
#define TEXT_GRAY    0x7BEF
#define ORANGE_COLOR 0xFD20
#define MATRIX_GREEN 0x07E0
#define BRIGHT_GREEN 0x07FF
#define DIM_GREEN    0x0320

/* ===== SENSOR READINGS (ALL RANDOMLY GENERATED) ===== */
int mq4Level = 350, mq7Level = 12, mq131Level = 45, noiseLevel = 58;
float tempC = 24.5, tempF = 76.1;
int tempHistory[60];
int historyIndex = 0;

/* ===== TIMING ===== */
unsigned long lastUpdate = 0, lastHistoryUpdate = 0, lastIntroTime = 0;
const unsigned long INTRO_INTERVAL = 30000;

/* ===== GAUGE ANIMATION ===== */
int mq4Target = 350, mq7Target = 12, mq131Target = 45;
float mq4Current = 350, mq7Current = 12, mq131Current = 45;

/* ===== ESP32 SERIAL COMMUNICATION (RFID ONLY) ===== */
// Using SoftwareSerial for ESP32 communication on Arduino Uno
// RX = Pin 2 (connect to ESP32 TX)
// TX = Pin 3 (connect to ESP32 RX)
#define ESP32_RX 2
#define ESP32_TX 3
#define ESP32_BAUD 9600  // SoftwareSerial works better at 9600

SoftwareSerial ESP32_SERIAL(ESP32_RX, ESP32_TX);

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Serial for debugging (USB)
  Serial.begin(9600);
  
  // SoftwareSerial for ESP32 RFID communication ONLY
  ESP32_SERIAL.begin(ESP32_BAUD);
  
  randomSeed(analogRead(0));
  
  tft.begin();
  tft.setRotation(1);
  
  for (int i = 0; i < 60; i++)
    tempHistory[i] = 20 + random(0, 15);
  
  introSequence();
  drawMainUI();
  lastIntroTime = millis();
}

/* ================= CIRCLE ANIMATION ================= */
void circleAnimation() {
  tft.fillScreen(DARK_BG);
  int cx = 160, cy = 120;
  
  // Expanding circles
  for (int r = 5; r <= 120; r += 5) {
    tft.drawCircle(cx, cy, r, CYAN_COLOR);
    tone(BUZZER_PIN, 1500 + (r * 10), 20);
    delay(20);
  }
  delay(50);
  
  // Contracting circles
  for (int r = 120; r >= 5; r -= 5) {
    tft.fillCircle(cx, cy, r, DARK_BG);
    tone(BUZZER_PIN, 2000 - (r * 5), 20);
    delay(15);
  }
  delay(50);
  tft.fillScreen(DARK_BG);
}

/* ================= INTRO SEQUENCE ================= */
void introSequence() {
  int cx = 160, cy = 120;
  
  // === CIRCLE CONVERGENCE ===
  tft.fillScreen(DARK_BG);
  for (int spiral = 0; spiral < 50; spiral++) {
    for (int i = 0; i < 12; i++) {
      float angle = (i * 30 + spiral * 12) * 3.14159 / 180.0;
      float radius = 130 - (spiral * 2.5);
      if (radius > 5) {
        int x = cx + cos(angle) * radius;
        int y = cy + sin(angle) * radius;
        tft.fillCircle(x, y, 2, CYAN_COLOR);
        tft.fillCircle(x, y, 1, TEXT_WHITE);
      }
    }
    if (spiral % 5 == 0) {
      tone(BUZZER_PIN, 2200 - (spiral * 20), 15);
    }
    delay(15);
  }
  delay(150);
  tft.fillScreen(DARK_BG);
  
  // === CAMPUS X ===
  tft.fillScreen(DARK_BG);
  tft.setTextSize(5);
  tft.setTextColor(CYAN_COLOR);
  const char* txt = "CAMPUS";
  for (int i = 0; i < 6; i++) {
    tft.setCursor(30 + i*45, cy-30);
    tft.print(txt[i]);
    tone(BUZZER_PIN, 1600 + i*100, 60);
    delay(80);
  }
  delay(150);
  
  for (int s = 4; s <= 10; s++) {
    tft.fillRect(cx-60, cy+5, 120, 65, DARK_BG);
    tft.setTextSize(s);
    tft.setTextColor(s == 10 ? TEXT_WHITE : CYAN_COLOR);
    tft.setCursor(cx - s*6, cy+10);
    tft.print("X");
    tone(BUZZER_PIN, 1200 + s*120, 40);
    delay(40);
  }
  delay(200);
  
  circleAnimation();
  
  // === THE ULTIMATE ROVER ===
  tft.fillScreen(DARK_BG);
  tft.setTextSize(3);
  tft.setTextColor(0x781F);
  tft.setCursor(45, cy-20);
  tft.print("THE ULTIMATE");
  delay(150);
  
  tft.setTextSize(5);
  tft.setTextColor(CYAN_COLOR);
  tft.setCursor(65, cy+15);
  tft.print("ROVER");
  tone(BUZZER_PIN, 1800, 150);
  delay(300);
  
  // === GAS LEAK DETECTION ===
  tft.fillScreen(DARK_BG);
  tft.setTextSize(4);
  tft.setTextColor(GREEN_COLOR);
  tft.setCursor(40, cy-20);
  tft.print("GAS LEAK");
  delay(120);
  
  tft.setTextColor(CYAN_COLOR);
  tft.setCursor(35, cy+20);
  tft.print("DETECTION");
  tone(BUZZER_PIN, 1500, 200);
  delay(400);
  
  circleAnimation();
  
  // === SMILEY ===
  tft.fillScreen(DARK_BG);
  cy = 100;
  for (int r = 10; r <= 60; r += 5) {
    tft.drawCircle(cx, cy, r, YELLOW_COLOR);
    delay(12);
  }
  tft.fillCircle(cx, cy, 60, YELLOW_COLOR);
  tft.drawCircle(cx, cy, 60, TEXT_WHITE);
  
  tft.fillCircle(cx-22, cy-12, 7, DARK_BG);
  delay(60);
  tft.fillCircle(cx+22, cy-12, 7, DARK_BG);
  delay(60);
  
  // Upward smile
  for (int a = 0; a <= 180; a += 10) {
    float rad = a * 3.14159 / 180.0;
    int x = cx + cos(rad) * 35;
    int y = cy + 12 + sin(rad) * 20;
    tft.fillCircle(x, y, 2, DARK_BG);
  }
  delay(150);
  
  tft.setTextSize(3);
  tft.setTextColor(TEXT_WHITE);
  tft.setCursor(65, cy+80);
  tft.print("HAPPY TO");
  delay(120);
  
  tft.setTextColor(GREEN_COLOR);
  tft.setCursor(60, cy+110);
  tft.print("SEE YOU!");
  tone(BUZZER_PIN, 2000, 150);
  delay(400);
  
  circleAnimation();
}

/* ================= MAIN UI ================= */
void drawMainUI() {
  tft.fillScreen(DARK_BG);
  tft.fillRect(0, 0, 320, 25, PANEL_BG);
  tft.setTextSize(2);
  tft.setTextColor(TEXT_WHITE);
  tft.setCursor(12, 6);
  tft.print("GAS LEAK DETECTION");
  
  tft.setTextSize(1);
  tft.setTextColor(CYAN_COLOR);
  tft.setCursor(235, 8);
  tft.print("RFID:");
  tft.setTextColor(GREEN_COLOR);
  tft.setCursor(265, 8);
  tft.print("READY");
  
  drawGasPanel();
  drawNoisePanel();
  drawTemperaturePanel();
  drawTemperatureGraphPanel();
  updateGauges();
  updateNoiseGauge();
  updateTemperatureDisplay();
  drawTemperatureGraph();
}

void drawGasPanel() {
  tft.fillRoundRect(5, 30, 210, 120, 8, PANEL_BG);
  tft.drawRoundRect(5, 30, 210, 120, 8, 0x4208);
  
  drawGaugeFrame(42, 40, "MQ-4");
  drawGaugeFrame(107, 40, "MQ-7");
  drawGaugeFrame(172, 40, "MQ131");
  
  tft.fillRoundRect(15, 135, 75, 10, 3, 0x0400);
  tft.drawRoundRect(15, 135, 75, 10, 3, GREEN_COLOR);
  tft.setTextSize(1);
  tft.setTextColor(GREEN_COLOR);
  tft.setCursor(20, 137);
  tft.print("STATUS: SAFE");
}

void drawGaugeFrame(int x, int y, const char* label) {
  tft.setTextSize(1);
  tft.setTextColor(TEXT_WHITE);
  int w = strlen(label) * 6;
  tft.setCursor(x - w/2, y);
  tft.print(label);
  drawArc(x, y + 35, 22, 180, 360, 0x4208);
}

void drawNoisePanel() {
  tft.fillRoundRect(220, 30, 95, 120, 8, PANEL_BG);
  tft.drawRoundRect(220, 30, 95, 120, 8, 0x4208);
  tft.setTextSize(1);
  tft.setTextColor(TEXT_WHITE);
  tft.setCursor(235, 38);
  tft.print("NOISE LEVEL");
  
  tft.fillRoundRect(263, 53, 8, 12, 3, TEXT_WHITE);
  tft.fillRect(265, 65, 4, 3, TEXT_WHITE);
  tft.drawLine(261, 68, 271, 68, TEXT_WHITE);
}

void drawTemperaturePanel() {
  tft.fillRoundRect(5, 155, 155, 60, 8, PANEL_BG);
  tft.drawRoundRect(5, 155, 155, 60, 8, 0x4208);
  tft.setTextSize(1);
  tft.setTextColor(TEXT_WHITE);
  tft.setCursor(15, 162);
  tft.print("TEMPERATURE");
  
  tft.drawCircle(25, 200, 6, GREEN_COLOR);
  tft.fillCircle(25, 200, 4, GREEN_COLOR);
  tft.fillRect(23, 180, 4, 20, GREEN_COLOR);
  tft.drawRect(22, 175, 6, 25, TEXT_WHITE);
}

void drawTemperatureGraphPanel() {
  tft.fillRoundRect(165, 155, 150, 60, 8, PANEL_BG);
  tft.drawRoundRect(165, 155, 150, 60, 8, 0x4208);
  tft.setTextSize(1);
  tft.setTextColor(TEXT_WHITE);
  tft.setCursor(175, 162);
  tft.print("TEMP GRAPH");
}

/* ================= LOOP ================= */
void loop() {
  unsigned long now = millis();
  
  // *** ONLY CHECK FOR RFID DATA FROM ESP32 ***
  checkESP32RfidData();
  
  // Intro sequence every 30 seconds
  if (now - lastIntroTime >= INTRO_INTERVAL) {
    introSequence();
    drawMainUI();
    lastIntroTime = now;
    lastUpdate = now;
    lastHistoryUpdate = now;
  }
  
  // Update sensor readings (randomly generated) every 2 seconds
  if (now - lastUpdate > 2000) {
    lastUpdate = now;
    updateSensorReadings();  // All sensor data is randomly generated
    updateGauges();
    updateNoiseGauge();
    updateTemperatureDisplay();
  }
  
  // Update temperature graph every 5 seconds
  if (now - lastHistoryUpdate > 5000) {
    lastHistoryUpdate = now;
    updateTemperatureHistory();
    drawTemperatureGraph();
  }
}

/* ================= ESP32 RFID COMMUNICATION (ONLY) ================= */
void checkESP32RfidData() {
  if (ESP32_SERIAL.available() > 0) {
    String data = ESP32_SERIAL.readStringUntil('\n');
    data.trim();
    
    // Debug output to USB Serial
    Serial.print("Received RFID from ESP32: ");
    Serial.println(data);
    
    // Expected format: "RFID:XX:XX:XX:XX" or "RFID:CARDID" or just "CARDID"
    if (data.startsWith("RFID:")) {
      String cardId = data.substring(5);
      showRfidNotification(cardId);
      ESP32_SERIAL.println("ACK");  // Send acknowledgment
    } 
    // Alternative: just the card ID without "RFID:" prefix
    else if (data.length() > 0) {
      showRfidNotification(data);
      ESP32_SERIAL.println("ACK");  // Send acknowledgment
    }
  }
}

void showRfidNotification(String cardId) {
  // Clear RFID status area
  tft.fillRect(235, 3, 80, 20, PANEL_BG);
  
  tft.setTextSize(1);
  tft.setTextColor(CYAN_COLOR);
  tft.setCursor(235, 8);
  tft.print("RFID:");
  
  // Blinking "SCAN" animation
  for (int b = 0; b < 3; b++) {
    tft.setTextColor(GREEN_COLOR);
    tft.setCursor(265, 8);
    tft.print("SCAN");
    tone(BUZZER_PIN, 2000, 70);
    delay(130);
    
    tft.fillRect(265, 3, 50, 20, PANEL_BG);
    delay(90);
  }
  
  // Show "OK" status
  tft.setTextColor(GREEN_COLOR);
  tft.setCursor(265, 8);
  tft.print("OK");
  tone(BUZZER_PIN, 1500, 90);
  delay(130);
  tone(BUZZER_PIN, 2500, 120);
  
  // Log to USB Serial for debugging
  Serial.print("RFID Card Detected: ");
  Serial.println(cardId);
  
  delay(1800);
  
  // Reset to "READY" status
  tft.fillRect(265, 3, 50, 20, PANEL_BG);
  tft.setTextColor(GREEN_COLOR);
  tft.setCursor(265, 8);
  tft.print("READY");
}

/* ================= SENSOR DATA UPDATE (RANDOM GENERATION) ================= */
void updateSensorReadings() {
  // Generate random sensor values - NOT FROM ESP32
  mq4Target = 300 + random(0, 100);
  mq7Target = 10 + random(0, 15);
  mq131Target = 40 + random(0, 20);
  noiseLevel = 50 + random(-10, 15);
  tempC = 24.0 + random(0, 15) / 10.0;
  tempF = (tempC * 9.0 / 5.0) + 32.0;
}

/* ================= GAUGE UPDATES ================= */
void updateGauges() {
  mq4Current = mq4Current * 0.75 + mq4Target * 0.25;
  mq7Current = mq7Current * 0.75 + mq7Target * 0.25;
  mq131Current = mq131Current * 0.75 + mq131Target * 0.25;
  
  drawGauge(42, 75, (int)mq4Current, 1000);
  drawGauge(107, 75, (int)mq7Current, 100);
  drawGauge(172, 75, (int)mq131Current, 200);
}

void drawGauge(int x, int y, int val, int maxVal) {
  int pct = map(constrain(val, 0, maxVal), 0, maxVal, 0, 100);
  
  tft.fillCircle(x, y, 24, PANEL_BG);
  drawArc(x, y, 22, 180, 360, 0x4208);
  
  uint16_t col = (pct < 33) ? GREEN_COLOR : (pct < 66) ? YELLOW_COLOR : RED_COLOR;
  int endA = 180 + (pct * 180 / 100);
  drawArc(x, y, 22, 180, endA, col);
  
  tft.setTextSize(1);
  tft.setTextColor(TEXT_WHITE);
  char buf[5];
  sprintf(buf, "%d%%", pct);
  int w = strlen(buf) * 6;
  tft.setCursor(x - w/2, y - 8);
  tft.print(buf);
  
  tft.setTextSize(2);
  tft.fillRect(x - 20, y + 20, 40, 16, PANEL_BG);
  sprintf(buf, "%d", val);
  w = strlen(buf) * 12;
  tft.setCursor(x - w/2, y + 20);
  tft.print(buf);
  
  int ly = y + 40;
  tft.fillRect(x - 15, ly, 8, 2, GREEN_COLOR);
  tft.fillRect(x - 3, ly, 8, 2, YELLOW_COLOR);
  tft.fillRect(x + 9, ly, 8, 2, RED_COLOR);
}

void updateNoiseGauge() {
  int x = 268, y = 88;
  tft.fillCircle(x, y, 26, PANEL_BG);
  drawArc(x, y, 24, 180, 360, 0x4208);
  
  int pct = map(constrain(noiseLevel, 0, 100), 0, 100, 0, 100);
  uint16_t col = (pct < 50) ? GREEN_COLOR : (pct < 75) ? YELLOW_COLOR : RED_COLOR;
  int endA = 180 + (pct * 180 / 100);
  drawArc(x, y, 24, 180, endA, col);
  
  tft.setTextSize(2);
  tft.setTextColor(TEXT_WHITE);
  tft.fillRect(x - 20, y + 20, 40, 16, PANEL_BG);
  char buf[6];
  sprintf(buf, "%d", noiseLevel);
  int w = strlen(buf) * 12;
  tft.setCursor(x - w/2, y + 20);
  tft.print(buf);
  
  tft.setTextSize(1);
  tft.setTextColor(TEXT_GRAY);
  tft.setCursor(x + w/2 + 2, y + 26);
  tft.print("dB");
}

void updateTemperatureDisplay() {
  tft.fillRect(45, 175, 110, 35, PANEL_BG);
  
  int h = map(constrain((int)tempC, 0, 50), 0, 50, 0, 20);
  tft.fillRect(23, 180 + (20 - h), 4, h, GREEN_COLOR);
  
  tft.setTextSize(2);
  tft.setTextColor(TEXT_WHITE);
  tft.setCursor(45, 188);
  tft.print(tempC, 1);
  tft.print((char)247);
  tft.print("C/");
  tft.print(tempF, 1);
  tft.print((char)247);
  tft.print("F");
  
  tft.setTextSize(1);
  tft.setTextColor(GREEN_COLOR);
  tft.setCursor(45, 203);
  tft.print("ENV: OK");
}

void drawTemperatureGraph() {
  int gx = 172, gy = 208, gw = 138, gh = 35;
  tft.fillRect(gx, gy - gh, gw, gh, PANEL_BG);
  
  for (int i = 0; i <= 4; i++) {
    int y = gy - (i * gh / 4);
    tft.drawFastHLine(gx, y, gw, 0x2965);
  }
  
  for (int i = 0; i < 59; i++) {
    int x1 = gx + (i * gw / 59);
    int y1 = gy - map(tempHistory[i], 0, 100, 0, gh);
    int x2 = gx + ((i + 1) * gw / 59);
    int y2 = gy - map(tempHistory[i + 1], 0, 100, 0, gh);
    tft.drawLine(x1, y1, x2, y2, GREEN_COLOR);
  }
}

void updateTemperatureHistory() {
  historyIndex = (historyIndex + 1) % 60;
  tempHistory[historyIndex] = (int)tempC + random(-5, 5);
}

void drawArc(int x, int y, int r, int sA, int eA, uint16_t col) {
  for (int a = sA; a < eA; a += 3) {
    float rad = a * 3.14159 / 180.0;
    int x1 = x + cos(rad) * r;
    int y1 = y + sin(rad) * r;
    int x2 = x + cos(rad) * (r - 3);
    int y2 = y + sin(rad) * (r - 3);
    tft.drawLine(x1, y1, x2, y2, col);
  }
}

/*
 * ═══════════════════════════════════════════════════════════════
 * KEY CHANGES IN THIS VERSION:
 * ═══════════════════════════════════════════════════════════════
 * 
 * 1. RFID DATA ONLY FROM ESP32:
 *    - Only RFID card detection is received via RX from ESP32
 *    - Function checkESP32RfidData() handles incoming RFID data
 *    - Accepts formats: "RFID:XX:XX:XX:XX" or just "CARDID"
 *    - Sends "ACK" acknowledgment back to ESP32
 * 
 * 2. ALL SENSOR DATA IS RANDOMLY GENERATED:
 *    - MQ-4, MQ-7, MQ-131 (gas sensors)
 *    - Noise level
 *    - Temperature (°C and °F)
 *    - All values are randomly varied in updateSensorReadings()
 * 
 * 3. SERIAL COMMUNICATION:
 *    - SoftwareSerial (Pins 2&3): For RFID communication with ESP32
 *    - Serial (USB): Debug output only
 * 
 * WIRING FOR ESP32 TO ARDUINO UNO:
 * - ESP32 TX → Arduino Pin 2 (RX)
 * - ESP32 RX → Arduino Pin 3 (TX)
 * - GND → GND (Common ground REQUIRED!)
 * 
 * IMPORTANT: ESP32 must also use 9600 baud for serial communication
 * 
 * ESP32 SHOULD SEND (when RFID card detected):
 * - "RFID:AA:BB:CC:DD\n" 
 * - OR just "AABBCCDD\n"
 * 
 * ARDUINO WILL RESPOND:
 * - "ACK\n" (acknowledgment)
 * 
 * ═══════════════════════════════════════════════════════════════
 */
