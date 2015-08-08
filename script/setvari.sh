#!/bin/sh

# Find internal eMMC device, fallback to mmcblk0 (Like CSSU rcS-late)
   mmclist="mmcblk0 mmcblk1"
    for emmc in $mmclist
     do
      if [ ! -e /dev/$emmc ];
       then
     continue;
      fi
      device=`stat /dev/$emmc | sed -n 's/.*Device type: //p'`
      major=`echo $device | sed 's/,.*//'`
      minor=`echo $device | sed 's/.*,//'`
      major=$((0x$major))
      minor=$((0x$minor))
       if grep -q internal /sys/dev/block/$major:$minor/device/../slot_name; 
        then
     break
       fi
    done
   #Use above to set uSD location
     if [ $emmc == "mmcblk0" ];
      then 
     usd=mmcblk1
      else
     usd=mmcblk0
     fi
   #Swap UDV's                           
   swap0=$(blkid -c /dev/null -o device -t TYPE=swap | tr ' ' '\n' | grep $emmc)
   swap1=$(blkid -c /dev/null -o device -t TYPE=swap | tr ' ' '\n' | grep $usd | sed -n 1p)
   swap2=$(blkid -c /dev/null -o device -t TYPE=swap | tr ' ' '\n' | grep $usd | sed -n 2p)

#### Common UDV's ####

## Nice
nice="/bin/busybox nice -19"

## Swap Command
xswapon="/bin/busybox swapon"
xswapoff="/bin/busybox swapoff"

#Temporary file locations
tempfile=/tmp/flopswaplast
inttempfile=/tmp/flopswapint
swaptempfile=/tmp/flopswapref
latesttempfile=/tmp/flopswapcur
locatetempfile=/tmp/flopswaploc

# Setup notification
r=run-standalone.sh
d=dbus-send
t=--type=method_call
o=org
f=freedesktop
n=Notifications

# Setup notification banner
banner(){
$r $d $t \
--dest=$o.$f.$n /$o/$f/$n $o.$f.$n.SystemNoteInfoprint string:"$1"
}

# Setup notification dialog
dialog(){
$r $d $t \
--dest=$o.$f.$n /$o/$f/$n $o.$f.$n.SystemNoteDialog string:"$1" uint32: string:
}

#Text Strings
sucess="Swap Refresh Successful"
safe="Safe to Remove Back Cover"
failed="Operation Failed"
bcwarn="DO NOT REMOVE BACK COVER - Please check swaps"
na="Swap Used is not available on device swap"
calc="One Swap space active ... calculating ..."
wait="Checking... Please Wait"
strip="Please check only one swap is active!"
ncard="No other swaps available"
prog="Moving Swap... This can take some time"

# Get and Print Current Swap
swap=$(awk '/dev/ {print $1}' /proc/swaps)

#Get threshold from QSettings (psb)
psb=$(grep 'PercentForDialog*' /home/user/.config/flopswap/flopswap.conf | cut -d "=" -f 2)

