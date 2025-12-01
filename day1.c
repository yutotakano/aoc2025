#include <stdio.h>
#include <stdlib.h>
#include <string.h>

_Bool processLine(char * line, int * currentValue) {
  if (strlen(line) == 0) {
    printf("Unexpected line with 0 length.\n");
    return 0;
  }

  int num = atoi(&line[1]);

  if (line[0] == 'R') {
    *currentValue = (*currentValue + num) % 100;
  } else if (line[0] == 'L') {
    *currentValue = (((*currentValue - num) % 100) + 100) % 100;
  }

  if (*currentValue == 0) {
    return 1;
  }

  return 0;
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
  char c;
  while ((c = fgetc(f)) && c != EOF) {
    if (c == '\n') {
      lineBuffer[writeIndex] = 0;
      if (processLine(lineBuffer, &currentValue)) {
        becameZeroCount++;
      };

      // reset line buffer
      memset(lineBuffer, 0, sizeof(lineBuffer) / sizeof(char));
      writeIndex = 0;

      continue;
    }

    lineBuffer[writeIndex] = c;
    writeIndex++;
  }

  printf("Became zero %d times\n", becameZeroCount);
}
