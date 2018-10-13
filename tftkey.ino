// Paint example specifically for the TFTLCD breakout board.
// If using the Arduino shield, use the tftpaint_shield.pde sketch instead!
// DOES NOT CURRENTLY WORK ON ARDUINO LEONARDO

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>
#include <Keyboard.h>

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7

// For the Arduino Mega, use digital pins 22 through 29

// (on the 2-row header at the end of the board).
//   D0 connects to digital pin 22
//   D1 connects to digital pin 23
//   D2 connects to digital pin 24
//   D3 connects to digital pin 25
//   D4 connects to digital pin 26
//   D5 connects to digital pin 27
//   D6 connects to digital pin 28
//   D7 connects to digital pin 29

// For the Arduino Due, use digital pins 33 through 40
// (on the 2-row header at the end of the board).
//   D0 connects to digital pin 33
//   D1 connects to digital pin 34
//   D2 connects to digital pin 35
//   D3 connects to digital pin 36
//   D4 connects to digital pin 37
//   D5 connects to digital pin 38
//   D6 connects to digital pin 39
//   D7 connects to digital pin 40

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GRAY    ((0x18 << 11) | (0x30 << 5) | 0x18)

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
#define BOXSIZE 44
#define BOXSPACE 5
#define PENRADIUS 3
#define Y_OFFSET 100

#define HYPHEN 0
#define LPAREN 1
#define RPAREN 2
#define STARTMARK 3
#define QUESTION 4
#define EXCLAIMATION 5
#define COMMA 6
#define PERIOD 7
#define LASTSYMBOL PERIOD
#define KANA(x) (x + LASTSYMBOL)

