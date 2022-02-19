EFI_DIR = libs/efi
EFI_BUILD_DIR = $(BUILD_DIR)/$(EFI_DIR)
EFI_SRC = $(EFI_DIR)/efi.c
EFI_H = $(EFI_DIR)/efi.h
EFI_OBJ = $(patsubst $(EFI_DIR)/%, $(EFI_BUILD_DIR)/%.o, $(EFI_SRC))

EFI_CC = clang
EFI_AR = llvm-ar
EFI_CFLAGS = -target x86_64-pc-unknown-windows -ffreestanding -mno-red-zone -std=c11 -Ilibs

EFI_LIB = $(EFI_BUILD_DIR)/libefi.a

ALL += $(EFI_LIB)

$(EFI_LIB): $(EFI_OBJ)
	@$(MKCWD)
	$(EFI_AR) rcs $@ $^

$(EFI_BUILD_DIR)/%.c.o: $(EFI_DIR)/%.c $(EFI_H)
	@$(MKCWD)
	$(EFI_CC) $(EFI_CFLAGS) -c -o $@ $<


