#!/usr/bin/env python3

from random import randrange


def main(fname, c):
    with open(fname, "r") as f:
        data = f.readlines()
    with open(fname + ".jitter", "w") as g:
        for l in data:
            try:
                g.write(str(max(0, int(l.strip()) + randrange(-1.0 * c, c))) + "\n")
            except ValueError:
                return


if __name__ == '__main__':
    from sys import argv
    if len(argv) > 1:
        main(argv[1], 10)
