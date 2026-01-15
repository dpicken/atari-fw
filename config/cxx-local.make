CMD_FIND_SRC_STEM = find $(src_dir) -mindepth 1 ! \( -path $(src_dir)/platform -prune \)

CXXFLAGS += -Wno-exit-time-destructors
