echo "*************Logical Operations TEST****************"

for i in {1..50};
do

var1=$(($RANDOM % 2))
var2=$(($RANDOM % 2))

echo "Logical OR Test"
echo "$var1 || $var2"
yippy_test=$(${YIPPY_EXE} -e "|| $var1 $var2")
py_test=$(python3 -c "print($var1 or $var2)")

if [[ $yippy_test -eq $py_test ]]; then
	echo -e "Test:OK\n"
	count_ok=$((count_ok + 1))
else
	echo -e "Test:Fail\n"
	count_fail=$((count_fail + 1))
fi

echo "Logical AND Test"
echo "$var1 && $var2"
yippy_test=$(${YIPPY_EXE} -e "&& $var1 $var2")
py_test=$(python3 -c "print($var1 and $var2)")

if [[ $yippy_test -eq $py_test ]]; then
	echo -e "Test:OK\n"
	count_ok=$((count_ok + 1))
else
	echo -e "Test:Fail\n"
	count_fail=$((count_fail + 1))
fi

done