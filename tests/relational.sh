echo "*************Realtional Operations TEST****************"

for i in {1..50};
do

var1=$RANDOM
var2=$RANDOM

echo "Relational < Test"
echo "$var1 < $var2"
yippy_test=$(${YIPPY_EXE} -e "< $var1 $var2")
py_test=$(python3 -c "print(int($var1 < $var2))")

if [[ $yippy_test -eq $py_test ]]; then
	echo -e "Test:OK\n"
	count_ok=$((count_ok + 1))
else
	echo -e "Test:Fail\n"
	count_fail=$((count_fail + 1))
fi

echo "Relational <= Test"
echo "$var1 <= $var2"
yippy_test=$(${YIPPY_EXE} -e "<= $var1 $var2")
py_test=$(python3 -c "print(int($var1 <= $var2))")

if [[ $yippy_test -eq $py_test ]]; then
	echo -e "Test:OK\n"
	count_ok=$((count_ok + 1))
else
	echo -e "Test:Fail\n"
	count_fail=$((count_fail + 1))
fi

echo "Relational > Test"
echo "$var1 > $var2"
yippy_test=$(${YIPPY_EXE} -e "> $var1 $var2")
py_test=$(python3 -c "print(int($var1 > $var2))")

if [[ $yippy_test -eq $py_test ]]; then
	echo -e "Test:OK\n"
	count_ok=$((count_ok + 1))
else
	echo -e "Test:Fail\n"
	count_fail=$((count_fail + 1))
fi

echo "Relational >= Test"
echo "$var1 >= $var2"
yippy_test=$(${YIPPY_EXE} -e ">= $var1 $var2")
py_test=$(python3 -c "print(int($var1 >= $var2))")

if [[ $yippy_test -eq $py_test ]]; then
	echo -e "Test:OK\n"
	count_ok=$((count_ok + 1))
else
	echo -e "Test:Fail\n"
	count_fail=$((count_fail + 1))
fi

echo "Relational == Test"
echo "$var1 == $var2"
yippy_test=$(${YIPPY_EXE} -e "== $var1 $var2")
py_test=$(python3 -c "print(int($var1 == $var2))")

if [[ $yippy_test -eq $py_test ]]; then
	echo -e "Test:OK\n"
	count_ok=$((count_ok + 1))
else
	echo -e "Test:Fail\n"
	count_fail=$((count_fail + 1))
fi

echo "Relational != Test"
echo "$var1 != $var2"
yippy_test=$(${YIPPY_EXE} -e "!= $var1 $var2")
py_test=$(python3 -c "print(int($var1 != $var2))")

if [[ $yippy_test -eq $py_test ]]; then
	echo -e "Test:OK\n"
	count_ok=$((count_ok + 1))
else
	echo -e "Test:Fail\n"
	count_fail=$((count_fail + 1))
fi

done
