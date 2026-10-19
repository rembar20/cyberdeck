#ifndef USB_DESCRIPTORS_H
#define USB_DESCRIPTORS_H

#include "tusb.h"

#ifdef __cplusplus
extern "C" {
#endif

// Numery interfejsów dla urządzenia kompozytowego (CDC + HID)
enum {
  ITF_NUM_CDC = 0,     // CDC Control (sterujący)
  ITF_NUM_CDC_DATA,    // CDC Data (danych)
  ITF_NUM_HID,         // HID (Klawiatura / Multimedia)
  ITF_NUM_TOTAL        // Łączna liczba interfejsów = 3
};

// ID raportów HID (używane przy wysyłaniu klawiszy)
enum {
  REPORT_ID_KEYBOARD = 1,
  REPORT_ID_CONSUMER,
};

#ifdef __cplusplus
}
#endif

#endif /* USB_DESCRIPTORS_H */