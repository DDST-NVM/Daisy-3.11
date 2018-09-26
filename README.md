# Daisy-3.11

# Daisy: How to Use

- Make sure that you have an Linux System Running with at least 6GB memory
- Take steps as follows:

> git clone git@github.com:DDST-NVM/Daisy-3.11.git

> cd Daisy/linux-3.11.0/

> sudo make mrproper

> sudo make localmodconfig (and then press enter for all options)

> sudo make -jN(N is the number of your cores)

> sudo make modules_install

> sudo make install

> sudo reboot

- During your reboot phase, please press `F12` or `SHIFT` to choose the linux kernel version. You should choose linux-3.12.49 for running Daisy System.
- If there is problem with the window showing during the boot phase, please reboot and edit the grub interface.
- If everything goes smoonthly, just return to the Daisy directory.
- Take steps as follows: 

> cd pcmapi

> chmod u=rwx create_so.sh

> ./create_so.sh 

> make

> ./ptest

- You can debug our user-mode codes with gdb.

