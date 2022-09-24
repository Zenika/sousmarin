const byte ledPin[] = { 2, 3, 4, 5, 6, 7, 8 };
byte ledCount = sizeof(ledPin) / sizeof(ledPin[0]);

void setup() {
  Serial.begin(9600);
  Serial.flush();
  for (byte index = 0; index < ledCount; index++) {
    pinMode(ledPin[index], OUTPUT);
  }
}

byte number = 0b011;
byte window_size = 1;
void loop() {
  //byte row[] = { 1, 0, 1 };
  //display(row);

  display_binary(number);
  animation_k2000();
  delay(100);
}

byte anim_direction = 1;
byte anim_count = 0;
byte anim_count_max = 5;

void animation_k2000() {
  if (anim_direction == 2) {
    number = number >> 1;
    anim_count --;
    if (anim_count == 0) {
      anim_direction = 1;
    }
  } else {
    number = number << 1;
    anim_count ++;
    if (anim_count == anim_count_max) {
      anim_direction = 2;
    }
  }
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
