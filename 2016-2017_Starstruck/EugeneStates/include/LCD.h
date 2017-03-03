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
 *
 * @param lcdPort is the LCD port (uart1 or uart2)
 */
MenuItem *lcdmInit(FILE *lcdPort);

/**
 * Adds built in menu features to active menu
 *
 * @param root is a pointer to the root MenuItem
 */
void lcdmAddDefaults(MenuItem *root);

/**
 * Creates and adds an LCD MenuItem. Returns a pointer to the MenuItem.
 *
 * @param name is the string that appears on the menu
 * @param root is a pointer to the root MenuItem
 * @param action is the name of the function to run when the item is selected
 * @param param is the parameter to the action function
 */
MenuItem *lcdmCraddItem(char *name, MenuItem *root, void *action, void *param);

/**
 * Creates and adds an LCD Submenu. Returns a pointer to the root MenuItem of the submenu.
 *
 * @param link is a pointer to the MenuItem that should switch to the newly created submenu when selected
 */
MenuItem *lcdmCraddSubmenu(MenuItem *link);


/**
 * Creates an LCD Menu Item
 *
 * @param name is the string that appears on the menu
 */
MenuItem *lcdmCreateItem(char *name);

/**
 * Adds menu item to LCD menu
 *
 * @param root is a pointer to the root MenuItem
 * @param item is a pointer to the item to add
 */
void lcdmAddItem(MenuItem *root, MenuItem *item);

/**
 * Updates LCD menu
 *
 * This should be run within the opcontrol loop
 * 
 * @param root is a pointer to the root MenuItem
 */
void lcdmLoop(MenuItem *root);

/**
 * Plays F, A# on the speaker if connected
 */
void beep();

/**
 * Plays A#, F on the speaker if connected
 */
void boop();

#endif //LCD_H_
