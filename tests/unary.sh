echo "*************Unary Operations TEST****************"

for i in {1..50};
do

bit=$RANDOM

echo "Unary Plus Test"
echo "+$bit"
yippy_test=$(${YIPPY_EXE} -e "+ $bit")
py_test=$(python3 -c "print(+ $bit)")

if [[ $yippy_test -eq $py_test ]]; then
	echo -e "Test:OK\n"
	count_ok=$((count_ok + 1))
else
	echo -e "Test:Fail\n"
	count_fail=$((count_fail + 1))
fi

echo "Unary Negation Test"
echo "-$bit"
yippy_test=$(${YIPPY_EXE} -e "- $bit")
py_test=$(python3 -c "print(- $bit)")

if [[ $yippy_test -eq $py_test ]]; then
	echo -e "Test:OK\n"
	count_ok=$((count_ok + 1))
else
	echo -e "Test:Fail\n"
	count_fail=$((count_fail + 1))
fi

echo "Unary Negation Test"
echo "!$bit"
yippy_test=$(${YIPPY_EXE} -e "! $bit")
py_test=$(python3 -c "print(int(not $bit))")

if [[ $yippy_test -eq $py_test ]]; then
	echo -e "Test:OK\n"
	count_ok=$((count_ok + 1))
else
	echo -e "Test:Fail\n"
	count_fail=$((count_fail + 1))
fi

echo "Unary One's Complement Test"
echo "~$bit"
yippy_test=$(${YIPPY_EXE} -e "~ $bit")
py_test=$(python3 -c "print(~ $bit)")

if [[ $yippy_test -eq $py_test ]]; then
	echo -e "Test:OK\n"
	count_ok=$((count_ok + 1))
else
	echo -e "Test:Fail\n"
	count_fail=$((count_fail + 1))
fi

done
