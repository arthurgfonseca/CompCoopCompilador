<?php

echo "========== ========== ==========\n";
echo "========== ========== ==========\n";

$arquivoEntrada = @fopen("estados.txt", "r");
$matrizDeTransicao;

function eEstadoFinal($estado) {
	if ($estado == 3)
		return true;
		
	return false;
}

function traduzirEntradaLidaNoWirthParaEntradaNoCodigoC($entradaLida) {
	switch ($entradaLida) {
		case '||':
			return "OPERADOR_OU";
		case 'else':
			return "PALAVRARESERVADA_else";
		case 'numero_fracionario':
			return "PALAVRARESERVADA_float";
		case 'if':
			return "PALAVRARESERVADA_if";
		case 'endif':
			return "PALAVRARESERVADA_endif";
		case 'numero_inteiro':
			return "PALAVRARESERVADA_int";
		case 'return':
			return "PALAVRARESERVADA_return";
		case 'void':
			return "PALAVRARESERVADA_void";
		case 'while':
			return "PALAVRARESERVADA_while";
		case 'endwhile':
			return "PALAVRARESERVADA_endwhile";
		case 'program':
			return "PALAVRARESERVADA_program";
		case 'execute':
			return "PALAVRARESERVADA_execute";
		case 'endprogram':
			return "PALAVRARESERVADA_endprogram";
		case 'enddeclare':
			return "PALAVRARESERVADA_enddeclare";
		case 'scan':
			return "PALAVRARESERVADA_scan";
		case 'print':
			return "PALAVRARESERVADA_print";
		case 'declare':
			return "PALAVRARESERVADA_declare";
		default:
			return "'".$entradaLida."'";
	}
}
if ($arquivoEntrada) {
    while (($buffer = fgets($arquivoEntrada, 4096)) !== false) {
        $estadoDePartida = explode(", ", substr($buffer, 1));
		$transicao = explode(") -> ", $estadoDePartida[1]);
		$estadoDeChegada = $transicao[1];
		$estadoDePartida = $estadoDePartida[0];
		$transicao = traduzirEntradaLidaNoWirthParaEntradaNoCodigoC(str_replace('"', "", $transicao[0]));
		$matrizDeTransicao[$estadoDePartida][$transicao] = str_replace("\n", "", $estadoDeChegada);
    }
    fclose($arquivoEntrada);
}

$primeiroIfGeral = true;

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
	if (isset($transicoes["'programa'"])) {
		$submaquina["nomeFuncao"] = "submaquinaProgramaCriarSubmaquina";
		$submaquina["proximoEstado"] = $transicoes["'programa'"];
		unset($transicoes["'programa'"]);
	}
	
	if (isset($transicoes["'comando'"])) {
		$submaquina["nomeFuncao"] = "submaquinaComandosCriarSubmaquina";
		$submaquina["proximoEstado"] = $transicoes["'comando'"];
		unset($transicoes["'comando'"]);
	}
	
	if (isset($transicoes["'expressao'"])) {
		$submaquina["nomeFuncao"] = "submaquinaExpressoesCriarSubmaquina";
		$submaquina["proximoEstado"] = $transicoes["'expressao'"];
		unset($transicoes["'expressao'"]);
	}
	
	if (isset($transicoes["'S'"])) {
		$submaquina["nomeFuncao"] = "subS";
		$submaquina["proximoEstado"] = $transicoes["'S'"];
		unset($transicoes["'S'"]);
	}
	
	foreach($transicoes as $entradaLida => $estadoDeChegada) {
		if ($primeiroIfTransicao) {
			echo "\tif (entradaLida == {$entradaLida}) {\n\t\tsubmaquinaAtual->estadoAtual = {$estadoDeChegada};\n";
			$primeiroIfTransicao = false;
		} else {
			echo "\telse if (entradaLida == {$entradaLida}) {\n\t\tsubmaquinaAtual->estadoAtual = {$estadoDeChegada};\n";
		}
		
		if (eEstadoFinal($estadoDeChegada))
			echo "\t\t*estaNoEstadoFinal = TRUE;\n";
			
		echo "\t}\n";
	}
	
	if ($submaquina != false) {
		if ($primeiroIfTransicao)
		{
			echo "\tsubmaquinaAtual->estadoAtual = {$submaquina['proximoEstado']};\n";
			echo "\tsubstituirSubmaquinaAtualColocandoAAntigaNaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, {$submaquina['nomeFuncao']}());\n";	
		}
		else {
			echo "\telse {\n\t\tsubmaquinaAtual->estadoAtual = {$submaquina['proximoEstado']};\n";
			echo "\t\tsubstituirSubmaquinaAtualColocandoAAntigaNaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal, {$submaquina['nomeFuncao']}());\n";
			echo "\t}\n";
		}
	}
	
	echo "}\n";
}

echo "else {\n\tchamarSubmaquinaDaPilha(entradaLida, algumaSubmaquinaTransitou, estaNoEstadoFinal);\n}\n";
echo "return;\n";

echo "----------------- ------------- -----------\n";
echo "----------------- ------------- -----------\n";
?>