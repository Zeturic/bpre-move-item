#!/usr/bin/env python3

import os, sys, argparse, configparser, shutil, subprocess, fnmatch, itertools
import os.path
import kewensis
from orderedset import OrderedSet
from collections import OrderedDict

src = "src"
build = "build"
obj = "obj"

# switch cwd, if specified by cmd line args
cmdlineargparser = argparse.ArgumentParser(description="Compiles C code and inserts it into a ROM.")
cmdlineargparser.add_argument("root", default=".", nargs="?", help="The root directory of the project to compile and insert. Defaults to the cwd.")

os.chdir(cmdlineargparser.parse_args().root)

# parse config.ini, if it exists
iniparser = configparser.ConfigParser(allow_no_value=True)
iniparser.optionxform = str

try:
    with open("config.ini", "r", encoding="UTF-8") as ini:
        iniparser.read_file(ini, "config.ini")
except FileNotFoundError:
    # no ini file is fine
    pass

free_space = iniparser.get("main", "free-space", fallback="0x08800000")
optimization_level = iniparser.get("main", "optimization-level", fallback="-O2")
reserve = iniparser.get("main", "reserve", fallback="0")
static_objects = OrderedSet() if "static" not in iniparser else OrderedSet(iniparser["static"])
defines = OrderedDict() if "defines" not in iniparser else OrderedDict(iniparser["defines"])
libgcc = OrderedSet() if "libgcc" not in iniparser else OrderedSet(iniparser["libgcc"])

try:
    free_space = int(free_space, 16)
except ValueError:
    print(f"Error :: {free_space} is not a hexadecimal integer.")
    sys.exit(1)

try:
    reserve = int(reserve)
except ValueError:
    print(f"Error :: {reserve} is not a decimal integer.")
    sys.exit(1)

if optimization_level not in ("-O", "-O0", "-O1", "-O2", "-O3", "-Ofast", "-Og", "-Os"):
    print(f"Error :: {optimization_level} is not an understood optimization level.")
    sys.exit(1)

# clean build dirs
shutil.rmtree(build, ignore_errors=True)
shutil.rmtree(obj, ignore_errors=True)
os.mkdir(build)
os.mkdir(obj)

DEVKITARM = os.environ.get("DEVKITARM", "")

if not os.path.exists(DEVKITARM):
    DEVKITARM = os.path.join(r"C:/", "devkitPro", "devkitARM")
    if not os.path.exists(DEVKITARM):
        print("Error :: Can't find devkitARM.")
        sys.exit(1)

CC = os.path.join(DEVKITARM, "bin", "arm-none-eabi-gcc")
LD = os.path.join(DEVKITARM, "bin", "arm-none-eabi-ld")

CFLAGS = [
    optimization_level,
    "-Wall",
    "-Wextra",
    "-mthumb",
    "-mno-thumb-interwork",
    "-fno-inline",
    "-fno-builtin",
    "-std=c11",
    "-mcpu=arm7tdmi",
    "-march=armv4t",
    "-mtune=arm7tdmi",
    "-c",
    *itertools.chain.from_iterable(("-D", f"{name}{f'={val}' if val is not None else ''}") for (name,val) in defines.items())
]

LDFLAGS = [
    "--relocatable"
]

relocatable_objects = OrderedSet()

if os.path.exists(src):
    for srcfile in (path for path in os.listdir(src) if fnmatch.fnmatch(path, "*.c")):
        objfile = srcfile.replace(".c", ".o")

        exit_code = subprocess.run([
            CC,
            *CFLAGS,
            "-mno-long-calls" if srcfile in static_objects else "-mlong-calls",
            os.path.join(src, srcfile),
            "-o",
            os.path.join(obj, objfile)
        ]).returncode

        if exit_code != 0:
            print("Error :: Compilation failed.")
            sys.exit(exit_code)

        if srcfile not in static_objects:
            relocatable_objects.add(objfile)

relocatable = os.path.join(obj, "relocatable.o")

# ensures the file exists
with open(relocatable, "w"): pass

# grabs all the requested object files from libgcc.a
# and marks them for inclusion in relocatable.o
if libgcc:
    with open(os.path.join(DEVKITARM, "lib", "gcc", "arm-none-eabi", "5.3.0", "thumb", "libgcc.a"), "rb") as library:
        data = kewensis.parse(library)
    for datum in data:
        if datum.filename in libgcc:
            libgcc.remove(datum.filename)
            with open(os.path.join(obj, datum.filename), "wb") as objfile:
                objfile.write(datum.contents)
            relocatable_objects.add(datum.filename)
        if not libgcc:
            break

if relocatable_objects:
    exit_code = subprocess.run([
        LD,
        *LDFLAGS,
        *(os.path.join(obj, objfile) for objfile in relocatable_objects),
        "-o",
        relocatable
    ]).returncode

    if exit_code != 0:
        print("Error :: Linking failed.")
        sys.exit(exit_code)
        
def round_up_to_4(x):
    if x & 0x3 == 0:
        return x
    else:
        return round_up_to_4(x + 1)

offset_mask = 0x08000000
free_space |= offset_mask

needed_words = round_up_to_4(os.stat(relocatable).st_size + reserve) >> 2
free_space = round_up_to_4(free_space)

def find_needed_words(needed_words, free_space):
    if needed_words == 0:
        return 0
    
    with open("rom.gba", "rb") as rom:
        rom.seek(offset_mask ^ free_space)

        record, start = 0, None

        while record < needed_words:
            val = rom.read(4)

            if val == b"\xff\xff\xff\xff":
                if start is None:
                    if record == 0:
                        record = 1
                    
                    start = rom.tell() - 4
                else:
                    record += 1
            else:
                start = None

    return start ^ offset_mask

shutil.copy("rom.gba", "test.gba")

if "ARMIPS" in os.environ:
    ARMIPS = os.environ["ARMIPS"]
else:
    ARMIPS = shutil.which("armips")
    
    if ARMIPS is None:
        print("Error :: Can't find armips.")
        sys.exit(1)

with open("main.asm", "r", encoding="UTF-8") as instream:
    with open(os.path.join(build, "main.asm"), "w", encoding="UTF-8") as outstream:
        for line in instream:
            print(line, end="", file=outstream)
        print("\n// Beyond this point is autogenerated.", file=outstream)
        print(f".definelabel allocation, {find_needed_words(needed_words, free_space)}", file=outstream)
        for (name, val) in defines.items():
            print(f".definelabel {name}, {val if val is not None else 0}", file=outstream)

exit_code = subprocess.run([
    ARMIPS,
    "-sym",
    "test.sym",
    os.path.join(build, "main.asm"),
    "-equ",
    "allocation_size",
    f"{needed_words << 2}"
]).returncode

if exit_code != 0:
    print("Error :: Assembly failed.")
    sys.exit(exit_code)
