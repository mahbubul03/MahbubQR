#include "MahbubQR.h"
#include "quirc.h"   // hidden inside .cpp, user never sees this

MahbubQR::MahbubQR() {
  _decoder     = NULL;
  _initialized = false;
  _cameraReady = false;
}

MahbubQR::~MahbubQR() {
  if (_decoder) {
    quirc_destroy((struct quirc*)_decoder);
    _decoder = NULL;
  }
}

bool MahbubQR::_initDecoder() {
  _decoder = quirc_new();
  return (_decoder != NULL);
}

bool MahbubQR::begin() {
  if (!_initDecoder()) {
    Serial.println("[MahbubQR] Decoder init failed!");
    return false;
  }
  _initialized = true;
  Serial.println("[MahbubQR] Ready!");
  return true;
}

bool MahbubQR::isReady() {
  return _initialized;
}

String MahbubQR::version() {
  return "MahbubQR v1.0.0 by Mahbubul Islam";
}

bool MahbubQR::_processFrame(camera_fb_t *fb, MahbubQRResult &result) {
  result.found   = false;
  result.length  = 0;
  result.version = 0;
  result.eccLevel = '?';
  memset(result.content, 0, sizeof(result.content));

  struct quirc *qr = (struct quirc*)_decoder;

  if (quirc_resize(qr, fb->width, fb->height) < 0) return false;

  uint8_t *buf = quirc_begin(qr, NULL, NULL);
  memcpy(buf, fb->buf, fb->len);
  quirc_end(qr);

  int count = quirc_count(qr);
  if (count == 0) return false;

  struct quirc_code code;
  struct quirc_data data;
  quirc_extract(qr, 0, &code);

  int err = quirc_decode(&code, &data);
  if (err != 0) return false;

  result.found   = true;
  result.length  = data.payload_len;
  result.version = data.version;
  result.eccLevel = "MLHQ"[data.ecc_level];
  strncpy(result.content, (char*)data.payload, sizeof(result.content) - 1);

  return true;
}

bool MahbubQR::scan(MahbubQRResult &result) {
  if (!_initialized) {
    result.found = false;
    return false;
  }

  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    result.found = false;
    return false;
  }

  bool found = _processFrame(fb, result);
  esp_camera_fb_return(fb);
  return found;
}

String MahbubQR::scanToString() {
  MahbubQRResult result;
  if (scan(result)) {
    return String(result.content);
  }
  return "";
}