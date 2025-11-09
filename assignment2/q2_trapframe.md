### Q2: Is there a way to eliminate the special TRAPFRAME page mapping in every user address space?

**Short answer:** Not with the current xv6 trap sequence. The trapframe is needed because, on a trap from user → kernel, the CPU is still using the *user* page table, so kernel memory (kernel stack, struct proc) is not yet mapped. The trampoline (`uservec` in `trampoline.S`) needs some place that *is* mapped to save the 32 user registers, so xv6 maps one kernel-owned page (the trapframe) into each user page table.

**Why it’s needed now :**
1. User code is running with the user pagetable.
2. A trap occurs → hardware jumps to `uservec` (trampoline).
3. At this moment, we are still on the user pagetable.
4. Kernel stack and `struct proc` live in kernel memory → not visible.
5. Therefore, `uservec` stores registers into the trapframe page, which *is* mapped in the user pagetable at a fixed VA.

This mapping is set up in `kernel/vm.c` (in `proc_pagetable(...)`) where xv6 does:
- map TRAMPOLINE
- map TRAPFRAME (the process’s `p->trapframe`) into the user pagetable

**Could we eliminate it?**
Yes, but only if we change the entry sequence. One way:
1. On trap entry, immediately switch from the user pagetable to the kernel pagetable (i.e. load the kernel `satp`).
2. Now kernel memory is visible.
3. Now we can save the 32 registers directly on the kernel stack or inside the proc structure.
4. Continue with the normal trap handling.

**Tradeoff:**
- Current xv6 = simpler: just map 1 page per process.
- Alternative = no special mapping, but trap entry assembly gets more complex because it must switch page tables first.

**Conclusion:** It is possible in principle, but xv6 keeps the special TRAPFRAME mapping to keep `trampoline.S` simple.

