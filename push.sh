#! /bin/bash
a=$1
if [ "$a" == "" ]; then
echo "Use Default"
a="MuXi"
fi
git add .
git commit -m "$a"
git push