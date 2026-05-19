# MahbubQR

**Lightweight QR Code Scanner Library for ESP32-S3 Camera**

A simple and fast QR code detection library designed for ESP32-S3 based camera boards.

---

## Features

- Fast and reliable QR code decoding
- Supports QR versions 1 to 40
- Returns decoded text, version, ECC level and more
- Easy to use Arduino compatible API
- Optimized for ESP32 Camera

---

## Installation

1. Download this repository
2. Copy the `MahbubQR` folder into your Arduino `libraries` folder
3. Restart Arduino IDE
4. Go to **Sketch → Include Library → MahbubQR**

---

## Credits
- MahbubQR — ESP32-S3 Camera QR Code Scanner
  Based on the excellent quirc library by Daniel Beer.

## Example

```cpp
#include "MahbubQR.h"

MahbubQR qr;

void setup() {
  Serial.begin(115200);
  
  if (!qr.begin()) {
    Serial.println("MahbubQR initialization failed!");
    while(1);
  }
  
  Serial.println("MahbubQR Ready!");
}

void loop() {
  MahbubQRResult result;
  
  if (qr.scan(result)) {
    Serial.println("\nQR Code Detected!");
    Serial.println("Content: " + String(result.content));
    Serial.print("Version: ");
    Serial.println(result.version);
    Serial.print("ECC Level: ");
    Serial.println(result.eccLevel);
  }
  
  delay(300);
}


