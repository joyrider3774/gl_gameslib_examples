#ifndef PRINTFUNCS_H
#define PRINTFUNCS_H

#include <gl_gamelib.h>

void printNumber(gl_Image* fontBitmapTable, s32 ax, s32 ay, u16 tileSize, s32 aNumber, s32 maxDigits);
void printMessage(gl_Image* fontBitmapTable, s32 ax, s32 ay, u16 tileSize, const char* amsg);



#endif