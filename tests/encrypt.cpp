#include <iostream>
#include <fstream>
#include <cstddef>
#include <vector>

#define KEY_SIZE 32
#define BLOCK_SIZE 16

namespace std {
typedef uint8_t byte;
}

class Cipher {
private:
	int block_size = 16;
public:
	int EncryptBlock(std::byte* key, std::byte *iv, std::byte *src, std::byte *dst) {
		for (int i = 0; i < block_size; ++i) {
			dst[i] = std::byte(src[i]+2);
		}
		return block_size;
	}
	int DecryptBlock(std::byte* key, std::byte *iv, std::byte *src, std::byte *dst) {
		for (int i = 0; i < block_size; ++i) {
			dst[i] = std::byte(src[i]-2);
		}
		return block_size;
	}
};

int encrypt(const char *fin, const char *fout) {
	Cipher cipher;
	std::ifstream in(fin, std::ifstream::binary);
	//ofstream out(fout, ofstream::binary);
	std::byte key[KEY_SIZE] = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2};
	std::byte iv[BLOCK_SIZE] = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6};

	std::byte block[BLOCK_SIZE], temp[BLOCK_SIZE];
	std::streamsize r;
	while ((r = in.readsome((char*)block, BLOCK_SIZE)) > 0) {
		cipher.EncryptBlock(key, iv, block, temp);
		std::cout.write((char*)temp, r);
	}

	return 0;
}

int decrypt(const char *fin, const char *fout) {
	Cipher cipher;
	std::ifstream in(fin, std::ifstream::binary);
	//ofstream out(fout, ofstream::binary);
	std::byte key[KEY_SIZE] = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2};
	std::byte iv[BLOCK_SIZE] = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6};

	std::byte block[BLOCK_SIZE], temp[BLOCK_SIZE];
	std::streamsize r;
	while ((r = in.readsome((char*)block, BLOCK_SIZE)) > 0) {
		cipher.DecryptBlock(key, iv, block, temp);
		std::cout.write((char*)temp, r);
	}

	return 0;
}

int main(int argc, const char *argv[]) {
	int mode = 0;
	for (int i=1; i < argc; ++i) {
		std::string a(argv[i]);
		if (a == "-d") mode = 1;
		else if (a == "-e") mode = 0;
		else {
			if (mode == 0) encrypt(argv[i], nullptr);
			else decrypt(argv[i], nullptr);
		}
	}
}
