from cs50 import get_float

# Ask user for change
while True:
    change = get_float("Change owed: ")
    if change < 0:
        change = get_float("Change owed: ")
    elif change >= 0:
        break

coins = 0

# Round the change
change = round(int(change * 100))

# Calculate coins
while change > 0:
    while change >= 25:
        coins += 1
        change -= 25
    while change >= 10:
        coins += 1
        change -= 10
    while change >= 5:
        coins += 1
        change -= 5
    while change >= 1:
        coins += 1
        change -= 1

# Print answer
print(coins)
