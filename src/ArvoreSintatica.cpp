#include <iostream>
#include <map>
#include <string>

#include "../../analisador-lexico/includes/AnalisadorLexico.h"
#include "../../analisador-lexico/includes/StructToken.h"

#include "../includes/ArvoreSintatica.h"



ArvoreSintatica::ArvoreSintatica( )
{

}

ArvoreSintatica::~ArvoreSintatica( )
{

}

void
ArvoreSintatica::insereNo( const std::string _descricao, const int _nivelLexico, const bool _isTerminal = false )
{
	this->noCorrente->insereFilho( _descricao, _nivelLexico, _isTerminal );
}
