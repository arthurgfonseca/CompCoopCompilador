<?php

$arquivoEntrada = @fopen("estados.txt", "r");
$matrizDeTransicao;

//submaquina programa
function PeEstadoFinal($estado) {
	if ($estado == 9)
		return true;
		
	return false;
}

//submaquina comando
function eEstadoFinal($estado) {
	if ($estado == 12)
		return true;
		
	return false;
}



//submaquina expressao

function EeEstadoFinal($estado) {
	if ($estado == 1 ||
	$estado ==  4 ||
	$estado ==  9 ||
	$estado ==  12 ||
	$estado ==  20 ||
	$estado ==  23)
		return true;
		
	return false;
}

//necessario fazer para todos os terminais exceto caracteres que o token é o proprio codigo ascii
function traduzirEntradaLidaNoWirthParaEntradaNoCodigoC($entradaLida) {
	$retorno = "";
	switch ($entradaLida) {
		case '||':
			$retorno = "entradaLida == OPERADOR_OU";
			break;
		case 'else':
			$retorno = "entradaLida == PALAVRARESERVADA_else";
			break;
		case 'numero_fracionario':
			$retorno = "entradaLida == PALAVRARESERVADA_float";
			break;
		case 'if':
			$retorno = "entradaLida == PALAVRARESERVADA_if";
			break;
		case 'endif':
			$retorno = "entradaLida == PALAVRARESERVADA_endif";
			break;
		case 'numero_inteiro':
			$retorno = "entradaLida == INTEIRO";
			break;
		case 'return':
			$retorno = "entradaLida == PALAVRARESERVADA_return";
			break;
		case 'void':
			$retorno = "entradaLida == PALAVRARESERVADA_void";
			break;
		case 'while':
			$retorno = "entradaLida == PALAVRARESERVADA_while";
			break;
		case 'endwhile':
			$retorno = "entradaLida == PALAVRARESERVADA_endwhile";
			break;
		case 'program':
			$retorno = "entradaLida == PALAVRARESERVADA_program";
			break;
		case 'execute':
			$retorno = "entradaLida == PALAVRARESERVADA_execute";
			break;
		case 'endprogram':
			$retorno = "entradaLida == PALAVRARESERVADA_endprogram";
			break;
		case 'enddeclare':
			$retorno = "entradaLida == PALAVRARESERVADA_enddeclare";
			break;
		case 'scan':
			$retorno = "entradaLida == PALAVRARESERVADA_scan";
			break;
		case 'print':
			$retorno = "entradaLida == PALAVRARESERVADA_print";
			break;
		case 'declare':
			$retorno = "entradaLida == PALAVRARESERVADA_declare";
			break;
		case 'tipo':
			$retorno = "(entradaLida == PALAVRARESERVADA_int || entradaLida == PALAVRARESERVADA_float)";
			break;
		case 'identificador':
			$retorno = "entradaLida == SIMBOLO";
			break;
		case 'operadores_matematicos':
			$retorno = "(entradaLida == '+' || entradaLida == '-' || entradaLida == '*' || entradaLida == '/')";
			break;
		case 'operadores_de_comparacao':
			$retorno = "entradaLida == OPERADOR";
			break;
		case '&&':
			$retorno = "entradaLida == '&'";
			break;
		default:
			$retorno = "entradaLida == '".$entradaLida."'";
			break;
	}
	return $retorno;
}
if ($arquivoEntrada) {
    while (($buffer = fgets($arquivoEntrada, 4096)) !== false) {
        $estadoDePartida = explode(", ", substr($buffer, 1));
		$transicao = explode(") -> ", $estadoDePartida[1]);
		$estadoDeChegada = $transicao[1];
		$estadoDePartida = $estadoDePartida[0];
		$transicao = str_replace('"', "", $transicao[0]);
		$matrizDeTransicao[$estadoDePartida][$transicao] = str_replace("\n", "", $estadoDeChegada);
    }
    fclose($arquivoEntrada);
}

$primeiroIfGeral = true;
echo "int naoEncontrouTransicao;\n";
echo "naoEncontrouTransicao = TRUE;\n";

echo "*algumaSubmaquinaTransitou = TRUE;\n*estaNoEstadoFinal = FALSE;\n\n";
foreach($matrizDeTransicao as $estadoDePartida => $transicoes) {
	if ($primeiroIfGeral) {
		echo "if (submaquinaAtual->estadoAtual == {$estadoDePartida}) {\n";
		$primeiroIfGeral = false;
	}
	else 
		echo "else if (submaquinaAtual->estadoAtual == {$estadoDePartida}) {\n";
	
	$primeiroIfTransicao = true;
	$submaquina = false;
	
	//verifica se existe submaquina
	if (isset($transicoes["programa"])) {
		$submaquina["nomeFuncao"] = "submaquinaProgramaCriarSubmaquina";
		$submaquina["proximoEstado"] = $transicoes["programa"];
		unset($transicoes["programa"]);
	}
	
	if (isset($transicoes["comandos"])) {
		$submaquina["nomeFuncao"] = "submaquinaComandosCriarSubmaquina";
		$submaquina["proximoEstado"] = $transicoes["comandos"];
		unset($transicoes["comandos"]);
	}
	
	if (isset($transicoes["expressao"])) {
		$submaquina["nomeFuncao"] = "submaquinaExpressoesCriarSubmaquina";
		$submaquina["proximoEstado"] = $transicoes["expressao"];
		unset($transicoes["expressao"]);
	}
	
	foreach($transicoes as $entradaLida => $estadoDeChegada) {

		if ($primeiroIfTransicao) {
			echo "\tif (".traduzirEntradaLidaNoWirthParaEntradaNoCodigoC($entradaLida).") {\n\t\tsubmaquinaAtual->estadoAtual = {$estadoDeChegada};\n";
			$primeiroIfTransicao = false;
		} else {
			echo "\telse if (".traduzirEntradaLidaNoWirthParaEntradaNoCodigoC($entradaLida).") {\n\t\tsubmaquinaAtual->estadoAtual = {$estadoDeChegada};\n";
		}
		
		if (eEstadoFinal($estadoDeChegada))
			echo "\t\t*estaNoEstadoFinal = TRUE;\n";
		
		echo "\t\tnaoEncontrouTransicao = FALSE;\n";
		echo "\t}\n";
	}
	
	if ($submaquina != false) {
		if ($primeiroIfTransicao)
		{
			echo "\tsubmaquinaAtual->estadoAtual = {$submaquina['proximoEstado']};\n";
			echo "\tsubstituirSubmaquinaAtualColocandoAAntigaNaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, acaoSemantica, {$submaquina['nomeFuncao']}());\n";	
			if (eEstadoFinal($submaquina['proximoEstado']))
				echo "\t*estaNoEstadoFinal = TRUE;\n";
			echo "\tnaoEncontrouTransicao = FALSE;\n";
		}
		else {
			echo "\telse {\n\t\tsubmaquinaAtual->estadoAtual = {$submaquina['proximoEstado']};\n";
			echo "\t\tsubstituirSubmaquinaAtualColocandoAAntigaNaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, acaoSemantica, {$submaquina['nomeFuncao']}());\n";
			if (eEstadoFinal($submaquina['proximoEstado']))
				echo "\t\t*estaNoEstadoFinal = TRUE;\n";
			echo "\t\tnaoEncontrouTransicao = FALSE;\n";
			echo "\t}\n";
		}
	}
	
	echo "}\n";
}

echo "\n\nif(naoEncontrouTransicao == TRUE) {\n\tchamarSubmaquinaDaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal);\n}\n";
echo "return;\n";

?>