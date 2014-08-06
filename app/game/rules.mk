LOCAL_DIR := $(GET_LOCAL_DIR)

INCLUDES += -I$(LK_TOP_DIR)/platform/msm_shared/include

OBJS += \
	$(LOCAL_DIR)/game.o \
	$(LOCAL_DIR)/../aboot/fastboot.o

