#ifndef ARVORESINTATICA_H_
#define ARVORESINTATICA_H_

#include <iostream>
#include <map>
#include <string>

#include "../includes/NoArvoreSintatica.h"



class ArvoreSintatica
{
	public:
		ArvoreSintatica( );

		virtual
		~ArvoreSintatica( );

		void
		insereNo( const std::string, const int, const bool );

	protected:

	private:
		NoArvoreSintatica*
		programa;

		NoArvoreSintatica*
		noCorrente;
};

#endif /* ARVORESINTATICA_H_ */
