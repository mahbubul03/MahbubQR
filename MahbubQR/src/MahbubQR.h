#ifndef MAHBUB_QR_H
#define MAHBUB_QR_H

#include "Arduino.h"
#include "esp_camera.h"

// QR scan result structure
struct MahbubQRResult {
  bool    found;
  char    content[500];
  int     length;
  int     version;
  char    eccLevel;
};

class MahbubQR {
  private:
    void     *_decoder;       // internal decoder, hidden from user
    bool      _initialized;
    bool      _cameraReady;

    // internal helpers
    bool _initDecoder();
    bool _processFrame(camera_fb_t *fb, MahbubQRResult &result);

  public:
    MahbubQR();
    ~MahbubQR();

    // Initialize the scanner
    bool begin();

    // Scan one frame — returns true if QR found
    bool scan(MahbubQRResult &result);

    // Simple scan — just returns the string content
    String scanToString();

    // Check if ready
    bool isReady();

    // Version info
    String version();
};

#endif