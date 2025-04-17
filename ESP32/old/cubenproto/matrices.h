#ifndef CONSTANTS_H
#define CONSTANTS_H

/*
This header contains stuff that I keep reusing when working with matrices.
It's like a standard template for matrix programs.
*/

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <vector>
#include <cstdint>
using namespace std;
#include <esp_random.h>

const int n = 32;
const int m = 64;
const int chain = 2;

#define R1_PIN 25
#define G1_PIN 26
#define B1_PIN 27
#define R2_PIN 14
#define G2_PIN 12
#define B2_PIN 13
#define A_PIN 23
#define B_PIN 19
#define C_PIN 5
#define D_PIN 17
#define E_PIN 18 // required for 1/32 scan panels, like 64x64. Any available pin would do, i.e. IO32
#define LAT_PIN 4
#define OE_PIN 15
#define CLK_PIN 16



//MatrixPanel_I2S_DMA matrixpanel;
MatrixPanel_I2S_DMA *matrixpanel = nullptr;

// This grid might need to be changed depending on how your panels are setup. Here, it is left POV side panel = left half of grid.

vector<vector<uint16_t>> col(n, vector<uint16_t>(m * chain, 0));

// Utility methods

void dispstats() {
  Serial.printf("Total heap: %d", ESP.getHeapSize());
  Serial.printf("Free heap: %d", ESP.getFreeHeap());
  Serial.printf("Total PSRAM: %d", ESP.getPsramSize());
  Serial.printf("Free PSRAM: %d", ESP.getFreePsram());
  Serial.printf("\n");
}

double rng() { // Random uniform distribution
  return rand() / (RAND_MAX + 1.0);
}

double runif(double a, double b) { // Random uniform [a, b)
  return (rng() * (b - a)) + a;
}

uint16_t protogen() { // Random hex color
  return matrixpanel->color565((int)runif(0, 256), (int)runif(0, 256), (int)runif(0, 256));
}

uint16_t protogenfullhue() { // Random color with full saturation and value
  int red = 0;
  int green = 0;
  int blue = 0;
  int partition = (int)runif(0, 255);
  int pos = (int)runif(0, 6);
  switch (pos) {
    case 0:
      return matrixpanel->color565(255, partition, 0);
    case 1:
      return matrixpanel->color565(255 - partition, 255, 0);
    case 2:
      return matrixpanel->color565(0, 255, partition);
    case 3:
      return matrixpanel->color565(0, 255 - partition, 255);
    case 4:
      return matrixpanel->color565(partition, 0, 255);
    default:
      return matrixpanel->color565(255, 255 - partition, 0);
  }
}

uint16_t hex565(int x) {
  // cout << x << " = [" << (x>>16) % (1<<8) << " " << (x>>8) % (1<<8) << " " << x % (1<<8) << "]\n";
  return matrixpanel->color565((x>>16) % (1<<8), (x>>8) % (1<<8), x % (1<<8));
}

uint16_t spectrum(int hour) {
  while (hour < 0) hour += 12;
  hour %= 12;
  uint16_t hrs[12] = {hex565(0xFF0000), hex565(0xFF8000), hex565(0xFFFF00), hex565(0x80FF00), hex565(0x00FF00), hex565(0x00FF80), hex565(0x00FFFF), hex565(0x0080FF), hex565(0x0000FF), hex565(0x8000FF), hex565(0xFF00FF), hex565(0xFF0080)};
  return hrs[hour];
}

// Displaying Outputs

void disp(vector<vector<bool>> grid) { // Boolean array is threshold. Displays color grid masked by bitmask
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m * chain; j++) {
      if (grid[i][j]) matrixpanel->drawPixel(j, i, col[i][j]);
      else matrixpanel->drawPixel(j, i, matrixpanel->color565(0, 0, 0));
    }
  }
}

void disp() { // Boolean array is threshold. Displays color grid masked by bitmask
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m * chain; j++) {
      matrixpanel->drawPixel(j, i, col[i][j]);
    }
  }
}

void playAnimation(vector<vector<vector<bool>>> anim, int d, bool backward) {
  for (int i = 0; i < anim.size(); i++) {
    int index = (backward) ? (anim.size() - 1 - i) : i;
    disp(anim[index]);
    delay(d);
  }
}

HUB75_I2S_CFG::i2s_pins _pins={R1_PIN, G1_PIN, B1_PIN, R2_PIN, G2_PIN, B2_PIN, A_PIN, B_PIN, C_PIN, D_PIN, E_PIN, LAT_PIN, OE_PIN, CLK_PIN};

void matrixsetup() {
  srand(esp_random());
  Serial.begin(9600);
  // put your setup code here, to run once:
  HUB75_I2S_CFG mxconfig(m, n, chain, _pins);

  mxconfig.gpio.e = 18;
  mxconfig.clkphase = false;
  mxconfig.driver = HUB75_I2S_CFG::FM6126A;

  // Display Setup
  matrixpanel = new MatrixPanel_I2S_DMA(mxconfig);
  matrixpanel->setLatBlanking(2);
  matrixpanel->begin();
  matrixpanel->clearScreen();
  matrixpanel->setBrightness8(64);

  matrixpanel->fillScreenRGB888(255, 255, 255);
  delay(100);
  matrixpanel->drawRect(0, 0, 10, 10, matrixpanel->color565(0, 0, 0));
  delay(100);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m * chain; j++) col[i][j] = matrixpanel->color565(255, 255, 255);
  }
  dispstats();
}

#endif