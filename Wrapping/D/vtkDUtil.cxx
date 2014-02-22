/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkDUtil.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifdef _INTEGRAL_MAX_BITS
#undef _INTEGRAL_MAX_BITS
#endif
#define _INTEGRAL_MAX_BITS 64

#include "vtkObject.h"
#include "vtkDebugLeaks.h"
#include "vtkWindows.h"

#include "vtkDUtil.h"

// Silence warning like
// "dereferencing type-punned pointer will break strict-aliasing rules"
// it happens because this kind of expression: (void **)(&e)
// pragma GCC diagnostic is available since gcc>=4.2
#if defined(__GNUC__) && (__GNUC__>4) || (__GNUC__==4 && __GNUC_MINOR__>=2)
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif


JNIEXPORT jlong vtkDGetId(JNIEnv *env,jobject obj)
{
  jfieldID id;
  jlong result;

  id = env->GetFieldID(env->GetObjectClass(obj),"vtkId","J");

  result = env->GetLongField(obj,id);
  return result;
}

JNIEXPORT void *vtkDGetPointerFromObject(JNIEnv *env, jobject obj)
{
  return obj ? (void*)(size_t)vtkDGetId(env, obj) : 0;
}

JNIEXPORT jarray vtkDMakeJArrayOfDoubleFromDouble(JNIEnv *env, double *ptr, int size)
{
  jdoubleArray ret;
  int i;
  jdouble *array;

  ret = env->NewDoubleArray(size);
  if (ret == 0)
    {
    // should throw an exception here
    return 0;
    }

  array = env->GetDoubleArrayElements(ret,NULL);

  // copy the data
  for (i = 0; i < size; i++)
    {
    array[i] = ptr[i];
    }

  env->ReleaseDoubleArrayElements(ret,array,0);
  return ret;
}

JNIEXPORT jarray vtkDMakeJArrayOfDoubleFromFloat(JNIEnv *env, float *ptr, int size)
{
  jdoubleArray ret;
  int i;
  jdouble *array;

  ret = env->NewDoubleArray(size);
  if (ret == 0)
    {
    // should throw an exception here
    return 0;
    }

  array = env->GetDoubleArrayElements(ret,NULL);

  // copy the data
  for (i = 0; i < size; i++)
    {
    array[i] = ptr[i];
    }

  env->ReleaseDoubleArrayElements(ret,array,0);
  return ret;
}

JNIEXPORT jarray vtkDMakeJArrayOfIntFromInt(JNIEnv *env, int *ptr, int size)
{
  jintArray ret;
  int i;
  jint *array;

  ret = env->NewIntArray(size);
  if (ret == 0)
    {
    // should throw an exception here
    return 0;
    }

  array = env->GetIntArrayElements(ret,NULL);

  // copy the data
  for (i = 0; i < size; i++)
    {
    array[i] = ptr[i];
    }

  env->ReleaseIntArrayElements(ret,array,0);
  return ret;
}

JNIEXPORT jarray vtkDMakeJArrayOfIntFromIdType(JNIEnv *env, vtkIdType *ptr, int size)
{
  jintArray ret;
  int i;
  jint *array;

  ret = env->NewIntArray(size);
  if (ret == 0)
    {
    // should throw an exception here
    return 0;
    }

  array = env->GetIntArrayElements(ret,NULL);

  // copy the data
  for (i = 0; i < size; i++)
    {
    array[i] = (int)ptr[i];
    }

  env->ReleaseIntArrayElements(ret,array,0);
  return ret;
}

#if defined(VTK_TYPE_USE_LONG_LONG)
JNIEXPORT jarray vtkDMakeJArrayOfIntFromLongLong(JNIEnv *env, long long *ptr, int size)
{
  jintArray ret;
  int i;
  jint *array;

  ret = env->NewIntArray(size);
  if (ret == 0)
    {
    // should throw an exception here
    return 0;
    }

  array = env->GetIntArrayElements(ret,NULL);

  // copy the data
  for (i = 0; i < size; i++)
    {
    array[i] = (int)ptr[i];
    }

  env->ReleaseIntArrayElements(ret,array,0);
  return ret;
}
#endif

