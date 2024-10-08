#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <ctime>
#include <cstring>
#include <bits/stdc++.h>
#include <filesystem>
#include <cstdint>
#include <sstream>
#include <iomanip>

#define uchar unsigned char
#define uint unsigned int

#define DBL_INT_ADD(a,b,c) if (a > 0xffffffff - (c)) ++b; a += c;
#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))

#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))

typedef unsigned long long int int64;
using namespace std;
namespace fs = std::filesystem;

/// SHA512 ENCRYPTION
int64 Message[80];
const int64 Constants[80]
    = { 0x428a2f98d728ae22, 0x7137449123ef65cd,
        0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc,
        0x3956c25bf348b538, 0x59f111f1b605d019,
        0x923f82a4af194f9b, 0xab1c5ed5da6d8118,
        0xd807aa98a3030242, 0x12835b0145706fbe,
        0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
        0x72be5d74f27b896f, 0x80deb1fe3b1696b1,
        0x9bdc06a725c71235, 0xc19bf174cf692694,
        0xe49b69c19ef14ad2, 0xefbe4786384f25e3,
        0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
        0x2de92c6f592b0275, 0x4a7484aa6ea6e483,
        0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
        0x983e5152ee66dfab, 0xa831c66d2db43210,
        0xb00327c898fb213f, 0xbf597fc7beef0ee4,
        0xc6e00bf33da88fc2, 0xd5a79147930aa725,
        0x06ca6351e003826f, 0x142929670a0e6e70,
        0x27b70a8546d22ffc, 0x2e1b21385c26c926,
        0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
        0x650a73548baf63de, 0x766a0abb3c77b2a8,
        0x81c2c92e47edaee6, 0x92722c851482353b,
        0xa2bfe8a14cf10364, 0xa81a664bbc423001,
        0xc24b8b70d0f89791, 0xc76c51a30654be30,
        0xd192e819d6ef5218, 0xd69906245565a910,
        0xf40e35855771202a, 0x106aa07032bbd1b8,
        0x19a4c116b8d2d0c8, 0x1e376c085141ab53,
        0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8,
        0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb,
        0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3,
        0x748f82ee5defb2fc, 0x78a5636f43172f60,
        0x84c87814a1f0ab72, 0x8cc702081a6439ec,
        0x90befffa23631e28, 0xa4506cebde82bde9,
        0xbef9a3f7b2c67915, 0xc67178f2e372532b,
        0xca273eceea26619c, 0xd186b8c721c0c207,
        0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178,
        0x06f067aa72176fba, 0x0a637dc5a2c898a6,
        0x113f9804bef90dae, 0x1b710b35131c471b,
        0x28db77f523047d84, 0x32caab7b40c72493,
        0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c,
        0x4cc5d4becb3e42b6, 0x597f299cfc657e2a,
        0x5fcb6fab3ad6faec, 0x6c44198c4a475817 };

string gethex(const string& bin) {
    static const unordered_map<string, string> binToHex = {
        {"0000", "0"}, {"0001", "1"}, {"0010", "2"}, {"0011", "3"},
        {"0100", "4"}, {"0101", "5"}, {"0110", "6"}, {"0111", "7"},
        {"1000", "8"}, {"1001", "9"}, {"1010", "a"}, {"1011", "b"},
        {"1100", "c"}, {"1101", "d"}, {"1110", "e"}, {"1111", "f"}
    };

    auto it = binToHex.find(bin);
    if (it != binToHex.end()) {
        return it->second;
    } else {
        throw invalid_argument("Invalid 4-bit binary string");
    }
}

string decimaltohex(int64 deci)
{
    string EQBIN = bitset<64>(deci).to_string();

    string hexstring = "";
    string temp;

    for (unsigned int i = 0; i < EQBIN.length(); i += 4) {
        temp = EQBIN.substr(i, 4);
        hexstring += gethex(temp);
    }

    return hexstring;
}

int64 BintoDec(string bin)
{

    int64 value = bitset<64>(bin).to_ullong();
    return value;
}

int64 rotate_right(int64 x, int n)
{
    return (x >> n) | (x << (64 - n));
}

int64 shift_right(int64 x, int n) { return (x >> n); }

void separator(string getBlock)
{
    int chunknum = 0;

    for (unsigned int i = 0; i < getBlock.length();i += 64, ++chunknum) {
        Message[chunknum]= BintoDec(getBlock.substr(i, 64));
    }

    for (int g = 16; g < 80; ++g) {

        int64 WordA = rotate_right(Message[g - 2], 19) ^ rotate_right(Message[g - 2], 61) ^ shift_right(Message[g - 2], 6);

        int64 WordB = Message[g - 7];

        int64 WordC = rotate_right(Message[g - 15], 1) ^ rotate_right(Message[g - 15], 8) ^ shift_right(Message[g - 15], 7);

        int64 WordD = Message[g - 16];

        int64 T = WordA + WordB + WordC + WordD;

        Message[g] = T;
    }
}

