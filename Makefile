# this file is for the android building process
# don't try and use it for compiling GridSim

all: build

build:
	cp *.h android/jni/src
	cp *.c android/jni/src
	cd android/jni/src; ndk-build
	
release: build
	cd android; ant debug
	cp android/bin/SDLActivity-debug-unaligned.apk .
	
clean:
	find android/jni/src -type f ! -name 'iconv.h' ! -name 'Android.mk' ! -name 'Android_static.mk' -delete
	cd android/jni/src; ndk-build clean
	cd android; ant clean