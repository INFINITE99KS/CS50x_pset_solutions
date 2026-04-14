# Prompts the user to enter a number.

# Defines a variable height.
height = 0

# Prompt the user for a pyramid height between 1 and 8 inclusive.
# Repeat prompt until valid input is provided.
while True:
    try:
        height = int(input("Height: "))
        if (height >= 1 and height <= 8):
            break
    except ValueError:
        pass

# A loop to print the pyramid.
for i in range(height):
    print(" "*(height-i-1), end="")
    print("#"*(i+1))
