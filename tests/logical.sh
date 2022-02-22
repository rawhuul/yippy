echo "*************Logical Operations TEST****************"

for i in {1..50};
do

var1=$(($RANDOM % 2))
var2=$(($RANDOM % 2))

echo "Logical OR Test"
echo "$var1 || $var2"
yippy_test=$(${YIPPY_EXE} -e "|| $var1 $var2")
py_test=$(python3 -c "print($var1 or $var2)")
if_ok "${yippy_test}" "${py_test}"

echo "Logical AND Test"
echo "$var1 && $var2"
yippy_test=$(${YIPPY_EXE} -e "&& $var1 $var2")
py_test=$(python3 -c "print($var1 and $var2)")
if_ok "${yippy_test}" "${py_test}"

done
