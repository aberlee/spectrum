# C Project Makefile

#===== Compiler / linker setup =====#
# gcc with MinGW setup.
CC := gcc
CFLAGS := -g -O3 -Wall -Wpedantic -Wextra -std=gnu99
DFLAGS := -MP -MMD
LFLAGS := -s -lm
INCLUDE := 
LIBRARY := 

#======== Source code setup ========#
# Directory for all project files.
SRC_DIR := ./src
INCLUDE += -I$(SRC_DIR)

# Source files
CFILES := $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/**/*.c)
HFILES := $(wildcard $(SRC_DIR)/*.h) $(wildcard $(SRC_DIR)/**/*.h)
IFILES := $(wildcard $(SRC_DIR)/*.i) $(wildcard $(SRC_DIR)/**/*.i)
SRC_SUB_DIRS := $(sort $(filter-out $(SRC_DIR)/,$(dir $(CFILES) $(HFILES) @(IFILES))))

# Important files
MAKEFILE := Makefile

#========== Allegro Setup ==========#
# Link to installation of Allegro5.
ALLEGRO_DIR := C:/lib/allegro/allegro
INCLUDE += -I$(ALLEGRO_DIR)/include
LIBRARY += -L$(ALLEGRO_DIR)/lib
LFLAGS += -lallegro -lallegro_audio -lallegro_font -lallegro_image -lallegro_primitives -lallegro_ttf

#=========== Build setup ===========#
# Get the names of all object files
# and dependency files.
BUILD_DIR := build
BUILD_SUB_DIRS := $(SRC_SUB_DIRS:$(SRC_DIR)/%=$(BUILD_DIR)/%)
OFILES := $(CFILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DFILES := $(OFILES:%.o=%.d)

#============ Main files ===========#
# Standalone text executable sources
# to link with the rest of the code.
MAIN_DIR := ./main
MCFILES := $(wildcard $(MAIN_DIR)/*.c)
MOFILES := $(MCFILES:$(MAIN_DIR)/%.c=$(BUILD_DIR)/$(MAIN_DIR)/%.o)
MDFILES := $(MOFILES:%.o=%.d)

EXECUTABLES := $(MCFILES:$(MAIN_DIR)/%.c=%.exe)

#============== Rules ==============#
.PHONY: default
default: $(BUILD_DIR) $(EXECUTABLES)

# Put all the .o files in the build directory
$(BUILD_DIR):
	@mkdir $(BUILD_DIR) $(BUILD_DIR)/$(MAIN_DIR) $(BUILD_SUB_DIRS)

# Compile the source files
.SECONDARY: $(DFILES)
.SECONDARY: $(OFILES)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(MAKEFILE)
	@$(CC) $(CFLAGS) $(DFLAGS) $(INCLUDE) -c $< -o $@
	
# Compile the main method executables
.SECONDARY: $(MDFILES)
.SECONDARY: $(MOFILES)
$(BUILD_DIR)/$(MAIN_DIR)/%.o: $(MAIN_DIR)/%.c $(MAKEFILE)
	@$(CC) $(CFLAGS) $(DFLAGS) $(INCLUDE) -c $< -o $@

# Automatic dependency files
-include $(DFILES)
-include $(MDFILES)

# Make executable for each driver
%.exe: $(BUILD_DIR)/$(MAIN_DIR)/%.o $(OFILES)
	@$(CC) -o $@ $^ $(LIBRARY) $(LFLAGS)

# Clean up build files and executable
.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR) $(EXECUTABLES)

#===================================#
