#!/bin/bash

cd src

# Создание файла сборки
mkdir -p build
cd build
cmake ..

# Сборка проекта
make

IFS=$'\n'

for (( i=1; i <= 3; i++ ))
do
echo "test № $i"
echo "$IFS"
file="test$i.txt"
./main "$file"
echo "-------------------------------"
done

# Очистка
make clean
cd ..