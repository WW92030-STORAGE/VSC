# VSC/ESP32

Example code to run this on an ESP32 with two 64x32 LED panels. Based on the template here: https://github.com/WW92030-STORAGE/ESP32_64x32P3/tree/main/TEMPLATE.  

# HOW TO USE THIS

In the `Libraries` directory of the Arduino IDE sketchbook location place a folder which contains the contents of `../src`. Then to import the library you can use something like `#include <VSC_everything.h>`.

# NOTES

- The LED panels library used here is https://github.com/mrcodetastic/ESP32-HUB75-MatrixPanel-DMA/ . The code also intends to use two LED panels.
- This can work on a most basic ESP32 (without external PSRAM) however it is highly recommended to use a WROVER or other PSRAM-equipped module.
- `icosphere` is memory intensive. Going above two subdivision levels is not recommended.
- Each subdirectory of this one is a separate sketch.
- Be aware of any outdated API.
- Make sure to enable RTTI as dynamic casting is used.
- Raytracing is very slow. Rasterization is a lot faster.

![Example!](../example_outputs/esp32rtx.png?raw=true)