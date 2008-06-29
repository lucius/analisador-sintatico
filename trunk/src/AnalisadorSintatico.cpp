#include <iostream>
#include <map>

#include "../../analisador-lexico/includes/AnalisadorLexico.h"
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
	}
	else
	{

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
	if( this->iteradorSaidaAnalisadorLexico->second.token == "label" )
	{
		_bloco->insereFilho( this->parteDeclaracoesRotulos() );
	}

	if( this->iteradorSaidaAnalisadorLexico->second.token == "type" )
	{
		_bloco->insereFilho( this->parteDefinicoesTipos() );
	}

	if( this->iteradorSaidaAnalisadorLexico->second.token == "var" )
	{
		_bloco->insereFilho( this->parteDeclaracoesVariaveis() );
	}

	if( (this->iteradorSaidaAnalisadorLexico->second.token == "procedure") ||
	    (this->iteradorSaidaAnalisadorLexico->second.token == "function") )
	{
		_bloco->insereFilho( this->parteDeclaracoesSubRotinas() );
	}

	/*
	 * PARTE OBRIGATORIA
	 */
	if( this->iteradorSaidaAnalisadorLexico->second.token == "begin" )
	{
		_bloco->insereFilho( this->comandoComposto() );
	}
	else
	{

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

				}
			}

			if( this->iteradorSaidaAnalisadorLexico->second.token == ";" )
			{
				_parteDeclaracoesRotulos->insereFilho( this->iteradorSaidaAnalisadorLexico->second.token, this->nivelLexicoAtual, true );
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

			}
		}
		else
		{

		}
	}
	else
	{

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

}




































