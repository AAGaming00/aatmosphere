/*
 * Copyright (C) Switch-OC-Suite
 *
 * Copyright (c) 2023 hanai3Bi
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "../oc_common.hpp"
#include "pcv_common.hpp"

namespace ams::ldr::oc::pcv {

namespace mariko {
    constexpr cvb_entry_t CpuCvbTableDefault[] = {
        // CPUB01_CVB_TABLE
        {  204000, {  721589, -12695, 27 }, {} },
        {  306000, {  747134, -14195, 27 }, {} },
        {  408000, {  776324, -15705, 27 }, {} },
        {  510000, {  809160, -17205, 27 }, {} },
        {  612000, {  845641, -18715, 27 }, {} },
        {  714000, {  885768, -20215, 27 }, {} },
        {  816000, {  929540, -21725, 27 }, {} },
        {  918000, {  976958, -23225, 27 }, {} },
        { 1020000, { 1028021, -24725, 27 }, { 1120000 } },
        { 1122000, { 1082730, -26235, 27 }, { 1120000 } },
        { 1224000, { 1141084, -27735, 27 }, { 1120000 } },
        { 1326000, { 1203084, -29245, 27 }, { 1120000 } },
        { 1428000, { 1268729, -30745, 27 }, { 1120000 } },
        { 1581000, { 1374032, -33005, 27 }, { 1120000 } },
        { 1683000, { 1448791, -34505, 27 }, { 1120000 } },
        { 1785000, { 1527196, -36015, 27 }, { 1120000 } },
        { 1887000, { 1609246, -37515, 27 }, { 1120000 } },
        { 1963500, { 1675751, -38635, 27 }, { 1120000 } },
        { },
    };

    constexpr u16 CpuMinVolts[] = { 800, 637, 620, 610 };

    constexpr u32 CpuClkOfficial  = 1963'500;
    constexpr u32 CpuVoltOfficial = 1120;

    constexpr cvb_entry_t GpuCvbTableDefault[] = {
        // GPUB01_NA_CVB_TABLE
        {  25000, { }, {  814294, 8144, -940, 808, -21583, 226 } },
        {  50000, { }, {  856185, 8144, -940, 808, -21583, 226 } },
        {  75000, { }, {  898077, 8144, -940, 808, -21583, 226 } },
        {  100000, { }, {  939968, 8144, -940, 808, -21583, 226 } },
        {  125000, { }, {  981860, 8144, -940, 808, -21583, 226 } },
        {  15000, { }, { 1023751, 8144, -940, 808, -21583, 226 } },
        {  300000, { }, { 1065642, 8144, -940, 808, -21583, 226 } },
        {  500000, { }, { 1107534, 8144, -940, 808, -21583, 226 } },
        {  800000, { }, { 1149425, 8144, -940, 808, -21583, 226 } },
        {  1000000, { }, { 1191317, 8144, -940, 808, -21583, 226 } },
        {  1300000, { }, { 1233208, 8144, -940, 808, -21583, 226 } },
        {  1500000, { }, { 1275100, 8144, -940, 808, -21583, 226 } },
        { },
    };

    constexpr u32 GpuClkPllLimit  = 1300'000'000;

    /* GPU Max Clock asm Pattern:
    *
    * MOV W11, #0x1000 MOV (wide immediate)                0x1000                              0xB (11)
    *  sf | opc |                 | hw  |                   imm16                        |      Rd
    * #31 |30 29|28 27 26 25 24 23|22 21|20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5 |4  3  2  1  0
    *   0 | 1 0 | 1  0  0  1  0  1| 0  0| 0  0  0  1  0  0  0  0  0  0  0  0  0  0  0  0 |0  1  0  1  1
    *
    * MOVK W11, #0xE, LSL#16     <shift>16                    0xE                              0xB (11)
    *  sf | opc |                 | hw  |                   imm16                        |      Rd
    * #31 |30 29|28 27 26 25 24 23|22 21|20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5 |4  3  2  1  0
    *   0 | 1 1 | 1  0  0  1  0  1| 0  1| 0  0  0  0  0  0  0  0  0  0  0  0  1  1  1  0 |0  1  0  1  1
    */
    inline constexpr u32 asm_pattern[] = { 0x52820000, 0x72A001C0 };
    inline auto asm_compare_no_rd = [](u32 ins1, u32 ins2) { return ((ins1 ^ ins2) >> 5) == 0; };
    inline auto asm_get_rd = [](u32 ins) { return ins & ((1 << 5) - 1); };
    inline auto asm_set_rd = [](u32 ins, u8 rd) { return (ins & 0xFFFFFFE0) | (rd & 0x1F); };
    inline auto asm_set_imm16 = [](u32 ins, u16 imm) { return (ins & 0xFFE0001F) | ((imm & 0xFFFF) << 5); };

    inline bool GpuMaxClockPatternFn(u32* ptr32) {
        return asm_compare_no_rd(*ptr32, asm_pattern[0]);
    }

    constexpr emc_dvb_dvfs_table_t EmcDvbTableDefault[] = {
        {  204000, {  637,  637,  637, } },
        {  408000, {  637,  637,  637, } },
        {  800000, {  637,  637,  637, } },
        { 1065600, {  637,  637,  637, } },
        { 1331200, {  650,  637,  637, } },
        { 1600000, {  675,  650,  637, } },
    };

    constexpr u32 EmcClkOSAlt     = 1331'200;
    constexpr u32 EmcClkPllmLimit = 2133'000'000;
    constexpr u32 EmcVddqDefault  = 600'000;
    constexpr u32 MemVdd2Default  = 1100'000;

    constexpr u32 MTC_TABLE_REV = 3;

    void Patch(uintptr_t mapped_nso, size_t nso_size);

}

