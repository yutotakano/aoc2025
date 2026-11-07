#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  FILE *f;
  if ((f = fopen("day6.txt", "r")) == NULL) {
    printf("Unable to open the file day6.txt!\n");
    exit(1);
  }

  char fileBuffer[8][5012] = {0};

  int32_t lineIndex = 0;
  int32_t writeIndex = 0;
  char c;
  while ((c = fgetc(f)) && c != EOF) {
    if (c == '\n') {
      fileBuffer[lineIndex][writeIndex] = 0; // null terminate
      lineIndex++;
      writeIndex = 0;
      continue;
    }

    fileBuffer[lineIndex][writeIndex] = c;
    writeIndex++;

    // Warn if going past
    if (writeIndex >= 5012) {
      printf("Warning: line %d is too long!\n", lineIndex + 1);
    }
  }

  int32_t lineLength = strlen(fileBuffer[lineIndex - 1]);

  // Iterate through last row and find pos of all markers
  // two loops, first to get count of markers so we can alloc
  uint32_t markerCount = 0;
  for (int i = 0; i < lineLength; i++) {
    if (fileBuffer[lineIndex - 1][i] == '+' ||
        fileBuffer[lineIndex - 1][i] == '*') {
      markerCount++;
    }
  }

  uint32_t *markerPositions = malloc(sizeof(uint32_t) * markerCount);
  for (int i = 0, j = 0; i < lineLength; i++) {
    if (fileBuffer[lineIndex - 1][i] == '+' ||
        fileBuffer[lineIndex - 1][i] == '*') {
      markerPositions[j++] = i;
    }
  }

  uint64_t totalSum = 0;
  for (int i = 0; i < markerCount; i++) {
    uint32_t pos = markerPositions[i];

    uint32_t next_pos =
        (i + 1 < markerCount) ? markerPositions[i + 1] : lineLength + 1;

    if (fileBuffer[lineIndex - 1][pos] == '+') {
      // Add the values
      uint64_t sum = 0;
      for (int j = pos; j < next_pos - 1; j++) {
        uint64_t val = 0;
        for (int k = 0; k < lineIndex - 1; k++) {
          if (fileBuffer[k][j] >= '0' && fileBuffer[k][j] <= '9') {
            val = val * 10 + (fileBuffer[k][j] - '0');
          }
        }
        sum += val;
      }
      totalSum += sum;
    } else {
      uint64_t product = 1;
      for (int j = pos; j < next_pos - 1; j++) {
        uint64_t val = 0;
        for (int k = 0; k < lineIndex - 1; k++) {
          if (fileBuffer[k][j] >= '0' && fileBuffer[k][j] <= '9') {
            val = val * 10 + (fileBuffer[k][j] - '0');
          }
        }
        product *= val;
      }
      totalSum += product;
    }
  }

  printf("Total sum of all operations: %llu\n", totalSum);
}
