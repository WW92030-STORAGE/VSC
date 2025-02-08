#include <VSC_everything.h>

// Original library and code base - https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <vector>
#include <cstdint>
using namespace std;

#include <esp_random.h>
#include <EEPROM.h>

extern "C" {
#include <esp_spiram.h>
#include <esp_himem.h>
}

#include "matrices.h"
#include "panels.h"

#include "VSCRender.h"

vector<vector<vector<bool>>> animation{grid0, grid1, grid2, grid3, grid4, grid5, grid6, grid7, grid8};

// EEPROM

#define EEPROM_SIZE 1
uint8_t flashbyte = 0;

void eeprom_setup() {
  EEPROM.begin(EEPROM_SIZE);
  flashbyte = EEPROM.read(0);
  EEPROM.write(0, (flashbyte != 0) ? 0 : 127);
  EEPROM.commit();
}

// SETUP

SET_LOOP_TASK_STACK_SIZE(16*1024);

#define SCREEN_L 64
#define SCREEN_H 32

Scene s(SCREEN_L, SCREEN_H);

void test() {
  dispstats();
  int SUB = 2;
	Mesh lao = icosphere(1, SUB);

  dispstats();

	float SSS = 1;

	Transform offset(Vector3(0, -2, -4) * SSS, Rotation3(Vector3(1, 1, 1), M_PI / 8));
	lao.Trans(offset);

	Mesh shi(lao);

  dispstats();
	

	shi.Trans(Transform(Vector3(2, 2, -1)));

	Mesh proto = icosphere(1, SUB);
	proto.Trans(Transform(Vector3(-1, 0, -4), Rotation3(Vector3(0, 1, 0), M_PI * 1.2)));

  dispstats();

	Scene s(SCREEN_L, SCREEN_H);

  dispstats();

	float A = 0;

	PointLight PL2(Vector3(1, 1, 1), A);
	PL2.Trans(Transform(Vector3(2, 1, 0)));
	s.lights.push_back(PL2);
  dispstats();

	s.clearBuffer();


  dispstats();


  int SPE = 2;
	

	s.fillMesh(lao, BaseMaterial(0xFF0000FF, SPE * SPE), true, true);
	s.fillMesh(shi, BaseMaterial(0x00FF00FF, SPE), true, true);
	s.fillMesh(proto, BaseMaterial(0x0000FFFF, 1), false);

  dispstats();

  RenderBuffer(s);
  disp();
}

void setup() {
  Serial.printf("BEGIN BEGIN\n");
  dispstats();
  eeprom_setup();
  matrixsetup();
  disp(grid0);


  test();

}

int blinkdelay = 0;
int blinkthreshold = 0;

// ANIMATION

void anim_loop(uint16_t color) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m * chain; j++) col[i][j] = color;
  }
  delay(100);
  // Serial.println(rand() % 1024);
  disp(grid0);

  blinkdelay++;
  if (blinkdelay >= blinkthreshold) {
    blinkdelay = 0;
    blinkthreshold = (int)(runif(16, 64));
    playAnimation(animation, 20, false);
    playAnimation(animation, 20, true);
    dispstats();
    Serial.printf("BLINK THRESHOLD %ld\n", blinkthreshold);
  }
}

// LOOP

void loop() {
  if (flashbyte == 4) {
    anim_loop(matrixpanel->color565(255, 255, 255));
    return;
  }
}