namespace erista {
    constexpr cvb_entry_t CpuCvbTableDefault[] = {
        // CPU_PLL_CVB_TABLE_ODN
        {  25000, {  721094 }, {} },
        {  50000, {  754040 }, {} },
        {  75000, {  786986 }, {} },
        {  100000, {  819932 }, {} },
        {  300000, {  852878 }, {} },
        {  500000, {  885824 }, {} },
        {  800000, {  918770 }, {} },
        { 1000000, {  951716 }, {} },
        { 1300000, {  984662 }, { -2875621,  358099, -8585 } },
        { 1500000, { 1017608 }, {   -52225,  104159, -2816 } },
        { 1800000, { 1050554 }, {  1076868,    8356,  -727 } },
        { 2000000, { 1083500 }, {  2208191,  -84659,  1240 } },
        { 2500000, { 1116446 }, {  2519460, -105063,  1611 } },
        { 2800000, { 1130000 }, {  2889664, -122173,  1834 } },
        { 3000000, { 1168000 }, {  5100873, -279186,  4747 } },
        { 3500000, { 1227500 }, {  5100873, -279186,  4747 } },
        { },
    };

    constexpr u32 CpuVoltL4T = 1300'000;

    constexpr u16 CpuMinVolts[] = { 950, 850, 825, 810 };

    inline bool CpuMaxVoltPatternFn(u32* ptr32) {
        u32 val = *ptr32;
        return (val == 1132 || val == 1170 || val == 1227);
    }

    constexpr u32 GpuClkPllLimit  = 921'600'000;

    /* GPU Max Clock asm Pattern:
    *
    * MOV W11, #0x1000 MOV (wide immediate)                0x1000                              0xB (11)
    *  sf | opc |                 | hw  |                   imm16                        |      Rd
    * #31 |30 29|28 27 26 25 24 23|22 21|20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5 |4  3  2  1  0
    *   0 | 1 0 | 1  0  0  1  0  1| 0  0| 0  0  0  1  0  0  0  0  0  0  0  0  0  0  0  0 |0  1  0  1  1
    *
    * MOVK W11, #0xE, LSL#16     <shift>16                    0xE                              0xB (11)
    *  sf | opc |                 | hw  |                   imm16                        |      Rd
    * #31 |30 29|28 27 26 25 24 23|22 21|20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5 |4  3  2  1  0
    *   0 | 1 1 | 1  0  0  1  0  1| 0  1| 0  0  0  0  0  0  0  0  0  0  0  0  1  1  1  0 |0  1  0  1  1
    */
    inline constexpr u32 asm_pattern[] = { 0x52820000, 0x72A001C0 };
    inline auto asm_compare_no_rd = [](u32 ins1, u32 ins2) { return ((ins1 ^ ins2) >> 5) == 0; };
    inline auto asm_get_rd = [](u32 ins) { return ins & ((1 << 5) - 1); };
    inline auto asm_set_rd = [](u32 ins, u8 rd) { return (ins & 0xFFFFFFE0) | (rd & 0x1F); };
    inline auto asm_set_imm16 = [](u32 ins, u16 imm) { return (ins & 0xFFE0001F) | ((imm & 0xFFFF) << 5); };

