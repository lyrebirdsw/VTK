.class files --> .o files
.jar file --> .a library (or .so)


1. Copy the Java directory as D
2. Refactor all JAVA, java references to D in CMake and code.
3. Remove all MAVAN references in the build
4. Update CMakeLists.txt for D compiler replacing .class generation
with .o compilation and .jar with linking obj files to a .a library.




