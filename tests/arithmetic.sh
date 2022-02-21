echo "*************Arithmetic Operations TEST****************"

var1=$RANDOM
var2=$RANDOM

echo "Addition Test"
echo -e "Adding $var1 and $var2\n\n"
yippy_test=$(${YIPPY_EXE} -e "+ $var1 $var2")
bash_test=$(expr ""$var1 "+" $var2"")

if [[ $yippy_test -eq $bash_test ]]; then
	echo "Test:OK"
	count_ok=$((count_ok + 1))
else
	echo "Test:Fail"
	count_fail=$((count_fail + 1))
fi

echo -e "\n"

echo "Subtraction Test"
echo -e "Subtracting $var1 and $var2\n\n"
yippy_test=$(${YIPPY_EXE} -e "- $var1 $var2")
bash_test=$(expr ""$var1 "-" $var2"")

if [[ $yippy_test -eq $bash_test ]]; then
	echo "Test:OK"
	count_ok=$((count_ok + 1))
else
	echo "Test:Fail"
	count_fail=$((count_fail + 1))
fi

echo -e "\n"

echo "Multiplication Test"
echo -e "Multiplying $var1 and $var2\n\n"
yippy_test=$(${YIPPY_EXE} -e "* $var1 $var2")
bash_test=$(expr ""$var1 "*" $var2"")

if [[ $yippy_test -eq $bash_test ]]; then
	echo "Test:OK"
	count_ok=$((count_ok + 1))
else
	echo "Test:Fail"
	count_fail=$((count_fail + 1))
fi

echo "Divide Test"
echo -e "Dividing $var1 and $var2\n\n"
yippy_test=$(${YIPPY_EXE} -e "/ $var1 $var2")
bash_test=$(awk -v var1=${var1} -v var2=${var2} 'BEGIN {print "%.6g",( var1 / var2 )}')
result=$(awk -v var1=${bash_test} -v var2=${yippy_test} 'BEGIN {print ( var1 == var2 )}')

if [[ $result -ne 0 ]]; then
	echo "Test:OK"
	count_ok=$((count_ok + 1))
else
	echo "Test:Fail"
	count_fail=$((count_fail + 1))
fi

echo -e "\n"

echo "Modulus Test"
echo -e "Mod $var1 and $var2\n\n"
yippy_test=$(${YIPPY_EXE} -e "% $var1 $var2")
bash_test=$(expr ""$var1 "%" $var2"")

if [[ $yippy_test -eq $bash_test ]]; then
	echo "Test:OK"
	count_ok=$((count_ok + 1))
else
	echo "Test:Fail"
	count_fail=$((count_fail + 1))
fi

echo -e "\n"
