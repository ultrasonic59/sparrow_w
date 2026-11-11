/**
 *	button.h
 *
 *	Adrian Remonda 2015
 *
 */
#ifndef BUTTON_H
#define BUTTON_H

#define PB_PIN 		51

int button_init(void);
void button_task(void *pvParameters);

#endif
