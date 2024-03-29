/*
   For infos how this works see 
   http://fabiensanglard.net/gebbwolf3d_v2.1.pdf chapter 4.9.5
   and
   http://www.shikadi.net/moddingwiki/Inverse_Frequency_Sound_format
   and
   http://www.shikadi.net/moddingwiki/AudioT_Format
*/

//constexpr uint8_t MUS_S*_SND_LEN = 30;
//constexpr uint8_t mus_s*_snd[] PROGMEM = { };

//constexpr uint8_t MUS_P*_SND_LEN = 15;
//constexpr uint8_t mus_p*_snd[] PROGMEM = { };

#ifndef _sound_h
#define _sound_h

#include <avr/pgmspace.h>
#include "constants.h"

constexpr uint8_t MUS_S1_SND_LEN = 38;
constexpr uint8_t mus_s1_snd[] PROGMEM = {0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x00, 0x00, 0x00, 0x00, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x00, 0x00, 0x00, 0x00,};


constexpr uint8_t MUS_P1_SND_LEN = 19;
constexpr uint8_t mus_p1_snd[] PROGMEM = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0x00, };

//S1

constexpr uint8_t MUS_P2_SND_LEN = 19;
constexpr uint8_t mus_p2_snd[] PROGMEM = {0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x00, 0x00, 0x00, 0x00, };

//S1

constexpr uint8_t MUS_P3_SND_LEN = 19;
constexpr uint8_t mus_p3_snd[] PROGMEM = {0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x43, 0x00, 0x00, 0x00, 0x00, };

//S1

constexpr uint8_t MUS_P4_SND_LEN = 19;
constexpr uint8_t mus_p4_snd[] PROGMEM = {0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x00, 0x00, 0x00, 0x00, };

//S1

constexpr uint8_t MUS_P5_SND_LEN = 19;
constexpr uint8_t mus_p5_snd[] PROGMEM = {0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x00, 0x00, 0x00, 0x00, };

constexpr uint8_t MUS_P6_SND_LEN = 19;
constexpr uint8_t mus_p6_snd[] PROGMEM = {0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x00, 0x00, 0x00, 0x00, };


//S1 

//P1

//S1
 
//P2

//S1

//P3

//S1

constexpr uint8_t MUS_P7_SND_LEN = 19;
constexpr uint8_t mus_p7_snd[] PROGMEM = {0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, };

//P4




constexpr uint8_t MUS_S21_SND_LEN = 38;
constexpr uint8_t mus_s21_snd[] PROGMEM = {0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x00, 0x00, 0x00, 0x00, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x89, 0x00, 0x00, 0x00, 0x00, };

constexpr uint8_t MUS_P21_SND_LEN = 19;
constexpr uint8_t mus_p21_snd[] PROGMEM = {0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x00, 0x00, 0x00, 0x00, };

//S1

constexpr uint8_t MUS_P22_SND_LEN = 19;
constexpr uint8_t mus_p22_snd[] PROGMEM = {0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x23, 0x00, 0x00, 0x00, 0x00, };
//S1

constexpr uint8_t MUS_P24_SND_LEN = 19;
constexpr uint8_t mus_p24_snd[] PROGMEM = {0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, };

//S1


constexpr uint8_t MUS_P26_SND_LEN = 19;
constexpr uint8_t mus_p26_snd[] PROGMEM = {0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x00, 0x00, 0x00, 0x00, };


constexpr uint8_t MUS_P27_SND_LEN = 38;
constexpr uint8_t mus_p27_snd[] PROGMEM = {0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, };

constexpr uint8_t MUS_P28_SND_LEN = 38;
constexpr uint8_t mus_p28_snd[] PROGMEM = {0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, };



constexpr uint8_t MUS_P29_SND_LEN = 38;
constexpr uint8_t mus_p29_snd[] PROGMEM = {0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, };


constexpr uint8_t MUS_EP_SND_LEN = 38;
constexpr uint8_t mus_ep_snd[] PROGMEM = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };

constexpr uint8_t MUS_EP2_SND_LEN = 19;
constexpr uint8_t mus_ep2_snd[] PROGMEM = {0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, };


constexpr uint8_t JUMP_SND_LEN = 18;
constexpr uint8_t jump_snd[] PROGMEM = {0x80, 0x80, 0x80, 0x80, 0x80, 0x55, 0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x25, 0x25, 0x25, 0x25, 0x25, };

constexpr uint8_t S_SND_LEN = 38;
constexpr uint8_t s_snd[] PROGMEM = {0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, };


constexpr uint8_t R1_SND_LEN = 9;
constexpr uint8_t r1_snd[] PROGMEM = { 0x95, 0x95, 0x95, 0x95, 0x95, 0x95};
constexpr uint8_t R2_SND_LEN = 9;
constexpr uint8_t r2_snd[] PROGMEM = { 0x50, 0x50, 0x50, 0x50, 0x50, 0x50};


