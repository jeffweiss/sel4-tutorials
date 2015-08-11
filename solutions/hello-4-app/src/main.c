/*
 * Copyright 2014, NICTA
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(NICTA_BSD)
 */

#include <autoconf.h>

#include <stdio.h>

#include <assert.h>
#include <stdlib.h>

#include <sel4/sel4.h>

#include <utils/util.h>

/* dummy global for libsel4muslcsys */
char _cpio_archive[1];

void abort(void) {
    while (1);
}

void __arch_putchar(int c) {
#ifdef CONFIG_DEBUG_BUILD
    seL4_DebugPutChar(c);
#endif
}

#define EP_CPTR 0x3
#define EP_BADGE 0x61


int
main(int argc, char **argv)
{
    UNUSED seL4_Word sender_badge;
    UNUSED seL4_MessageInfo_t tag;
    UNUSED seL4_Word msg;

    printf("process_2: hey hey hey\n");

    tag = seL4_Wait(EP_CPTR, &sender_badge);
    assert(sender_badge == EP_BADGE);

    assert(seL4_MessageInfo_get_length(tag) == 1);
    msg = seL4_GetMR(0); 
    printf("process_2: got a message %#x from %#x\n", msg, sender_badge);

    seL4_SetMR(0, ~msg);
    seL4_ReplyWait(EP_CPTR, tag, &sender_badge);

    return 0;
}

