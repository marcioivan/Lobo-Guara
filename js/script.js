var url = 'wss://io.adafruit.com:443/mqtt/';
var username = 'krautz';
var aio_key  = '1619a03e813b499987b0ebeb3f94e503';
var red_tts = new SpeechSynthesisUtterance();
red_tts.lang = 'pt-BR';
var latido_audio = new Audio("js/som/graw.mp3");
var rosnado_audio = new Audio("js/som/rosnado.mp3");
var mode = 1;
var falando = false;

setTimeout(function() {
}, 500);

var client = mqtt.connect(url, {
	username: username,
	password: aio_key
});

$(document).ready(function(){
	$('#btn-modocrianca').removeClass("active");
	$('#btn-modoadulto').addClass("active");
});

client.on('connect', function() {
	console.log("MQTT Conectado!");
	client.subscribe(username + "/feeds/proximity");
	client.subscribe(username + "/feeds/latido");
	client.subscribe(username + "/feeds/cabeca");
	client.subscribe(username + "/feeds/corpo");
	client.subscribe(username + "/feeds/pata");
	client.subscribe(username + "/feeds/rabo");
	client.subscribe(username + "/feeds/modo");
});

client.on('error', function(e) {
	console.log(e);
});

function sendFeed(feed, valor) {
	client.publish(username + "/feeds/" + feed, valor);
	console.log("Enviado: " + username + "/feeds/" + feed + ": " + valor);
}

client.on('message', function(topic, message) {

	console.log("Recebido: " + topic.toString() + ": " + message.toString());

	if (topic.toString() == username + "/feeds/modo") {
		mode = parseInt(message);
		if(mode == 0){
			$('#btn-modocrianca').addClass("active");
			$('#btn-modoadulto').removeClass("active");
		} else{
			$('#btn-modocrianca').removeClass("active");
			$('#btn-modoadulto').addClass("active");
		}
	}

	if (topic.toString() == username + "/feeds/proximity") {
		var proximity = parseInt(message);
		var element = document.getElementById("lobo");
		if (proximity == 1) {
			element.classList.add("incomodado");
			rosnado_audio.play();
		} else {
			setTimeout(function() {
				rosnado_audio.pause();
				rosnado_audio.currentTime = 0;
				element.classList.remove("incomodado");
			}, 100);
		}
	}

	if (topic.toString() == username + "/feeds/latido") {
		var latido = parseInt(message);
		var element = document.getElementById("lobo");
		if (latido == 1) {
			element.classList.add("latido");
			latido_audio.play();
		} else {
			setTimeout(function() {
				element.classList.remove("latido");
			}, 1000);
		}
	}

	if (topic.toString() == username + "/feeds/cabeca") {
		var cabeca = parseInt(message);
		var element = document.getElementById("lobo");
		if (cabeca == 1 && !falando) {
			element.classList.add("cabeca");
			if (mode == 1) {
				red_tts.text = 'O formato da cabeça se parece com o de uma raposa. O focinho é esguio e as orelhas são grandes. Como os outros canídeos, sua boca tem 42 dentes.';
				speechSynthesis.speak(red_tts);
				displaySubtitle(red_tts.text);
				setTimeout(hideSubtitle, 15000);
			}
			if (mode == 0) {
				red_tts.text = 'Cabeça';
				speechSynthesis.speak(red_tts);
				displaySubtitle(red_tts.text);
				setTimeout(hideSubtitle, 1500);
			}
		} else {
			if (mode == 1) {
				setTimeout(function() {
					element.classList.remove("cabeca");
				}, 15000);
			} else {
				setTimeout(function() {
					element.classList.remove("cabeca");
				}, 1500);
			}

		}
	}

	if (topic.toString() == username + "/feeds/corpo") {
		var corpo = parseInt(message);
		var element = document.getElementById("lobo");
		if (corpo == 1 && !falando) {
			element.classList.add("corpo");
			if (mode == 1) {
				red_tts.text = 'A pelagem do corpo varia do vermelho-dourado ao laranja e os pelos da nuca e patas são pretos. A parte inferior da mandíbula e a ponta da cauda são brancos.';
				speechSynthesis.speak(red_tts);
				displaySubtitle(red_tts.text);
				setTimeout(hideSubtitle, 12000);
			}
			if (mode == 0) {
				red_tts.text = 'Corpo';
				speechSynthesis.speak(red_tts);
				displaySubtitle(red_tts.text);
				setTimeout(hideSubtitle, 1500);
			}
		} else {
			if (mode == 1) {
				setTimeout(function() {
					element.classList.remove("corpo");
				}, 12000);
			} else {
				setTimeout(function() {
					element.classList.remove("corpo");
				}, 1500);
			}
		}
	}

	if (topic.toString() == username + "/feeds/pata") {
		var pata = parseInt(message);
		var element = document.getElementById("lobo");
		if (pata == 1 && !falando) {
			element.classList.add("pata");
			if (mode == 1) {
				red_tts.text = 'As pegadas têm entre 7 e 9 centimetros de comprimento e 5,5 e 7 centimetros de largura.';
				console.log(red_tts);
				speechSynthesis.speak(red_tts);
				displaySubtitle(red_tts.text);
				setTimeout(hideSubtitle, 10000);
			}
			if (mode == 0) {
				red_tts.text = 'Pata';
				speechSynthesis.speak(red_tts);
				displaySubtitle(red_tts.text);
				setTimeout(hideSubtitle, 1500);
			}
		} else {
			if (mode == 1) {
				setTimeout(function() {
					element.classList.remove("pata");
				}, 10000);
			} else {
				setTimeout(function() {
					element.classList.remove("pata");
				}, 1500);
			}
		}
	}

	if (topic.toString() == username + "/feeds/rabo") {
		var rabo = parseInt(message);
		var element = document.getElementById("lobo");
		if (rabo == 1 && !falando) {
			element.classList.add("rabo");
			if (mode == 1) {
				red_tts.text = 'É o maior canídeo da América do Sul, atingindo entre 95 e 115 centimetros de comprimento, com uma cauda medindo entre 38 e 50 centimetros e atingindo até 90 cm de altura.';
				speechSynthesis.speak(red_tts);
				displaySubtitle(red_tts.text);
				setTimeout(hideSubtitle, 20000);
			}
			if (mode == 0) {
				red_tts.text = 'Rabo';
				speechSynthesis.speak(red_tts);
				displaySubtitle(red_tts.text);
				setTimeout(hideSubtitle, 1500);
			}
		} else {
			if (mode == 1) {
				setTimeout(function() {
					element.classList.remove("rabo");
				}, 20000);
			} else {
				setTimeout(function() {
					element.classList.remove("rabo");
				}, 1500);
			}
		}
	}

});
function displaySubtitle(text){
	$(".subtext").html(text);
	$(".subtext").show();
	falando = true;
}

function hideSubtitle(){
	$(".subtext").hide();
	falando = false;
}
