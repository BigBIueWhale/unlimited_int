#include "unlimited_int.hpp"
using namespace unlimited;
reciprocal_information::reciprocal_information(std::shared_ptr<unlimited_int> reciprocal, many_bits amount_shifted)
{
	this->reciprocal = reciprocal;
	this->amount_shifted = amount_shifted;
}
void reciprocals_database::clear()
{
	this->most_recent.clear();
	this->reciprocals_map.clear();
}
reciprocal_information::reciprocal_information(const reciprocal_information& other)
{
	this->amount_shifted = other.amount_shifted;
	this->reciprocal = other.reciprocal;
}
reciprocal_information_for_database::reciprocal_information_for_database(reciprocal_information reciprocal_info, std::shared_ptr<unlimited_int> hash_of_dividend, custom_Node<size_t>* link_to_list)
{
	this->reciprocal = reciprocal_info.reciprocal;
	this->amount_shifted = reciprocal_info.amount_shifted;
	this->hash_of_dividend = hash_of_dividend;
	this->link_to_list = link_to_list;
}
reciprocal_information_for_database::reciprocal_information_for_database(const reciprocal_information_for_database& other)
{
	this->reciprocal = other.reciprocal;
	this->amount_shifted = other.amount_shifted;
	this->hash_of_dividend = other.hash_of_dividend;
	this->link_to_list = other.link_to_list;
}