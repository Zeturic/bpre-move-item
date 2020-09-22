#!/usr/bin/env python

from __future__ import print_function

import argparse, sys

def round_up_to_4(x):
    if x & 0x3 == 0:
        return x
    else:
        return round_up_to_4(x + 1)

def find_needed_bytes(*, rom, needed_bytes, start_at):
    if needed_bytes == 0:
        return 0

    needed_words = round_up_to_4(needed_bytes) >> 2
    start_at = round_up_to_4(start_at)

    rom.seek(start_at)

    record, start = 0, None

    while record < needed_words:
        val = rom.read(4)

        if val == b"\xff\xff\xff\xff":
            if start is None:
                assert record == 0
                record = 1

                start = rom.tell() - 4
            else:
                record += 1
        elif len(val) < 4:
            raise EOFError()
        else:
            record, start = 0, None

    # sanity check
    rom.seek(start)
    assert rom.read(round_up_to_4(needed_bytes)) == b"\xff\xff\xff\xff" * needed_words

    return start

def main():
    argparser = argparse.ArgumentParser(description="Locates free space inside a GBA ROM.")
    argparser.add_argument("--rom", dest="ROM", required=True)
    argparser.add_argument("--needed-bytes", dest="NEEDED_BYTES", required=True)
    argparser.add_argument("--start-at", dest="START_AT", required=True)

    args = argparser.parse_args()
    args.NEEDED_BYTES = int(args.NEEDED_BYTES, 0)
    args.START_AT = int(args.START_AT, 0) & 0x1FFFFFF

    try:
        with open(args.ROM, "rb") as rom:
            addr = find_needed_bytes(rom=rom, needed_bytes=args.NEEDED_BYTES, start_at=args.START_AT) | 0x08000000
            print("0x{0:08X}".format(addr))
    except EOFError:
        print("{}: error: end of file reached before a suitable location was found".format(argparser.prog), file=sys.stderr)
        return 1

if __name__ == "__main__":
    sys.exit(main())
