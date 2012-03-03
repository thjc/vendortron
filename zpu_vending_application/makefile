# Makefile to compile and link a program for the ZPU architecture
# Creates a raw binary RAM image to load - vendortron.bin
# Creates an assembler listing file - vendortron.lst

# Path to the gcc executable and friends
TOOLPATH=../toolchain/linux/bin/

# Path to libraries
LIBPATH=./lib/

# Libraries
LIBS=-lezpu

# The compiler/linker
CC=$(TOOLPATH)zpu-elf-gcc

AR=$(TOOLPATH)zpu-elf-ar

# objdump. Disassemble including line numbers and source code. 
OD=$(TOOLPATH)zpu-elf-objdump -d -l --source
#OD=$(TOOLPATH)zpu-elf-objdump -D -l --source

# objcopy. Just copy relevent sections to a binary image.
OC=$(TOOLPATH)zpu-elf-objcopy -O binary

# Compiler flags. Compile only, debug info, all warnings, optimize for size
CFLAGS=-c -g -Wall -Os -I./include -L./lib

# The ZPU platform Phi board (phi) or Able board (able)
ZPUPLATFORM=-phi

#Linker flags. phi platform, shrink(relax) immediates, remove unwanted sections, ezpu linker commands, libraries
LDFLAGS=$(ZPUPLATFORM) -Wl,--relax -Wl,--gc-sections -Wl,-Tezpu.x

# Source files, add more here if you have
SOURCES=src/vendortron.c 

# Create a list of object file names from source file names
OBJECTS=$(SOURCES:.c=.o)

LIBRARY=lib/libezpu.a

# The program to build 
EXECUTABLE=vendortron

# Binary output file name
BINARY=$(EXECUTABLE).bin

#Listing output file name
LISTING=$(EXECUTABLE).lst

# By default build an executeable, a raw binary RAM image and an assembler listing file 
all: $(SOURCES) $(EXECUTABLE) $(BINARY) $(LISTING)
	

lib/ezpu.o: lib/ezpu.c
	$(CC) -c -g -Wall -Os $^ -o $@

$(LIBRARY): lib/ezpu.o
	$(AR) rcs $@ $^

# Link the executable from object files
$(EXECUTABLE): $(OBJECTS) $(LIBRARY)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ -L$(LIBPATH) $(LIBS)

# Convert elf executable to raw binary image
$(BINARY): $(EXECUTABLE)
	$(OC) $(EXECUTABLE) $(BINARY)
	cp $(BINARY) ../files/rom
  
# Create a listing file
$(LISTING): $(EXECUTABLE)
	$(OD) $(EXECUTABLE) > $(LISTING)

# Compile .c files into objexts .o
.c.o:
	$(CC) $(CFLAGS) $< -o $@

# Clean up 
clean:
	rm -rf *.o lib/*.o $(EXECUTABLE) $(BINARY) $(LISTING) $(LIBRARY)

  