#if defined(VTK_TYPE_USE___INT64)
JNIEXPORT jarray vtkDMakeJArrayOfIntFrom__Int64(JNIEnv *env, __int64 *ptr, int size)
{
  jintArray ret;
  int i;
  jint *array;

  ret = env->NewIntArray(size);
  if (ret == 0)
    {
    // should throw an exception here
    return 0;
    }

  array = env->GetIntArrayElements(ret,NULL);

  // copy the data
  for (i = 0; i < size; i++)
    {
    array[i] = (int)ptr[i];
    }

  env->ReleaseIntArrayElements(ret,array,0);
  return ret;
}
#endif

JNIEXPORT jarray vtkDMakeJArrayOfIntFromSignedChar(JNIEnv *env, signed char *ptr, int size)
{
  jintArray ret;
  int i;
  jint *array;

  ret = env->NewIntArray(size);
  if (ret == 0)
    {
    // should throw an exception here
    return 0;
    }

  array = env->GetIntArrayElements(ret,NULL);

  // copy the data
  for (i = 0; i < size; i++)
    {
    array[i] = (int)ptr[i];
    }

  env->ReleaseIntArrayElements(ret,array,0);
  return ret;
}

JNIEXPORT jarray vtkDMakeJArrayOfFloatFromFloat(JNIEnv *env, float *ptr, int size)
{
  jfloatArray ret;
  int i;
  jfloat *array;

  ret = env->NewFloatArray(size);
  if (ret == 0)
    {
    // should throw an exception here
    return 0;
    }

  array = env->GetFloatArrayElements(ret,NULL);

  // copy the data
  for (i = 0; i < size; i++)
    {
    array[i] = ptr[i];
    }

  env->ReleaseFloatArrayElements(ret,array,0);
  return ret;
}

JNIEXPORT jarray vtkDMakeJArrayOfShortFromShort(JNIEnv *env, short *ptr, int size)
{
  jshortArray ret;
  int i;
  jshort *array;

  ret = env->NewShortArray(size);
  if (ret == 0)
    {
    // should throw an exception here
    return 0;
    }

  array = env->GetShortArrayElements(ret,NULL);

  // copy the data
  for (i = 0; i < size; i++)
    {
    array[i] = ptr[i];
    }

  env->ReleaseShortArrayElements(ret,array,0);
  return ret;
}

JNIEXPORT jarray vtkDMakeJArrayOfByteFromUnsignedChar(JNIEnv *env, unsigned char *ptr, int size)
{
  jbyteArray ret;
  int i;
  jbyte *array;

  ret = env->NewByteArray(size);
  if (ret == 0)
    {
    // should throw an exception here
    return 0;
    }

  array = env->GetByteArrayElements(ret,NULL);

  // copy the data
  for (i = 0; i < size; i++)
    {
    array[i] = ptr[i];
    }

  env->ReleaseByteArrayElements(ret,array,0);
  return ret;
}

JNIEXPORT jarray vtkDMakeJArrayOfLongFromLong(JNIEnv *env, long *ptr, int size)
{
  cout.flush();
  jlongArray ret;
  int i;
  jlong *array;

  ret = env->NewLongArray(size);
  if (ret == 0)
    {
    // should throw an exception here
    return 0;
    }

  array = env->GetLongArrayElements(ret,NULL);

  // copy the data
  for (i = 0; i < size; i++)
    {
    array[i] = ptr[i];
    }

  env->ReleaseLongArrayElements(ret,array,0);
  return ret;
}

