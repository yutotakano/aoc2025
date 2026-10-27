#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parseRange(char *rangeBuffer, uint64_t *smallValue, uint64_t *largeValue) {
  char *token = strtok(rangeBuffer, "-");
  assert(token != NULL);
  *smallValue = strtoull(token, NULL, 10);

  token = strtok(NULL, "-");
  assert(token != NULL);
  *largeValue = strtoull(token, NULL, 10);
}

uint64_t sumInvalidIdsInRange(uint64_t smallValue, uint64_t largeValue) {
  uint64_t sum = 0;
  char buffer[32] = {0};
  for (uint64_t i = smallValue; i <= largeValue; i++) {
    uint8_t count = snprintf(buffer, sizeof(buffer), "%llu", i);
    if (count % 2 != 0) {
      continue;
    }

    bool isSmurfed = true;
    for (uint8_t j = 0; j < count / 2; j++) {
      if (buffer[j] != buffer[j + count / 2]) {
        isSmurfed = false;
        break;
      }
    }

    if (isSmurfed) {
      sum += i;
    }
  }
  return sum;
}

uint64_t sumMoreInvalidIdsInRange(uint64_t smallValue, uint64_t largeValue) {
  uint64_t sum = 0;
  char buffer[32] = {0};
  for (uint64_t i = smallValue; i <= largeValue; i++) {
    uint8_t count = snprintf(buffer, sizeof(buffer), "%llu", i);

    bool isSmurfedForAnyK = false;
    for (uint8_t k = 2; k <= count; k++) {
      if (count % k != 0) {
        continue;
      }
      bool isSmurfedForK = true;
      for (uint8_t j = 0; j < count / k; j++) {
        for (uint8_t l = j + count / k; l < count; l += count / k) {
          if (buffer[j] != buffer[l]) {
            isSmurfedForK = false;
            break;
          }
        }
      }
      if (isSmurfedForK) {
        isSmurfedForAnyK = true;
        break;
      }
    }

    if (isSmurfedForAnyK) {
      sum += i;
    }
  }
  return sum;
}

int main() {
  FILE *f;
  if ((f = fopen("day2.txt", "r")) == NULL) {
    printf("Unable to open the file day2.txt!\n");
    exit(1);
  }

  uint64_t smallValue = 0;
  uint64_t largeValue = 0;
  char rangeBuffer[64] = {0};
  int writeIndex = 0;
  char c;
  uint64_t invalid_id_sum = 0;
  uint64_t more_invalid_id_sum = 0;
  while ((c = fgetc(f)) && c != EOF) {
    if (c == ',') {
      rangeBuffer[writeIndex] = 0;
      parseRange(rangeBuffer, &smallValue, &largeValue);
      invalid_id_sum += sumInvalidIdsInRange(smallValue, largeValue);
      more_invalid_id_sum += sumMoreInvalidIdsInRange(smallValue, largeValue);

      // reset line buffer
      memset(rangeBuffer, 0, sizeof(rangeBuffer) / sizeof(char));
      writeIndex = 0;

      continue;
    }

    rangeBuffer[writeIndex] = c;
    writeIndex++;
  }

  // final parse
  rangeBuffer[writeIndex] = 0;
  parseRange(rangeBuffer, &smallValue, &largeValue);
  invalid_id_sum += sumInvalidIdsInRange(smallValue, largeValue);
  more_invalid_id_sum += sumMoreInvalidIdsInRange(smallValue, largeValue);

  printf("Sum of invalid IDs: %llu\n", invalid_id_sum);
  printf("Sum of more invalid IDs: %llu\n", more_invalid_id_sum);
}
