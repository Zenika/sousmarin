const byte ledPin[] = { 2, 3, 4, 5, 6, 7, 8 };
byte ledCount = sizeof(ledPin) / sizeof(ledPin[0]);

void setup() {
//  Serial.begin(9600);
//  Serial.flush();
  for (byte index = 0; index < ledCount; index++) {
    pinMode(ledPin[index], OUTPUT);
  }
}

byte number = 0b011;
void loop() {
  //byte row[] = { 1, 0, 1 };
  //display(row);

  display_binary(number);
  //animation_k2000();
  animation_table();

}

int table_anim_ledTime[] = {-10, 10, -5, 30, -20, 20, 5};

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
  delay(200);
}

byte k2000_anim_direction = 0;
byte k2000_anim_count = 0;
byte k2000_anim_count_max = 5;

void animation_k2000() {
  switch (k2000_anim_direction) {
    case 0:
      number = 0b011;
      k2000_anim_direction = 1;
    case 1:
      number = number << 1;
      k2000_anim_count ++;
      if (k2000_anim_count == k2000_anim_count_max) {
        k2000_anim_direction = 2;
      }
      break;
    case 2:
      number = number >> 1;
      k2000_anim_count --;
      if (k2000_anim_count == 0) {
        k2000_anim_direction = 1;
      }
      break;
  }
  delay(100);
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
