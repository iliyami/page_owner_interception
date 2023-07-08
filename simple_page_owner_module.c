#include <linux/kprobes.h>

#define PROC_FILENAME "page_order"


// The function pointer to store the original set_page_owner function
static void (*set_page_owner_orig)(struct page *);

// The kprobe pre-handler function
static int set_page_owner_pre(struct kprobe *kp, struct pt_regs *regs)
{
    static int counter = 0;
    if (counter < 5) {
        struct page *page = (struct page *)regs->di; // first arg of the __set_page_owner is in the rdi register in x86-64 arch
        pr_err("__set_page_owner called with page pointer: %p\n", page);
        counter++;
    } else {
        unregister_kprobe(kp);
    }
    return 0;
}

// Declare the kprobe instance
static struct kprobe kp = {
    .symbol_name = "__set_page_owner",
    .pre_handler = set_page_owner_pre,
};

// The module initialization function
static int __init page_owner_module_init(void)
{
    // Register the kprobe
    int ret = register_kprobe(&kp);
    if (ret < 0) {
        pr_err("Failed to register kprobe: %d\n", ret);
        return ret;
    }

    // Store the original set_page_owner function
    set_page_owner_orig = (void *)kp.addr;

    pr_info("page_owner_module initialized\n");
    return 0;
}

// The module exit function
static void __exit page_owner_module_exit(void)
{
    // Unregister the kprobe
    unregister_kprobe(&kp);

    pr_info("page_owner_module exited\n");
}

module_init(page_owner_module_init);
module_exit(page_owner_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Iliya Mi");
MODULE_DESCRIPTION("A simple kernel module for intercepting set_page_owner function");