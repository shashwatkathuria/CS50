#include<stdio.h>
#include<cs50.h>

int main(void)
{
    int height;
    //prompting user for positive height
    do
    {
        height = get_int("Height:  ");
    }
    while (height < 0 || height > 23);
    //pyramid
    for (int i = 0; i < height; i++)
    {
        //spaces
        for (int space = 0; space < (height - 1 - i); space++)
        {
            printf(" ");
        }
        //left part of pyramid
        for (int hash = 0; hash < i + 1; hash++)
        {
            printf("#");
        }
        printf("  ");
        //right part of pyramid
        for (int hash = 0; hash < i + 1; hash++)
        {
            printf("#");
        }
        printf("\n");
    }



}
