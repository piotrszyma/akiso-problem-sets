clear
rm -rf webtrace
mkdir webtrace
cd webtrace
lynx -dump $1 > page.html
git init > /dev/null
git add .
git commit -m "1" > /dev/null
i=2
while true;
do
  sleep $2

  lynx -dump $1 > page.html
  clear
  git diff
  git commit -a -m "$i" > /dev/null
  i=$((i+1))
done;
