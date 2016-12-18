printf '%12s %17s\n' UID NAME;
for files in /proc/*; do
    if [ -e $files/status ];
    then
      NAME=$(cat $files/status | awk 'FNR == '1' {print $2}')
      printf '%12s %17s\n' ${files##/proc/} $NAME;
    fi
done
