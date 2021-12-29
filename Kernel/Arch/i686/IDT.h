#pragma once

#include <stdint.h>

struct InterruptDescriptor {
    uint16_t offset0;
    uint16_t selector;
    uint8_t zeros;
    uint8_t gate;
    uint16_t offset1;

    InterruptDescriptor(uint32_t offset, uint16_t selector, uint8_t gate)
        : offset0{static_cast<uint16_t>(offset & 0xffff)}, selector{selector},
        zeros{0x0000}, gate{gate}, offset1{static_cast<uint16_t>(offset >> 16)} {}
} __attribute__ ((packed));

void setupIDT();