int64 maj(int64 a, int64 b, int64 c)
{
    return (a & b) ^ (b & c) ^ (c & a);
}

int64 Ch(int64 e, int64 f, int64 g)
{
    return (e & f) ^ (~e & g);
}

int64 sigmaE(int64 e)
{
    return rotate_right(e, 14) ^ rotate_right(e, 18) ^ rotate_right(e, 41);
}

int64 sigmaA(int64 a)
{

    return rotate_right(a, 28) ^ rotate_right(a, 34) ^ rotate_right(a, 39);
}

void Func(int64 a, int64 b, int64 c, int64& d, int64 e, int64 f, int64 g, int64& h, int K)
{
    int64 T1 = h + Ch(e, f, g) + sigmaE(e) + Message[K] + Constants[K];
    int64 T2 = sigmaA(a) + maj(a, b, c);

    d = d + T1;
    h = T1 + T2;
}

string SHA512(string myString)
{
    int64 A = 0x6a09e667f3bcc908;
    int64 B = 0xbb67ae8584caa73b;
    int64 C = 0x3c6ef372fe94f82b;
    int64 D = 0xa54ff53a5f1d36f1;
    int64 E = 0x510e527fade682d1;
    int64 F = 0x9b05688c2b3e6c1f;
    int64 G = 0x1f83d9abfb41bd6b;
    int64 H = 0x5be0cd19137e2179;

    int64 AA, BB, CC, DD, EE, FF, GG, HH;

    stringstream fixedstream;

    for (int i = 0; i < myString.size(); ++i) {
        fixedstream << bitset<8>(myString[i]);
    }

    string s1024;

    s1024 = fixedstream.str();

    int orilen = s1024.length();
    int tobeadded;

    int modded = s1024.length() % 1024;

    if (1024 - modded >= 128) {
        tobeadded = 1024 - modded;
    } else if (1024 - modded < 128) {
        tobeadded = 2048 - modded;
    }

    s1024 += "1";

    for (int y = 0; y < tobeadded - 129; y++) {
        s1024 += "0";
    }

    string lengthbits = std::bitset<128>(orilen).to_string();

    s1024 += lengthbits;

    int blocksnumber = s1024.length() / 1024;

    int chunknum = 0;

    string Blocks[blocksnumber];

    for (int i = 0; i < s1024.length(); i += 1024, ++chunknum) {
        Blocks[chunknum] = s1024.substr(i, 1024);
    }

    for (int letsgo = 0; letsgo < blocksnumber; ++letsgo) {

        separator(Blocks[letsgo]);

        AA = A;
        BB = B;
        CC = C;
        DD = D;
        EE = E;
        FF = F;
        GG = G;
        HH = H;

        int count = 0;

        for (int i = 0; i < 10; i++) {

            Func(A, B, C, D, E, F, G, H, count);
            count++;
            Func(H, A, B, C, D, E, F, G, count);
            count++;
            Func(G, H, A, B, C, D, E, F, count);
            count++;
            Func(F, G, H, A, B, C, D, E, count);
            count++;
            Func(E, F, G, H, A, B, C, D, count);
            count++;
            Func(D, E, F, G, H, A, B, C, count);
            count++;
            Func(C, D, E, F, G, H, A, B, count);
            count++;
            Func(B, C, D, E, F, G, H, A, count);
            count++;
        }

        A += AA;
        B += BB;
        C += CC;
        D += DD;
        E += EE;
        F += FF;
        G += GG;
        H += HH;
    }

    stringstream output;

    output << decimaltohex(A);
    output << decimaltohex(B);
    output << decimaltohex(C);
    output << decimaltohex(D);
    output << decimaltohex(E);
    output << decimaltohex(F);
    output << decimaltohex(G);
    output << decimaltohex(H);


    return output.str();
}


/// SHA256 ENCRYPTION

typedef struct {
	uchar data[64];
	uint datalen;
	uint bitlen[2];
	uint state[8];
} SHA256_CTX;

uint k[64] = {
	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
	0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
	0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
	0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
	0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
	0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
	0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
	0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

void SHA256Transform(SHA256_CTX *ctx, uchar data[])
{
	uint a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];

	for (i = 0, j = 0; i < 16; ++i, j += 4)
		m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);
	for (; i < 64; ++i)
		m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];

	a = ctx->state[0];
	b = ctx->state[1];
	c = ctx->state[2];
	d = ctx->state[3];
	e = ctx->state[4];
	f = ctx->state[5];
	g = ctx->state[6];
	h = ctx->state[7];

	for (i = 0; i < 64; ++i) {
		t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
		t2 = EP0(a) + MAJ(a, b, c);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}

	ctx->state[0] += a;
	ctx->state[1] += b;
	ctx->state[2] += c;
	ctx->state[3] += d;
	ctx->state[4] += e;
	ctx->state[5] += f;
	ctx->state[6] += g;
	ctx->state[7] += h;
}