    inline bool GpuMaxClockPatternFn(u32* ptr32) {
        return asm_compare_no_rd(*ptr32, asm_pattern[0]);
    }

    constexpr cvb_entry_t GpuCvbTableDefault[] = {
        // NA_FREQ_CVB_TABLE
        {   76800, { }, {  814294, 8144, -940, 808, -21583, 226 } },
        {  153600, { }, {  856185, 8144, -940, 808, -21583, 226 } },
        {  230400, { }, {  898077, 8144, -940, 808, -21583, 226 } },
        {  307200, { }, {  939968, 8144, -940, 808, -21583, 226 } },
        {  384000, { }, {  981860, 8144, -940, 808, -21583, 226 } },
        {  460800, { }, { 1023751, 8144, -940, 808, -21583, 226 } },
        {  537600, { }, { 1065642, 8144, -940, 808, -21583, 226 } },
        {  614400, { }, { 1107534, 8144, -940, 808, -21583, 226 } },
        {  691200, { }, { 1149425, 8144, -940, 808, -21583, 226 } },
        {  768000, { }, { 1191317, 8144, -940, 808, -21583, 226 } },
        {  844800, { }, { 1233208, 8144, -940, 808, -21583, 226 } },
        {  921600, { }, { 1275100, 8144, -940, 808, -21583, 226 } },
        { },
    };

    constexpr u32 MemVoltHOS      = 1125'000;
    constexpr u32 EmcClkPllmLimit = 1866'000'000;

    constexpr u32 MTC_TABLE_REV = 7;

    void Patch(uintptr_t mapped_nso, size_t nso_size);
}

template<bool isMariko>
Result CpuFreqCvbTable(u32* ptr) {
    cvb_entry_t* default_table = isMariko ? (cvb_entry_t *)(&mariko::CpuCvbTableDefault) : (cvb_entry_t *)(&erista::CpuCvbTableDefault);
    cvb_entry_t* customize_table = const_cast<cvb_entry_t *>(isMariko ? (C.marikoCpuUV ? C.marikoCpuDvfsTableSLT : C.marikoCpuDvfsTable) : C.eristaCpuDvfsTable);

    u32 cpu_max_volt = isMariko ? C.marikoCpuMaxVolt : C.eristaCpuMaxVolt;
    u32 cpu_freq_threshold = 1020'000;
    if (isMariko) {
        cpu_freq_threshold = C.marikoCpuUV ? 2193'000 : 2091'000;
    } else {
        cpu_freq_threshold = cpu_max_volt >= 1235 ? 1887'000 : 1428'000;
    }

    size_t default_entry_count = GetDvfsTableEntryCount(default_table);
    size_t default_table_size = default_entry_count * sizeof(cvb_entry_t);
    size_t customize_entry_count = GetDvfsTableEntryCount(customize_table);
    size_t customize_table_size = customize_entry_count * sizeof(cvb_entry_t);

    // Validate existing table
    cvb_entry_t* table_free = reinterpret_cast<cvb_entry_t *>(ptr) + 1;
    void* cpu_cvb_table_head = reinterpret_cast<u8 *>(table_free) - default_table_size;
    bool validated = std::memcmp(cpu_cvb_table_head, default_table, default_table_size) == 0;
    R_UNLESS(validated, ldr::ResultInvalidCpuDvfs());

    std::memcpy(cpu_cvb_table_head, static_cast<void *>(customize_table), customize_table_size);

    // Patch CPU max volt
    if (cpu_max_volt) {
        cvb_entry_t* entry = static_cast<cvb_entry_t *>(cpu_cvb_table_head);
        for (size_t i = 0; i < customize_entry_count; i++) {
            if (entry->freq >= cpu_freq_threshold) {
                if (isMariko) {
                    PATCH_OFFSET(&(entry->cvb_pll_param.c0), cpu_max_volt * 1000);
                } else {
                    PATCH_OFFSET(&(entry->cvb_dfll_param.c0), cpu_max_volt * 1000);
                }
            }
            entry++;
        }
    }

    R_SUCCEED();
}

