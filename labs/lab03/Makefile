# Build file for CS61C Lab 3
# You should not need to edit this file.

# This file requires GNU make and depends on paths on instruction machines.

####

## Variables

# Source files (java code)
WORDCOUNT = WordCount.java
NUMSTR = NumStr
# Output JAR file for WordCOunt
TARGET = wc.jar
# As of 9/5/13, the local machines are using JDK 7 but our EC2 servers only supports JRE 6
# Tells the compiler to compiel for JRE6
COMPAT_FLAGS = -source 6 -target 6
# Extra JARs to have on the classpath when compiling.
CLASSPATH = /home/ff/cs61c/hadoop/hadoop-core.jar:/home/ff/cs61c/hadoop/lib/commons-cli.jar
# javac command to use
JAVAC = javac -g -deprecation -cp $(CLASSPATH)
# jar command to use
JAR = jar

## Make targets

# General form is target: dependencies (targets or files), followed by
# commands to run to build the target from the dependencies.

# Default target.
all: $(TARGET)

$(TARGET): classes $(WORDCOUNT)
	$(JAVAC) $(COMPAT_FLAGS) -d classes $(WORDCOUNT)
	$(JAR) cf $(TARGET) -C classes .

classes:
	mkdir classes

clean:
	rm -rf classes $(TARGET)  $(NUMSTR).class

numstr:
	$(JAVAC) $(NUMSTR).java
	java -cp $(CLASSPATH):. $(NUMSTR)


.PHONY: clean all
