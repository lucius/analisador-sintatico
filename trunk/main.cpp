#include <map>

#include "./analisador-lexico/includes/StructToken.h"
#include "./analisador-lexico/includes/AnalisadorLexico.h"
#include "./analisador-lexico/includes/ErrosExecucao.h"
#include "./analisador-lexico/includes/LogErros.h"

#include "./includes/AnalisadorSintatico.h"


int
main(int argc, char* argv[])
{
	/*
	 * Instancia o Analisador Lexico
	 */
	AnalisadorLexico*
	lexico;

	AnalisadorSintatico*
	sintatico;

	try
	{
		/*
		 * Verifica se foi especificado o nome do arquivo-fonte
		 */
		if ( argv[1] == NULL ) throw ( new ErrosExecucao("Voce Precisa Definir qual arquivo sera compilado") );
		/*
		 * Verifica se foi especificado o arquivo no qual o log do Analisador Lexico será salvo
		 */
		else if ( argv[2] )
		{
			lexico = new AnalisadorLexico( argv[1], argv[2] );
		}
		/*
		 * Se nao foi especificado um nome para o log, o resultado será exibido na saida padrao do sistema
		 */
		else
		{
			lexico = new AnalisadorLexico( argv[1] );
		}

		sintatico = new AnalisadorSintatico( lexico->getMapAnalisadorLexico() );
	}
	/*
	 * Se ocorrer algum problema durante a execucao do Analisador Lexico, o erro sera exibido
	 */
	catch ( ErrosExecucao* erro )
	{
		std::cout << erro->getErro( );
	}

	/*
	 * Exibe o log de Erros do Analisador
	 */
	LogErros::getInstancia( ).getLog( );

	return (EXIT_SUCCESS);
}
