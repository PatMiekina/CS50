from cs50 import get_int

# Request height
while True:
    height = get_int('Height: ')
    if height < 1 or height > 8:
        height = get_int('Height: ')
    if height >= 1 or height <= 8:
        break

# Print tower
for i in range(1, height + 1):
    print(" " * (height - i) + "#" * i + "  " + "#" * i)