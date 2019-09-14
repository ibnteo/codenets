/*
 * Project: Chord keyboard Codenets
 * Version: 0.1 (Beta)
 * Date: 2019-09-13
 * Author: Vladimir Romanovich <ibnteo@gmail.com>
 * License: MIT
 * https://github.com/ibnteo/codenets
 */

#include "Descriptors.h"
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/power.h>
#include <avr/wdt.h>

void Ports_Init(void);
void LED_On(void);
void LED_Off(void);
void LED_Toggle(void);
void LED_Switch(bool);
void LED2_On(void);
void LED2_Off(void);
void LED2_Toggle(void);
void LED2_Switch(bool);

void Keyboard_Scan(void);
void Settings_Read(void);
void Settings_Write(void);
void Layout_Switch(void);
void Hardware_Setup(void);

uint16_t Chords[7] = {0, 0, 0, 0, 0, 0, 0};
// Ports_Init(), LEDs(), Keyboard_Scan()
#include "codenets.h"

// Layers
#define HID_KEYBOARD_LAYER_FN 0xFF
#define HID_KEYBOARD_LAYER_FN2 0xFE
#define HID_KEYBOARD_LAYER_1 0xFD
#define HID_KEYBOARD_LAYER_2 0xFC
#define HID_KEYBOARD_LAYER_MOU 0xFB
//#include "layouts.h"
#include "zorax.h"

#define LAYER1 0
#define LAYER2 1
#define LAYER_FN 2
#define LAYER_FN2 3

#define NAV_MODE 0
#define MOU_MODE 1

#define MACROS_BUFFER_SIZE 200
#define MACROS_BUFFER_MAX 100
uint8_t Macros_Buffer[MACROS_BUFFER_SIZE];
uint8_t Macros_Index = 0;

int16_t Mouse_X;
int16_t Mouse_Y;
int16_t Mouse_W;
uint8_t Mouse_Button;
uint8_t Mouse_Button_Click;

// const int16_t mouMoves[16] PROGMEM = {20, 1,   5,   10,  15,  30,  40,  50,
//                                      75, 100, 200, 300, 400, 500, 750, 1000};
// const int16_t mouScrolls[16] PROGMEM = {10, 1,  2,  4,  7,  15, 20, 25,
//                                        30, 40, 50, 60, 70, 80, 90, 100};

bool Chord_Growing = true;
uint8_t Q_Mods = 0;
// uint8_t Q_Multiplier = 0;
uint8_t Q_Nav = NAV_MODE;
uint8_t Layer_Current = LAYER2;

// uint16_t Chords_Last[7] = {0, 0, 0, 0, 0, 0, 0};
uint8_t Press_Index = 0;
uint16_t Chords_Press[72];

// EEPROM Settings
#define LAYOUTS_TWO 0
#define LAYOUTS_ONE 1

uint8_t Layout_Mode = LAYOUTS_TWO;
uint8_t EE_Layout_Mode EEMEM;

#define OS_LINUX 0
#define OS_WINDOWS 1
#define OS_MAC 2
#define OS_ANDROID 3

uint8_t OS_Mode = OS_LINUX;
uint8_t EE_OS_Mode EEMEM;

#define LEVEL_1 0
#define LEVEL_2 1
#define LEVEL_3 2

uint8_t Level_Mode = LEVEL_1;
uint8_t EE_Level_Mode EEMEM;

uint8_t Meta = HID_KEYBOARD_MODIFIER_LEFTCTRL;

bool Settings = false;

void Settings_Read() {
  Layout_Mode = eeprom_read_byte(&EE_Layout_Mode);
  OS_Mode = eeprom_read_byte(&EE_OS_Mode);
  Level_Mode = eeprom_read_byte(&EE_Level_Mode);
  Meta = (OS_Mode == OS_MAC) ? HID_KEYBOARD_MODIFIER_LEFTGUI
                             : HID_KEYBOARD_MODIFIER_LEFTCTRL;
}
void Settings_Write() {
  eeprom_write_byte(&EE_Layout_Mode, Layout_Mode);
  eeprom_write_byte(&EE_OS_Mode, OS_Mode);
  eeprom_write_byte(&EE_Level_Mode, Level_Mode);
}

