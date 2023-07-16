# Kernel Module - Page Order Filtering

This kernel module creates a proc file named "page_order" and implements functionality to filter page orders based on the provided values. Additionally, the module intercepts the `__set_page_owner` function to monitor page ownership changes.

## Proc File

The module creates a proc file named "page_order" to provide information about the page orders and their participation. When the proc file is read, it displays the order and participation values in a tabular format.

### Reading the Proc File

The `page_order_show` function is called when the proc file is read. It formats and displays the order and participation values for each entry in the `page_orders` array.

### Opening the Proc File

The `page_order_open` function is called when the proc file is opened. It sets up the file for reading by calling `single_open` with the `page_order_show` function.

### Proc File Operations

The `page_order_ops` structure defines the proc file operations, including the open, read, lseek, and release functions.

## Interception of __set_page_owner Function

The module intercepts the `__set_page_owner` function using a kprobe to monitor page ownership changes.

### Pre-Handler Function

The `set_page_owner_pre` function serves as the pre-handler for the kprobe. It is called before the execution of the original `__set_page_owner` function. In this function, the main page order is retrieved from the `si` register (x86-64 architecture) and compared with the page orders defined in the `page_orders` array. If a match is found, information about the order and participation is printed.

### Kprobe Initialization

The `kp` structure defines the kprobe instance. It specifies the symbol name to be intercepted (`__set_page_owner`) and the pre-handler function (`set_page_owner_pre`).

## Module Initialization and Cleanup

The `page_owner_module_init` function is the entry point for the module. It registers the proc file and the kprobe. If the registration fails, appropriate error messages are displayed, and the module initialization fails.

The `page_owner_module_exit` function is called when the module is unloaded. It unregisters the kprobe and removes the proc file.

## Module Information

The module specifies the license type, author, and a brief description using the `MODULE_LICENSE`, `MODULE_AUTHOR`, and `MODULE_DESCRIPTION` macros, respectively.


---


# BPF Script - Page Owner Information

This BPF script is designed to capture and log various events related to the page owner information for memory pages in the Linux kernel. It utilizes kprobes to hook into specific kernel functions and print relevant information when those functions are executed.

## Kprobes

The script defines multiple kprobes to capture different events related to page owner information. Each kprobe is associated with a specific kernel function and triggers when that function is called.

### Kprobe: __set_page_owner

This kprobe is triggered when the page owner information for a page of memory is set to a new value. It captures the process ID (pid) and process name (comm) of the newly assigned owner and logs this information along with the timestamp.

### Kprobe: __dump_page_owner

This kprobe is triggered when the page owner information for a page of memory is dumped (i.e., printed to the kernel log). It captures the process ID (pid) and process name (comm) and logs this information along with the timestamp.

### Kprobe: init_page_owner

This kprobe is triggered when the kernel initializes the page owner data structures for a process. It captures the process ID (pid) and process name (comm) and logs this information along with the timestamp.

### Kprobe: read_page_owner

This kprobe is triggered when a process reads the page owner information for a page of memory. It captures the process ID (pid) and process name (comm) and logs this information along with the timestamp.

### Kprobe: need_page_owner

This kprobe is triggered when the kernel determines that a page of memory needs a page owner assigned to it. The page owner is assigned based on the process that last accessed the page. It captures the process ID (pid) and process name (comm) and logs this information along with the timestamp.

### Kprobe: __reset_page_owner

This kprobe is triggered when the page owner information for a page of memory is reset (cleared). It captures the process ID (pid) and process name (comm) and logs this information along with the timestamp.

### Kprobe: __copy_page_owner

This kprobe is triggered when the page owner information for a page of memory is copied from one page to another. It captures the process ID (pid) and process name (comm) and logs this information along with the timestamp.

### Kprobe: __split_page_owner

This kprobe is triggered when a page owner is split across multiple processes. It captures the process ID (pid) and process name (comm) and logs this information along with the timestamp.

### Kprobe: __set_page_owner_migrate_reason

This kprobe is triggered when a page owner is migrated from one process to another. It captures the process ID (pid) and logs this information along with the timestamp.

## Output Format

The script uses `printf` statements to log the captured information. The output format for each event is as follows:

- **Event: __set_page_owner**
  ```
  page_owner info {set}: timestamp=%llu pid=%d proc_name=%s
  ```

- **Event: __dump_page_owner**
  ```
  page_owner info {dump}: timestamp=%llu pid=%d proc_name=%s
  ```

- **Event: init_page_owner**
  ```
  page_owner info {init}: timestamp=%llu pid=%d proc_name=%s
  ```

- **Event: read_page_owner**
  ```
  page_owner info {read}: timestamp=%llu pid=%d proc_name=%s
  ```

- **Event: need_page_owner**
  ```
  page_owner info {need}: timestamp=%llu pid=%d proc_name=%s
  ```

- **Event: __reset_page_owner**
  ```
  page_owner info {reset}: timestamp=%llu pid=%d proc_name=%s
  ```

- **Event: __copy_page_owner**
  ```
  page_owner info {copy}: timestamp=%llu pid=%d proc_name=%s
  ```

- **Event: __split_page_owner**
  ```
  page_owner info {split}: timestamp=%llu pid=%d proc_name=%s
  ```

- **Event: __set_page_owner_migrate_reason**
  ```
  page_owner info {set migrate}: timestamp=%llu pid=%d
  ```


---


Please check the provided report [PDF file](https://github.com/iliyami/page_owner_interception/blob/master/LinuxFinalProjectReport.pdf) for more details.