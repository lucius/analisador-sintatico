#include <iostream>
#include <map>
#include <regex.h>

#include "../../analisador-lexico/includes/AnalisadorLexico.h"
#include "../../analisador-lexico/includes/LogErros.h"
#include "../../analisador-lexico/includes/StructToken.h"

#include "../includes/AnalisadorSintatico.h"
#include "../includes/NoArvoreSintatica.h"

AnalisadorSintatico::AnalisadorSintatico( std::map<int, StructToken> _saidaAnalisadorLexico )
{
	this->saidaAnalisadorLexico = _saidaAnalisadorLexico;
	this->iteradorSaidaAnalisadorLexico = this->saidaAnalisadorLexico.begin( );
	this->nivelLexicoAtual = 0;

	this->iniciaAnalise( );
}

AnalisadorSintatico::~AnalisadorSintatico( )
{

}

void
AnalisadorSintatico::iniciaAnalise( )
{
	this->raiz = this->programa( );
}

NoArvoreSintatica*
AnalisadorSintatico::programa( )
{
	NoArvoreSintatica*
	_programa = new NoArvoreSintatica( "<PROGRAMA>", this->nivelLexicoAtual, false );

	if ( this->iteradorSaidaAnalisadorLexico->second.token == "program")
	{
		_programa->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
		++this->iteradorSaidaAnalisadorLexico;

		if( this->iteradorSaidaAnalisadorLexico->second.classificacao == "IDENTIFICADOR" )
		{
			_programa->insereFilho( this->identificador() );

			if ( this->iteradorSaidaAnalisadorLexico->second.token == "(" )
			{
				_programa->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
				++this->iteradorSaidaAnalisadorLexico;

				if( this->iteradorSaidaAnalisadorLexico->second.classificacao == "IDENTIFICADOR" )
				{
					_programa->insereFilho( this->listaIdentificadores() );
				}

				if ( this->iteradorSaidaAnalisadorLexico->second.token == ")" )
				{
					_programa->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
					++this->iteradorSaidaAnalisadorLexico;

					if( this->iteradorSaidaAnalisadorLexico->second.token == ";" )
					{
						_programa->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
						++this->iteradorSaidaAnalisadorLexico;

						_programa->insereFilho( this->bloco() );

						if( this->iteradorSaidaAnalisadorLexico->second.token == "." )
						{
							_programa->insereFilho(  this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
							++this->iteradorSaidaAnalisadorLexico;
						}
						else
						{
							LogErros::getInstancia().insereErro(this->iteradorSaidaAnalisadorLexico->second.linha, "Esperado: '.' apos 'end'");
						}
					}
					else
					{
						LogErros::getInstancia().insereErro(this->iteradorSaidaAnalisadorLexico->second.linha, "Esperado: ';' apos ')'");
					}
				}
				else
				{
					LogErros::getInstancia().insereErro(this->iteradorSaidaAnalisadorLexico->second.linha, "Esperado: ')'");
				}
			}
			else
			{
				LogErros::getInstancia().insereErro(this->iteradorSaidaAnalisadorLexico->second.linha, "Esperado: '(' apos " + (this->iteradorSaidaAnalisadorLexico-1)->second.token );
			}
		}
		else
		{
			LogErros::getInstancia().insereErro(this->iteradorSaidaAnalisadorLexico->second.linha, "Esperado: identificador apos 'program'");
		}
	}
	else
	{
		LogErros::getInstancia().insereErro(this->iteradorSaidaAnalisadorLexico->second.linha, "Esperado: 'program'");
	}

	return _programa;
}

NoArvoreSintatica*
AnalisadorSintatico::bloco( )
{
	NoArvoreSintatica*
	_bloco = new NoArvoreSintatica( "<BLOCO>", this->nivelLexicoAtual, false );

	/*
	 * PARTES NÃO OBRIGATORIAS
	 */
	_bloco->insereFilho( this->parteDeclaracoesRotulos() );
	_bloco->insereFilho( this->parteDefinicoesTipos() );
	_bloco->insereFilho( this->parteDeclaracoesVariaveis() );
	_bloco->insereFilho( this->parteDeclaracoesSubRotinas() );

	/*
	 * PARTE OBRIGATORIA
	 */
	if( this->iteradorSaidaAnalisadorLexico->second.token == "begin" )
	{
		_bloco->insereFilho( this->comandoComposto() );
	}
	else
	{
		LogErros::getInstancia().insereErro(this->iteradorSaidaAnalisadorLexico->second.linha, "Esperado: 'begin' apos ';'");
	}

	return _bloco;
}

NoArvoreSintatica*
AnalisadorSintatico::parteDeclaracoesRotulos( )
{
	NoArvoreSintatica*
	_parteDeclaracoesRotulos = new NoArvoreSintatica( "<PARTE_DECLARACOES_ROTULOS>", this->nivelLexicoAtual, false );

	if( this->iteradorSaidaAnalisadorLexico->second.token == "label" )
	{
		_parteDeclaracoesRotulos->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
		++this->iteradorSaidaAnalisadorLexico;

		if( this->iteradorSaidaAnalisadorLexico->second.classificacao == "NUMERO" )
		{
			_parteDeclaracoesRotulos->insereFilho( this->numero() );

			while( this->iteradorSaidaAnalisadorLexico->second.token == "," )
			{
				_parteDeclaracoesRotulos->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
				++this->iteradorSaidaAnalisadorLexico;

				if( this->iteradorSaidaAnalisadorLexico->second.classificacao == "NUMERO" )
				{
					_parteDeclaracoesRotulos->insereFilho( this->numero() );
				}
				else
				{
					LogErros::getInstancia().insereErro(this->iteradorSaidaAnalisadorLexico->second.linha, "Esperado: numero apos ','");
				}
			}

			if( this->iteradorSaidaAnalisadorLexico->second.token == ";" )
			{
				_parteDeclaracoesRotulos->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
				++this->iteradorSaidaAnalisadorLexico;
			}
			else
			{
				LogErros::getInstancia().insereErro(this->iteradorSaidaAnalisadorLexico->second.linha, "Esperado: ';'");
			}
		}
		else
		{
			LogErros::getInstancia().insereErro(this->iteradorSaidaAnalisadorLexico->second.linha, "Esperado: numero apos 'label'");
		}
	}
	else
	{
		return NULL;
	}

	return _parteDeclaracoesRotulos;
}

NoArvoreSintatica*
AnalisadorSintatico::parteDefinicoesTipos( )
{
	NoArvoreSintatica*
	_parteDefinicoesTipos = new NoArvoreSintatica( "<PARTE_DEFINICOES_TIPOS>", this->nivelLexicoAtual, false );

	if( this->iteradorSaidaAnalisadorLexico->second.token == "type" )
	{
		_parteDefinicoesTipos->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
		++this->iteradorSaidaAnalisadorLexico;

		if( this->iteradorSaidaAnalisadorLexico->second.classificacao == "IDENTIFICADOR" )
		{
			_parteDefinicoesTipos->insereFilho( this->definicaoTipo() );

			while( this->iteradorSaidaAnalisadorLexico->second.token == ";" )
			{
				_parteDefinicoesTipos->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
				++this->iteradorSaidaAnalisadorLexico;

				if( this->iteradorSaidaAnalisadorLexico->second.classificacao == "IDENTIFICADOR" )
				{
					_parteDefinicoesTipos->insereFilho( this->definicaoTipo() );
				}
				else
				{
					LogErros::getInstancia().insereErro(this->iteradorSaidaAnalisadorLexico->second.linha, "Esperado: identificador apos ';'");
				}
			}
		}
		else
		{
			LogErros::getInstancia().insereErro(this->iteradorSaidaAnalisadorLexico->second.linha, "Esperado: identificador apos 'type'");
		}
	}
	else
	{
		return NULL;
	}

	return _parteDefinicoesTipos;
}

NoArvoreSintatica*
AnalisadorSintatico::definicaoTipo()
{
	NoArvoreSintatica*
	_definicaoTipo = new NoArvoreSintatica( "<DEFINICAO_TIPO>", this->nivelLexicoAtual, false );

	if( this->iteradorSaidaAnalisadorLexico->second.classificacao == "IDENTIFICADOR" )
	{
		_definicaoTipo->insereFilho( this->identificador() );

		if( this->iteradorSaidaAnalisadorLexico->second.token == "=" )
		{
			_definicaoTipo->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
			++this->iteradorSaidaAnalisadorLexico;

			if( (this->iteradorSaidaAnalisadorLexico->second.classificacao == "IDENTIFICADOR") ||
				(this->iteradorSaidaAnalisadorLexico->second.token == "array") )
			{
				_definicaoTipo->insereFilho( this->tipo() );
			}
			else
			{
				LogErros::getInstancia().insereErro(this->iteradorSaidaAnalisadorLexico->second.linha, "Esperado: identificador ou 'array' apos '='");
			}
		}
		else
		{
			LogErros::getInstancia().insereErro(this->iteradorSaidaAnalisadorLexico->second.linha, "Esperado: '=' apos " + (this->iteradorSaidaAnalisadorLexico-1)->second.token);

		}
	}
	else
	{
		return NULL;
	}

	return _definicaoTipo;
}

NoArvoreSintatica*
AnalisadorSintatico::tipo( )
{
	NoArvoreSintatica*
	_tipo = new NoArvoreSintatica( "<TIPO>", this->nivelLexicoAtual, false);

	if( this->iteradorSaidaAnalisadorLexico->second.classificacao == "IDENTIFICADOR" )
	{
		_tipo->insereFilho( this->identificador() );
	}
	else if( this->iteradorSaidaAnalisadorLexico->second.token == "array" )
	{
		_tipo->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
		++this->iteradorSaidaAnalisadorLexico;

		if( this->iteradorSaidaAnalisadorLexico->second.token == "[" )
		{
			_tipo->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
			++this->iteradorSaidaAnalisadorLexico;

			_tipo->insereFilho( this->indice() );

			while( this->iteradorSaidaAnalisadorLexico->second.token == "," )
			{
				_tipo->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
				++this->iteradorSaidaAnalisadorLexico;

				_tipo->insereFilho( this->indice() );
			}

			if( this->iteradorSaidaAnalisadorLexico->second.token == "]" )
			{
				_tipo->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
				++this->iteradorSaidaAnalisadorLexico;

				if( this->iteradorSaidaAnalisadorLexico->second.token == "of" )
				{
					_tipo->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
					++this->iteradorSaidaAnalisadorLexico;

					if( (this->iteradorSaidaAnalisadorLexico->second.classificacao == "IDENTIFICADOR") ||
						(this->iteradorSaidaAnalisadorLexico->second.token == "array") )
					{
						_tipo->insereFilho( this->tipo() );
					}
					else
					{
						LogErros::getInstancia().insereErro(this->iteradorSaidaAnalisadorLexico->second.linha, "Esperado: identificador ou 'array' apos 'of'");
					}
				}
				else
				{
					LogErros::getInstancia().insereErro(this->iteradorSaidaAnalisadorLexico->second.linha, "Esperado: 'of' apos ']'");
				}
			}
			else
			{
				LogErros::getInstancia().insereErro(this->iteradorSaidaAnalisadorLexico->second.linha, "Esperado: ']' apos " + (this->iteradorSaidaAnalisadorLexico-1)->second.token);
			}
		}
		else
		{

		}
	}

	return _tipo;
}

NoArvoreSintatica*
AnalisadorSintatico::indice( )
{
	NoArvoreSintatica*
	_indice = new NoArvoreSintatica( "<INDICE>", this->nivelLexicoAtual, false );

	if( this->iteradorSaidaAnalisadorLexico->second.classificacao == "NUMERO" )
	{
		_indice->insereFilho( this->numero() );

		if( this->iteradorSaidaAnalisadorLexico->second.token == "." )
		{
			_indice->insereFilho(this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
			++this->iteradorSaidaAnalisadorLexico;

			if( this->iteradorSaidaAnalisadorLexico->second.token == "." )
			{
				_indice->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
				++this->iteradorSaidaAnalisadorLexico;

				if( this->iteradorSaidaAnalisadorLexico->second.classificacao == "NUMERO" )
				{
					_indice->insereFilho( this->numero() );
				}
			}
			else
			{

			}
		}
		else
		{

		}
	}
	else
	{

	}

	return _indice;
}

NoArvoreSintatica*
AnalisadorSintatico::parteDeclaracoesVariaveis( )
{
	NoArvoreSintatica*
	_parteDeclaracoesVariaveis = new NoArvoreSintatica( "<PARTE_DECLARACOES_VARIAVEIS>", this->nivelLexicoAtual, false );

	if( this->iteradorSaidaAnalisadorLexico->second.token == "var" )
	{
		_parteDeclaracoesVariaveis->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
		++this->iteradorSaidaAnalisadorLexico;

		if( this->iteradorSaidaAnalisadorLexico->second.classificacao == "IDENTIFICADOR" )
		{
			_parteDeclaracoesVariaveis->insereFilho( this->declaracaoVariaveis() );

			while( this->iteradorSaidaAnalisadorLexico->second.token == ";" )
			{
				_parteDeclaracoesVariaveis->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
				++this->iteradorSaidaAnalisadorLexico;

				if( this->iteradorSaidaAnalisadorLexico->second.classificacao == "IDENTIFICADOR" )
				{
					_parteDeclaracoesVariaveis->insereFilho( this->declaracaoVariaveis() );
				}
			}
		}
		else
		{

		}
	}
	else
	{

	}

	return _parteDeclaracoesVariaveis;
}

NoArvoreSintatica*
AnalisadorSintatico::declaracaoVariaveis( )
{
	NoArvoreSintatica*
	_declaracaoVariaveis = new NoArvoreSintatica( "<DECLARACAO_VARIAVEIS>", this->nivelLexicoAtual, false );

	if( this->iteradorSaidaAnalisadorLexico->second.classificacao == "IDENTIFICADOR" )
	{
		_declaracaoVariaveis->insereFilho( this->listaIdentificadores() );

		if( this->iteradorSaidaAnalisadorLexico->second.token == ":" )
		{
			_declaracaoVariaveis->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
			++this->iteradorSaidaAnalisadorLexico;

			if( (this->iteradorSaidaAnalisadorLexico->second.classificacao == "IDENTIFICADOR") ||
				(this->iteradorSaidaAnalisadorLexico->second.token == "array") )
			{
				_declaracaoVariaveis->insereFilho( this->tipo() );
			}
			else
			{

			}
		}
		else
		{

		}
	}
	else
	{

	}

	return _declaracaoVariaveis;
}

NoArvoreSintatica*
AnalisadorSintatico::listaIdentificadores( )
{
	NoArvoreSintatica*
	_listaIdentificadores = new NoArvoreSintatica( "<LISTA_IDENTIFICADORES>", this->nivelLexicoAtual, false );

	if( this->iteradorSaidaAnalisadorLexico->second.classificacao == "IDENTIFICADOR" )
	{
		while( this->iteradorSaidaAnalisadorLexico->second.classificacao == "IDENTIFICADOR" )
		{
			_listaIdentificadores->insereFilho( this->identificador() );

			if( this->iteradorSaidaAnalisadorLexico->second.token == "," )
			{
				_listaIdentificadores->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
				++this->iteradorSaidaAnalisadorLexico;
			}
			else
			{

			}
		}
	}
	else
	{

	}

	return _listaIdentificadores;
}

NoArvoreSintatica*
AnalisadorSintatico::parteDeclaracoesSubRotinas( )
{
	NoArvoreSintatica*
	_parteDeclaracoesSubRotinas = new NoArvoreSintatica( "<PARTE_DECLARACOES_SUB_ROTINAS>", this->nivelLexicoAtual, false );

	while( (this->iteradorSaidaAnalisadorLexico->second.token == "procedure") ||
		   (this->iteradorSaidaAnalisadorLexico->second.token == "function") )
	{
		if( this->iteradorSaidaAnalisadorLexico->second.token == "procedure" )
		{
			_parteDeclaracoesSubRotinas->insereFilho( this->declaracaoProcedimento() );
		}
		else if( this->iteradorSaidaAnalisadorLexico->second.token == "function" )
		{
			_parteDeclaracoesSubRotinas->insereFilho( this->declaracaoFuncao() );
		}
	}

	return _parteDeclaracoesSubRotinas;
}

NoArvoreSintatica*
AnalisadorSintatico::declaracaoProcedimento( )
{
	NoArvoreSintatica*
	_declaracaoProcedimento = new NoArvoreSintatica( "<DECLARACAO_PROCEDIMENTO>", this->nivelLexicoAtual, false );

	if( this->iteradorSaidaAnalisadorLexico->second.token == "procedure" )
	{
		_declaracaoProcedimento->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
		++this->iteradorSaidaAnalisadorLexico;

		if( this->iteradorSaidaAnalisadorLexico->second.classificacao == "IDENTIFICADOR" )
		{
			_declaracaoProcedimento->insereFilho( this->identificador() );

			if( this->iteradorSaidaAnalisadorLexico->second.token == "(" )
			{
				_declaracaoProcedimento->insereFilho( this->parametrosFormais() );
			}

			if( this->iteradorSaidaAnalisadorLexico->second.token == ";" )
			{
				_declaracaoProcedimento->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
				++this->iteradorSaidaAnalisadorLexico;

				_declaracaoProcedimento->insereFilho( this->bloco() );
			}
			else
			{

			}

		}
		else
		{

		}
	}
	else
	{

	}

	return _declaracaoProcedimento;
}

NoArvoreSintatica*
AnalisadorSintatico::declaracaoFuncao( )
{
	NoArvoreSintatica*
	_declaracaoFuncao = new NoArvoreSintatica( "<DECLARACAO_FUNCAO>", this->nivelLexicoAtual, false );

	if( this->iteradorSaidaAnalisadorLexico->second.token == "function" )
	{
		_declaracaoFuncao->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
		++this->iteradorSaidaAnalisadorLexico;

		if( this->iteradorSaidaAnalisadorLexico->second.classificacao == "IDENTIFICADOR" )
		{
			_declaracaoFuncao->insereFilho( this->identificador() );

			if( this->iteradorSaidaAnalisadorLexico->second.token == "(" )
			{
				_declaracaoFuncao->insereFilho( this->parametrosFormais() );
			}

			if( this->iteradorSaidaAnalisadorLexico->second.token == ":" )
			{
				_declaracaoFuncao->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
				++this->iteradorSaidaAnalisadorLexico;

				if( this->iteradorSaidaAnalisadorLexico->second.classificacao == "IDENTIFICADOR" )
				{
					_declaracaoFuncao->insereFilho( this->identificador() );

					if( this->iteradorSaidaAnalisadorLexico->second.token == ";" )
					{
						_declaracaoFuncao->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
						++this->iteradorSaidaAnalisadorLexico;

						_declaracaoFuncao->insereFilho( this->bloco() );
					}
					else
					{

					}
				}
				else
				{

				}
			}
			else
			{

			}
		}
		else
		{

		}
	}
	else
	{

	}

	return _declaracaoFuncao;
}

NoArvoreSintatica*
AnalisadorSintatico::parametrosFormais( )
{
	NoArvoreSintatica*
	_parametrosFormais = new NoArvoreSintatica( "<PARAMETROS_FORMAIS>", this->nivelLexicoAtual, false );

	if( this->iteradorSaidaAnalisadorLexico->second.token == "(" )
	{
		_parametrosFormais->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
		++this->iteradorSaidaAnalisadorLexico;

		if( (this->iteradorSaidaAnalisadorLexico->second.classificacao == "IDENTIFICADOR") ||
			(this->iteradorSaidaAnalisadorLexico->second.token == "var")                   ||
		    (this->iteradorSaidaAnalisadorLexico->second.token == "function")              ||
		    (this->iteradorSaidaAnalisadorLexico->second.token == "procedure") )
		{
			_parametrosFormais->insereFilho( this->secaoParametrosFormais() );
		}

		while( this->iteradorSaidaAnalisadorLexico->second.token == ";" )
		{
			_parametrosFormais->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
			++this->iteradorSaidaAnalisadorLexico;

			if( (this->iteradorSaidaAnalisadorLexico->second.classificacao == "IDENTIFICADOR") ||
				(this->iteradorSaidaAnalisadorLexico->second.token == "var")                   ||
			    (this->iteradorSaidaAnalisadorLexico->second.token == "function")              ||
			    (this->iteradorSaidaAnalisadorLexico->second.token == "procedure") )
			{
				_parametrosFormais->insereFilho( this->secaoParametrosFormais() );
			}
			else
			{

			}
		}

		if( this->iteradorSaidaAnalisadorLexico->second.token == ")" )
		{
			_parametrosFormais->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
			++this->iteradorSaidaAnalisadorLexico;
		}
		else
		{

		}
	}
	else
	{

	}

	return _parametrosFormais;
}

NoArvoreSintatica*
AnalisadorSintatico::secaoParametrosFormais( )
{
	NoArvoreSintatica*
	_secaoParametrosFormais = new NoArvoreSintatica( "<SECAO_PARAMETROS_FORMAIS>", this->nivelLexicoAtual, false );


	if( this->iteradorSaidaAnalisadorLexico->second.classificacao == "IDENTIFICADOR" )
	{
		_secaoParametrosFormais->insereFilho( this->listaIdentificadores() );

		if( this->iteradorSaidaAnalisadorLexico->second.token == ":" )
		{
			_secaoParametrosFormais->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
			++this->iteradorSaidaAnalisadorLexico;

			if( this->iteradorSaidaAnalisadorLexico->second.classificacao == "IDENTIFICADOR" )
			{
				_secaoParametrosFormais->insereFilho( this->identificador() );
			}
			else
			{

			}
		}
		else
		{

		}
	}
	else if( this->iteradorSaidaAnalisadorLexico->second.token == "var" )
	{
		_secaoParametrosFormais->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
		++this->iteradorSaidaAnalisadorLexico;

		if( this->iteradorSaidaAnalisadorLexico->second.classificacao == "IDENTIFICADOR" )
		{
			_secaoParametrosFormais->insereFilho( this->listaIdentificadores() );

			if( this->iteradorSaidaAnalisadorLexico->second.token == ":" )
			{
				_secaoParametrosFormais->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
				++this->iteradorSaidaAnalisadorLexico;

				if( this->iteradorSaidaAnalisadorLexico->second.classificacao == "IDENTIFICADOR" )
				{
					_secaoParametrosFormais->insereFilho( this->identificador() );
				}
				else
				{

				}
			}
			else
			{

			}
		}
		else
		{

		}
	}
	else if( this->iteradorSaidaAnalisadorLexico->second.token == "function" )
	{
		_secaoParametrosFormais->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
		++this->iteradorSaidaAnalisadorLexico;

		if( this->iteradorSaidaAnalisadorLexico->second.classificacao == "IDENTIFICADOR" )
		{
			_secaoParametrosFormais->insereFilho( this->listaIdentificadores() );

			if( this->iteradorSaidaAnalisadorLexico->second.token == ":" )
			{
				_secaoParametrosFormais->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
				++this->iteradorSaidaAnalisadorLexico;

				if( this->iteradorSaidaAnalisadorLexico->second.classificacao == "IDENTIFICADOR" )
				{
					_secaoParametrosFormais->insereFilho( this->identificador() );
				}
				else
				{

				}
			}
			else
			{

			}
		}
		else
		{

		}
	}
	else if( this->iteradorSaidaAnalisadorLexico->second.token == "procedure" )
	{
		_secaoParametrosFormais->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
		++this->iteradorSaidaAnalisadorLexico;

		if( this->iteradorSaidaAnalisadorLexico->second.classificacao == "IDENTIFICADOR" )
		{
			_secaoParametrosFormais->insereFilho( this->listaIdentificadores() );
		}
		else
		{

		}
	}

	return _secaoParametrosFormais;
}

NoArvoreSintatica*
AnalisadorSintatico::comandoComposto( )
{
	NoArvoreSintatica*
	_comandoComposto = new NoArvoreSintatica( "<COMANDO_COMPOSTO>", this->nivelLexicoAtual, false );

	if( this->iteradorSaidaAnalisadorLexico->second.token == "begin" )
	{
		_comandoComposto->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
		++this->iteradorSaidaAnalisadorLexico;

		_comandoComposto->insereFilho( this->comando() );

		while( this->iteradorSaidaAnalisadorLexico->second.token == ";" )
		{
			_comandoComposto->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
			++this->iteradorSaidaAnalisadorLexico;

			_comandoComposto->insereFilho( this->comando() );
		}

		if( this->iteradorSaidaAnalisadorLexico->second.token == "end" )
		{
			_comandoComposto->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
			++this->iteradorSaidaAnalisadorLexico;
		}
		else
		{

		}
	}
	else
	{

	}

	return _comandoComposto;
}

NoArvoreSintatica*
AnalisadorSintatico::comando( )
{
	NoArvoreSintatica*
	_comando = new NoArvoreSintatica( "<COMANDO>", this->nivelLexicoAtual, false );

	if( this->iteradorSaidaAnalisadorLexico->second.classificacao == "NUMERO" )
	{
		_comando->insereFilho( this->numero() );

		if( this->iteradorSaidaAnalisadorLexico->second.token == ":" )
		{
			_comando->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
			++this->iteradorSaidaAnalisadorLexico;
		}
		else
		{

		}
	}

	_comando->insereFilho( this->comandoSemRotulo() );

	return _comando;
}


/*
 * FALTA ESTE
 */

NoArvoreSintatica*
AnalisadorSintatico::comandoSemRotulo( )
{
	NoArvoreSintatica*
	_comandoSemRotulo = new NoArvoreSintatica( "<COMANDO_SEM_ROTULO>", this->nivelLexicoAtual, false );

	return _comandoSemRotulo;
}

NoArvoreSintatica*
AnalisadorSintatico::atribuicao(  )
{
	NoArvoreSintatica*
	_atribuicao = new NoArvoreSintatica( "<ATRIBUICAO>", this->nivelLexicoAtual, false );

	if( this->iteradorSaidaAnalisadorLexico->second.classificacao == "IDENTIFICADOR" )
	{
		_atribuicao->insereFilho( this->atribuicao() );

		if( this->iteradorSaidaAnalisadorLexico->second.token == ":=" )
		{
			_atribuicao->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
			++this->iteradorSaidaAnalisadorLexico;

			_atribuicao->insereFilho( this->expressao() );
		}
		else
		{

		}
	}
	else
	{

	}

	return _atribuicao;
}

NoArvoreSintatica*
AnalisadorSintatico::chamadaProcedimento( )
{
	NoArvoreSintatica*
	_chamadaProcedimento = new NoArvoreSintatica( "<CHAMADA_PROCEDIMENTO>", this->nivelLexicoAtual, false );

	if( this->iteradorSaidaAnalisadorLexico->second.classificacao == "IDENTIFICADOR" )
	{
		_chamadaProcedimento->insereFilho( this->identificador() );

		if( this->iteradorSaidaAnalisadorLexico->second.token == "(")
		{
			_chamadaProcedimento->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
			++this->iteradorSaidaAnalisadorLexico;

			_chamadaProcedimento->insereFilho( this->listaExpressoes() );

			if( this->iteradorSaidaAnalisadorLexico->second.token == ")" )
			{
				_chamadaProcedimento->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
				++this->iteradorSaidaAnalisadorLexico;
			}
			else
			{

			}
		}
		else
		{

		}
	}
	else
	{

	}

	return _chamadaProcedimento;
}

NoArvoreSintatica*
AnalisadorSintatico::desvios( )
{
	NoArvoreSintatica*
	_desvios = new NoArvoreSintatica( "<DESVIOS>", this->nivelLexicoAtual, false );

	if( this->iteradorSaidaAnalisadorLexico->second.token == "goto" )
	{
		_desvios->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
		++this->iteradorSaidaAnalisadorLexico;

		_desvios->insereFilho( this->numero() );
	}
	else
	{

	}

	return _desvios;
}

NoArvoreSintatica*
AnalisadorSintatico::comandoCondicional( )
{
	NoArvoreSintatica*
	_comandoCondicional = new NoArvoreSintatica( "<COMANDO_CONDICIONAL>", this->nivelLexicoAtual, false );

	if( this->iteradorSaidaAnalisadorLexico->second.token == "if" )
	{
		_comandoCondicional->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
		++this->iteradorSaidaAnalisadorLexico;

		_comandoCondicional->insereFilho( this->expressao() );

		if( this->iteradorSaidaAnalisadorLexico->second.token == "then" )
		{
			_comandoCondicional->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
			++this->iteradorSaidaAnalisadorLexico;

			_comandoCondicional->insereFilho( this->comandoSemRotulo() );

			if( this->iteradorSaidaAnalisadorLexico->second.token == "else" )
			{
				_comandoCondicional->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
				++this->iteradorSaidaAnalisadorLexico;

				_comandoCondicional->insereFilho( this->comandoSemRotulo() );
			}
			else
			{

			}
		}
		else
		{

		}
	}
	else
	{

	}

	return _comandoCondicional;
}

NoArvoreSintatica*
AnalisadorSintatico::comandoRepetitivo( )
{
	NoArvoreSintatica*
	_comandoRepetitivo = new NoArvoreSintatica( "<COMANDO_REPETITIVO>", this->nivelLexicoAtual, false );

	if( this->iteradorSaidaAnalisadorLexico->second.token == "while" )
	{
		_comandoRepetitivo->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
		++this->iteradorSaidaAnalisadorLexico;

		_comandoRepetitivo->insereFilho( this->expressao() );

		if( this->iteradorSaidaAnalisadorLexico->second.token == "do" )
		{
			_comandoRepetitivo->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
			++this->iteradorSaidaAnalisadorLexico;

			_comandoRepetitivo->insereFilho( comandoSemRotulo() );
		}
		else
		{

		}
	}
	else
	{

	}

	return _comandoRepetitivo;
}

NoArvoreSintatica*
AnalisadorSintatico::listaExpressoes( )
{
	NoArvoreSintatica*
	_listaExpressoes = new NoArvoreSintatica( "<LISTA_EXPRESSOES>", this->nivelLexicoAtual, false );

	_listaExpressoes->insereFilho( this->expressao() );
	while( this->iteradorSaidaAnalisadorLexico->second.token == "," )
	{
		_listaExpressoes->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
		++this->iteradorSaidaAnalisadorLexico;

		_listaExpressoes->insereFilho( this->expressao() );
	}

	return _listaExpressoes;
}

NoArvoreSintatica*
AnalisadorSintatico::expressao( )
{
	NoArvoreSintatica*
	_expressao = new NoArvoreSintatica( "<EXPRESSAO>", this->nivelLexicoAtual, false );

	_expressao->insereFilho( this->expressaoSimples() );

	if( (this->iteradorSaidaAnalisadorLexico->second.token == "=" ) ||
		(this->iteradorSaidaAnalisadorLexico->second.token == "<>") ||
		(this->iteradorSaidaAnalisadorLexico->second.token == "<" ) ||
		(this->iteradorSaidaAnalisadorLexico->second.token == "<=") ||
		(this->iteradorSaidaAnalisadorLexico->second.token == ">=") ||
		(this->iteradorSaidaAnalisadorLexico->second.token == ">" ) )
	{
		_expressao->insereFilho( this->relacao() );
		_expressao->insereFilho( this->expressaoSimples() );
	}

	return _expressao;
}

NoArvoreSintatica*
AnalisadorSintatico::relacao( )
{
	NoArvoreSintatica*
	_relacao = new NoArvoreSintatica( "<RELACAO>", this->nivelLexicoAtual, false );

	if( (this->iteradorSaidaAnalisadorLexico->second.token == "=" ) ||
		(this->iteradorSaidaAnalisadorLexico->second.token == "<>") ||
		(this->iteradorSaidaAnalisadorLexico->second.token == "<" ) ||
		(this->iteradorSaidaAnalisadorLexico->second.token == "<=") ||
		(this->iteradorSaidaAnalisadorLexico->second.token == ">=") ||
		(this->iteradorSaidaAnalisadorLexico->second.token == ">" ) )
	{
		_relacao->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
		++this->iteradorSaidaAnalisadorLexico;
	}

	return _relacao;
}

NoArvoreSintatica*
AnalisadorSintatico::expressaoSimples( )
{
	NoArvoreSintatica*
	_expressaoSimples = new NoArvoreSintatica( "<EXPRESSAO_SIMPLES>", this->nivelLexicoAtual, false );

	if( (this->iteradorSaidaAnalisadorLexico->second.token == "+") ||
		(this->iteradorSaidaAnalisadorLexico->second.token == "-") )
	{
		_expressaoSimples->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
		++this->iteradorSaidaAnalisadorLexico;
	}

	_expressaoSimples->insereFilho( this->termo() );

	while( (this->iteradorSaidaAnalisadorLexico->second.token == "+") ||
		   (this->iteradorSaidaAnalisadorLexico->second.token == "-") ||
		   (this->iteradorSaidaAnalisadorLexico->second.token == "or") )
	{
		_expressaoSimples->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
		++this->iteradorSaidaAnalisadorLexico;

		_expressaoSimples->insereFilho( this->termo() );
	}

	return _expressaoSimples;
}

NoArvoreSintatica*
AnalisadorSintatico::termo( )
{
	NoArvoreSintatica*
	_termo = new NoArvoreSintatica( "<TERMO>", this->nivelLexicoAtual, false );

	_termo->insereFilho( this->fator() );

	while( (this->iteradorSaidaAnalisadorLexico->second.token == "*")   ||
		   (this->iteradorSaidaAnalisadorLexico->second.token == "div") ||
		   (this->iteradorSaidaAnalisadorLexico->second.token == "and") )
	{
		_termo->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
		++this->iteradorSaidaAnalisadorLexico;

		_termo->insereFilho( this->fator() );
	}

	return _termo;
}

/*
 * FALTA ESTE
 */

NoArvoreSintatica*
AnalisadorSintatico::fator( )
{
	NoArvoreSintatica*
	_fator = new NoArvoreSintatica( "<FATOR>", this->nivelLexicoAtual, false );

	return _fator;
}

NoArvoreSintatica*
AnalisadorSintatico::variavel( )
{
	NoArvoreSintatica*
	_variavel = new NoArvoreSintatica( "<VARIAVEL>", this->nivelLexicoAtual, false );

	_variavel->insereFilho( this->identificador() );

	return _variavel;
}

NoArvoreSintatica*
AnalisadorSintatico::chamadaFuncao( )
{
	NoArvoreSintatica*
	_chamadaFuncao = new NoArvoreSintatica( "<CHAMADA_FUNCAO>", this->nivelLexicoAtual, false );

	_chamadaFuncao->insereFilho( this->identificador() );

	if( this->iteradorSaidaAnalisadorLexico->second.token == "(" )
	{
		_chamadaFuncao->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
		++this->iteradorSaidaAnalisadorLexico;

		_chamadaFuncao->insereFilho( this->listaExpressoes() );
	}

	return _chamadaFuncao;
}

NoArvoreSintatica*
AnalisadorSintatico::numero( )
{
	NoArvoreSintatica*
	_numero = new NoArvoreSintatica( "<NUMERO>", this->nivelLexicoAtual, false );

	size_t
	posicaoCorte;

	if( this->iteradorSaidaAnalisadorLexico->second.classificacao == "NUMERO" )
	{
		while ( this->iteradorSaidaAnalisadorLexico->second.token.size() != posicaoCorte )
		{
			_numero->insereFilho( this->digito(this->iteradorSaidaAnalisadorLexico->second.token.substr(posicaoCorte, 1)) );
			++posicaoCorte;
		}
		++this->iteradorSaidaAnalisadorLexico;
	}

	return _numero;
}

NoArvoreSintatica*
AnalisadorSintatico::digito( std::string _digitoInsercao )
{
	NoArvoreSintatica*
	_digito = new NoArvoreSintatica( "<DIGITO>", this->nivelLexicoAtual, false );

	_digito->insereFilho( _digitoInsercao, this->nivelLexicoAtual, true );

	return _digito;
}

NoArvoreSintatica*
AnalisadorSintatico::identificador( )
{
	NoArvoreSintatica*
	_identificador = new NoArvoreSintatica( "<IDENTIFICADOR>", this->nivelLexicoAtual, false );

	regex_t
	expressaoRegularDigito;

	regex_t
	expressaoRegularLetra;

	size_t
	posicaoCorte = 0;

	if( this->iteradorSaidaAnalisadorLexico->second.classificacao == "IDENTIFICADOR" )
	{
		if ( !regcomp(&expressaoRegularDigito, "[^0-9]", REG_EXTENDED|REG_ICASE|REG_NOSUB) )
		{
		}
		if ( !regcomp(&expressaoRegularLetra, "[^A-Z]", REG_EXTENDED|REG_ICASE|REG_NOSUB) )
		{
		}

		while ( this->iteradorSaidaAnalisadorLexico->second.token.size() != posicaoCorte )
		{
			if( !regexec(&expressaoRegularDigito, this->iteradorSaidaAnalisadorLexico->second.token.c_str(), 0, (regmatch_t *)NULL, 0) )
			{
				_identificador->insereFilho( this->digito(this->iteradorSaidaAnalisadorLexico->second.token.substr(posicaoCorte, 1)) );
			}
			else if ( !regexec(&expressaoRegularLetra, this->iteradorSaidaAnalisadorLexico->second.token.c_str(), 0, (regmatch_t *)NULL, 0) )
			{
				_identificador->insereFilho( this->letra(this->iteradorSaidaAnalisadorLexico->second.token.substr(posicaoCorte, 1)) );
			}

			++posicaoCorte;
		}
		++this->iteradorSaidaAnalisadorLexico;
	}

	regfree( &expressaoRegularDigito );
	regfree( &expressaoRegularLetra );

	return _identificador;
}

NoArvoreSintatica*
AnalisadorSintatico::letra( std::string _letraInsercao )
{
	NoArvoreSintatica*
	_letra = new NoArvoreSintatica( "<LETRA>", this->nivelLexicoAtual, false );

	_letra->insereFilho( _letraInsercao, this->nivelLexicoAtual, true );

	return _letra;
}

NoArvoreSintatica*
AnalisadorSintatico::comandoLeitura( )
{

}

NoArvoreSintatica*
AnalisadorSintatico::comandoEscrita( )
{

}






























































































