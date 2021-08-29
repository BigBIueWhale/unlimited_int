#include "unlimited_int.hpp"
using namespace unlimited;
reciprocal_information::reciprocal_information(std::shared_ptr<unlimited_int> reciprocal, size_t amount_shifted) :
	reciprocal(reciprocal),
	amount_shifted(amount_shifted){}
void reciprocals_database::clear()
{
	this->most_recent.clear();
	this->reciprocals_map.clear();
}
reciprocal_information::reciprocal_information(const reciprocal_information& other) :
	amount_shifted(other.amount_shifted),
	reciprocal(other.reciprocal) {}
reciprocal_information_for_database::reciprocal_information_for_database(const reciprocal_information& reciprocal_info, std::shared_ptr<unlimited_int> hash_of_dividend, custom_linked_list_node<size_t>* link_to_list) :
	reciprocal_information(reciprocal_info),
	hash_of_dividend(hash_of_dividend),
	link_to_list(link_to_list) {}
reciprocal_information_for_database::reciprocal_information_for_database(const reciprocal_information_for_database& other) : 
	reciprocal_information(other.reciprocal, other.amount_shifted),
	hash_of_dividend(other.hash_of_dividend),
	link_to_list(other.link_to_list) {}