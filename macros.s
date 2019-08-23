.macro window_template, bg, tilemap_left, tilemap_top, width, height, palette_num, base_block
    .byte bg, tilemap_left, tilemap_top, width, height, palette_num
    .halfword base_block
.endmacro

.macro safeupdateptr, addr, old, new
    .if readu32("rom.gba", addr & 0x1FFFFFF) != old
        .error "Value doesn't matched expected."
    .else
        .org addr
        .word new
    .endif
.endmacro

.macro incbinm, filename, addr, size
    .incbin filename, addr & 0x1FFFFFF, size
.endmacro