#ifndef PHYSTESTS_VSC_PHYS
#define PHYSTESTS_VSC_PHYS

#include <cmath>
#include <fstream>


#include "everything.h"
#include <iostream>
#include <chrono>

#include "texture_texture.h"
#include "main_utils.h"

using namespace std;



void BoundingVolume1() {
	BoundingSphere sphere = {Vector3(0, 0, 0), 10};

	cout << sphere.to_string() << endl;
}


#endif