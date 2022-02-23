echo "*************Arithmetic Operations TEST****************"

for i in {1..50}; do

	var1=$RANDOM
	var2=$RANDOM

	echo "Addition Test"
	echo "Adding $var1 and $var2"
	yippy_test=$(${YIPPY_EXE} -e "+ $var1 $var2")
	bash_test=$(expr ""$var1 "+" $var2"")
	if_ok "${yippy_test}" "${bash_test}"

	echo "Subtraction Test"
	echo "Subtracting $var1 and $var2"
	yippy_test=$(${YIPPY_EXE} -e "- $var1 $var2")
	bash_test=$(expr ""$var1 "-" $var2"")
	if_ok "${yippy_test}" "${bash_test}"

	echo "Multiplication Test"
	echo "Multiplying $var1 and $var2"
	yippy_test=$(${YIPPY_EXE} -e "* $var1 $var2")
	bash_test=$(expr ""$var1 "*" $var2"")
	if_ok "${yippy_test}" "${bash_test}"

	echo "Divide Test"
	echo "Dividing $var1 and $var2"
	yippy_test=$(${YIPPY_EXE} -e "/ $var1 $var2")
	bash_test=$(python3 -c "print(\"{:.6f}\".format($var1/$var2))")
	result=$(python3 -c "print(1) if $bash_test==$yippy_test else print(0)")
	if [[ $result -eq 1 ]]; then
		echo -e "Test:OK\n"
	else
		exit 1
	fi

	echo "Modulus Test"
	echo "Mod $var1 and $var2"
	yippy_test=$(${YIPPY_EXE} -e "% $var1 $var2")
	bash_test=$(expr ""$var1 "%" $var2"")
	if_ok "${yippy_test}" "${bash_test}"

done
