/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "pico/unique_id.h"
#include "tusb.h"
#include "usb_descriptors.h"
#define EPNUM_CDC_NOTIF   0x81  // Endpoint powiadomień CDC (IN)
#define EPNUM_CDC_OUT     0x02  // Endpoint odbioru danych z PC do mikrokontrolera (OUT)
#define EPNUM_CDC_IN      0x82  // Endpoint wysyłania danych z mikrokontrolera do PC (IN)
#define EPNUM_HID         0x83  // Endpoint wysyłania raportów klawiatury/HID (IN)
#define CONFIG_TOTAL_LEN  (TUD_CONFIG_DESC_LEN + TUD_CDC_DESC_LEN + TUD_HID_DESC_LEN)

static tusb_desc_device_t const desc_device = {
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = 0x0200, // Wersja USB 2.0
    
    // Kluczowa zmiana dla urządzeń Composite (wielofunkcyjnych):
    .bDeviceClass       = TUSB_CLASS_MISC,
    .bDeviceSubClass    = MISC_SUBCLASS_COMMON,
    .bDeviceProtocol    = MISC_PROTOCOL_IAD, // Informuje OS, że urządzenie składa się z grup interfejsów (IAD)
    
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE, // Rozmiar paczki dla endpointu zerowego (zazwyczaj 64)
    .idVendor           = 0xCafe,  // Twój VID (Vendor ID) - możesz wymyślić na własne potrzeby w DIY
    .idProduct          = 0x400f,  // Twój PID (Product ID)
    .bcdDevice          = 0x0100,  // Wersja Twojego urządzenia (np. 1.0.0)
    
    .iManufacturer      = 0x01,    // Indeks tekstu: Kto wyprodukował? (np. "Moja Firma")
    .iProduct           = 0x02,    // Indeks tekstu: Jak się nazywa? (np. "CyberDeck 12-btn")
    .iSerialNumber      = 0x03,    // Indeks tekstu: Numer seryjny
    
    .bNumConfigurations = 0x01     // Ile mamy konfiguracji (zazwyczaj 1)
};

// Właściwy opis raportu HID
uint8_t const desc_hid_report[] = {
  // 1. Standardowa klawiatura (obsłuży m.in. F13, F14 itd.)
  TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(REPORT_ID_KEYBOARD)),
  
  // 2. Klawisze multimedialne / Consumer Control (obsłuży Mute, Volume Up, Volume Down)
  TUD_HID_REPORT_DESC_CONSUMER(HID_REPORT_ID(REPORT_ID_CONSUMER)),

  // 3. System Control (np. Sleep, Power, Wake)
  TUD_HID_REPORT_DESC_SYSTEM_CONTROL(HID_REPORT_ID(REPORT_ID_SYSTEM_CONTROL))
};

// Funkcja zwrotna, o którą TinyUSB upomni się w trakcie działania
uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance) {
  (void) instance;
  return desc_hid_report;
}

uint8_t const *tud_descriptor_device_cb(void) {
  return (uint8_t const *)&desc_device;
}

uint8_t const desc_configuration[] = {
  // 1. Ogólny nagłówek konfiguracji (1 konfiguracja, 3 interfejsy, maksymalny pobór prądu 100mA)
  TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),

  // 2. Kloc CDC (Tworzy od razu interfejs sterujący i danych)
  TUD_CDC_DESCRIPTOR(ITF_NUM_CDC, 4, EPNUM_CDC_NOTIF, 8, EPNUM_CDC_OUT, EPNUM_CDC_IN, 64),

  // 3. Kloc HID (Twoja klawiatura / multimedia zdefiniowana w Sekcji 2)
  TUD_HID_DESCRIPTOR(ITF_NUM_HID, 0, HID_ITF_PROTOCOL_NONE, sizeof(desc_hid_report), EPNUM_HID, CFG_TUD_HID_EP_BUFSIZE, 5)
};

uint8_t const * tud_descriptor_configuration_cb(uint8_t index) {
  (void) index; // Ignorujemy indeks, bo mamy tylko jedną konfigurację
  return desc_configuration; // Zwracamy adres naszej "kanapki"
}

//--------------------------------------------------------------------+
// 4. String Descriptors (Napisy tekstowe)
//--------------------------------------------------------------------+
static char const *string_desc_arr[] =
{
  (const char[]) { 0x09, 0x04 }, // 0: Język angielski
  "CyberDeck Studio",            // 1: Producent (.iManufacturer)
  "CyberDeck 12-Keys",           // 2: Produkt (.iProduct)
  NULL,                          // 3: Numer seryjny (generowany automatycznie przez Pico SDK)
  "CyberDeck UART Port"          // 4: Nazwa interfejsu CDC
};

static uint16_t _desc_str[32 + 1];

uint16_t const * tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
  (void) langid;
  size_t chr_count;

  if ( index == 0) {
    memcpy(&_desc_str[1], string_desc_arr[0], 2);
    chr_count = 1;
  } else if ( index == 3 ) { // Indeks 3 to numer seryjny (STRID_SERIAL)
    char serial[2 * PICO_UNIQUE_BOARD_ID_SIZE_BYTES + 1];
    pico_get_unique_board_id_string(serial, sizeof(serial));
    chr_count = strlen(serial);
    for (size_t i = 0; i < chr_count; i++) {
      _desc_str[1 + i] = serial[i];
    }
  } else {
    if ( !(index < sizeof(string_desc_arr) / sizeof(string_desc_arr[0])) ) return NULL;
    const char *str = string_desc_arr[index];
    chr_count = strlen(str);
    if ( chr_count > 32 ) chr_count = 32;
    for ( size_t i = 0; i < chr_count; i++ ) {
      _desc_str[1 + i] = str[i];
    }
  }

  _desc_str[0] = (uint16_t) ((TUSB_DESC_STRING << 8) | (2 * chr_count + 2));
  return _desc_str;
}

// Funkcja zwrotna wywoływana, gdy host pyta o raport (często wymagana przez stos)
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen) {
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;
  return 0;
}

// Funkcja zwrotna wywoływana, gdy host wysyła raport do urządzenia (np. stan LED CapsLock/NumLock)
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) bufsize;
}