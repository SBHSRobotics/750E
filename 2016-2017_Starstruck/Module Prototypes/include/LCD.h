#ifndef LCD_H_
#define LCD_H_

#include <main.h>

typedef void (*Function)(void *);

// Menu Item Struct
typedef struct MenuItem {
  char *name;
  struct MenuItem *next;
  struct MenuItem *previous;
  struct MenuItem *select;
  void (*action)(void *);
  void (*param);
}MenuItem;

/**
 * Initializes LCD Menu module and returns root MenuItem
 *
 * This function should be called from initialize()
 * DO NOT initialize the LCD separately
 */
MenuItem *lcdmInit(FILE *lcdPort);

/**
 * Adds built in menu features to active menu
 *
 */
void lcdmAddDefaults(MenuItem *root);

/**
 * Creates and adds an LCD Menu Item
 *
 * @param name is the string that appears on the menu
 * @param root is the root MenuItem to add the new item to
 * @param action is the function to run when the item is selected
 * @param param is the parameter to the action7 function
 */
MenuItem *lcdmCraddItem(char *name, MenuItem *root, void *action, void *param);

MenuItem *lcdmCraddSubmenu(MenuItem *link);


/**
 * reates an LCD Menu Item
 *
 * @param name is the string that appears on the menu
 */
MenuItem *lcdmCreateItem(char *name);

/**
 * Adds menu item to LCD menu
 *
 * @param name is the string that appears on the menu
 */
void lcdmAddItem(MenuItem *root, MenuItem *item);

/**
 * Updates LCD menu
 *
 * This should be run within the opcontrol loop
 */
void lcdmLoop(MenuItem *root);

void boop();

#endif //LCD_H_
