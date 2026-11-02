#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parse_range(char *rangeBuffer, uint64_t *smallValue,
                 uint64_t *largeValue) {
  char *token = strtok(rangeBuffer, "-");
  assert(token != NULL);
  *smallValue = strtoull(token, NULL, 10);

  token = strtok(NULL, "-");
  assert(token != NULL);
  *largeValue = strtoull(token, NULL, 10);
}

void parse_ingredient_index(char *lineBuffer, uint64_t *ingredient_index) {
  *ingredient_index = strtoull(lineBuffer, NULL, 10);
}

int main() {
  FILE *f;
  if ((f = fopen("day6.txt", "r")) == NULL) {
    printf("Unable to open the file day6.txt!\n");
    exit(1);
  }

  uint64_t currentArrayListSize = 128;
  uint64_t currentArrayListWriteIndex = 0;
  uint64_t *addList = malloc(sizeof(uint64_t) * currentArrayListSize);
  uint64_t *multiplyList = malloc(sizeof(uint64_t) * currentArrayListSize);
  for (uint64_t i = 0; i < currentArrayListSize; i++) {
    addList[i] = 0;
    multiplyList[i] = 1; // can't use memset since 64 bit
  }

  uint64_t grand_total = 0;

  char numberBuffer[32] = {0};

  int writeIndex = 0;
  char c;
  uint64_t fresh_ingredients = 0;
  bool parsing_ranges = true;
  uint64_t ingredient_index = 0;
  while ((c = fgetc(f)) && c != EOF) {
    if (c == ' ' || c == '\n') {
      if (writeIndex == 0 && c == ' ') {
        // double space or more
        continue;
      }
      numberBuffer[writeIndex] = 0;
      uint64_t number = strtoull(numberBuffer, NULL, 10);

      if (currentArrayListWriteIndex >= currentArrayListSize) {
        // extend by double
        currentArrayListSize *= 2;
        addList = realloc(addList, sizeof(uint64_t) * currentArrayListSize);
        multiplyList =
            realloc(multiplyList, sizeof(uint64_t) * currentArrayListSize);
        for (uint64_t i = currentArrayListWriteIndex; i < currentArrayListSize;
             i++) {
          addList[i] = 0;
          multiplyList[i] = 1;
        }
      }

      addList[currentArrayListWriteIndex] += number;
      multiplyList[currentArrayListWriteIndex] *= number;
      currentArrayListWriteIndex++;
      writeIndex = 0;

      if (c == '\n') {
        currentArrayListWriteIndex = 0;
      }

      continue;
    }

    if (c == '+') {
      grand_total += addList[currentArrayListWriteIndex];
      currentArrayListWriteIndex++;
      continue;
    }

    if (c == '*') {
      grand_total += multiplyList[currentArrayListWriteIndex];
      currentArrayListWriteIndex++;
      continue;
    }

    numberBuffer[writeIndex] = c;
    writeIndex++;
  }

  printf("Grand total: %llu\n", grand_total);
  free(addList);
  free(multiplyList);
}
