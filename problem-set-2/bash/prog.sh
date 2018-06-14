NUMBER_OF_LINES=$(cat /proc/net/dev | awk 'END{print NR}')

i=3

while ((i<=$NUMBER_OF_LINES)); do
  NETWORK_NAME[$i]=$(cat /proc/net/dev | awk 'FNR == '$i' {print $1}')
  OLD_DOWN[$i]=$(cat /proc/net/dev | awk 'FNR == '$i' {print $10}')
  OLD_UP[$i]=$(cat /proc/net/dev | awk 'FNR == '$i' {print $2}')
  i=$((i+1))
done

clear


i=3
j=1
while true; do
    sleep 1
    clear
    echo Networks status:
  while ((i<=$NUMBER_OF_LINES)); do
    NEW_DOWN[$i]=$(cat /proc/net/dev | awk 'FNR == '$i' {print $10}')
    NEW_UP[$i]=$(cat /proc/net/dev | awk 'FNR == '$i' {print $2}')
    SPEED_DOWN[$i]=$((NEW_DOWN[i]-OLD_DOWN[i]))
    SPEED_UP[$i]=$((NEW_UP[i]-OLD_UP[i]))
    OLD_DOWN[$i]=${NEW_DOWN[i]}
    OLD_UP[$i]=${NEW_UP[i]}

    SUM_DOWN[$i]=$((SUM_DOWN[i] + SPEED_DOWN[i]))
    AVER_DOWN[$i]=$((SUM_DOWN[i] / j))
    SUM_UP[$i]=$((SUM_UP[i] + SPEED_UP[i]))
    AVER_UP[$i]=$((SUM_UP[i] / j))

    CONV[0]=${SPEED_UP[i]}
    CONV[1]=${AVER_UP[i]}
    CONV[2]=${SPEED_DOWN[i]}
    CONV[3]=${AVER_DOWN[i]}

    k=0

    while ((k<=3)); do

      # if ((CONV[k] < 1000))
      #   then
      #     echo ${CONV[k]}
      # fi
       if ((CONV[k] < 1000)); then
         CONV[$k]="$((CONV[$k])) B/s"
       elif ((CONV[k] < 1000000)); then
         CONV[$k]="$((CONV[k] / 1000)) kB/s"
       else
         CONV[$k]="$((CONV[k] / 1000000)) MB/s"
       fi
    k=$((k+1))

    done


    echo ${NETWORK_NAME[i]} UP: ${CONV[0]} AV: ${CONV[1]} DN: ${CONV[2]} AV: ${CONV[3]}
    i=$((i+1))
    j=$((j+1))
  done
    BATTERY_POWER=$(cat /sys/class/power_supply/BAT0/uevent | grep POWER_SUPPLY_CAPACITY=)
    BATTERY_POWER=${BATTERY_POWER#*=}
    PC_TIME=$(cat /proc/uptime | awk '{print $1}' | cut -f1 -d".")

    echo 'Actual battery level: '$BATTERY_POWER '%'
    echo 'PC running time:' $((PC_TIME)) 's |' $((PC_TIME/60)) 'm |' $((PC_TIME/3600)) 'hrs |' $((PC_TIME/(3600 * 24))) 'days'

    i=3
done

#OLD_DOWN
#OLD_UP

#NEW_DOWN
#NEW_UP
