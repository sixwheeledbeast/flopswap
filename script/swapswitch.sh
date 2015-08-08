#!/bin/sh

# Load source
source /opt/flopswap/script/setvari.sh

# Get and Banner Current Swap
banner "$swap"
sleep 5

#Logic
if [ "$swap" = "$swap1" ] ;
then
   touch "$swaptempfile"
   $nice $xswapon $swap2
      banner "$prog"
   $nice $xswapoff $swap1
  sleep 3
    if [ `awk '/dev/ {print $1}' /proc/swaps` = "$swap2" ] ;
     then
      banner "$sucess"
      rm "$inttempfile"
      rm "$swaptempfile"
  sleep 5
    banner "$swap2"
    fi
elif [ `awk '/dev/ {print $1}' /proc/swaps` = "$swap2" ] ;
then
   touch "$swaptempfile"
   $nice $xswapon $swap1
       banner "$prog"
   $nice $xswapoff $swap2
  sleep 3
    if [ `awk '/dev/ {print $1}' /proc/swaps` = "$swap1" ] ;
     then
      banner "$sucess"
      rm "$inttempfile"
      rm "$swaptempfile"
  sleep 5
   banner "$swap1" 
    fi
elif [ "$swap" = "$swap0" ] ;
then
 if [ -e "$swap1" ] ;
  then
   touch "$swaptempfile"
   $nice $xswapon $swap1
       banner "$prog"
   $nice $xswapoff $swap0
  sleep 3
  else 
  banner "$ncard" && exit 1
  fi
    if [ `awk '/dev/ {print $1}' /proc/swaps` = "$swap1" ] ;
     then
      banner "$sucess"
      rm "$inttempfile"
      rm "$swaptempfile"
  sleep 5
 banner "$swap1"
    fi
else
   dialog "$failed" && exit 1
fi

### Start calculation save to /tmp
sleep 2
#Get current swap from /proc/swaps
disk=$(awk '/dev/ {print $1}' /proc/swaps | cut -d "/" -f 3)
#Get blocks used since boot from /proc/diskstats
ublocks=$(awk '{if ($3=="'"$disk"'") print $10}' /proc/diskstats)
echo $ublocks ublocks
### int32 workaround ###
## Should measure swap >2TB ##
umbytes=$(($ublocks/2048))
########################
echo $umbytes umbytes
echo "$umbytes" > /tmp/flopswaplast 
sleep 5 && exit 1
