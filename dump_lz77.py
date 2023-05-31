#!/usr/bin/env python3

from sys import stdin, stdout

header = int.from_bytes(stdin.buffer.read(4), 'little')
header_size = header >> 8

out = bytearray()
while len(out) < header_size:
    flags = stdin.buffer.read(1)[0]
    for x in range(8):
        if flags & (1 << (7 - x)):
            byte = stdin.buffer.read(1)[0]
            length = (byte >> 4) + 3
            offset = (byte & 0xf) << 8
            offset |= stdin.buffer.read(1)[0]
            for x in range(length):
                out.append(out[-1 - offset])
        else:
            out += stdin.buffer.read(1)
        if len(out) >= header_size:
            break

stdout.buffer.write(out)
