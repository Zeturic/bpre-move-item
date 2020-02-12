import argparse, sys
from pathlib import Path

argparser = argparse.ArgumentParser(description="Locates free space inside a GBA ROM.")
argparser.add_argument("--rom", dest="ROM", required=True)
argparser.add_argument("--needed-bytes", dest="NEEDED_BYTES", required=True)
argparser.add_argument("--start-at", dest="START_AT", required=True)

args = argparser.parse_args()
args.NEEDED_BYTES = int(args.NEEDED_BYTES, 0)
args.START_AT = int(args.START_AT, 0) & 0x1FFFFFF

def round_up_to_4(x):
    if x & 0x3 == 0:
        return x
    else:
        return round_up_to_4(x + 1)

def find_needed_bytes(rom, needed_bytes, start_at):
    if needed_bytes == 0:
        return 0

    needed_words = round_up_to_4(needed_bytes) >> 2
    start_at = round_up_to_4(start_at)
    
    with open(rom, "rb") as rom:
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
                print(f"{Path(sys.argv[0]).name}: error: end of file reached before a suitable location was found", file=sys.stderr)
                sys.exit(1)
            else:
                record, start = 0, None

        # sanity check
        rom.seek(start)
        assert rom.read(round_up_to_4(needed_bytes)) == b"\xff\xff\xff\xff" * needed_words

    return start


addr = find_needed_bytes(rom=args.ROM, needed_bytes=args.NEEDED_BYTES, start_at=args.START_AT) | 0x08000000

print(f"0x{addr :08X}")
