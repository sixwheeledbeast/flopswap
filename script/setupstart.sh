#!/bin/sh

# Set Upstart event
   cp /opt/flopswap/script/relocateswap /etc/event.d/relocateswap

# Set swap_sd as 1 (for latest CSSU upstart)
   if grep -q swap_sd\"0\" /etc/default/mount-opts-overwrite; 
    then
       sed -i -e "s@swap_sd=\"0\"@swap_sd=\"1\"@" /etc/default/mount-opts-overwrite    
       echo "Sucessfully replaced swap_sd with 1"
     else
       echo "swap_sd=\"1\"" >> /etc/default/mount-opts-overwrite
       echo "Added swap_sd 1 EOF"
   fi

