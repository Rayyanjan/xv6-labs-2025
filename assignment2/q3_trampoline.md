### Q3: Could xv6 be modified to eliminate the special TRAMPOLINE page mapping?

**Short answer:** In principle yes, but the current xv6 design uses the TRAMPOLINE page to keep trap entry/return very simple, so xv6 keeps it.

---

#### What xv6 does now

- In `kernel/vm.c`, function `proc_pagetable(struct proc *p)`, xv6 maps:

  1. `TRAMPOLINE` → a page of code (`trampoline.S`)
  2. `TRAPFRAME`  → the process’s trapframe page

  Example (from `vm.c`):

  ```c
  // map trampoline code at high address
  mappages(pagetable, TRAMPOLINE, PGSIZE,
           (uint64)trampoline, PTE_R | PTE_X);

The virtual addresses are defined in kernel/memlayout.h:

#define TRAMPOLINE (MAXVA - PGSIZE)
#define TRAPFRAME  (TRAMPOLINE - PGSIZE)
The mapped page contains uservec (trap from user) and userret (return to user) in kernel/trampoline.S.
This page is mapped at the same VA in every process so the kernel always knows where to jump.

####  Why xv6 wants this page:

A trap happens while the CPU is still using the user page table.

The kernel wants to run a small piece of trusted code at that moment.

User page tables do not map the whole kernel, so xv6 maps exactly one kernel-controlled page (the TRAMPOLINE) into every user page table.

Because it is at the same address for all processes, the code in trampoline.S can be written once and used for all of them.

### Could we remove it?
Yes, but we would have to change the trap entry to do an immediate page-table switch.
One possible alternative design:

On trap entry, immediately switch from the user page table to the kernel page table (load the kernel satp).

After switching, the kernel text is visible, so the trap/return code could live in normal kernel memory instead of in a special mapped page.

When returning to user, switch back to the user page table and execute sret.

This eliminates the need to map the TRAMPOLINE page into every user address space.
However, it makes the assembly path longer and more complex. xv6 is a teaching OS and prefers the current simpler approach: “map 1 code page everywhere and use it for all traps.”

### Conclusion:
xv6 can be modified to remove the special TRAMPOLINE mapping if we are willing to change the early trap sequence to switch to the kernel page table first.
The stock xv6 keeps the TRAMPOLINE page to keep trampoline.S simple and to guarantee a known, mapped entry point in every user address space.