void Layout_Switch() {
  if (OS_Mode == OS_WINDOWS) { // Ctrl+Shift
    Macros_Buffer[Macros_Index++] = 0;
    Macros_Buffer[Macros_Index++] =
        HID_KEYBOARD_MODIFIER_LEFTSHIFT | HID_KEYBOARD_MODIFIER_LEFTCTRL;
  } else if (OS_Mode == OS_MAC ||
             OS_Mode == OS_ANDROID) { // Cmd+Space || Win+Space
    Macros_Buffer[Macros_Index++] = HID_KEYBOARD_SC_SPACE;
    Macros_Buffer[Macros_Index++] = HID_KEYBOARD_MODIFIER_LEFTGUI;
  } else { // Alt+Shift
    Macros_Buffer[Macros_Index++] = 0;
    Macros_Buffer[Macros_Index++] =
        HID_KEYBOARD_MODIFIER_LEFTSHIFT | HID_KEYBOARD_MODIFIER_LEFTALT;
  }
}

void Get_Mods(uint8_t chord) {
  Q_Mods = 0;
  bool mods_dbl[6] = {false, false, false, false, false, false};
  uint8_t keyCode = 0;
  for (int8_t i = 0; i < 12; i++) {
    keyCode = 0;
    if (((i > 0 && i < 6) &&
         ((chord & (0x3 << (i - 1))) == (0x3 << (i - 1)))) ||
        ((i == 0) && ((chord & 0x21) == 0x21)) ||
        ((i >= 6) && (chord & (0x1 << (i - 6))))) {
      keyCode = pgm_read_byte(&Layer_Mods[i]);
    }
    if (keyCode) {
      if (i == 0) {
        mods_dbl[0] = true;
        mods_dbl[5] = true;
      } else if (i < 6) {
        mods_dbl[i - 1] = true;
        mods_dbl[i] = true;
      }
      if ((i < 6) || ((i >= 6) && !mods_dbl[i - 6])) {
        if (keyCode >= HID_KEYBOARD_SC_LEFT_CONTROL &&
            keyCode <= HID_KEYBOARD_SC_RIGHT_GUI) {
          Q_Mods ^= 1 << (keyCode - HID_KEYBOARD_SC_LEFT_CONTROL);
        }
      }
    }
  }
}

bool IsVowels(uint8_t keyCode) {
  bool v = false;
  if (Layer_Current == LAYER2) {
    if (keyCode == HID_KEYBOARD_SC_U || keyCode == HID_KEYBOARD_SC_E ||
        keyCode == HID_KEYBOARD_SC_Y || keyCode == HID_KEYBOARD_SC_A ||
        keyCode == HID_KEYBOARD_SC_I || keyCode == HID_KEYBOARD_SC_O)
      v = true;
  } else if (Layer_Current == LAYER1) {
    if (keyCode == HID_KEYBOARD_RU_A || keyCode == HID_KEYBOARD_RU_E ||
        keyCode == HID_KEYBOARD_RU_I || keyCode == HID_KEYBOARD_RU_O ||
        keyCode == HID_KEYBOARD_RU_U || keyCode == HID_KEYBOARD_RU_SOFTSIGN ||
        keyCode == HID_KEYBOARD_RU_YI || keyCode == HID_KEYBOARD_RU_YE ||
        keyCode == HID_KEYBOARD_RU_YU || keyCode == HID_KEYBOARD_RU_YA ||
        keyCode == HID_KEYBOARD_RU_YO)
      v = true;
  }
  return v;
}

/** Buffer to hold the previously generated Keyboard HID report, for
 * comparison purposes inside the HID class driver. */
static uint8_t PrevKeyboardHIDReportBuffer[sizeof(USB_KeyboardReport_Data_t)];

/** Buffer to hold the previously generated Mouse HID report, for comparison
 * purposes inside the HID class driver. */
static uint8_t PrevMouseHIDReportBuffer[sizeof(USB_MouseReport_Data_t)];

/** LUFA HID Class driver interface configuration and state information. This
 * structure is passed to all HID Class driver functions, so that multiple
 * instances of the same class within a device can be differentiated from one
 * another. This is for the keyboard HID interface within the device.
 */
