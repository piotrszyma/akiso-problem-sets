#cat /proc/net/dev
# awk '$1 == "enp0s3" {print $10}'
# cat /proc/net/dev

function convert() {

  number=$1
  if ((number < 1000))
  then
   number_converted="$number B/s"
 elif ((number < 1000000))
  then
     number_converted="$((number /1000)) KB/s"
  else
     number_converted="$((number/1000000)) MB/s"
  fi
  echo $number_converted
}

i=1

OLD_DOWN=$(cat /proc/net/dev | awk '$1 == "enp0s3:" {print $2}')
OLD_UP=$(cat /proc/net/dev | awk '$1 == "enp0s3:" {print $10}')

while true; do
  sleep 1
  NEW_DOWN=$(cat /proc/net/dev | awk '$1 == "enp0s3:" {print $2}')
  NEW_UP=$(cat /proc/net/dev | awk '$1 == "enp0s3:" {print $10}')
  SPEED_DOWN=$((NEW_DOWN-OLD_DOWN))
  SPEED_UP=$((NEW_UP-OLD_UP))
  OLD_DOWN=$NEW_DOWN
  OLD_UP=$NEW_UP

  SUM_DOWN=$((SUM_DOWN + SPEED_DOWN))
  AVER_DOWN=$((SUM_DOWN / i))

  SUM_UP=$((SUM_UP + SPEED_UP))
  AVER_UP=$((SUM_UP / i))

  BATTERY_POWER=$(cat /sys/class/power_supply/BAT0/uevent | grep POWER_SUPPLY_CAPACITY=)
  BATTERY_POWER=${BATTERY_POWER#*=}
  PC_TIME=$(cat /proc/uptime | awk '{print $1}' | cut -f1 -d".")



  echo '*****************************'
  echo 'Internet speed status:'
  echo 'Actual download: '
  convert $SPEED_DOWN
  echo 'Average download: '
  convert $AVER_DOWN
  echo 'Actual upload: '
  convert $SPEED_UP
  echo 'Average upload: '
  convert $AVER_UP
  echo 'Actual battery level: '$BATTERY_POWER '%'
  echo 'PC running time:'
  echo '- in seconds: ' $((PC_TIME))
  echo '- in minutes: ' $((PC_TIME/60))
  echo '- in hours: ' $((PC_TIME/(60*60)))
  echo '- in days: ' $((PC_TIME/(3600*24)))
  echo '*****************************' $'\n\n'

  i=$((i+1))

done

function convert() {
  number = $1
  f ((number < 1000))
  then
   number_converted="$number B/s"
 elif ((number < 1000000))
  then
     number_converted="$((number /1000)) KB/s"
  else
     number_converted="$((number/1000000)) MB/s"
  fi
  return number_converted
}
