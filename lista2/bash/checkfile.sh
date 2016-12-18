# ls -p | grep -v / |
# while read i; do
#   mv $i `echo $i | tr [:upper:] [:lower:]`;
# done

find -type f -exec ls -ltu {} \; | sort -k 5 -rn | awk '{print $8}' | while read i; do md5sum $i; done |uniq -D --check-chars 16