USB_ClassInfo_HID_Device_t Keyboard_HID_Interface = {
    .Config =
        {
            .InterfaceNumber = 0,
            .ReportINEndpoint =
                {
                    .Address = KEYBOARD_IN_EPADDR,
                    .Size = HID_EPSIZE,
                    .Banks = 1,
                },
            .PrevReportINBuffer = PrevKeyboardHIDReportBuffer,
            .PrevReportINBufferSize = sizeof(PrevKeyboardHIDReportBuffer),
        },
};

/** LUFA HID Class driver interface configuration and state information. This
 * structure is passed to all HID Class driver functions, so that multiple
 * instances of the same class within a device can be differentiated from one
 * another. This is for the mouse HID interface within the device.
 */
USB_ClassInfo_HID_Device_t Mouse_HID_Interface = {
    .Config =
        {
            .InterfaceNumber = 1,
            .ReportINEndpoint =
                {
                    .Address = MOUSE_IN_EPADDR,
                    .Size = HID_EPSIZE,
                    .Banks = 1,
                },
            .PrevReportINBuffer = PrevMouseHIDReportBuffer,
            .PrevReportINBufferSize = sizeof(PrevMouseHIDReportBuffer),
        },
};

/** Configures the board hardware and chip peripherals for the demo's
 * functionality. */
void Hardware_Setup() {
  // Set for 16 MHz clock
  // CLKPR = 0x80; CLKPR = 0;

  /* Disable watchdog if enabled by bootloader/fuses */
  MCUSR &= ~(1 << WDRF);
  wdt_disable();

  /* Disable clock division */
  clock_prescale_set(clock_div_1);

  Ports_Init();
  LED_On();
  USB_Init();
  LED_Off();
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void) {
  // LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void) {
  // LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void) {
  bool ConfigSuccess = true;

  ConfigSuccess &= HID_Device_ConfigureEndpoints(&Keyboard_HID_Interface);
  ConfigSuccess &= HID_Device_ConfigureEndpoints(&Mouse_HID_Interface);

  USB_Device_EnableSOFEvents();

  // LED_Switch(! ConfigSuccess);
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void) {
  HID_Device_ProcessControlRequest(&Keyboard_HID_Interface);
  HID_Device_ProcessControlRequest(&Mouse_HID_Interface);
}

/** Event handler for the USB device Start Of Frame event. */
void EVENT_USB_Device_StartOfFrame(void) {
  HID_Device_MillisecondElapsed(&Keyboard_HID_Interface);
  HID_Device_MillisecondElapsed(&Mouse_HID_Interface);
}

/** HID class driver callback function for the creation of HID reports to the
 * host.
 *
 *  \param[in]     HIDInterfaceInfo  Pointer to the HID class interface
 * configuration structure being referenced \param[in,out] ReportID    Report
 * ID requested by the host if non-zero, otherwise callback should set to the
 * generated report ID \param[in]     ReportType  Type of the report to
 * create, either HID_REPORT_ITEM_In or HID_REPORT_ITEM_Feature \param[out]
 * ReportData Pointer to a buffer where the created report should be stored
 * \param[out] ReportSize  Number of bytes written in the report (or zero if
 * no report is to be sent)
 *
 *  \return Boolean true to force the sending of the report, false to let the
 * library determine if it needs to be sent
 */
