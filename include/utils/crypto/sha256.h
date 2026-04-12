#ifndef SHA256_EXT
#define SHA256_EXT

#include <cmath>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <climits>
#include <string>

#include <iostream>

// print_binary(...) needs ../stringutils.h
// #include "../stringutils.h"

/*

SHA-256 Hash algorithm. Breaks message into 512 bit chunks, and the function generates a combined hash based on the message.

*/

namespace SHA256 {
uint32_t k[64] = {   0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
   0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
   0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
   0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
   0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
   0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
   0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
   0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};
};

// Not done yet

uint32_t rightrotate(uint32_t x, int iter = 1) {
    if (iter <= 0) return x;
    for (int i = 0; i < iter; i++) {
        bool sign = x & 1;
        x >>= 1;
        if (sign) x |= (1L<<31);
    }
    return x;
}

uint32_t leftrotate(uint32_t x, int iter = 1) {
    if (iter <= 0) return x;
    for (int i = 0; i < iter; i++) {
        bool sign = x & (1L<<31);
        x <<= 1;
        x |= sign;
    }
    return x;
}

void sha256util(std::string& input, int si, std::vector<uint32_t>& H) {
    // std::cout << si << "\n";
    uint32_t w[64] = {0};

    // 1. copy the chunk into w[0 ... 15]
    for (int i = 0; i < 16; i++) {
        int index = si + (i<<2);
        uint32_t a0 = (unsigned char)(input[index]);
        uint32_t a1 = (unsigned char)(input[index + 1]);
        uint32_t a2 = (unsigned char)(input[index + 2]);
        uint32_t a3 = (unsigned char)(input[index + 3]);
        // std::cout << print_binary(a0<<24) + " | " + print_binary(a1) + " | " + print_binary(a2 << 8) + " | " + print_binary(a3) + "\n";
        w[i] = (a0 << 24) | (a1 << 16) | (a2 << 8) | a3;
    }

    // for (int i = 0; i < 16; i++) std::cout << print_binary(w[i]) << "\n";

    for (int i = 16; i < 64; i++) {
        uint32_t s0 = rightrotate(w[i-15], 7) ^ rightrotate(w[i-15], 18) ^ (w[i-15] >> 3);
        uint32_t s1 = rightrotate(w[i-2], 17) ^ rightrotate(w[i-2], 19) ^ (w[i-2] >> 10);
        w[i] = w[i-16] + s0 + w[i-7] + s1;
    }

    // for (int i = 16; i < 64; i++) std::cout << print_binary(w[i]) << "\n";

    uint32_t a = H[0];
    uint32_t b = H[1];
    uint32_t c = H[2];
    uint32_t d = H[3];
    uint32_t e = H[4];
    uint32_t f = H[5];
    uint32_t g = H[6];
    uint32_t h = H[7];

    for (int i = 0; i < 64; i++) {
        uint32_t S1 = rightrotate(e, 6) ^ rightrotate(e, 11) ^ rightrotate(e, 25);
        uint32_t ch = (e & f) ^ ((~e) & g);
        uint32_t temp1 = h + S1 + ch + SHA256::k[i] + w[i];
        uint32_t S0 = rightrotate(a, 2) ^ rightrotate(a, 13) ^ rightrotate(a, 22);
        uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
        uint32_t temp2 = S0 + maj;
 
        h = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
    }

    H[0] += a;
    H[1] += b;
    H[2] += c;
    H[3] += d;
    H[4] += e;
    H[5] += f;
    H[6] += g;
    H[7] += h;
}

std::vector<uint32_t> sha256(std::string input2) {
    std::string input = input2;
    uint64_t L = (input.size())<<3; // length in bits
    std::vector<uint32_t> h({0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19});

    // determine the value of K such that L + 1 + K + 64 % 512 == 0
    uint64_t baseline = L>>9;
    for (int i = 0; i < 8; baseline++) {
        if (L + 1 + 64 <= (baseline<<9)) break;
        i++;
    }

    uint64_t Lprime = baseline<<9;
    uint64_t K = Lprime - L - 65;

    // append 10000000 to the string
    input.push_back((char)(128));
    K -= 7;

    // append zeros
    for (int i = 0; i < (K>>3); i++) input.push_back('\0');

    // big endian integer, L
    for (int i = 7; i >= 0; i--) {
        uint8_t slice = (L >> (i<<3)) & 255;
        input.push_back((char)(slice));
    }

    // std::cout << input.size() << "\n";

    // std::cout << print_string_as_binary(input) << "\n";

    for (int i = 0; i < input.size(); i += (512>>3)) {
        sha256util(input, i, h);
    } 

    return h;
}

#endif