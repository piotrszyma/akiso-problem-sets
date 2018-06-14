ARR[0]=1
ARR[1]=2
i=2
j=1

echo 10000000 | awk '{
    ex = index("KMG", substr($1, length($1)))
    val = substr($1, 0, length($1) - 1)

    prod = val * 10^(ex * 3)

    sum += prod
}
END {print sum}'
