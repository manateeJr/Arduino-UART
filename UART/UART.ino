#define BITLENGTH 8
#define RXPIN 0

#define WRONGSTOPBITLED 3
#define WRONGPARITYLED 5
#define CORRECTCHARLED 4

#define MILLION 1000000

#define BAUDRATE 9600 // max working baudrate is 9600, the program works with lower, but not with higher. Only change the baudrate here and it all works.
#define BITTIME MILLION / BAUDRATE  // in microseconds
#define HALFBITTIME BITTIME / 2

#define EVENPARITY 1  // use this for even parity

char data_bits;
int data_buffer;
int parity_bit;
int stop_bit;

unsigned long long uart_timer = 0;
bool go_next = false;
int parity_counter = 0;
int bit_counter = 0;

void setup() {
  Serial.begin(BAUDRATE);

  pinMode(RXPIN, INPUT);

  pinMode(WRONGPARITYLED, OUTPUT);
  pinMode(CORRECTCHARLED, OUTPUT);
  pinMode(WRONGSTOPBITLED, OUTPUT);
}

enum stateUART {
  WAIT,
  READ_DATA,
  READ_PARITY,
  READ_STOP,
  CHECK,
};

stateUART currentState = WAIT;

void loop() {
  unsigned long long current_time = micros();
  switch (currentState) {
    case WAIT:
      if (digitalRead(RXPIN) == LOW) {
        uart_timer = micros();
        go_next = true;
      }

      if (go_next && (current_time - uart_timer > HALFBITTIME)) {
        uart_timer = current_time;
        go_next = false;
        currentState = READ_DATA;
      }
      break;

    case READ_DATA:
      if (current_time - uart_timer > BITTIME) {
        uart_timer = current_time;
        data_bits |= digitalRead(RXPIN) << bit_counter;
        if (digitalRead(RXPIN)) { parity_counter++; }
        bit_counter++;
      }
      if (bit_counter == BITLENGTH) {
        uart_timer = current_time;
        currentState = READ_PARITY;
      }
      break;

    case READ_PARITY:
      if (current_time - uart_timer > BITTIME) {
        uart_timer = current_time;
        if (digitalRead(RXPIN)) { parity_counter++; }
        currentState = READ_STOP;
      }
      break;

    case READ_STOP:
      if (current_time - uart_timer > BITTIME) {
        uart_timer = current_time;
        stop_bit = digitalRead(RXPIN);
        currentState = CHECK;
      }
      break;

    case CHECK:
      currentState = WAIT;
      Serial.write(data_bits);
      check_character();
      check_stop_bit();
      check_parity();
      data_bits = 0;
      parity_counter = 0;
      bit_counter = 0;
      break;
  }
}

void check_character() {
  if (data_bits == 'a') {  // correct character ('a')
    digitalWrite(CORRECTCHARLED, HIGH);
  } else {  // wrong character
    digitalWrite(CORRECTCHARLED, LOW);
  }
}

void check_stop_bit() {
  if (stop_bit) {  //  correct stop bit
    digitalWrite(WRONGSTOPBITLED, LOW);
  } else {  //   wrong stop bit
    digitalWrite(WRONGSTOPBITLED, HIGH);
  }
}

void check_parity() {
  if (~parity_counter & EVENPARITY) {  // correct parity (isEven)
    digitalWrite(WRONGPARITYLED, LOW);
  } else {  // wrong parity
    digitalWrite(WRONGPARITYLED, HIGH);
  }
}