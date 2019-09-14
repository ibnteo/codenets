// Vowels Layer1
#define HID_KEYBOARD_RU_A HID_KEYBOARD_SC_F
#define HID_KEYBOARD_RU_E HID_KEYBOARD_SC_T
#define HID_KEYBOARD_RU_I HID_KEYBOARD_SC_B
#define HID_KEYBOARD_RU_O HID_KEYBOARD_SC_J
#define HID_KEYBOARD_RU_U HID_KEYBOARD_SC_E
#define HID_KEYBOARD_RU_SOFTSIGN HID_KEYBOARD_SC_M
#define HID_KEYBOARD_RU_YI HID_KEYBOARD_SC_S
#define HID_KEYBOARD_RU_YE HID_KEYBOARD_SC_APOSTROPHE_AND_QUOTE
#define HID_KEYBOARD_RU_YU HID_KEYBOARD_SC_DOT_AND_GREATER_THAN_SIGN
#define HID_KEYBOARD_RU_YA HID_KEYBOARD_SC_Z
#define HID_KEYBOARD_RU_YO HID_KEYBOARD_SC_GRAVE_ACCENT_AND_TILDE

// Modificators
const uint8_t Layer_Mods[12] PROGMEM = {
    HID_KEYBOARD_SC_LEFT_GUI,     // Alt+AltGr
    0,                            // Alt+Ctrl
    0,                            // Ctrl+Shift
    0,                            // Shift+Space
    HID_KEYBOARD_LAYER_FN2,       // Space+Fn
    HID_KEYBOARD_SC_LEFT_GUI,     // Fn+AltGr
    HID_KEYBOARD_SC_LEFT_ALT,     // Alt
    HID_KEYBOARD_SC_LEFT_CONTROL, // Ctrl
    HID_KEYBOARD_SC_LEFT_SHIFT,   // Shift
    HID_KEYBOARD_SC_SPACE,        // Space
    HID_KEYBOARD_LAYER_FN,        // Fn
    HID_KEYBOARD_SC_RIGHT_ALT     // AltGr
};

