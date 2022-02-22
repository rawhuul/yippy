echo "*************Bitwise Operations TEST****************"

for i in {1..50};
do

var1=$(($RANDOM % 2))
var2=$(($RANDOM % 2))

echo "Bitwise OR Test"
echo "$var1 | $var2"
yippy_test=$(${YIPPY_EXE} -e "| $var1 $var2")
py_test=$(python3 -c "print($var1 | $var2)")
if_ok "${yippy_test}" "${py_test}"

echo "Bitwise AND Test"
echo "$var1 & $var2"
yippy_test=$(${YIPPY_EXE} -e "& $var1 $var2")
py_test=$(python3 -c "print($var1 & $var2)")
if_ok "${yippy_test}" "${py_test}"

var1=$(($RANDOM % 20))
var2=$(($RANDOM % 20))

echo "Bitwise << Test"
echo "$var1 << $var2"
yippy_test=$(${YIPPY_EXE} -e "<< $var1 $var2")
py_test=$(python3 -c "print($var1 << $var2)")
if_ok "${yippy_test}" "${py_test}"

echo "Bitwise >> Test"
echo "$var1 >> $var2"
yippy_test=$(${YIPPY_EXE} -e ">> $var1 $var2")
py_test=$(python3 -c "print($var1 >> $var2)")
if_ok "${yippy_test}" "${py_test}"

echo "Bitwise XOR Test"
echo "$var1 ^ $var2"
yippy_test=$(${YIPPY_EXE} -e "^ $var1 $var2")
py_test=$(python3 -c "print($var1 ^ $var2)")
if_ok "${yippy_test}" "${py_test}"

done
