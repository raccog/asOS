include $(MAKE_DIR)/*.mk

HW_SRC_DIR = $(ROOT_DIR)/libs/hw
HW_BIN_DIR = $(BIN_DIR)/$(TARGET)/hw

HW_SRC = 	mmap.c
HW_H =		mmap.h \
			efi/efi.h
HW_OBJ = $(patsubst %, $(HW_BIN_DIR)/%.o, $(HW_SRC))

HW_CC = clang
HW_AR = llvm-ar
HW_CFLAGS = $(CFLAGS) -target $(TARGET) -std=c11 -I$(ROOT_DIR)/libs -D__LIB__='"HWLIB"'

HW_LIB = $(BIN_DIR)/$(TARGET)/libhardware.a

all: $(HW_LIB)
.PHONY: all

$(HW_LIB): $(HW_OBJ)
	$(MKCWD)
	$(HW_AR) rcs $@ $^

$(HW_BIN_DIR)/%.c.o: %.c $(HW_H)
	$(MKCWD)
	$(HW_CC) $(HW_CFLAGS) -c -o $@ $<

