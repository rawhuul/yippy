echo "*************Arithmetic Operations TEST****************"

var1=$RANDOM
var2=$RANDOM

echo "Addition Test"
echo "Adding $var1 and $var2"
yippy_test=$(${YIPPY_EXE} -e "+ $var1 $var2")
bash_test=$(expr ""$var1 "+" $var2"")

if [[ $yippy_test -eq $bash_test ]]; then
	echo -e "Test:OK\n"
	count_ok=$((count_ok + 1))
else
	echo -e "Test:Fail\n"
	count_fail=$((count_fail + 1))
fi

echo "Subtraction Test"
echo "Subtracting $var1 and $var2"
yippy_test=$(${YIPPY_EXE} -e "- $var1 $var2")
bash_test=$(expr ""$var1 "-" $var2"")

if [[ $yippy_test -eq $bash_test ]]; then
	echo -e "Test:OK\n"
	count_ok=$((count_ok + 1))
else
	echo -e "Test:Fail\n"
	count_fail=$((count_fail + 1))
fi

echo "Multiplication Test"
echo "Multiplying $var1 and $var2"
yippy_test=$(${YIPPY_EXE} -e "* $var1 $var2")
bash_test=$(expr ""$var1 "*" $var2"")

if [[ $yippy_test -eq $bash_test ]]; then
	echo -e "Test:OK\n"
	count_ok=$((count_ok + 1))
else
	echo -e "Test:Fail\n"
	count_fail=$((count_fail + 1))
fi

echo "Divide Test"
echo "Dividing $var1 and $var2"
yippy_test=$(${YIPPY_EXE} -e "/ $var1 $var2")
bash_test=$(python3 -c "print(\"{:.6f}\".format($var1/$var2))")
result=$(python3 -c "print(1) if $bash_test==$yippy_test else print(0)")

if [[ $result -ne 0 ]]; then
	echo -e "Test:OK\n"
	count_ok=$((count_ok + 1))
else
	echo -e "Test:Fail\n"
	count_fail=$((count_fail + 1))
fi

echo "Modulus Test"
echo "Mod $var1 and $var2"
yippy_test=$(${YIPPY_EXE} -e "% $var1 $var2")
bash_test=$(expr ""$var1 "%" $var2"")

if [[ $yippy_test -eq $bash_test ]]; then
	echo -e "Test:OK\n"
	count_ok=$((count_ok + 1))
else
	echo -e "Test:Fail\n"
	count_fail=$((count_fail + 1))
fi