void SHA256Init(SHA256_CTX *ctx)
{
	ctx->datalen = 0;
	ctx->bitlen[0] = 0;
	ctx->bitlen[1] = 0;
	ctx->state[0] = 0x6a09e667;
	ctx->state[1] = 0xbb67ae85;
	ctx->state[2] = 0x3c6ef372;
	ctx->state[3] = 0xa54ff53a;
	ctx->state[4] = 0x510e527f;
	ctx->state[5] = 0x9b05688c;
	ctx->state[6] = 0x1f83d9ab;
	ctx->state[7] = 0x5be0cd19;
}

void SHA256Update(SHA256_CTX *ctx, uchar data[], uint len)
{
	for (uint i = 0; i < len; ++i) {
		ctx->data[ctx->datalen] = data[i];
		ctx->datalen++;
		if (ctx->datalen == 64) {
			SHA256Transform(ctx, ctx->data);
			DBL_INT_ADD(ctx->bitlen[0], ctx->bitlen[1], 512);
			ctx->datalen = 0;
		}
	}
}

void SHA256Final(SHA256_CTX *ctx, uchar hash[])
{
	uint i = ctx->datalen;

	if (ctx->datalen < 56) {
		ctx->data[i++] = 0x80;

		while (i < 56)
			ctx->data[i++] = 0x00;
	}
	else {
		ctx->data[i++] = 0x80;

		while (i < 64)
			ctx->data[i++] = 0x00;

		SHA256Transform(ctx, ctx->data);
		memset(ctx->data, 0, 56);
	}

	DBL_INT_ADD(ctx->bitlen[0], ctx->bitlen[1], ctx->datalen * 8);
	ctx->data[63] = ctx->bitlen[0];
	ctx->data[62] = ctx->bitlen[0] >> 8;
	ctx->data[61] = ctx->bitlen[0] >> 16;
	ctx->data[60] = ctx->bitlen[0] >> 24;
	ctx->data[59] = ctx->bitlen[1];
	ctx->data[58] = ctx->bitlen[1] >> 8;
	ctx->data[57] = ctx->bitlen[1] >> 16;
	ctx->data[56] = ctx->bitlen[1] >> 24;
	SHA256Transform(ctx, ctx->data);

	for (i = 0; i < 4; ++i) {
		hash[i] = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 4] = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 8] = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 20] = (ctx->state[5] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 24] = (ctx->state[6] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 28] = (ctx->state[7] >> (24 - i * 8)) & 0x000000ff;
	}
}

// MD5 ENCRYPTION

const uint32_t K[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

const uint32_t S[64] = {
    7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
    5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
    4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
    6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
};

uint32_t leftRotate(uint32_t x, uint32_t c) {
    return (x << c) | (x >> (32 - c));
}

std::string md5(const std::string& input) {
    uint32_t a0 = 0x67452301;
    uint32_t b0 = 0xefcdab89;
    uint32_t c0 = 0x98badcfe;
    uint32_t d0 = 0x10325476;

    uint64_t originalLength = input.length() * 8;
    std::string msg = input;
    msg += (char)0x80;
    while ((msg.length() * 8) % 512 != 448) {
        msg += (char)0x00;
    }
    msg += std::string(8, '\0');

    for (int i = 0; i < 8; ++i) {
        msg[msg.length() - 8 + i] = (originalLength >> (i * 8)) & 0xFF;
    }

    auto F = [](uint32_t x, uint32_t y, uint32_t z) { return (x & y) | (~x & z); };
    auto G = [](uint32_t x, uint32_t y, uint32_t z) { return (x & z) | (y & ~z); };
    auto H = [](uint32_t x, uint32_t y, uint32_t z) { return x ^ y ^ z; };
    auto I = [](uint32_t x, uint32_t y, uint32_t z) { return y ^ (x | ~z); };

    auto rotate_left = [](uint32_t x, uint32_t n) { return (x << n) | (x >> (32 - n)); };

    const uint32_t K[64] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
        0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
        0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
        0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
        0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
        0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
    };

    const uint32_t S[64] = {
        7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
        5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
        4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
        6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
    };

    auto process_chunk = [&](const std::string& chunk) {
        uint32_t A = a0, B = b0, C = c0, D = d0;

        for (int i = 0; i < 64; ++i) {
            uint32_t F_val, g;
            if (i < 16) {
                F_val = F(B, C, D);
                g = i;
            } else if (i < 32) {
                F_val = G(B, C, D);
                g = (5 * i + 1) % 16;
            } else if (i < 48) {
                F_val = H(B, C, D);
                g = (3 * i + 5) % 16;
            } else {
                F_val = I(B, C, D);
                g = (7 * i) % 16;
            }

            uint32_t temp = D;
            D = C;
            C = B;
            B = B + rotate_left(A + F_val + K[i] + ((uint32_t*)chunk.data())[g], S[i]);
            A = temp;
        }

        a0 += A;
        b0 += B;
        c0 += C;
        d0 += D;
    };

    for (size_t offset = 0; offset < msg.size(); offset += 64) {
        process_chunk(msg.substr(offset, 64));
    }

    std::stringstream output;
    auto to_hex = [&](uint32_t value) {
        for (int i = 0; i < 4; ++i) {
            output << std::hex << std::setw(2) << std::setfill('0') << ((value >> (i * 8)) & 0xFF);
        }
    };

    to_hex(a0);
    to_hex(b0);
    to_hex(c0);
    to_hex(d0);

    return output.str();
}


