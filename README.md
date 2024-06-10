# Project TOS
    This is a C++ OOP project using Qt to build a Tower of Savior game.

### Course
    Introduction to Computer Science 2 lectured by Prof. Yean-Ru Chen, in 2024 NCKUEE.

### Group 16
#### Member & Work Contribution
- **蔡承希**：
- **蔡辰鑫**：
- **黃士洵**：


How to Compile & Run (for Cygwin)
Compile:
In the game folder ( /game/ ) , use the following command :
$ qmake-qt5 “QT+=widgets” “QT+=coregui” “QT+=widgetsmultimedia”
$ make

Run:
Open second terminal, use the following command :
$ startxwin
$ ./bin/pulseaudio

then back to the initial terminal and input :
$ export DISPLAY=:0.0
$ ./Project2_Group16

How to Compile & Run (for Linux Bash)
In the game folder ( /game/ ) , use the following command :
$ make
$ ./Project2_Group16
