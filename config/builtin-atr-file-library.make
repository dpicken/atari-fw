LOCAL_ATR_DIR := ./atr
LOCAL_ATR_FILES := $(sort $(wildcard $(LOCAL_ATR_DIR)/*.atr))

EXTERNAL_ATR_DIR := ../atari-atr
EXTERNAL_ATR_FILES := $(sort $(shell [ -d $(EXTERNAL_ATR_DIR) ] && find $(EXTERNAL_ATR_DIR)/ -type f -name '*.atr' -print0 | xargs -0 -I% bash -c 'mv "%" $$(echo "%" | tr " " "-" | tr "[:upper:]" "[:lower:]")' && find $(EXTERNAL_ATR_DIR)/ -type f -name '*.atr'))

ATR_FILES ?= $(LOCAL_ATR_FILES) $(EXTERNAL_ATR_FILES)

SRC_MEDIA_BUILTIN_ATR_FILE_LIBRARY := $(SRC_DIR)/media/BuiltinAtrFileLibrary.gen

SRC_MEDIA_BUILTIN_ATR_FILE_LIBRARY_FILES :=
SRC_MEDIA_BUILTIN_ATR_FILE_LIBRARY_FILES += $(SRC_MEDIA_BUILTIN_ATR_FILE_LIBRARY).data.h
SRC_MEDIA_BUILTIN_ATR_FILE_LIBRARY_FILES += $(SRC_MEDIA_BUILTIN_ATR_FILE_LIBRARY).index.h
SRC_MEDIA_BUILTIN_ATR_FILE_LIBRARY_FILES += $(SRC_MEDIA_BUILTIN_ATR_FILE_LIBRARY).title.h

.PHONY: $(SRC_MEDIA_BUILTIN_ATR_FILE_LIBRARY).data.h
$(SRC_MEDIA_BUILTIN_ATR_FILE_LIBRARY).data.h:
	$(echo_build_message)
	$(echo_recipe)[ -n "$(ATR_FILES)" ] || (echo Error: no atr files && false)
	$(echo_recipe)hexdump -v -C $(ATR_FILES) | cut -s -d ' ' -f 2-19 | tr -s ' ' | sed 's/ /, 0x/g' | sed 's/^, / /' | tr '\n' ',' | sed 's/,$$/\n/' >$@

.PHONY: $(SRC_MEDIA_BUILTIN_ATR_FILE_LIBRARY).index.h | $(SRC_MEDIA_BUILTIN_ATR_FILE_LIBRARY).data.h
$(SRC_MEDIA_BUILTIN_ATR_FILE_LIBRARY).index.h:
	$(echo_build_message)
	$(echo_recipe)stat -f %z $(ATR_FILES) | tr '\n' ',' | sed 's/,$$/\n/' >$@

.PHONY: $(SRC_MEDIA_BUILTIN_ATR_FILE_LIBRARY).title.h | $(SRC_MEDIA_BUILTIN_ATR_FILE_LIBRARY).data.h
$(SRC_MEDIA_BUILTIN_ATR_FILE_LIBRARY).title.h:
	$(echo_build_message)
	$(echo_recipe)echo "$(ATR_FILES)" | xargs -n1 basename | xargs -I% echo '"%"' | tr '\n' ',' | sed 's/,$$/\n/' >$@

.PHONY: builtin-atr-file-library
builtin-atr-file-library: $(SRC_MEDIA_BUILTIN_ATR_FILE_LIBRARY_FILES)

.PHONY: builtin-atr-file-library-clean
builtin-atr-file-library-clean:
	$(echo_recipe)rm -f $(SRC_MEDIA_BUILTIN_ATR_FILE_LIBRARY_FILES)

all: builtin-atr-file-library
clean: builtin-atr-file-library-clean
