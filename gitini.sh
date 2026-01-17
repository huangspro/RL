#!/bin/bash

echo "input the remote repository name"
read repo
echo "Please give a local name the repository"
read reponame

git init && git remote add $reponame https://github.com/huangspro/$repo && git add . && git commit -m "1" && git push -u $reponame master