JNIEXPORT jarray vtkDMakeJArrayOfUnsignedLongFromUnsignedLong(JNIEnv *env,unsigned long *ptr,int size)
{
  cout.flush();
  jlongArray ret;
  int i;
  jlong *array;

  ret = env->NewLongArray(size);
  if (ret == 0)
    {
    // should throw an exception here
    return 0;
    }

  array = env->GetLongArrayElements(ret,NULL);

  // copy the data
  for (i = 0; i < size; i++)
    {
    array[i] = ptr[i];
    }

  env->ReleaseLongArrayElements(ret,array,0);
  return ret;
}

JNIEXPORT jarray vtkDMakeJArrayOfUnsignedShortFromUnsignedShort(JNIEnv *env,unsigned short *ptr,int size)
{
  cout.flush();
  jshortArray ret;
  int i;
  jshort *array;

  ret = env->NewShortArray(size);
  if (ret == 0)
    {
    // should throw an exception here
    return 0;
    }

  array = env->GetShortArrayElements(ret,NULL);

  // copy the data
  for (i = 0; i < size; i++)
    {
    array[i] = ptr[i];
    }

  env->ReleaseShortArrayElements(ret,array,0);
  return ret;
}

JNIEXPORT jarray vtkDMakeJArrayOfUnsignedCharFromUnsignedChar(JNIEnv *env,unsigned char *ptr,int size)
{
  cout.flush();
  jbyteArray ret;
  int i;
  jbyte *array;

  ret = env->NewByteArray(size);
  if (ret == 0)
    {
    // should throw an exception here
    return 0;
    }

  array = env->GetByteArrayElements(ret,NULL);

  // copy the data
  for (i = 0; i < size; i++)
    {
    array[i] = ptr[i];
    }

  env->ReleaseByteArrayElements(ret,array,0);
  return ret;
}

JNIEXPORT jarray vtkDMakeJArrayOfUnsignedIntFromUnsignedInt(JNIEnv *env,unsigned int *ptr,int size)
{
  cout.flush();
  jintArray ret;
  int i;
  jint *array;

  ret = env->NewIntArray(size);
  if (ret == 0)
    {
    // should throw an exception here
    return 0;
    }

  array = env->GetIntArrayElements(ret,NULL);

  // copy the data
  for (i = 0; i < size; i++)
    {
    array[i] = ptr[i];
    }

  env->ReleaseIntArrayElements(ret,array,0);
  return ret;
}

JNIEXPORT jarray vtkDMakeJArrayOfCharFromChar(JNIEnv *env, char *ptr, int size)
{
  cout.flush();
  jcharArray ret;
  int i;
  jchar *array;

  ret = env->NewCharArray(size);
  if (ret == 0)
    {
    // should throw an exception here
    return 0;
    }

  array = env->GetCharArrayElements(ret,NULL);

  // copy the data
  for (i = 0; i < size; i++)
    {
    array[i] = ptr[i];
    }

  env->ReleaseCharArrayElements(ret,array,0);
  return ret;
}

JNIEXPORT jarray vtkDMakeJArrayOfIntFromBool(JNIEnv *env, bool *ptr,int size)
{
  cout.flush();
  jintArray ret;
  int i;
  jint *array;

  ret = env->NewIntArray(size);
  if (ret == 0)
    {
    // should throw an exception here
    return 0;
    }

  array = env->GetIntArrayElements(ret,NULL);

  // copy the data
  for (i = 0; i < size; i++)
    {
    array[i] = ptr[i];
    }

  env->ReleaseIntArrayElements(ret,array,0);
  return ret;
}

// http://d.sun.com/docs/books/jni/html/pitfalls.html#12400
static void JNU_ThrowByName(JNIEnv *env, const char *name, const char *msg)
{
  jclass cls = env->FindClass(name);
  /* if cls is NULL, an exception has already been thrown */
  if (cls != NULL) {
    env->ThrowNew(cls, msg);
  }
  /* free the local ref */
  env->DeleteLocalRef(cls);
}

