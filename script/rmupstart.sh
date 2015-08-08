#!/bin/sh

# Remove Upstart event
rm /etc/event.d/relocateswap

# Set swap_sd as 0 (for latest CSSU upstart)
sed -i -e "s@swap_sd=\"1\"@swap_sd=\"0\"@" /etc/default/mount-opts-overwrite
