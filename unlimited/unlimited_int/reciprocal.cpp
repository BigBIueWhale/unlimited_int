#include "unlimited_int.hpp"
using namespace unlimited;
reciprocal_information::reciprocal_information(std::unique_ptr<unlimited_int>&& reciprocal, size_t amount_shifted) :
	reciprocal(std::move(reciprocal))
{
	this->amount_shifted = amount_shifted;
}
void reciprocals_database::clear()
{
	this->most_recent.clear();
	this->reciprocals_map.clear();
}
reciprocal_information::reciprocal_information(reciprocal_information&& other) noexcept :
	reciprocal(std::move(other.reciprocal))
{
	this->amount_shifted = other.amount_shifted;
	other.amount_shifted = (size_t)0;
}
reciprocal_information_for_database::reciprocal_information_for_database(reciprocal_information&& reciprocal_info, std::unique_ptr<unlimited_int>&& hash_of_dividend, custom_linked_list_node<size_t>* link_to_list) :
	reciprocal_information(std::move(reciprocal_info)),
	hash_of_dividend(std::move(hash_of_dividend))
{
	this->link_to_list = link_to_list;
	link_to_list = nullptr;
}
reciprocal_information_for_database::reciprocal_information_for_database(reciprocal_information_for_database&& other) noexcept: 
	reciprocal_information(std::move(other.reciprocal), other.amount_shifted),
	hash_of_dividend(std::move(other.hash_of_dividend))
{
	this->link_to_list = other.link_to_list;
	other.link_to_list = nullptr;
}
