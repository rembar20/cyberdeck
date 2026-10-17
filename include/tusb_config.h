#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// 1. Informujemy TinyUSB, że pracujemy na procesorze RP2040 (Raspberry Pi Pico)
#define CFG_TUSB_MCU OPT_MCU_RP2040

// 2. Włączamy główny stos USB w trybie urządzenia (Device)
#define CFG_TUD_ENABLED 1

// ==========================================
// TRYB PORTU USB (Tego brakowało!)
// ==========================================
// Oznacza to: port 0 działa w trybie urządzenia (BOARD_TUD_RHPORT) z prędkością Full Speed
#define CFG_TUSB_RHPORT0_MODE    (OPT_MODE_DEVICE | OPT_MODE_FULL_SPEED)

// ==========================================
// WYBÓR KLAS USB (Włączasz to, co potrzebujesz)
// ==========================================

// Wirtualny port szeregowy (CDC) - przydaje się do debugowania i printf()
#define CFG_TUD_CDC 1

// Urządzenie HID (Klawiatura / Mysz / Kontroler) - idealne do CyberDecka!
#define CFG_TUD_HID 1

// Pozostałe klasy (na razie wyłączone, żeby nie zaśmiecać pamięci)
#define CFG_TUD_MSC 0   // Pamięć masowa (pendrive)
#define CFG_TUD_MIDI 0  // Instrument muzyczny MIDI
#define CFG_TUD_VENDOR 0 // Własny protokół niestandardowy

// ==========================================
// ROZMIARY BUFORÓW (Standardowe wartości)
// ==========================================
#define CFG_TUD_CDC_RX_BUFSIZE 256
#define CFG_TUD_CDC_TX_BUFSIZE 256

// Rozmiar bufora dla HID (musi pomieścić raport wysyłany do komputera)
#define CFG_TUD_HID_BUFSIZE 16

#ifdef __cplusplus
}
#endif

#endif /* _TUSB_CONFIG_H_ */