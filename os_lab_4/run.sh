echo -e "Запуск main_1 с одной реализацией функции\n"

gcc -c main_1.c first_realization.c

gcc main_1.o first_realization.o -o res1

./res1

echo -e "\n------------------------------------------------\n"
echo -e "Запуск main_2\n"

gcc -o libFirst.so -shared -fPIC first_realization.c
gcc -o libSecond.so -shared -fPIC second_realization.c
gcc main_2.c -L. -lFirst -lSecond -o res2
export LD_LIBRARY_PATH=.

./res2