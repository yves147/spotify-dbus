cd ..
gcc spotifydbus.c \
	-I/usr/include/dbus-1.0/ \
	-I/usr/lib/x86_64-linux-gnu/dbus-1.0/include \
	-ldbus-1
#    `pkg-config --cflags dbus-glib-1` \
#    `pkg-config --cflags dbus-1` \
#    `pkg-config --cflags glib-2.0`
