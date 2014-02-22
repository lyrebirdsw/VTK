find_package(D REQUIRED)
find_package(JNI REQUIRED)

include_directories(${D_INCLUDE_PATH} ${D_INCLUDE_PATH2})

# Add the option to package VTK for custom D packaging
option(VTK_D_INSTALL "Use the D rules to build the native libraries." OFF)
if(VTK_D_INSTALL)
  set(VTK_CUSTOM_LIBRARY_SUFFIX "" CACHE STRING "" FORCE)
  mark_as_advanced(VTK_CUSTOM_LIBRARY_SUFFIX)
  if(APPLE)
     set(CMAKE_INSTALL_NAME_DIR @loader_path CACHE STRING "" FORCE)
     mark_as_advanced(CMAKE_INSTALL_NAME_DIR)
  endif()
  if(NOT MAVEN_LOCAL_NATIVE_NAME)
    set(MAVEN_LOCAL_NATIVE_NAME "${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}" CACHE STRING "")
  endif()
  if(NOT MAVEN_NATIVE_ARTIFACTS)
    set(MAVEN_NATIVE_ARTIFACTS "${MAVEN_LOCAL_NATIVE_NAME}" CACHE STRING "")
  endif()
  if(NOT MAVEN_LOCAL_REPOSITORY)
    set(MAVEN_LOCAL_REPOSITORY "\${user.home}/vtk-maven-repos" CACHE STRING "")
  endif()
  if(NOT MAVEN_VTK_GROUP_ID)
    set(MAVEN_VTK_GROUP_ID "kitware.community" CACHE STRING "")
  endif()
  mark_as_advanced(MAVEN_LOCAL_REPOSITORY MAVEN_NATIVE_ARTIFACTS MAVEN_LOCAL_NATIVE_NAME MAVEN_VTK_GROUP_ID)
  set(MAVEN_DEPENDENCY_XML)
endif()

# Add the option for build the SWT component to VTK.
option(VTK_D_SWT_COMPONENT "Should SWT component for D be built (it requires Eclipse)?" OFF)

if(VTK_D_SWT_COMPONENT)
  find_package(SWT REQUIRED)
endif()

# Add the option for build the JOGL component to VTK.
set(JOGL_VERSION "2.0.2")
option(VTK_D_JOGL_COMPONENT "Need JOGL jar files" OFF)