constexpr uint8_t SHOT_SND_LEN = 44;
constexpr uint8_t shot_snd[] PROGMEM = { 0x10, 0x10 , 0x10 , 0x6e , 0x2a , 0x20 , 0x28 , 0x28 , 0x9b , 0x28 , 0x20 , 0x20 , 0x21 , 0x57 , 0x20 , 0x20 , 0x20 , 0x67 , 0x20 , 0x20 , 0x29 , 0x20 , 0x73 , 0x20 , 0x20 , 0x20 , 0x89,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };


constexpr uint8_t GET_KEY_SND_LEN = 90;
constexpr uint8_t SHOOT_SND_LEN = 27;
constexpr uint8_t shoot_snd[] PROGMEM = { 0x10, 0x10 , 0x10 , 0x6e , 0x2a , 0x20 , 0x28 , 0x28 , 0x9b , 0x28 , 0x20 , 0x20 , 0x21 , 0x57 , 0x20 , 0x20 , 0x20 , 0x67 , 0x20 , 0x20 , 0x29 , 0x20 , 0x73 , 0x20 , 0x20 , 0x20 , 0x89,};
constexpr uint8_t get_key_snd[] PROGMEM = {0x24, 0x24, 0x24 , 0x24 , 0x24 , 0x24 , 0x24 , 0x24 , 0x24 , 0x24 , 0x24 , 0x24 , 0x24 , 0x24 , 0x24 , 0x24 , 0x24 , 0x24 , 0x24 , 0x24 , 0x24 , 0x24 , 0x24 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x20 , 0x20 , 0x20 , 0x20 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x20 , 0x20 , 0x20 , 0x20 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x37 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x19 , 0x19 , 0x19 , 0x19 , 0x19 , 0x19 , 0x19 , 0x19 , 0x19 , 0x19 , 0x19 , 0x19 , 0x19 , 0x19 , 0x19 , 0x19 };
constexpr uint8_t HIT_WALL_SND_LEN = 8;
constexpr uint8_t hit_wall_snd[] PROGMEM = { 0x83 , 0x83 , 0x82 , 0x8e , 0x8a , 0x89 , 0x86 , 0x84};
constexpr uint8_t WALK1_SND_LEN = 3;
constexpr uint8_t walk1_snd[] PROGMEM = { 0x8f, 0x8e, 0x8e};
constexpr uint8_t WALK2_SND_LEN = 3;
constexpr uint8_t walk2_snd[] PROGMEM = { 0x84, 0x87, 0x84};
constexpr uint8_t MEDKIT_SND_LEN = 69;
constexpr uint8_t medkit_snd[] PROGMEM = {0x55 , 0x20 , 0x3a , 0x3a , 0x3a , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x33 , 0x33 , 0x33 , 0x33 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x26 , 0x26 , 0x26 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x16 , 0x16 , 0x16 , 0x16 , 0x16 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x16 , 0x16 , 0x16 , 0x16 , 0x16 , 0x16 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x20 , 0x15 , 0x15 , 0x15 , 0x15 , 0x15 , 0x15, 0x15};
constexpr uint8_t MELEE_SND_LEN = 9;
constexpr uint8_t melee_snd[] PROGMEM = {0x8f, 0x8e, 0x8e};


uint8_t idx = 0;
bool sound = false;
uint16_t snd_ptr = 0;
uint8_t snd_len = 0;
uint8_t music = 0;

void sound_init() {
  
  pinMode(SOUND_PIN, OUTPUT);

  TCCR2A = (1 << WGM21); // CTC
  TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20); // prescaler 1024
  OCR2A = 112 - 1; // 16000000 / 1024 / 112 -> 139,5 Hz
  TIMSK2 = (1 << OCIE2A);
}

void playSound(const uint8_t* snd, uint8_t len) {
  snd_ptr = reinterpret_cast<uint16_t>(snd);
  snd_len = len;
  sound = true;
}

// Set the frequency that we will get on pin OCR1A
void setFrequency(uint16_t freq) {
  uint32_t requiredDivisor = (F_CPU / 2) / (uint32_t)freq;

  uint16_t prescalerVal;
  uint8_t prescalerBits;
  if (requiredDivisor < 65536UL) {
    prescalerVal = 1;
    prescalerBits = 1; // prescaler 1
  } else if (requiredDivisor < 8 * 65536UL) {
    prescalerVal = 8;
    prescalerBits = 2; // prescaler 8
  } else if (requiredDivisor < 64 * 65536UL) {
    prescalerVal = 64;
    prescalerBits = 3; // prescaler 64
  } else if (requiredDivisor < 256 * 65536UL) {
    prescalerVal = 256;
    prescalerBits = 4; // prescaler 256
  } else {
    prescalerVal = 1024;
    prescalerBits = 5; // prescaler 1024
  }
  
  uint16_t top = ((requiredDivisor + (prescalerVal / 2)) / prescalerVal) - 1;
  TCCR1A = _BV(COM1A0) /*+ _BV(COM1B0)*/;
  if (freq == 0){TCCR1A = 0;}
  TCCR1B = (1 << WGM12) | prescalerBits;  // CTC
  TCCR1C = _BV(FOC1A);
  OCR1A = top;
}

