## Move Item

Black and White 2 introduced a feature where you can move items directly between two Pokémon in your party from within the party menu, rather than having to use the bag as middle man. This ports that feature to FR ROMs.

This README is still a work in progress and may contain errors.

### Prerequisites

#### Shell and basic tools

First, you have to be using a POSIX shell, such as `bash`; the Windows Command Prompt and Powershell will not work. If you are on Windows, I highly recommend using [WSL](https://docs.microsoft.com/en-us/windows/wsl/install-win10), though the MSYS2 that comes with an installation of devkitARM can also be used. If you're on Windows and want to use devkitARM's MSYS2, you can get the devkitARM Windows installer [here](https://github.com/devkitPro/installer/releases).

You'll need `git`, `cmake`, `python`, and your distro's equivalent of `build-essential`.

For Ubuntu, this means running:

```shell
$ sudo apt update
$ sudo apt install build-essential git cmake python
```

For MSYS2 it's:

```shell
$ pacman -Sy base-devel git cmake python
```

#### devkitARM

If you're on Windows and using the bundled MSYS2, devkitARM comes preinstalled so you should skip this section.

You'll also need to install devkitARM. Installation instructions can be found [here](https://devkitpro.org/wiki/devkitPro_pacman#Installing_devkitPro_Pacman). For Ubuntu the following should work:

```shell
$ wget https://github.com/devkitPro/pacman/releases/download/devkitpro-pacman-1.0.1/devkitpro-pacman.deb
$ sudo dpkg -i devkitpro-pacman.deb
$ sudo dkp-pacman -Sy gba-dev
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

That last command may require root privileges (e.g. `sudo cp ./armips /usr/local/bin/armips`) depending on your machine. You can confirm that it's set up correctly by attempting to run `armips`; if you get usage info, it's set up correctly.

### Setup

#### Cloning the repo and building Preproc

Now we can /finally/ download and start configuring this repository.

In your terminal, navigate to whatever folder you want to download this repo into. If you're using WSL, you can navigate to the folder using Windows Explorer, and then `shift + right click` and select `Open Linux shell here`.

Then, do the following to download the repo and build Preproc:

```shell
$ git clone https://github.com/Zeturic/mvitem.git
$ cd mvitem
$ make -C tools/preproc
```

#### Adding your ROM

Copy your ROM to this directory and rename it `rom.gba`.

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

Unlike in the previous step, the build system is smart enough to find enough free space on its own.

However, if you want the code to be inserted at a particular address, you can specify it by updating the definition of `START_AT` in `config.mk`; if the given address is acceptable (i.e. is word-aligned and has enough bytes of free space), it will be inserted there. Otherwise, it will just use `START_AT` to determine where in the ROM it should start looking for free space.

If you want to edit the strings involved in this feature - for example, to decapitalize `MOVE` to `Move` - they can be found and modified in `src/strings.c`.

Once you're ready, run the following to build the project:

```shell
make
```

Also unlike the previous step, this won't actually modify `rom.gba`, instead your output will be in `test.gba`. Naturally, test it in an emulator.

### Credits

The project structure, `charmap.txt` and `Preproc` are all from [pokeemerald](https://github.com/pret/pokeemerald).