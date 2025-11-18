#include "sys.h"

void WFI_SET(void)
{
    __ASM volatile("wfi");
}

void INTX_DISABLE(void)
{          
    __ASM volatile("cpsid i");
}

void INTX_ENABLE(void)
{
    __ASM volatile("cpsie i");
}

void MSR_MSP(u32 addr)
{
    __ASM volatile ("MSR MSP, %0" : : "r" (addr));
    __ASM volatile ("BX lr");
}