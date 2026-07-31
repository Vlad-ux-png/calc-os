#ifndef SOUND_H
#define SOUND_H
#include <stdint.h>

#define NOTE_E 329
#define NOTE_f 349
#define NOTE_g 392
#define NOTE_a 440
#define NOTE_aS 466
#define NOTE_b 493
#define NOTE_cH 523
#define NOTE_cSH 554
#define NOTE_dH 587
#define NOTE_dSH 622
#define NOTE_eH 659
#define NOTE_fH 698
#define NOTE_fSH 740
#define NOTE_gH 784
#define NOTE_gSH 830
#define NOTE_aH 880

#define ONE_BEAT   250  
#define HALF_BEAT  125

void play_sound(uint32_t frequency);
void stop_sound();
void beep(uint32_t frequency, uint32_t ticks);
void play_note(uint32_t base_frequency, uint32_t duration_ms);

#endif 