template<bool isMariko>
Result GpuFreqCvbTable(u32* ptr) {
    cvb_entry_t* default_table = isMariko ? (cvb_entry_t *)(&mariko::GpuCvbTableDefault) : (cvb_entry_t *)(&erista::GpuCvbTableDefault);
    cvb_entry_t* customize_table;
    if (isMariko) {
        switch (C.marikoGpuUV) {
            case 0:
                customize_table = const_cast<cvb_entry_t *>(C.marikoGpuDvfsTable);
                break;
            case 1:
                customize_table = const_cast<cvb_entry_t *>(C.marikoGpuDvfsTableSLT);
                break;
            case 2:
                customize_table = const_cast<cvb_entry_t *>(C.marikoGpuDvfsTableHiOPT);
                break;
            default:
                customize_table = const_cast<cvb_entry_t *>(C.marikoGpuDvfsTable);
                break;
        }
    } else {
        customize_table = const_cast<cvb_entry_t *>(C.eristaGpuDvfsTable);
    }

    size_t default_entry_count = GetDvfsTableEntryCount(default_table);
    size_t default_table_size = default_entry_count * sizeof(cvb_entry_t);
    size_t customize_entry_count = GetDvfsTableEntryCount(customize_table);
    size_t customize_table_size = customize_entry_count * sizeof(cvb_entry_t);

    // Validate existing table
    cvb_entry_t* table_free = reinterpret_cast<cvb_entry_t *>(ptr) + 1;
    void* gpu_cvb_table_head = reinterpret_cast<u8 *>(table_free) - default_table_size;
    bool validated = std::memcmp(gpu_cvb_table_head, default_table, default_table_size) == 0;
    R_UNLESS(validated, ldr::ResultInvalidGpuDvfs());

    std::memcpy(gpu_cvb_table_head, (void*)customize_table, customize_table_size);

    // Patch GPU volt
    if (isMariko && C.marikoGpuUV == 3) {
        cvb_entry_t* entry = static_cast<cvb_entry_t *>(gpu_cvb_table_head);
        for (size_t i = 0; i < customize_entry_count; i++) {
            PATCH_OFFSET(&(entry->cvb_pll_param.c0), C.marikoGpuVoltArray[i] * 1000);
            PATCH_OFFSET(&(entry->cvb_pll_param.c1), 0);
            PATCH_OFFSET(&(entry->cvb_pll_param.c2), 0);
            PATCH_OFFSET(&(entry->cvb_pll_param.c3), 0);
            PATCH_OFFSET(&(entry->cvb_pll_param.c4), 0);
            PATCH_OFFSET(&(entry->cvb_pll_param.c5), 0);
            entry++;
        }
    }
    else if (C.commonGpuVoltOffset) {
        cvb_entry_t* entry = static_cast<cvb_entry_t *>(gpu_cvb_table_head);
        for (size_t i = 0; i < customize_entry_count; i++) {
            PATCH_OFFSET(&(entry->cvb_pll_param.c0), (entry->cvb_pll_param.c0 - C.commonGpuVoltOffset*1000));
            entry++;
        }
    }

    R_SUCCEED();
};

Result MemFreqPllmLimit(u32* ptr);
Result MemVoltHandler(u32* ptr); // Used for Erista MEM Vdd2 + EMC Vddq or Mariko MEM Vdd2

template<typename T>
Result MemMtcCustomizeTable(T* dst, T* src) {
    constexpr u32 mtc_magic = std::is_same_v<T, MarikoMtcTable> ? MARIKO_MTC_MAGIC : ERISTA_MTC_MAGIC;
    R_UNLESS(src->rev == mtc_magic, ldr::ResultInvalidMtcMagic());

    constexpr u32 ZERO_VAL = UINT32_MAX;
    // Skip params from dvfs_ver to clock_src;
    for (size_t offset = offsetof(T, clk_src_emc); offset < sizeof(T); offset += sizeof(u32)) {
        u32* src_ent = reinterpret_cast<u32 *>(reinterpret_cast<size_t>(src) + offset);
        u32* dst_ent = reinterpret_cast<u32 *>(reinterpret_cast<size_t>(dst) + offset);
        u32 src_val = *src_ent;

        if (src_val) {
            PATCH_OFFSET(dst_ent, src_val == ZERO_VAL ? 0 : src_val);
        }
    }

    R_SUCCEED();
};

void SafetyCheck();
void Patch(uintptr_t mapped_nso, size_t nso_size);

}