const PROGMEM uint8_t fonts[] = {
 // -
0x00, 0x00, 0x80, 0x7e, 0x00, 0x00, 0x00, 0x00,
 // (
0x02, 0x04, 0x08, 0x08, 0x08, 0x04, 0x02, 0x00,
 // )
0x80, 0x40, 0x20, 0x20, 0x20, 0x40, 0x80, 0x00,
// >
0x00, 0xc0, 0x38, 0x06, 0x38, 0xc0, 0x00, 0x00,
// ?
0x3c, 0x42, 0x02, 0x0c, 0x10, 0x00, 0x10, 0x00,
// !
0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x10, 0x00,
// ,
0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x00,
/// .
0x00, 0x00, 0x00, 0x00, 0x40, 0xa0, 0x40, 0x00,


 // ぁ
0x00, 0x10, 0x3c, 0x10, 0x3c, 0x5a, 0x32, 0x00,
 // あ
0x20, 0x7c, 0x20, 0x3c, 0x6a, 0xb2, 0x64, 0x00,
 // ぃ
0x00, 0x00, 0x48, 0x44, 0x44, 0x50, 0x20, 0x00,
 // い
0x00, 0x88, 0x84, 0x82, 0x82, 0x50, 0x20, 0x00,
 // ぅ
0x00, 0x38, 0x00, 0x38, 0x44, 0x08, 0x30, 0x00,
 // う
0x3c, 0x00, 0x3c, 0x42, 0x02, 0x04, 0x38, 0x00,
 // ぇ
0x00, 0x38, 0x00, 0x78, 0x10, 0x30, 0x4c, 0x00,
 // え
0x3c, 0x00, 0x7c, 0x08, 0x18, 0x28, 0x46, 0x00,
 // ぉ
0x00, 0x20, 0x74, 0x20, 0x38, 0x64, 0x28, 0x00,
 // お
0x20, 0xf4, 0x22, 0x3c, 0x62, 0xa2, 0x6c, 0x00,
 // か
0x20, 0x20, 0xf4, 0x2a, 0x4a, 0x48, 0xb0, 0x00,
 // が
0x2a, 0x20, 0xf4, 0x2a, 0x4a, 0x48, 0xb0, 0x00,
 // き
0x10, 0x7c, 0x08, 0x7e, 0x24, 0x40, 0x3c, 0x00,
 // ぎ
0x14, 0x7c, 0x08, 0x7e, 0x24, 0x40, 0x3c, 0x00,
 // く
0x04, 0x08, 0x30, 0x40, 0x30, 0x08, 0x04, 0x00,
 // ぐ
0x04, 0x08, 0x36, 0x40, 0x30, 0x08, 0x04, 0x00,
 // け
0x88, 0x88, 0xbe, 0x88, 0x88, 0x88, 0x10, 0x00,
 // げ
0x8a, 0x88, 0xbe, 0x88, 0x88, 0x88, 0x10, 0x00,
 // こ
0x00, 0x3c, 0x00, 0x00, 0x20, 0x40, 0x3e, 0x00,
 // ご
0x0a, 0x3c, 0x00, 0x00, 0x20, 0x40, 0x3e, 0x00,
 // さ
0x08, 0x08, 0x7e, 0x04, 0x24, 0x40, 0x3c, 0x00,
 // ざ
0x0a, 0x08, 0x7e, 0x04, 0x24, 0x40, 0x3c, 0x00,
 // し
0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x1c, 0x00,
 // じ
0x2a, 0x20, 0x20, 0x20, 0x20, 0x22, 0x1c, 0x00,
 // す
0x08, 0xfe, 0x18, 0x28, 0x18, 0x08, 0x10, 0x00,
 // ず
0x0a, 0xfe, 0x18, 0x28, 0x18, 0x08, 0x10, 0x00,
 // せ
0x24, 0x24, 0xfe, 0x24, 0x2c, 0x20, 0x1e, 0x00,
 // ぜ
0x26, 0x24, 0xfe, 0x24, 0x2c, 0x20, 0x1e, 0x00,
 // そ
0x3c, 0x08, 0x10, 0x7e, 0x10, 0x10, 0x0c, 0x00,
 // ぞ
0x3c, 0x0a, 0x10, 0x7e, 0x10, 0x10, 0x0c, 0x00,
 // た
0x20, 0xf0, 0x2e, 0x40, 0x48, 0x50, 0x8e, 0x00,
 // だ
0x2a, 0xf0, 0x2e, 0x40, 0x48, 0x50, 0x8e, 0x00,
 // ち
0x08, 0x7e, 0x10, 0x1c, 0x22, 0x02, 0x1c, 0x00,
 // ぢ
0x0a, 0x7e, 0x10, 0x1c, 0x22, 0x02, 0x1c, 0x00,
 // っ
0x00, 0x00, 0x00, 0x18, 0x64, 0x04, 0x18, 0x00,
 // つ
0x00, 0x3c, 0xc2, 0x02, 0x02, 0x1c, 0x00, 0x00,
 // づ
0x0a, 0x3c, 0xc2, 0x02, 0x02, 0x1c, 0x00, 0x00,
 // て
0x0e, 0x74, 0x08, 0x10, 0x10, 0x08, 0x06, 0x00,
 // で
0x0e, 0x74, 0x0a, 0x10, 0x10, 0x08, 0x06, 0x00,
 // と
0x20, 0x20, 0x26, 0x18, 0x20, 0x40, 0x3e, 0x00,
 // ど
0x2a, 0x20, 0x26, 0x18, 0x20, 0x40, 0x3e, 0x00,
 // な
0x20, 0xf4, 0x22, 0x44, 0x9c, 0x26, 0x18, 0x00,
 // に
0x80, 0x9c, 0x80, 0x80, 0x90, 0xa0, 0x9e, 0x00,
 // ぬ
0x08, 0x48, 0x5c, 0x6a, 0xb2, 0xa6, 0x56, 0x00,
 // ね
0x20, 0x2c, 0xf2, 0x22, 0x66, 0xaa, 0x24, 0x00,
 // の
0x00, 0x38, 0x54, 0x92, 0xa2, 0x44, 0x18, 0x00,
 // は
0x84, 0x84, 0xbe, 0x84, 0x9c, 0xa4, 0x9a, 0x00,
 // ば
0x86, 0x84, 0xbe, 0x84, 0x9c, 0xa4, 0x9a, 0x00,
 // ぱ
0x84, 0x8a, 0xbe, 0x84, 0x9c, 0xa4, 0x9a, 0x00,
 // ひ
0x28, 0xe4, 0x26, 0x44, 0x44, 0x44, 0x38, 0x00,
 // び
0x2a, 0xe4, 0x26, 0x44, 0x44, 0x44, 0x38, 0x00,
 // ぴ
0x2c, 0xea, 0x26, 0x44, 0x44, 0x44, 0x38, 0x00,
 // ふ
0x10, 0x08, 0x10, 0x10, 0x4c, 0x4a, 0xb2, 0x00,
 // ぶ
0x16, 0x08, 0x10, 0x10, 0x4c, 0x4a, 0xb2, 0x00,
 // ぷ
0x14, 0x0a, 0x14, 0x10, 0x4c, 0x4a, 0xb2, 0x00,
 // へ
0x00, 0x20, 0x50, 0x88, 0x06, 0x00, 0x00, 0x00,
 // べ
0x0a, 0x20, 0x50, 0x88, 0x06, 0x00, 0x00, 0x00,
 // ぺ
0x04, 0x2a, 0x54, 0x88, 0x06, 0x00, 0x00, 0x00,
 // ほ
0x80, 0xbe, 0x88, 0xbe, 0x88, 0xbc, 0xba, 0x00,
 // ぼ
0x8a, 0xbe, 0x88, 0xbe, 0x88, 0xbc, 0xba, 0x00,
 // ぽ
0x84, 0xba, 0x8c, 0xbe, 0x88, 0xbc, 0xba, 0x00,
 // ま
0x08, 0x7e, 0x08, 0x7e, 0x08, 0x7c, 0x7a, 0x00,
 // み
0x70, 0x10, 0x24, 0x7c, 0xa6, 0xc4, 0x18, 0x00,
 // む
0x20, 0xf4, 0x22, 0x60, 0xa0, 0x62, 0x3c, 0x00,
 // め
0x08, 0x48, 0x5c, 0x6a, 0xb2, 0xa2, 0x54, 0x00,
 // も
0x10, 0x7c, 0x20, 0x7c, 0x22, 0x22, 0x1c, 0x00,
 // ゃ
0x00, 0x28, 0x2c, 0x72, 0x14, 0x10, 0x08, 0x00,
 // や
0x48, 0x5c, 0xe2, 0x24, 0x20, 0x10, 0x10, 0x00,
 // ゅ
0x00, 0x10, 0x58, 0x74, 0x54, 0x18, 0x20, 0x00,
 // ゆ
0x10, 0xbc, 0xd2, 0x92, 0xbc, 0x10, 0x20, 0x00,
 // ょ
0x00, 0x08, 0x08, 0x0c, 0x38, 0x48, 0x34, 0x00,
 // よ
0x08, 0x08, 0x0e, 0x08, 0x38, 0x4c, 0x32, 0x00,
 // ら
0x30, 0x08, 0x40, 0x5c, 0x62, 0x02, 0x3c, 0x00,
 // り
0x58, 0x64, 0x44, 0x44, 0x04, 0x08, 0x30, 0x00,
 // る
0x3c, 0x08, 0x10, 0x3c, 0x42, 0x32, 0x3c, 0x00,
 // れ
0x20, 0x2c, 0xf4, 0x24, 0x64, 0xa4, 0x22, 0x00,
 // ろ
0x3c, 0x08, 0x10, 0x3c, 0x42, 0x02, 0x3c, 0x00,
 // ゎ
0x00, 0x10, 0x14, 0x7a, 0x32, 0x52, 0x14, 0x00,
 // わ
0x20, 0x2c, 0xf2, 0x22, 0x62, 0xa2, 0x2c, 0x00,
 // ゐ
0x70, 0x10, 0x3c, 0x52, 0x96, 0xaa, 0x44, 0x00,
 // ゑ
0x38, 0x10, 0x38, 0x44, 0x18, 0x6c, 0x92, 0x00,
 // を
0x10, 0x7c, 0x20, 0x76, 0x98, 0x28, 0x1e, 0x00,
 // ん
0x10, 0x10, 0x20, 0x20, 0x50, 0x52, 0x8c, 0x00,
};

