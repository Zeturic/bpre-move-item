.macro importm, filename, addr, size
    .import filename, addr & 0x1FFFFFF, size
.endmacro

.macro updatebrmptr, addr
    .if readu32("rom.gba", addr & 0x1FFFFFF) != bottom_right_menu_old
        .error "Are you sure about the old location?"
    .else
        .org addr
        .word bottom_right_menu_new
    .endif
.endmacro