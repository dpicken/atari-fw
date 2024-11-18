LOCAL_ATR_DIR := ./atr
LOCAL_ATR_FILES := $(sort $(wildcard $(LOCAL_ATR_DIR)/*.atr))

EXTERNAL_ATR_DIR ?= ../atari-atr
EXTERNAL_ATR_FILES := $(if $(EXTERNAL_ATR_DIR),$(sort $(shell [ -d $(EXTERNAL_ATR_DIR) ] && find $(EXTERNAL_ATR_DIR)/ -type f -name '*.atr' -print0 | xargs -0 -I% bash -c 'mv "%" $$(echo "%" | tr " " "-" | tr "[:upper:]" "[:lower:]")' && find $(EXTERNAL_ATR_DIR) -type f -name '*.atr')),)

ATR_FILES := $(LOCAL_ATR_FILES) $(EXTERNAL_ATR_FILES)
ATR_DIRS := $(sort $(dir $(ATR_FILES)))
ATR_DEPS := $(ATR_FILES) $(ATR_DIRS)

SRC_MEDIA_BUILTIN_ATR_LIBRARY := $(SRC_DIR)/media/BuiltinAtrLibrary.gen

SRC_MEDIA_BUILTIN_ATR_LIBRARY_FILES :=
SRC_MEDIA_BUILTIN_ATR_LIBRARY_FILES += $(SRC_MEDIA_BUILTIN_ATR_LIBRARY).data.h
SRC_MEDIA_BUILTIN_ATR_LIBRARY_FILES += $(SRC_MEDIA_BUILTIN_ATR_LIBRARY).index.h
SRC_MEDIA_BUILTIN_ATR_LIBRARY_FILES += $(SRC_MEDIA_BUILTIN_ATR_LIBRARY).title.h

$(SRC_MEDIA_BUILTIN_ATR_LIBRARY).data.h: $(ATR_DEPS)
	$(echo_build_message)
	$(echo_recipe)[ -n "$(ATR_FILES)" ] || (echo Error: no atr files && false)
	$(echo_recipe)hexdump -v -C $(ATR_FILES) | cut -s -d ' ' -f 2-19 | tr -s ' ' | sed 's/ /, 0x/g' | sed 's/^, / /' | tr '\n' ',' | sed 's/,$$/\n/' >$@

$(SRC_MEDIA_BUILTIN_ATR_LIBRARY).index.h: $(SRC_MEDIA_BUILTIN_ATR_LIBRARY).data.h
	$(echo_build_message)
	$(echo_recipe)stat -f %z $(ATR_FILES) | tr '\n' ',' | sed 's/,$$/\n/' >$@

$(SRC_MEDIA_BUILTIN_ATR_LIBRARY).title.h: $(SRC_MEDIA_BUILTIN_ATR_LIBRARY).data.h
	$(echo_build_message)
	$(echo_recipe)echo "$(ATR_FILES)" | xargs -n1 basename | xargs -I% echo '"%"' | tr '\n' ',' | sed 's/,$$/\n/' >$@

.PHONY: builtin-atr-library
builtin-atr-library: $(SRC_MEDIA_BUILTIN_ATR_LIBRARY_FILES)

.PHONY: builtin-atr-library-clean
builtin-atr-library-clean:
	$(echo_recipe)rm -f $(SRC_MEDIA_BUILTIN_ATR_LIBRARY_FILES)

init: builtin-atr-library
clean: builtin-atr-library-clean
