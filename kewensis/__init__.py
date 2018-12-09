import sys, re, os

class BadArchive(Exception):
    pass

class EOF(Exception):
    pass

class FileData():
    def __repr__(self):
        strings = []
        for attr in dir(self):
            if not attr.startswith("_"):
                strings.append(f"{attr}: {getattr(self, attr)}")
        return f'<{", ".join(strings)}>'

def possibly_empty_int(x, base=10):
    if x.strip():
        return int(x, base)
    else:
        return 0

def parse(archive):
    if archive.read(8) != b"!<arch>\x0A":
        raise BadArchive()

    data = []

    while True:
        try:
            datum = FileData()

            datum.filename = archive.read(16).strip()
            if not len(datum.filename):
                raise EOF()

            if datum.filename[:1] != b"/":
                datum.filename = datum.filename[:-1]
            elif datum.filename not in (b"/", b"//"):
                pos = archive.tell()
                archive.seek(extended_filenames_pos + int(datum.filename[1:]))

                characters = []
                while True:
                    character = archive.read(1)
                    if character in (b"/", b"\n"): break
                    characters.append(character)
                datum.filename = b"".join(characters)

                archive.seek(pos)

            datum.filename = datum.filename.decode("UTF-8")

            datum.lastmodified = possibly_empty_int(archive.read(12))
            datum.owner = possibly_empty_int(archive.read(6))
            datum.group = possibly_empty_int(archive.read(6))
            datum.mode = possibly_empty_int(archive.read(8), 8)
            datum.size = possibly_empty_int(archive.read(10))

            if archive.read(2) != b"\x60\x0A":
                raise BadArchive()

            if datum.filename == "//":
                extended_filenames_pos = archive.tell()
            datum.contents = archive.read(datum.size)

            data.append(datum)
        except EOF:
            break

    return data