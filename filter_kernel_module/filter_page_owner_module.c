#include <linux/kprobes.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <linux/mm_types.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/fs_struct.h>

#define PROC_FILENAME "page_order"

struct page_order {
    int order;
    int participate;
};

static struct page_order page_orders[] = {
    {0, 100},
    {1, 20},
};

static int page_order_show(struct seq_file *m, void *v)
{
    int i;

    seq_puts(m, "Order\tParticipation\n");
    for (i = 0; i < ARRAY_SIZE(page_orders); i++)
        seq_printf(m, "%d\t%d\n", page_orders[i].order, page_orders[i].participate);

    return 0;
}

static int page_order_open(struct inode *inode, struct file *file)
{
    return single_open(file, page_order_show, NULL);
}

static const struct proc_ops page_order_ops = {
    .proc_open = page_order_open,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
};

// The function pointer to store the original set_page_owner function
static void (*set_page_owner_orig)(struct page *);

// The kprobe pre-handler function
static int set_page_owner_pre(struct kprobe *kp, struct pt_regs *regs)
{
    static int counter = 0;

    if (counter < 20)
    {
        unsigned int mainPageOrder = regs->si; // second arg of the __set_page_owner is in the rsi(si) register in x86-64 arch
        int i;

        counter++;

        for (i = 0; i < ARRAY_SIZE(page_orders); i++)
        {
            if (page_orders[i].order == mainPageOrder)
            {
                pr_info("Order: %d, Participation: %d\n", page_orders[i].order, page_orders[i].participate);
                break;
            }
        }
    }

    else
    {
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

    struct proc_dir_entry *proc_entry;
    int ret = 0;
    
    // Register the proc file mode=0 (reading access)
    proc_entry = proc_create(PROC_FILENAME, 0, NULL, &page_order_ops);
    if (!proc_entry) {
        pr_err("Failed to create /proc/%s\n", PROC_FILENAME);
        return -ENOMEM;
    }

    // Register the kprobe
    ret = register_kprobe(&kp);
    if (ret < 0) {
        pr_err("Failed to register kprobe: %d\n", ret);
        remove_proc_entry(PROC_FILENAME, NULL);
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
    remove_proc_entry(PROC_FILENAME, NULL);
    pr_info("page_owner_module exited\n");
}

module_init(page_owner_module_init);
module_exit(page_owner_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Iliya Mi");
MODULE_DESCRIPTION("A simple kernel module for intercepting set_page_owner function");