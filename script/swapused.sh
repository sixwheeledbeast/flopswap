#!/bin/sh

source /opt/flopswap/script/setvari.sh

## Check source file echos.
echo $swap
#Print to file
echo $swap > "$locatetempfile"
banner "$swap"
 if [ "$swap" = "$swap1" ] ;
  then
    if [ -e $inttempfile ];
     then
      rm "$inttempfile"
    fi
 elif [ "$swap" = "$swap2" ] ;
   then
    if [ -e $inttempfile ];
     then
      rm "$inttempfile"
    fi
 elif [ "$swap" = "$swap0" ] ;
  then
   touch "$inttempfile"
 else
  echo "You shouldn't have got here"
   exit 1
 fi
###########
sleep 3
###########
#Get current swap from /proc/swaps
diskcheck=/dev/$(awk '/dev/ {print $1}' /proc/swaps | cut -d "/" -f 3)
echo $diskcheck
if [ $diskcheck == $swap0 ] ;
 then
  banner "$na"
exit 1
 else
  banner "$wait"
 sleep 2
fi

if [ $diskcheck == $swap1 ] || [ $diskcheck == $swap2 ] ;
 then
  banner "$calc"
 sleep 2
 else
  banner "$failed"
  sleep 5
  banner "$strip"
exit 1
fi
###########

#Check for existance of last megabytes file
#If not there touch and zero
if [ -e $tempfile ];
 then
        touch "$tempfile"
 else
  touch "$tempfile"
 echo "0" > "$tempfile"
fi

#Get current swap from /proc/swaps
disk=$(awk '/dev/ {print $1}' /proc/swaps | cut -d "/" -f 3)
#Get blocks used since boot from /proc/diskstats
ublocks=$(awk '{if ($3=="'"$disk"'") print $10}' /proc/diskstats)
echo $ublocks ublocks
### int32 workaround ###
## Should measure swap >2TB ##
umbytes=$(($ublocks/2048))
########################
#Get Swap size from /proc/swaps
swapsize=$(awk '{if ($1=="'"/dev/$disk"'") print $3}' /proc/swaps)
#Divide by 1024 to get swap size in megabytes
swapsizembytes=$(($swapsize/1024))
echo $swapsizembytes swapsizembytes
#Get last megabytes value from /tmp folder
lasttotalsmb=$(awk '{print $1}' /tmp/flopswaplast)
echo $lasttotalsmb lasttotalsmb
#Banner if int32 overrun
 if [ $lasttotalsmb -lt 0 ]; 
  then
 banner "$failed"
  sleep 1
dialog "

Busybox can no longer calculate your total swap.
Please reboot to restart calculation or use cron/alarmed to trigger FlopSwap.
See the Wiki page found in the Help section for more details

"
exit 0
  else
#Subtract last from current value to get latest used value
getcurrent=$(($umbytes-$lasttotalsmb))
echo $getcurrent getcurrent
timeshundred=$(($getcurrent*100))
echo $timeshundred timeshundred
#Get divide to get percent
getpercent=$(($timeshundred/$swapsizembytes))
echo $getpercent % Fresh Swap Written
#Print banner
banner "$getpercent % Fresh Swap Written
$getcurrent Mb of $swapsizembytes Mb"
#Print to file
echo "$getpercent % ($getcurrent Mb of $swapsizembytes Mb)" > "$latesttempfile"
#If 94% show warning dialog
if [ $getpercent -ge $psb ]; then
dialog "

***Fresh Swap Recommended***
Used \"$getcurrent Mb ($getpercent) %\" of \"$swapsizembytes Mb\"
On $(hostname) at $(date)

"
 fi
fi
exit 0
