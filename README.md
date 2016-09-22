Test Marlin lcdprint
====================

This is a project to test the new language engine for Marlin.
The purpose is to handle the UTF-8 strings to support HD44780 and
U8GLIB in a embedded system.

The original project is here: https://github.com/yhfudev/u8glib-fontutf8.git

The font tool you need is in the latest Marlin: buildroot/share/fonts/*
or you can get from the pull request: https://github.com/MarlinFirmware/Marlin/pull/4704



How to compile and update the font data
---------------------------------------

If you changed the UTF-8 strings bracked by _UxGT() in the project,
you need to run the script genallfont.sh.

So you'd best to put the source folder in Marlin source folder,
to access the required font tools.


Try the local docker if travis meet error:
https://docs.travis-ci.com/user/common-build-problems/#Troubleshooting-Locally-in-a-Docker-Image

