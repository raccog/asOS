#include "Kernel/Arch/i686/IDT.h"

#include "StdLib/StdLib.h"

uint8_t *IDT = reinterpret_cast<uint8_t *>(0x10010);

extern "C" void isr0();

void setupIDT() {
    auto descriptor0 = InterruptDescriptor(reinterpret_cast<uint32_t>(isr0), 0x0008, 0x8e);
    memcpy(IDT, reinterpret_cast<uint8_t *>(&descriptor0), sizeof(InterruptDescriptor));
}