bool CALLBACK_HID_Device_CreateHIDReport(
    USB_ClassInfo_HID_Device_t *const HIDInterfaceInfo, uint8_t *const ReportID,
    const uint8_t ReportType, void *ReportData, uint16_t *const ReportSize) {
  if (Macros_Index < (MACROS_BUFFER_SIZE - MACROS_BUFFER_MAX)) {

    uint16_t chords[7] = {Chords[0], Chords[1], Chords[2], Chords[3],
                          Chords[4], Chords[5], Chords[6]};
    Keyboard_Scan();

    bool isPress = Chords[0] > chords[0] || Chords[1] > chords[1] ||
                   Chords[2] > chords[2] || Chords[3] > chords[3] ||
                   Chords[4] > chords[4] || Chords[5] > chords[5] ||
                   Chords[6] > chords[6];
    bool isRelease = (Chords[0] < chords[0] || Chords[1] < chords[1] ||
                      Chords[2] < chords[2] || Chords[3] < chords[3] ||
                      Chords[4] < chords[4] || Chords[5] < chords[5] ||
                      Chords[6] < chords[6]);

    if (isRelease) {
      Get_Mods(chords[0]);
      if (Chord_Growing) {
        uint8_t multiplier = 0;

        uint8_t layer = (Q_Mods & ~(HID_KEYBOARD_MODIFIER_LEFTSHIFT |
                                    HID_KEYBOARD_MODIFIER_RIGHTSHIFT))
                            ? LAYER1
                            : Layer_Current;

        if (Settings) { // Settings
          Settings = false;
          if (chords[4] & 0x3) {
            Layout_Mode = (chords[4] & 0x3) - 1;
          }
          if (chords[5] & 0x3) {
            OS_Mode = (chords[5] & 0x3) - 1;
            Meta = (OS_Mode == OS_MAC) ? HID_KEYBOARD_MODIFIER_LEFTGUI
                                       : HID_KEYBOARD_MODIFIER_LEFTCTRL;
          }
          if (chords[6] & 0x3) {
            Level_Mode = (chords[6] & 0x3) - 1;
          }
          if (chords[0] & 0x20) {
            Settings_Write();
          }
          LED_Toggle();
        } else if (chords[0] == 0x10 &&
                   ((chords[4] == 0x14 && chords[5] == 0xA) ||
                    (chords[4] == 0xA && chords[5] == 0x14))) { // Settings Mode
          Settings = true;
          LED_Toggle();
        } else {
          uint8_t keyCode = 0;
          bool isSpace = false;
          // Mods
          bool mods_dbl[6] = {false, false, false, false, false, false};
          for (int8_t i = 0; i < 12; i++) {
            keyCode = 0;
            if (((i > 0 && i < 6) &&
                 ((chords[0] & (0x3 << (i - 1))) == (0x3 << (i - 1)))) ||
                ((i == 0) && ((chords[0] & 0x21) == 0x21)) ||
                ((i >= 6) && ((chords[0] & (0x1 << (i - 6)))))) {
              keyCode = pgm_read_byte(&Layer_Mods[i]);
            }
            if (keyCode) {
              if (i == 0) {
                mods_dbl[0] = true;
                mods_dbl[5] = true;
              } else if (i < 6) {
                mods_dbl[i - 1] = true;
                mods_dbl[i] = true;
              }
              if ((i < 6) || ((i >= 6) && !mods_dbl[i - 6])) {
                if (keyCode >= HID_KEYBOARD_SC_LEFT_CONTROL &&
                    keyCode <= HID_KEYBOARD_SC_RIGHT_GUI) {
                  // nothing
                } else if (keyCode == HID_KEYBOARD_LAYER_FN) {
                  layer = LAYER_FN;
                } else if (keyCode == HID_KEYBOARD_LAYER_FN2) {
                  layer = LAYER_FN2;
                } else if (keyCode == HID_KEYBOARD_SC_SPACE) {
                  isSpace = true;
                } else {
                  Macros_Buffer[Macros_Index++] = keyCode;
                  Macros_Buffer[Macros_Index++] = Q_Mods;
                }
              }
            }
          }
          // Symbols
          if (Level_Mode == LEVEL_1) {
            bool chords_dbl[6][5] = {{false, false, false, false, false},
                                     {false, false, false, false, false},
                                     {false, false, false, false, false},
                                     {false, false, false, false, false},
                                     {false, false, false, false, false},
                                     {false, false, false, false, false}};
            for (int8_t k = 0; k < Press_Index; k += 2) {
              int8_t j = Chords_Press[k];
              int8_t i = Chords_Press[k + 1];
              keyCode = 0;
              if (j > 5) {
                if (layer != LAYER_FN && layer != LAYER_FN2) {
                  keyCode = HID_KEYBOARD_SC_SPACE;
                }
              } else {
                if (!keyCode && i > 0 && !(chords_dbl[j][i - 1] || chords_dbl[j][i]) &&
                    (chords[j + 1] & (1 << (i - 1)))) {
                  keyCode = pgm_read_byte(&Layers[layer][j][i - 1]);
                  if (keyCode) {
                    chords_dbl[j][i - 1] = true;
                    chords_dbl[j][i] = true;
                  }
                }
                if (!keyCode && !(chords_dbl[j][i] || chords_dbl[j][i + 1]) &&
                    ((i < 4) && (chords[j + 1] & (1 << (i + 1))))) {
                  keyCode = pgm_read_byte(&Layers[layer][j][i]);
                  if (keyCode) {
                    chords_dbl[j][i] = true;
                    chords_dbl[j][i + 1] = true;
                  }
                }
                if (!keyCode && !chords_dbl[j][i]) {
                  keyCode = pgm_read_byte(&Layers[layer][j][i + 4]);
                }
              }
              if (keyCode) {
                if (keyCode == HID_KEYBOARD_LAYER_1) {
                  if (Layer_Current != LAYER1) {
                    Layout_Switch();
                    Layer_Current = LAYER1;
                    LED_On();
                  }
                } else if (keyCode == HID_KEYBOARD_LAYER_2) {
                  if (Layer_Current != LAYER2) {
                    Layout_Switch();
                    Layer_Current = LAYER2;
                    LED_Off();
                  }
                } else if (keyCode == HID_KEYBOARD_LAYER_MOU) {
                  // TODO: mouse
                } else if (keyCode >= HID_KEYBOARD_SC_1_AND_EXCLAMATION &&
                           keyCode <=
                               HID_KEYBOARD_SC_0_AND_CLOSING_PARENTHESIS &&
                           (chords[4] || chords[5] ||
                            (chords[6] & ~0x1)) && layer == LAYER_FN) { // Multiplier
                  multiplier +=
                      (keyCode - HID_KEYBOARD_SC_1_AND_EXCLAMATION) + 1;
                } else if ((keyCode == HID_KEYBOARD_SC_HOME ||
                            keyCode == HID_KEYBOARD_SC_END) &&
                           (Q_Mods & (HID_KEYBOARD_MODIFIER_LEFTCTRL |
                                      HID_KEYBOARD_MODIFIER_RIGHTCTRL))) {
                  if (multiplier > 1) {
                    multiplier = 0;
                    Macros_Buffer[Macros_Index] = keyCode;
                    Macros_Index++;
                    Macros_Buffer[Macros_Index] = Q_Mods;
                    Macros_Index++;
                  } else {
                    Macros_Buffer[Macros_Index] = keyCode;
                    Macros_Index++;
                    Macros_Buffer[Macros_Index] =
                        Macros_Index == Q_Mods &
                        ~(HID_KEYBOARD_MODIFIER_LEFTCTRL |
                          HID_KEYBOARD_MODIFIER_RIGHTCTRL);
                    Macros_Index++;
                  }
                } else {
                  Macros_Buffer[Macros_Index] = keyCode;
                  Macros_Index++;
                  Macros_Buffer[Macros_Index] = Macros_Index == 1 ? Q_Mods : 0;
                  Macros_Index++;
                }
              }
            }
          } else {
            for (int8_t v = 0; v <= 1; v++) {
              for (int8_t j = 0; j < 6; j++) {
                bool chords_dbl[5] = {false, false, false, false, false};
                for (int8_t i = 0; i < 9; i++) {
                  keyCode = 0;
                  if (((i < 4) &&
                       ((chords[j + 1] & (0x3 << i)) == (0x3 << i))) ||
                      ((i >= 4) && (chords[j + 1] & (0x1 << (i - 4))))) {
                    keyCode = pgm_read_byte(&Layers[layer][j][i]);
                  }
                  if (keyCode) {
                    if (i < 4) {
                      chords_dbl[i] = true;
                      chords_dbl[i + 1] = true;
                    }
                    if ((i < 4) || ((i >= 4) && !chords_dbl[i - 4])) {
                      if (!v && keyCode == HID_KEYBOARD_LAYER_1) {
                        if (Layer_Current != LAYER1) {
                          Layout_Switch();
                          Layer_Current = LAYER1;
                          LED_On();
                        }
                      } else if (!v && keyCode == HID_KEYBOARD_LAYER_2) {
                        if (Layer_Current != LAYER2) {
                          Layout_Switch();
                          Layer_Current = LAYER2;
                          LED_Off();
                        }
                      } else if (!v && keyCode == HID_KEYBOARD_LAYER_MOU) {
                        // TODO: mouse
                      } else if (
                          !v && keyCode >= HID_KEYBOARD_SC_1_AND_EXCLAMATION &&
                          keyCode <=
                              HID_KEYBOARD_SC_0_AND_CLOSING_PARENTHESIS &&
                          (chords[4] || chords[5] ||
                           (chords[6] & ~0x1)) && layer == LAYER_FN) { // Multiplier
                        multiplier +=
                            (keyCode - HID_KEYBOARD_SC_1_AND_EXCLAMATION) + 1;
                      } else if (!v &&
                                 (keyCode == HID_KEYBOARD_SC_HOME ||
                                  keyCode == HID_KEYBOARD_SC_END) &&
                                 (Q_Mods & (HID_KEYBOARD_MODIFIER_LEFTCTRL |
                                            HID_KEYBOARD_MODIFIER_RIGHTCTRL))) {
                        if (multiplier > 1) {
                          multiplier = 0;
                          Macros_Buffer[Macros_Index] = keyCode;
                          Macros_Index++;
                          Macros_Buffer[Macros_Index] = Q_Mods;
                          Macros_Index++;
                        } else {
                          Macros_Buffer[Macros_Index] = keyCode;
                          Macros_Index++;
                          Macros_Buffer[Macros_Index] =
                              Macros_Index == Q_Mods &
                              ~(HID_KEYBOARD_MODIFIER_LEFTCTRL |
                                HID_KEYBOARD_MODIFIER_RIGHTCTRL);
                          Macros_Index++;
                        }
                      } else if ((!v && !IsVowels(keyCode)) ||
                                 (v && IsVowels(keyCode))) {
                        Macros_Buffer[Macros_Index] = keyCode;
                        Macros_Index++;
                        Macros_Buffer[Macros_Index] =
                            Macros_Index == 1 ? Q_Mods : 0;
                        Macros_Index++;
                      }
                    }
                  }
                }
              }
            }
            if (isSpace) {
              Macros_Buffer[Macros_Index] = HID_KEYBOARD_SC_SPACE;
              Macros_Index++;
              Macros_Buffer[Macros_Index] = Macros_Index == 1 ? Q_Mods : 0;
              Macros_Index++;
            }
          }
        }

        if (Macros_Index == 2 && multiplier && Macros_Buffer[0]) {
          if (multiplier == 1) {
            Macros_Buffer[1] |= HID_KEYBOARD_MODIFIER_LEFTGUI;
          } else {
            for (uint8_t i = 1; i < multiplier; i++) {
              Macros_Buffer[Macros_Index++] = Macros_Buffer[i * 2 - 2];
              Macros_Buffer[Macros_Index++] = Macros_Buffer[i * 2 - 1];
            }
          }
        }

        /*Chords_Last[0] = chords[0];
        Chords_Last[1] = chords[1];
        Chords_Last[2] = chords[2];
        Chords_Last[3] = chords[3];
        Chords_Last[4] = chords[4];
        Chords_Last[5] = chords[5];
        Chords_Last[6] = chords[6];*/
        Chord_Growing = false;

        // if (Layer_Current != layer) {
        //  Layout_Switch();
        //}
      }

      if (Level_Mode == LEVEL_1) { // Quasichords
        Press_Index = 0;
      }
    }
    if (isPress) {
      Chord_Growing = true;
      if (Level_Mode == LEVEL_1) { // Quasichords
        if (Chords[0] > (chords[0] & Chords[0])) { // Space
          uint16_t ChordPress =
              Chords[0] - (chords[0] & Chords[0]);
          if (ChordPress & 0x10) {
            Chords_Press[Press_Index] = 6;
            Press_Index++;
            Chords_Press[Press_Index] = 5;
            Press_Index++;
          }
        }
        for (uint8_t j = 0; j < 6; j++) {
          if (Chords[j + 1] > (chords[j + 1] & Chords[j + 1])) {
            uint8_t ChordPress =
                Chords[j + 1] - (chords[j + 1] & Chords[j + 1]);
            for (int8_t i = 0; i < 5; i++) {
              if (ChordPress & (1 << i)) {
                Chords_Press[Press_Index] = j;
                Press_Index++;
                Chords_Press[Press_Index] = i;
                Press_Index++;
              }
            }
          }
        }
      }
    }
    Get_Mods(chords[0]);
  }
  /* Determine which interface must have its report generated */
  if (HIDInterfaceInfo == &Keyboard_HID_Interface) {
    USB_KeyboardReport_Data_t *KeyboardReport =
        (USB_KeyboardReport_Data_t *)ReportData;

    if (Macros_Index) {
      KeyboardReport->KeyCode[0] =
          (Macros_Buffer[0] == 0xFF ? 0 : Macros_Buffer[0]);
      KeyboardReport->Modifier = Macros_Buffer[1];
      if (Macros_Buffer[0] == Macros_Buffer[2] && Macros_Buffer[2] != 0xFF) {
        Macros_Buffer[0] = 0xFF;
      } else {
        for (uint8_t i = 0; i <= (Macros_Index - 2); i += 2) {
          Macros_Buffer[i + 0] = Macros_Buffer[i + 2];
          Macros_Buffer[i + 1] = Macros_Buffer[i + 3];
          if (!Macros_Buffer[i + 0] && !Macros_Buffer[i + 1])
            break;
        }
        Macros_Buffer[MACROS_BUFFER_SIZE - 1] = 0;
        Macros_Buffer[MACROS_BUFFER_SIZE - 2] = 0;
        Macros_Index -= 2;
      }
    } else {
      KeyboardReport->Modifier = Q_Mods;
    }
    LED2_Switch(KeyboardReport->Modifier);

    *ReportSize = sizeof(USB_KeyboardReport_Data_t);
    return true;
  } else {
    USB_MouseReport_Data_t *MouseReport = (USB_MouseReport_Data_t *)ReportData;

    int8_t mouseX = Mouse_X > 100 ? 100 : (Mouse_X < -100 ? -100 : Mouse_X);
    if (Mouse_X > 100)
      Mouse_X -= 100;
    else if (Mouse_X < -100)
      Mouse_X += 100;
    else
      Mouse_X = 0;

    int8_t mouseY = Mouse_Y > 100 ? 100 : (Mouse_Y < -100 ? -100 : Mouse_Y);
    if (Mouse_Y > 100)
      Mouse_Y -= 100;
    else if (Mouse_Y < -100)
      Mouse_Y += 100;
    else
      Mouse_Y = 0;

    int8_t mouseW = Mouse_W > 100 ? 100 : (Mouse_W < -100 ? -100 : Mouse_W);
    if (Mouse_W > 100)
      Mouse_W -= 100;
    else if (Mouse_W < -100)
      Mouse_W += 100;
    else
      Mouse_W = 0;

    MouseReport->X = mouseX;
    MouseReport->Y = mouseY;
    MouseReport->W = mouseW;

    MouseReport->Button = Mouse_Button | Mouse_Button_Click;
    if (!Mouse_X && !Mouse_Y && !Mouse_W)
      Mouse_Button_Click = 0;

    *ReportSize = sizeof(USB_MouseReport_Data_t);
    return true;
  }
}

