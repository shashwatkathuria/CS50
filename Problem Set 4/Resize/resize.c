// Resizes a BMP file by a factor of n.

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize n infile outfile\n");
        return 1;
    }

    // retrieve the scaling factor as a float
    double scale = atof(argv[1]);

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // determine padding for scanlines (for the image we are reading from)
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // an array to store all of the pixels in the image we are reading from
    RGBTRIPLE image[bi.biWidth][abs(bi.biHeight)];

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // store it in the array
            image[j][i] = triple;
        }

        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);
    }

    // change the width and height measurements to suit the new scaled image
    bi.biWidth *= scale;
    bi.biHeight *= scale;

    // recalculate the padding (for the sclaed image we are writing)
    padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // calculate the number of bytes in the non-header part of the image
    bi.biSizeImage = abs(bi.biHeight) * (bi.biWidth * sizeof(RGBTRIPLE) + padding);

    // calculate the total number of bytes in the image (with the headers)
    bf.bfSize = bi.biSizeImage + 54;

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // writing to the scaled image's out file
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // use the scale factor to find the pixel that should be placed
            triple = image[(int)(j / scale)][(int)(i / scale)];

            // printf("(%i, %i, %i)\n", triple.rgbtRed, triple.rgbtGreen, triple.rgbtBlue);

            // write the pixel to the file
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }

        // write the buffer
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }


    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}