#define ID 0

const PROGMEM int8_t mutateTable[] = {
ID, // -
ID,  // (
ID,  // )
ID, // >
ID, // ?
ID, // !
ID, // ,
ID, /// .

+1, // ぁ
-1, // あ
+1, // ぃ
-1, // い
+1, // ぅ
-1, // う
+1, // ぇ
-1, // え
+1, // ぉ
-1, // お
+1, // か
-1, // が
+1, // き
-1, // ぎ
+1, // く
-1, // ぐ
+1, // け
-1, // げ
+1, // こ
-1, // ご
+1, // さ
-1, // ざ
+1, // し
-1, // じ
+1, // す
-1, // ず
+1, // せ
-1, // ぜ
+1, // そ
-1, // ぞ
+1, // た
-1, // だ
+1, // ち
-1, // ぢ
+2, // っ
-1, // つ
-1, // づ
+1, // て
-1, // で
+1, // と
-1, // ど
ID, // な
ID, // に
ID, // ぬ
ID, // ね
ID, // の
+1, // は
+1, // ば
-2, // ぱ
+1, // ひ
+1, // び
-2, // ぴ
+1, // ふ
+1, // ぶ
-2, // ぷ
+1, // へ
+1, // べ
-2, // ぺ
+1, // ほ
+1, // ぼ
-2, // ぽ
ID, // ま
ID, // み
ID, // む
ID, // め
ID, // も
+1, // ゃ
-1, // や
+1, // ゅ
-1, // ゆ
+1, // ょ
-1, // よ
ID, // ら
ID, // り
ID, // る
ID, // れ
ID, // ろ
+1, // ゎ
-1, // わ
ID, // ゐ
ID, // ゑ
ID, // を
ID, // ん
};

