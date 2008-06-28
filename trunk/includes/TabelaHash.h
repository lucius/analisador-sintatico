#ifndef TABELAHASH_H_
#define TABELAHASH_H_

#include <ext/hash_map>
#include <iostream>

#include "../includes/FuncaoHash.h"

typedef __gnu_cxx::hash_map< std::string, structSintatico, funcaoHash<std::string> > TabelaHash;

#endif /* TABELAHASH_H_ */
