#include <ezButton.h>

#define LED_ON 1
#define LED_OFF 0

const byte ledPin[] = { PB12, PB13, PB14, PB15, PA1, PA9, PA10, PA0, PC15, PA15, PB3, PB4, PB5, PB6, PB7, PB10 };
ezButton button(PB9);
byte potPin = A2;

byte led_count = sizeof(ledPin) / sizeof(ledPin[0]);

void setup() {
//  Serial.begin(9600);
//  Serial.flush();
  for (byte index = 0; index < led_count; index++) {
    pinMode(ledPin[index], OUTPUT);
  }
  pinMode(potPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

byte current_animation = 4;
#define max_animation 6
bool toggle_led = true;
int speed_animation = 100; // fallback value

void loop() {
  button.loop();

  if (button.isPressed()) {
    current_animation = (current_animation + 1) % max_animation ;
  }

  speed_animation = map(analogRead(potPin), 0, 1023, 60, 600);

  toggle_led = !toggle_led;
  digitalWrite(LED_BUILTIN, toggle_led);

  switch (current_animation) {
    case 0:
      animation_table();
      break;
    case 1:
      animation_k2000();
      break;
    case 2:
      animation_linear(false);
      break;
    case 3:
      animation_linear(true);
      break;
    case 4:
      animation_table();
      animation_linear(true);
      break;
    case 5:
      animation_single();
      break;
  }
  delay(speed_animation);

}

int single_anim_index = 0;
void animation_single() {

  if (single_anim_index > 0) {
    digitalWrite(ledPin[single_anim_index - 1], LED_OFF);
  }
  if (((single_anim_index - 1) % led_count) < 0) {
    digitalWrite(ledPin[led_count - 1], LED_OFF);
  }
  digitalWrite(ledPin[single_anim_index], LED_ON);

  single_anim_index = (single_anim_index + 1) % led_count;
}

int linear_anim_index = 0;
int linear_anim_dir = 1;
int linear_nb_led = 12;
void animation_linear(bool reverse) {
  if (linear_anim_index >= linear_nb_led && linear_anim_dir == 1) {
    if (reverse) {
      linear_anim_dir = -1;
    } else {
      for (byte index = 0; index < linear_nb_led; index++) {
        digitalWrite(ledPin[index], LED_OFF);
      }
    }
  }
  if (linear_anim_dir == 1) {
    digitalWrite(ledPin[linear_anim_index], LED_ON);
  } else {
    digitalWrite(ledPin[linear_anim_index], LED_OFF);
  }
  linear_anim_index = linear_anim_index + linear_anim_dir;
  if (linear_anim_index == -1) {
    linear_anim_index = 0;
    linear_anim_dir = 1;
  }
}

int table_anim_ledTime[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -10, 10, -5, 30};

int table_nb_led = 4;
int table_led_start = 12;
void animation_table() {
  for (byte index = table_led_start; index < table_led_start + table_nb_led; index++) {
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
}

byte k2000_anim_direction = 0;
int k2000_anim_count = 0;
#define k2000_led_count 13
#define k2000_led_on 1
uint16_t k2000_anim_number = 0b0;

void animation_k2000() {
  switch (k2000_anim_direction) {
    case 0:
      k2000_anim_number = 0b0011;
      k2000_anim_direction = 1;
    case 1:
      k2000_anim_number = k2000_anim_number << 1;
      k2000_anim_count ++;
      if (k2000_anim_count + 1 == k2000_led_count - k2000_led_on) {
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
  display_binary(k2000_anim_number, k2000_led_count);
}

void display_table(byte row[]) {
  for (byte index = 0; index < led_count; index++) {
    digitalWrite(ledPin[index], row[index]);
  }
}

void display_binary(uint16_t number_to_display, int nb_led) {
  for (byte index = 0; index < nb_led - 1; index++) {
    /*    Serial.print("led ");
        Serial.print(ledPin[index]);
        Serial.print(" ");
        Serial.println(bitRead(number_to_display, index) ? "o" : " "); */

    digitalWrite(ledPin[index], bitRead(number_to_display, index));
  }
}
