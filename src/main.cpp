#include "Arduino.h" 
#include "music_notes.h" 
#include "MelodyBuzzer.h"


int BUZZER_PIN = 13;
mz::MelodyBuzzer melodyBuzzer(BUZZER_PIN, &tone, &noTone, &millis);

int melody[] = {
   NOTE_D4, NOTE_D4,
   NOTE_G4, NOTE_AS4, NOTE_A4,
   NOTE_G4, NOTE_D5,
   NOTE_C5,
   NOTE_A4,
   NOTE_G4, NOTE_AS4, NOTE_A4,
   NOTE_F4, NOTE_GS4,
   NOTE_D4,
   NOTE_D4,

   NOTE_G4, NOTE_AS4, NOTE_A4,
   NOTE_G4, NOTE_D5,
   NOTE_F5, NOTE_E5,
   NOTE_DS5, NOTE_B4,
   NOTE_DS5, NOTE_D5, NOTE_CS5,
   NOTE_CS4, NOTE_B4,
   NOTE_G4,
   NOTE_AS4,

   NOTE_D5, NOTE_AS4,
   NOTE_D5, NOTE_AS4,
   NOTE_DS5, NOTE_D5,
   NOTE_CS5, NOTE_A4,
   NOTE_AS4, NOTE_D5, NOTE_CS5,
   NOTE_CS4, NOTE_D4,
   NOTE_D5,
   NOTE_AS4, NOTE_AS4,

   NOTE_D5, NOTE_AS4,
   NOTE_D5, NOTE_AS4,
   NOTE_F5, NOTE_E5,
   NOTE_DS5, NOTE_B4,
   NOTE_DS5, NOTE_D5, NOTE_CS5,
   NOTE_CS4, NOTE_AS4,
   NOTE_G4
};

int melodySize = sizeof(melody) / sizeof(melody[0]);

int durations[] = {
   2, 4,
   4, 8, 4,
   2, 4,
   2,
   2,
   4, 8, 4,
   2, 4,
   1,
   4,

   4, 8, 4,
   2, 4,
   2, 4,
   2, 4,
   4, 8, 4,
   2, 4,
   1,
   4,

   2, 4,
   2, 4,
   2, 4,
   2, 4,
   4, 8, 4,
   2, 4,
   1,
   4, 4,

   2, 4,
   2, 4,
   2, 4,
   2, 4,
   4, 8, 4,
   2, 4,
   1
};

int volume = 255; // Volume level (0-255)

void setup() {
   Serial.begin(9600);
   while (!Serial) {};
   pinMode(BUZZER_PIN, OUTPUT); // Set buzzer pin as output
}

int melodyPtr = 0;

int calculateDurationMilliseconds(int durationVal) {
   return 1000 / durations[melodyPtr];
}

void onTuneEnd(unsigned int frequency, unsigned long startTime, unsigned long endTime, unsigned long currentTime) {
   Serial.print("Frequency: "); Serial.print(frequency);
   Serial.print(" ; Start Time: "); Serial.print(startTime);
   Serial.print(" ; End Time: "); Serial.print(endTime);
   Serial.print(" ; Current Time: "); Serial.print(currentTime);
   Serial.println();

   int duration = calculateDurationMilliseconds(durations[melodyPtr]);
   int pause = duration * 0.25; // Adding a small delay between each note
   delay(pause);

   ++melodyPtr;
}

void playNoteWithVolume(int frequency, int duration, int volume) {
   int dutyCycle = map(volume, 0, 255, 0, 128); // Map volume to the duty cycle range

   for (long i = 0; i < duration * 1000L; i += frequency * 2) {
       digitalWrite(BUZZER_PIN, HIGH);
       delayMicroseconds(dutyCycle);
       digitalWrite(BUZZER_PIN, LOW);
       delayMicroseconds(dutyCycle);
   }
}

void loop() {
   bool songPlaying = melodyPtr < melodySize;
   unsigned int delayTime = songPlaying ? 50 : 5000;

   if (songPlaying) {
       playNoteWithVolume(melody[melodyPtr], calculateDurationMilliseconds(durations[melodyPtr]), volume);
       onTuneEnd(melody[melodyPtr], millis(), millis(), millis());
   }

   melodyBuzzer.update();

   delay(delayTime);
}
