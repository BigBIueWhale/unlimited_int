#include "unlimited_int.hpp"
void SHA512_compress_message_block(const uint64_t* message_block, uint64_t* parameter_hash_values); //accepts a message block with 1024 bits, meaning an array of 16 32bit integers

#define ROTATER(x, amount) ((x >> amount) | (x << (64 - amount)))
#define LOWERCASE_SIGMA0(x) ((ROTATER(x, 1)) ^ (ROTATER(x, 8)) ^ (x >> 7))
#define LOWERCASE_SIGMA1(x) ((ROTATER(x, 19)) ^ (ROTATER(x, 61)) ^ (x >> 6))
#define UPPERCASE_SIGMA0(x) ((ROTATER(x, 28)) ^ (ROTATER(x, 34)) ^ (ROTATER(x, 39)))
#define UPPERCASE_SIGMA1(x) ((ROTATER(x, 14)) ^ (ROTATER(x, 18)) ^ (ROTATER(x, 41)))
#define CHOICE(x, y, z) ((x & y) ^ ((~x) & z))
#define MAJORITY(x, y, z) ((x & y) ^ (x & z) ^ (y & z))

using namespace unlimited;
unlimited_int* unlimited_int::calculate_sha512_hash() const
{
	uint64_t current_hash_values[8] = {
		0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL, 0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
		0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL, 0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL
	};
	uint64_t current_block[16];
	if (this->is_zero())
	{
		current_block[0] = 1ULL << 63; //the 1 at the end of the message
		for (int word_index = 1; word_index < 16; ++word_index)
			current_block[word_index] = 0U;
		SHA512_compress_message_block(current_block, current_hash_values);
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
		int progress_to_reach_64_bits = 0;
		uint64_t number_builder = 0U;
		const uint64_t top_num = (uint64_t)most_significant_used_few_bits_in_number;
		const int len_mod_4 = this->num_of_used_ints % 4;
		length_of_preimage_in_bits += ((4 - len_mod_4) % 4) * 16;
		const int amount_to_shift = ((len_mod_4 == 0) ? 3 : (len_mod_4 - 1)) * 16;
		number_builder = top_num << amount_to_shift;
		progress_to_reach_64_bits = 64 - amount_to_shift;
		if (progress_to_reach_64_bits == 64)
		{
			current_block[index_in_block] = number_builder;
			++index_in_block;
			number_builder = 0U;
			progress_to_reach_64_bits = 0;
		}
#elif NUM_OF_BITS_few_bits == 32
		few_bits previous_few_bits;
		bool have_beginning_of_number;
		if (this->num_of_used_ints % 2 == 1)
		{
			length_of_preimage_in_bits += 32;
			previous_few_bits = 0U;
			current_block[index_in_block] = (uint64_t)most_significant_used_few_bits_in_number;
			++index_in_block;
			have_beginning_of_number = false;
		}
		else
		{
			previous_few_bits = most_significant_used_few_bits_in_number;
			have_beginning_of_number = true;
		}
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
				const uint64_t current_num = (uint64_t)current_few_bits;
				progress_to_reach_64_bits += 16;
				const int amount_to_shift = 64 - progress_to_reach_64_bits;
				number_builder |= current_num << amount_to_shift;
				if (progress_to_reach_64_bits == 64)
				{
					current_block[index_in_block] = number_builder;
					++index_in_block;
					number_builder = 0U;
					progress_to_reach_64_bits = 0;
				}
#elif NUM_OF_BITS_few_bits == 32
				if (have_beginning_of_number)
				{
					current_block[index_in_block] = ((uint64_t)previous_few_bits << 32) + (uint64_t)current_few_bits;
					have_beginning_of_number = false;
					++index_in_block;
				}
				else
				{
					previous_few_bits = current_few_bits;
					have_beginning_of_number = true;
				}
#endif
				if (index_in_block >= 16)
				{
					index_in_block = 0;
					SHA512_compress_message_block(current_block, current_hash_values);
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
			SHA512_compress_message_block(current_block, current_hash_values);
			for (int word_index = 0; word_index < 16; ++word_index)
				current_block[word_index] = 0x0U;
		}
		else
		{
			for (int word_index = index_in_block; word_index < 16; ++word_index)
				current_block[word_index] = 0x0U;
		}
		current_block[index_in_block] |= (uint64_t)0x1U << 63; //sets the 1 bit in the end of the data
		const int num_of_uint64s_left = 16 - (index_in_block + 1);
		if (num_of_uint64s_left < 2) //not enough place left in current_block to assign the 64-bit length of the data in bits.
		{
			SHA512_compress_message_block(current_block, current_hash_values);
			for (int word_index = 0; word_index < 16; ++word_index)
				current_block[word_index] = 0x0U;
		}
		current_block[15] = (uint64_t)length_of_preimage_in_bits;
		SHA512_compress_message_block(current_block, current_hash_values);
	}
	return new unlimited_int(current_hash_values, 8);
}
void SHA512_compress_message_block(const uint64_t* message_block, uint64_t* parameter_hash_values)
{
    uint64_t message_schedule[80];
    for (size_t word_index = 0; word_index < 64; ++word_index)
        message_schedule[word_index] = message_block[word_index];
    for (size_t word_index = 16; word_index < 80; ++word_index)
        message_schedule[word_index] = LOWERCASE_SIGMA1(message_schedule[word_index - 2]) + message_schedule[word_index - 7] + LOWERCASE_SIGMA0(message_schedule[word_index - 15]) + message_schedule[word_index - 16];

    const uint64_t constants[80] = {
        0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL, 0x3956c25bf348b538ULL,
        0x59f111f1b605d019ULL, 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL, 0xd807aa98a3030242ULL, 0x12835b0145706fbeULL,
        0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL, 0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL,
        0xc19bf174cf692694ULL, 0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL, 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL,
        0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL, 0x983e5152ee66dfabULL,
        0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL, 0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL,
        0x06ca6351e003826fULL, 0x142929670a0e6e70ULL, 0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL,
        0x53380d139d95b3dfULL, 0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL,
        0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL, 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL, 0xd192e819d6ef5218ULL,
        0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL, 0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL,
        0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL, 0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL, 0x5b9cca4f7763e373ULL,
        0x682e6ff3d6b2b8a3ULL, 0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
        0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL, 0xca273eceea26619cULL,
        0xd186b8c721c0c207ULL, 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL, 0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL,
        0x113f9804bef90daeULL, 0x1b710b35131c471bULL, 0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL,
        0x431d67c49c100d4cULL, 0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL, 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL
    };
    uint64_t a = parameter_hash_values[0];
    uint64_t b = parameter_hash_values[1];
    uint64_t c = parameter_hash_values[2];
    uint64_t d = parameter_hash_values[3];
    uint64_t e = parameter_hash_values[4];
    uint64_t f = parameter_hash_values[5];
    uint64_t g = parameter_hash_values[6];
    uint64_t h = parameter_hash_values[7];
    for (size_t word_index = 0; word_index < 80; ++word_index)
    {
        const uint64_t T1 = UPPERCASE_SIGMA1(e) + CHOICE(e, f, g) + h + constants[word_index] + message_schedule[word_index];
        const uint64_t T2 = UPPERCASE_SIGMA0(a) + MAJORITY(a, b, c);
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
