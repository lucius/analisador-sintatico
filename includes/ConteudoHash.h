#ifndef CONTEUDOHASH_H_
#define CONTEUDOHASH_H_

#include <iostream>
#include <string>

#include "../includes/StructHashVariavel.h"
#include "../includes/StructHashProcedureFunction.h"
#include "../includes/StructHashParametrosFormais.h"



class ConteudoHash
{
	public:
		ConteudoHash( std::string, std::string, unsigned short int, std::string, unsigned short int );

		ConteudoHash( std::string, std::string, unsigned short int, std::string, unsigned short int, bool );

		ConteudoHash( std::string, std::string, unsigned short int, std::string, unsigned short int, std::string, unsigned short int );

		std::string
		getConteudo();

	protected:

	private:
		StructHashVariavel*
		variavel;

		StructHashProcedureFunction*
		procedureFunction;

		StructHashParametrosFormais*
		parametrosFormais;
};

#endif /* CONTEUDOHASH_H_ */
