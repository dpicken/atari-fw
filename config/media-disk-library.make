LOCAL_ATR_DIR := ./atr
LOCAL_ATR_FILES := $(sort $(wildcard $(LOCAL_ATR_DIR)/*.atr))

THIRD_PARTY_ATR_DIR := ./third-party/atr
THIRD_PARTY_ATR_FILES := $(sort $(wildcard $(THIRD_PARTY_ATR_DIR)/*.atr))

ATR_FILES ?= $(if $(THIRD_PARTY_ATR_FILES),$(THIRD_PARTY_ATR_FILES),$(LOCAL_ATR_FILES))

SRC_MEDIA_DISK_LIBRARY := $(SRC_DIR)/media/DiskLibraryAtr.gen

.PHONY: fix-third-party-atr-file-names
fix-third-party-atr-file-names:
	$(echo_build_message)
	$(echo_recipe)find $(THIRD_PARTY_ATR_DIR)/ -type f -name '*.atr' -print0 | xargs -0 -I% bash -c 'mv "%" $$(echo "%" | tr " " "-" | tr "[:upper:]" "[:lower:]")'

.PHONY: $(SRC_MEDIA_DISK_LIBRARY).data.h
$(SRC_MEDIA_DISK_LIBRARY).data.h: | fix-third-party-atr-file-names
	$(echo_build_message)
	$(echo_recipe)[ -n "$(ATR_FILES)" ] || (echo Error: no atr files && false)
	$(echo_recipe)hexdump -v -C $(ATR_FILES) | cut -s -d ' ' -f 2-19 | tr -s ' ' | sed 's/ /, 0x/g' | sed 's/^, / /' | tr '\n' ',' | sed 's/,$$/\n/' >$@

.PHONY: $(SRC_MEDIA_DISK_LIBRARY).index.h | $(SRC_MEDIA_DISK_LIBRARY).data.h
$(SRC_MEDIA_DISK_LIBRARY).index.h:
	$(echo_build_message)
	$(echo_recipe)stat -f %z $(ATR_FILES) | tr '\n' ',' | sed 's/,$$/\n/' >$@

.PHONY: $(SRC_MEDIA_DISK_LIBRARY).title.h | $(SRC_MEDIA_DISK_LIBRARY).data.h
$(SRC_MEDIA_DISK_LIBRARY).title.h:
	$(echo_build_message)
	$(echo_recipe)echo "$(ATR_FILES)" | xargs -n1 basename | xargs -I% echo '"%"' | tr '\n' ',' | sed 's/,$$/\n/' >$@

.PHONY: media-disk-library
media-disk-library: $(SRC_MEDIA_DISK_LIBRARY).data.h $(SRC_MEDIA_DISK_LIBRARY).index.h $(SRC_MEDIA_DISK_LIBRARY).title.h

.PHONY: media-disk-library-clean
media-disk-library-clean:
	$(echo_recipe)rm -f $(SRC_MEDIA_DISK_LIBRARY).data.h $(SRC_MEDIA_DISK_LIBRARY).index.h

all: media-disk-library
clean: media-disk-library-clean
