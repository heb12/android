#include <string.h>
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>

#include "biblec/biblec.h"
#include "fbrp/reference.h"
#include "fbrp/fbrp.h"

struct Translation loadedTranslations[10];

JNIEXPORT jintArray JNICALL
Java_com_heb12_android_MainActivity_cgetVerses(JNIEnv* env, jobject thiz, jstring ref) {
	const char *nativeRef = (*env)->GetStringUTFChars(env, ref, 0);

	int *tryRef;
	struct Reference reference;
	parseReference(tryRef, nativeRef, strlen(nativeRef), &reference);

	int to = 1;
	if (reference.verse[0].r[0] != reference.verse[0].r[1]) {
		to = reference.verse[0].r[1] - reference.verse[0].r[0] + 1;
	}

	char result[30][600];
	int tryStruct;
	getVerses(
		&tryStruct,
		result,
		loadedTranslations[0],
		reference.book,
		reference.chapter[0].r[0],
		reference.verse[0].r[0],
		to
	);

	// Convert native array into java array
	jobjectArray array;
	array = (*env)->NewObjectArray(
		env,
		to,
		(*env)->FindClass(env, "java/lang/String"),
		(*env)->NewStringUTF(env, "")
	 );

	for (int i = 0; i < to; i++) {
		(*env)->SetObjectArrayElement(
			env,
			array,
			i,
			(*env)->NewStringUTF(env, result[i])
		);
	}

	return array;
}

JNIEXPORT jstring JNICALL
Java_com_heb12_android_MainActivity_cloadTranslation(JNIEnv* env, jobject thiz, jstring indexLocation, jstring textLocation) {
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
		(*env)->ThrowNew(env, error, "Error");
	}


	// Why? Because Java.
	return (*env)->NewStringUTF(env, "Void not working. jstring does.");
}
