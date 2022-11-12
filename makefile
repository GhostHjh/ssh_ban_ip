ban_ip: main.o ban_ip.o my_ini.o select_ip.o
	g++ main.o ban_ip.o my_ini.o select_ip.o -o ban_ip
	del *.o

main.o:main.cpp include/ban_ip.h
	g++ -c main.cpp -I include

ban_ip.o: src/ban_ip.cpp include/ban_ip.h
	g++ -c src/ban_ip.cpp -I include

my_ini.o: src/my_ini.cpp include/my_ini.h
	g++ -c src/my_ini.cpp -I include

select_ip.o: src/select_ip.cpp include/select_ip.h
	g++ -c src/select_ip.cpp -I include

win_clean:
	del *.o

clean:
	rm -rf main.o ban_ip.o my_ini.o select_ip.o