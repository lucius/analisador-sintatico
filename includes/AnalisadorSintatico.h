#ifndef ANALISADORSINTATICO_H_
#define ANALISADORSINTATICO_H_

#include <iostream>
#include <map>

#include "../../analisador-lexico/includes/AnalisadorLexico.h"
#include "../../analisador-lexico/includes/StructToken.h"

#include "../includes/NoArvoreSintatica.h"



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

		unsigned int
		nivelLexicoAtual;

		void
		iniciaAnalise( );

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
		sessaoParametrosFormais( );

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
		digito( );

		NoArvoreSintatica*
		identificador( );

		NoArvoreSintatica*
		letra( );

		NoArvoreSintatica*
		comandoLeitura( );

		NoArvoreSintatica*
		comandoEscrita( );
};

#endif /* ANALISADORSINTATICO_H_ */
