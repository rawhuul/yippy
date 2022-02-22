echo "*************Bitwise Operations TEST****************"

for i in {1..50};
do

var1=$(($RANDOM % 2))
var2=$(($RANDOM % 2))

echo "Bitwise OR Test"
echo "$var1 | $var2"
yippy_test=$(${YIPPY_EXE} -e "| $var1 $var2")
py_test=$(python3 -c "print($var1 | $var2)")

if [[ $yippy_test -eq $py_test ]]; then
	echo -e "Test:OK\n"
	count_ok=$((count_ok + 1))
else
	echo -e "Test:Fail\n"
	count_fail=$((count_fail + 1))
fi

echo "Bitwise AND Test"
echo "$var1 & $var2"
yippy_test=$(${YIPPY_EXE} -e "& $var1 $var2")
py_test=$(python3 -c "print($var1 & $var2)")

if [[ $yippy_test -eq $py_test ]]; then
	echo -e "Test:OK\n"
	count_ok=$((count_ok + 1))
else
	echo -e "Test:Fail\n"
	count_fail=$((count_fail + 1))
fi

var1=$(($RANDOM % 20))
var2=$(($RANDOM % 20))

echo "Bitwise << Test"
echo "$var1 << $var2"
yippy_test=$(${YIPPY_EXE} -e "<< $var1 $var2")
py_test=$(python3 -c "print($var1 << $var2)")

if [[ $yippy_test -eq $py_test ]]; then
	echo -e "Test:OK\n"
	count_ok=$((count_ok + 1))
else
	echo -e "Test:Fail\n"
	count_fail=$((count_fail + 1))
fi

echo "Bitwise >> Test"
echo "$var1 >> $var2"
yippy_test=$(${YIPPY_EXE} -e ">> $var1 $var2")
py_test=$(python3 -c "print($var1 >> $var2)")

if [[ $yippy_test -eq $py_test ]]; then
	echo -e "Test:OK\n"
	count_ok=$((count_ok + 1))
else
	echo -e "Test:Fail\n"
	count_fail=$((count_fail + 1))
fi

echo "Bitwise XOR Test"
echo "$var1 ^ $var2"
yippy_test=$(${YIPPY_EXE} -e "^ $var1 $var2")
py_test=$(python3 -c "print($var1 ^ $var2)")

if [[ $yippy_test -eq $py_test ]]; then
	echo -e "Test:OK\n"
	count_ok=$((count_ok + 1))
else
	echo -e "Test:Fail\n"
	count_fail=$((count_fail + 1))
fi

done
