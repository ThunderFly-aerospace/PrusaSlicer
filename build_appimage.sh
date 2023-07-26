#!/bin/bash
set -e
set -v

cd deps
rm -rf build
mkdir build
cd build
cmake .. -DDEP_WX_GTK3=ON
make -j 12
cd ../..

rm -rf build
mkdir build
cd build
cmake .. -DSLIC3R_STATIC=1 -DSLIC3R_GTK=3 -DSLIC3R_PCH=OFF -DCMAKE_PREFIX_PATH=$(pwd)/../deps/build/destdir/usr/local -DCMAKE_INSTALL_PREFIX=/usr
make -j4
make install DESTDIR=AppImage

wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
chmod +x linuxdeploy-x86_64.AppImage
./linuxdeploy-x86_64.AppImage --appdir AppImage --desktop-file ../prusaslicer_mod.desktop -i ../resources/icons/PrusaSlicer_192px.png --output appimage
cp Prusa_Slicer_Modified-x86_64.AppImage ..
