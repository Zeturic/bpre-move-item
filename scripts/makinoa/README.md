### Makinoa

Simple build system for C projects for Generation III Pokemon ROM Hacks.

Licensed under the Simplified BSD License.

#### How do I build a project with this?

* `Python 3.6` or later is required.
* Have a `DEVKITARM` environment variable pointing to the `devkitARM v45` installation directory (likely `C:\devkitPro\devkitARM`).
* Have an `ARMIPS` environment variable pointing to `armips.exe` (not the folder it's in).
* Rename your ROM `rom.gba` and place it in the project's root directory.
* Open a terminal in the project's root directory. On Windows, this can be done easily by navigating to the directory in Windows Explorer, clicking on the address bar, typing `cmd` and hitting enter.
* Run `python scripts/makinoa`. This will generate a modified copy of `rom.gba` in the form of `test.gba` and leave `rom.gba` unmodified.

#### How do I view/edit my environment variables?

I'm going to assume you're on Windows and using the Command Prompt. If you're on a *nix system or using Powershell, you're probably more than capable of manipulating them yourself.

First, let's double check that you actually need to do this.

Open the command prompt, and run `echo %DEVKITARM%`.

* If you see `%DEVKITARM%`, that means you don't have a `DEVKITARM` variable and will need to set it.
* If you see `/opt/devkitpro/devkitARM`, you're fine as long as your `devkitARM` installation directory is actually `C:\devkitPro\devkitARM`.
* If you see something else, but it is not the location of your `devkitARM v45` installation directory, then you'll need to change the environment variable.

Now let's repeat the test for `armips`. Run `echo %ARMIPS%`.

* If you see `%ARMIPS%`, that means you don't have a `ARMIPS` variable and will need to set it.
* If you see something else, but it is not the location of your `armips` executable, then you'll need to change the environment variable.

Note that `ARMIPS` needs to be the `armips` executable itself, not just the containing directory (i.e. it needs to end in `armips.exe`).

If your environment variables need to be changed, keep reading.

Hit the windows key, and start typing `edit environment variables`, and open the result. You should see two lists of environment variables; one for your user account and one for 
your system.

If the environment variable you need does not exist in the list of user-level environment variables, just create it using the `New` button. If it already exists, just use the `Edit` button instead.

User-level environment variables override system-level environment variables, so even if there is a wrong value for `ARMIPS` or `DEVKITARM` in the system-level environment variables, you can safely ignore it as long as it's correct in your user-level environment variables.

Any command prompts that were open during this process won't see your changes; make sure you close and reopen them. After doing that, I suggest rerunning the `echo %DEVKITARM%` and `echo %ARMIPS%` tests mentioned above to make sure everything is correct.