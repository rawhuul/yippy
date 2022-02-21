echo "*************Arithmetic Operations TEST****************"

var1=1
var2=2

echo "Addition Test"
printf "Adding $var1 and $var2\n\n"
yippy_test=$(${YIPPY_EXE} -e "+ $var1 $var2")
bash_test=$(expr ""$var1 "+" $var2"")

if [[ $yippy_test -eq $bash_test ]]; then
	echo "Test:OK"
	count_ok=$((count_ok + 1))
else
	echo "Test:Fail"
	count_fail=$((count_fail + 1))
fi

printf "\n"

echo "Subtraction Test"
printf "Subtracting $var1 and $var2\n\n"
yippy_test=$(${YIPPY_EXE} -e "- $var1 $var2")
bash_test=$(expr ""$var1 "-" $var2"")

if [[ $yippy_test -eq $bash_test ]]; then
	echo "Test:OK"
	count_ok=$((count_ok + 1))
else
	echo "Test:Fail"
	count_fail=$((count_fail + 1))
fi

printf "\n"

echo "Multiplication Test"
printf "Multiplying $var1 and $var2\n\n"
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
printf "Dividing $var1 and $var2\n\n"
yippy_test=$(${YIPPY_EXE} -e "/ $var2 $var1")
bash_test=$(expr ""$var2 "/" $var1"")

if [[ $yippy_test -eq $bash_test ]]; then
	echo "Test:OK"
	count_ok=$((count_ok + 1))
else
	echo "Test:Fail"
	count_fail=$((count_fail + 1))
fi

printf "\n"

echo "Modulus Test"
printf "Mod $var1 and $var2\n\n"
yippy_test=$(${YIPPY_EXE} -e "% $var1 $var2")
bash_test=$(expr ""$var1 "%" $var2"")

if [[ $yippy_test -eq $bash_test ]]; then
	echo "Test:OK"
	count_ok=$((count_ok + 1))
else
	echo "Test:Fail"
	count_fail=$((count_fail + 1))
fi

printf "\n"