void off() {
  TCCR1A = 0;
}

ISR(TIMER2_COMPA_vect) {
  if (sound) {
    if (idx++ < snd_len) {
      uint16_t freq = 1192030 / (60 * (uint16_t) pgm_read_byte(snd_ptr + idx)); // 1193181 
      if(freq > 65534){freq = 0;}
      setFrequency(freq);
      

    } else {
      idx = 0;
      off();
      sound = false;
      if (snd_len == 38 || snd_len == 19) {
        music++;
      }
      if (snd_len == 44) {
        playSound(mus_s1_snd, MUS_S1_SND_LEN);
      }
      if (music == 3  || music == 5 || music == 7 || music == 9 || music == 12 || music == 14 || music == 16 || music == 18 || music == 25  || music == 27 || music == 29 || music == 31 || music == 34 || music == 36 || music == 38 || music == 40) {
        playSound(mus_s1_snd, MUS_S1_SND_LEN);
      }
      else if (music == 2 || music == 13 || music == 24 || music == 35) {
        playSound(mus_p1_snd, MUS_P1_SND_LEN);
      }
      else if (music == 4 || music == 15 || music == 26 || music == 37) {
        playSound(mus_p2_snd, MUS_P2_SND_LEN);
      }
      else if (music == 6 || music == 17 || music == 28 || music == 39) {
        playSound(mus_p3_snd, MUS_P3_SND_LEN);
      }
      else if (music == 8 || music == 30) {
        playSound(mus_p4_snd, MUS_P4_SND_LEN);
      }
      else if (music == 10 || music == 32) {
        playSound(mus_p5_snd, MUS_P5_SND_LEN);
      }
      else if (music == 11 || music == 33) {
        playSound(mus_p6_snd, MUS_P6_SND_LEN);
      }
      else if (music == 19 || music == 20 || music == 21 || music == 41 || music == 42 || music == 43) {
        playSound(mus_p7_snd, MUS_P7_SND_LEN);
      }
      else if (music == 22 || music == 44) {
        playSound(mus_ep_snd, MUS_EP_SND_LEN);
      }
      else if (music == 23 || music == 45) {
        playSound(mus_s1_snd, MUS_S1_SND_LEN);
      }


      else if (music == 47  || music == 49 || music == 51 || music == 53 || music == 56 || music == 58 || music == 60 || music == 62) {
        playSound(mus_s1_snd, MUS_S1_SND_LEN);
      }
      else if (music == 46 || music == 57) {
        playSound(mus_p1_snd, MUS_P1_SND_LEN);
      }
      else if (music == 48 || music == 59) {
        playSound(mus_p2_snd, MUS_P2_SND_LEN);
      }
      else if (music == 50 || music == 61) {
        playSound(mus_p3_snd, MUS_P3_SND_LEN);
      }
      else if (music == 52) {
        playSound(mus_p4_snd, MUS_P4_SND_LEN);
      }
      else if (music == 54) {
        playSound(mus_p5_snd, MUS_P5_SND_LEN);
      }
      else if (music == 55) {
        playSound(mus_p6_snd, MUS_P6_SND_LEN);
      }
      else if (music == 63 || music == 64 || music == 65) {
        playSound(mus_p7_snd, MUS_P7_SND_LEN);
      }
      else if (music == 66) {
        playSound(mus_ep_snd, MUS_EP_SND_LEN);
      }





      else if (music == 68  || music == 70 || music == 72 || music == 74 || music == 77 || music == 79 || music == 81 ) {
        playSound(mus_s21_snd, MUS_S21_SND_LEN);
      }
      else if (music == 67 || music == 78) {
        playSound(mus_p21_snd, MUS_P21_SND_LEN);
      }
      else if (music == 69 || music == 80) {
        playSound(mus_p22_snd, MUS_P22_SND_LEN);
      }
      else if (music == 71 ) {
        playSound(mus_p2_snd, MUS_P2_SND_LEN);
      }
      else if (music == 73) {
        playSound(mus_p24_snd, MUS_P24_SND_LEN);
      }
      else if (music == 75) {
        playSound(mus_p2_snd, MUS_P2_SND_LEN);
      }
      else if (music == 76) {
        playSound(mus_p26_snd, MUS_P26_SND_LEN);
      }
      else if (music == 82 ) {
        playSound(mus_p27_snd, MUS_P27_SND_LEN);
      }
      else if (music == 83 ) {
        playSound(mus_p28_snd, MUS_P28_SND_LEN);
      }
      else if (music == 84) {
        playSound(mus_p29_snd, MUS_P29_SND_LEN);
      }
      else if (music == 85 ) {
        playSound(mus_ep2_snd, MUS_EP2_SND_LEN);
      }
      else if (music == 86 || music == 87 || music == 88) {
        playSound(mus_ep_snd, MUS_EP_SND_LEN);
      }
      else if (music == 89) {
        playSound(mus_s1_snd, MUS_S1_SND_LEN);
        music = 2;
      }
      
      
    }
  }
}

#endif