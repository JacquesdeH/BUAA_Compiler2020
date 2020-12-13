import os
import sys

L = 8
R = 8
BENCH_NAME = "PUBGEN"
BENCH_ID = 1

testfilePath = os.path.join("..", os.path.join("testcases", os.path.join(BENCH_NAME, str(BENCH_ID)))) if BENCH_ID > 0 \
    else os.path.join("..", os.path.join("testcases", os.path.join(BENCH_NAME)))
mipsJar = os.path.join(".", "Mars-jdk7-Re-v5.jar")

compilePath = os.path.join("..", "cmake-build-debug-compiler-course")
exeFile = os.path.join(compilePath, os.path.join("Project.exe"))
os.system("copy " + exeFile + " " + "Project.exe")

for i in range(L, R+1):
    print("Judging on testcase " + str(i))
    inputFile = os.path.join(testfilePath, os.path.join("input"+str(i)+".txt"))
    outputFile = os.path.join(testfilePath, os.path.join("output"+str(i)+".txt"))
    testFile = os.path.join(testfilePath, os.path.join("testfile"+str(i)+".txt"))
    os.system("copy " + inputFile + " " + "stdin.txt")
    os.system("copy " + outputFile + " " + "stdout.txt")
    os.system("copy " + testFile + " " + "testfile.txt")
    # start judging
    os.system("Project.exe 2>comperr.txt")
    os.system("java -jar " + mipsJar + " mips.txt" + " <stdin.txt 1>ansout.txt 2>marserr.txt")
    # beautify ansout.txt
    lines = open("ansout.txt").readlines()
    open("ansout.txt", 'w').writelines(lines[2:-1])
    os.system("fc ansout.txt stdout.txt")
    comperrs = open("comperr.txt").readlines()
    if len(comperrs) > 0:
        print("comperrs not empty!!!!!!!!!!!!!!!!!", file=sys.stderr)
    marserrs = open("marserr.txt").readlines()
    if len(marserrs) > 0:
        print("marserr not empty!!!!!!!!!!!!!!!!!", file=sys.stderr)
    print()

