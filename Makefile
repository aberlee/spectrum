# C Project Makefile

###### Compiler / linker setup ######
# gcc with MinGW setup.
CC := gcc
CFLAGS := -g -O3 -Wall -Wpedantic -Wextra -std=gnu99
DFLAGS := -MP -MMD
LFLAGS := -g -lm -Wl,--subsystem,windows
DEFINE := 
INCLUDE := 
LIBRARY := 

# debug.h
DEFINE += -DVERBOSE -DDEBUG

######### Source code setup #########
# Directory for all project files.
SRC_DIR := src
INCLUDE_DIR := include
INCLUDE += -I$(INCLUDE_DIR)

# Source files
CFILES := $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/**/*.c)
HFILES := $(wildcard $(INCLUDE_DIR)/*.h) $(wildcard $(INCLUDE_DIR)/**/*.h)
IFILES := $(wildcard $(INCLUDE_DIR)/*.i) $(wildcard $(INCLUDE_DIR)/**/*.i)
RFILES := spectrum.rc

# Important files
MAKEFILE := Makefile

########### Allegro Setup ###########
# Link to installation of Allegro5.
ALLEGRO_DIR := lib/allegro
INCLUDE += -I$(ALLEGRO_DIR)/include
LIBRARY += -L$(ALLEGRO_DIR)/lib
LFLAGS += -lallegro -lallegro_audio -lallegro_acodec -lallegro_font -lallegro_ttf
LFLAGS += -lallegro_image -lallegro_color -lallegro_primitives  -lallegro_main 

############ Build setup ############
# Get the names of all object files
# and dependency files.
BUILD_DIR := build
OFILES := $(CFILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DFILES := $(OFILES:%.o=%.d)
WFILES := $(RFILES:%.rc=$(BUILD_DIR)/%.rc.o)
EXECUTABLE := spectrum.exe

############### Rules ###############
.PHONY: default
default: $(BUILD_DIR) $(EXECUTABLE)

# Put all the .o files in the build directory
$(BUILD_DIR):
	@mkdir $(BUILD_DIR)

# Put the version information in the build directory
$(BUILD_DIR)/%.rc.o: %.rc
	@windres $(DEFINE) -J rc $< $@ 

# Compile the source files
.SECONDARY: $(DFILES)
.SECONDARY: $(OFILES)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(MAKEFILE)
	@$(CC) $(CFLAGS) $(DEFINE) $(DFLAGS) $(INCLUDE) -c $< -o $@

# Automatic dependency files
-include $(DFILES)

# Make executable for each driver
$(EXECUTABLE): $(OFILES) $(WFILES)
	@$(CC) -o $@ $^ $(LIBRARY) $(LFLAGS)

# Clean up build files and executable
.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR) $(EXECUTABLE)