string SHA256(char* data) {
    SHA256_CTX ctx;
    unsigned char hash[32];
    string output;

    SHA256Init(&ctx);
    SHA256Update(&ctx, reinterpret_cast<uchar*>(data), strlen(data));
    SHA256Final(&ctx, hash);

    for (int i = 0; i < 32; i++) {
        char buf[3];
        sprintf(buf, "%02x", hash[i]);
        output += buf;
    }

    return output;
}


string trim(const string& str) {
    auto start = str.begin();
    while (start != str.end() && isspace(*start)) {
        start++;
    }
    auto end = str.end();
    do {
        end--;
    } while (distance(start, end) > 0 && isspace(*end));
    return string(start, end + 1);
}

string encryptSHA512(const string& input) {
    return SHA512(input);
}

string encryptSHA256(const string& input) {
    SHA256_CTX ctx;
    uchar hash[32];
    SHA256Init(&ctx);
    SHA256Update(&ctx, (uchar*)input.c_str(), input.length());
    SHA256Final(&ctx, hash);
    
    std::stringstream ss;
    for (int i = 0; i < 32; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

void clearConsole() {
    std::cout << "\033[2J\033[1;1H";
}



int main() {

    // main program loop
    while (true) {
        clearConsole();

        int choice = 0;
        bool inmenu = false;
        // Set static custom key
        //string key = "5d41402abc4b2a76b9719d911017c592"; // "hello"
        string key = "";

        cout << "1) Generate hashing key" << endl;
        cout << "2) SHA256 hashing" << endl;
        cout << "3) SHA512 hashing" << endl;
        cout << "4) Set hashing key" << endl << endl;

        cout << ": ";
        cin >> choice;

        if (choice != 0) {
            inmenu = true;
        }

        clearConsole();

        while (inmenu) {
            if (choice == 1) {
                string base;
                cout << "Generate hashing key" << endl << endl;
                cout << "Enter a string: ";
                cin >> base;
                key = md5(base);
                cout << "Your generated key : " << key << endl;
                cout << "Save this key and keep it safe. Press enter to return home...";
                cin.ignore();
                getline(cin, base);
                inmenu = false;
            } else if (choice == 2) {
                string text;
                cout << "SHA256 hashing" << endl << endl;
                cout << "Enter a string: ";
                cin >> text;
                cout << "Generated SHA256 hash :" << encryptSHA256(text + key) << endl;
                cout << "Press enter to return home...";
                cin.ignore();
                getline(cin, text);
                inmenu = false;
            } else if (choice == 3) {
                string text;
                cout << "SHA512 hashing" << endl << endl;
                cout << "Enter a string: ";
                cin >> text;
                cout << "Generated SHA512 hash :" << encryptSHA512(text + key) << endl;
                cout << "Press enter to return home...";
                cin.ignore();
                getline(cin, text);
                inmenu = false;
            } else if (choice == 4) {
                string input;
                cout << "Set hashing key" << endl << endl;

                if (!key.empty()) {
                    cout << "Note : you already have a key set." << endl;
                    for (int i = 0; i < key.length(); i++) {
                        if (i < 3) {
                            cout << key.at(i);
                        } else {
                            cout << "*";
                        }
                    }
                }
                cout << endl << "Enter your key: ";
                cin.ignore();
                getline(cin, key);

                if (key.empty()) {
                    inmenu = false;
                }

                cout << "Press enter to return home...";
                getline(cin, input);
                inmenu = false;
            } else {
                inmenu = false;
            }
        }
    }

    return 0;
}
