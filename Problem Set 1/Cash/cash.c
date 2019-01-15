#include<stdio.h>
#include<cs50.h>
#include<math.h>
int main(void)
{
    float cash;
    //prompting user for positive float
    do
    {
        cash = get_float("Change owed:  ");
    }
    while (cash < 0);
    int n = 0;
    //decrementing by 0.25 until ccash drops to less than 1
    if (cash >= 1)
    {
        while (cash >= 1)
        {
            cash -= 0.25;
            n++;
        }
    }
    cash = round(cash * 100);
    int temp = (int) cash;
    //n denotes no of coins owed
    int change = temp % 100;
    //evaluating no of coins by greedy algorithm
    while (change > 0)
    {
        //decrementing change accordingly
        if (change >= 25)
        {
            change -= 25;
            n++;
        }
        else if (change >= 10)
        {
            change -= 10;
            n++;
        }
        else if (change >= 5)
        {
            change -= 5;
            n++;
        }
        else if (change >= 1)
        {
            change -= 1;
            n++;
        }
        else
        {
            break;
        }
    }
    //printing no of coins owed
    printf("The number of minimum coins required is:%i\n", n);

}