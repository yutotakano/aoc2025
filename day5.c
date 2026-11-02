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

uint64_t count_fresh_ingredients(uint64_t ingredient_index,
                                 uint64_t *fresh_ranges) {
  for (uint32_t i = 0; i < 1024; i += 2) {
    if (fresh_ranges[i] == 0 && fresh_ranges[i + 1] == 0) {
      break;
    }
    if (ingredient_index >= fresh_ranges[i] &&
        ingredient_index <= fresh_ranges[i + 1]) {
      return 1;
    }
  }
  return 0;
}

struct Range {
  uint64_t smallValue;
  uint64_t largeValue;
};

int compare_ranges(const void *a, const void *b) {
  const struct Range *rangeA = (const struct Range *)a;
  const struct Range *rangeB = (const struct Range *)b;

  // order by largetValue in increasing order
  if (rangeA->largeValue < rangeB->largeValue) {
    return -1;
  } else if (rangeA->largeValue > rangeB->largeValue) {
    return 1;
  } else {
    return 0;
  }
}

void simplify_ranges(uint64_t *raw_ranges, uint32_t *raw_range_count) {
  // work backwards and merge
  struct Range *ranges = (struct Range *)raw_ranges;
  int32_t range_count = *raw_range_count / 2;

  qsort(ranges, range_count, sizeof(struct Range), compare_ranges);

  for (uint32_t i = range_count - 1; i > 0; i--) {
    if (ranges[i - 1].largeValue >= ranges[i].smallValue) {
      // merge
      ranges[i - 1].largeValue = ranges[i - 1].largeValue > ranges[i].largeValue
                                     ? ranges[i - 1].largeValue
                                     : ranges[i].largeValue;
      ranges[i - 1].smallValue = ranges[i - 1].smallValue < ranges[i].smallValue
                                     ? ranges[i - 1].smallValue
                                     : ranges[i].smallValue;
      // remove the current range by shifting left
      for (uint32_t j = i; j < range_count - 1; j++) {
        ranges[j] = ranges[j + 1];
      }
      range_count--;
    }
  }

  *raw_range_count = range_count * 2;
}

uint64_t sum_range_count(uint64_t *ranges, uint32_t range_count) {
  uint64_t sum = 0;
  for (uint32_t i = 0; i < range_count; i += 2) {
    sum += ranges[i + 1] - ranges[i] + 1;
  }
  return sum;
}

int main() {
  FILE *f;
  if ((f = fopen("day5.txt", "r")) == NULL) {
    printf("Unable to open the file day5.txt!\n");
    exit(1);
  }

  uint64_t smallValue = 0;
  uint64_t largeValue = 0;
  char lineBuffer[64] = {0};
  uint64_t fresh_ranges[1024] = {0};
  uint32_t fresh_ranges_count = 0;

  int writeIndex = 0;
  char c;
  uint64_t fresh_ingredients = 0;
  bool parsing_ranges = true;
  uint64_t ingredient_index = 0;
  while ((c = fgetc(f)) && c != EOF) {
    if (c == '\n') {
      if (writeIndex == 0) {
        // double new line
        parsing_ranges = false;
        continue;
      }
      if (parsing_ranges) {
        lineBuffer[writeIndex] = 0;
        parse_range(lineBuffer, &smallValue, &largeValue);
        fresh_ranges[fresh_ranges_count++] = smallValue;
        fresh_ranges[fresh_ranges_count++] = largeValue;
        // reset line buffer
        memset(lineBuffer, 0, sizeof(lineBuffer) / sizeof(char));
        writeIndex = 0;
      } else {
        lineBuffer[writeIndex] = 0;
        parse_ingredient_index(lineBuffer, &ingredient_index);
        memset(lineBuffer, 0, sizeof(lineBuffer) / sizeof(char));
        writeIndex = 0;

        fresh_ingredients +=
            count_fresh_ingredients(ingredient_index, fresh_ranges);
      }

      continue;
    }

    lineBuffer[writeIndex] = c;
    writeIndex++;
  }

  // final parse
  lineBuffer[writeIndex] = 0;
  parse_ingredient_index(lineBuffer, &ingredient_index);
  fresh_ingredients += count_fresh_ingredients(ingredient_index, fresh_ranges);

  simplify_ranges(fresh_ranges, &fresh_ranges_count);
  uint64_t fresh_ingredient_total =
      sum_range_count(fresh_ranges, fresh_ranges_count);

  printf("Fresh ingredients: %llu\n", fresh_ingredients);
  printf("Total fresh ingredients: %llu\n", fresh_ingredient_total);
}
