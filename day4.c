#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
      lines[lineLength + 1 + i] = '.';
      movablePaperRolls++;
    }
  }
  return movablePaperRolls;
}

uint64_t deletePaperRolls(FILE *f, FILE *tmpfile) {
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

      // Write middle line to temporary file
      fwrite(threeLineBuffer + (writeIndex - 2) / 3 + 1, 1,
             (writeIndex - 2) / 3, tmpfile);
      fputc('\n', tmpfile);

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

  fwrite(threeLineBuffer + (writeIndex - 2) / 3 + 1, 1, (writeIndex - 2) / 3,
         tmpfile);
  fputc('\n', tmpfile);

  fclose(f);
  return movablePaperRolls;
}

int main() {
  int tmpfile;
  char tmpfilename[] = "day4XXXXXXX";
  if ((tmpfile = mkstemp(tmpfilename)) == -1) {
    printf("Unable to create temporary file!\n");
    exit(1);
  }

  char tmpfilename2[] = "day4XXXXXXX-2";
  memcpy(tmpfilename2, tmpfilename, sizeof(tmpfilename));

  FILE *tmpfilePtr = fdopen(tmpfile, "w+");

  uint64_t movablePaperRolls = 0;
  bool first = true;
  while (1) {
    // make sure temp file seek is at the beginning
    fseek(tmpfilePtr, 0, SEEK_SET);

    if (first) {
      FILE *f;
      if ((f = fopen("day4.txt", "r")) == NULL) {
        printf("Unable to open the file day4.txt!\n");
        exit(1);
      }
      movablePaperRolls = deletePaperRolls(f, tmpfilePtr);
      first = false;
      continue;
    }

    // Move the temporary file to one with -2 and process again
    if (rename(tmpfilename, tmpfilename2) != 0) {
      printf("Unable to rename the temporary file!\n");
      exit(1);
    }

    FILE *f;
    if ((f = fopen(tmpfilename2, "r")) == NULL) {
      printf("Unable to open the temporary file!\n");
      exit(1);
    }
    uint64_t tmpMovablePaperRolls = 0;
    tmpMovablePaperRolls = deletePaperRolls(f, tmpfilePtr);
    fclose(f);
    if (tmpMovablePaperRolls == 0) {
      break;
    }
    movablePaperRolls += tmpMovablePaperRolls;
  }

  fclose(tmpfilePtr);
  remove(tmpfilename2);
  remove(tmpfilename);

  printf("Sum of total movable paper rolls: %llu\n", movablePaperRolls);
}
