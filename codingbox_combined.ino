#include <ks_Matrix.h>

// Buzzer definitions and song
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_G4 392
#define NOTE_D4 294
#define NOTE_C4 262
#define REST 0

Matrix myMatrix(A4, A5);

int K1 = 12;
int K2 = 13;
int fanPin = 10;
int buzzer = 4;
int tempo = 114;

// Melody (simplified for demo purposes)
int melody[] = {
  NOTE_E4, 4, NOTE_E4, 4, NOTE_F4, 4, NOTE_G4, 4,
  NOTE_G4, 4, NOTE_F4, 4, NOTE_E4, 4, NOTE_D4, 4,
  NOTE_C4, 4, NOTE_C4, 4, NOTE_D4, 4, NOTE_E4, 4,
  NOTE_D4, -4, NOTE_C4, 8, NOTE_C4, 2
};

int notes = sizeof(melody) / sizeof(melody[0]) / 2;
int wholenote = (60000 * 4) / tempo;

void playMelody() {
  for (int thisNote = 0; thisNote < notes * 2; thisNote += 2) {
    int divider = melody[thisNote + 1];
    int noteDuration = 0;

    if (divider > 0) {
      noteDuration = wholenote / divider;
    } else if (divider < 0) {
      noteDuration = wholenote / abs(divider);
      noteDuration *= 1.5;
    }

    tone(buzzer, melody[thisNote], noteDuration * 0.9);
    delay(noteDuration);
    noTone(buzzer);
  }
}

void setup() {
  myMatrix.begin(112);
  myMatrix.clear();

  pinMode(K1, INPUT);
  pinMode(K2, INPUT);
  pinMode(fanPin, OUTPUT);
  pinMode(buzzer, OUTPUT);

  digitalWrite(fanPin, LOW);  // fan off
  noTone(buzzer);             // no sound

  myMatrix.drawCircle(3, 3, 2, 1);
  myMatrix.writeDisplay();
}

void loop() {
  int K1_level = digitalRead(K1);
  int K2_level = digitalRead(K2);

  if (K1_level == 0) {
    myMatrix.setTextSize(1);
    myMatrix.setTextWrap(false);
    myMatrix.setTextColor(1);
    myMatrix.setRotation(0);
    myMatrix.clear();
    myMatrix.setCursor(2, 0);
    myMatrix.print("L");
    myMatrix.writeDisplay();

    analogWrite(fanPin, 150); // Turn on fan
  }

  if (K2_level == 0) {
    myMatrix.setTextSize(1);
    myMatrix.setTextWrap(false);
    myMatrix.setTextColor(1);
    myMatrix.setRotation(0);
    myMatrix.clear();
    myMatrix.setCursor(2, 0);
    myMatrix.print("R");
    myMatrix.writeDisplay();

    digitalWrite(fanPin, LOW); // Turn off fan
    playMelody();              // Play music
  }
}
