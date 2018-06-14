# find $1 -type f -exec ls -ltu {} \; | sort -k 5 -rn | sed 's/^[^/]*//' |
# while read i; do md5sum "${i:1}"; done |
# uniq --all-repeated=prepend --check-chars 16 | awk '{print $2}' |
# while read j;
# do
#     if [[ ${j} != '' ]]
#       then du -h "${j}";
#     else
#           echo;
#     fi;
# done

# echo ''

find $1 -type f -exec ls -ltu {} \; | sort -k 5 -rn | awk '{$1=$2=$3=$4=$5=$6=$7=""; print $0}' |
while read i; do md5sum "${i}"; done | uniq --all-repeated=prepend --check-chars 16 |
awk '{$1 = ""; print $0'} |
while read j;
do
    if [[ ${j} != '' ]]
      then du -h "${j}";
    else
          echo;
    fi;
done
