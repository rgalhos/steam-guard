default: build

build:
	g++ -Wall src/main.cpp -o steam-guard -lcurl -lssl -lcrypto
