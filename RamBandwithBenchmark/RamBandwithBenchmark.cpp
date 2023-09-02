#include <chrono>
typedef unsigned char byte;
// RamBandwithBenchmark.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>


void do_math(byte* data, byte* result, unsigned int byte_count)
{
	int end = reinterpret_cast<int>(data + byte_count);

	__asm vmovdqu ymm0, [data]

	while(reinterpret_cast<int>(data) < end)
	{
		__asm vpaddb ymm1, ymm0, ymm0
		__asm vmovdqu ymm0, [data]
		data += 32;
		__asm vmovdqu [result], ymm1
		result += 32;
	}
}

constexpr auto DATASIZE = 1024 * 1024 * 1024;
constexpr auto ITERATIONS = 50;
int main()
{
	const auto src = malloc(DATASIZE);
	const auto dst = malloc(DATASIZE);

	const auto start = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < ITERATIONS; i++)
	{
		do_math(static_cast<byte*>(src), static_cast<byte*>(dst), DATASIZE);
	}

	const auto end = std::chrono::high_resolution_clock::now();

	const std::chrono::duration<double> duration = end - start;
	std::cout << "Execution time: " << duration.count() << " seconds" << std::endl;
	std::cout << "Bandwith: " << ITERATIONS / duration.count() * 2 << " GB/s " << ITERATIONS / duration.count() * 2 * 8 << " Gbit/s" << std::endl;

	return 0;
}

