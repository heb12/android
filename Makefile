# Gradle 8 probably needed to compile correctly
gflag := -Dorg.gradle.java.home=/usr/lib/jvm/java-8-openjdk-amd64

gflag += --info

all:
	export ANDROID_HOME=/usr/lib/android-sdk; sudo gradle build $(gflag)

adb:
	adb uninstall com.heb12.android
	adb install build/outputs/apk/debug/android-debug.apk
	adb shell monkey -p 'com.heb12.android' -v 1

clean:
	rm -rf .externalNativeBuild .gradle build

setup:
	touch local.properties
	echo 'sdk.dir=/usr/lib/android-sdk/' > local.properties
	echo 'ndk.dir=/usr/lib/android-ndk/' >> local.properties
	@echo 'Installing android stuff via apt. (Ctrl+c to stop)'
	@read

	sudo apt install android-sdk android-sdk-platform-23 
	sudo apt install google-android-ndk-installer
	sudo apt install openjdk-8-jdk
	sudo apt install libncurses5
	sudo apt install cmake

	@echo 'Accepting license..'
	git clone https://github.com/Shadowstyler/android-sdk-licenses.git
	sudo cp -a android-sdk-licenses/*-license /usr/lib/android-sdk/licenses 
	rm -rf android-sdk-licenses