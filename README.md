## Move Item

Black and White 2 introduced a feature where you can move items directly between two Pokémon in your party from within the party menu, rather than having to use the bag as middle man. This ports that feature to FR ROMs.

This README is still a work in progress and may contain errors.

### Prerequisites

#### Shell and basic tools

First, you have to be using a POSIX shell, such as `bash`; the Windows Command Prompt and Powershell will not work. If you are on Windows, I highly recommend using [WSL](https://docs.microsoft.com/en-us/windows/wsl/install-win10), though the MSYS2 that comes with an installation of devkitARM can also be used. If you're on Windows and want to use devkitARM's MSYS2, you can get the devkitARM Windows installer [here](https://github.com/devkitPro/installer/releases).

You'll need `git`, `cmake`, `python`, `nano`, and your distro's equivalent of `build-essential`. For Ubuntu, this means `sudo apt install build-essential git cmake python nano`; for MSYS2 it's `pacman -Sy base-devel git cmake python nano`.

#### devkitARM

If you're on Windows and using the bundled MSYS2, devkitARM comes preinstalled so skip this section.

You'll also need to install devkitARM. Installation instructions can be found [here](https://devkitpro.org/wiki/devkitPro_pacman#Installing_devkitPro_Pacman). For Ubuntu the following should work (skip the first command if it's not a WSL Ubuntu install):

```shell
$ sudo ln -s /proc/self/mounts /etc/mtab
$ wget https://github.com/devkitPro/pacman/releases/download/devkitpro-pacman-1.0.1/devkitpro-pacman.deb
$ sudo dpkg -i devkitpro-pacman.deb
$ dkp-pacman -Sy gba-dev
$ printf 'export DEVKITPRO=/opt/devkitpro\nexport DEVKITARM=${DEVKITPRO}/devkitARM\nexport PATH=${DEVKITPRO}/tools/bin:$PATH' >> ~/.bash_profile
$ source ~/.bash_profile
```

You can confirm everything installed properly by running `$DEVKITARM/bin/arm-none-eabi-gcc --version`. If you get version information, it's correct.

#### Armips

You will need to have [Armips](https://github.com/Kingcom/armips) available in the `PATH`. You should already have everything you need to build `armips`; you can install `armips` with:

```shell
$ cd ~
$ git clone https://github.com/Kingcom/armips.git
$ mkdir armips/build && cd armips/build
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ make
$ cp ./armips /usr/local/bin/armips
```

That last command may require root privileges (e.g. `sudo cp ./armips /usr/local/bin/armips`) depending on your machine. You can confirm that it's setup correctly by attempting to run `armips`; if you get usage info, it's set up correctly.

### Setup

#### Cloning the repo and building Preproc

Now we can /finally/ download and start configuring this repository.

If you're using WSL, your `C:` drive is mounted as `/mnt/c`. Future steps will require access to a text editor, so unless you're comfortable with a terminal text editor, you should clone the repo on your `C:` drive. For example, if you wanted to download this repo to `C:\Users\Nate\RomHacking`, you would do:

```shell
$ cd /mnt/c/Users/Nate/RomHacking
$ git clone https://github.com/Zeturic/mvitem.git
$ cd mvitem
$ make -C tools/preproc
```

#### Adding your ROM

Copy your ROM to this directory and rename it `rom.gba`.

In the above WSL example, you would navigate to `C:\Users\Nate\RomHacking\mvitem` in Windows Explorer and put your ROM there.

#### Repointing sCursorOptions

Every cursor option in the party menu (`SUMMARY`, `TAKE`, `CUT`, etc) has an associated entry in `sCursorOptions`. We're adding a new cursor option, `MOVE`, so we'll need to repoint `sCursorOptions` and add the new entry. However, we can't just repoint it the normal way. Repointing it and adding the entry for `MENU` at the end would cause it to treat `MENU` as a field move, causing some weird bugs.

Repointing it properly is somewhat involved, so `repoint-cursor-options.asm` is included to do it for you. You'll need to modify it slightly, however.

Open `repoint-cursor-options.asm` in the text editor of your choice.

Modify the definition of `free_space`. The table will be repointed to wherever you specify, so make sure it's actually free space in your ROM. Look for `0x100` bytes of free space from a word aligned address (read: an address ending in `0`, `4`, `8`, or `C`).

By default, FR has `12` field moves. If you've previously expanded them - for example, to add Rock Climb - update the definition of `NUM_FIELD_MOVE_CURSOR_OPTIONS`.

Once you're done modifying it, save it and return to the terminal and run:

```shell
$ make repoint-cursor-options
```

Your ROM will be modified in place, and it'll give you output describing what it did. It'll also give you a value for `MENU_MOVE_ITEM`, though it should always be `0x12`.

#### Building the project itself

Open `config.mk` in the text editor of your choice.

Update the definition of `START_AT`. Unlike in the previous step, the build system is smart enough to find enough free space on its own; `START_AT` is only there to tell it where to start looking. If the given address is acceptable (i.e. is word-aligned and has enough bytes of free space), it will be inserted there.

If you want to edit the strings involved in this feature - for example, to decapitalize `MOVE` to `Move` - they can be found and modified by opening `src/strings.c` in a text editor.

Once you're ready, run the following to build the project:

```shell
make
```

Also unlike the previous step, this won't actually modify `rom.gba`, instead your output will be in `test.gba`. Naturally, test it in an emulator.

### Credits

The project structure, `charmap.txt` and `Preproc` are all from `pokeemerald`.