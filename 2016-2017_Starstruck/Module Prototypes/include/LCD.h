#ifndef LCD_H_

#define LCD_H_

typedef void (*Function)(void *);

// Menu Item Struct
typedef struct MenuItem {
  char name[12];
  struct MenuItem *next;
  struct MenuItem *previous;
  struct MenuItem *select;
  Function action;
}MenuItem;

/**
 * Initializes LCD Menu module and returns root MenuItem
 *
 * This function should be called from initialize()
 * DO NOT initialize the LCD separately
 */
MenuItem *lcdmInit();

/**
 * Adds built in menu features to active menu
 *
 */
void lcdmAddDefaults(MenuItem *root);

/**
 * Initializes LCD Menu module
 *
 * This function should be called from initialize()
 * DO NOT initialize the LCD separately
 */
MenuItem lcdmCreateItem(char name[12], MenuItem *select, Function action);

/**
 * Adds menu item to LCD menu
 *
 * @param name is the string that appears on the menu
 */
void lcdmAddItem(MenuItem *root, MenuItem item);

/**
 * Updates LCD menu
 *
 * This should be run within the opcontrol loop
 */
void lcdmLoop(MenuItem *root);

#endif //LCD_H_