const PROGMEM int8_t romajiKeySequenceTable[] = {
'-', 0, 0, 0, // -
'(', 0, 0, 0,  // (
')', 0, 0, 0,  // )
'>', 0, 0, 0, // >
'?', 0, 0, 0, // ?
'!', 0, 0, 0, // !
',', 0, 0, 0, // ,
'.', 0, 0, 0, /// .

'x', 'x', 'a', 0, // ぁ
'a', 0, 0, 0, // あ
'x', 'x', 'i', 0, // ぃ
'i', 0, 0, 0, // い
'x', 'x', 'u', 0, // ぅ
'u', 0, 0, 0, // う
'x', 'x', 'e', 0, // ぇ
'e', 0, 0, 0, // え
'x', 'x', 'o', 0, // ぉ
'o', 0, 0, 0, // お
'k', 'a', 0, 0, // か
'g', 'a', 0, 0, // が
'k', 'i', 0, 0, // き
'g', 'i', 0, 0, // ぎ
'k', 'u', 0, 0, // く
'g', 'u', 0, 0, // ぐ
'k', 'e', 0, 0, // け
'g', 'e', 0, 0, // げ
'k', 'o', 0, 0, // こ
'g', 'o', 0, 0, // ご
's', 'a', 0, 0, // さ
'z', 'a', 0, 0, // ざ
's', 'i', 0, 0, // し
'z', 'i', 0, 0, // じ
's', 'u', 0, 0, // す
'z', 'u', 0, 0, // ず
's', 'e', 0, 0, // せ
'z', 'e', 0, 0, // ぜ
's', 'o', 0, 0, // そ
'z', 'o', 0, 0, // ぞ
't', 'a', 0, 0, // た
'd', 'a', 0, 0, // だ
't', 'i', 0, 0, // ち
'd', 'i', 0, 0, // ぢ
'x', 't', 'u', 0, // っ
't', 'u', 0, 0, // つ
'd', 'u', 0, 0, // づ
't', 'e', 0, 0, // て
'd', 'e', 0, 0, // で
't', 'o', 0, 0, // と
'd', 'o', 0, 0, // ど
'n', 'a', 0, 0, // な
'n', 'i', 0, 0, // に
'n', 'u', 0, 0, // ぬ
'n', 'e', 0, 0, // ね
'n', 'o', 0, 0, // の
'h', 'a', 0, 0, // は
'b', 'a', 0, 0, // ば
'p', 'a', 0, 0, // ぱ
'h', 'i', 0, 0, // ひ
'b', 'i', 0, 0, // び
'p', 'i', 0, 0, // ぴ
'h', 'u', 0, 0, // ふ
'b', 'u', 0, 0, // ぶ
'p', 'u', 0, 0, // ぷ
'h', 'e', 0, 0, // へ
'b', 'e', 0, 0, // べ
'p', 'e', 0, 0, // ぺ
'h', 'o', 0, 0, // ほ
'b', 'o', 0, 0, // ぼ
'p', 'o', 0, 0, // ぽ
'm', 'a', 0, 0, // ま
'm', 'i', 0, 0, // み
'm', 'u', 0, 0, // む
'm', 'e', 0, 0, // め
'm', 'o', 0, 0, // も
'x', 'y', 'a', 0, // ゃ
'y', 'a', 0, 0, // や
'x', 'y', 'u', 0, // ゅ
'y', 'u', 0, 0, // ゆ
'x', 'y', 'o', 0, // ょ
'y', 'o', 0, 0, // よ
'r', 'a', 0, 0, // ら
'r', 'i', 0, 0, // り
'r', 'u', 0, 0, // る
'r', 'e', 0, 0, // れ
'r', 'o', 0, 0, // ろ
'x', 'w', 'a', 0, // ゎ
'w', 'a', 0, 0, // わ
'x', 'w', 'i', 0, // ゐ
'x', 'w', 'e', 0, // ゑ
'w', 'o', 0, 0, // を
'n', 'n', 0, 0, // ん
};

