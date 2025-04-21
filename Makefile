TARGET := no-deal

all: $(TARGET).z64
.PHONY: all

include $(N64_INST)/include/n64.mk

SRC_DIR := src
BUILD_DIR := build

# Find all source files recursively
SRC := $(shell find $(SRC_DIR) -name "*.c")
# Create list of object files
OBJS := $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Create directories for object files
OBJDIRS := $(sort $(dir $(OBJS)))
$(shell mkdir -p $(OBJDIRS))

# DFS filesystem stuff
DFS_DIR := $(wildcard $(SRC_DIR)/filesystem/*)

# ROM settings
$(TARGET).z64: N64_ROM_TITLE="No Deal"
$(TARGET).z64: N64_ROM_REGIONFREE=true

# Link object files first to create the ELF
$(TARGET).elf: $(OBJS)

# Create the DFS filesystem
$(BUILD_DIR)/$(TARGET).dfs: $(DFS_DIR)
	@echo "    [FILESYSTEM] $@"
	$(N64_MKDFS) $@ $(SRC_DIR)/filesystem

# Make ROM depend on filesystem
$(TARGET).z64: $(BUILD_DIR)/$(TARGET).dfs

# Explicit pattern rule for object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "    [CC] $<"
	$(CC) -c $(CFLAGS) -o $@ $<

# Clean up build products
clean:
	rm -rf $(BUILD_DIR)/* *.z64 *.elf filesystem.dat
.PHONY: clean

# Include dependency files if they exist
-include $(wildcard $(BUILD_DIR)/*.d)

