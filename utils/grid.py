# arg[1] = X
# arg[2] = Y

import sys

if len(sys.argv) != 3:
    print("Must Pass X and Y Size Arguments")
    exit(1)

X = int(sys.argv[1])
Y = int(sys.argv[2])

color_a = 0x88
color_b = 0xAA

print(f'Making Grid Of Size ({X}, {Y})')

def IsEven(i):
    return not (i & 1)

def PrintColor(color):
    print(f" {hex(color)},{hex(color)},{hex(color)},", end="")

def Process(_X, y, _Y):
    color = color_a
    for x in range(_X):
        if x % 8 == 0:
            print("")
        if x >= int(_X)/2:
            if y >= int(_Y)/2:
                color = color_a
            else:
                color = color_b
        else:
            if y >= int(_Y)/2:
                color = color_b
            else:
                color = color_a
        PrintColor(color)

if not IsEven(int(X)) or not IsEven(int(Y)):
    print("X and Y Must Be Even")
    exit(1)


for y in range(Y):
    print("\n", end="")
    if int(X) > 8:
        Process(X, y, Y)
    else:
        color = color_a
        for x in range(X):
            if x >= int(X)/2:
                if y >= int(Y)/2:
                    color = color_a
                else:
                    color = color_b
            else:
                if y >= int(Y)/2:
                    color = color_b
                else:
                    color = color_a
            PrintColor(color)

