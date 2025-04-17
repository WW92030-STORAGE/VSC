#include <VSC_everything.h>

// Original library and code base - https://github.com/mrfaptastic/ESP32-HUB75-MatrixPanel-I2S-DMA
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <vector>
#include <cstdint>
#include <iostream>
using namespace std;

#include <esp_random.h>
#include <EEPROM.h>

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
  EEPROM.write(0, (flashbyte + 1) % 2);
  EEPROM.commit();
}

// SETUP

SET_LOOP_TASK_STACK_SIZE(16*1024);

#define SCREEN_L 64
#define SCREEN_H 32

RayTracer s(0, SCREEN_L, SCREEN_H);

Mesh cube0 = cube(2);
Mesh cube1 = cube(0.5);
Mesh cube2 = cube(0.5);
Mesh ground = GridSquare(64, 1);

void setupscene() {
  float A = 0.1;
  PointLight P2(Vector3(1, 1, 1), A);
	P2.Trans(Transform(Vector3(0, 2, 0)));
	s.lights.push_back(P2);


  Transform back(Vector3(0, -1, -5), Rotation3(Vector3(0, 1, 0), -0.4 + M_PI));
	cube0.Trans(back);

	Transform back2(Vector3(-0.5, -0.5, -3), Rotation3(Vector3(0, 1, 0), -0.4));
	cube1.Trans(back2);

	Transform back3(Vector3(1, -1, -3), Rotation3(Vector3(0, 1, 0), -0.4));
	cube2.Trans(back3);

  ground.Trans(Transform(Vector3(0, -2.5, 0)));

  float SP = 64;
	BaseMaterial red(0xFF000000, SP, 1);
	BaseMaterial green(0x00FF0000, SP, 1);
	BaseMaterial blue(0x0000FF00, SP, 1);
	BaseMaterial cyan(0x00FFFF00, SP, 1);
	BaseMaterial white(0x80808000, SP, 1);

	s.addMesh(&cube0, &red, false);
	s.addMesh(&cube1, &green, false);
	s.addMesh(&cube2, &blue, false);
	// s.addMesh(&ground, &white, false);
}

void setup() {
  Serial.printf("BEGIN BEGIN\n");
  dispstats();
  eeprom_setup();
  matrixsetup();
  disp(grid0);

  setupscene();
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


void loop() {
  if (flashbyte == 0) {
    anim_loop(matrixpanel->color565(255, 255, 255));
    return;
  }

  // Data
  float A = 0.1;


  s.UseBVH = true;
  s.DEPTH = 0;
  s.render();

  RenderBuffer(s);
  disp();

  dispstats();

}
