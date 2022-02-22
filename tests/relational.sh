echo "*************Realtional Operations TEST****************"

for i in {1..50};
do

var1=$RANDOM
var2=$RANDOM

echo "Relational < Test"
echo "$var1 < $var2"
yippy_test=$(${YIPPY_EXE} -e "< $var1 $var2")
py_test=$(python3 -c "print(int($var1 < $var2))")
if_ok "${yippy_test}" "${py_test}"

echo "Relational <= Test"
echo "$var1 <= $var2"
yippy_test=$(${YIPPY_EXE} -e "<= $var1 $var2")
py_test=$(python3 -c "print(int($var1 <= $var2))")
if_ok "${yippy_test}" "${py_test}"

echo "Relational > Test"
echo "$var1 > $var2"
yippy_test=$(${YIPPY_EXE} -e "> $var1 $var2")
py_test=$(python3 -c "print(int($var1 > $var2))")
if_ok "${yippy_test}" "${py_test}"

echo "Relational >= Test"
echo "$var1 >= $var2"
yippy_test=$(${YIPPY_EXE} -e ">= $var1 $var2")
py_test=$(python3 -c "print(int($var1 >= $var2))")
if_ok "${yippy_test}" "${py_test}"

echo "Relational == Test"
echo "$var1 == $var2"
yippy_test=$(${YIPPY_EXE} -e "== $var1 $var2")
py_test=$(python3 -c "print(int($var1 == $var2))")
if_ok "${yippy_test}" "${py_test}"

echo "Relational != Test"
echo "$var1 != $var2"
yippy_test=$(${YIPPY_EXE} -e "!= $var1 $var2")
py_test=$(python3 -c "print(int($var1 != $var2))")
if_ok "${yippy_test}" "${py_test}"

done
