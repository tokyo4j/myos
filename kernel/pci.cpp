#include "pci.hpp"
#include "kmalloc.hpp"
#include <int.hpp>
#include <screen.hpp>
#include <utils.hpp>
#include <x86.hpp>

#define PCI_ADDR_PORT 0xcf8
#define PCI_DATA_PORT 0xcfc

#define PCI_ENCODE_ADDR(bus, slot, func, offset) \
    ((u32)0x80000000 | (u32)(bus) << 16 | (u32)(slot) << 11 | (u32)func << 8 | (u32)offset)

static u32 read_pci_conf_32(u8 bus, u8 slot, u8 func, u8 offset)
{
    u32 addr = PCI_ENCODE_ADDR(bus, slot, func, offset);
    outl(PCI_ADDR_PORT, addr);
    return inl(PCI_DATA_PORT);
}

// returns 0 if invalid, 1 otherwise
static int try_read_pci_conf(u8 bus, u8 slot, u8 func, PCIConfigStructure* dst)
{
    u16 vendor_id = read_pci_conf_32(bus, slot, func, 0x00) & 0x0000ffff;
    if (vendor_id == 0xffff)
        return 0;

    for (u8 i = 0; i < 16; i++)
        ((u32*)dst)[i] = read_pci_conf_32(bus, slot, 0, i << 2);

    return 1;
}

static void scan_pci()
{
    for (int bus = 0; bus < 256; bus++) {
        for (int slot = 0; slot < 32; slot++) {
            for (int func = 0; func < 8; func++) {
                PCIConfigStructure buf;
                int status = try_read_pci_conf(bus, slot, func, &buf);
                if (status == 0)
                    break;

                kprintf("vendor id: %04x, ", buf.c.vendor_id);
                kprintf("device id: %04x, ", buf.c.device_id);
                kprintf("class: %02x, ", buf.c.class_code);
                kprintf("subclass: %02x, ", buf.c.subclass);
                kprintf("header type: %02x\n", buf.c.header_type);

                if (!(buf.c.header_type & 0x80))
                    break;
            }
        }
    }
}

void init_pci()
{
    scan_pci();
}