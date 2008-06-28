#ifndef ANALISADORSINTATICO_H_
#define ANALISADORSINTATICO_H_

#include <iostream>
#include <map>

#include "../../analisador-lexico/includes/AnalisadorLexico.h"
#include "../../analisador-lexico/includes/StructToken.h"

#include "../includes/ArvoreSintatica.h"



class AnalisadorSintatico
{
	public:
		AnalisadorSintatico( std::map<int, StructToken> );

		virtual
		~AnalisadorSintatico( );

	private:
		std::map<int, StructToken>
		saidaAnalisadorLexico;

		ArvoreSintatica
		arvore;

		/*
		 * DECLARACAO DOS NAO TERMINAIS
		 */
		void
		programa( );

		void
		bloco( );

		void
		parteDeclaracoesRotulos( );

		void
		parteDefinicoesTipos( );

		void
		definicaoTipo( );

		void
		tipo( );

		void
		indice( );

		void
		parteDeclaracaoVariaveis( );

		void
		declaracaoVariaveis( );

		void
		listaIdentificadores( );

		void
		parteDeclaracoesSubRotinas( );

		void
		declaracaoProcedimentos( );

		void
		declaracaoFuncao( );

		void
		parametrosFormais( );

		void
		sessaoParametrosFormais( );

		void
		comandoComposto( );

		void
		comando( );

		void
		comandoSemRotulo( );

		void
		atribuicao( );

		void
		chamadaProcedimento( );

		void
		desvios( );

		void
		comandoCondicional( );

		void
		comandoRepetitivo( );

		void
		listaExpressoes( );

		void
		expressao( );

		void
		relacao( );

		void
		expressaoSimples( );

		void
		termo( );

		void
		fator( );

		void
		variavel( );

		void
		chamadaFuncao( );

		void
		numero( );

		void
		digito( );

		void
		identificador( );

		void
		letra( );

		void
		comandoLeitura( );

		void
		comandoEscrita( );
};

#endif /* ANALISADORSINTATICO_H_ */
