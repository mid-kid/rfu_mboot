#!/usr/bin/env python3

from sys import argv, stdin, stdout

section = ".text"
if len(argv) > 1:
    section = argv[1]

name = "data"
if len(argv) > 2:
    name = argv[2]

stdout.write("#if 1\n")
stdout.write("__asm__(\"\n")
stdout.write(".section %s\n" % section)
if name != "data":
    stdout.write("@.global %s\n" % name)
if section == ".text":
    stdout.write(".type %s, function\n" % name)
    stdout.write(".thumb_func\n")
else:
    stdout.write(".type %s, object\n" % name)
stdout.write("%s:\n" % name)

stdout.write(".2byte ")
word = stdin.buffer.read(2)
while word:
    stdout.write("0x%02x%02x" % (word[1], word[0]))
    word = stdin.buffer.read(2)
    if not word:
        break
    stdout.write(",")
stdout.write("\n")

stdout.write(".size %s, .-%s\n" % (name, name))
stdout.write("\");\n")
stdout.write("#else\n")
stdout.write("#endif\n")
