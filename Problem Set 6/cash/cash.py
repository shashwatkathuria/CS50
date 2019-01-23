from cs50 import get_float
from math import floor


def main():
    # prompts user for change owed
    while True:
        dollars_owed = get_float("Change owed: ")
        cents_owed = floor(dollars_owed * 100)

        if cents_owed > 0:
            break
    # calculates no of coins greedily
    quarters = cents_owed // 25
    dimes = (cents_owed % 25) // 10
    nickels = ((cents_owed % 25) % 10) // 5
    pennies = ((cents_owed % 25) % 10) % 5

    # prints total no of coins
    print(f"{quarters + dimes + nickels + pennies}")


# calls main
if __name__ == "__main__":
    main()