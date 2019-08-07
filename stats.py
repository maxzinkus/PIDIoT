#!/usr/bin/env python3

N = 100


def stats():
    s = 0.0
    least = None
    most = None
    for _ in range(N):
        try:
            d = float(input())
            s += d
            if least is None or d < least:
                least = d
            if most is None or d > most:
                most = d
        except ValueError:
            return None
    return(s/float(N), least, most)


if __name__ == '__main__':
    print(stats())
