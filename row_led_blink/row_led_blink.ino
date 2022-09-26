#include <ezButton.h>

const byte ledPin[] = { 4, 5, 6, 7, 8, 9, 10 };
ezButton button(2);
byte potPin = A1;

byte ledCount = sizeof(ledPin) / sizeof(ledPin[0]);

void setup() {
  Serial.begin(9600);
  Serial.flush();
  for (byte index = 0; index < ledCount; index++) {
    pinMode(ledPin[index], OUTPUT);
  }
  pinMode(potPin, INPUT);
}

byte current_animation = 0;
#define max_animation 2

int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

int speed_animation = 30;
void loop() {
  button.loop();

  if (button.isPressed()) {
    current_animation = (current_animation + 1) % max_animation ;
  }

  speed_animation = map(analogRead(potPin), 0, 1023, 50, 600);
  
  switch (current_animation) {
    case 0:
      animation_table();
      break;
    case 1:
      animation_k2000();
      break;
  }
}

int table_anim_ledTime[] = { -10, 10, -5, 30, -20, 20, 5};

void animation_table() {
  for (byte index = 0; index < ledCount; index++) {
    digitalWrite(ledPin[index], table_anim_ledTime[index] > 0);

    if (table_anim_ledTime[index] > 0) {
      table_anim_ledTime[index] = table_anim_ledTime[index] - 1;
      if (table_anim_ledTime[index] == 0) {
        byte value = random(50);
        table_anim_ledTime[index] = -value;
      }
    } else {
      table_anim_ledTime[index] = table_anim_ledTime[index] + 1;
      if (table_anim_ledTime[index] == 0) {
        byte value = random(50);
        table_anim_ledTime[index] = value;
      }
    }

  }
  delay(speed_animation * 4);
}

byte k2000_anim_direction = 0;
byte k2000_anim_count = 0;
byte k2000_anim_count_max = 5;
byte k2000_anim_number = 0b011;

void animation_k2000() {
  switch (k2000_anim_direction) {
    case 0:
      k2000_anim_number = 0b011;
      k2000_anim_direction = 1;
    case 1:
      k2000_anim_number = k2000_anim_number << 1;
      k2000_anim_count ++;
      if (k2000_anim_count == k2000_anim_count_max) {
        k2000_anim_direction = 2;
      }
      break;
    case 2:
      k2000_anim_number = k2000_anim_number >> 1;
      k2000_anim_count --;
      if (k2000_anim_count == 0) {
        k2000_anim_direction = 1;
      }
      break;
  }
  display_binary(k2000_anim_number);
  delay(speed_animation);
}

void display_table(byte row[]) {
  for (byte index = 0; index < ledCount; index++) {
    digitalWrite(ledPin[index], row[index]);
  }
}

void display_binary(byte number_to_display) {
  for (byte index = 0; index < ledCount; index++) {
    digitalWrite(ledPin[index], bitRead(number_to_display, index));
  }
}
