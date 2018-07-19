#include "codumpresale.hpp"

void codumpresale::apply(account_name contributor,
                         bool acceptedterms)
{
  require_auth(contributor);

  whitelists whitelist_table(_self, _self);
  auto itr = whitelist_table.find(contributor);
  if (itr == whitelist_table.end()) {
    // create
    whitelist_table.emplace(_self, [&](auto &wt) {
      wt.contributor = contributor;
      wt.acceptedterms = acceptedterms;
      wt.approved = 0;
    });
  } else {
    // update
    whitelist_table.modify(itr, _self, [&](auto &wt) {
      wt.contributor = contributor;
      wt.acceptedterms = acceptedterms;
    });
  }
}

void codumpresale::approve(account_name contributor)
{
  require_auth(_self);
  eosio_assert(is_account(contributor), "contributor account does not exist");

  whitelists whitelist_table(_self, _self);
  auto itr = whitelist_table.find(contributor);

  eosio_assert(itr != whitelist_table.end(), "there is no such contributor in whitelist");
  eosio_assert(itr->acceptedterms, "contributor have not accepted terms");

  whitelist_table.modify(itr, _self, [&](auto &wt) {
    wt.approved = now();
  });
}

EOSIO_ABI(codumpresale, (apply)(approve))