const uint8_t Layers[4][6][9] PROGMEM = {
    {// Layer1 (QWERTY)
     {
         HID_KEYBOARD_SC_GRAVE_ACCENT_AND_TILDE, // QW
         0,                                      // WE
         0,                                      // ER
         0,                                      // RT
         HID_KEYBOARD_SC_Q,                      // Q
         HID_KEYBOARD_SC_W,                      // W
         HID_KEYBOARD_SC_E,                      // E
         HID_KEYBOARD_SC_R,                      // R
         HID_KEYBOARD_SC_T,                      // T
     },
     {
         HID_KEYBOARD_SC_TAB, // AS
         0,                   // SD
         0,                   // DF
         0,                   // FG
         HID_KEYBOARD_SC_A,   // A
         HID_KEYBOARD_SC_S,   // S
         HID_KEYBOARD_SC_D,   // D
         HID_KEYBOARD_SC_F,   // F
         HID_KEYBOARD_SC_G,   // G
     },
     {
         0,                 // ZX
         0,                 // XC
         0,                 // CV
         0,                 // VB
         HID_KEYBOARD_SC_Z, // Z
         HID_KEYBOARD_SC_X, // X
         HID_KEYBOARD_SC_C, // C
         HID_KEYBOARD_SC_V, // V
         HID_KEYBOARD_SC_B, // B
     },
     {
         0,                                                 // YU
         0,                                                 // UI
         HID_KEYBOARD_SC_OPENING_BRACKET_AND_OPENING_BRACE, // IO
         HID_KEYBOARD_SC_CLOSING_BRACKET_AND_CLOSING_BRACE, // OP
         HID_KEYBOARD_SC_Y,                                 // Y
         HID_KEYBOARD_SC_U,                                 // U
         HID_KEYBOARD_SC_I,                                 // I
         HID_KEYBOARD_SC_O,                                 // O
         HID_KEYBOARD_SC_P,                                 // P
     },
     {
         0,                                    // HJ
         0,                                    // JK
         0,                                    // KL
         HID_KEYBOARD_SC_APOSTROPHE_AND_QUOTE, // L;
         HID_KEYBOARD_SC_H,                    // H
         HID_KEYBOARD_SC_J,                    // J
         HID_KEYBOARD_SC_K,                    // K
         HID_KEYBOARD_SC_L,                    // L
         HID_KEYBOARD_SC_SEMICOLON_AND_COLON,  // ;
     },
     {
         0,                                         // NM
         0,                                         // M,
         0,                                         // ,.
         0,                                         // ./
         HID_KEYBOARD_SC_N,                         // N
         HID_KEYBOARD_SC_M,                         // M
         HID_KEYBOARD_SC_COMMA_AND_LESS_THAN_SIGN,  // ,<
         HID_KEYBOARD_SC_DOT_AND_GREATER_THAN_SIGN, // .>
         HID_KEYBOARD_SC_SLASH_AND_QUESTION_MARK,   // /?
     }},
    {// Layer2 (JCUKEN)
     {
         HID_KEYBOARD_SC_GRAVE_ACCENT_AND_TILDE, // JC
         0,                                      // CU
         0,                                      // UK
         0,                                      // KE
         HID_KEYBOARD_SC_J,                      // J
         HID_KEYBOARD_SC_C,                      // C
         HID_KEYBOARD_SC_U,                      // U
         HID_KEYBOARD_SC_K,                      // K
         HID_KEYBOARD_SC_E,                      // E
     },
     {
         HID_KEYBOARD_SC_TAB, // FY
         0,                   // YW
         0,                   // WA
         0,                   // AP
         HID_KEYBOARD_SC_F,   // F
         HID_KEYBOARD_SC_Y,   // Y
         HID_KEYBOARD_SC_W,   // W
         HID_KEYBOARD_SC_A,   // A
         HID_KEYBOARD_SC_P,   // P
     },
     {
         0,                                       // Q/
         0,                                       // /S
         0,                                       // SM
         0,                                       // MI
         HID_KEYBOARD_SC_Q,                       // Q
         HID_KEYBOARD_SC_SLASH_AND_QUESTION_MARK, // /
         HID_KEYBOARD_SC_S,                       // S
         HID_KEYBOARD_SC_M,                       // M
         HID_KEYBOARD_SC_I,                       // I
     },
     {
         0,                                                 // NG
         0,                                                 // GH
         HID_KEYBOARD_SC_OPENING_BRACKET_AND_OPENING_BRACE, // HV
         HID_KEYBOARD_SC_CLOSING_BRACKET_AND_CLOSING_BRACE, // VZ
         HID_KEYBOARD_SC_N,                                 // N
         HID_KEYBOARD_SC_G,                                 // G
         HID_KEYBOARD_SC_H,                                 // H
         HID_KEYBOARD_SC_V,                                 // V
         HID_KEYBOARD_SC_Z,                                 // Z
     },
     {
         0,                                    // RO
         0,                                    // OL
         0,                                    // LD
         HID_KEYBOARD_SC_APOSTROPHE_AND_QUOTE, // D;
         HID_KEYBOARD_SC_R,                    // R
         HID_KEYBOARD_SC_O,                    // O
         HID_KEYBOARD_SC_L,                    // L
         HID_KEYBOARD_SC_D,                    // D
         HID_KEYBOARD_SC_SEMICOLON_AND_COLON,  // ;
     },
     {
         0,                                         // TX
         0,                                         // XB
         0,                                         // B,
         0,                                         // ,.
         HID_KEYBOARD_SC_T,                         // T
         HID_KEYBOARD_SC_X,                         // X
         HID_KEYBOARD_SC_B,                         // B
         HID_KEYBOARD_SC_COMMA_AND_LESS_THAN_SIGN,  // ,
         HID_KEYBOARD_SC_DOT_AND_GREATER_THAN_SIGN, // .
     }},
    {// Navigation and Numbers
     {
         0,                                 // QW
         0,                                 // WE
         0,                                 // ER
         0,                                 // RT
         HID_KEYBOARD_SC_1_AND_EXCLAMATION, // Q
         HID_KEYBOARD_SC_2_AND_AT,          // W
         HID_KEYBOARD_SC_3_AND_HASHMARK,    // E
         HID_KEYBOARD_SC_4_AND_DOLLAR,      // R
         HID_KEYBOARD_SC_5_AND_PERCENTAGE,  // T
     },
     {
         0,                                         // AS
         0,                                         // SD
         0,                                         // DF
         0,                                         // FG
         HID_KEYBOARD_SC_6_AND_CARET,               // A
         HID_KEYBOARD_SC_7_AND_AND_AMPERSAND,       // S
         HID_KEYBOARD_SC_8_AND_ASTERISK,            // D
         HID_KEYBOARD_SC_9_AND_OPENING_PARENTHESIS, // F
         HID_KEYBOARD_SC_0_AND_CLOSING_PARENTHESIS, // G
     },
     {
         HID_KEYBOARD_LAYER_1,                      // ZX
         0,                                         // XC
         0,                                         // CV
         0,                                         // VB
         HID_KEYBOARD_SC_BACKSLASH_AND_PIPE,        // Z
         HID_KEYBOARD_SC_SLASH_AND_QUESTION_MARK,   // X
         HID_KEYBOARD_SC_EQUAL_AND_PLUS,            // C
         HID_KEYBOARD_SC_MINUS_AND_UNDERSCORE,      // V
         HID_KEYBOARD_SC_DOT_AND_GREATER_THAN_SIGN, // B
     },
     {
         0,                         // YU
         0,                         // UI
         HID_KEYBOARD_SC_INSERT,    // IO
         0,                         // OP
         HID_KEYBOARD_SC_PAGE_UP,   // Y
         HID_KEYBOARD_LAYER_MOU,    // U
         HID_KEYBOARD_SC_UP_ARROW,  // I
         HID_KEYBOARD_SC_DELETE,    // O
         HID_KEYBOARD_SC_BACKSPACE, // P
     },
     {
         0,                           // HJ
         0,                           // JK
         0,                           // KL
         0,                           // L;
         HID_KEYBOARD_SC_PAGE_DOWN,   // H
         HID_KEYBOARD_SC_LEFT_ARROW,  // J
         HID_KEYBOARD_SC_DOWN_ARROW,  // K
         HID_KEYBOARD_SC_RIGHT_ARROW, // L
         HID_KEYBOARD_SC_ENTER,       // ;
     },
     {
         0,                      // NM
         0,                      // M,
         0,                      // ,.
         HID_KEYBOARD_LAYER_2,   // ./
         HID_KEYBOARD_SC_SPACE,  // N
         HID_KEYBOARD_SC_HOME,   // M
         HID_KEYBOARD_SC_TAB,    // ,<
         HID_KEYBOARD_SC_END,    // .>
         HID_KEYBOARD_SC_ESCAPE, // /?
     }},
    {// Function and Number block
     {
         0,                            // QW
         0,                            // WE
         0,                            // ER
         0,                            // RT
         HID_KEYBOARD_SC_F1,           // Q
         HID_KEYBOARD_SC_F2,           // W
         HID_KEYBOARD_SC_F3,           // E
         HID_KEYBOARD_SC_F4,           // R
         HID_KEYBOARD_SC_PRINT_SCREEN, // T
     },
     {
         HID_KEYBOARD_SC_CAPS_LOCK,   // AS
         0,                           // SD
         0,                           // DF
         0,                           // FG
         HID_KEYBOARD_SC_F5,          // A
         HID_KEYBOARD_SC_F6,          // S
         HID_KEYBOARD_SC_F7,          // D
         HID_KEYBOARD_SC_F8,          // F
         HID_KEYBOARD_SC_SCROLL_LOCK, // G
     },
     {
         0,                     // ZX
         0,                     // XC
         0,                     // CV
         0,                     // VB
         HID_KEYBOARD_SC_F9,    // Z
         HID_KEYBOARD_SC_F10,   // X
         HID_KEYBOARD_SC_F11,   // C
         HID_KEYBOARD_SC_F12,   // V
         HID_KEYBOARD_SC_PAUSE, // B
     },
     {
         HID_KEYBOARD_SC_NUM_LOCK,              // YU
         0,                                     // UI
         0,                                     // IO
         0,                                     // OP
         HID_KEYBOARD_SC_KEYPAD_SLASH,          // Y
         HID_KEYBOARD_SC_KEYPAD_7_AND_HOME,     // U
         HID_KEYBOARD_SC_KEYPAD_8_AND_UP_ARROW, // I
         HID_KEYBOARD_SC_KEYPAD_9_AND_PAGE_UP,  // O
         HID_KEYBOARD_SC_KEYPAD_MINUS,          // P
     },
     {
         0,                                        // HJ
         0,                                        // JK
         0,                                        // KL
         0,                                        // L;
         HID_KEYBOARD_SC_KEYPAD_ASTERISK,          // H
         HID_KEYBOARD_SC_KEYPAD_4_AND_LEFT_ARROW,  // J
         HID_KEYBOARD_SC_KEYPAD_5,                 // K
         HID_KEYBOARD_SC_KEYPAD_6_AND_RIGHT_ARROW, // L
         HID_KEYBOARD_SC_KEYPAD_PLUS,              // ;
     },
     {
         0,                                       // NM
         0,                                       // M,
         0,                                       // ,.
         HID_KEYBOARD_SC_KEYPAD_ENTER,            // ./
         HID_KEYBOARD_SC_KEYPAD_0_AND_INSERT,     // N
         HID_KEYBOARD_SC_KEYPAD_1_AND_END,        // M
         HID_KEYBOARD_SC_KEYPAD_2_AND_DOWN_ARROW, // ,<
         HID_KEYBOARD_SC_KEYPAD_3_AND_PAGE_DOWN,  // .>
         HID_KEYBOARD_SC_KEYPAD_DOT_AND_DELETE,   // /?
     }}};
