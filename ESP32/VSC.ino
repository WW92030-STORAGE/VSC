#include <VSC_everything.h>

// Original library and code base - https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <vector>
#include <cstdint>
#include <iostream>
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
#include "OBJ.h"

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

Mesh proto = cube(1);

void sceneSetup() {
  dispstats();

  Transform forwards(Vector3(0, 0, -2), Rotation3(Vector3(0, 1, 0), M_PI / 4));
  proto.Trans(forwards);

  dispstats();

	float A = 0.2;

  float S = sqrtf(3);

  PointLight PL(Vector3(1, 0, 0), A);
	PL.Trans(Transform(Vector3(-S, 1, 0)));
	s.lights.push_back(PL);

	PointLight PL2(Vector3(0, 0, 1), A);
	PL2.Trans(Transform(Vector3(S, 1, 0)));
	s.lights.push_back(PL2);

  PointLight PL3(Vector3(0, 1, 0), A);
	PL3.Trans(Transform(Vector3(0, -2, 0)));
	s.lights.push_back(PL3);
  dispstats();

	s.clearBuffer();


  dispstats();


  int SPE = 2;

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


  sceneSetup();

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

Vector3 axis(1, 1, 1);



void loop() {
  if (flashbyte == 0) {
    anim_loop(matrixpanel->color565(255, 255, 255));
    return;
  }

  axis = Rotation3(Vector3(0, 0, 1), 0.037) * axis;

  Transform t(Vector3(0, 0, 0), Rotation3(axis, 0.1));
  Transform goback(proto.transform.origin, Matrix3::eye());

  proto.Trans(goback.inv());
  proto.Trans(t);
  proto.Trans(goback);

  s.clearBuffer();
  s.fillMesh(proto, BaseMaterial(0xFFFFFFFF, 1), true, true, false);
  RenderBuffer(s);
  disp();


  delay(50);

}