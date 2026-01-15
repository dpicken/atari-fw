BUILTIN_DIR := ./builtin
BUILTIN_FILES := $(sort $(shell find $(BUILTIN_DIR) -type f))

BUILTIN_DIRS := $(sort $(dir $(BUILTIN_FILES)))
BUILTIN_DEPS := $(BUILTIN_FILES) $(BUILTIN_DIRS)

SRC_FS_BUILTIN_LIBRARY := $(SRC_DIR)/fs/builtin/Library.gen

SRC_FS_BUILTIN_LIBRARY_FILES :=
SRC_FS_BUILTIN_LIBRARY_FILES += $(SRC_FS_BUILTIN_LIBRARY).data.h
SRC_FS_BUILTIN_LIBRARY_FILES += $(SRC_FS_BUILTIN_LIBRARY).sizes.h
SRC_FS_BUILTIN_LIBRARY_FILES += $(SRC_FS_BUILTIN_LIBRARY).paths.h

$(SRC_FS_BUILTIN_LIBRARY).data.h: $(BUILTIN_DEPS)
	$(echo_build_message)
	$(echo_recipe)[ -n "$(BUILTIN_FILES)" ] || (echo Error: no builtinfiles && false)
	$(echo_recipe)hexdump -v -C $(BUILTIN_FILES) | cut -s -d ' ' -f 2-19 | tr -s ' ' | sed 's@ *$$@@g' | sed 's/ /, 0x/g' | sed 's/^, / /' | tr '\n' ',' | sed 's/,$$/\n/' >$@

STAT_SIZE_linux := stat -c %s
STAT_SIZE_darwin := stat -f %z
STAT_SIZE := $(STAT_SIZE_$(host_os))

$(SRC_FS_BUILTIN_LIBRARY).sizes.h: $(SRC_FS_BUILTIN_LIBRARY).data.h
	$(echo_build_message)
	$(echo_recipe)$(STAT_SIZE) $(BUILTIN_FILES) | tr '\n' ',' | sed 's/,$$/\n/' >$@

$(SRC_FS_BUILTIN_LIBRARY).paths.h: $(SRC_FS_BUILTIN_LIBRARY).data.h
	$(echo_build_message)
	$(echo_recipe)echo "$(BUILTIN_FILES)" | xargs -n1 basename | xargs -I% echo '"%"' | tr '\n' ',' | sed 's/,$$/\n/' >$@

.PHONY: fs-builtin-library
fs-builtin-library: $(SRC_FS_BUILTIN_LIBRARY_FILES)

.PHONY: fs-builtin-library-clean
fs-builtin-library-clean:
	$(echo_recipe)rm -f $(SRC_FS_BUILTIN_LIBRARY_FILES)

init: fs-builtin-library
clean: fs-builtin-library-clean
