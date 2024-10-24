SD_IMAGE_DIR ?= ../sd-image
SD_IMAGE_FILE_EXT := image
SD_IMAGE_FILES := $(shell [ -d $(SD_IMAGE_DIR) ] && find $(SD_IMAGE_DIR) -type f -name '*.$(SD_IMAGE_FILE_EXT)')

SRC_FS_TEST_DATA_DIR := $(SRC_DIR)/fs/test/data
SRC_FS_TEST_DATA_FILE_EXT := data
SRC_FS_TEST_DATA_FILES := $(patsubst %.$(SD_IMAGE_FILE_EXT),%.$(SRC_FS_TEST_DATA_FILE_EXT),$(patsubst $(SD_IMAGE_DIR)/%,$(SRC_FS_TEST_DATA_DIR)/%,$(SD_IMAGE_FILES)))

$(SRC_FS_TEST_DATA_DIR)/%.$(SRC_FS_TEST_DATA_FILE_EXT): $(SD_IMAGE_DIR)/%.$(SD_IMAGE_FILE_EXT)
	$(echo_build_message)
	$(echo_recipe)hexdump -v -C $^ | cut -s -d ' ' -f 2-19 | tr -s ' ' | sed 's/ /, 0x/g' | sed 's/^, / /' | tr '\n' ',' | sed 's/,$$/\n/' >$@

.PHONY: fs_test_data_files
fs_test_data_files: $(SRC_FS_TEST_DATA_FILES)
