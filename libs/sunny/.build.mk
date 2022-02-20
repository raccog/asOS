SUNNY_DIR = libs/sunny
SUNNY_BUILD_DIR = $(BUILD_DIR)/$(SUNNY_DIR)
SUNNY_SRC = $(SUNNY_DIR)/mmap.c
SUNNY_H = $(SUNNY_DIR)/mmap.h
SUNNY_OBJ = $(patsubst $(SUNNY_DIR)/%, $(SUNNY_BUILD_DIR)/%.o, $(SUNNY_SRC))

SUNNY_CC = clang
SUNNY_AR = llvm-ar
SUNNY_CFLAGS = -target x86_64-pc-unknown-windows -ffreestanding -mno-red-zone -std=c11 -Ilibs

SUNNY_H_DEPS = $(STD_H) $(EFI_H)
SUNNY_LIB_DEPS = $(STD_LIB) $(EFI_LIB)

SUNNY_LIB = $(SUNNY_BUILD_DIR)/libsunny.a

ALL += $(SUNNY_LIB)

$(SUNNY_LIB): $(SUNNY_OBJ) $(SUNNY_LIB_DEPS)
	@$(MKCWD)
	$(SUNNY_AR) rcs $@ $^

$(SUNNY_BUILD_DIR)/%.c.o: $(SUNNY_DIR)/%.c $(SUNNY_H) $(SUNNY_H_DEPS)
	@$(MKCWD)
	$(SUNNY_CC) $(SUNNY_CFLAGS) -c -o $@ $<


