/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkDUtil.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef __vtkDUtil_h
#define __vtkDUtil_h

#include "vtkSystemIncludes.h"
#include <jni.h>
#include "vtkCommand.h"
#include "vtkWrappingDModule.h"

#include <string>

extern VTKWRAPPINGD_EXPORT jlong q(JNIEnv *env,jobject obj);

extern VTKWRAPPINGD_EXPORT void *vtkDGetPointerFromObject(JNIEnv *env,jobject obj);
extern VTKWRAPPINGD_EXPORT char *vtkDUTFToChar(JNIEnv *env, jstring in);
extern VTKWRAPPINGD_EXPORT bool vtkDUTFToString(JNIEnv *env, jstring in, std::string &out);
extern VTKWRAPPINGD_EXPORT jstring vtkDMakeDString(JNIEnv *env, const char *in);

extern VTKWRAPPINGD_EXPORT jarray vtkDMakeJArrayOfFloatFromFloat(JNIEnv *env,
             float *arr, int size);
extern VTKWRAPPINGD_EXPORT jarray vtkDMakeJArrayOfDoubleFromFloat(JNIEnv *env,
             float *arr, int size);
extern VTKWRAPPINGD_EXPORT jarray vtkDMakeJArrayOfDoubleFromDouble(JNIEnv *env,
              double *arr, int size);
extern VTKWRAPPINGD_EXPORT jarray vtkDMakeJArrayOfShortFromShort(JNIEnv *env, short *arr, int size);
extern VTKWRAPPINGD_EXPORT jarray vtkDMakeJArrayOfIntFromInt(JNIEnv *env, int *arr, int size);
extern VTKWRAPPINGD_EXPORT jarray vtkDMakeJArrayOfIntFromIdType(JNIEnv *env, vtkIdType *arr, int size);
#if defined(VTK_TYPE_USE_LONG_LONG)
extern VTKWRAPPINGD_EXPORT jarray vtkDMakeJArrayOfIntFromLongLong(JNIEnv *env, long long *arr, int size);
#endif
#if defined(VTK_TYPE_USE___INT64)
extern VTKWRAPPINGD_EXPORT jarray vtkDMakeJArrayOfIntFrom__Int64(JNIEnv *env, __int64 *arr, int size);
#endif
extern VTKWRAPPINGD_EXPORT jarray vtkDMakeJArrayOfIntFromSignedChar(JNIEnv *env, signed char *arr, int size);
extern VTKWRAPPINGD_EXPORT jarray vtkDMakeJArrayOfLongFromLong(JNIEnv *env, long *arr, int size);
extern VTKWRAPPINGD_EXPORT jarray vtkDMakeJArrayOfByteFromUnsignedChar(JNIEnv *env, unsigned char *arr, int size);
extern VTKWRAPPINGD_EXPORT jarray vtkDMakeJArrayOfByteFromChar(JNIEnv *env, char *arr, int size);
extern VTKWRAPPINGD_EXPORT jarray vtkDMakeJArrayOfCharFromChar(JNIEnv *env, char *arr, int size);
extern VTKWRAPPINGD_EXPORT jarray vtkDMakeJArrayOfUnsignedCharFromUnsignedChar(JNIEnv *env, unsigned char *arr, int size);
extern VTKWRAPPINGD_EXPORT jarray vtkDMakeJArrayOfUnsignedIntFromUnsignedInt(JNIEnv *env, unsigned int *arr, int size);
extern VTKWRAPPINGD_EXPORT jarray vtkDMakeJArrayOfUnsignedShortFromUnsignedShort(JNIEnv *env,unsigned short *ptr,int size);
extern VTKWRAPPINGD_EXPORT jarray vtkDMakeJArrayOfUnsignedLongFromUnsignedLong(JNIEnv *env, unsigned long *arr, int size);

// this is the void pointer parameter passed to the vtk callback routines on
// behalf of the D interface for callbacks.
struct vtkDVoidFuncArg
{
  DVM *vm;
  jobject  uobj;
  jmethodID mid;
} ;

extern VTKWRAPPINGD_EXPORT void vtkDVoidFunc(void *);
extern VTKWRAPPINGD_EXPORT void vtkDVoidFuncArgDelete(void *);

class VTKWRAPPINGD_EXPORT vtkDCommand : public vtkCommand
{
public:
  static vtkDCommand *New() { return new vtkDCommand; };

  void SetGlobalRef(jobject obj) { this->uobj = obj; };
  void SetMethodID(jmethodID id) { this->mid = id; };
  void AssignDVM(JNIEnv *env) { env->GetDVM(&(this->vm)); };

  void Execute(vtkObject *, unsigned long, void *);

  DVM *vm;
  jobject  uobj;
  jmethodID mid;
protected:
  vtkDCommand();
  ~vtkDCommand();
};

#endif
// VTK-HeaderTest-Exclude: vtkDUtil.h
