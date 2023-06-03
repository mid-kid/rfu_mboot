#!/usr/bin/env python3

from sys import argv, stdin, stdout

stdout.write("__asm__(\"\n")
stdout.write(".section %s\n" % argv[1])
if argv[1] == ".text":
    stdout.write(".type data, function\n")
    stdout.write(".thumb_func\n")
else:
    stdout.write(".type data, object\n")
stdout.write("data:\n")

stdout.write(".byte ")
byte = stdin.buffer.read(1)
while byte:
    stdout.write("0x%02x" % byte[0])
    byte = stdin.buffer.read(1)
    if not byte:
        break
    stdout.write(",")
stdout.write("\n")

stdout.write(".size data, .-data\n")
stdout.write("\");")
