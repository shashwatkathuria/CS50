#include<stdio.h>
#include<cs50.h>
#include<math.h>

int main(void)
{
    //prompting for card no
    long long int cardno = get_long_long("Number: ");
    int sum1 = 0, sum2 = 0, r, x1, x2;
    long long int temp;
    temp = cardno;
    //adding even places sums
    do
    {
        //doing necessary calculations
        temp /= 10;
        r = temp % 10;
        temp /= 10;
        if (r * 2 >= 10)
        {
            r *= 2;
            x1 = r % 10;
            r = r / 10;
            x2 = r % 10;
            sum1 += x1 + x2;
        }
        else
        {
            sum1 += r * 2;
        }
    }
    while (temp != 0);
    temp = cardno;
    do
    {
        r = temp % 10;
        temp /= 100;
        sum2 += r;
    }
    while (temp != 0);
    //doing required card company calculations
    int mastercard = cardno / pow(10, 14);
    int amex = cardno / pow(10, 13);
    int visa1 = cardno / pow(10, 12);
    int visa2 = cardno / pow(10, 15);
    //printing the result if satisfied
    if ((sum1 + sum2) % 10 == 0)
    {
        //printing
        if (mastercard >= 51 && mastercard <= 55)
        {
            printf("MASTERCARD\n");
            return 0;
        }
        else if (amex == 34 || amex == 37)
        {
            printf("AMEX\n");
            return 0;
        }

        else if (visa1 == 4 || visa2 == 4)
        {
            printf("VISA\n");
            return 0;
        }
    }
    //else printing invalid card no
    printf("INVALID\n");
}