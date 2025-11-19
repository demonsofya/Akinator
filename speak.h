#ifndef SPEAK_H_INCLUDED
#define SPEAK_H_INCLUDED

#define TX_USE_SPEAK
#include "TXLib.h"

HWND CreateAkinatorWindow();
void DestroyAkinatorWindow();

const HDC boy_image_dissapoint    = txLoadImage("images/akinator_boy_1.bmp");
const HDC boy_image_cringe_1      = txLoadImage("images/akinator_boy_2.bmp");
const HDC boy_image_what_the_f    = txLoadImage("images/akinator_boy_3.bmp");
const HDC boy_image_ok            = txLoadImage("images/akinator_boy_4.bmp");
const HDC boy_image_train         = txLoadImage("images/akinator_boy_5.bmp");
const HDC boy_image_cringe_2      = txLoadImage("images/akinator_boy_6.bmp");
const HDC boy_image_fuck_you      = txLoadImage("images/akinator_boy_7.bmp");

const int WINDOW_X_SIZE = 600;
const int WINDOW_Y_SIZE = 800;

const int TEXT_SIZE     = 40;
const int TEXT_X_POS    = 40;
const int TEXT_Y_POS    = 200;

void ChangeImage(const HDC image);
void ChangeText(const char *text);
void ChangeImageAndText(const HDC image, const char *text);

#endif // SPEAK_H_INCLUDED
