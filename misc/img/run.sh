~/STM32CubeIDE/workspace_1.14.0/libraries/ugfx/tools/file2c/binaries/osx/file2c -dcs piggy_96_96.bmp ../../Core/Inc/img/piggy_96_96.h
convert -monochrome -negate -resize 96x96 piggy-bank.png piggy_96_96.bmp

convert -monochrome -negate -resize 48x48 drizzle.png drizzle_48_48.bmp
~/STM32CubeIDE/workspace_1.14.0/libraries/ugfx/tools/file2c/binaries/osx/file2c -dcs drizzle_48_48.bmp ../../Core/Inc/img/drizzle_48_48.h
