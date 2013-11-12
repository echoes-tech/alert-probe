gcc -g `pkg-config --cflags gobject-2.0 gio-2.0` main.c `pkg-config --libs gobject-2.0 gio-2.0` -o server
