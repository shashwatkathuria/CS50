#include<cs50.h>
#include<stdio.h>
//#include"bmp.h"
// BMP-related data types based on Microsoft's own

#include <stdint.h>

// aliases for C/C++ primitive data types
// https://msdn.microsoft.com/en-us/library/cc230309.aspx
typedef uint8_t  BYTE;
typedef uint32_t DWORD;
typedef int32_t  LONG;
typedef uint16_t WORD;

// information about the type, size, and layout of a file
// https://msdn.microsoft.com/en-us/library/dd183374(v=vs.85).aspx
typedef struct
{
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
} __attribute__((__packed__))
BITMAPFILEHEADER;

// information about the dimensions and color format
// https://msdn.microsoft.com/en-us/library/dd183376(v=vs.85).aspx
typedef struct
{
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} __attribute__((__packed__))
BITMAPINFOHEADER;

// relative intensities of red, green, and blue
// https://msdn.microsoft.com/en-us/library/dd162939(v=vs.85).aspx
typedef struct
{
    BYTE rgbtBlue;
    BYTE rgbtGreen;
    BYTE rgbtRed;
} __attribute__((__packed__))
RGBTRIPLE;


int main(int argc, char *argv[])
{
    //Checking if input is correct
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }
    char *infile = argv[1];
    FILE *inptr = fopen(infile, "r");
    //Checking if the file pointer is assigned or not
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }
    bool flag = false;
    BYTE buffer[512];
    //declaring pointer to the photo to be recovered
    FILE *newphotoptr;
    int n = 0;
    //recovering images until the end of file isn't reached
    while (fread(buffer, 1, 512, inptr) != 0x00)
    {
        //checking for start of the photo
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //initializing photo number accordingly
            if (!flag)
            {
                flag = true;
                char filename[8];
                sprintf(filename, "%03i.jpg", n++);
                newphotoptr = fopen(filename, "w");
                if (newphotoptr == NULL)
                {
                    return 2;
                }
                fwrite(buffer, 1, 512, newphotoptr);
            }
            else
            {
                fclose(newphotoptr);
                char filename[8];
                sprintf(filename, "%03i.jpg", n++);
                newphotoptr = fopen(filename, "w");
                if (newphotoptr == NULL)
                {
                    return 2;
                }
                fwrite(buffer, 1, 512, newphotoptr);
            }
        }
        //continuing to write into new photo file
        else
        {
            if (flag == true)
            {
                fwrite(buffer, 1, 512, newphotoptr);
            }
        }
    }
    //closing inptr
    fclose(inptr);
    //closing newphotoptr
    fclose(newphotoptr);
    return 0;
}