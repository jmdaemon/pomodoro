# Pomodoro

Pomodoro is a pomodoro timer program designed to help you manage your time, and study better.
There are 4 time intervals, each with a default of 25 minutes.
After each interval is a 10 minute break.

After four time intervals follows a longer 30 minute break.
After every break, the program is paused and the user can resume the timer manually after the break.

## Usage

``` bash
$ pomodoro
Starting Pomodoro Timer
Pomodoro #1
Hit any key to resume the timer
Pomodoro #2
Hit any key to resume the timer
Pomodoro #3
Hit any key to resume the timer
Pomodoro #4
Pomodoro Interval Complete
```

## Building

Run `make release` to build the binary.
The binary will be generated inside `build/release/bin/bytesize`.

To run a clean build execute: `make clean` to remove the build artifacts.

## Installation

Run `sudo make install` to install the program to `/usr/local/bin/`.
If you want to change installation directory, run `PREFIX=/usr/bin/ make install`.

To uninstall run `sudo make uninstall`.

