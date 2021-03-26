JDKPATH = /usr
LIBPATH = lib/bufmgrAssign.jar:lib/junit.jar:lib/org.hamcrest.core_1.3.0.v201303031735.jar

CLASSPATH = .:..:$(LIBPATH)
BINPATH = $(JDKPATH)/bin
JAVAC = $(JDKPATH)/bin/javac 
JAVA  = $(JDKPATH)/bin/java 

PROGS = xx

all: $(PROGS)

compile:src/*/*.java
	$(JAVAC) -cp $(CLASSPATH) -d bin src/*/*.java

xx : compile
	$(JAVA) -cp $(CLASSPATH):bin org.junit.runner.JUnitCore tests.BMTest

