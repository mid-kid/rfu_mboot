#!/usr/bin/env python3

from sys import argv, stdin, stdout

stdout.write("__attribute__((section(\"%s\"))) " % argv[1])
stdout.write("static const unsigned char data[]={")
while True:
    byte = stdin.buffer.read(1)
    if not byte:
        break
    stdout.write("0x%02x," % byte[0])
stdout.write("};")
