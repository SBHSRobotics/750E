#include <main.h>

MenuItem *lcdmInit() {
  char title[] = "<    Select    >";
  MenuItem root = {.name = title,
                   .previous = malloc(sizeof(MenuItem *)),
                   .next = malloc(sizeof(MenuItem *)),
                   .select = malloc(sizeof(MenuItem *)),
                   .action = malloc(sizeof(Function))
                  };
  return &root;
}

void lcdmAddDefaults(MenuItem *root) {
  //TODO implement
}

MenuItem lcdmCreateItem(char name[12], MenuItem *select, Function action) {
  MenuItem item = {.name = name,
                   .previous = malloc(sizeof(MenuItem *)),
                   .next = malloc(sizeof(MenuItem *)),
                   .select = select,
                   .action = action
                  };
      return item;
}

void lcdmAddItem(MenuItem *root, MenuItem item) {
  MenuItem *last = (*root).previous;
  (*last).next = &item;
  (*root).previous = &item;
}

void lcdmLoop(MenuItem *root) {
  // TODO implement
}
