#pragma once

#include <eosiolib/eosio.hpp>
#include <eosiolib/types.hpp>
#include <eosiolib/asset.hpp>

#include <string>

using std::string;
using namespace eosio;

class codumpresale : public contract
{
public:
  codumpresale(account_name self) : contract(self) {}

  /// @abi action
  void apply(account_name contributor, bool acceptedterms);

  /// @abi action
  void approve(account_name contributor);

  inline bool is_contributor_approved(account_name contributor) const;

private:
  /// @abi table whitelist i64
  struct whitelist
  {
    account_name contributor;
    bool acceptedterms;
    time approved;

    uint64_t primary_key() const { return contributor; }

    EOSLIB_SERIALIZE(whitelist, (contributor)(acceptedterms)(approved))
  };

  typedef eosio::multi_index<N(whitelist), whitelist> whitelists;
}; // class codumpresale.

bool codumpresale::is_contributor_approved(account_name contributor) const
{
  whitelists whitelist_table(_self, _self);
  auto itr = whitelist_table.find(contributor);
  if (itr != whitelist_table.end() && itr->acceptedterms && itr->approved > 0) {
    return true;
  }
  return false;
}
