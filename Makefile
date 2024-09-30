all:app

app: StringList.o StringListTest.o
	g++ StringList.o StringListTest.o -o app

StringList.o: StringList.cpp StringList.h
	g++ -c StringList.cpp StringList.h

StringListTest.o: StringListTest.cpp
	g++ -c StringListTest.cpp

clean:
	rm -f *.o *.exe *.h.gch