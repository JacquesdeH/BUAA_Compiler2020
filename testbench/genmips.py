import os

compilePath = os.path.join("..", "cmake-build-debug-compiler-course")
dstTestfile = os.path.join(compilePath, os.path.join("testfile.txt"))
os.system("copy " + "testfile.txt" + " " + dstTestfile)


