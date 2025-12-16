#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <gl_gamelib.h>
#include "commonvars.h"
#include "printfuncs.h"

// function provided by @Pharap
// If the array is not big enough,
s32 formatInteger(char *array, s32 integer)
{
    s32 maxDigits = 10;
    s32 maxCharacters = (maxDigits + 1);
    s32 lastIndex = (maxCharacters - 1);

    array[lastIndex] = '\0';

    if (integer == 0)
    {
        array[lastIndex - 1] = '0';
        return 1;
    }

    s32 digits = 0;

    do
    {
        s32 digit = integer % 10;
        integer /= 10;

        ++digits;

        array[lastIndex - digits] = ('0' + digit);
    } while (integer > 0);

    return digits;
};

//print a number on levelselect or game screen
void printNumber(gl_Image* fontBitmapTable, s32 ax, s32 ay, u16 tileSize, s32 aNumber, s32 maxDigits)
{
    const s32 buffSize = 10;
    char number[10 + 1]; //buffsize + 1
    s32 digits = formatInteger(number, aNumber);
    s32 maxFor = digits;
    if (digits > maxDigits)
        maxFor = maxDigits;
    for (s32 c = 0; c < maxFor; c++)
    {
        if (number[buffSize - digits + c] == '\0')
            return;

        gl_drawImage(fontBitmapTable, (ax + (maxDigits - digits) + c) * tileSize, ay * tileSize, number[buffSize - digits + c] - 47 + 1);

    }
}

//print a message on the title screen on ax,ay, the tileset from titlescreen contains an alphabet
void printMessage(gl_Image* fontBitmapTable, s32 ax, s32 ay, u16 tileSize, const char* amsg)
{
    // based on input from @Pharap
    const char *p = amsg;
    s32 index = 0;
   
    while (1)
    {
        char fChar = *(p++);
        int tile = 61;
        switch (fChar)
        {
            case '\0':
                return;

            case ':':
                tile = 37;
                break;

            case 'a':
                tile = 40;
                break;

            case 'b':
                tile = 38;
                break;

            default:
                if ((fChar >= 'A') && (fChar <= 'Z'))
                    tile = fChar - 54;

                if ((fChar >= '0') && (fChar <= '9'))
                    tile = fChar - 47;
                break;
         }
        gl_drawImage(fontBitmapTable, (ax + index) * tileSize, ay * tileSize, tile+1);
        ++index;
    }
}
