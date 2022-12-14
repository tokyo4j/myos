#pragma once
#include <int.hpp>

struct PCIConfigStructure {
    struct {
        u16 vendor_id;
        u16 device_id;
        u16 command;
        u16 status;
        u8 revision_id;
        u8 prog_if;
        u8 subclass;
        u8 class_code;
        u8 cache_line_size;
        u8 latency_timer;
        u8 header_type;
        u8 bist;
    } __attribute__((packed)) c;
    union {
        struct {
            u32 bar[6];
            u32 cic_ptr;
            u16 sub_vendor_id;
            u16 sub_id;
            u32 exrom_base_addr;
            u8 cap_ptr;
            u8 reserved1[3];
            u32 reserved2;
            u8 int_line;
            u8 int_pin;
            u8 min_grant;
            u8 max_latency;
        } __attribute__((packed)) type1;
        struct {
            u32 bar[2];
            u8 primary_bus_no;
            u8 secondary_bus_no;
            u8 sub_bus_num;
            u8 secondary_latency_timer;
            u8 io_base;
            u8 io_limit;
            u16 secondary_status;
            u16 mem_base;
            u16 mem_limit;
            u16 prefetchable_mem_base;
            u16 prefetchable_mem_limit;
            u32 prefetchable_base_upper;
            u32 prefetchable_limit_upper;
            u16 io_base_upper;
            u16 io_limit_upper;
            u8 cap_ptr;
            u8 reserved1[3];
            u32 exrom_base_addr;
            u8 int_line;
            u8 int_pin;
            u16 bridge_ctl;
        } __attribute__((packed)) type2;
    } u;
} __attribute__((packed));

void init_pci();