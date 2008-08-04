#ifndef TABELAHASH_H_
#define TABELAHASH_H_

#include <ext/hash_map>
#include <iostream>
#include <string>

#include "../includes/FuncaoHash.h"
#include "../includes/ConteudoHash.h"

typedef __gnu_cxx::hash_map< std::string, ConteudoHash, FuncaoHash<std::string> > TabelaHash;

#endif /* TABELAHASH_H_ */