#define MINPRESSURE 10
#define MAXPRESSURE 1000
#define FLICK_THRESHOLD 8

#define VEC_STAND 0
#define VEC_LEFT 1
#define VEC_UP 2
#define VEC_RIGHT 3
#define VEC_DOWN 4

const PROGMEM uint8_t keymap[4 * 5 * 5] = {
  0xff, 0xff, 0xff, 0xff, 0xff ,
  KANA(0x02), KANA(0x04), KANA(0x06), KANA(0x08), KANA(0x0a) , // あいうえお
  KANA(0x0b), KANA(0x0d), KANA(0x0f), KANA(0x11), KANA(0x13) ,
  KANA(0x15), KANA(0x17), KANA(0x19), KANA(0x1b), KANA(0x1d) ,
  0xff, 0xff, 0xff, 0xff, 0xff , // bkps
  0xff, 0xff, 0xff, 0xff, 0xff ,
  KANA(0x1f), KANA(0x21), KANA(0x24), KANA(0x26), KANA(0x28) ,
  KANA(0x2a), KANA(0x2b), KANA(0x2c), KANA(0x2d), KANA(0x2e) ,
  KANA(0x2f), KANA(0x32), KANA(0x35), KANA(0x38), KANA(0x3b) ,
  0xff, 0xff, 0xff, 0xff, 0xff ,
  0xff, 0xff, 0xff, 0xff, 0xff , // white space
  KANA(0x3e), KANA(0x3f), KANA(0x40), KANA(0x41), KANA(0x42) ,
  KANA(0x44), LPAREN,     KANA(0x46), RPAREN,     KANA(0x48) ,
  KANA(0x49), KANA(0x4a), KANA(0x4b), KANA(0x4c), KANA(0x4d) ,
  0xff, 0xff, 0xff, 0xff, 0xff , // flush
  0xff, 0xff, 0xff, 0xff, 0xff ,
  0xff, 0xff, 0xff, 0xff, 0xff , // mutate
  KANA(0x4f), KANA(0x52), KANA(0x53), HYPHEN, 0xff ,
  COMMA, PERIOD, QUESTION, EXCLAIMATION, 0xff ,
  0xff, 0xff, 0xff, 0xff, 0xff , // enter
};

int nx = -1, ny = -1, px = -1, py = -1;
int initX = -1, initY = -1;
int lastX = -1, lastY = -1;
int offCount = 0;
uint8_t inputBuffer = 0xff;

#define FONT(x) ((fonts) + ((x) * 8))

int cursor = 0;

void putLetter(uint8_t fontIdx) {
  tft.drawBitmap(cursor * 8, 0, FONT(fontIdx), 8, 8, WHITE);
  cursor ++;
}

void deleteLetter() {
  cursor --;
  tft.fillRect(cursor * 8, 0, 8, 8, BLACK);
}

void clearLine() {
  tft.fillRect(0, 0, tft.width(), 8, BLACK);
  cursor = 0;
  putLetter(STARTMARK);
}

void flushInputBuffer() {
  if (inputBuffer == 0xff) { return; }
  for (int i = 0; i < 4; ++i) {
    unsigned char ascii = pgm_read_byte(romajiKeySequenceTable + inputBuffer * 4 + i);
    if (ascii == 0) break;
    Keyboard.write(ascii);
  }
  inputBuffer = 0xff;
  deleteLetter();
}

void mutateInputBuffer() {
  if (inputBuffer == 0xff) { return; }
  inputBuffer = inputBuffer + pgm_read_byte(mutateTable + inputBuffer);
}

