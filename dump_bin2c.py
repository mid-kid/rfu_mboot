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

stdout.write(".2byte ")
word = stdin.buffer.read(2)
while word:
    stdout.write("0x%02x%02x" % (word[1], word[0]))
    word = stdin.buffer.read(2)
    if not word:
        break
    stdout.write(",")
stdout.write("\n")

stdout.write(".size data, .-data\n")
stdout.write("\");")
