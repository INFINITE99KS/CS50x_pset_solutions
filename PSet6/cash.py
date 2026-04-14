# Cash, just ported to python from C.

def main():

    # Getting user input, and handling unexpected inputs as well.
    change = 0
    while True:
        try:
            change = float(input("Change Owed: "))
            if change >= 0:
                break
        except ValueError:
            pass

    change *= 100  # Multiplying by 100, so the program can handle Change Owed better.
    change = int(change)
    coins_total = 0  # To keep track of total coins.

    quarters = coins(change, 25)  # Calculates quarters.
    change -= quarters*25  # Adjusting change after calculating quarters.
    coins_total += quarters

    dimes = coins(change, 10)    # Calcualtes dimes.
    change -= dimes*10  # Adjusting change after calculating dimes.
    coins_total += dimes

    nickels = coins(change, 5)   # Calculates nickels.
    change -= nickels*5  # Adjusting change after calculating nickels.
    coins_total += nickels

    pennies = coins(change, 1)   # Calculates pennies.
    change -= pennies*1  # Adjusting change after calculating pennies.
    coins_total += pennies

    print(f"{coins_total}")


def coins(change, coin_type):
    coin = change // coin_type
    return coin


main()
