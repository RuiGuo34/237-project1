#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rui Guo");
MODULE_DESCRIPTION("PMUON");

int init_module(void) {
    unsigned int v;
    printk("Turn PMU on\n");

    // 1. Enable "User Enable Register"
    asm volatile("mcr p15, 0, %0, c9, c14, 0\n\t" :: "r" (0x00000001)); 

    // 2. Reset Performance Monitor Control Register(PMCR), Count Enable Set Register, and Overflow Flag Status Register
    asm volatile ("mcr p15, 0, %0, c9, c12, 0\n\t" :: "r"(0x00000017));
    asm volatile ("mcr p15, 0, %0, c9, c12, 1\n\t" :: "r"(0x8000003f));
    asm volatile ("mcr p15, 0, %0, c9, c12, 3\n\t" :: "r"(0x8000003f));

    // 3. Disable Interrupt Enable Clear Register
    asm volatile("mcr p15, 0, %0, c9, c14, 2\n\t" :: "r" (~0));

    // 4. Read how many event counters exist 
    asm volatile("mrc p15, 0, %0, c9, c12, 0\n\t" : "=r" (v)); // Read PMCR
    printk("pmon_init(): have %d configurable event counters.\n", (v >> 11) & 0x1f);

    // 5. Set event counter registers 
    // ***** YOUR CODE STARTS HERE *****
	asm volatile ("mcr p15, 0, %0, c9, c12, 5\n\t" :: "r"(0x0)); //select first event counter
	asm volatile ("mcr p15, 0, %0, c9, c13, 1\n\t" :: "r"(0x4)); //L1 Data Cache Access
	asm volatile ("mrc p15, 0, %0, c9, c13, 2\n\t" : "=r" (v));
	printk("counter 1 value %d\n", v);

	asm volatile ("mcr p15, 0, %0, c9, c12, 5\n\t" :: "r"(0x1)); //select second event counter
	asm volatile ("mcr p15, 0, %0, c9, c13, 1\n\t" :: "r"(0x3)); //L1 Data Cache Miss
	asm volatile ("mrc p15, 0, %0, c9, c13, 2\n\t" : "=r" (v));
	printk("counter 2 value %d\n", v);

	asm volatile ("mcr p15, 0, %0, c9, c12, 5\n\t" :: "r"(0x2)); //select third event counter
	asm volatile ("mcr p15, 0, %0, c9, c13, 1\n\t" :: "r"(0x16)); //L2 Data Cache Access
	asm volatile ("mrc p15, 0, %0, c9, c13, 2\n\t" : "=r" (v));
	printk("counter 3 value %d\n", v);


	asm volatile ("mcr p15, 0, %0, c9, c12, 5\n\t" :: "r"(0x3)); //select fourth event counter
	asm volatile ("mcr p15, 0, %0, c9, c13, 1\n\t" :: "r"(0x17)); //L2 Data Cache Miss
	asm volatile ("mrc p15, 0, %0, c9, c13, 2\n\t" : "=r" (v));
	printk("counter 4 value %d\n", v);

	asm volatile ("mcr p15, 0, %0, c9, c12, 5\n\t" :: "r"(0x4)); //select fifth event counter
	asm volatile ("mcr p15, 0, %0, c9, c13, 1\n\t" :: "r"(0xC9)); //Conditional Branch Executed
	asm volatile ("mrc p15, 0, %0, c9, c13, 2\n\t" : "=r" (v));
	printk("counter 5 value %d\n", v);

	asm volatile ("mcr p15, 0, %0, c9, c12, 5\n\t" :: "r"(0x5)); //select sixth event counter
	asm volatile ("mcr p15, 0, %0, c9, c13, 1\n\t" :: "r"(0xCC)); //Conditional Branch mispredicted
	asm volatile ("mrc p15, 0, %0, c9, c13, 2\n\t" : "=r" (v));
	printk("counter 6 value %d\n", v);
	printk("finished\n");
    // ***** YOUR CODE ENDS HERE *******
    return 0;
}

void cleanup_module(void) {
    printk("GOODBYE, PMU Off\n");
}


