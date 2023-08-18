#ifndef platform_rp2040_qtpy_tusb_config_h
#define platform_rp2040_qtpy_tusb_config_h

#define CFG_TUSB_RHPORT0_MODE       OPT_MODE_HOST
#define CFG_TUSB_OS                 OPT_OS_PICO

#define CFG_TUSB_MEM_SECTION
#define CFG_TUSB_MEM_ALIGN          __attribute__ ((aligned(4)))

#define CFG_TUH_ENUMERATION_BUFSIZE 256
#define CFG_TUH_HUB                 1
#define CFG_TUH_HID                 4
#define CFG_TUH_DEVICE_MAX          (CFG_TUH_HUB ? 4 : 1)

#define CFG_TUH_HID_EPIN_BUFSIZE    64
#define CFG_TUH_HID_EPOUT_BUFSIZE   64

#endif // ifndef platform_rp2040_qtpy_tusb_config_h
