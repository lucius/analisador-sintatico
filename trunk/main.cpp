#include <map>

#include "../analisador-lexico/includes/StructToken.h"

#include "./includes/LogErros.h"
#include "./includes/AnalisadorSintatico.h"



std::map<int, StructToken>
saidaAnalisadorLexico;

void
montaSaida( char* _token, char* _identificador )
{
	static unsigned short int
	i = 0;

	StructToken
	_tmp;

	_tmp.token = _token;
	_tmp.classificacao = _identificador;
	saidaAnalisadorLexico[i++] = _tmp;

}

int
main( )
{
	AnalisadorSintatico*
	teste;

//	std::map<int, StructToken>
//	saidaAnalisadorLexico;
//
//	StructToken
//	_tmp;
//

	montaSaida( "program", "PALAVRA_CHAVE"      );
	montaSaida( "gcd",     "IDENTIFICADOR"      );
	montaSaida( "(",       "SIMBOLOS_ESPECIAIS" );
	montaSaida( "input",   "IDENTIFICADOR"      );
	montaSaida( ",",       "PONTUACAO"          );
	montaSaida( "output",  "IDENTIFICADOR"      );
	montaSaida( ")",       "SIMBOLOS_ESPECIAIS" );
	montaSaida( ";",       "PONTUACAO"          );
	montaSaida( "var",     "PALAVRA_CHAVE"      );
	montaSaida( "a", "IDENTIFICADOR" );
	montaSaida( ",",       "PONTUACAO"          );
	montaSaida( "bd3", "IDENTIFICADOR" );
	montaSaida( ":", "PONTUACAO" );
	montaSaida( "integer", "IDENTIFICADOR" );
	montaSaida( ";", "PONTUACAO" );
	montaSaida( "begin", "PALAVRA_CHAVE" );
/*	montaSaida( "while", "PALAVRA_CHAVE" );
	montaSaida( "i", "IDENTIFICADOR" );
	montaSaida( "<>", "PALAVRA_CHAVE" );
	montaSaida( "j", "IDENTIFICADOR" );
	montaSaida( "do", "PALAVRA_CHAVE" );*/
	montaSaida( "end", "PALAVRA_CHAVE" );
	montaSaida( ".", "PONTUACAO" );

	teste = new AnalisadorSintatico( saidaAnalisadorLexico );

	LogErros::getInstancia().getLog();
	return 0;
}

