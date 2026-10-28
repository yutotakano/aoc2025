#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint64_t bankJoltage(char *bankBuffer) {
  // find largest number before last digit
  uint8_t tens_ix = 0;
  uint8_t tens = 0;
  for (int8_t i = 0; i < strlen(bankBuffer) - 1; i++) {
    if ((uint8_t)bankBuffer[i] - '0' > tens) {
      tens = (uint8_t)bankBuffer[i] - '0';
      tens_ix = i;
    }
  }

  // find largest ones after tens_ix
  uint8_t ones = 0;
  for (uint8_t i = tens_ix + 1; i < strlen(bankBuffer); i++) {
    if ((uint8_t)bankBuffer[i] - '0' > ones) {
      ones = (uint8_t)bankBuffer[i] - '0';
    }
  }

  return tens * 10 + ones;
}

uint64_t bank12Joltage(char *bankBuffer) {
  uint8_t prev_digit_ix = -1;
  uint64_t result = 0;
  for (uint8_t digit = 0; digit < 12; digit++) {
    // find largest number before last digit
    uint8_t val = 0;
    for (int8_t i = prev_digit_ix + 1; i < strlen(bankBuffer) - (11 - digit);
         i++) {
      if ((uint8_t)bankBuffer[i] - '0' > val) {
        val = (uint8_t)bankBuffer[i] - '0';
        prev_digit_ix = i;
      }
    }
    result += val * (uint64_t)pow(10, 11 - digit);
  }
  return result;
}

int main() {
  FILE *f;
  if ((f = fopen("day3.txt", "r")) == NULL) {
    printf("Unable to open the file day3.txt!\n");
    exit(1);
  }

  char bankBuffer[128] = {0};
  int writeIndex = 0;
  char c;
  uint64_t total_joltage = 0;
  uint64_t total_12_joltage = 0;
  while ((c = fgetc(f)) && c != EOF) {
    if (c == '\n') {
      bankBuffer[writeIndex] = 0;
      total_joltage += bankJoltage(bankBuffer);
      total_12_joltage += bank12Joltage(bankBuffer);

      // reset line buffer
      memset(bankBuffer, 0, sizeof(bankBuffer) / sizeof(char));
      writeIndex = 0;

      continue;
    }

    bankBuffer[writeIndex] = c;
    writeIndex++;
  }

  printf("Sum of total joltage: %llu\n", total_joltage);
  printf("Sum of total 12 joltage: %llu\n", total_12_joltage);

  fclose(f);
}
