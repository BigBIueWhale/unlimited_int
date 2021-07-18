#include "unlimited_int.hpp"
#define ROTATER(x, amount) ((x >> amount) | (x << (32 - amount)))
#define LOWERCASE_SIGMA0(x) ((ROTATER(x, 7)) ^ (ROTATER(x, 18)) ^ (x >> 3))
#define LOWERCASE_SIGMA1(x) ((ROTATER(x, 17)) ^ (ROTATER(x, 19)) ^ (x >> 10))
#define UPPERCASE_SIGMA0(x) ((ROTATER(x, 2)) ^ (ROTATER(x, 13)) ^ (ROTATER(x, 22)))
#define UPPERCASE_SIGMA1(x) ((ROTATER(x, 6)) ^ (ROTATER(x, 11)) ^ (ROTATER(x, 25)))
#define CHOICE(x, y, z) ((x & y) ^ ((~x) & z))
#define MAJORITY(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
void SHA256_compress_message_block(const uint32_t* message_block, uint32_t* parameter_hash_values); //accepts a message block with 512 bits, meaning an array of 16 32bit integers
using namespace unlimited;

unlimited_int* unlimited_int::calculate_sha256_hash() const
{
	uint32_t current_hash_values[8] = {
		0x6a09e667U, 0xbb67ae85U, 0x3c6ef372U, 0xa54ff53aU,
		0x510e527fU, 0x9b05688cU, 0x1f83d9abU, 0x5be0cd19U
	};
	uint32_t current_block[16];
	if (this->is_zero())
	{
		current_block[0] = (uint32_t)1ULL << 31; //the 1 at the end of the message
		for (int word_index = 1; word_index < 16; ++word_index)
			current_block[word_index] = 0U;
		SHA256_compress_message_block(current_block, current_hash_values);
	}
	else
	{
		const Node* current_int_array_Node = this->get_most_significant_used_int_array();
		int_array current_int_array = *current_int_array_Node->value;
		many_bits_signed current_index_in_int_array = current_int_array.num_of_used_ints - 1;
		const few_bits most_significant_used_few_bits_in_number = current_int_array.intarr[current_index_in_int_array];
		many_bits length_of_preimage_in_bits = this->num_of_used_ints * (many_bits)(sizeof(few_bits)) * (many_bits)8U;
		int index_in_block = 0;
#if NUM_OF_BITS_few_bits == 16
		few_bits previous_few_bits;
		bool have_beginning_of_number;
		if (this->num_of_used_ints % 2 == 1)
		{
			length_of_preimage_in_bits += 16;
			previous_few_bits = 0U;
			current_block[index_in_block] = (uint32_t)most_significant_used_few_bits_in_number;
			++index_in_block;
			have_beginning_of_number = false;
		}
		else
		{
			previous_few_bits = most_significant_used_few_bits_in_number;
			have_beginning_of_number = true;
		}
#else
		current_block[index_in_block] = most_significant_used_few_bits_in_number;
		++index_in_block;
#endif
		--current_index_in_int_array;
		if (current_index_in_int_array < 0)
		{
			current_int_array_Node = current_int_array_Node->previous;
			if (current_int_array_Node != nullptr) //reached the end of the list_of_int_arrays
			{
				current_int_array = *current_int_array_Node->value;
				current_index_in_int_array = current_int_array.num_of_used_ints - 1;
			}
		}
		if (current_int_array_Node != nullptr)
		{
			while (true)
			{
				const few_bits current_few_bits = current_int_array.intarr[current_index_in_int_array];
#if NUM_OF_BITS_few_bits == 16
				if (have_beginning_of_number)
				{
					current_block[index_in_block] = ((uint32_t)previous_few_bits << 16) + (uint32_t)current_few_bits;
					have_beginning_of_number = false;
					++index_in_block;
				}
				else
				{
					previous_few_bits = current_few_bits;
					have_beginning_of_number = true;
				}
#else
				current_block[index_in_block] = current_few_bits;
				++index_in_block;
#endif
				if (index_in_block >= 16)
				{
					index_in_block = 0;
					SHA256_compress_message_block(current_block, current_hash_values);
				}
				--current_index_in_int_array;
				if (current_index_in_int_array < 0)
				{
					current_int_array_Node = current_int_array_Node->previous;
					if (current_int_array_Node == nullptr) //reached the end of the list_of_int_arrays
						break;
					current_int_array = *current_int_array_Node->value;
					current_index_in_int_array = current_int_array.num_of_used_ints - 1;
				}
			}
		}
		if (index_in_block >= 16) //not enough room for the 1 at the end of the current block
		{
			index_in_block = 0;
			SHA256_compress_message_block(current_block, current_hash_values);
			for (int word_index = 0; word_index < 16; ++word_index)
				current_block[word_index] = 0x0U;
		}
		else
		{
			for (int word_index = index_in_block; word_index < 16; ++word_index)
				current_block[word_index] = 0x0U;
		}
		current_block[index_in_block] |= (uint32_t)0x1U << 31; //sets the 1 bit in the end of the data
		const int num_of_uint32s_left = 16 - (index_in_block + 1);
		if (num_of_uint32s_left < 2) //not enough place left in current_block to assign the 64-bit length of the data in bits.
		{
			SHA256_compress_message_block(current_block, current_hash_values);
			for (int word_index = 0; word_index < 16; ++word_index)
				current_block[word_index] = 0x0U;
		}
#if NUM_OF_BITS_few_bits == 32
		current_block[15] = (uint32_t)(length_of_preimage_in_bits & MASK_LOW_BITS);
		current_block[14] = (uint32_t)(length_of_preimage_in_bits >> NUM_OF_BITS_few_bits);
#else
		current_block[15] = (uint32_t)length_of_preimage_in_bits;
#endif
		SHA256_compress_message_block(current_block, current_hash_values);
	}
	return new unlimited_int(current_hash_values, 8);
}
void SHA256_compress_message_block(const uint32_t* message_block, uint32_t* parameter_hash_values)
{
	uint32_t message_schedule[64];
	for (size_t word_index = 0; word_index < 16; ++word_index)
		message_schedule[word_index] = message_block[word_index];
	for (size_t word_index = 16; word_index < 64; ++word_index)
		message_schedule[word_index] = LOWERCASE_SIGMA1(message_schedule[word_index - 2]) + message_schedule[word_index - 7] + LOWERCASE_SIGMA0(message_schedule[word_index - 15]) + message_schedule[word_index - 16];

	const uint32_t constants[64] = {
		0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
		0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
		0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
		0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
	};
	uint32_t a = parameter_hash_values[0];
	uint32_t b = parameter_hash_values[1];
	uint32_t c = parameter_hash_values[2];
	uint32_t d = parameter_hash_values[3];
	uint32_t e = parameter_hash_values[4];
	uint32_t f = parameter_hash_values[5];
	uint32_t g = parameter_hash_values[6];
	uint32_t h = parameter_hash_values[7];
	for (size_t word_index = 0; word_index < 64; ++word_index)
	{
		const uint32_t T1 = UPPERCASE_SIGMA1(e) + CHOICE(e, f, g) + h + constants[word_index] + message_schedule[word_index];
		const uint32_t T2 = UPPERCASE_SIGMA0(a) + MAJORITY(a, b, c);
		h = g;
		g = f;
		f = e;
		e = d + T1;
		d = c;
		c = b;
		b = a;
		a = T1 + T2;
	}
	parameter_hash_values[0] += a;
	parameter_hash_values[1] += b;
	parameter_hash_values[2] += c;
	parameter_hash_values[3] += d;
	parameter_hash_values[4] += e;
	parameter_hash_values[5] += f;
	parameter_hash_values[6] += g;
	parameter_hash_values[7] += h;
}
