#Makefile to generate appsboot.mbn

ifeq ($(BOOTLOADER_OUT),.)
APPSBOOTHEADER_DIR  := $(BUILDDIR)
else
APPSBOOTHEADER_DIR  := $(BOOTLOADER_OUT)/../..
endif

SRC_DIR  := target/$(TARGET)/tools
COMPILER := gcc

ifeq ($(ENABLE_2NDSTAGE_BOOT), 1)
  APPSBOOTHDR_FILES := lkboot.img
else
  ifeq ($(EMMC_BOOT), 1)
    APPSBOOTHDR_FILES := EMMCBOOT.MBN
  else
    ifeq ($(BUILD_NANDWRITE), 1)
      APPSBOOTHDR_FILES :=
    else
      APPSBOOTHDR_FILES := appsboot.mbn
    endif
  endif
endif

APPSBOOTHEADER: $(APPSBOOTHDR_FILES)

mkbootimg:
	${COMPILER} $(SRC_DIR)/mkbootimg/mkbootimg.c $(SRC_DIR)/mkbootimg/sha.c -o $(BUILDDIR)/mkbootimg

lkboot.img: mkbootimg $(OUTBIN)
	$(APPSBOOTHEADER_DIR)/mkbootimg --kernel $(OUTBIN) --ramdisk /dev/null --pagesize 2048 \
		--base $(shell printf "0x%X" $$(($(MEMBASE)-0x8000))) -o $(APPSBOOTHEADER_DIR)/lkboot.img

appsboot.mbn: appsboothd.mbn $(OUTBIN)
	cp $(OUTBIN) $(APPSBOOTHEADER_DIR)/appsboot.raw
	cat $(APPSBOOTHEADER_DIR)/appsboothd.mbn $(OUTBIN) > $(APPSBOOTHEADER_DIR)/appsboot.mbn
	rm -f $(APPSBOOTHEADER_DIR)/appsboothd.mbn

appsboothd.mbn: mkheader $(OUTBIN)
	$(BUILDDIR)/mkheader $(OUTBIN) $(APPSBOOTHEADER_DIR)/appsboothd.mbn

EMMCBOOT.MBN: emmc_appsboothd.mbn $(OUTBIN)
	cp $(OUTBIN) $(APPSBOOTHEADER_DIR)/emmc_appsboot.raw
	cat $(APPSBOOTHEADER_DIR)/emmc_appsboothd.mbn $(OUTBIN) > $(APPSBOOTHEADER_DIR)/EMMCBOOT.MBN
	cat $(APPSBOOTHEADER_DIR)/emmc_appsboothd.mbn $(OUTBIN) > $(APPSBOOTHEADER_DIR)/emmc_appsboot.mbn
	rm -f $(APPSBOOTHEADER_DIR)/emmc_appsboothd.mbn

emmc_appsboothd.mbn: mkheader $(OUTBIN)
	$(BUILDDIR)/mkheader $(OUTBIN) $(APPSBOOTHEADER_DIR)/emmc_appsboothd.mbn

mkheader: $(SRC_DIR)/mkheader.c
	@mkdir -p $(BUILDDIR)
	${COMPILER} -DMEMBASE=$(MEMBASE) $(SRC_DIR)/mkheader.c -o $(BUILDDIR)/mkheader
