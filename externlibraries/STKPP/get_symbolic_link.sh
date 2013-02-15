#!/bin/bash

function make_symbolic_link_for_projects
{
# loop over the files
for file in $1/*
do 
link=./projects/$2/$(basename $1)/$(basename $file)
ln -s ../../../$file $link
done
}

function make_symbolic_link_for_include
{
# loop over header files
for file in $1/*
do 
link=./include/$(basename $file)
ln -s ../$file $link
done
}
#create symbolic link of all the projects
projects=( $(find ../../../stkpp/projects -maxdepth 1 -type d -printf '%P\n') )
for project in ${projects[@]}
do
make_symbolic_link_for_projects ../../../stkpp/projects/$project/include $project
make_symbolic_link_for_projects ../../../stkpp/projects/$project/src $project
#symbolic link for cmakefiles inside project
ln -s ../../../../../stkpp/projects/$project/CMakeLists.txt ./projects/$project/CMakeLists.txt
done

#create symbolic link for headers
make_symbolic_link_for_include ../../../stkpp/include

#create symbolic link for other cmakefiles
#ln -s ../../../../stkpp/projects/CMakeLists.txt ./projects/CMakeLists.txt
#ln -s ../../../stkpp/CMakeLists.txt CMakeLists.txt



#create symbolic link for operators and product inside Arrays project

function make_symbolic_link_for_others_in_arrays
{
# loop over the files
for file in $1/*
do 
link=./projects/Arrays/include/$(basename $1)/$(basename $file)
ln -s ../../../../$file $link
done
}

make_symbolic_link_for_others_in_arrays ../../../stkpp/projects/Arrays/include/operators
make_symbolic_link_for_others_in_arrays ../../../stkpp/projects/Arrays/include/product