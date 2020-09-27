#include <string.h>
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>

#include "biblec/biblec.h"
#include "biblec/main.h"
#include "fbrp/reference.h"
#include "fbrp/fbrp.h"

struct Translation loadedTranslations[10];

// So java wants massive function names. Why. Because Java.
JNIEXPORT jintArray JNICALL
Java_com_heb12_android_MainActivity_cgetVerses
(JNIEnv* env, jobject thiz, jstring ref) {
	const char *nativeRef = (*env)->GetStringUTFChars(env, ref, 0);

	int *tryRef;
	struct Reference reference = parseReference(tryRef, nativeRef);

	int verseStart = 1;
	int verseEnd = 0;
	if (reference.verseLength != 0) {
		verseStart = reference.verse[0].r[0];
		verseEnd = reference.verse[0].r[1];
	}

	int tryReader;
	struct Reader reader = reader_new(
		&tryReader,
		&loadedTranslations[0],
		reference.book,
		reference.chapter[0].r[0],
		verseStart,
		verseEnd
	);

	if (tryReader) {
		jclass error = (*env)->FindClass(env, "java/lang/IllegalArgumentException");
		(*env)->ThrowNew(env, error, "Error getting verse.");
		return 0;
	}

	// Create a Java array
	jobjectArray array;
	array = (*env)->NewObjectArray(
		env,
		reader.to + 1,
		(*env)->FindClass(env, "java/lang/String"),
		(*env)->NewStringUTF(env, "")
	);

	// Insert verses into Java array
	int i = 0;
	while (1) {
		if (reader_next(&reader)) {break;}
		(*env)->SetObjectArrayElement(
			env,
			array,
			i,
			(*env)->NewStringUTF(env, reader.result)
		);
		i++;
	}

	fclose(reader.file);

	return array;
}

JNIEXPORT jstring JNICALL
Java_com_heb12_android_MainActivity_cloadTranslation
(JNIEnv* env, jobject thiz, jstring indexLocation, jstring textLocation) {
	const char *nindexLocation = (*env)->GetStringUTFChars(env, indexLocation, 0);
	const char *ntextLocation = (*env)->GetStringUTFChars(env, textLocation, 0);

	int fileError = 0;
	parseIndexFile(
		&fileError,
		&loadedTranslations[0],
		nindexLocation,
		ntextLocation
	);

	(*env)->ReleaseStringUTFChars(env, indexLocation, nindexLocation);
	(*env)->ReleaseStringUTFChars(env, textLocation, ntextLocation);

	if (fileError) {
		//__android_log_write(ANDROID_LOG_ERROR, "Heb12", "Error");
		jclass error = (*env)->FindClass(env, "java/lang/IllegalArgumentException");
		(*env)->ThrowNew(env, error, "Error getting verses.");
	}

	// Why? Because Java.
	return (*env)->NewStringUTF(env, "Void not working. jstring does.");
}
