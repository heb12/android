#include <string.h>
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <android/log.h>
#include "biblec/biblec.h"
#include "fbrp/reference.h"
#include "fbrp/fbrp.h"

struct Translation loadedTranslations[10];

JNIEXPORT jintArray JNICALL Java_com_heb12_android_MainActivity_arrayTest(JNIEnv* env, jobject thiz, jstring ref) {
	int tryFile = 0;
	parseIndexFile(
		&tryFile,
		&loadedTranslations[0],
		"/sdcard/heb12data/web.i",
		"/sdcard/heb12data/web.t"
	);

	const char *nativeRef = (*env)->GetStringUTFChars(env, ref, 0);

	int *tryRef;
	struct Reference reference;
	parseReference(tryRef, nativeRef, &reference);

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
