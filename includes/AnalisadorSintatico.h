#ifndef ANALISADORSINTATICO_H_
#define ANALISADORSINTATICO_H_

#include <iostream>
#include <map>
#include <stack>

#include "../../analisador-lexico/includes/AnalisadorLexico.h"
#include "../../analisador-lexico/includes/StructToken.h"

#include "../includes/NoArvoreSintatica.h"
#include "../includes/TabelaHash.h"



class AnalisadorSintatico
{
	public:
		AnalisadorSintatico( std::map<int, StructToken> );

		virtual
		~AnalisadorSintatico( );

	private:
		std::map<int, StructToken>
		saidaAnalisadorLexico;

		std::map<int, StructToken>::iterator
		iteradorSaidaAnalisadorLexico;

		NoArvoreSintatica*
		raiz;

		TabelaHash
		hash;

		unsigned int
		nivelLexicoAtual;

		unsigned int
		deslocamentoAtual;

		std::stack<NoArvoreSintatica*>
		pilhaDeslocamento;

		void
		iniciaAnalise( );

		void
		imprimeArvore( NoArvoreSintatica*, unsigned short int );

		/*
		 * DECLARACAO DOS NAO TERMINAIS
		 */
		NoArvoreSintatica*
		programa( );

		NoArvoreSintatica*
		bloco( );

		NoArvoreSintatica*
		parteDeclaracoesRotulos( );

		NoArvoreSintatica*
		parteDefinicoesTipos( );

		NoArvoreSintatica*
		definicaoTipo( );

		NoArvoreSintatica*
		tipo( );

		NoArvoreSintatica*
		indice( );

		NoArvoreSintatica*
		parteDeclaracoesVariaveis( );

		NoArvoreSintatica*
		declaracaoVariaveis( );

		NoArvoreSintatica*
		listaIdentificadores( );

		NoArvoreSintatica*
		parteDeclaracoesSubRotinas( );

		NoArvoreSintatica*
		declaracaoProcedimento( );

		NoArvoreSintatica*
		declaracaoFuncao( );

		NoArvoreSintatica*
		parametrosFormais( );

		NoArvoreSintatica*
		secaoParametrosFormais( );

		NoArvoreSintatica*
		comandoComposto( );

		NoArvoreSintatica*
		comando( );

		NoArvoreSintatica*
		comandoSemRotulo( );

		NoArvoreSintatica*
		atribuicao( );

		NoArvoreSintatica*
		chamadaProcedimento( );

		NoArvoreSintatica*
		desvios( );

		NoArvoreSintatica*
		comandoCondicional( );

		NoArvoreSintatica*
		comandoRepetitivo( );

		NoArvoreSintatica*
		listaExpressoes( );

		NoArvoreSintatica*
		expressao( );

		NoArvoreSintatica*
		relacao( );

		NoArvoreSintatica*
		expressaoSimples( );

		NoArvoreSintatica*
		termo( );

		NoArvoreSintatica*
		fator( );

		NoArvoreSintatica*
		variavel( );

		NoArvoreSintatica*
		chamadaFuncao( );

		NoArvoreSintatica*
		numero( );

		NoArvoreSintatica*
		digito( std::string );

		NoArvoreSintatica*
		identificador( );

		NoArvoreSintatica*
		letra( std::string );

		NoArvoreSintatica*
		comandoLeitura( );

		NoArvoreSintatica*
		comandoEscrita( );
};

#endif /* ANALISADORSINTATICO_H_ */
