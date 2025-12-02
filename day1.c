#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void processLine(char * line, int * currentValue, int * becameZeroAtEndCount, int * becameZeroMidwayCount) {
  if (strlen(line) == 0) {
    printf("Unexpected line with 0 length.\n");
    return;
  }

  int num = atoi(&line[1]);
  int higherNumber, lowerNumber, finalNumber;
  if (line[0] == 'R') {
    higherNumber = *currentValue + num;
    lowerNumber = *currentValue;
    finalNumber = *currentValue + num;
  } else if (line[0] == 'L') {
    higherNumber = *currentValue;
    lowerNumber = *currentValue - num;
    finalNumber = *currentValue - num;
  }

  // count number of multiple of 100s in between (low, high)
  int roundedDown = ((higherNumber) / 100) * 100;
  int roundedUp = ((lowerNumber - 1 + 100) / 100) * 100;
  if (lowerNumber < 0) {
    roundedUp = ((-lowerNumber) / 100) * -100;
  }
  if (roundedDown - roundedUp >= 0) {
    (*becameZeroMidwayCount) += (roundedDown - roundedUp) / 100 + 1;
  }
  printf("Between %d and %d (towards %d), %d happened\n", lowerNumber, higherNumber, finalNumber, (roundedDown - roundedUp) / 100 + 1);
  *currentValue = ((finalNumber % 100) + 100) % 100;

  if (*currentValue == 0) {
    (*becameZeroAtEndCount)++;
  }

  return;
}


int main() {
  FILE *f;
  if (fopen_s(&f, "day1.txt", "r")) {
    printf("Unable to open the file day1.txt!\n");
    exit(1);
  }

  int currentValue = 50;
  char lineBuffer[64] = {0};
  int writeIndex = 0;
  int becameZeroCount = 0;
  int becameZeroMidwayCount = 0;
  char c;
  while ((c = fgetc(f)) && c != EOF) {
    if (c == '\n') {
      lineBuffer[writeIndex] = 0;
      processLine(lineBuffer, &currentValue, &becameZeroCount, &becameZeroMidwayCount);

      // reset line buffer
      memset(lineBuffer, 0, sizeof(lineBuffer) / sizeof(char));
      writeIndex = 0;

      continue;
    }

    lineBuffer[writeIndex] = c;
    writeIndex++;
  }

  printf("Became zero %d times\n", becameZeroCount);
  printf("Became zero %d times incl. midway\n", becameZeroMidwayCount - becameZeroCount);
}
