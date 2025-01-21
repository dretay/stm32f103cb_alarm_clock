# Generating images
- From https://wiki.ugfx.io/index.php/ROMFS you need to compile file2c from the repo before. Just compile it manually for OSX like this `gcc -Wall -o file2c file2c.c && cp file2c ../binaries/osx`
- Crop SVG in inkscape and export as png
- Resize to the appropriate size and color depth using imagemagick `convert -monochrome -negate -resize 96x96 piggy-bank.png piggy_96_96.bmp`
- Then then generate header image like this: `~/STM32CubeIDE/workspace_1.14.0/libraries/ugfx/tools/file2c/binaries/osx/file2c -dcs piggy_96_96.bmp ../../Core/Inc/img/piggy_96_96.h`