MyTiny6410LinuxDeviceDriverLab based on linux 2.6.38

Lab1: two fundmental structure in linux kernel, Linklist and Timer

Lab2: a simple chatacter device dirver memdev

Lab3: a complex character device driver with semaphore 1.implement a device driver supports complex(2) driver

Lab4: ioctl

    1.Implement 3 different ioctls command

    2.Understand the workflow of ioctl
  
      2-1.validate the command: direction check and function number check
    
      2-2.validate the user space memory access right
    
      3-1.command excuted by switch

    3.What's the ioctl command format
  
      direction size magic function
    
          1     24    8      8
         
Lab5:Waiting queue and Blocking I/O