if(VTK_D_JOGL_COMPONENT)
  set(MAVEN_REPO "$ENV{HOME}/.m2/repository")
  find_file(JOGL_LIB jogl-all-${JOGL_VERSION}.jar PATHS ${MAVEN_REPO}/org/jogamp/jogl/jogl-all/${JOGL_VERSION} )
  find_file(JOGL_GLUE gluegen-rt-${JOGL_VERSION}.jar PATHS ${MAVEN_REPO}/org/jogamp/gluegen/gluegen-rt/${JOGL_VERSION})
  mark_as_advanced(JOGL_LIB JOGL_GLUE)
  if(NOT JOGL_LIB)
     message(ERROR "
     CMake can not build VTK with VTK_D_JOGL_COMPONENT ON if the JOGL jar file is not provided.
     Please set the JOGL_LIB variable.
     ")
  endif()
  if(NOT JOGL_GLUE)
     message(ERROR "
     CMake can not build VTK with VTK_D_JOGL_COMPONENT ON if the GLUE jar file is not provided.
     Please set the JOGL_GLUE variable.
     ")
  endif()
endif()

set(WrappingD_SRCS
  vtkDUtil.cxx
  )

vtk_module_library(vtkWrappingD ${WrappingD_SRCS})

# dc runs out memory on Mac so increase heapsize
if(APPLE)
  set(DC_OPTIONS -J-Xmx512m)
endif()

foreach(module ${VTK_MODULES_ENABLED})
  if(NOT ${module}_EXCLUDE_FROM_WRAPPING)
    include(${${module}_BINARY_DIR}/DDependencies.cmake OPTIONAL)
    list(APPEND _MODULES_TO_WRAP ${module})
  endif()
endforeach()

if(NOT VTK_JAR_PATH)
  set(VTK_JAR_PATH "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
endif()

set(VTK_D_SOURCE_FILES ${VTK_D_DEPENDENCIES})
string(REGEX REPLACE "\\.d" ".class;" VTK_D_CLASS_FILES ${VTK_D_SOURCE_FILES})

# Setup custom targets
if (BUILD_TESTING)
  add_custom_target(VTKDTestClasses ALL DEPENDS ${VTK_BINARY_DIR}/d/dc_tests_stamp.txt)
endif()
add_custom_target(VTKDJar ALL DEPENDS ${VTK_JAR_PATH}/vtk.jar)
add_custom_target(VTKDClasses ALL DEPENDS ${VTK_BINARY_DIR}/d/dc_stamp.txt)

add_dependencies(VTKDJar VTKDClasses)
if (BUILD_TESTING)
  add_dependencies(VTKDTestClasses VTKDJar)
endif()
# Some platforms have libjvm in a subdirectory from
# the rest of the d libraries and are missing the symlink.

if(D_AWT_LIBRARY)
  foreach(__d_library ${D_AWT_LIBRARY})
    get_filename_component(D_LIB_DIR ${__d_library} PATH)
    if(EXISTS ${D_LIB_DIR}/xawt)
      link_directories(${D_LIB_DIR}/xawt)
    endif()
    if(EXISTS ${D_LIB_DIR}/client)
       link_directories(${D_LIB_DIR}/client)
    endif()
    if(EXISTS ${D_LIB_DIR}/server)
      link_directories(${D_LIB_DIR}/server)
    endif()
  endforeach()
endif()

set(CMAKE_SKIP_RPATH 0)
include_directories(${vtkCommonCore_SOURCE_DIR})
include_directories(${vtkCommonCore_BINARY_DIR})
add_executable(VTKDExecutable VTKD.cxx)
set(D_LIBRARIES)

foreach(module ${_MODULES_TO_WRAP})

  string(REGEX REPLACE "^vtk" "" _module_no_prefix "${module}")

  if(APPLE AND NOT VTK_D_INSTALL)
    set(src ${VTK_JAR_PATH}/libvtk${_module_no_prefix}D.dylib)
    set(tgt ${VTK_JAR_PATH}/libvtk${_module_no_prefix}D.jnilib)
    add_custom_command(OUTPUT ${tgt}
                       COMMAND ln
                       ARGS -sf ${src} ${tgt}
                       DEPENDS ${src})
    set(D_LIBRARIES ${D_LIBRARIES} ${tgt})
  endif()

  target_link_libraries(VTKDExecutable ${module}D)

  add_custom_target(VTKD${_module_no_prefix} ALL)
  add_dependencies(VTKD${_module_no_prefix} VTKDExecutable)
  add_dependencies(VTKD${_module_no_prefix} ${module}DDClasses)
  add_dependencies(VTKDJar VTKD${_module_no_prefix})
  # Make sure all .d files are build before any .class files are built.
  foreach(otherModule ${_MODULES_TO_WRAP})
    add_dependencies(VTKD${_module_no_prefix} ${otherModule}DDClasses)
  endforeach()

  # Make sure all the VTKD${kit} projects build *before* VTKDClasses.
  add_dependencies(VTKDClasses VTKD${_module_no_prefix})
  add_dependencies(VTKDClasses ${module}D)
endforeach()

# KITS is used in vtkSettings.d.in ( module name no prefix )
string(REGEX REPLACE "vtk" "" KITS "${_MODULES_TO_WRAP}")
configure_file(${VTK_SOURCE_DIR}/Wrapping/D/vtk/vtkSettings.d.in
               ${VTK_BINARY_DIR}/d/vtk/vtkSettings.d)

# configure vtkNativeLibrary.d to enable loading of VTK module libraries
set(VTK_D_NATIVE_LIBRARIES)
foreach(module ${VTK_MODULES_ALL})
  if(NOT ${module}_EXCLUDE_FROM_WRAPPING)
    string(TOUPPER ${module} _enum_name)
    set(_enabled "false")
    if(${${module}_ENABLED})
      set(_enabled "true")
    endif()
    set(_enum_contr_call "    ${_enum_name}(\"${module}D\", ${_enabled}),\n")
    set(VTK_D_NATIVE_LIBRARIES "${VTK_D_NATIVE_LIBRARIES}${_enum_contr_call}")
  endif()
endforeach()

# replace last , with ;
string(REGEX REPLACE "^(.*),\n$" "\\1;\n" VTK_D_NATIVE_LIBRARIES ${VTK_D_NATIVE_LIBRARIES})

configure_file(${VTK_SOURCE_DIR}/Wrapping/D/vtk/vtkNativeLibrary.d.in
               ${VTK_BINARY_DIR}/d/vtk/vtkNativeLibrary.d)

# Add to class list so classes get cleaned up
list(APPEND VTK_D_CLASS_FILES ${VTK_BINARY_DIR}/d/vtk/vtkSettings.class)

set(VTK_D_DEPENDENCIES
  CellType
  vtkReferenceInformation
  vtkDGarbageCollector
  vtkDMemoryManager
  vtkDMemoryManagerImpl
  vtkNativeLibrary
  vtkSettings
  )

set(VTK_D_SAMPLE_DEPENDENCIES)

if(vtkRenderingCore_ENABLED)
  if (BUILD_TESTING)
  set(VTK_D_DEPENDENCIES ${VTK_D_DEPENDENCIES}
    vtkDTesting)
  endif()
  set(VTK_D_DEPENDENCIES ${VTK_D_DEPENDENCIES}
    AxesActor
    vtkRenderWindowPanel
    )
endif()

if(vtkInteractionWidgets_ENABLED)
  set(VTK_D_DEPENDENCIES ${VTK_D_DEPENDENCIES}
    vtkCanvas
    )
endif()

if(vtkRenderingCore_ENABLED AND vtkIOImage_ENABLED)
  set(VTK_D_DEPENDENCIES ${VTK_D_DEPENDENCIES}
    vtkPanel

    rendering/vtkAbstractComponent
    rendering/vtkComponent
    rendering/vtkEventInterceptor
    rendering/vtkInteractorForwarder
    rendering/awt/vtkAwtComponent
    rendering/awt/vtkInternalAwtComponent
    )
  if(VTK_D_SWT_COMPONENT)
    set(VTK_D_DEPENDENCIES ${VTK_D_DEPENDENCIES}
      rendering/swt/vtkSwtComponent
      rendering/swt/vtkInternalSwtComponent
      rendering/swt/vtkSwtInteractorForwarderDecorator
    )
  endif()
  if(VTK_D_JOGL_COMPONENT)
    set(VTK_D_DEPENDENCIES ${VTK_D_DEPENDENCIES}
      rendering/jogl/vtkAbstractJoglComponent
      rendering/jogl/vtkJoglCanvasComponent
      rendering/jogl/vtkJoglPanelComponent
      sample/rendering/JoglConeRendering
    )
  endif()
  set(VTK_D_SAMPLE_DEPENDENCIES ${VTK_D_SAMPLE_DEPENDENCIES}
    InternalFrames
    Demo
    SimpleVTK
    VTKCanvas
    ReleaseVtkPanel
    rendering/AwtConeRendering
    rendering/annotation/LabeledCubeAxesActor
    )
  if(VTK_D_SWT_COMPONENT)
    set(VTK_D_SAMPLE_DEPENDENCIES ${VTK_D_SAMPLE_DEPENDENCIES}
      rendering/SwtConeRendering
    )
  endif()
endif()

if(vtkFiltersModeling_ENABLED AND vtkIOImage_ENABLED)
  set(VTK_D_SAMPLE_DEPENDENCIES ${VTK_D_SAMPLE_DEPENDENCIES}
    ImagePlaneWidget
    )
endif()

# Copy over the sample D files
foreach(jfile ${VTK_D_SAMPLE_DEPENDENCIES})
  set(src ${VTK_SOURCE_DIR}/Wrapping/D/vtk/sample/${jfile}.d)
  set(dst ${VTK_D_HOME}/sample/${jfile}.d)
  list(APPEND VTK_D_SOURCE_FILES ${dst})
  set(class ${VTK_D_HOME}/sample/${jfile}.class)
  # Add to class list so classes get cleaned up
  list(APPEND VTK_D_CLASS_FILES ${class})
  configure_file(${src} ${dst} COPYONLY)
endforeach()

# Copy over the handcrafted D classes
foreach(jfile ${VTK_D_DEPENDENCIES})
  set(src ${VTK_SOURCE_DIR}/Wrapping/D/vtk/${jfile}.d)
  set(dst ${VTK_D_HOME}/${jfile}.d)
  list(APPEND VTK_D_SOURCE_FILES ${dst})
  set(class ${VTK_D_HOME}/${jfile}.class)
  # Add to class list so classes get cleaned up
  list(APPEND VTK_D_CLASS_FILES ${class})
  if(jfile MATCHES "vtkSettings" OR jfile MATCHES "vtkNativeLibrary")
  else()
    configure_file(${src} ${dst} COPYONLY)
  endif()
endforeach()

if(WIN32)
  set(SEPARATOR "\\;")
else()
  set(SEPARATOR ":")
endif()

if(VTK_D_SWT_COMPONENT)
  set(SWT_FILES "${VTK_BINARY_DIR}/d/vtk/rendering/swt/*.d")
endif()

if(VTK_D_JOGL_COMPONENT)
  set(JOGL_FILES "${VTK_BINARY_DIR}/d/vtk/rendering/jogl/*.d" "${VTK_BINARY_DIR}/d/vtk/sample/rendering/JoglConeRendering.d")
endif()

# Handle D class path
set(D_COMPILATION_CLASSPATH "${VTK_D_HOME}/..")
if(ECLIPSE_SWT_LIBRARIES)
    set(D_COMPILATION_CLASSPATH "${D_COMPILATION_CLASSPATH}${SEPARATOR}${ECLIPSE_SWT_LIBRARIES}")
endif()
if(JOGL_LIB)
    set(D_COMPILATION_CLASSPATH "${D_COMPILATION_CLASSPATH}${SEPARATOR}${JOGL_LIB}")
endif()
if(JOGL_GLUE)
    set(D_COMPILATION_CLASSPATH "${D_COMPILATION_CLASSPATH}${SEPARATOR}${JOGL_GLUE}")
endif()

# Compile the D classes
add_custom_command(
  OUTPUT ${VTK_BINARY_DIR}/d/dc_stamp.txt
  DEPENDS ${VTK_D_SOURCE_FILES}
  COMMAND ${D_COMPILE} ${DC_OPTIONS}
    -source 1.5 -classpath ${D_COMPILATION_CLASSPATH} -sourcepath ${VTK_SOURCE_DIR}/Wrapping/D/ -d ${VTK_BINARY_DIR}/d
    ${VTK_BINARY_DIR}/d/vtk/*.d ${VTK_BINARY_DIR}/d/vtk/rendering/*.d ${VTK_BINARY_DIR}/d/vtk/rendering/awt/*.d ${VTK_BINARY_DIR}/d/vtk/sample/*.d ${SWT_FILES} ${JOGL_FILES}
  COMMAND ${CMAKE_COMMAND} -E touch ${VTK_BINARY_DIR}/d/dc_stamp.txt
  COMMENT "Compiling D Classes"
  )

# Create the JAR file containing all compiled classes and
add_custom_command(
  COMMAND ${D_ARCHIVE} -cvf  "${VTK_JAR_PATH}/vtk.jar"
  -C ${VTK_BINARY_DIR}/d
  vtk
  DEPENDS ${VTK_BINARY_DIR}/d/dc_stamp.txt ${D_LIBRARIES}
  OUTPUT ${VTK_JAR_PATH}/vtk.jar
  COMMENT "D Archive"
  )

if(BUILD_TESTING)
  foreach(jfile
    Regression
    ConcurrencyGC
    DDelete
    ManualGC
    DGCAndDelete)
    list(APPEND VTK_D_TESTS ${VTK_SOURCE_DIR}/Wrapping/D/vtk/test/${jfile}.d)
    # Add to class list so classes get cleaned up
    list(APPEND VTK_D_CLASS_FILES ${VTK_BINARY_DIR}/Wrapping/D/vtk/test/${jfile}.class)
  endforeach()

  # Compile D Tests
  add_custom_command(
    OUTPUT ${VTK_BINARY_DIR}/d/dc_tests_stamp.txt
    COMMAND ${D_COMPILE} ${DC_OPTIONS} -classpath ${VTK_JAR_PATH}/vtk.jar${SEPARATOR}${VTK_BINARY_DIR}/Wrapping/D
    -source 1.5 -sourcepath ${VTK_SOURCE_DIR}/Wrapping/D
    -d ${VTK_BINARY_DIR}/Wrapping/D
    ${VTK_D_TESTS}
    COMMAND ${CMAKE_COMMAND} -E touch ${VTK_BINARY_DIR}/d/dc_tests_stamp.txt
    DEPENDS ${VTK_JAR_PATH}/vtk.jar ${VTK_D_TESTS}
    COMMENT "Compiling D Tests"
    )

  # Since vtkTesting is within vtkTestingRendering we need blockers
  if(vtkTestingRendering_ENABLED)

    # Require Rendering
    if(vtkRenderingOpenGL_ENABLED)
      # If D_DATAMODEL_FLAG is set, pass it as first argument. Otherwise, it's empty.
      # For example, use "-d64" to force 64-bit d jvm.
      #
      ExternalData_add_test(VTKData
         DRegression ${D_RUNTIME} ${VTK_TEST_D_DATAMODEL_FLAG}
         -classpath "${VTK_JAR_PATH}/vtk.jar${SEPARATOR}${VTK_BINARY_DIR}/Wrapping/D"
         vtk.test.Regression -D ${VTK_TEST_DATA_DIR}
         -V DATA{Baseline/Cone.png,:}
         -T ${VTK_TEST_OUTPUT_DIR}
         )
    endif()

    add_test(DGCRegression ${D_RUNTIME} ${VTK_TEST_D_DATAMODEL_FLAG}
      -classpath ${VTK_JAR_PATH}/vtk.jar${SEPARATOR}${VTK_BINARY_DIR}/Wrapping/D
      vtk.test.ConcurrencyGC
      -T ${VTK_TEST_OUTPUT_DIR}
      )

    add_test(DDeleteRegression ${D_RUNTIME} ${VTK_TEST_D_DATAMODEL_FLAG}
      -classpath ${VTK_JAR_PATH}/vtk.jar${SEPARATOR}${VTK_BINARY_DIR}/Wrapping/D
      vtk.test.DDelete
     -T ${VTK_TEST_OUTPUT_DIR}
     )

    add_test(DManualGC ${D_RUNTIME} ${VTK_TEST_D_DATAMODEL_FLAG}
      -classpath ${VTK_JAR_PATH}/vtk.jar${SEPARATOR}${VTK_BINARY_DIR}/Wrapping/D
      vtk.test.ManualGC
      -T ${VTK_TEST_OUTPUT_DIR}
      )

    add_test(DDeleteAndGC ${D_RUNTIME} ${VTK_TEST_D_DATAMODEL_FLAG}
      -classpath ${VTK_JAR_PATH}/vtk.jar${SEPARATOR}${VTK_BINARY_DIR}/Wrapping/D
      vtk.test.DGCAndDelete
     -T ${VTK_TEST_OUTPUT_DIR}
     )
  endif()
endif()

# Make sure .class files are cleaned up
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${VTK_D_CLASS_FILES}")
install(FILES
  ${VTK_JAR_PATH}/vtk.jar
  DESTINATION ${VTK_INSTALL_LIBRARY_DIR}
  COMPONENT RuntimeLibraries)

#-----------------------------------------------------------------------------
# Post-Install script for Custom D Packaging
# This install rule MUST stay at the bottom of that CMakeLists file as
# it has to be the last install rule that get executed
if(VTK_D_INSTALL)
  set(MAVEN_NATIVE_ARTIFACT_XML)
  foreach(native_name ${MAVEN_NATIVE_ARTIFACTS})
     set(MAVEN_NATIVE_ARTIFACT_XML "${MAVEN_NATIVE_ARTIFACT_XML}\n                                <artifact><file>vtk-${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}-natives-${native_name}.jar</file><classifier>natives-${native_name}</classifier><type>jar</type></artifact>")
  endforeach()
  if(VTK_D_JOGL_COMPONENT)
    set(MAVEN_DEPENDENCY_XML "${MAVEN_DEPENDENCY_XML}\n      <dependency><groupId>org.jogamp.jogl</groupId><artifactId>jogl-all-main</artifactId><version>${JOGL_VERSION}</version></dependency>\n      <dependency><groupId>org.jogamp.gluegen</groupId><artifactId>gluegen-rt-main</artifactId><version>${JOGL_VERSION}</version></dependency>")
  endif()
  configure_file(DInstall.cmake.in
               ${VTK_BINARY_DIR}/DInstall.cmake @ONLY)
  configure_file(Maven/pom.xml.in
               ${VTK_BINARY_DIR}/pom.xml @ONLY)
  configure_file(Maven/README.txt.in
               ${VTK_BINARY_DIR}/MAVEN-README.txt @ONLY)
  install(SCRIPT ${VTK_BINARY_DIR}/DInstall.cmake)
endif(VTK_D_INSTALL)
