SOURCES= main.c png.c keyboard.c
HEADERS= 

BUILD_DIR=build/
#Relative to build dir
TARGET_NAME=cglfw
C_INCLUDE_DIR=-I ./ -I /opt/cuda/include
TOOLCHAIN=

#We add the python files as depency for make so it detects when we change them
PYTHON_LIBS=


#Generic stuff from here

DEPS=$(SOURCES:%.c=$(BUILD_DIR)%.d)

OBJECTS=$(SOURCES:%.c=%.o)


TARGET=$(BUILD_DIR)$(TARGET_NAME)

CC=$(TOOLCHAIN)gcc
OBJCOPY=$(TOOLCHAIN)objcopy
OBJDUMP=$(TOOLCHAIN)objdump
SIZETOOL=$(TOOLCHAIN)size


C_DEFS=
C_FLAGS=-O3 -Wall -std=gnu99 -fdata-sections -ffunction-sections -fPIC
CPU=-march=native

LINK_SCRIPT=
LINK_FLAGS= -L /opt/cuda/lib64 -lcudart -lGL -lglfw -lm -lpng -Wl,--gc-sections -dead-strip 
# --shared

default: all

print_size: all
	@echo 'Checking size: $@'
	$(SIZETOOL)  --format=berkeley "$(TARGET:.bin=.elf)"
	@echo ' '

all: $(PYTHON_LIBS) $(SOURCES) $(TARGET)

testing: $(TARGET)
	@echo -= Running tests =-
	@echo
	@python3 -i test1.py $(UART)


print_debug:
	@echo Build target: $(TARGET)
	@echo
	@echo Sources: $(SOURCES)
	@echo
	@echo Dependency output: $(DEPS)
	@echo
	@echo Precompiled: $(PRECOMPILED)
	@echo
	@echo Processed by python: $(PYTHON_PROCESSED)
	@echo
	@echo Objects: $(OBJECTS)
	@echo



build/test1.so: test1.cu
	nvcc -v -Xcompiler -fPIC -Xcompiler --shared -o build/test1.so test1.cu


$(OBJECTS): %.o: %.c
	@echo 'Building file: $<'
	$(CC) -c $(CPU) $(C_FLAGS) $(C_DEFS) $(C_INCLUDE_DIR) -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

$(TARGET): $(OBJECTS) build/test1.so
	@echo 'Building target: $@'
	$(CC) $(LINK_FLAGS) $(LINK_SCRIPT) -o "$(TARGET)" $(OBJECTS) build/test1.so $(STARTUP_SCRIPT)
#	$(CC) $(LINK_FLAGS) $(LINK_SCRIPT) -o "$(TARGET)" $(OBJECTS) cudatest3.so $(STARTUP_SCRIPT)
	@echo 'Finished building target: $@'
	@echo ' '



clean:
	rm -f $(PRECOMPILED) $(PYTHON_PROCESSED) $(OBJECTS) $(TARGET) $(OBJECTS:.o=.lst) $(OBJECTS:.o=.d)

#.PHONY: all clean target Makefile $(PYTHON_LIBS)
