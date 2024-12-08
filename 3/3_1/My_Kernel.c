#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/printk.h>
#include <linux/proc_fs.h>
#include <asm/current.h>

#define procfs_name "Mythread_info"
#define BUFSIZE  1024
char buf[BUFSIZE];

static ssize_t Mywrite(struct file *fileptr, const char __user *ubuf, size_t buffer_len, loff_t *offset){
    /* Do nothing */
	return 0;
}


static ssize_t Myread(struct file *fileptr, char __user *ubuf, size_t buffer_len, loff_t *offset){
    /*Your code here*/
    struct task_struct *task, *thread;
    int len = 0;
    
    // 如果已經讀完 (*offset > 0)，返回0表示EOF
    if (*offset > 0)
        return 0;
    
    // 清空buffer
    memset(buf, 0, BUFSIZE);
    
    // 獲取當前process
    task = current;
    
    // 遍歷所有threads
    for_each_thread(task, thread) {
        if(current->pid == thread->pid)
            continue;
        // 格式化thread資訊並加入buffer
        len += sprintf(buf + len, 
                      "PID: %d, TID: %d, Priority: %d, State: %u\n",
                      current->pid,           // Process ID
                      thread->pid,         // Thread ID
                      thread->prio,        // Priority
                      thread->__state);    // State
    }
    
    // 確保不超過buffer大小
    if (len > buffer_len)
        len = buffer_len;
        
    // 複製資料到user space
    if (copy_to_user(ubuf, buf, len))
        return -EFAULT;
    
    // 更新offset
    *offset += len;
    
    // 返回讀取的bytes數
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