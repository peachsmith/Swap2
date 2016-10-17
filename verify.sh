#!/usr/bin/env bash

# get the expected results
cor1=$(<./tests/correct1.txt)
cor2=$(<./tests/correct2.txt)
cor3=$(<./tests/correct3.txt)
cor4=$(<./tests/correct4.txt)
cor5=$(<./tests/correct5.txt)
cor6=$(<./tests/correct6.txt)
cor7=$(<./tests/correct7.txt)
cor8=$(<./tests/correct8.txt)

# get the actual results
res1=$(<./tests/result1.txt)
res2=$(<./tests/result2.txt)
res3=$(<./tests/result3.txt)
res4=$(<./tests/result4.txt)
res5=$(<./tests/result5.txt)
res6=$(<./tests/result6.txt)
res7=$(<./tests/result7.txt)
res8=$(<./tests/result8.txt)

# the total number of test cases
cases=8

# the number of test cases that passed
passed=0

# compare expected results with actual results
if [ "$res1" == "$cor1" ]; then
	echo Test 1: pass
	let "passed++"
else
	echo Test 1: fail
fi

if [ "$res2" == "$cor2" ]; then
	echo Test 2: pass
	let "passed++"
else
	echo Test 2: fail
fi

if [ "$res3" == "$cor3" ]; then
	echo Test 3: pass
	let "passed++"
else
	echo Test 3: fail
fi

if [ "$res4" == "$cor4" ]; then
	echo Test 4: pass
	let "passed++"
else
	echo Test 4: fail
fi

if [ "$res5" == "$cor5" ]; then
	echo Test 5: pass
	let "passed++"
else
	echo Test 5: fail
fi

if [ "$res6" == "$cor6" ]; then
	echo Test 6: pass
	let "passed++"
else
	echo Test 6: fail
fi

if [ "$res7" == "$cor7" ]; then
	echo Test 7: pass
	let "passed++"
else
	echo Test 7: fail
fi

if [ "$res8" == "$cor8" ]; then
	echo Test 8: pass
	let "passed++"
else
	echo Test 8: fail
fi

echo ============================================
echo $passed out of $cases test cases succeeded
echo ============================================
