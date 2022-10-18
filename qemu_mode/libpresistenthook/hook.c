#include "../qemuafl/qemuafl/api.h"

#include <stdint.h>
#include <string.h>

void afl_persistent_hook(struct arm64_regs* regs, uint64_t guest_base, uint8_t* input_buf, uint32_t input_buf_len)
{
    for (int i = 0; i < input_buf_len; ++i) {
        ((uint8_t*) (regs->x0))[i] = input_buf[i];
    }
    regs->x1 = input_buf_len;

    return;
}

int afl_persistent_hook_init(void)
{
    return 1;
}
