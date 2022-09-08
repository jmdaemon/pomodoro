# Pomodoro

Pomodoro is a pomodoro timer program designed to help you manage your time, and study better.
There are 4 time intervals, each with a default of 25 minutes.
After each interval is a 10 minute break.

After four time intervals follows a longer 30 minute break.
After every break, the program is paused and the user can resume the timer manually after the break.

## Usage

``` bash
Starting Pomodoro Timer
Pomodoro #1
Working for 4 seconds
14:26 / 14:26 [##################################################]
Taking a short break for 0 minutes
Hit any key to resume the timer
Pomodoro #2
Working for 4 seconds
14:26 / 14:26 [##################################################]
Taking a short break for 0 minutes
Hit any key to resume the timer
Pomodoro #3
Working for 4 seconds
14:26 / 14:26 [##################################################]
Taking a short break for 0 minutes
Hit any key to resume the timer
Pomodoro #4
Working for 4 seconds
14:26 / 14:26 [##################################################]
Taking a longer break for 0 minutes
Pomodoro Interval Complete
```

## Build

### CMake

To build with CMake:

``` bash
cmake --preset gcc-release-unix-makefiles
cd build/cmake/gcc-release-unix-makefiles
make
```

Then to install pomodoro:

``` bash
cd build/cmake/gcc-release-unix-makefiles
cmake install .
make install
```

from the binary directory.

If you'd like to uninstall then run

``` bash
cmake uninstall .
sudo make uninstall
```

### Make

To build with pure make:

``` bash
make release bin
```

Then to install, run:

``` bash
sudo make install
```

from the root project directory.

Run

``` bash
sudo make uninstall
```

to uninstall pomodoro.

### Meson

To build with meson:

``` bash
meson build/meson/release --buildtype release
cd build/meson/release
meson compile
```

To install with meson:

``` bash
sudo meson install
```

To uninstall

``` bash
sudo meson uninstall
```