/** HID class driver callback function for the processing of HID reports from
 * the host.
 *
 *  \param[in] HIDInterfaceInfo  Pointer to the HID class interface
 * configuration structure being referenced \param[in] ReportID    Report ID
 * of the received report from the host \param[in] ReportType  The type of
 * report that the host has sent, either HID_REPORT_ITEM_Out or
 * HID_REPORT_ITEM_Feature \param[in] ReportData  Pointer to a buffer where
 * the received report has been stored \param[in] ReportSize  Size in bytes of
 * the received HID report
 */
void CALLBACK_HID_Device_ProcessHIDReport(
    USB_ClassInfo_HID_Device_t *const HIDInterfaceInfo, const uint8_t ReportID,
    const uint8_t ReportType, const void *ReportData,
    const uint16_t ReportSize) {
  if (HIDInterfaceInfo == &Keyboard_HID_Interface) {
    uint8_t *LEDReport = (uint8_t *)ReportData;

    if (*LEDReport & HID_KEYBOARD_LED_CAPSLOCK) {
      // LED_On();
    }

    if (*LEDReport & HID_KEYBOARD_LED_SCROLLLOCK) {
      // LED_On();
    }

    /*uint8_t  LEDMask   = LEDS_NO_LEDS;

    if (*LEDReport & HID_KEYBOARD_LED_NUMLOCK)
      LEDMask |= LEDS_LED1;

    LEDs_SetAllLEDs(LEDMask);*/
  }
}

int main(void) {
  Hardware_Setup();
  GlobalInterruptEnable();
  Settings_Read();
  while (true) {
    HID_Device_USBTask(&Keyboard_HID_Interface);
    HID_Device_USBTask(&Mouse_HID_Interface);
    USB_USBTask();
  }
}
