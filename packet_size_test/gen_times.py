#!/usr/bin/env python3


def abstimes():
    with open("times.txt", "r") as f:
        data = f.readlines()
    with open("abs_times.txt", "w") as out:
        for line in data:
            h, m, s = line.strip().split(':')
            try:
                sec = int(h)*3600 + int(m)*60 + int(s)
                out.write(str(sec) + "\n")
            except ValueError:
                return

def tdiffs():
    with open("abs_times.txt", "r") as f:
        data = f.readlines()
    t = None
    with open("tdiffs.txt", "w") as out:
        for line in data:
            try:
                p = t
                t = int(line.strip())
            except ValueError:
                return
            if p is None:
                continue
            out.write(str(t-p) + "\n")

def main():
    abstimes()
    tdiffs()


if __name__ == '__main__':
    main()
