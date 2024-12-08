#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/printk.h>
#include <linux/proc_fs.h>
#include <asm/current.h>

#define procfs_name "Mythread_info"
#define BUFSIZE  1024
char buf[BUFSIZE]; //kernel buffer

static ssize_t Mywrite(struct file *fileptr, const char __user *ubuf, size_t buffer_len, loff_t *offset){
    /*Your code here*/
    if (buffer_len > BUFSIZE - 1 || *offset > 0)
        return -EINVAL;
    
    // Copy data from user space to kernel buffer
    if (copy_from_user(buf, ubuf, buffer_len))
        return -EFAULT;
    
    // Ensure null termination
    buf[buffer_len] = '\0';
    
    // Update offset
    *offset += buffer_len;
    
    return buffer_len;
    /****************/
}


static ssize_t Myread(struct file *fileptr, char __user *ubuf, size_t buffer_len, loff_t *offset){
    /*Your code here*/
    int len;
    char output[BUFSIZE];
    
    // Return 0 if offset is not 0 (indicates end of file)
    if (*offset > 0)
        return 0;
    
    // Format the output string with process info
    len = snprintf(output, BUFSIZE, "%s\nPID: %d, TID: %d, time: %lld\n", 
                  buf, 
                  current->tgid,    // Process ID
                  current->pid,     // Thread ID
                  current->utime / 100 / 1000);  // Time in milliseconds
    
    // Don't overflow user's buffer
    if (len > buffer_len)
        len = buffer_len;
    
    // Copy data to user space
    if (copy_to_user(ubuf, output, len))
        return -EFAULT;
    
    // Update offset
    *offset += len;
    
    return len;
    /****************/
}

static struct proc_ops Myops = {
    .proc_read = Myread,
    .proc_write = Mywrite,
};

static int My_Kernel_Init(void){
    proc_create(procfs_name, 0644, NULL, &Myops);   
    pr_info("My kernel says Hi");
    return 0;
}

static void My_Kernel_Exit(void){
    pr_info("My kernel says GOODBYE");
}

module_init(My_Kernel_Init);
module_exit(My_Kernel_Exit);

MODULE_LICENSE("GPL");