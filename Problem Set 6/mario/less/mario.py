from cs50 import get_int


def main():
    # checks for valid prompts
    while True:
        height = get_int("Height: ")
        width = height + 1
        if height >= 0 and height <= 23:
            break
    # outputs spaces and hashes accordingly
    for i in range(1, height + 1):
        num_hashes = i + 1
        num_spaces = width - num_hashes

        print(" " * num_spaces, end="")
        print("#" * num_hashes)


# calls main
if __name__ == "__main__":
    main()