static char *JNU_GetStringNativeChars(JNIEnv *env, jstring jstr)
{
  if (jstr == NULL) {
    return NULL;
  }
  jbyteArray bytes = 0;
  jthrowable exc;
  char *result = 0;
  if (env->EnsureLocalCapacity(2) < 0) {
    return 0; /* out of memory error */
  }
  jclass Class_d_lang_String = env->FindClass("d/lang/String");
  jmethodID MID_String_getBytes = env->GetMethodID(
    Class_d_lang_String, "getBytes", "()[B");
  bytes = (jbyteArray) env->CallObjectMethod(jstr,
    MID_String_getBytes);
  exc = env->ExceptionOccurred();
  if (!exc) {
    jint len = env->GetArrayLength(bytes);
    result = new char [len + 1];

    if (result == 0) {
      JNU_ThrowByName(env, "d/lang/OutOfMemoryError",
        0);
      env->DeleteLocalRef(bytes);
      return 0;
    }
    env->GetByteArrayRegion(bytes, 0, len,
      (jbyte *)result);
    result[len] = 0; /* NULL-terminate */
  } else {
    env->DeleteLocalRef(exc);
  }
  env->DeleteLocalRef(bytes);
  return result;
}

JNIEXPORT char *vtkDUTFToChar(JNIEnv *env, jstring in)
{
  return JNU_GetStringNativeChars(env, in);
}

JNIEXPORT bool vtkDUTFToString(JNIEnv *env, jstring in, std::string &out)
{
  const char *cstring = JNU_GetStringNativeChars(env, in);
  if( cstring )
    {
    out = cstring;
    delete[] cstring;
    return true;
    }

  return false;
}

JNIEXPORT jstring vtkDMakeDString(JNIEnv *env, const char *in)
{
  if (!in) {
    return env->NewStringUTF("");
  } else {
    return env->NewStringUTF(in);
  }
}

//**jcp this is the callback inteface stub for D. no user parms are passed
//since the callback must be a method of a class. We make the rash assumption
//that the <this> pointer will anchor any required other elements for the
//called functions. - edited by km
JNIEXPORT void vtkDVoidFunc(void* f)
{
  vtkDVoidFuncArg *iprm = (vtkDVoidFuncArg *)f;
  // make sure we have a valid method ID
  if (iprm->mid)
    {
    JNIEnv *e;
    // it should already be atached
#ifdef JNI_VERSION_1_2
    iprm->vm->AttachCurrentThread((void **)(&e),NULL);
#else
    iprm->vm->AttachCurrentThread((JNIEnv_**)(&e),NULL);
#endif
    e->CallVoidMethod(iprm->uobj,iprm->mid,NULL);
    }
}

JNIEXPORT void vtkDVoidFuncArgDelete(void* arg)
{
  vtkDVoidFuncArg *arg2;

  arg2 = (vtkDVoidFuncArg *)arg;

  JNIEnv *e;
  // it should already be atached
#ifdef JNI_VERSION_1_2
  arg2->vm->AttachCurrentThread((void **)(&e),NULL);
#else
  arg2->vm->AttachCurrentThread((JNIEnv_**)(&e),NULL);
#endif
  // free the structure
  e->DeleteGlobalRef(arg2->uobj);
  delete arg2;
}

vtkDCommand::vtkDCommand()
{
  this->vm = NULL;
}

vtkDCommand::~vtkDCommand()
{
  JNIEnv *e;
  // it should already be atached
#ifdef JNI_VERSION_1_2
  this->vm->AttachCurrentThread((void **)(&e),NULL);
#else
  this->vm->AttachCurrentThread((JNIEnv_**)(&e),NULL);
#endif
  // free the structure
  e->DeleteGlobalRef(this->uobj);
}

void vtkDCommand::Execute(vtkObject *, unsigned long, void *)
{
  // make sure we have a valid method ID
  if (this->mid)
    {
    JNIEnv *e;
    // it should already be atached
#ifdef JNI_VERSION_1_2
    this->vm->AttachCurrentThread((void **)(&e),NULL);
#else
    this->vm->AttachCurrentThread((JNIEnv_**)(&e),NULL);
#endif
    e->CallVoidMethod(this->uobj,this->mid,NULL);
    }
}
