#!/bin/bash

#DO NOT REMOVE THE FOLLOWING TWO LINES
git add $0 >> .local.git.out
git commit -a -m "Lab 2 commit" >> .local.git.out
git push >> .local.git.out || echo


#Your code here

#Get the file from the user
FILE=$1
PW="`egrep ".*" $FILE`"
TOTAL=0

#get size of password
#if size is < 6 or > 32 print error
if [ ${#PW} -lt 6 ] || [ ${#PW} -gt 32 ] ; then
  echo "Error: Password length invalid."
  exit
fi

#increase total points by 1 for each character
let TOTAL=$TOTAL+${#PW}
echo "Length total: $TOTAL"

#increase total by 5 for special character
if egrep -q '[#$\+%@]' $FILE ; then
  let TOTAL=$TOTAL+5
  echo "Special Char total: $TOTAL"
fi

#increase total by 5 for number
if egrep -q '[0-9]' $FILE ; then
  let TOTAL=$TOTAL+5
  echo "Number total: $TOTAL"
fi

#increase total by 5 for letter
if egrep -q '[A-Za-z]' $FILE ; then
  let TOTAL=$TOTAL+5
  echo "Letter total: $TOTAL"
fi

#decrease total by 10 for repeated character
if egrep -q '([a-zA-Z0-9])\1+' $FILE ; then
  let TOTAL=$TOTAL-10
  echo "Repeated total: $TOTAL"
fi

#decrease total by 3 for 3 or more consecutive lowercase characters
if egrep -q '[a-z][a-z]+[a-z]' $FILE ; then
  let TOTAL=$TOTAL-3
  echo "Consecutive lower total: $TOTAL"
fi

#decrease total by 3 for 3 or more consecutive uppercase characters
if egrep -q '[A-Z][A-Z]+[A-Z]' $FILE ; then
  let TOTAL=$TOTAL-3
  echo "Consecutive upper total: $TOTAL"
fi

#decrease total by 3 for 3 or more consecutive numbers
if egrep -q '[0-9][0-9]+[0-9]' $FILE ; then
  let TOTAL=$TOTAL-3
  echo "Consecutive num total: $TOTAL"
fi

#print the total score
echo "Password Score: $TOTAL"
