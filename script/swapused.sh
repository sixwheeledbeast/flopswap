#!/bin/sh

helpme()
{
  echo "USAGE: swapused.sh [-hq]"
  echo "This script should be run as root" # FIXME root detection
  echo "  -h      print this text"
  echo "  -q      quiet mode. No popups are displayed and nothing is written to stdout unless fresh swap is needed or in case of errors"
}

quiet()
{
  if ! $QUIET 
  then "$@"
  fi
}

QUIET=false # Quiet mode

# Handle args
if [ $# -gt 1 ]
then 
  helpme
elif [ $# -eq 0 ]
then 
  echo "NOT using quiet mode..."
elif [ "$1" == "-q" ]
then
  QUIET=true
else
  helpme
fi

source /opt/flopswap/script/setvari.sh

## Check source file echos.
quiet echo $swap
#Print to file
echo $swap > "$locatetempfile"
quiet banner "$swap"
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
   echo "ERROR"
   quiet echo "You shouldn't have got here" 
   exit 1
 fi
###########
sleep 3
###########
#Get current swap from /proc/swaps
diskcheck=/dev/$(awk '/dev/ {print $1}' /proc/swaps | cut -d "/" -f 3)
quiet echo $diskcheck
if [ $diskcheck == $swap0 ] ;
 then
  quiet banner "$na"
exit 1
 else
  quiet banner "$wait"
 sleep 2
fi

if [ $diskcheck == $swap1 ] || [ $diskcheck == $swap2 ] ;
 then
  quiet banner "$calc"
 sleep 2
 else
  quiet banner "$failed"
  echo "ERROR"
  sleep 5
  quiet banner "$strip"
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
quiet echo $ublocks ublocks
### int32 workaround ###
## Should measure swap >2TB ##
umbytes=$(($ublocks/2048))
########################
#Get Swap size from /proc/swaps
swapsize=$(awk '{if ($1=="'"/dev/$disk"'") print $3}' /proc/swaps)
#Divide by 1024 to get swap size in megabytes
swapsizembytes=$(($swapsize/1024))
quiet echo $swapsizembytes swapsizembytes
#Get last megabytes value from /tmp folder
lasttotalsmb=$(awk '{print $1}' /tmp/flopswaplast)
quiet echo $lasttotalsmb lasttotalsmb
#Banner if int32 overrun
 if [ $lasttotalsmb -lt 0 ]; 
  then
  echo "ERROR"
  quiet echo "$failed"
  quiet banner "$failed"
  sleep 1
quiet dialog "

Busybox can no longer calculate your total swap.
Please reboot to restart calculation or use cron/alarmed to trigger FlopSwap.
See the Wiki page found in the Help section for more details

"
exit 0
  else
#Subtract last from current value to get latest used value
getcurrent=$(($umbytes-$lasttotalsmb))
quiet echo $getcurrent getcurrent
timeshundred=$(($getcurrent*100))
quiet echo $timeshundred timeshundred
#Get divide to get percent
getpercent=$(($timeshundred/$swapsizembytes))
quiet echo $getpercent % Fresh Swap Written
#Print banner
quiet banner "$getpercent % Fresh Swap Written
$getcurrent Mb of $swapsizembytes Mb"
#Print to file
echo "$getpercent % ($getcurrent Mb of $swapsizembytes Mb)" > "$latesttempfile"
#If 94% show warning dialog
if [ $getpercent -ge $psb ]; then
quiet dialog "

***Fresh Swap Recommended***
Used \"$getcurrent Mb ($getpercent) %\" of \"$swapsizembytes Mb\"
On $(hostname) at $(date)

"
 if $QUIET ; then echo "NEEDFRESH"; fi
 fi
fi
exit 0
