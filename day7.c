#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// length includes until the second newline char
uint64_t processTwoLines(char *twoLineBuffer, int length) {
  uint64_t splitCount = 0;

  for (int i = 0; i < length / 2; i++) {
    if (twoLineBuffer[i] == 'S') {
      // Directly below S is a |
      if (twoLineBuffer[length / 2 + i] != '^') {
        twoLineBuffer[length / 2 + i] = '|';
      } else {
        splitCount++;
        // right and left
        twoLineBuffer[length / 2 + i - 1] = '|';
        twoLineBuffer[length / 2 + i + 1] = '|';
      }
    } else if (twoLineBuffer[i] == '|') {
      // Propgate line
      if (twoLineBuffer[length / 2 + i] != '^') {
        twoLineBuffer[length / 2 + i] = '|';
      } else {
        splitCount++;
        // right and left
        twoLineBuffer[length / 2 + i - 1] = '|';
        twoLineBuffer[length / 2 + i + 1] = '|';
      }
    }
  }
  return splitCount;
}

int main() {
  FILE *f;
  if ((f = fopen("day7.txt", "r")) == NULL) {
    printf("Unable to open the file day7.txt!\n");
    exit(1);
  }

  char twoLineBuffer[512] = {0};
  int writeIndex = 0;
  char c;
  bool first = true;
  uint64_t splitCount = 0;
  while ((c = fgetc(f)) && c != EOF) {
    if (c == '\n') {
      twoLineBuffer[writeIndex] = c;
      writeIndex++;
      if (first) {
        // keep going until we have two lines
        first = false;
        continue;
      }

      splitCount += processTwoLines(twoLineBuffer, writeIndex);

      // Shift the second line to the first line and reset writeIndex
      memmove(twoLineBuffer, twoLineBuffer + writeIndex / 2, writeIndex / 2);
      writeIndex -= writeIndex / 2;
      continue;
    }

    twoLineBuffer[writeIndex] = c;
    writeIndex++;
  }

  // last line doesn't matter, we don't split anyway

  printf("Total splits: %llu\n", splitCount);

  fclose(f);
}
