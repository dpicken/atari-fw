CMD_FIND_SRC_STEM = find $(src_dir) ! \( -path $(src_dir)/platform -prune \) -and -mindepth 1

CXXFLAGS += -Wno-exit-time-destructors