void processKeyInput(int8_t nx, int8_t ny, int8_t flickVector) {
  if (nx < 0 || ny < 0) { return; } // unsupported
  if (nx == 0) { return; } // not implemented yet
  if (nx == 4) { // funcitons
    switch (ny) {
    case 0: // bksp
      if (inputBuffer == 0xff) {
        Keyboard.write(8); // bksp
      } else {
        inputBuffer = 0xff;
        deleteLetter();
      }
      break;
    case 1: // spc
      flushInputBuffer();
      Keyboard.write(' ');
      break;
    case 2: // flush
      flushInputBuffer();
      break;
    case 3: // enter
      flushInputBuffer();
      Keyboard.write('\n');
      break;
    }
    return;
  }
  if (nx == 1 && ny == 3) {
    deleteLetter();
    mutateInputBuffer();
    putLetter(inputBuffer);
    return;
  }
  uint8_t next = pgm_read_byte(keymap + (ny * 5 + nx) * 5 + flickVector);
  if (next != 0xff) {
    flushInputBuffer();
    inputBuffer = next;
    putLetter(inputBuffer);
  }
}

void setup(void) {
  Serial.begin(9600);
  Serial.println(F("Paint!"));

  tft.reset();

  uint16_t identifier = tft.readID();

  if(identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if(identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } else if(identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } else if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else if(identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
  } else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(F("If using the Adafruit 2.8\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_ADAFRUIT_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Adafruit_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    return;
  }

  tft.begin(identifier);

  tft.fillScreen(BLACK);

  clearLine();

  for (int y = 0; y < 4; ++y) {
    for (int x = 0; x < 5; ++x) {
      colorRect(x, y, false);
    }
  }

  pinMode(13, OUTPUT);

  Keyboard.begin();
}

boolean isValidTile(int nx, int ny) {
  return nx >= 0 && nx < 5 && ny >= 0 && ny < 4;
}

void colorRect(int x, int y, boolean pressed) {
  if (isValidTile(x, y)) {
    tft.fillRect((BOXSIZE + BOXSPACE) * x, Y_OFFSET + (BOXSIZE + BOXSPACE) * y, BOXSIZE, BOXSIZE,
                 pressed ? RED
                 : x >= 1 && x <= 3 ? WHITE
                 : GRAY);
  }
}

int kx = -1, ky = -1;

void loop()
{
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13, LOW);

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);

  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    // scale from 0->1023 to tft.width
    int x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    int y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
    nx = (x - BOXSPACE / 2) / (BOXSIZE + BOXSPACE);
    ny = (y - Y_OFFSET - BOXSPACE / 2) / (BOXSIZE + BOXSPACE);

    if (!(isValidTile(nx, ny))) { return; }

    if (initX < 0 || initY < 0) {
      initX = x;
      initY = y;
    }
    if (kx == -1) {
      kx = nx;
    }
    if (ky == -1) {
      ky = ny;
    }
    lastX = x;
    lastY = y;

    if (px != nx || py != ny) {
      colorRect(px, py, false);
      colorRect(nx, ny, true);
    }
    px = nx;
    py = ny;
    offCount = 0;
  } else if (nx > -1 && ny > -1) {
    offCount ++;
    if (offCount > 50) {
      // off

      int dx = lastX - initX;
      int dy = lastY - initY;
      uint8_t vec = VEC_STAND;

      if (abs(dx) > abs(dy)) {
        if (dx > FLICK_THRESHOLD) {
          vec = VEC_RIGHT;
        } else if (dx < - FLICK_THRESHOLD) {
          vec = VEC_LEFT;
        }
      } else {
        if (dy > FLICK_THRESHOLD) {
          vec = VEC_DOWN;
        } else if (dy < - FLICK_THRESHOLD) {
          vec = VEC_UP;
        }
      }

      processKeyInput(kx, ky, vec);

      if (nx != -1 && ny != -1)
        colorRect(nx, ny, false);
      if (px != -1 && py != -1)
        colorRect(px, py, false);
      nx = -1;
      ny = -1;
      px = -1;
      py = -1;
      initX = -1;
      initY = -1;
      lastX = -1;
      lastY = -1;
      kx = -1;
      ky = -1;
    }
  }
}
