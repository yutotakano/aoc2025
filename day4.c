#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint64_t processCentralLine(char *lines, int length) {
  assert((length - 2) % 3 == 0); // new line chars
  uint8_t lineLength = (length - 2) / 3;

  uint64_t movablePaperRolls = 0;
  for (int i = 0; i < lineLength; i++) {
    if (lines[lineLength + 1 + i] != '@') {
      continue;
    }
    uint8_t occupied = 0;
    if (lines[i] == '@') {
      occupied++;
    }
    if (lines[i + 1] == '@') {
      occupied++;
    }
    if (i < lineLength - 1 && lines[lineLength + 1 + i + 1] == '@') {
      occupied++;
    }
    if (i < lineLength - 1 && lines[2 * lineLength + 2 + i + 1] == '@') {
      occupied++;
    }
    if (lines[2 * lineLength + 2 + i] == '@') {
      occupied++;
    }
    if (i > 0 && lines[2 * lineLength + 2 + i - 1] == '@') {
      occupied++;
    }
    if (i > 0 && lines[lineLength + 1 + i - 1] == '@') {
      occupied++;
    }
    if (i > 0 && lines[i - 1] == '@') {
      occupied++;
    }
    if (occupied < 4) {
      movablePaperRolls++;
    }
  }
  return movablePaperRolls;
}

int main() {
  FILE *f;
  if ((f = fopen("day4.txt", "r")) == NULL) {
    printf("Unable to open the file day4.txt!\n");
    exit(1);
  }

  char threeLineBuffer[512] = {0};
  int writeIndex = 0;
  char c;
  uint64_t movablePaperRolls = 0;
  uint8_t lineCount = 0;
  while ((c = fgetc(f)) && c != EOF) {
    if (c == '\n') {
      lineCount++;
      if (lineCount < 2) {
        threeLineBuffer[writeIndex] = c;
        writeIndex++;
        continue;
      }
      if (lineCount == 2) {
        // move the buffer to the right by one line length so
        // if we only have two lines we can still process the first line
        memmove(threeLineBuffer + (writeIndex - 1) / 2 + 1, threeLineBuffer,
                writeIndex);
        memset(threeLineBuffer, '.', (writeIndex - 1) / 2);
        threeLineBuffer[(writeIndex - 1) / 2] = '\n';
        writeIndex += (writeIndex - 1) / 2 + 1;
      }

      threeLineBuffer[writeIndex] = 0;
      movablePaperRolls += processCentralLine(threeLineBuffer, writeIndex);

      // from third line onwarsd, shift the buffer
      memmove(threeLineBuffer, threeLineBuffer + (writeIndex - 2) / 3 + 1,
              writeIndex - (writeIndex - 2) / 3);
      writeIndex -= (writeIndex - 2) / 3 + 1;
      threeLineBuffer[writeIndex] = c;
      writeIndex++;
      continue;
    }

    threeLineBuffer[writeIndex] = c;
    writeIndex++;
  }

  // process last line by adding a line of dots
  memset(threeLineBuffer + writeIndex, '.', (writeIndex - 2) / 2);
  writeIndex += (writeIndex - 2) / 2;
  threeLineBuffer[writeIndex] = 0;
  movablePaperRolls += processCentralLine(threeLineBuffer, writeIndex);

  printf("Sum of total movable paper rolls: %llu\n", movablePaperRolls);

  fclose(f);
}
