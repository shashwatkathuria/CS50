#include<stdio.h>
#include<cs50.h>

int main(void)
{
    int     height;
    //prompts the user for a positive integer height
    do
    {

        height = get_int("Height:  ");

    }
    while (height < 0 || height > 23);
    //prints the necessary pyramid
    for (int i = 0 ; i < height ; i++)
    {
        //printing spaces
        for (int space = 0 ; space < height - i - 1 ; space++)
        {
            printf(" ");
        }
        //printing hashes
        for (int hash = 0 ; hash < i + 2; hash++)
        {
            printf("#");
        }
        printf("\n");
    }

}