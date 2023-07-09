// Kernel module, create a read/write proc file (VFS)
#include<linux/module.h>
#include<linux/init.h>
#include<linux/sched.h>
#include<linux/uaccess.h>
#include<linux/fs.h>
#include<linux/seq_file.h>
#include<linux/slab.h>
#include<linux/proc_fs.h>

#define PROC_FILENAME "page_order"


struct page_order {
    int order;
    int participate;
};

static struct page_order page_orders[] = {
    {0, 20},
    {1, 50},
    {2, 80},
    {3, 100},
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

// The module initialization function
static int __init page_order_proc_module_init(void)
{
    // Register the proc file mode=0 (reading access)
    struct proc_dir_entry *proc_entry = proc_create(PROC_FILENAME, 0, NULL, &page_order_ops);
    if (!proc_entry) {
        pr_err("Failed to create /proc/%s\n", PROC_FILENAME);
        return -ENOMEM;
    }

    printk(KERN_INFO "page order proc initialized successfully!\n");
    return 0;
}

static void __exit page_order_proc_module_exit(void){
	remove_proc_entry(PROC_FILENAME,NULL);
	printk(KERN_INFO "%s module removed successfully!\n", PROC_FILENAME);
}

module_init(page_order_proc_module_init);
module_exit(page_order_proc_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Iliya Mi");
MODULE_DESCRIPTION("A simple kernel module for creating page order proc");