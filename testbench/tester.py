import os

L = 1
R = 8
testfilePath = os.path.join("..", os.path.join("testcases", os.path.join("PUBGEN", "1")))
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
    os.system("Project.exe")
    os.system("java -jar " + mipsJar + " mips.txt" + " <stdin.txt 1>ansout.txt 2>anserr.txt")
    # beautify ansout.txt
    lines = open("ansout.txt").readlines()
    open("ansout.txt", 'w').writelines(lines[2:])
    os.system("fc ansout.txt stdout.txt")

