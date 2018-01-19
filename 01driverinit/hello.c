
#include <linux/module.h>	// for init_module()

static int __init my_init( void );
static void __exit my_exit( void );


module_init( my_init );
module_exit( my_exit );

MODULE_LICENSE("GPL");

static char *myname="user";

module_param(myname, charp, 0000);
MODULE_PARM_DESC(myname, "Craig Yang");

static int __init my_init( void )
{

    printk( "<1>\nInstalling  module, hello world, %s\n",myname );

    return	0;
}


static void __exit my_exit( void )
{
    printk( "<1>Removing  module, bye\n");
}
